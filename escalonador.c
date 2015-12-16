/* TODO LIST:
 
 considerar que processos estão há muito tempo sendo passados pra trás. então passa pra frente.
 
 cache miss = anterior não é o pai
 
 FIFO: diferença está no adiciona na fila, que vai ser bem mais simples:
 
 */


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

typedef enum _proc_Status { 
    LIVRE = -1, 
    OCUPADO
} ProcStatus;

typedef struct _proc {
    Processo *p;
    ProcStatus status;
} Processador;

void print_processo(Processo p)
{
    printf("|ID: %d| STATUS: %d| PAI: %d| CICLOS: %d| REAL: %d| ESTIMADO: %d| UT_ENTRADA: %d|\n", 
                                                            p.id,
                                                            p.status,
                                                            p.id_proc_pai,
                                                            p.ciclos,
                                                            p.tempo_real,
                                                            p.tempo_estimado,
                                                            p.ut_de_entrada);
}

void print_processos(Processo processos[], int n)
{
    int i = 0;
    for (i = 0; i < n; i++) 
    {
        print_processo(processos[i]);
    }
}

int esta_na_fila(Fila *fila, Processo *p)
{
    int i = 0;

    if (p == NULL)
        return -1;

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
    for(i = fila->tam; i > pos_pai + 2; i--) //vai copiando pra cima os processos pra abrir lugar pro novo entrar
    {
        fila->fila[i] = fila->fila[i - 1];
    }
}

