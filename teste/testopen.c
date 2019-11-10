#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "../include/t2fs.h"
#include "../include/bitmap2.h"
#include "../include/functions_aux.h"

int main(int argc, char const *argv[]){
    SUPERBLOCK newSB;
    int part = 0;
    if(format2(part,1) != 0){
        printf("Erro na format2\n");
        return -1;
    }
    printf("Format2 feito\n");



    int aux = mount(part);
    printf("%d\n", mountpart);
    printf("%d\n", aux);

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

    char dir[] = "file3";
    int testcreate = create2(dir);
    printf("Return de Create2 do arquivo %s- %d\n", dir, testcreate);

    testcreate = create2(dir);
    printf("Return de Create2 do arquivo %s- %d\n", dir, testcreate);

    int handle = open2(dir);
    printf("Return de Open2 do arquivo %s - %d\n", dir, handle);

    char dir2[] = "file1";
    handle = open2(dir2);
    printf("Return de Open2 do arquivo %s - %d\n", dir2, handle);

    testcreate = create2(dir2);
    printf("Return de Create2 do arquivo %s- %d\n", dir2, testcreate);

    handle = open2(dir2);
    printf("Return de Open2 do arquivo %s - %d\n", dir2, handle);

    return 0;
}
