#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define FILA_TAM 10
#define M 20

#define PRONTO 0
#define BLOQUEADO 1
#define EXECUTANDO 2
#define TEMINADO 3


typedef struct _processo {
	int id;
	int id_proc_pai;
	int tempo_estimado;
	int tempo_real;
	int status;
	int ut_de_entrada;
	int ciclos;
} Processo;

typedef struct _fila {
	Processos *fila[FILA_TAM];
	int tam;
	int tempo;
} Fila;

int esta_na_fila(Fila *fila, Processo *p)
{
	int i = 0;

	for (int i = 0; i < fila->tam; i++) 
	{
		if (fila->fila[i]->id == p->id)
			return i;
	}
	return -1;
}

int adiciona_na_fila(Fila *fila, Processo *p, Processo *pai) 
{
	fila->tam++;

	int j;

	if (esta_na_fila(fila, pai) == -1) 
	{
		fila->fila[fila->tam] = p;

		p->status = EXECUTANDO;
		p->tempo_estimado = p->tempo_estimado + fila->tempo;
	}
}
void adiciona_na_fila(int *tam, int *fila, int p[M][7], int *id_processo, int *tempo_fila, int *tempo_geral, int pos_pai)
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
Fila *cria_fila_vazia()
{
	Fila *fila = malloc(sizeof(Fila));
	fila->tam = 0;
	fila->tempo = 0;

	return fila;
}
void le_processos(Processos processos[])
{
	int n = 0, i;
	FILE *arq;
	arq = fopen("processos.txt","r");

	if(arq == NULL)
	{
		printf("Erro ao ler arquivo.");
		return;
	}

	fscanf(arq,"%d", &n);

	for (i = 0; i < n; i++)
	{
		fscanf(arq,"%d %d %d %d %d %d", &processos[i]->id, 
										&processos[i]->id_proc_pai, 
										&processos[i]->tempo_estimado,
										&processos[i]->tempo_real, 
										&processos[i]->status, 
										&processos[i]->ut_de_entrada);
		
	}

	fclose(arq);

}
int main() 
{
	FILE *arq_resultado;

	Processos processos[M];

	Fila *fila1 = cria_fila_vazia(),
		 *fila2 = cria_fila_vazia();


	le_processos(processos);


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
}