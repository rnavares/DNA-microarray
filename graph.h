/**
* Fichero : graph.h     
* Ricardo Navares
*
*  Structures and prototypeds for graph
*
*/

#ifndef __GRAPH_H__
#define __GRAPH_H__

/* constants */
#define VISITED 1
#define NO_VISITED 0
#define EDGE_ONE 1
#define EDGE_TWO 2
#define DIRECTED 1
#define NO_DIRECTED 0
#define PATH_NOT_FOUND 0
#define PATH_FOUND 1

/* graph structures */
typedef struct _edge {
  int orig;          
  int dest;          
  int weight;        
  struct _edge *next; /* next edge */
} edge;

typedef struct {
  int  id;	
  int  num_edges_out; 
  int  num_edges_in;
  edge *edges;	
} vertex;

typedef struct {
  int n_vertices;    
  int n_edges;       
  int directed;      
  vertex *vertices;  
} graph;

graph *generate_graph(int *gr, int dir);
edge *new_edge(int n_edge, int type, int *gr);
void free_graph(graph *g);
char *graph_to_char (graph *g);
graph *regular_graph(int n_vertices);
int dijkstra(int start, int *distances, int *previous, graph *g);
int** cmtp_floyd_warshall(graph *g);
int get_cost (graph *g, int vertex1, int vertex2);
int min (int n1, int n2);
int** cmtp_dijkstra(graph *g);
graph *generate_graph_dir(int n_vertices, int n_edgess, int min_pond, int max_pond);
int random_between(int min, int max);
graph *read_graph(char *file_name);
int TSPNN(graph* g, int origin, int* path);
edge* get_vertex_NN(int origin, graph *g, int *visited);
int find_path_euler(graph* g, int* path);
int BP_CE (graph *g, int vertex, int *path, int position);
int check_path(graph *g, int* path, int position);

#endif
