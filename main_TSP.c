#include <stdio.h>
#include <stdlib.h>
#include <limits.h>

#include "microarray.h"


void free_sequences(DNAsequence** list_sequences, int num_sequences);


int main(int argc, char *argv[]){

  DNAsequence** sequences = NULL;
  DNAsequence* macrosequence = NULL;
  DNAsequence* macrosequence_aux = NULL;

  graph* g = NULL;
  char* chain_graph  = NULL;
  int* path = NULL;	

  int i = 0;
  int long_macrosequence_min = INT_MAX;

  if(argc!=4 || atoi(argv[1]) <= 0 || atoi(argv[2]) <= 0 || atoi(argv[3]) <= 0 || atoi(argv[3]) < atoi(argv[2])){
    printf("\n\n========== Error: Arguments ==========\n\n");
    printf("Para ejecutar: main_TSP num_sequences[>0] long_min[>0] long_max[>0]\n");
    return 0;
  }

  sequences = (DNAsequence**) malloc (atoi(argv[1])*sizeof(DNAsequence*));
  
  if(!sequences){
    printf("Error: memory\n");
    return 0;
  }

  for(i = 0; i < atoi(argv[1]); i++){
    sequences[i] = create_random_sequence(random_between(atoi(argv[2]), atoi(argv[3])));
    if(!sequences[i]){
      printf("Error: memory\n");			
      free_sequences(sequences, --i);
      return 0;
    }
  }

  printf("\n=== SEQUENCES TO OVERLAP ===\n");
  for(i = 0; i < atoi(argv[1]); i++){
    printf("\n");
    print_sequence(sequences[i]);
  }
	

  g = get_graph_TSPNN(sequences, atoi(argv[1]));
  if(!g){
    printf("Error: get_graph_TSPNN \n");	
    free_sequences(sequences, atoi(argv[1]));
    return 0;
  }

  chain_graph = graph_to_char(g);
  if(!chain_graph){
    printf(" Error: graph_to_char\n");
    free_sequences(sequences, atoi(argv[1]));
    free_graph(g);
    return 0;
  }
  printf("\n\n=== GRAPH ===\n\n");
  printf("%s\n", chain_graph);
  free(chain_graph);


  path = (int*) malloc((g->n_vertices) * sizeof(int));
  if(!path) {
    printf("Error: memory\n");
    free_sequences(sequences, atoi(argv[1]));
    free_graph(g);
    return 0;
  }

  printf("\n\n=== OBTAINED SEQUENCES===\n\n");
  for(i = 0; i < atoi(argv[1]); i++){	

    if(TSPNN(g, i, path) == 0){
      printf("Error: nop path TSP\n");
      if(macrosequence_aux){free_sequence(macrosequence_aux);}
      free_sequences(sequences, atoi(argv[1]));
      free_graph(g);
      free(path);			
      return 0;
    }

    /*Generamos la macrosecuencia a partir de dicho camino*/			
    macrosequence = generate_macro_sequence_DNA(sequences, atoi(argv[1]), path);
    if(!macrosequence){
      printf("Error: no path TSP\n");
      if(macrosequence_aux){free_sequence(macrosequence_aux);}
      free_sequences(sequences, atoi(argv[1]));
      free_graph(g);
      free(path);			
      return 0;
    }

    print_sequence(macrosequence); printf("\n");

    if(macrosequence->longitude < long_macrosequence_min){
      if(macrosequence_aux){free_sequence(macrosequence_aux);}
      macrosequence_aux = macrosequence;
      long_macrosequence_min = macrosequence->longitude;
      macrosequence = NULL;			
      
    }else{
      free_sequence(macrosequence);
    }
  }

  printf("\n\n=== FINAL SEQUENCE ===\n\n");
  print_sequence(macrosequence_aux);
  
  if(macrosequence_aux){free_sequence(macrosequence_aux);}
  free_sequences(sequences, atoi(argv[1]));
  free_graph(g);
  free(path);			
  return 1;
}



void free_sequences(DNAsequence** list_sequences, int num_sequences){
	
  int i = 0;

  if(!list_sequences || !(*list_sequences)){
    return;
  }
  for(i = 0; i < num_sequences; i++){
    if(list_sequences[i]){
      free_sequence(list_sequences[i]);
    }
  }
  free(list_sequences);
}
