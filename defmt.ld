SECTIONS
{
    .defmt (INFO):
    {
        __start_defmt = .;
        KEEP(*(*.defmt.header))
        KEEP(*(*.defmt.strings))
        __end_defmt = .;
    }
}
INSERT AFTER .debug;
