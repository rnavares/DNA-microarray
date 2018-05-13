#include "microarray.h"
#include <stdio.h>
#include <stdlib.h>
#include <time.h>


void print_path(int node, int *dist, int *prev);

int main(int argc, char *argv[]){

  DNAsequence* sequence = NULL;
  DNAsequence* sequence_recovered = NULL;
  spectrum* spec = NULL;
  spectrum* spec_reduced = NULL;
  graph* g = NULL;
  char* chain_graph = NULL;
  int* path = NULL;
  int i = 0;

  /*Input sequence and spectrum size*/
  if(argc!=3){
    printf("\n\n========== Error: Arguments ==========\n\n");
    printf("Execution: >main long_sequence long_spectrum\n");
    return 0;
  }

  sequence = create_random_sequence(atoi(argv[1]));
  if(!sequence){
    printf("Error: create_random_sequence\n");
    return 0;
  }

  print_sequence(sequence);

  spec = generate_spectrum(sequence, atoi(argv[2]));
  if(!spec){
    printf("Error: generate_sectrum\n");
    free_sequence(sequence);
    return 0;
  }

  print_spectrum(spec);

  g = create_euler_graph(spec, &spec_reduced);
  if(!g){
    printf("Error:create_euler_graph\n");
    free_sequence(sequence);
    free_spectrum(spec);
    return 0;
  }

  print_spectrum(spec_reduced);

  chain_graph = graph_to_char(g);
  if(!chain_graph){
    printf("Error:  graph_to_char\n");
    free_sequence(sequence);
    free_spectrum(spec);
    free_spectrum(spec_reduced);
    free_graph(g);
    return 0;
  }

  printf("\n=== GRAPH ===\n");
  printf("%s\n", chain_graph);
  free(chain_graph);


  path = (int*) malloc ((g->n_edges + 1) * sizeof(int));
  if(!path) {
    printf("Error: memory\n");
    free_sequence(sequence);
    free_spectrum(spec);
    free_spectrum(spec_reduced);
    free_graph(g);
    return 0;
  }

  if(!find_path_euler(g, path)){
    printf("Error: No euler path found\n");
    free_sequence(sequence);
    free_spectrum(spec);
    free_spectrum(spec_reduced);
    free_graph(g);
    free(path);
    return 0;
  }

  printf("\n=== EULER PATH ===\n");
  for(i = 0; i < g->n_edges; i++){
    printf(" %d-> ", path[i]);
  }
  printf(" %d\n\n", path[g->n_edges]);
  

  sequence_recovered = recover_sequence_DNA(spec_reduced, path, (g->n_edges + 1));
  if(!sequence_recovered){
    printf(" Error: cannot recover DNA sequence\n");
    free_sequence(sequence);
    free_spectrum(spec);
    free_spectrum(spec_reduced);
    free_graph(g);
    free(path);
    return 0;
  }

  print_sequence(sequence_recovered);

  free_sequence(sequence);
  free_sequence(sequence_recovered);
  free_spectrum(spec);
  free_spectrum(spec_reduced);
  free_graph(g);
  free(path);

  /* DNAsequence *sequence, **sequences;
  spectrum *spec;
  int i = 0, j = 0;
  graph *g;
  */

  /* http://faculty.samford.edu/~sfdonald/Courses/cosc407/Papers/Computing_with_DNA%20(Adleman).pdf */
  /* sequence = create_random_sequence(20);
  print_sequence(sequence);

  spec = generate_spectrum(sequence, 5);
  print_spectrum(spec);

  sequences = (DNAsequence **) malloc (spec->num_mers * sizeof(DNAsequence*));
  if(sequences == NULL){
    printf("No memory");
    return 0;
  }

  for (i = 0; i < spec->num_mers; i++){
    sequences[i] = (DNAsequence *) malloc (sizeof(DNAsequence));
    if(sequences[i] == NULL){
      for(j = 0; j < i; j++){
	free(sequences[j]);
      }
      free(sequences);
      return 0;
       
    }
  }

  for (i = 0; i < spec->num_mers; i++){
    sequences[i]->longitude = spec->longitude_mers;
    sequences[i]->nucleotids = spec->l_mers[i];
  }
 

  g = get_graph_TSPNN(sequences, spec->num_mers);
  
  printf("mer: %s\n",sequences[1]->nucleotids);
    
  free_sequence(sequence);
  for(j = 0; j < spec->num_mers; j++){
    free(sequences[j]);
  }
  free(sequences);
  free_spectrum(spec);  
  */
  
  return 1;
  
}


void print_path(int node, int *dist, int *prev)
{
  printf("v%d", node);
  if (prev[node] == NO_VISITED) {
    printf("No visited");
  }
  while (prev[node] != -1) {
    printf("[d=%d]->v%d", dist[node], prev[node]);
    node = prev[node];
  }
  printf("\n");
}
