#include "../include/t2fs.h"
#include "../include/bitmap2.h"
#include "../include/t2disk.h"
#include "../include/apidisk.h"
#include "../include/functions_aux.h"
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
int inodeAreaStartSector;
char Path[128];
int rem_blocks_part;
DIRENT2 root;


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

    hasInit = 1;
    return 0;
}

int createSuperblock(int partition, int sectors_per_block){
    SUPERBLOCK newSB;

    float nblocks = (((diskMBR.partition[partition].endAddress - diskMBR.partition[partition].startAddress))+1)/sectors_per_block;

    float bitmapblocks = nblocks/(8*diskMBR.sectorSize*sectors_per_block);
    float bitmapinode = (nblocks/10)/(8*diskMBR.sectorSize*sectors_per_block);

    initializeOpenFiles();

    strcpy(newSB.id,"T2FS");
    newSB.version = diskMBR.diskVersion;
    newSB.superblockSize = 1;

    newSB.freeBlocksBitmapSize = ceil2(bitmapblocks);
    newSB.freeInodeBitmapSize = ceil2(bitmapinode);

    newSB.inodeAreaSize = ceil2((nblocks/10));

    newSB.blockSize = sectors_per_block;
    newSB.diskSize = ceil2(nblocks);

    newSB.Checksum = 0;     //--------------MODIFICAR--------------

    rem_blocks_part = nblocks - (1 + newSB.freeBlocksBitmapSize + newSB.freeInodeBitmapSize + newSB.inodeAreaSize);

    BYTE buffer[SECTOR_SIZE] = {diskMBR.partition[partition].startAddress};
    memcpy(buffer, &newSB, sizeof(SUPERBLOCK));

    printf("Data Blocks - %d\n", rem_blocks_part);

    superblock = newSB;

    inodeAreaStartSector = superblock.superblockSize*superblock.blockSize + superblock.freeBlocksBitmapSize*superblock.blockSize + superblock.freeInodeBitmapSize*superblock.blockSize;
    getInodeFromInodeNumber(0, &inode);

    return write_sector(diskMBR.partition[partition].startAddress, buffer);
}

int getInodeFromInodeNumber(DWORD inodeNumber, INODE *inode){

	int inodeSector = inodeAreaStartSector + inodeNumber/INODE_PER_SECTOR;
	BYTE buffer[SECTOR_SIZE];

	if(getBitmap2(BITMAP_INODE, inodeNumber) == 0){
		return -1;
	}

	if(read_sector(inodeSector, buffer) != 0){
		return -1;
	}

	int inode_byte_start = (inodeNumber % INODE_PER_SECTOR)*INODE_SIZE;

	inode->blocksFileSize = *((DWORD*) (buffer + inode_byte_start + 0));
	inode->bytesFileSize = *((DWORD*) (buffer + inode_byte_start + 4));
	inode->dataPtr[0] = *((DWORD*) (buffer + inode_byte_start + 8));
	inode->dataPtr[1] = *((DWORD*) (buffer + inode_byte_start + 12));
	inode->singleIndPtr = *((DWORD*) (buffer + inode_byte_start + 16));
	inode->doubleIndPtr = *((DWORD*) (buffer + inode_byte_start + 20));
	inode->RefCounter = *((DWORD*) (buffer + inode_byte_start + 24));
	inode->reservado = *((DWORD*) (buffer + inode_byte_start + 28));

    printf("----------------INODE----------------\n");
    printf("%d\n", inode_byte_start);
    printf("%d\n", inodeAreaStartSector);
    printf("%d\n", INODE_SIZE);
    printf("%d\n", INODE_PER_SECTOR);
    printf("%u\n", inode->blocksFileSize);
    printf("%u\n", inode->bytesFileSize);
    printf("%u\n", inode->dataPtr[0]);
    printf("%u\n", inode->dataPtr[1]);
    printf("%u\n", inode->singleIndPtr );
    printf("%u\n", inode->doubleIndPtr);
    printf("%u\n", inode->RefCounter);
    printf("%u\n", inode->reservado);
    printf("----------------INODE----------------\n");

	return 0;
}

void initializeOpenFiles(){
	int i;
	for(i = 0; i < MAX_OPEN_FILES; i++){
		openFiles[i].record.TypeVal = TYPEVAL_INVALIDO;
		openFiles[i].record.inodeNumber = INVALID_PTR;
	}
}

int mountpartition(int partition){
    if(mountpart == NULL)
    {
        *mountpart = diskMBR.partition[partition].startAddress;
        return 0;
    }
    else{
        return -1;
    }
}
