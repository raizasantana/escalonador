#include <stdio.h>
#include <stdlib.h>
#define M 20

/*
*   A matriz de processos terá 6 posicoes para cada processo.
*   [id_processo | processo_pai | tempo_estimado | tempo_real | status | UT_de_entrada]
*   O tempo começa a contar assim que ele sai da fila de prontos e chega no processador.
*   O UT_de_entrada marca o momento de entrada do processo na fila em relação ao primeiro processo. UT = Unidades de Tempo
*   O status pode ser 0 - PRONTO; 1 - BLOQUEADO; 2 - EXECUTANDO; 3 - TERMINADO
*/

void adiciona_na_fila(int *tam, int fila[10], int p[M][7], int *id_processo, int *tempo_fila, int *tempo, int pos_pai)
{
    (*tam)++; //Aumenta o tamanho da fila
    int j;

    if(pos_pai == -1)//Processo pai não está na fila
    {

        fila[(*tam)] = (*id_processo);
        p[(*id_processo)][4] = 2; //Processo entre no status executando
        p[(*id_processo)][2] = p[(*id_processo)][2] + (*tempo_fila); //Atualiza o tempo estimado do processo
        (*tempo_fila) = p[(*id_processo)][2]; // Atualiza o tempo da fila com o novo processo
        (*tempo) += p[(*id_processo)][2];//Atualiza o tempo total com o tempo do processo
    } else
    {

        int p_aux = fila[pos_pai+ 1]; //Salva o processo seguinte ao pai

       fila[pos_pai + 1] = (*id_processo); //Passa o novo processo na frente do antigo

        fila[(*tam)] = (*id_processo); //Adiciona o processo no final fila 1

        p[pos_pai + 1][4] = 2; //Processo entre no status executando

        p[pos_pai + 1][2] = p[pos_pai + 1][2] + (*tempo_fila); //Atualiza o tempo estimado do processo
        (*tempo_fila) = p[pos_pai + 1][2]; // Atualiza o tempo da fila com o novo processo
        (*tempo) += p[pos_pai + 1][2]; //Aumenta o tempo geral da execução dos processos
    }

    for(j = 0; j< 10; j++) //Envelhecimento dos processos
        p[j][6]++;

}
void le_processos(int processos[M][6])
{
    int n = 0, i;
    FILE *arq;
    arq = fopen("processos.txt","r");

    if(arq == NULL)
    {
        printf("Erro ao ler arquivo.");
        return 0;
    }

    fscanf(arq,"%d",&n);

    for (i = 0; i < n; i++)
    {
        fscanf(arq,"%d %d %d %d %d %d",&processos[i][0],&processos[i][1], &processos[i][2],&processos[i][3], &processos[i][4], &processos[i][5]);
    }

    fclose(arq);

}

int processo_pai_esta_na_fila(int fila[10], int p)
{
    int i;

    for(i = 0; i < 10; i++)
        if(fila[i] == p)
            return i;

    return -1;
}

int main()
{

    int p[M][7]; //[ id_processo processo_pai tempo status ut_entrada ciclos]
    int fila1[10], fila2[10];
    int i, f1 = 0, f2 = 0, j;
    int tempo_geral = 0, tempo_f1 = 0, tempo_f2 = 0;
    le_processos(p);

    //Primeiro Processo entra na fila 1
    fila1[0] = p[0][0];
    f1++;

    //Adicionando o tempo do processo na fila
    tempo_geral = tempo_f1 + p[0][2];
    p[0][2] = tempo_geral;
    tempo_f1 = tempo_geral;

    for(j = 0; j< 10; j++) //Processos iniciando ciclos = 0
                p[j][6] = 0;
    i = 1;
    //Escalonamento
    while(i < 20)
    {

        if(p[i][1] > 0)//Se existe dependencia
        {
            int posicao_pai = processo_pai_esta_na_fila(fila1,p[i][1]);

            if(posicao_pai > -1 && posicao_pai < 9 && f1 < 9) //Se processo pai está na fila1 e ainda tem espaco nela
                 adiciona_na_fila(&f1,fila1,p,&i,&tempo_f1,&tempo_geral,posicao_pai);//add na fila1

           else if (f2 < 9)//Se não, o processo entra na fila 2, se houver espaço.
                adiciona_na_fila(&f2,fila2,p,&i,&tempo_f2,&tempo_geral,posicao_pai);//add na fila2

       }
       else if(f1> f2 && f2 < 9) {//Se a fila1 é maior que a 2, adiciona na fila 2
            //printf("[%d] F2: f1 %d f2 %d\n",i,f1,f2);
            adiciona_na_fila(&f2,fila2,p,&i,&tempo_f2,&tempo_geral,-1);//add na fila2
            }
        else {//Adiciona na fila 1
              //printf("[%d] F1: f1 %d f2 %d\n",i,f1,f2);
              adiciona_na_fila(&f1,fila1,p,&i,&tempo_f1,&tempo_geral,-1);//add na fila2
              }
         i++;

    }
    }
       /*
        *       ENTRANDO NA FILA
        */
        //Se o processo "pai" está na fila 1 e a fila 1 não estiver cheia, entao o processo entra na fila


        /*
        *       SAINDO DA FILA
        */

     /*   for(j = 0; j < 10; j++)
        {
            if(tempo_f1 > p[fila1[i]][2]) //Se o tempo da fila está maior do que o tempo estimado do processo, ele já saiu da fila
            {
                p[fila1[i]][4] = p[fila1[i]][2]; //Salvando o tempo de execução
                fila1[i] = -1;
                f1--;

            }

            if(tempo_f2 > p[fila2[i]][2]) //Se o tempo da fila está maior do que o tempo estimado do processo, ele já saiu da fila
            {
                fila2[i] = -1;
                f2--;
            }
        }*}}*/
