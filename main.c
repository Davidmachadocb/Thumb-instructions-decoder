#include<stdio.h>
#include<stdlib.h>
#include"map.h"
#include"Decode.h"

int menu(){
    int i;
	printf("----------MENU----------\n");
	printf("|1 - EXECUTAR          |\n");
	printf("|2 - EXIT              |\n");
    printf("------------------------\n");
	printf("Choose: "); scanf("%d",&i);
	printf("\n");
	return i;
}

int main(){
    int escolha, number;
    FILE *ptr_file, *ptr_file2;
    char line[10];

    do{
        escolha = menu();
        switch(escolha){
            case(1):

                ptr_file = fopen("input.txt", "r");
                ptr_file2 = fopen("output.txt", "w");

                if(ptr_file == NULL || ptr_file2 == NULL){
                    printf("Error!");
                    return 1;
                }

                while(fgets(line, 10, ptr_file) != NULL){
                    number = strtol (line,NULL,16);
                    mapFunction(number, ptr_file2);
                }

                fclose(ptr_file);
                fclose(ptr_file2);
                
                printf("Programa executado com sucesso e finalizado!\n");
                return 0;

            break;

            case(2):
                printf("Acabou o programa\n");
                return 0;
            break;

            default:
                system("cls");
                printf("escolha nao valida!!! escolha novamente\n");
            break;
        }
    }while(escolha != 2);
    return 0;
}
