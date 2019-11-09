#include "../include/bitmap2.h"
#include "../include/t2disk.h"
#include "../include/apidisk.h"
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#define	INVALID_PTR	-1

#define SECTOR_SIZE 256
#define DIRECTORY_FT 0x02
#define INODE_SIZE sizeof(INODE)
#define INODE_PER_SECTOR 256/sizeof(INODE)
#define MAX_OPEN_FILES 10

#define PTR_SIZE 4
#define PTR_PER_SECTOR 64
#define BLOCK_SIZE superblock.blockSize

#define RECORD_SIZE sizeof(Record)
#define RECORD_PER_SECTOR 256/sizeof(Record)

int hasInit;
DWORD mountpart;

typedef struct t2fs_record Record;
typedef struct t2fs_inode INODE;
typedef struct t2fs_superbloco SUPERBLOCK;

//INODE inode;
SUPERBLOCK superblock;

typedef struct t2fs_openfile{
	struct t2fs_record record;
	DWORD currentPointer; // Em bytes a partir do inicio do arquivo!
} OpenFile;
OpenFile openFiles[MAX_OPEN_FILES];

OpenFile DirRoot;

typedef struct {
    DWORD startAddress;
    DWORD endAddress;
    BYTE name[24];
} PARTTE;

typedef struct {
    WORD diskVersion;
    WORD sectorSize;
    WORD partitionTableStart;
    WORD numberPartitions;
    PARTTE partition[4];
} MBR;
MBR diskMBR;



INODE getdirInode();
int ceil2(float num);
int diskInit();
int createSuperblock(int partition, int sectors_per_block);
int getInodeFromInodeNumber(DWORD inodeNumber, INODE *inoderoot);
int mountpartition(int partition);
void initializeOpenFiles();

int initdirINode();
int getPointers(DWORD blockNumber, DWORD *pointers);

int getRecordsFromEntryBlock(DWORD blockNumber, Record *records);

int getRecordFromEntryBlock(DWORD blockNumber, char *filename, Record *record);

int getRecordFromDir(INODE dirInode, char *filename, Record *recordOut);

int initNewFileInode();

void writeDwordOnBuffer(unsigned char *buffer, int start, DWORD dword);

int writeInodeOnDisk(INODE inode, int inodeNumber);

int writeRecordOnDir(DWORD blockNum, Record record, int recordNum);

int updateDirInode(INODE dirInode);

int addRecordOnDir(INODE *dirInode, Record record);

int initNewEntryBlock();

int initNewPointerBlock();

int writePointerOnBlock(DWORD blockNum, DWORD pointer, int index);

void removeAllDataFromInode(int inodeNumber);

FILE2 getFreeFileHandle();

int isFileHandleValid(FILE2 handle);
