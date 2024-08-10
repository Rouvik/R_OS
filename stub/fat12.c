#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>

typedef struct BootRecord
{
    // BIOS parameter block
    uint8_t instr[3];
    uint8_t oem_identifier[8];
    uint16_t bytes_per_sector;
    uint8_t sectors_per_cluster;
    uint16_t reserved_sectors;
    uint8_t num_fats;
    uint16_t num_roots;
    uint16_t total_sectors;
    uint8_t media_descriptor;
    uint16_t sectors_per_fat;
    uint16_t sectors_per_track;
    uint16_t heads;
    uint32_t hidden_sectors;
    uint32_t large_sector_count;

    // extended boot record
    uint8_t drive_number;
    uint8_t winNT_flags;
    uint8_t signature;
    uint8_t volume_id[4];
    uint8_t volume_label[11];
    uint8_t system_idetifier[8];
} __attribute((packed)) BootRecord_t;

/**
 * @brief Reads a disk file to boot record structure
 * 
 * @param disk The disk to read boot record from
 * @param record The record structure to read to
 * @return int 1 if success, 0 if error
 */
int readBootSector(FILE *disk, BootRecord_t *record)
{
    return fread(record, sizeof(BootRecord_t), 1, disk);
}

void printBootRecord(BootRecord_t *record)
{
    char oem[9];
    memcpy(oem, record->oem_identifier, 8);
    oem[8] = 0;

    char label[12];
    memcpy(label, record->volume_label, 11);
    label[11] = 0;

    char sysIden[9];
    memcpy(sysIden, record->system_idetifier, 8);
    sysIden[8] = 0;

    printf("OEM: %s\n"
                  "Bytes per sector: %d\n"
                  "Sectors per cluster: %d\n"
                  "Reserved sectors: %d\n"
                  "Number of FAT\'s: %d\n"
                  "Number of root entries: %d\n"
                  "Total sectors: %d\n"
                  "Media descriptor: %02x\n"
                  "Sectors per FAT: %d\n"
                  "Sectors per track: %d\n"
                  "Number of heads: %d\n"
                  "Number of hidden sectors: %d\n"
                  "Large sector count: %d\n"
                  "\nExtended boot record:\n"
                  "Drive number: %02x\n"
                  "Windows NT flags: %d\n"
                  "Signature: %02x\n"
                  "Serial Number: %02x-%02x-%02x-%02x\n"
                  "Label: %s\n"
                  "System identifier: %s\n",
                  oem,
                  record->bytes_per_sector,
                  record->sectors_per_cluster,
                  record->reserved_sectors,
                  record->num_fats,
                  record->num_roots,
                  record->total_sectors,
                  record->media_descriptor,
                  record->sectors_per_fat,
                  record->sectors_per_track,
                  record->heads,
                  record->hidden_sectors,
                  record->large_sector_count,
                  record->drive_number,
                  record->winNT_flags,
                  record->signature,
                  record->volume_id[0], record->volume_id[1], record->volume_id[2], record->volume_id[3],
                  label,
                  sysIden);
}

int main(int argc, const char *argv[])
{
    if (argc < 2)
    {
        printf("Syntax: %s <disk>", argv[0]);
        return 0;
    }
    
    FILE *disk = fopen(argv[1], "r");
    if (!disk)
    {
        fprintf(stderr, "Failed to open disk: %s\n", argv[1]);
        return 1;
    }
    
    BootRecord_t bootRecord;
    if (!readBootSector(disk, &bootRecord))
    {
        fputs("Failed to read boot record from disk", stderr);
        fclose(disk);
        return 1;
    }

    printBootRecord(&bootRecord);
    
    fclose(disk);
    return 0;
}