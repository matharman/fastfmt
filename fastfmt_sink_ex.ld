SECTIONS
{
    .fastfmt (INFO): ALIGN(4)
    {
        __start_fastfmt = .;
        KEEP(*(*.fastfmt.header))
        KEEP(*(*.fastfmt.strings))
        __end_fastfmt = .;
    }
}
INSERT AFTER .debug;
