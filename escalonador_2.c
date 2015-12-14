#include <stdio.h>
#include <stdlib.h>
#define M 20

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

        p[(*id_processo)][4] = 2; //Processo entre no status executando
        p[(*id_processo)][2] = p[(*id_processo)][2] + (*tempo_fila); //Atualiza o tempo estimado do processo

        (*tempo_fila) = (*tempo_fila) + p[(*id_processo)][2]; // Atualiza o tempo da fila com o novo processo
        (*tempo) = (*tempo) + p[(*id_processo)][2];//Atualiza o tempo total com o tempo do processo

    } else
    {

        int p_aux = fila[pos_pai+ 1]; //Salva o processo seguinte ao pai
        fila[pos_pai + 1] = (*id_processo); //Passa o novo processo na frente do antigo
        fila[*tam] = (*id_processo); //Adiciona o processo no final fila 1

        p[pos_pai + 1][4] = 2; //Processo entre no status executando
        p[pos_pai + 1][2] = p[pos_pai + 1][2] + (*tempo_fila); //Atualiza o tempo estimado do processo

        (*tempo_fila) = p[pos_pai + 1][2]; // Atualiza o tempo da fila com o novo processo
        (*tempo) += p[pos_pai + 1][2]; //Aumenta o tempo geral da execução dos processos
    }

    for(j = 0; j< 10; j++) //Envelhecimento dos processos
        p[j][6]++;


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
    FILE *arq_resultado;
    int p[M][7]; //[ id_processo processo_pai tempo status ut_entrada ciclos]
    int fila1[10], fila2[10];
    int i, f1 = 0, f2 = 0, j, k;
    int tempo_geral = 0, tempo_f1 = 0, tempo_f2 = 0;
    int tr_f1 = 1, tr_f2 = 0;

    zeros(&fila1,10);
    zeros(&fila2,10);

    le_processos(p);

    //Primeiro Processo entra na fila 1
    fila1[0] = p[0][0];
    f1++;

    //Adicionando o tempo do processo na fila
    tempo_geral = tempo_f1 + p[0][2];
    p[0][2] = tempo_geral;
    tempo_f1 = tempo_geral;

    for(j = 0; j< 20; j++) //Processos iniciando ciclos = 0
                p[j][6] = 0;
    i = 1;

    //Escalonamento
    int ciclos = 1;
    while(ciclos <= 500)
    {


     if(p[i][4] == 3) //Se um processo já foi executado
     {
        ciclos++;
        continue;
     }

     //printf("f1 %d f2 %d\n",f1,f2);
        //Entrando na fila
       if((p[i][1] > 0)) //Se existir dependencia e o processo pai ainda n tiver sido executado
        {

            int posicao_pai = processo_pai_esta_na_fila(fila1,p[i][1]);


            if(posicao_pai > -1 && posicao_pai < 9 && f1 < 9) //Se processo pai está na fila1 e ainda tem espaco nela
            { 
                 adiciona_na_fila(&f1,fila1,p,&i,&tempo_f1,&tempo_geral,posicao_pai);//add na fila1
                 tr_f1 = tr_f1 + 1;

            } else if (f2 < 9) //Se não, o processo entra na fila 2, se houver espaço.
                {
                    adiciona_na_fila(&f2,fila2,p,&i,&tempo_f2,&tempo_geral,posicao_pai);//add na fila2
                    tr_f2= tr_f2 + 1;

                 }
                 if (p[p[i][1]][4] == 3)
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
            p[i][6]++;
            printf("f1 %d f2 %d",f1,f2);
            tr_f1 = tr_f1 + 1;
            tr_f2 = tr_f2 +1;

        }

         //Retirando os processos da fila 1
         for(k = 0; k < f1; k++)
         {
            //  printf("f1 %d\n",f1);
            if(tr_f1 >  p[fila1[k]][2])
            {

                f1--;
                //printf("pid %d trf %d\n",fila1[k],tr_f1);
                p[fila1[k]][4] = 3;
                p[fila1[k]][3] = p[fila1[k]][5] + tempo_f1;
                tr_f1 = tr_f1 + p[fila1[k]][3];
            }

         }

        //Retirando os processos da fila 2
         for(k = 0; k < f2; k++)
         {
            //printf("fila[k] %d\n",fila2[k]);
            if(tr_f2 > p[fila2[k]][2])
            {
            //printf("pid %d trf %d\n",fila2[k],tr_f2);
                f2--;
                p[fila2[k]][4] = 3;
                p[fila2[k]][3] = p[fila2[k]][5] + tempo_f2;
                tr_f2 = tr_f2 + p[fila1[k]][3];
            }

         }



         (i >= 19) ? (i = 0) : (i++);
         ciclos++;



    }
       // printf("tr1 %d tr2 %d\n",tr_f1,tr_f2);
    arq_resultado = fopen("resultado.txt","w+");
    fprintf(arq_resultado,"PID Estimado Real\n");
   

    for(i = 0; i < 20; i++)
        printf("%d %d %d\n",p[i][0],p[i][2],p[i][3]);

    fclose(arq_resultado);

}

