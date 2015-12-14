#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define FILA_TAM 10
#define M 20

#define ID 0
#define ID_PROCESSO_PAI 1
#define TEMPO_ESTIMADO 2
#define TEMPO_REAL 3
#define STATUS 4
#define UT_DE_ENTRADA 5
#define CICLOS 6


/*
*   A matriz de processos terá 6 posicoes para cada processo.
*   [id_processo | processo_pai | tempo_estimado | tempo_real | status | UT_de_entrada | ciclos]
*   O tempo começa a contar assim que ele sai da fila de prontos e chega no processador.
*   O UT_de_entrada marca o momento de entrada do processo na fila em relação ao primeiro processo. UT = Unidades de Tempo
*   O status pode ser 0 - PRONTO; 1 - BLOQUEADO; 2 - EXECUTANDO; 3 - TERMINADO
*/

void zeros(int *vetor,int tam)
{
    int i;
    for(i = 0; i < tam; i++)
        vetor[i] = 0;
}

void adiciona_na_fila(int *tam, int *fila, int p[M][7], int *id_processo, int *tempo_fila, int *tempo, int pos_pai)
{

    *tam = *tam + 1;
    int j;

    if(pos_pai == -1)//Processo pai não está na fila
    {

        fila[*tam] = (*id_processo);

        p[(*id_processo)][STATUS] = 2; //Processo entre no status executando
        p[(*id_processo)][TEMPO_ESTIMADO] = p[(*id_processo)][TEMPO_ESTIMADO] + (*tempo_fila); //Atualiza o tempo estimado do processo

        (*tempo_fila) = (*tempo_fila) + p[(*id_processo)][TEMPO_ESTIMADO]; // Atualiza o tempo da fila com o novo processo
        (*tempo) = (*tempo) + p[(*id_processo)][TEMPO_ESTIMADO];//Atualiza o tempo total com o tempo do processo

    } else
    {

        int p_aux = fila[pos_pai+ 1]; //Salva o processo seguinte ao pai
        fila[pos_pai + 1] = (*id_processo); //Passa o novo processo na frente do antigo
        fila[*tam] = (*id_processo); //Adiciona o processo no final fila 1

        p[pos_pai + 1][STATUS] = 2; //Processo entre no status executando
        p[pos_pai + 1][TEMPO_ESTIMADO] = p[pos_pai + 1][TEMPO_ESTIMADO] + (*tempo_fila); //Atualiza o tempo estimado do processo

        (*tempo_fila) = p[pos_pai + 1][TEMPO_ESTIMADO]; // Atualiza o tempo da fila com o novo processo
        (*tempo) += p[pos_pai + 1][TEMPO_ESTIMADO]; //Aumenta o tempo geral da execução dos processos
    }

    for(j = 0; j< 10; j++) //Envelhecimento dos processos
        p[j][CICLOS]++;


}
void le_processos(int processos[][7])
{
    int n = 0, i;
    FILE *arq;
    arq = fopen("processos.txt","r");

    if(arq == NULL)
    {
        printf("Erro ao ler arquivo.");
        return;
    }

    fscanf(arq,"%d",&n);

    for (i = 0; i < n; i++)
    {
        fscanf(arq,"%d %d %d %d %d %d",&processos[i][ID],&processos[i][ID_PROCESSO_PAI], &processos[i][TEMPO_ESTIMADO],&processos[i][TEMPO_REAL], &processos[i][STATUS], &processos[i][UT_DE_ENTRADA]);
    }

    fclose(arq);

}

