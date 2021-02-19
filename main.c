
#include <stdio.h>
#include <stdlib.h>
#include "Decode.h"

int main(){

    int number;

    FILE *ptr_file, *ptr_file2;

    ptr_file = fopen("teste.txt", "r");
    ptr_file2 = fopen("teste_copy.txt", "w");

    if(ptr_file == NULL || ptr_file2 == NULL){
        printf("Error!");
        return 1;
    }

    while(fgets(line, 10, ptr_file) != NULL){
        number = strtol (line,NULL,16);
        Decode7(number, ptr_file2);
    }

    fclose(ptr_file);
    fclose(ptr_file2);


    return 0;
}
