#include "microarray.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

char* get_overlap(DNAsequence* prior, DNAsequence* posterior)

{
  int i = 0, longitude = 0, longitude2 = 0;
  int longitude_overlap = 0, longitude_aux = 0;
  char *seq1 = NULL, *seq2 = NULL, *overlap = NULL;

  
  if ((prior == NULL) || (posterior == NULL)) {
      printf("Error get_overlap: incorrect args\n");
      return NULL;
  }

  if(prior->longitude < posterior->longitude){
      longitude = prior->longitude;
  } else {
      longitude = posterior->longitude;
  }

  longitude2 = prior->longitude;

  seq1 = (char *) malloc (longitude * sizeof(char));
  if (seq1 == NULL) {
      printf("Error get_overlap: no memory\n");
      return NULL;
  }

  seq2 = (char *) malloc (longitude * sizeof(char));
  if (seq2 == NULL) {
      printf("Error get_overlap: no memory\n");
      free(seq1);
      return NULL;
  }  

  while(longitude_aux < longitude){
    for (i = 0; i < longitude_aux; i++){
      seq1[i] = prior->nucleotids[(longitude2 - longitude_aux) + i];
      seq2[i] = posterior->nucleotids[i];
    }
      
    if (strcmp(seq1,seq2) == 0){
      longitude_overlap = longitude_aux;
    }
      
    longitude_aux++;
     
  }

  overlap = (char *) malloc (longitude_overlap * sizeof(char));
  if (overlap == NULL){
    printf("Error get_overlap: no memory\n");
    free(seq1);
    free(seq2);
    return NULL;
  }  
  
  for(i = 0; i < longitude_overlap; i++){
      overlap[i] = posterior->nucleotids[i];
  }

  return overlap;
}


DNAsequence* generate_macro_sequence_DNA(DNAsequence** sequences, int num_sequences, int* path)
{
  int i = 0, j = 0, count = 0;
  char *overlap;
  DNAsequence* macrosec = NULL;

  if (sequences == NULL){
    printf("Error generate_macro_sequence_DNAa: arg no valid\n");
     return NULL;
  }

  if (path == NULL){
    printf("Error generate_macro_sequence_DNA: arg no valid\n");
    return NULL;
  }

  if (num_sequences <= 0){
    printf("Error generate_macro_sequence_DNA: arg no valid\n");
    return NULL;
  }
  
  macrosec = (DNAsequence *) malloc (sizeof(DNAsequence));
  if(macrosec == NULL) {
    printf("Error generate_macro_sequence_DNA: no memory\n");
    return NULL;
  }

  macrosec->nucleotids = (char *) malloc (num_sequences * 20 * sizeof(char));
  if (macrosec->nucleotids == NULL){
    printf("Error generate_macro_sequence_DNA: no memory\n");
    free(macrosec);
    return NULL;
  }

  for(i = 0; i < sequences[0]->longitude; i++){
    macrosec->nucleotids[count] = sequences[0]->nucleotids[i];
    count++;
  }

  for (i = 1; i < num_sequences; i++){
    overlap = get_overlap(sequences[i-1],sequences[i]);

    for(j = strlen(overlap); j < sequences[i]->longitude; j++){
      macrosec->nucleotids[count] = sequences[i]->nucleotids[j];
      count++;
    }
  }

  macrosec->longitude = count;

  return macrosec;
  
}


