#include <stdio.h>
#define N 20

int main()
{

    FILE *arq;
    int i, depende;

    arq = fopen("processos.txt","w+");

    //Escreve a quantidade de processos a serem escalonados
    fprintf(arq,"%d \n",N);

    //Gera as entradas e dependências para cada processo (add mais info)
    for(i = 1; i< N; i++)
    {
        depende = rand()% i + 1; //Indica o processo dependente
        if( depende == i)
            depende = 0;

        int ut = i + 5; //Gerando o tempo que cada um entrou na fila
        fprintf(arq,"%d %d %d %d %d %d\n",i,depende, rand()%20 + 1, 0, 0, ut);
    }

    fclose(arq);
}
