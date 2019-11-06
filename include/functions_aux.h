#include "../include/bitmap2.h"
#include "../include/t2disk.h"
#include "../include/apidisk.h"

#define SECTOR_SIZE 256
#define DIRECTORY_FT 0x02

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

typedef struct {
	char    id[4];					/** "T2FS" */
	WORD    version;				/** 0x7E32 */
	WORD    superblockSize;			/** 1 = Número de blocos ocupados pelo superbloco */
	WORD	freeBlocksBitmapSize;	/** Número de blocos do bitmap de blocos de dados */
	WORD	freeInodeBitmapSize;	/** Número de blocos do bitmap de i-nodes */
	WORD	inodeAreaSize;			/** Número de blocos reservados para os i-nodes */
	WORD	blockSize;				/** Número de setores que formam um bloco */
	DWORD	diskSize;				/** Número total de blocos da partição */
	DWORD	Checksum;				/** Soma dos 5 primeiros inteiros de 32 bits do superbloco */
} SUPERBLOCK;
SUPERBLOCK superblock;

int diskInit();
int createSuperblock(int partition, int sectors_per_block);
int initFS(int partition);
