# Process:
- The bootbase readies the stack with sp = 0x7C00 and segments at 0x00 and prints some info
- Then proceeds to load the second stage bootsetup.bin from disk at the end of 512 bytes at 0x7E00
- This is accomplished by loading 1 sector from sector 2 and reading the `Boot table` from there
- Then getting info about the other sectors used by bootsetup.bin it loads those sectors too at 0x7E00 + 0x200 = 0x8000
- The bootloader also stores valuable information at location 0x7C03
in a table format described below `Boot Info Table`

# Boot table:
|Offset|Size  |Name            |Description|
|:----:|------|:--------------:|-----------|
|0x00  |1 byte|reserved_sectors|Stores the number or reserved sectors containing the bootsetup.bin|

`* future additions to the table might be considered`

# Boot Info table:
|Offset|Size  |Name        |Description|
|:----:|------|:----------:|-----------|
|0x00  |1 byte|drive_number|Stores the drive number in dl set by BIOS

`* future additions to the table might be considered`