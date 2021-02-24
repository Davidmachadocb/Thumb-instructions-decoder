#include<stdio.h>
#include<stdlib.h>
#include"map.h"
#include"Decode.h"


//Função para plotar o menu e receber o valor de escolha do usuario.
int menu(){
    int i;
	printf("----------MENU----------\n");
	printf("|1 - EXECUTAR          |\n");
	printf("|2 - SAIR              |\n");
	printf("------------------------\n");
	printf("Choose: "); scanf("%d",&i);
	printf("\n");
	return i;
}

int main(){
    int escolha, number;            //Escolha é variavel que vai receber o valor do menu,
									//number vai receber a conversão feita da string para o número hexadecimal.
    FILE *ptr_input, *ptr_output;	//Ponteiros para os arquivos de entrada e saida.
    char line[10];                  //String para receber as linhas do arquivo de entrada.

	//do while para criar a interface do menu e repetir ela quando um valor não valido for digitado.
    do{
        escolha = menu();
        switch(escolha){
			
            case(1):
				//Nesse programa vai ser considerado que sempre o nome do arquivo de entrada vai ser input e o arquivo de saida gerado vai ser o output
                ptr_input = fopen("input.in", "r");   //fopen para que o ponteiro aponte para o arquivo input e que so tenha permissão de leitura do arquivo
                ptr_output = fopen("output.out", "w"); //fopen para criar o arquivo de saida e assim poder escrever nele.

				//Caso de teste para verificar o que causou a ptr_input ser NULL, um causa pode ser que o arquivo não existe, possui um nome diferente
				// ou outro tipo de erro.
                if(ptr_input == NULL){
					printf("Error! O arquivo não existe, possui um nome diferente de input ou um outro motivo.\n");
                    return 1;
                }
                
                // Verificar se a ptr_output não foi NULL.
                if(ptr_output == NULL){
					printf("Error!");
					return 1;
				}

				//Laço para pegar as linhas do input enquanto ele não acabar, a string que representa a linha é salvo em line.
                while(fgets(line, 10, ptr_input) != NULL){
                    number = strtol (line,NULL,16);	//função para converter a string da linha que representa uma instrução 
													//em hexadecimal em um número hexadecimal.
                    
                    mapFunction(number, ptr_output);//Função para achar os mnemonics das instruções em hexadecimal.
													// Essa função se encontra na map.c
                }

                fclose(ptr_input);		//com o laço finalizado, o programa pode ser encerrado e com isso os ponteiros
                fclose(ptr_output);
                
                printf("Programa executado com sucesso e finalizado!\n");
                return 0;

            break;

			// Caso queira sair do Programa.
            case(2):
                printf("Programa finalizado.\n");
                return 0;
            break;

			// Foi feito uma escolha não valida.
            default:
                system("cls");
                printf("escolha nao valida!!! escolha novamente\n");
            break;
            
        }
    }while(escolha != 2);
    
    return 0;
}
