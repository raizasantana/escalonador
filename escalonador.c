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
} Fila;

int esta_na_fila(Fila *fila, Processo *p)
{
	int i = 0;

	for (int i = 0; i < fila->tam; i++) {
		if (fila->fila[i]->id == p->id)
			return i;
	}
	return -1;
}

int adiciona_na_fila(Fila *fila, Processo *p, Processo *pai) 
{
	fila->tam++;

	int j;

	if (pai->id )
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
int main() 
{

}