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
	WORD    superblockSize;			/** 1 = N�mero de blocos ocupados pelo superbloco */
	WORD	freeBlocksBitmapSize;	/** N�mero de blocos do bitmap de blocos de dados */
	WORD	freeInodeBitmapSize;	/** N�mero de blocos do bitmap de i-nodes */
	WORD	inodeAreaSize;			/** N�mero de blocos reservados para os i-nodes */
	WORD	blockSize;				/** N�mero de setores que formam um bloco */
	DWORD	diskSize;				/** N�mero total de blocos da parti��o */
	DWORD	Checksum;				/** Soma dos 5 primeiros inteiros de 32 bits do superbloco */
} SUPERBLOCK;
SUPERBLOCK superblock;

int diskInit();
int createSuperblock(int partition, int sectors_per_block);
int initFS(int partition);
