/*
  5 filosofi sono a tavola per mangiare
  Per mangiare occorrono 2 forchette
  Alla destra di ciascun filosofo c'è una forchetta.
  Per mangiare il filosofo deve prendere sia la forchetta alla sua destra
  e anche la forchetta alla sua sinistra
  Dopo aver mangiato il filosofo mette a posto le forchette e pensa per un po'
  Poi gli viene ancora fame e cerca di mangiare
  VINCOLI DEL PROBLEMA
    1) Evidentemente non potranno mangiare tutti contemporaneamente ma occorre assicurare
      il massimo grado di parallelismo. Cioè devo permetere che 2 filosofi (non adiacenti) mangino assieme
      utilizzando 4 delle 5 forchette.
    2) Contemporaneamente andrebbe evitato il deadlock: se ciascun filosofo prende la forchetta
      di sinistra e poi si mette in attesa di quella di destra, si bloccano tutti indefinitamente.
  Se si accorgono del blocco e tutti posano assieme la loro forchetta e poi le riprendono assieme
  allora potrebbero non mangiare mai nessuno (starvation).
  Come evitare il deadlock:
    1. Prendere due forchette assieme, solo quando sono libere entrambe
    2. Prendere le forchette in ordine diverso (tutti prendono per prima la forchetta alla destra, uno solo la forchetta a sinistra).
*/
#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <pthread.h>
#include <semaphore.h>

#define N 5

pthread_mutex_t m1, m2;
sem_t S[N];
int fil[N] = {0, 1, 2, 3, 4};

void *filosofo(void *num);
void prendiForchette(int i);

int main(int argc, char const *argv[]) {
  /* code */
  srand(time(NULL));
  pthread_t t[N];
  pthread_mutex_unlock(&m1);
  pthread_mutex_unlock(&m2);
  for(int i=0; i<N; i++) sem_init(&S[i], 0, 1); // Sbloccato
  for(int i=0; i<N; i++) pthread_create(&t[i], NULL, filosofo, &fil[i]);
  for(int i=0; i<N; i++) pthread_join(t[i], NULL);
  return 0;
}

void *filosofo(void *num){
  int i = *(int *)num;
  while(1){
    sleep(rand()%N);
    printf("Il filosofo %d sta pensando\n", i+1);
    sleep(rand()%N);
    printf("Il filosofo %d ha fame\n", i+1);
    sleep(rand()%N);
    // prendi forchette
    prendiForchette(i);
    sleep(rand()%N);
  }
}

void prendiForchette(int i){
  pthread_mutex_lock(&m1);
  int val, valDx;
  sem_getvalue(&S[i], &val);
  sem_getvalue(&S[(i+1)%N], &valDx);
  pthread_mutex_unlock(&m1);
  if (val==1&&valDx==1) {
    /* code */
    //pthread_mutex_lock(&m2);
    sem_wait(&S[(i)%N]);
    sem_wait(&S[(i+1)%N]);
    //pthread_mutex_unlock(&m2);
    printf("Il filosofo %d mangia\n", i+1);
    sleep(rand()%N);
    sem_post(&S[i]);
    sem_post(&S[(i+1)%N]);
    // posa forchette
  }else{
    prendiForchette(i);
  }
}