int adiciona_na_fila(Fila *fila, Processo *p, Processo *pai, int pos_pai)
{
    int j;

    print_processo(*p);

    if (esta_na_fila(fila, pai) == -1) // pai não está na fila. add no final da fila.
    {
        printf("\nIF adicionando o processo: %d na fila\n", p->id);

        p->status = EXECUTANDO;
        fila->fila[fila->tam] = p;
        fila->tam++;
        
        return 1; // deu cache miss
    }
    else // pai está na fila. add depois do pai e move todos os outros pra trás
    {
        printf("\nELSE adicionando o processo: %d na fila\n", p->id);
        desloca_fila(fila, pos_pai);
        p->status = EXECUTANDO;
        fila->fila[pos_pai + 1] = p;
        fila->tam++;
        
        return 0; // nao deu cache miss
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
void le_processos(Processo processos[], int *n)
{
    int  i;
    FILE *arq;
    arq = fopen("processos.txt","r");

    if(arq == NULL)
    {
        printf("Erro ao ler arquivo.");
        return;
    }

    fscanf(arq,"%d", n);
    // printf("n: %d\n", *n);
    
    printf("\n**************************************\nLeitura dos processos:\n\n");
    for (i = 0; i < *n; i++)
    {
        fscanf(arq,"%d %d %d %d %d %d\n", &processos[i].id, 
                                        &processos[i].id_proc_pai, 
                                        &processos[i].tempo_estimado,
                                        &processos[i].tempo_real, 
                                        &processos[i].status, 
                                        &processos[i].ut_de_entrada);
        processos[i].ciclos = 0;


        print_processo(processos[i]);
    }
    
    printf("\n***************************************\n\n");

    fclose(arq);

}

void remove_primeiro(Fila* fila)
{
    int i;
    for (i = 0; i < fila->tam - 1; i++) {
        fila->fila[i] = fila->fila[i + 1];
    }
    fila->tam = fila->tam - 1;
}

void incrementa_temporeal_processos_fila(Fila* fila)
{
    int i;
    for (i = 0; i < fila->tam; i++) {
        fila->fila[i]->tempo_real++;
    }
}

void incrementa_ciclos_processos_nos_processadores(Processador *processador1, Processador *processador2)
{
    if (processador1->status == OCUPADO) 
    {
        Processo *p = processador1->p;
        p->ciclos++;
        printf("\nProcesso %d @ processador1: ciclos %d de %d\n", p->id, p->ciclos, p->tempo_estimado);
    }
    if (processador2->status == OCUPADO) 
    {
        Processo *p = processador2->p;
        p->ciclos++;
        printf("\nProcesso %d @ processador1: ciclos %d de %d\n", p->id, p->ciclos, p->tempo_estimado);
    }    
}

void print_fila(Fila *fila)
{
    int i = 0;
    if (fila->tam == 0) {
        printf("\nFila vazia\n");
    }
    for (i = 0; i < fila->tam; i++) 
    {
        print_processo(*fila->fila[i]);
    }

}

void aloca_processo(Processador *processador, Processo *p)
{
    processador->p = p;
    processador->status = OCUPADO;
}
void libera_processador(Processador *processador)
{
    processador->p = NULL;
    processador->status = LIVRE;
}

int esta_vazia(Fila *fila)
{
    return fila->tam == 0;
}
int esta_cheia(Fila *fila)
{
    return fila->tam == FILA_TAM;
}
Processo *get_pai(Processo p)
{
    if (1) {}
}
int main() 
{
    Processo pai_null = {.id = -1, 
                        .id_proc_pai = -1, 
                        .tempo_estimado = 0,   
                        .tempo_real = 0,
                        .status = 0,
                        .ut_de_entrada = 0,
                        .ciclos = 0}; 

    FILE *arq_resultado;

    int n = 0;

    Processo processos[M + 1];
    
    Processador processador1 = {.p = NULL, .status = LIVRE};
    Processador processador2 = {.p = NULL, .status = LIVRE};

    Fila *fila1 = cria_fila_vazia(),
         *fila2 = cria_fila_vazia();


    le_processos(processos, &n);

    int tempo_geral = 0, i = 0, j = 0, k = 0;
    
    int cache_miss = 0;

    // printf("id: %d\n", processos[2].id);

    //adiciona_na_fila(Fila *fila, Processo *p, Processo *pai, int *tempo_geral, int pos_pai) 
    //printf("esta na fila? %d\n", esta_na_fila(fila1, &pai_null));
    //adiciona_na_fila(fila1, &processos[0], &pai_null, &tempo_geral, esta_na_fila(fila1, &pai_null));
    //printf("esta na fila? %d\n", esta_na_fila(fila1, &pai_null));

    //adiciona_na_fila(fila1, &processos[1], &processos[0], &tempo_geral, esta_na_fila(fila1, &processos[0]));

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
        
        printf("\n***************");
        printf("\nFILA 1:");
        print_fila(fila1);
        printf("\nFILA 2:");
        print_fila(fila2);
        printf("\n***************");
        
        
        if (processador1.status == LIVRE && !esta_vazia(fila1)) { // está livre. pega o primeiro da fila1, se fila tiver alguém
            printf("\nprocessador1 esta livre e vai pegar o p%d\n", fila1->fila[0]->id);
            aloca_processo(&processador1, fila1->fila[0]);
            remove_primeiro(fila1);
        }
        if (processador2.status == LIVRE  && !esta_vazia(fila2)) { // está livre. pega o primeiro da fila2, se fila tiver alguém
            printf("\nprocessador2 esta livre e vai pegar o p%d\n", fila2->fila[0]->id);
            aloca_processo(&processador2, fila2->fila[0]);
            remove_primeiro(fila2);
        }
        
        
        if (id_proc < n && processos[id_proc].status != TERMINADO && (!esta_cheia(fila1) || !esta_cheia(fila2))) {
        
            processos[id_proc].tempo_real = ciclos;
            
            if (fila1->tam < FILA_TAM - 1 && esta_na_fila(fila1, &processos[processos[id_proc].id_proc_pai]) > -1) // se a fila1 ainda não está cheia e o pai está nela
            {
                printf("\ndebug 1: add pq tinha pai na fila 1");
                cache_miss += adiciona_na_fila(fila1, &processos[id_proc], &processos[processos[id_proc].id_proc_pai], esta_na_fila(fila1, &processos[processos[id_proc].id_proc_pai]));
                id_proc++; // chama o próximo pra ser escalonado
            }
            else if (fila2->tam < FILA_TAM - 1 && esta_na_fila(fila2, &processos[processos[id_proc].id_proc_pai]) > -1) // se o pai estiver na fila2 e esta ainda não estiver cheia
            {
                printf("\ndebug 2: add pq tinha pai na fila 2");
                cache_miss += adiciona_na_fila(fila2, &processos[id_proc], &processos[processos[id_proc].id_proc_pai], esta_na_fila(fila2, &processos[processos[id_proc].id_proc_pai]));
                id_proc++;
            }
            else // não tinha pai em nenhuma fila: adiciona na fila menor
            {
                printf("\nprocesso %d nao tinha pai em nenhuma fila. fila1->tam: %d, fila2->tam:%d\n", id_proc, fila1->tam, fila2->tam);
                if (fila1->tam < fila2->tam)
                {
                    printf("\ndebug 3: add na fila 1 pq era menor\n");
                    // print_processo(processos[id_proc]);
                   cache_miss += adiciona_na_fila(fila1, &processos[id_proc], &processos[processos[id_proc].id_proc_pai], esta_na_fila(fila1, &processos[processos[id_proc].id_proc_pai]));
                } else {
                    printf("\ndebug 3: add na fila 2 pq era menor\n");
                    // print_processo(processos[id_proc]);
                   cache_miss += adiciona_na_fila(fila2, &processos[id_proc], &processos[processos[id_proc].id_proc_pai], esta_na_fila(fila2, &processos[processos[id_proc].id_proc_pai]));
                }
                id_proc++;
            }
        }
        
        // print_processo(*processador1.p);
        // printf("CHEGOU AQUI!\n");
        
        if (processador1.status == OCUPADO && processador1.p->ciclos == processador1.p->tempo_estimado) {
            printf("\nprocessador1: processo %d terminou!\n", processador1.p->id);
            processador1.p->status = TERMINADO;
            processador1.p->tempo_real = processador1.p->tempo_real + processador1.p->ciclos;
            libera_processador(&processador1); // coloca o processador1 como livre de novo
        }
        
        if (processador2.status == OCUPADO && processador2.p->ciclos == processador2.p->tempo_estimado) {
            printf("\nprocessador2: processo %d terminou!\n", processador2.p->id);
            processador2.p->status = TERMINADO;
            processador2.p->tempo_real = processador2.p->tempo_real + processador2.p->ciclos;
            libera_processador(&processador2); // processador 2 está livre
        }
        
        incrementa_ciclos_processos_nos_processadores(&processador1, &processador2);
        incrementa_temporeal_processos_fila(fila1);
        incrementa_temporeal_processos_fila(fila2);
        
        //Se o proc foi adicionado a uma fila, seu ciclo não deveria ser aumentado, já que ele já recebeu o ciclo atual no inicio do while. Rever isso
        
        ciclos++;
        
        if (id_proc == n && processador1.status == LIVRE && processador2.status == LIVRE && fila1->tam == 0 && fila2->tam == 0){ // processador 1 não está ficando livre nunca;
            printf("\nTerminou\n");
            
            for (i = 0; i < n; i++) {
                printf("Processo %d: tempo_estimado: %d tempo_real: %d\n", i, processos[i].tempo_estimado, processos[i].tempo_real);
            }
            
            printf("\nCache misses: %d\n", cache_miss);
            
            exit(0);
        }
    }

    free(fila1);
    free(fila2);

    return 0;

}