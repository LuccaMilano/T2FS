#include "../include/t2fs.h"
#include "../include/bitmap2.h"
#include "../include/t2disk.h"
#include "../include/apidisk.h"
#include "../include/functions_aux.h"
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
int inodeAreaStartSector;
int dataAreaStartBlock;

char Path[128];
int rem_blocks_part;
DIRENT2 root;

INODE dirINode;

INODE getdirInode(){
    return dirINode;
}


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
    mountpart = 0;
    return 0;
}

int createSuperblock(int partition, int sectors_per_block){
    SUPERBLOCK newSB;

    float nblocks = (((diskMBR.partition[partition].endAddress - diskMBR.partition[partition].startAddress))+1)/sectors_per_block;

    float bitmapblocks = nblocks/(8*diskMBR.sectorSize*sectors_per_block);
    float bitmapinode = (nblocks/10)/(8*diskMBR.sectorSize*sectors_per_block);

    initializeOpenFiles();

    strncpy(newSB.id,"T2FS",4);
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

    return write_sector(diskMBR.partition[partition].startAddress, buffer);
}

int getInodeFromInodeNumber(DWORD inodeNumber, INODE *inode){

	int inodeSector = inodeAreaStartSector + inodeNumber/INODE_PER_SECTOR;
	BYTE buffer[SECTOR_SIZE];
	printf("mountpart 1 - %d\n", mountpart);

    openBitmap2(mountpart);

    printf("First Free Inode: %d\n", searchBitmap2(BITMAP_INODE, 0));

    int test = getBitmap2(BITMAP_INODE, inodeNumber);
    printf("getbitmap - %d\n", test);
	if(test == 0){
		return -1;
	}

    closeBitmap2();

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

	//memcpy(inode, buffer, sizeof(INODE));

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

    //memcpy(buffer, &inode, sizeof(INODE));
    //write_sector(inodeAreaStartSector, buffer);
    //printf("AreaStartSector - %d\n", inodeAreaStartSector);

	return 0;
}

void initializeOpenFiles(){
	int i;
	for(i = 0; i < MAX_OPEN_FILES; i++){
		openFiles[i].record.TypeVal = TYPEVAL_INVALIDO;
		openFiles[i].record.inodeNumber = INVALID_PTR;
	}
}
int initdirINode(){
    dirINode.blocksFileSize = 0;
	dirINode.bytesFileSize = 0;
	dirINode.dataPtr[0] = dataAreaStartBlock;
	dirINode.dataPtr[1] = INVALID_PTR;
	dirINode.singleIndPtr = INVALID_PTR;
	dirINode.doubleIndPtr = INVALID_PTR;
	dirINode.RefCounter = INVALID_PTR;
	dirINode.reservado = INVALID_PTR;

    BYTE buffer[SECTOR_SIZE] = {0};
    memcpy(buffer, &dirINode, sizeof(INODE));

	return write_sector(inodeAreaStartSector, buffer);
}
int mountpartition(int partition){
    if(mountpart == 0)
    {
        mountpart = diskMBR.partition[partition].startAddress;

        BYTE buffer[SECTOR_SIZE] = {0};
        if(read_sector(diskMBR.partition[partition].startAddress, buffer) != 0)
            return -1;

        memcpy(&superblock, buffer, sizeof(SUPERBLOCK));

        dataAreaStartBlock= mountpart + superblock.superblockSize + superblock.freeBlocksBitmapSize + superblock.freeInodeBitmapSize + superblock.inodeAreaSize;
        inodeAreaStartSector = mountpart + superblock.superblockSize*superblock.blockSize + superblock.freeBlocksBitmapSize*superblock.blockSize + superblock.freeInodeBitmapSize*superblock.blockSize;

        printf("DATA start - %d\n", dataAreaStartBlock);
        printf("INODE start - %d\n", inodeAreaStartSector);
/*
        openBitmap2(mountpart);
        printf("First Free Inode: %d\n", searchBitmap2(BITMAP_INODE, 0));
        closeBitmap2();
*/
        if(initdirINode() != 0)
            return -1;

        INODE inode;
        getInodeFromInodeNumber(0, &inode);
        return 0;
    }
    else{
        return -1;
    }
}