spectrum* generate_spectrum(DNAsequence* sequence, int longitude_mers)
{
  
  int i = 0, j = 0, rand_num = 0;
  spectrum* spec = NULL;
  char *paux;

  if (sequence == NULL ) {
    printf ("Error generate_spectrum: no valid arg\n");
    return NULL;
  }

  if (longitude_mers < 0) {
    printf ("Error generate_spectrum: no valid arg\n");
    return NULL;
  }

  spec = (spectrum *) malloc (sizeof(spectrum));
  if (spec == NULL){
    printf("Error generate_spectrum: no memory\n");
    return NULL;
  }
  
  spec->num_mers = sequence->longitude - (longitude_mers - 1);
  spec->longitude_mers = longitude_mers; 
  
  spec->l_mers = (char **) malloc ( 20 * spec->num_mers * sizeof(char));
  if (spec->l_mers == NULL){
    printf("Error get_spectrum: no memoy\n");
    if (spec) free(spec);
      return NULL;
  }
  
  for (i = 0; i < spec->num_mers; i++){
    spec->l_mers[i] = (char *) malloc (longitude_mers * sizeof (char));
    if (spec->l_mers[i] == NULL){
      printf("Error get_spectrum: no memory\n");

      for (j = 0; j < i; j++) free(spec->l_mers[j]);
      if (spec->l_mers) free (spec->l_mers);
      if (spec) free(spec);
      return NULL;
    }
  }
  
  for (i = 0; i < spec->num_mers; i++){
    for (j = 0; j < longitude_mers; j++){
      spec->l_mers[i][j] = sequence->nucleotids[j+i];
    }
  }
  
  /* randomize seq */
  for (i = 0; i < spec->num_mers; i++){
    rand_num = random_between(i, spec->num_mers - 1);
    paux = spec->l_mers[i];
    spec->l_mers[i] = spec->l_mers[rand_num];
    spec->l_mers[rand_num] = paux;
    /*swap(&cadena->l_meros[i], cadena->l_meros[num_aleat]);*/
  }

  return spec;
}


graph* get_graph_TSPNN(DNAsequence** sequences, int num_sequences)
{
    graph *g = NULL;
    char *overlap = NULL;
    int *int_gr = NULL, i, j, position = 0;

    if ((sequences == NULL) || (num_sequences < 1)){
        printf("Error get_graph_TSPNN: args\n");
        return NULL;
    }

    int_gr = (int*) malloc ((2 + 3 * num_sequences * (num_sequences - 1)) * sizeof(int));
    if (int_gr == NULL){
      printf("Error get_graph_TSPNN: memory\n");
      return NULL;
    }

    int_gr[position++] = num_sequences;
    int_gr[position++] = num_sequences * (num_sequences - 1);

    /* Para cada secuencia */
    for (i = 0; i < num_sequences; i++){
        for (j = 0; j < num_sequences; j++)
            if (j != i){
                /* Origin */
                int_gr[position++] = i;

                /* Dest */
                int_gr[position++] = j;

                overlap = get_overlap(sequences[i], sequences[j]);
                if (overlap == NULL){
                    printf("Error get_graph_TSPNN: get_overlap\n");
                    free(int_gr); int_gr = NULL;
                    return NULL;
                }

		/* edge weight */
		int_gr[position++] = sequences[i]->longitude - strlen(overlap);
            }
    }

    g = generate_graph(int_gr, DIRECTED);
    if (g == NULL){
        printf("Error get_graph_TSPNN: generate_graph\n");
        free(int_gr); int_gr = NULL;
        return NULL;
    }

    free(int_gr); int_gr = NULL;

    return g;
}

void free_spectrum(spectrum* spec)
{
  int j = 0;
     
  for (j = 0; j < spec->num_mers; j++){
       free(spec->l_mers[j]);
  }

  if (spec->l_mers) free (spec->l_mers);
  if (spec) free(spec);

  return;
}


