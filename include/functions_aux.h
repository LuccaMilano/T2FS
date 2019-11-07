#include "../include/bitmap2.h"
#include "../include/t2disk.h"
#include "../include/apidisk.h"

#define	INVALID_PTR	-1
#define SECTOR_SIZE 256
#define DIRECTORY_FT 0x02
#define INODE_SIZE sizeof(INODE)
#define INODE_PER_SECTOR 256/sizeof(INODE)
#define MAX_OPEN_FILES 10

int hasInit;
DWORD *mountpart;

typedef struct t2fs_record Record;
typedef struct t2fs_inode INODE;
typedef struct t2fs_superbloco SUPERBLOCK;

INODE inode;
SUPERBLOCK superblock;

typedef struct t2fs_openfile{
	struct t2fs_record record;
	DWORD currentPointer; // Em bytes a partir do inicio do arquivo!
} OpenFile;
OpenFile openFiles[MAX_OPEN_FILES];

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




int ceil2(float num);
int diskInit();
int createSuperblock(int partition, int sectors_per_block);
int getInodeFromInodeNumber(DWORD inodeNumber, INODE *inoderoot);
int mountpartition(int partition);
void initializeOpenFiles();