int processo_pai_esta_na_fila(int fila[], int p)
{
    int i;

    for(i = 0; i < FILA_TAM; i++)
        if(fila[i] == p)
            return i;

    return -1;
}
int main()
{
    FILE *arq_resultado;
    int p[M][7]; //[ id_processo processo_pai tempo status ut_entrada ciclos]
    int fila1[FILA_TAM], fila2[FILA_TAM];
    int i, f1 = 0, f2 = 0, j, k;
    int tempo_geral = 0, tempo_f1 = 0, tempo_f2 = 0;
    int tr_f1 = 1, tr_f2 = 0;

    zeros(fila1, FILA_TAM);  
    zeros(fila2, FILA_TAM); 
    // memset(fila1, 0, FILA_TAM * sizeof(int));
    // memset(fila2, 0, FILA_TAM * sizeof(int));


    le_processos(p);

    //Primeiro Processo entra na fila 1
    fila1[0] = p[0][ID];
    f1++;

    //Adicionando o tempo do processo na fila
    tempo_geral = tempo_f1 + p[0][TEMPO_ESTIMADO];
    p[0][TEMPO_ESTIMADO] = tempo_geral;
    tempo_f1 = tempo_geral;

    for(j = 0; j< 20; j++) //Processos iniciando ciclos = 0
                p[j][CICLOS] = 0;
    i = 1;

    //Escalonamento
    int ciclos = 1;
    while(ciclos <= 500)
    {


     if(p[i][STATUS] == 3) //Se um processo já foi executado
     {
        ciclos++;
        continue;
     }

     //printf("f1 %d f2 %d\n",f1,f2);
        //Entrando na fila
       if((p[i][ID] > 0)) //Se existir dependencia e o processo pai ainda n tiver sido executado
        {

            int posicao_pai = processo_pai_esta_na_fila(fila1, p[i][ID_PROCESSO_PAI]);


            if(posicao_pai > -1 && posicao_pai < 9 && f1 < 9) //Se processo pai está na fila1 e ainda tem espaco nela
            { 
                 adiciona_na_fila(&f1, fila1, p, &i, &tempo_f1, &tempo_geral, posicao_pai);//add na fila1
                 tr_f1 = tr_f1 + 1;

            } else if (f2 < 9) //Se não, o processo entra na fila 2, se houver espaço.
                {
                    adiciona_na_fila(&f2, fila2, p, &i, &tempo_f2, &tempo_geral, posicao_pai);//add na fila2
                    tr_f2 = tr_f2 + 1;

                 }
                 if (p[p[i][ID_PROCESSO_PAI]][STATUS] == 3)
                 {
                    printf("pid %d\n",i);
                 }

       }
        else if(f1> f2 && f2 < 9)//Se a fila1 é maior que a 2, adiciona na fila 2
         {
            adiciona_na_fila(&f2,fila2,p,&i,&tempo_f2,&tempo_geral,-1);
            tr_f2= tr_f2 + 1;

          }//add na fila2
        else if (f1 < 9){//Adiciona na fila 1
            tr_f1 = tr_f1 + 1;
            adiciona_na_fila(&f1,fila1,p,&i,&tempo_f1,&tempo_geral,-1);//add na fila2

            }
        else{
            p[i][CICLOS]++;
            printf("f1 %d f2 %d",f1,f2);
            tr_f1 = tr_f1 + 1;
            tr_f2 = tr_f2 +1;

        }

         //Retirando os processos da fila 1
         for(k = 0; k < f1; k++)
         {
            //  printf("f1 %d\n",f1);
            if(tr_f1 >  p[fila1[k]][TEMPO_ESTIMADO])
            {

                f1--;
                //printf("pid %d trf %d\n",fila1[k],tr_f1);
                p[fila1[k]][STATUS] = 3;
                p[fila1[k]][TEMPO_REAL] = p[fila1[k]][UT_DE_ENTRADA] + tempo_f1;
                tr_f1 = tr_f1 + p[fila1[k]][TEMPO_REAL];
            }

         }

        //Retirando os processos da fila 2
         for(k = 0; k < f2; k++)
         {
            //printf("fila[k] %d\n",fila2[k]);
            if(tr_f2 > p[fila2[k]][TEMPO_ESTIMADO])
            {
            //printf("pid %d trf %d\n",fila2[k],tr_f2);
                f2--;
                p[fila2[k]][STATUS] = 3;
                p[fila2[k]][TEMPO_REAL] = p[fila2[k]][UT_DE_ENTRADA] + tempo_f2;
                tr_f2 = tr_f2 + p[fila1[k]][TEMPO_REAL];
            }

         }



         (i >= 19) ? (i = 0) : (i++);
         ciclos++;



    }
       // printf("tr1 %d tr2 %d\n",tr_f1,tr_f2);
    arq_resultado = fopen("resultado.txt","w+");
    fprintf(arq_resultado,"PID Estimado Real\n");
   

    for(i = 0; i < 20; i++)
        printf("%d %d %d\n",p[i][ID], p[i][TEMPO_ESTIMADO], p[i][TEMPO_REAL]);

    fclose(arq_resultado);

}

