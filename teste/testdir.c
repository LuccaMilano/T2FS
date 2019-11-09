#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "../include/t2fs.h"
#include "../include/bitmap2.h"
#include "../include/functions_aux.h"

int main(int argc, char const *argv[])
{
    SUPERBLOCK newSB;
    int part = 0;
    if(format2(part,1) != 0){
        printf("Error at format2\n");
        return -1;
    }
    printf("Particao formatada com sucesso!\n");
    int aux = mount(part);
    printf("%d\n", mountpart);
    printf("%d\n", aux);


	char dir[] = "/";
	int handle= -1;

	handle = opendir2();

	if (handle >= 0){
		printf("O diretório %s foi aberto e seu handle é %d.\n", dir, handle);
	}

	printf("Conteúdo do diretório %s de handle %d:\n", dir, handle);

	if (closedir2(0) == 0){
		printf("O diretório %s de handle %d foi fechado.\n", dir, handle);
	}

	return 0;
}