int getPointers(DWORD blockNumber, DWORD *pointers){
	unsigned char buffer[SECTOR_SIZE];
	int i, j;

	for(i = 0; i < superblock.blockSize; i++){ // For all sector of block
		int sectorNumber = blockNumber*superblock.blockSize + i;
		read_sector(sectorNumber, buffer);
		for(j = 0; j < PTR_PER_SECTOR; j++){  // For all record of sector
			pointers[j + i*PTR_PER_SECTOR] = *((DWORD*)(buffer + j*PTR_SIZE));
		}
	}
	return 0;
}

int getRecordsFromEntryBlock(DWORD blockNumber, Record *records){
	unsigned char buffer[SECTOR_SIZE];
	int i, j, c;

	for(i = 0; i < superblock.blockSize; i++){ // For all sector of block
		int sectorNumber = blockNumber*BLOCK_SIZE + i;
		read_sector(sectorNumber, buffer);
		for(j = 0; j < RECORD_PER_SECTOR; j++){  // For all record of sector
			records[j + i*RECORD_PER_SECTOR].TypeVal = buffer[j*RECORD_SIZE];
			for(c = 0; c < 59; c++){
				records[j + i*RECORD_PER_SECTOR].name[c] = buffer[1 + c + j*RECORD_SIZE];
			}
			records[j + i*RECORD_PER_SECTOR].inodeNumber = *((DWORD*)(buffer + 60 + j*RECORD_SIZE));
		}
	}
	return 0;
}

int getRecordFromEntryBlock(DWORD blockNumber, char *filename, Record *record){

	Record records[RECORD_PER_SECTOR*BLOCK_SIZE];
	getRecordsFromEntryBlock(blockNumber, records);

	int i;
	for(i = 0; i < RECORD_PER_SECTOR*superblock.blockSize; i++){
		if(records[i].TypeVal != TYPEVAL_INVALIDO && strcmp(records[i].name, filename) == 0){
			*record = records[i];
			return 0;
		}
	}
	return -1; // File not found
}

int getRecordFromDir(INODE dirInode, char *filename, Record *recordOut){
	int i, j;
	// Search on direct pointers
	Record record;
	DWORD pointers[PTR_PER_SECTOR*BLOCK_SIZE];
	DWORD doublePointers[PTR_PER_SECTOR*BLOCK_SIZE];

	for(i = 0; i < 2; i++){
		if(dirInode.dataPtr[i] != INVALID_PTR){
			if(getRecordFromEntryBlock(dirInode.dataPtr[i], filename, &record) == 0){
				*recordOut = record;
				return 0;
			}
		}
	}
	// Search on simple indirection
	if(dirInode.singleIndPtr != INVALID_PTR){
		getPointers(dirInode.singleIndPtr, pointers);
		for(i = 0; i < PTR_PER_SECTOR*BLOCK_SIZE; i++){
			if(pointers[i] != INVALID_PTR){
				if(getRecordFromEntryBlock(pointers[i], filename, &record) == 0){
					*recordOut = record;
					return 0;
				}
			}
		}
	}
	// Search on double indirection
	if(dirInode.doubleIndPtr != INVALID_PTR){
		getPointers(dirInode.doubleIndPtr, doublePointers);
		for(i = 0; i < PTR_PER_SECTOR*BLOCK_SIZE; i++){
			if(doublePointers[i] != INVALID_PTR){
				getPointers(doublePointers[i], pointers);
				for(j = 0; j < PTR_PER_SECTOR*BLOCK_SIZE; j++){
					if(pointers[j] != INVALID_PTR){
						if(getRecordFromEntryBlock(pointers[j], filename, &record) == 0){
							*recordOut = record;
							return 0;
						}
					}
				}
			}
		}
	}

	return -1;
}

