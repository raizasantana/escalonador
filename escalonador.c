#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define FILA_TAM 10
#define M 20

#define PRONTO 0
#define BLOQUEADO 1
#define EXECUTANDO 2
#define TERMINADO 3


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

void desloca_fila(Fila *fila, int pos_pai)
{
    int i;
    for(i = fila->tam; i > pos_pai+2; i--) //vai copiando pra cima os processos pra abrir lugar pro novo entrar
    {
        fila->fila[i] = fila->fila[i-1];
    }
}

void adiciona_na_fila(Fila *fila, Processo *p, Processo *pai, int pos_pai)
{
    int j;

    if (esta_na_fila(fila, pai) == -1) // pai não está na fila. add no final da fila.
    {
        printf("IF adicionando o processo: %d na fila\n", p->id);
        
        p->status = EXECUTANDO;
        fila->fila[fila->tam] = p;
    }
    else // pai está na fila. add depois do pai e move todos os outros pra trás
    {
        printf("ELSE adicionando o processo: %d na fila\n", p->id);
        desloca_fila(fila, pos_pai);
        p->status = EXECUTANDO;
        fila->fila[pos_pai + 1] = p;
    }
    
    fila->tam++;
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
void le_processos(Processo processos[], int *n)
{
    int  i;
    FILE *arq;
    arq = fopen("processos-teste.txt","r");

    if(arq == NULL)
    {
        printf("Erro ao ler arquivo.");
        return;
    }

    fscanf(arq,"%d", n);
    // printf("n: %d\n", *n);

    for (i = 0; i < *n; i++)
    {
        fscanf(arq,"%d %d %d %d %d %d\n", &processos[i].id, 
                                        &processos[i].id_proc_pai, 
                                        &processos[i].tempo_estimado,
                                        &processos[i].tempo_real, 
                                        &processos[i].status, 
                                        &processos[i].ut_de_entrada);


       // printf("proc: %d %d %d %d %d %d\n", processos[i].id, 
       //                                  processos[i].id_proc_pai, 
       //                                  processos[i].tempo_estimado,
       //                                  processos[i].tempo_real, 
       //                                  processos[i].status, 
       //                                  processos[i].ut_de_entrada);
    }

    fclose(arq);

}

void remove_primeiro(Fila* fila)
{
    int i;
    for (i = 0; i < fila->tam-1; i++) {
        fila->fila[i] = fila->fila[i+1];
    }
    fila->tam = fila->tam -1;
}

void incrementa_temporeal_processos_fila(Fila* fila)
{
    int i;
    for (i=0; i < fila->tam; i++) {
        fila->fila[i]->tempo_real++;
    }
}

void incrementa_ciclos_processos_nos_processadores(int processador1, int processador2, Processo* processos)
{
    if (processador1 != -1) {
        processos[processador1].ciclos++;
    }
    
    if (processador2 != -1) {
        processos[processador2].ciclos++;
    }
}
void print_fila(Fila *fila)
{
    int i = 0;
    printf("exibindo a fila...\n");
    for (i = 0; i < fila->tam; i++) 
    {
        printf("Processo -- id: %d status: %d proc_pai: %d\n", fila->fila[i]->id, 
                                                                fila->fila[i]->status,
                                                                fila->fila[i]->id_proc_pai);
    }

}

void print_processos(Processo processos[], int n)
{
    int i = 0; 
    printf("exibindo os processos...\n");
    for (i = 0; i < n; i++) 
    {
        printf("Processo -- id: %d status: %d proc_pai: %d\n", processos[i].id, 
                                                               processos[i].status,
                                                               processos[i].id_proc_pai);
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

    int n = 0;

    Processo processos[M];
    
    int processador1 = -1, processador2 = -1;

    Fila *fila1 = cria_fila_vazia(),
         *fila2 = cria_fila_vazia();


    le_processos(processos, &n);

    int tempo_geral = 0, i = 0, j = 0, k = 0;

    // printf("id: %d\n", processos[2].id);

    //adiciona_na_fila(Fila *fila, Processo *p, Processo *pai, int *tempo_geral, int pos_pai) 
    //printf("esta na fila? %d\n", esta_na_fila(fila1, &pai_null));
    processos[0].id_proc_pai = -2;
    //adiciona_na_fila(fila1, &processos[0], &pai_null, &tempo_geral, esta_na_fila(fila1, &pai_null));
    //printf("esta na fila? %d\n", esta_na_fila(fila1, &pai_null));

    //adiciona_na_fila(fila1, &processos[1], &processos[0], &tempo_geral, esta_na_fila(fila1, &processos[0]));

    print_fila(fila1);

    for(j = 0; j < M; j++){ //Processos iniciando ciclos = 0
        processos[j].ciclos = 0;
        processos[j].tempo_real = 0;
    }
    // print_processos(processos, n);

    int ciclos = 1;
    
    int id_proc = 0;

    while(1)
    {
        printf("\n---------\nciclo: %d", ciclos);
        
        if (id_proc < n && processos[id_proc].status != TERMINADO && (fila1->tam < 9 || fila2->tam < 9) ) {
        
            processos[id_proc].tempo_real = ciclos;
            
            if (fila1->tam < 9 && esta_na_fila(fila1, &processos[processos[id_proc].id_proc_pai]) > -1) // se a fila1 ainda não está cheia e o pai está nela
            {
                printf("\ndebug 1: add pq tinha pai na fila 1");
                adiciona_na_fila(fila1, &processos[id_proc], &processos[processos[id_proc].id_proc_pai], esta_na_fila(fila1, &processos[processos[id_proc].id_proc_pai]));
                id_proc ++; // chama o próximo pra ser escalonado
            }
            else if (fila2->tam < 9 && esta_na_fila(fila2, &processos[processos[id_proc].id_proc_pai]) > -1) // se o pai estiver na fila2 e esta ainda não estiver cheia
            {
                printf("\ndebug 2: add pq tinha pai na fila 2");
                adiciona_na_fila(fila2, &processos[id_proc], &processos[processos[id_proc].id_proc_pai], esta_na_fila(fila2, &processos[processos[id_proc].id_proc_pai]));
                id_proc++;
            }
            else // não tinha pai em nenhuma fila: adiciona na fila menor
            {
                printf("\nnao tinha pai em nenhuma fila. fila1->tam: %d, fila2->tam:%d\n", fila1->tam, fila2->tam);
                if(fila1->tam < fila2->tam){
                    printf("\n debug 3: add na fila 1 pq era menor\n");
                    adiciona_na_fila(fila1, &processos[id_proc], &processos[processos[id_proc].id_proc_pai], esta_na_fila(fila1, &processos[processos[id_proc].id_proc_pai]));
                } else {
                    printf("\n debug 3: add na fila 2 pq era menor\n");
                    adiciona_na_fila(fila2, &processos[id_proc], &processos[processos[id_proc].id_proc_pai], esta_na_fila(fila2, &processos[processos[id_proc].id_proc_pai]));
                }
                id_proc++;
            }
        }
        
        if (processador1 == -1 && fila1->tam > 0) { // está livre. pega o primeiro da fila1, se fila tiver alguém
            printf("\nprocessador1 esta livre e vai pegar o p%d\n", fila1->fila[0]->id);
            processador1 = fila1->fila[0]->id;
            remove_primeiro(fila1);
        }
        if (processador2 == -1 && fila2->tam > 0) { // está livre. pega o primeiro da fila2, se fila tiver alguém
            printf("\nprocessador2 esta livre e vai pegar o p%d\n", fila2->fila[0]->id);
            processador2 = fila2->fila[0]->id;
            remove_primeiro(fila2);
        }
        
        if (processos[processador1].ciclos == processos[processador1].tempo_estimado) {
            printf("\nprocessador1: processo %d terminou!\n", processos[processador1].id);
            processos[processador1].status = TERMINADO;
            processador1 = -1; // coloca o processador1 como livre de novo
        }
        
        if (processos[processador2].ciclos == processos[processador2].tempo_estimado) {
            printf("\nprocessador2: processo %d terminou!\n", processos[processador2].id);
            processos[processador2].status = TERMINADO;
            processador2 = -1; // processador 2 está livre
        }
        
        incrementa_ciclos_processos_nos_processadores(processador1, processador2, processos);
        incrementa_temporeal_processos_fila(fila1);
        incrementa_temporeal_processos_fila(fila2);
        
        //Se o proc foi adicionado a uma fila, seu ciclo não deveria ser aumentado, já que ele já recebeu o ciclo atual no inicio do while. Rever isso
        
        ciclos++;
        
        if (id_proc == n && processador1 == -1 && processador2 == -1){ // processador 1 não está ficando livre nunca;
            printf("\nTerminou\n");
            exit(0);
        }
    }


    return 0;

}