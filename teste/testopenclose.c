#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "../include/t2fs.h"
#include "../include/bitmap2.h"
#include "../include/functions_aux.h"

int main(int argc, char const *argv[]){
    SUPERBLOCK newSB;
    int part = 0;
    if(format2(part,2) != 0){
        printf("Error at format2\n");
        return -1;
    }
    printf("Particao formatada com sucesso!\n");



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

    char file[] = "file";

    int testcreate = create2(file);
    printf("Return de Create2 do arquivo %s- %d\n", file, testcreate);

    int testclose = close2(testcreate);
    printf("Return de Close2 do arquivo %s com handle %d - %d\n", file, testcreate, testclose);

    int handle = open2(file);
    printf("Return de Open2 do arquivo %s - %d\n", file, handle);

    char file2[] = "file2";

    testcreate = create2(file2);
    printf("Return de Create2 do arquivo %s- %d\n", file2, testcreate);

    testclose = close2(testcreate);
    printf("Return de Close2 do arquivo %s com handle %d - %d\n", file2, testcreate, testclose);

    handle = open2(file2);
    printf("Return de Open2 do arquivo %s - %d\n", file2, handle);

    int handle2 = open2(file);
    printf("Return de Open2 do arquivo %s - %d\n", file, handle2);

    testclose = close2(handle);
    printf("Return de Close2 do arquivo %s com handle %d - %d\n", file2, handle, testclose);

    handle2 = 0;
    testclose = close2(handle2);
    printf("Return de Close2 do arquivo %s com handle %d - %d\n", file, handle2, testclose);

    return 0;
}

