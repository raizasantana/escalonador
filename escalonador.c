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
	Processo *fila[FILA_TAM];
	int tam;
	int tempo_estimado;
	int tempo_real;
} Fila;

int esta_na_fila(Fila *fila, Processo *p)
{
	int i = 0;

	for (i = 0; i < fila->tam; i++) 
	{
		if (fila->fila[i] == NULL)
			return -1;
		else if (fila->fila[i]->id == p->id)
			return i;
	}
	return -1;
}

int adiciona_na_fila(Fila *fila, Processo *p, Processo *pai, int *tempo_geral, int pos_pai) 
{
	

	int j;

	if (esta_na_fila(fila, pai) == -1) 
	{
		fila->fila[fila->tam] = p;

		p->status = EXECUTANDO;
		p->tempo_estimado = p->tempo_estimado + fila->tempo_estimado;

		fila->tempo_estimado = fila->tempo_estimado + p->tempo_estimado;

		*tempo_geral = *tempo_geral + p->tempo_estimado;
	} 
	else  
	{
		fila->fila[pos_pai + 1] = p;
		fila->fila[fila->tam] = p;

		fila->fila[pos_pai + 1]->status = EXECUTANDO;
		fila->fila[pos_pai + 1]->tempo_estimado + fila->tempo_estimado;

		fila->tempo_estimado = fila->fila[pos_pai + 1]->tempo_estimado + fila->tempo_estimado;
		*tempo_geral = *tempo_geral + fila->fila[pos_pai + 1]->tempo_estimado;
	}
	fila->tam++;
	for(j = 0; j < fila->tam; j++) //Envelhecimento dos processos
	{
        fila->fila[j]->ciclos++;
    }
}
Fila *cria_fila_vazia()
{
	Fila *fila = malloc(sizeof(Fila));
	int i = 0;
	for (i = 0; i < FILA_TAM; i++)
		fila->fila[i] = NULL;

	fila->tam = 0;
	fila->tempo_estimado = 0;
	fila->tempo_real = 0;

	return fila;
}
void le_processos(Processo processos[])
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
		fscanf(arq,"%d %d %d %d %d %d", &processos[i].id, 
										&processos[i].id_proc_pai, 
										&processos[i].tempo_estimado,
										&processos[i].tempo_real, 
										&processos[i].status, 
										&processos[i].ut_de_entrada);
		
	}

	fclose(arq);

}
void print_fila(Fila *fila)
{
	int i = 0;
	for (i = 0; i < fila->tam; i++) 
	{
		printf("Processo -- id: %d status: %d\n", fila->fila[i]->id, fila->fila[i]->status);
	}

}
int main() 
{
	Processo pai_null = {.id = -2, 
						.id_proc_pai = -2, 
						.tempo_estimado = -2, 	
						.tempo_real = -2,
						.status = -2,
						.ut_de_entrada = -2,
						.ciclos = -2}; 

	FILE *arq_resultado;

	Processo processos[M];

	Fila *fila1 = cria_fila_vazia(),
		 *fila2 = cria_fila_vazia();


	le_processos(processos);

	int tempo_geral = 0, i = 0, j = 0, k = 0;

	printf("id: %d\n", processos[2].id);

	//adiciona_na_fila(Fila *fila, Processo *p, Processo *pai, int *tempo_geral, int pos_pai) 
	//printf("esta na fila? %d\n", esta_na_fila(fila1, &pai_null));
	adiciona_na_fila(fila1, &processos[0], &pai_null, &tempo_geral, -1);
	//printf("esta na fila? %d\n", esta_na_fila(fila1, &pai_null));

	print_fila(fila1);

	for(j = 0; j < M; j++) //Processos iniciando ciclos = 0
		processos[j].ciclos = 0;

	int ciclos = 1;


	return 0;
}