void writeDwordOnBuffer(unsigned char *buffer, int start, DWORD dword){
	unsigned char *aux;
	aux = (unsigned char*)&dword;
	int i;
	for(i = 0; i < 4; i++)
		buffer[start + i] = aux[i];
}

int writeInodeOnDisk(INODE inode, int inodeNumber){
	int inodeSector = inodeAreaStartSector + inodeNumber/INODE_PER_SECTOR;
	BYTE buffer[SECTOR_SIZE] = {0};

    if(read_sector(inodeSector, buffer) != 0)
		return -1;

    int inode_byte_start = (inodeNumber % INODE_PER_SECTOR)*INODE_SIZE;
	writeDwordOnBuffer(buffer, inode_byte_start + 0, inode.blocksFileSize);
	writeDwordOnBuffer(buffer, inode_byte_start + 4, inode.bytesFileSize);
	writeDwordOnBuffer(buffer, inode_byte_start + 8, inode.dataPtr[0]);
	writeDwordOnBuffer(buffer, inode_byte_start + 12, inode.dataPtr[1]);
	writeDwordOnBuffer(buffer, inode_byte_start + 16, inode.singleIndPtr);
	writeDwordOnBuffer(buffer, inode_byte_start + 20, inode.doubleIndPtr);
	writeDwordOnBuffer(buffer, inode_byte_start + 24, inode.RefCounter);
	writeDwordOnBuffer(buffer, inode_byte_start + 28, inode.reservado);

	if(write_sector(inodeSector, buffer) != 0)
		return -1;


	return 0;
}

int initNewFileInode(){
    openBitmap2(mountpart);

	DWORD inodeNumber = searchBitmap2(BITMAP_INODE, 0);
	if(inodeNumber <= 0)
		return -1;
	INODE inode;
	inode.blocksFileSize = 1;
	inode.bytesFileSize = 0;
	inode.dataPtr[1] = INVALID_PTR;
	inode.singleIndPtr = INVALID_PTR;
	inode.doubleIndPtr = INVALID_PTR;

	int blockNum = searchBitmap2(BITMAP_DADOS, 0);
	if(blockNum <= 0)
		return -1;
	setBitmap2(BITMAP_DADOS, blockNum, 1);

	inode.dataPtr[0] = blockNum+dataAreaStartBlock;

	setBitmap2(BITMAP_INODE, inodeNumber, 1);

	if(writeInodeOnDisk(inode, inodeNumber) != 0)
		return -1;

    closeBitmap2();
	return inodeNumber;
}

int writeRecordOnDir(DWORD blockNum, Record record, int recordNum){
	unsigned char buffer[SECTOR_SIZE];
	int i;
	int sector = blockNum*BLOCK_SIZE + (recordNum*RECORD_SIZE)/(SECTOR_SIZE);
	int byte_start = (recordNum % RECORD_PER_SECTOR)*RECORD_SIZE;
	if(read_sector(sector, buffer) != 0){
		//printf("Erro leitura do setor %d\n", sector);
		return -1;
	}

	buffer[byte_start] = record.TypeVal;
	for(i = 0; i < 59; i++){
		buffer[1 + i + byte_start] = record.name[i];
	}
	writeDwordOnBuffer(buffer, byte_start + 60, record.inodeNumber);
	if(write_sector(sector, buffer) != 0){
		return -1;
	}

	return 0;
}

int updateDirInode(INODE dirInode){
	Record record;
/*
	if(getRecordFromDir(dirInode, ".", &record) != 0){
		return -1;
	}
*/
	if(writeInodeOnDisk(dirInode, record.inodeNumber) != 0)
		return -1;

	return 0;
}