graph* create_euler_graph (spectrum* spec,  spectrum** spec_reduced)
{
  int i = 0, j = 0, k = 0, l = 0, count = 0;
  int repeated = 0, overlap1 = 0, overlap2 = 0;
  int origin = 0, dest = 0;
  char *mer;
  int num_edges = 0, *chain = NULL;
  graph *gr;

  if (spec == NULL){
    printf("Error  create_euler_graph: no spectrum\n");
    return NULL;
  }
  
  
  (* spec_reduced) = (spectrum *) malloc (sizeof (spectrum));
  
  if (spec_reduced == NULL){
    printf("Error create_euler_graph: no spectrum\n");
    free (spec);
    return NULL;
  }
  
  (*spec_reduced)->longitude_mers = spec->longitude_mers - 1;
    
  (*spec_reduced)->l_mers = (char **) malloc (20 * spec->num_mers * sizeof(char));
  if ((*spec_reduced)->l_mers == NULL){
      printf("Error create_euler_graph: no memory\n");
      if (spec_reduced) free(spec_reduced);
      return NULL;
  }
  
  for (i = 0; i < (2 * spec->num_mers) - 1; i++){
      (*spec_reduced)->l_mers[i] = (char *) malloc ((*spec_reduced)->longitude_mers * sizeof (char));
      if ((*spec_reduced)->l_mers[i] == NULL)
	{
	   printf("Error  create_euler_graph: no memory\n");
	   for (j = 0; j < i; j++)
	     free((*spec_reduced)->l_mers[j]);
	   
	   if ((*spec_reduced)->l_mers) free ((*spec_reduced)->l_mers);
	   if (spec_reduced) free(spec_reduced);
	   return NULL;
	}
   }
  

  mer = (char *) malloc ((spec->longitude_mers-1) * sizeof(char));
  if (mer == NULL ){
      printf ("Error create_euler_graph: no memory\n");
      return NULL;
  }


  for(i = 0; i < spec->num_mers; i++){
    for (j = 0; j < (*spec_reduced)->longitude_mers; j++){
      mer[j] = spec->l_mers[i][j];
    }

    for (k = 0; k < count ; k++){
      if (strcmp(mer,(*spec_reduced)->l_mers[k]) == 0) {
	repeated = 1;
      }
		
    }

    if (repeated == 0){
      for (l = 0; l < (*spec_reduced)->longitude_mers; l++)
	(*spec_reduced)->l_mers[count][l] = mer[l];

      count++;
      (*spec_reduced)->num_mers++;
    }
      
    repeated = 0;

    for (j = 0; j < (*spec_reduced)->longitude_mers; j++){
      mer[j] = spec->l_mers[i][j+1];
    }

    for (k = 0; k < count ;k++)	{
      if (strcmp(mer,(*spec_reduced)->l_mers[k]) == 0){
	repeated = 1; 
      }
    }
      
    if (repeated == 0){
      for (l = 0; l < (*spec_reduced)->longitude_mers; l++)
	(*spec_reduced)->l_mers[count][l] = mer[l];
	 
      count++;
      (*spec_reduced)->num_mers++;
	  
	  
    }
      
    repeated = 0;
  }

  chain = (int *) malloc ( ((*spec_reduced)->num_mers *(*spec_reduced)->num_mers) * sizeof(int));
  if(chain == NULL){
    printf("Error create_euler_graph: no memory\n");
    free(mer);
    return NULL;
  }

  chain[0] = (*spec_reduced)->num_mers;
     
  for(i = 0; i < spec->num_mers; i++){
      for (j = 0; j < (*spec_reduced)->num_mers; j++){
	  for(k = 0; k < (*spec_reduced)->longitude_mers;k++){
	    mer[k] = spec->l_mers[i][k];
	  }

	  if (strcmp(mer,(*spec_reduced)->l_mers[j]) == 0){
	      overlap1++;
	      origin = j;
	  }

	  for(k = 1; k < spec->longitude_mers; k++)
	    mer[k-1] = spec->l_mers[i][k];

	  if (strcmp(mer,(*spec_reduced)->l_mers[j]) == 0){
	      overlap2++;
	      dest = j;
	  }
	  
	  if ((overlap1 + overlap2) == 2){
	      chain[3 * num_edges + 2] = origin;
	      chain[3 * num_edges + 3] = dest;
	      chain[3 * num_edges + 4] = 1;
	      num_edges++;
	      chain[1] = num_edges;
	      break;
	  }
	  
	  

      }
      
      overlap1 = 0;
      overlap2 = 0;
  }

  gr = generate_graph(chain,DIRECTED);
  if (gr == NULL){
      printf("Error crea_grafo_eule: error generate _graph\n");
      free(chain);
      free(mer);
      return NULL;
  }

  if (mer) free(mer);
  if (chain) free(chain);

  return gr;
  
}


