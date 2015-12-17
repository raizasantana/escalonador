#include <stdio.h>
#include <stdlib.h>
#define N 500
#define CICLOS_MAX 100

/*
*
*/
int main()
{

    FILE *arq;
    int i, depende;

    arq = fopen("processos.txt","w+");

    //Escreve a quantidade de processos a serem escalonados
    fprintf(arq,"%d \n",N);

     srand (time(NULL));
    //Gera as entradas e dependências para cada processo 
    for(i = 0; i< N; i++)
    {
	 if (i==0)
            depende = -2;
        else
            depende = rand()%i + 1; //Indica o processo dependente
        
        if(depende == i || (depende % 7)==0)
            depende = -2;

        int ut = i + 5; //Gerando o tempo que cada um entrou na fila
        fprintf(arq,"%d %d %d %d %d %d\n", i, depende, rand()%20 + 1, 0, 0, ut);
    }

    fclose(arq);
}
