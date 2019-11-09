
/**
*/
#include "../include/t2fs.h"
#include "../include/bitmap2.h"
#include "../include/t2disk.h"
#include "../include/apidisk.h"
#include "../include/functions_aux.h"


/*-----------------------------------------------------------------------------
Função:	Informa a identificação dos desenvolvedores do T2FS.
-----------------------------------------------------------------------------*/
int identify2 (char *name, int size) {
    strncpy(name, "Joao C ALmeida da Silva - 00287707\nLucca Strelow Milano - 00287683",size);
	return 0;
}

/*-----------------------------------------------------------------------------
Função:	Formata logicamente uma partição do disco virtual t2fs_disk.dat para o sistema de
		arquivos T2FS definido usando blocos de dados de tamanho
		corresponde a um múltiplo de setores dados por sectors_per_block.
-----------------------------------------------------------------------------*/
int format2(int partition, int sectors_per_block){
	if(diskInit() != 0)
        return -1;
    if(createSuperblock(partition, sectors_per_block) != 0)
        return -2;


    openBitmap2(diskMBR.partition[partition].startAddress);
    setBitmap2(BITMAP_INODE, 0, 1);
    setBitmap2(BITMAP_DADOS, 0, 1);
    closeBitmap2();

    return 0;
}

/*-----------------------------------------------------------------------------
Função:	Monta a partição indicada por "partition" no diretório raiz
-----------------------------------------------------------------------------*/
int mount(int partition) {
    if(hasInit == 0)
        if(diskInit() != 1)
            return -1;
    //printf("MOUNT\n");
	return mountpartition(partition);
}

/*-----------------------------------------------------------------------------
Função:	Desmonta a partição atualmente montada, liberando o ponto de montagem.
-----------------------------------------------------------------------------*/
int unmount(void) {
    if(hasInit != 0)
        if(diskInit() != 1)
            return -1;

    mountpart = 0;

	return 0;
}

/*-----------------------------------------------------------------------------
Função:	Função usada para criar um novo arquivo no disco e abrí-lo,
		sendo, nesse último aspecto, equivalente a função open2.
		No entanto, diferentemente da open2, se filename referenciar um
		arquivo já existente, o mesmo terá seu conteúdo removido e
		assumirá um tamanho de zero bytes.
-----------------------------------------------------------------------------*/
FILE2 create2 (char *filename) {

	INODE dirInode;
    Record record;

    dirInode = getdirInode();

    //printf("CREATING 1\n");

    if(getRecordFromDir(dirInode, filename, &record) == 0){
        printf("Arquivo ja existe\n");
		return -1; //arquivo ja existe
    }

    strcpy(record.name, filename);
	record.TypeVal = TYPEVAL_REGULAR;

    //printf("CREATING 2\n");

    int inodeNum = initNewFileInode();
	if(inodeNum == -1)
		return -1;

    record.inodeNumber = inodeNum;

    //printf("CREATING 3\n");

    if(addRecordOnDir(&dirInode, record) != 0){
		removeAllDataFromInode(inodeNum);

		openBitmap2(mountpart);
		setBitmap2(BITMAP_INODE, inodeNum, 0);
		closeBitmap2();

		return -1;
	}
    //printf("FILE CRIADA COM SUCESSO\n");
    return open2(filename);
}

/*-----------------------------------------------------------------------------
Função:	Função usada para remover (apagar) um arquivo do disco.
-----------------------------------------------------------------------------*/
int delete2 (char *filename) {
	return -1;
}

/*-----------------------------------------------------------------------------
Função:	Função que abre um arquivo existente no disco.
-----------------------------------------------------------------------------*/
FILE2 open2 (char *filename) {

	FILE2 freeHandle = getFreeFileHandle();
	//printf("OPENING\n");
	if(freeHandle == -1){
	    printf("ERRO OPENING 1\n");
		return -1;  // OpenFiles is full
	}
	Record record;

	INODE dirInode;
	dirInode = getdirInode();

	if(getRecordFromDir(dirInode, filename, &record) != 0){
	    printf("ERRO OPENING 2\n");
		return -1;
	}

	if(record.TypeVal == TYPEVAL_REGULAR){
		openFiles[freeHandle].record = record;
		openFiles[freeHandle].currentPointer = 0;
		return freeHandle;
	}
    printf("ERRO OPENING 3\n");
	return -1;
}

/*-----------------------------------------------------------------------------
Função:	Função usada para fechar um arquivo.
-----------------------------------------------------------------------------*/
int close2 (FILE2 handle) {

	if(isFileHandleValid(handle)){
	 	openFiles[handle].record.TypeVal = TYPEVAL_INVALIDO;
		openFiles[handle].record.inodeNumber = INVALID_PTR;
        return 0;
    }

	return -1;
}

/*-----------------------------------------------------------------------------
Função:	Função usada para realizar a leitura de uma certa quantidade
		de bytes (size) de um arquivo.
-----------------------------------------------------------------------------*/
int read2 (FILE2 handle, char *buffer, int size) {
	return -1;
}

/*-----------------------------------------------------------------------------
Função:	Função usada para realizar a escrita de uma certa quantidade
		de bytes (size) de  um arquivo.
-----------------------------------------------------------------------------*/
int write2 (FILE2 handle, char *buffer, int size) {
	return -1;
}

/*-----------------------------------------------------------------------------
Função:	Função que abre um diretório existente no disco.
-----------------------------------------------------------------------------*/
int opendir2 (void) {
	return 0;
}

/*-----------------------------------------------------------------------------
Função:	Função usada para ler as entradas de um diretório.
-----------------------------------------------------------------------------*/
int readdir2 (DIRENT2 *dentry) {
	return -1;
}

/*-----------------------------------------------------------------------------
Função:	Função usada para fechar um diretório.
-----------------------------------------------------------------------------*/
int closedir2 (void) {
    DirRoot.record.TypeVal = TYPEVAL_INVALIDO;
	DirRoot.record.inodeNumber = INVALID_PTR;
	return 0;
}

/*-----------------------------------------------------------------------------
Função:	Função usada para criar um caminho alternativo (softlink)
-----------------------------------------------------------------------------*/
int sln2 (char *linkname, char *filename) {
	return -1;
}

/*-----------------------------------------------------------------------------
Função:	Função usada para criar um caminho alternativo (hardlink)
-----------------------------------------------------------------------------*/
int hln2(char *linkname, char *filename) {
	return -1;
}



