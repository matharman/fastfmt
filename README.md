# fastfmt
Goofing around with non-allocating format string linker sections.

The idea is simple: 
- Reduce code size and CPU overhead of logging without compromising on printf capabilities. 
- Let a host side computer do the work of processing the data for the human. My workstation is fast; the target is not.

### Inspired By:
- [defmt](https://github.com/knurling-rs/defmt)
    - Credit where credit is due: I transcribed the rzCOBS encoding used by defmt into C for this project because it's awesome.
- [memfault compact logs](https://docs.memfault.com/docs/mcu/compact-logs/)
- [TRICE](https://github.com/rokath/trice)
