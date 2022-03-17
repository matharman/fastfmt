package main

import (
	"bufio"
	"bytes"
	"encoding/binary"
	"errors"
	"fastfmt/rzcobs"
	"fmt"
	"io"
	"os"
	"strconv"
	"strings"

	"github.com/jacobsa/go-serial/serial"
)

const tty = "/dev/ttyACM0"
const database = "fastfmt_strings_nojson"
const frame_header = "##FASTFMT##"

type FormatSpec struct {
	Filename   string
	LineNumber int
	ArgCount   int
	Spec       string
}

func main() {
	_, err := LoadFormatSpecTable(database)
	if err != nil {
		fmt.Println(err)
		return
	}

	// ConsumeStream(fmtSpec)
}

func LoadFormatSpecTable(filename string) (map[int64]FormatSpec, error) {
	f, err := os.Open(filename)
	if err != nil {
		return nil, fmt.Errorf("error opening format spec %v: %v", filename, err)
	}
	defer f.Close()

	tbl := make(map[int64]FormatSpec)

	for {
		offset, err := f.Seek(0, io.SeekCurrent)
		if err != nil {
			if errors.Is(err, io.EOF) {
				return tbl, nil
			}

			return tbl, fmt.Errorf("error getting current offset: %v", err)
		}

		fmt.Println("offset", offset)

		rdr := bufio.NewReader(f)

		b, err := rdr.Peek(len(frame_header))
		if err != nil {
			if errors.Is(err, io.EOF) {
				return tbl, nil
			}
			return tbl, fmt.Errorf("error reading spec %v", err)
		}

		fmt.Println("Peeked", string(b))
		if bytes.Equal(b, []byte(frame_header)) {
			rdr.Discard(len(b))
		}

		var spec FormatSpec
		b, err = rdr.ReadBytes(byte(';'))
		if err != nil {
			if errors.Is(err, io.EOF) {
				return tbl, nil
			}
			return tbl, fmt.Errorf("error reading spec %v", err)
		}
		fmt.Println("filename seg", string(b[:len(b)-1]))

		spec.Filename = string(b[:len(b)-1])

		b, err = rdr.ReadBytes(byte(';'))
		if err != nil {
			if errors.Is(err, io.EOF) {
				return tbl, nil
			}
			return tbl, fmt.Errorf("error reading spec %v", err)
		}
		fmt.Println("linenumber seg", string(b[:len(b)-1]))

		spec.LineNumber, err = strconv.Atoi(string(b[:len(b)-1]))
		if err != nil {
			return tbl, err
		}

		b, err = rdr.ReadBytes(byte(';'))
		if err != nil {
			if errors.Is(err, io.EOF) {
				return tbl, nil
			}
			return tbl, fmt.Errorf("error reading spec %v", err)
		}
		fmt.Println("argcount seg", string(b[:len(b)-1]))

		spec.ArgCount, err = strconv.Atoi(string(b[:len(b)-1]))
		if err != nil {
			return tbl, err
		}

		b, err = rdr.ReadBytes(0)
		if err != nil {
			if errors.Is(err, io.EOF) {
				return tbl, nil
			}
			return tbl, fmt.Errorf("error reading spec %v", err)
		}

		spec.Spec = string(b[:len(b)-1])
		spec.Spec = strings.ReplaceAll(spec.Spec, "\\\"", "\"")
		fmt.Println("FormatSpec", spec.Spec)
	}
}

func ConsumeStream(tbl map[int64]FormatSpec) {
	fmt.Println("Opening", tty)

	opts := serial.OpenOptions{
		PortName:              tty,
		BaudRate:              115200,
		DataBits:              8,
		StopBits:              1,
		InterCharacterTimeout: 1000,
		MinimumReadSize:       2,
	}

	port, err := serial.Open(opts)
	if err != nil {
		fmt.Printf("error opening port %v: %v", tty, err)
		return
	}

	defer port.Close()

	frames := make(chan []byte)
	go func() {
		for {
			frame := <-frames

			level := frame[0]
			offset := binary.LittleEndian.Uint32(frame[1:5])
			args := frame[5:]

			if spec, ok := tbl[int64(offset)]; ok {
				fmt.Printf("[%v][%v:%v] %v, args [% x]\n", level, spec.Filename, spec.LineNumber, spec.Spec, args)
			}
		}
	}()

	rdr := bufio.NewReader(port)
	for {
		msg, err := rdr.ReadBytes(0)
		if err != nil {
			fmt.Printf("read error: %v\n", err)
			continue
		}

		frame, err := rzcobs.Decode(msg[:len(msg)-1])
		if err != nil {
			fmt.Printf("decode error: %v\n", err)
			continue
		}

		frames <- frame
	}
}
