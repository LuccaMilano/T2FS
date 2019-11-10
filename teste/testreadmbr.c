#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "../include/t2fs.h"
#include "../include/functions_aux.h"

int main(int argc, char const *argv[]){
    if(diskInit() != 0){
        printf("Erro na leitura do bloco MBR\n");
        return -1;
    }
    printf("Versao do disco: %d\n", diskMBR.diskVersion);
    printf("Tamanho do setor: %d\n", diskMBR.sectorSize);
    printf("Comeco da particao: %d\n", diskMBR.partitionTableStart);
    printf("Numero de particoes: %d\n\n", diskMBR.numberPartitions);

    int i;
    for(i=0; i<4; i++){
        printf("Particao %d nome: %s\n", i, diskMBR.partition[i].name);
        printf("Particao %d tamanho: %u\n", i, (((diskMBR.partition[i].endAddress - diskMBR.partition[i].startAddress))+1));
        printf("Particao %d primeiro setor: %u\n\n", i, diskMBR.partition[i].startAddress);
        printf("Particao %d ultimo setor: %u\n\n", i, diskMBR.partition[i].endAddress);
    }
return 0;
}

