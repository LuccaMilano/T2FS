#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "../include/t2fs.h"

int main(){
    char *name = malloc(100*sizeof(char));
    identify2(name,100);

    printf("%s\n", name);

return 0;
}