DNAsequence* recover_sequence_DNA(spectrum* spec_reduced, int* path, int longitude_path)
{
  int i = 0, count = 0;
  DNAsequence* sequence = NULL;
 

  if (spec_reduced == NULL) {
      printf("Error recover_seuence_DNA: arg\n");
      return NULL;
  }

  if (path == NULL) {
      printf("Error recover_seuence_DNA: args\n");
      return NULL;
  }

  if (longitude_path < 0){
      printf("Error recover_seuence_DNA: args\n");
      return NULL;
  }

  sequence = (DNAsequence*) malloc (sizeof (DNAsequence));
  if (sequence == NULL) {
      printf ("Error recover_seuence_DNA: no memory\n");
      return NULL;
   }

  sequence->longitude = spec_reduced->longitude_mers + longitude_path - 1; 
  sequence->nucleotids = (char *) malloc (sequence->longitude * sizeof (char));
  if (sequence->nucleotids == NULL){
      printf ("Error recover_seuence_DNA: no memory\n");
      if (sequence) free(sequence);
      return NULL;
   }

   
  for(i = 0; i < spec_reduced->longitude_mers; i++){
    sequence->nucleotids[i] = spec_reduced->l_mers[path[0]][i];
  }

  count = 1;
  for (i = spec_reduced->longitude_mers; i < sequence->longitude;i++){
    sequence->nucleotids[i] = spec_reduced->l_mers[path[count]][spec_reduced->longitude_mers - 1];
    count++;
  }

  return sequence;
}


DNAsequence* create_random_sequence(int long_sequence)
{
  int i = 0;
  DNAsequence* sequence = NULL;
  char nucleotids[4] = {'A', 'G', 'T', 'C'};

  if(long_sequence <= 0){
    printf("Error create_random_sequence: args ");
    return NULL;
  }
	
  sequence = (DNAsequence*) malloc(sizeof(DNAsequence));
  if(!sequence){
    printf("Error create_random_sequence: memory ");
    return NULL;
  }

  sequence->longitude = long_sequence;

  sequence->nucleotids = (char*)calloc(2 * (sequence->longitude + 1), sizeof(char));
  if(!sequence->nucleotids){
     printf("Error create_random_sequence: args ");
     return NULL;
  }

  for(i = 0; i < long_sequence; i++){
    sequence->nucleotids[i] = nucleotids[random_between(0,3)];	
  }

  return sequence;
}

void free_sequence(DNAsequence* sequence)
{
  if(!sequence){
    return;
  }
  if(!sequence->nucleotids){
    free(sequence);
    return;
  }
  free(sequence->nucleotids);
  free(sequence);
}


void print_sequence(DNAsequence* sequence)
{

  if(!sequence || !sequence->nucleotids || sequence->longitude <= 0){
    printf("Error print_sequence: args");
    return;
  }

  printf("=== SEQUENCE ===\n");
  printf("long: %d\t data:%s\n", sequence->longitude, sequence->nucleotids);
}


void print_spectrum (spectrum * spec)
{
  int i = 0;

  if(!spec || spec->longitude_mers < 0 || spec->num_mers < 0 || !spec->l_mers){
    printf("Error print_spectrum: args\n");
    return;
  }

  printf("=== SPECTRUM ===\n");
  printf("Num_mers:  %d\n", spec->num_mers);
  printf("long_mers: %d\n", spec->longitude_mers);

  for(i = 0; i < spec->num_mers; i++){
    printf("%s\n", spec->l_mers[i]);
  }
	
}


