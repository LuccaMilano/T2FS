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


/*
    openBitmap2(diskMBR.partition[part].startAddress);
    int aux2 = searchBitmap2(1,0);
    printf("Bitmap2 index- %d\n",aux2);
    setBitmap2 (1, 0, 1);
    printf("Novo valor - %d\n", getBitmap2 (1, 0));
    aux2 = searchBitmap2(1,0);
    printf("Bitmap2 index - %d\n",aux2);
    setBitmap2 (1, 1, 1);
    setBitmap2 (1, 2, 1);
    aux2 = searchBitmap2(1,1);
    printf("Bitmap2 index 1- %d\n",aux2);
    aux2 = searchBitmap2(1,0);
    printf("Bitmap2 index 0 - %d\n",aux2);
*/
    char dir[] = "file3";
    int testcreate = create2(dir);
    printf("Return do Create2 - %d\n", testcreate);

    testcreate = create2("file5");
    printf("Return do Create2 - %d\n", testcreate);

    return 0;
}