int addRecordOnDir(INODE *dirInode, Record record){
	int i, j, k;
	Record records[RECORD_PER_SECTOR*BLOCK_SIZE];
	DWORD pointers[PTR_PER_SECTOR*BLOCK_SIZE];
	DWORD doublePointers[PTR_PER_SECTOR*BLOCK_SIZE];

	// Direto 0
	getRecordsFromEntryBlock(dirInode->dataPtr[0], records);
	for(i = 0; i < RECORD_PER_SECTOR*BLOCK_SIZE; i++){
		if(records[i].TypeVal == TYPEVAL_INVALIDO){
			if(writeRecordOnDir(dirInode->dataPtr[0], record, i) == 0){
				return updateDirInode(*dirInode);
			}
		}
	}

	// Direto 1
	if(dirInode->dataPtr[1] == INVALID_PTR){
		dirInode->dataPtr[1] = initNewEntryBlock();
		if(dirInode->dataPtr[1] == -1)
			return -1;
		dirInode->blocksFileSize += 1;
		dirInode->bytesFileSize += SECTOR_SIZE*BLOCK_SIZE;
	}
	getRecordsFromEntryBlock(dirInode->dataPtr[1], records);
	for(i = 0; i < RECORD_PER_SECTOR*BLOCK_SIZE; i++){
		if(records[i].TypeVal == TYPEVAL_INVALIDO){
			if(writeRecordOnDir(dirInode->dataPtr[1], record, i) == 0)
				return updateDirInode(*dirInode);
		}
	}

	// Indireção Simples
	if(dirInode->singleIndPtr == INVALID_PTR){
		dirInode->singleIndPtr = initNewPointerBlock();
		if(dirInode->singleIndPtr == -1)
			return -1;
	}
	getPointers(dirInode->singleIndPtr, pointers);
	for(i = 0; i < PTR_PER_SECTOR*BLOCK_SIZE; i++){
		if(pointers[i] != INVALID_PTR){
			getRecordsFromEntryBlock(pointers[i], records);
			for(j = 0; j < RECORD_PER_SECTOR*BLOCK_SIZE; j++){
				if(records[j].TypeVal == TYPEVAL_INVALIDO){
					if(writeRecordOnDir(pointers[i], record, j) == 0)
						return updateDirInode(*dirInode);
				}
			}
		}
		else{
			pointers[i] = initNewEntryBlock();
			if(pointers[i] == -1)
				return -1;
			dirInode->blocksFileSize += 1;
			dirInode->bytesFileSize += SECTOR_SIZE*BLOCK_SIZE;
			writePointerOnBlock(dirInode->singleIndPtr, pointers[i], i);
			writeRecordOnDir(pointers[i], record, 0);
			return updateDirInode(*dirInode);
		}
	}

	// Indireção Dupla
	if(dirInode->doubleIndPtr == INVALID_PTR){
		dirInode->doubleIndPtr = initNewPointerBlock();
		if(dirInode->doubleIndPtr == -1)
			return -1;
	}
	getPointers(dirInode->doubleIndPtr, doublePointers);
	for(k = 0; k < PTR_PER_SECTOR*BLOCK_SIZE; k++){
		if(doublePointers[k] == INVALID_PTR){
			doublePointers[k] = initNewPointerBlock();
			if(doublePointers[k] == -1)
				return -1;
			writePointerOnBlock(dirInode->doubleIndPtr, doublePointers[k], k);
		}
		getPointers(doublePointers[k], pointers);
		for(i = 0; i < PTR_PER_SECTOR*BLOCK_SIZE; i++){
			if(pointers[i] != INVALID_PTR){
				getRecordsFromEntryBlock(pointers[i], records);
				for(j = 0; j < RECORD_PER_SECTOR*BLOCK_SIZE; j++){
					if(records[j].TypeVal == TYPEVAL_INVALIDO){
						if(writeRecordOnDir(pointers[i], record, j) == 0)
							return updateDirInode(*dirInode);
					}
				}
			}
			else{
				pointers[i] = initNewEntryBlock();
				if(pointers[i] == -1)
					return -1;
				dirInode->blocksFileSize += 1;
				dirInode->bytesFileSize += SECTOR_SIZE*BLOCK_SIZE;
				writePointerOnBlock(doublePointers[k], pointers[i], i);
				writeRecordOnDir(pointers[i], record, 0);
				return updateDirInode(*dirInode);
			}
		}
	}

	return -1;
}

