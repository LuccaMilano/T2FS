#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "../include/t2fs.h"
#include "../include/functions_aux.h"

int main(int argc, char const *argv[]){
    SUPERBLOCK newSB;
    INODE inode1;
    int part = 0;
    if(format2(part,2) != 0){
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

    printf("%c%c%c%c\n", newSB.id[0],newSB.id[1],newSB.id[2],newSB.id[3]);
    printf("%u\n", newSB.version);
    printf("%u\n", newSB.superblockSize);
    printf("%u\n", newSB.freeBlocksBitmapSize);
    printf("%u\n", newSB.freeInodeBitmapSize);
    printf("%u\n", newSB.inodeAreaSize);
    printf("%u\n", newSB.blockSize);
    printf("%u\n", newSB.diskSize );
    printf("%u\n", newSB.Checksum);

    int inodeAreaStartSector1 = mountpart + newSB.superblockSize*newSB.blockSize + newSB.freeBlocksBitmapSize*newSB.blockSize + newSB.freeInodeBitmapSize*newSB.blockSize;
    printf("AreaStartSector - %d\n", inodeAreaStartSector1);
    if(read_sector(inodeAreaStartSector1, buffer) != 0)
        return -1;

    memcpy(&inode1, buffer, sizeof(INODE));
    printf("%u\n", inode1.blocksFileSize);
	printf("%u\n",inode1.bytesFileSize);
	printf("%u\n",inode1.dataPtr[0]);
	printf("%u\n",inode1.dataPtr[1]);
	printf("%u\n",inode1.singleIndPtr);
	printf("%u\n",inode1.doubleIndPtr);
	printf("%u\n",inode1.RefCounter);
	printf("%u\n",inode1.reservado);


    write_sector(inodeAreaStartSector1, buffer);
    printf("AreaStartSector - %d\n", inodeAreaStartSector1);
    if(read_sector(inodeAreaStartSector1, buffer) != 0)
        return -1;
    memcpy(&inode1, buffer, sizeof(INODE));
    printf("%u\n", inode1.blocksFileSize);
	printf("%u\n",inode1.bytesFileSize);
	printf("%u\n",inode1.dataPtr[0]);
	printf("%u\n",inode1.dataPtr[1]);
	printf("%u\n",inode1.singleIndPtr);
	printf("%u\n",inode1.doubleIndPtr);
	printf("%u\n",inode1.RefCounter);
	printf("%u\n",inode1.reservado);


/*
    BYTE buffer1[SECTOR_SIZE] = {inodeAreaStartSector1};
    printf("AreaStartSector - %d\n", inodeAreaStartSector1);
    if(read_sector(inodeAreaStartSector1, buffer1) != 0)
        return -1;

    memcpy(&inode1, buffer, sizeof(INODE));
    printf("%u\n",inode1.blocksFileSize);
	printf("%u\n",inode1.bytesFileSize);
	printf("%u\n",inode1.dataPtr[0]);
	printf("%u\n",inode1.dataPtr[1]);
	printf("%u\n",inode1.singleIndPtr);
	printf("%u\n",inode1.doubleIndPtr);
	printf("%u\n",inode1.RefCounter);
	printf("%u\n",inode1.reservado);

	getInodeFromInodeNumber(0, &inode1);
    getInodeFromInodeNumber(0, &inode1);
    getInodeFromInodeNumber(0, &inode1);
    getInodeFromInodeNumber(0, &inode1);
*/
    return 0;
}
