#ifndef __MICROARRAY_H__
#define __MICROARRAY_H__

#include "graph.h"

typedef struct {
	int longitude;		 
	char* nucleotids;
}DNAsequence;



typedef struct {
  int    longitude_mers;  
  int    num_mers;       
  char** l_mers;     	 
} spectrum;

char* get_overlap(DNAsequence* prior, DNAsequence* posterior);
DNAsequence* generate_macro_sequence_DNA(DNAsequence** sequences, int num_sequences, int* path);
spectrum* generate_spectrum(DNAsequence* sequence, int longitude_mers);
graph* get_graph_TSPNN(DNAsequence** sequences, int num_sequences);
void free_spectrum(spectrum* espec);
graph* create_euler_graph (spectrum* spec,  spectrum** spec_reduced);
DNAsequence* recover_sequence_DNA(spectrum* spec_reduced, int* path, int longitude_path);
DNAsequence* create_random_sequence(int long_sequence);
void free_sequence(DNAsequence* sequence);
void print_sequence(DNAsequence* sequence);
void print_spectrum (spectrum * spec);

#endif