int initNewEntryBlock(){
	Record emptyRecord;
	int i;
	emptyRecord.TypeVal = TYPEVAL_INVALIDO;
	emptyRecord.inodeNumber = INVALID_PTR;

	openBitmap2(mountpart);

	int blockNum = searchBitmap2(BITMAP_DADOS, 0);
	if(blockNum <= 0)
		return -1;
	for(i = 0; i < RECORD_PER_SECTOR*BLOCK_SIZE; i++){
		if(writeRecordOnDir(blockNum + dataAreaStartBlock, emptyRecord, i) != 0)
			return -1;
	}
	setBitmap2(BITMAP_DADOS, blockNum, 1);

	closeBitmap2();

	return blockNum;
}

int initNewPointerBlock(){
	DWORD pointer = INVALID_PTR;
	int i;
	unsigned char buffer[SECTOR_SIZE];
	for(i = 0; i < PTR_PER_SECTOR; i++){
		writeDwordOnBuffer(buffer, i*PTR_SIZE, pointer);
	}

	openBitmap2(mountpart);

	int blockNum = searchBitmap2(BITMAP_DADOS, 0);
	if(blockNum <= 0)
		return -1;
	for(i = 0; i < BLOCK_SIZE; i++){
		if(write_sector(blockNum*BLOCK_SIZE + i + dataAreaStartBlock*BLOCK_SIZE, buffer) != 0)
			return -1;
	}
	setBitmap2(BITMAP_DADOS, blockNum, 1);

	closeBitmap2();

	return blockNum;
}

int writePointerOnBlock(DWORD blockNum, DWORD pointer, int index){
	unsigned char buffer[SECTOR_SIZE];
	int sector = blockNum*BLOCK_SIZE + (index*PTR_SIZE)/(SECTOR_SIZE);
	int byte_start = (index % PTR_PER_SECTOR)*PTR_SIZE;

	if(read_sector(sector, buffer) != 0){
		//printf("Erro leitura do setor %d\n", sector);
		return -1;
	}

	writeDwordOnBuffer(buffer, byte_start, pointer);
	if(write_sector(sector, buffer) != 0){
		return -1;
	}

	return 0;
}

void removeAllDataFromInode(int inodeNumber){
	INODE inode;
	DWORD pointers[PTR_PER_SECTOR*BLOCK_SIZE];
	DWORD doublePointers[PTR_PER_SECTOR*BLOCK_SIZE];
	int i, j;

	getInodeFromInodeNumber(inodeNumber, &inode);

    openBitmap2(mountpart);

	// Libera blocos diretos
	for(i = 0; i < 2; i++){
		if(inode.dataPtr[i] != INVALID_PTR){
			setBitmap2(BITMAP_DADOS, inode.dataPtr[i], 0);
		}
	}

 	// Libera indireção simples
	if(inode.singleIndPtr != INVALID_PTR){
		getPointers(inode.singleIndPtr, pointers);
		for(i = 0; i < PTR_PER_SECTOR*BLOCK_SIZE; i++){
			if(pointers[i] != INVALID_PTR){
				setBitmap2(BITMAP_DADOS, pointers[i], 0);
			}
		}
		setBitmap2(BITMAP_DADOS, inode.singleIndPtr, 0);
	}

	// Libera indireção dupla
	if(inode.doubleIndPtr != INVALID_PTR){
		getPointers(inode.doubleIndPtr, doublePointers);
		for(i = 0; i < PTR_PER_SECTOR*BLOCK_SIZE; i++){
			if(doublePointers[i] != INVALID_PTR){
				getPointers(doublePointers[i], pointers);
				for(j = 0; j < PTR_PER_SECTOR*BLOCK_SIZE; j++){
					if(pointers[j] != INVALID_PTR){
						setBitmap2(BITMAP_DADOS, pointers[j], 0);
					}
				}
				setBitmap2(BITMAP_DADOS, doublePointers[i], 0);
			}
		}
		setBitmap2(BITMAP_DADOS, inode.doubleIndPtr, 0);
	}
	closeBitmap2();
}
