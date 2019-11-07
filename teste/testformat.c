#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "../include/t2fs.h"
#include "../include/functions_aux.h"

int main(int argc, char const *argv[]){
    SUPERBLOCK newSB;
    int part = 0;
    if(format2(part,2) != 0){
        printf("Error at format2\n");
        return -1;
    }
    printf("Particao formatada com sucesso!\n");

    BYTE buffer[SECTOR_SIZE] = {diskMBR.partition[part].startAddress};
    if(read_sector(diskMBR.partition[part].startAddress, buffer) != 0)
        return -1;
    memcpy(&newSB, buffer, sizeof(SUPERBLOCK));

    printf("%s\n", newSB.id);
    printf("%u\n", newSB.version);
    printf("%u\n", newSB.superblockSize);
    printf("%u\n", newSB.freeBlocksBitmapSize);
    printf("%u\n", newSB.freeInodeBitmapSize);
    printf("%u\n", newSB.inodeAreaSize);
    printf("%u\n", newSB.blockSize);
    printf("%u\n", newSB.diskSize );
    printf("%u\n", newSB.Checksum);
    return 0;
}
