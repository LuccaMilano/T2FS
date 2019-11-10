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
    printf("Return do Create2 1(Novo)- %d\n", testcreate);

    testcreate = create2("file5");
    printf("Return do Create2 2(Novo)- %d\n", testcreate);

    testcreate = create2("file5");
    printf("Return do Create2 3(Existe)- %d\n", testcreate);

    testcreate = create2("file5");
    printf("Return do Create2 4(Existe)- %d\n", testcreate);

    testcreate = create2(dir);
    printf("Return do Create2 8(Existe)- %d\n", testcreate);

    testcreate = create2("file6");
    printf("Return do Create2 5(Novo)- %d\n", testcreate);

    testcreate = create2("file7");
    printf("Return do Create2 6(Novo)- %d\n", testcreate);

    testcreate = create2("file5");
    printf("Return do Create2 7(Existe)- %d\n", testcreate);



    return 0;
}
