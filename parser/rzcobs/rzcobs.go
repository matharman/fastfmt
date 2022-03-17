// Package rzcobs provides a decoder for rzcobs streams
package rzcobs

import (
	"bytes"
	"errors"
	"fmt"
	"io"
)

type Error struct {
	message string
}

func (e *Error) Error() string {
	return e.message
}

var MalformedStream = Error{
	"malformed stream",
}

var WriterExhausted = Error{
	"writer exhausted",
}

var EndOfStream = Error{
	"end of stream",
}

func reverseBytes(s []byte) {
	for i, j := 0, len(s)-1; i < j; i, j = i+1, j-1 {
		s[i], s[j] = s[j], s[i]
	}
}

func Decode(data []byte) ([]byte, error) {
	if data == nil {
		return nil, errors.New("nil rzcobs data")
	}

	reverseBytes(data)
	reversed := bytes.NewReader(data)

	writer := bytes.NewBuffer(make([]byte, reversed.Len()))

	writeNextByte := func(r *bytes.Reader, w *bytes.Buffer) error {
		next, err := r.ReadByte()
		if err != nil {
			if errors.Is(err, io.EOF) {
				err = &EndOfStream
			}

			return fmt.Errorf("lookahead: %v", err)
		}

		if err = w.WriteByte(next); err != nil {
			return &WriterExhausted
		}

		return nil
	}

	for reversed.Len() > 0 {
		b, err := reversed.ReadByte()
		if err != nil {
			return nil, &EndOfStream
		}

		switch {
		case b == 0:
			return nil, &MalformedStream
		case b >= 0x01 && b <= 0x7F:
			for i := 0; i < 7; i++ {
				if b&(1<<(6-i)) == 0 {
					if err = writeNextByte(reversed, writer); err != nil {
						return nil, fmt.Errorf("expanding zeroes: %v", err)
					}
				} else {
					if err = writer.WriteByte(0); err != nil {
						return nil, err
					}
				}
			}
		case b >= 0x80 && b <= 0xFE:
			n := (b & 0x7F) + 7
			if err = writer.WriteByte(0); err != nil {
				return nil, &WriterExhausted
			}
			for i := 0; i < int(n); i++ {
				if err = writeNextByte(reversed, writer); err != nil {
					return nil, fmt.Errorf("output short sequence: %v", err)
				}
			}
		case b == 0xFF:
			for i := 0; i < 134; i++ {
				if err = writeNextByte(reversed, writer); err != nil {
					return nil, fmt.Errorf("output long sequence: %v", err)
				}
			}
		}
	}

	result := writer.Bytes()
	reverseBytes(result)

	return result, nil
}
