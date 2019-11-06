#include "../include/t2fs.h"
#include "../include/bitmap2.h"
#include "../include/t2disk.h"
#include "../include/apidisk.h"
#include "../include/functions_aux.h"
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

int hasInit = -1;
char Path[128];


int ceil2(float num) {
    int inum = (int)num;
    if (num == (float)inum) {
        return inum;
    }
    return inum + 1;
}

int diskInit(){
    BYTE buffer[SECTOR_SIZE] = {0};

    if(read_sector(0, buffer) != 0)
        return -1;

    memcpy(&diskMBR, buffer, sizeof(MBR));

    return 0;
}

int createSuperblock(int partition, int sectors_per_block){
    SUPERBLOCK newSB;
    float nblocks = (((diskMBR.partition[partition].endAddress - diskMBR.partition[partition].startAddress))+1)/sectors_per_block;
    float bitmapblocks = nblocks/(SECTOR_SIZE*sectors_per_block);  // Talvez tenha que dividir por 8
    float bitmapinode = (nblocks/10)/(SECTOR_SIZE*sectors_per_block);

    strcpy(newSB.id,"T2FS");
    newSB.version = diskMBR.diskVersion;
    newSB.superblockSize = 1;

    newSB.freeBlocksBitmapSize = ceil2(bitmapblocks);
    newSB.freeInodeBitmapSize = ceil2(bitmapinode);

    newSB.inodeAreaSize = ceil2((nblocks/10));
    newSB.blockSize = sectors_per_block;
    newSB.diskSize = ceil2(nblocks);

    newSB.Checksum = 0;     //--------------MODIFICAR--------------

    BYTE buffer[SECTOR_SIZE] = {diskMBR.partition[partition].startAddress};
    memcpy(buffer, &newSB, sizeof(SUPERBLOCK));

    return write_sector(diskMBR.partition[partition].startAddress, buffer);
}

/*
int createRootDir(){
    DIRENT2 *this = malloc(sizeof(DIRENT2));

    strcpy((this.name, "/"));
    this->fileType = DIRECTORY_FT;
    this->fileSize = sizeof(DIRENT2);

    return 0;
}
*/

int initFS(int partition){
    diskInit();

    BYTE buffer[SECTOR_SIZE] = {diskMBR.partition[partition].startAddress};

    if(read_sector(diskMBR.partition[partition].startAddress, buffer) != 0)
        return -1;

    memcpy(&superblock, buffer, sizeof(SUPERBLOCK));

    //strcpy(Path, "/")

    hasInit = 1;
    return 0;
}
