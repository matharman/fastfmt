SECTIONS
{
    /* For embedded targets, put the start address after the end of the real address space. Then it won't allocate :)
     * eg: .fastfmt 0xFFFFFFFFF (INFO): */
    .fastfmt (INFO):
    {
        __start_fastfmt = .;
        KEEP(*(*.fastfmt.header))
        KEEP(*(*.fastfmt.strings))
        __end_fastfmt = .;
    }
}
INSERT AFTER .debug;

/* INSERT AFTER trick is used to include ld's default linker script as well.
 * I don't want to maintain an amd64 linker script, do you?
 */
