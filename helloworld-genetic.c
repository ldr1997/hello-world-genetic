// converted to C from http://www.generation5.org/content/2003/data/gahelloworld.cpp

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <time.h>

#define POPULATION_SIZE 2048
#define MAX_ITERATIONS 1500
#define KEEP_RATE 0.15f
#define MUTATION_RATE 35 //rand % 100 pls
#define TARGET "Good morning!"
#define TARGET_LENGTH 13

const int KEEP = POPULATION_SIZE * KEEP_RATE;

typedef struct {
  char string[TARGET_LENGTH + 1];
  int fitness;
} individual;

void init(individual **pop, individual **buffer) {
  int i, j;

  for (i=0; i<POPULATION_SIZE; i++) {
    //initialize fitness
    (*pop)[i].fitness = 0;
    (*buffer)[i].fitness = 0;

    //get random string ONLY for population array
    for (j=0; j<TARGET_LENGTH; j++)
      (*pop)[i].string[j] = (char)((rand() % 95) + 32);
    (*pop)[i].string[j] = '\0'; //last one should be '\0'
  }
}

void fitnessCalc(individual **pop){
  int i, j;
  for (i=0; i<POPULATION_SIZE; i++){
    for (j=0; j<TARGET_LENGTH; j++){
      (*pop)[i].fitness += abs((int)((*pop)[i].string[j] - TARGET[j]));
    }
  }
}

void sort(individual **pop){
  int i, j, smallest;
  individual temp;
  for (i=0; i<POPULATION_SIZE; i++) {
    smallest = i;

    for (j=i+1; j<POPULATION_SIZE; j++){
      if ((*pop)[j].fitness <= (*pop)[smallest].fitness) smallest = j;
    }
    if (smallest != i) {
      temp = (*pop)[i];
      (*pop)[i] = (*pop)[smallest];
      (*pop)[smallest] = temp;
    }
  }
}

//after sorting, the best of the best is kept
void keepBest(individual **pop, individual **buffer){
  int i;
  for (i=0; i<KEEP; i++) {
    (*buffer)[i] = (*pop)[i];
  }
}

void mutate(individual *a){
  int pos = rand() % TARGET_LENGTH;
  (*a).string[pos] = (char)((rand() % 95) + 32);
}

void reproduce(individual **pop, individual **buffer){
  int mut1, mut2, pos, i, j;

  for(i=KEEP; i<POPULATION_SIZE; i++){
    mut1 = rand() % (POPULATION_SIZE / 2);
    do {mut2 = rand() % (POPULATION_SIZE / 2);} while (mut1 == mut2);
    pos = rand() % TARGET_LENGTH;

    for (j=0; j<pos; j++) (*buffer)[i].string[j] = (*pop)[mut1].string[j];
    for (j=pos; j<TARGET_LENGTH; j++) (*buffer)[i].string[j] = (*pop)[mut2].string[j];
    (*buffer)[i].string[j] = '\0';
    if (rand()%100 <= MUTATION_RATE) mutate(&(*buffer)[i]);
  }
}

void swap(individual **pop, individual **buffer){
  int i;
  individual temp;
  for (i=0; i<POPULATION_SIZE; i++){
    (*pop)[i].fitness = (*buffer)[i].fitness;
    strcpy((*pop)[i].string, (*buffer)[i].string);
  }
}

int main(){
  int i;
  srand((unsigned)time(NULL));

  individual *pop = (individual*) malloc(sizeof(individual) * POPULATION_SIZE);
  individual *buffer = (individual*) malloc(sizeof(individual) * POPULATION_SIZE);

  init(&pop, &buffer);
  for (i=0; i<MAX_ITERATIONS; i++) {
    fitnessCalc(&pop);
    sort(&pop);
    printf("%d) BEST:  %s (fitness %d)\n", i+1, pop[0].string, pop[0].fitness);
    if (pop[0].fitness == 0) break;
    keepBest(&pop, &buffer);
    reproduce (&pop, &buffer);
    swap(&pop, &buffer);
  }

  return 0;
}
