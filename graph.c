#include "pq.h"
#include "graph.h"
#include <stdio.h>
#include <stdlib.h>
#include <limits.h>
#include <string.h>

graph *generate_graph(int *gr, int dir)
{
  graph *pgraph = NULL;
  int i = 0; 
  edge *pedge = NULL;


  if (gr == NULL ){
    printf("Error in generate_graph: null graph\n");
    return NULL;
  }

  if ((dir != DIRECTED) && (dir != NO_DIRECTED)){
    printf("Error in generate_graph: parameter dir\n");
    return NULL;
  }

  pgraph = (graph *) malloc(sizeof(graph));
  if (pgraph == NULL) {
    printf("Error in generate_graph: no memory\n");
    return NULL;
  }

  /* initialize */
  pgraph->n_vertices = gr[0];
  pgraph->n_edges = gr[1];

  if ((dir == DIRECTED) || (dir == NO_DIRECTED)){
    pgraph->directed = dir;
  }
  else {
    printf("Error input generate_graph: error dir value\n");
    free(pgraph);
    return NULL;
  }


  pgraph->vertices = (vertex *) malloc(sizeof(vertex) * pgraph->n_vertices);
  if (pgraph->vertices == NULL) {
    free(pgraph);
    printf("Error generate graph: no memory for vertices\n");
    return NULL;
  }

  for (i=0; i < gr[0]; i++){
    pgraph->vertices[i].edges = NULL;
    pgraph->vertices[i].id = i;
    pgraph->vertices[i].num_edges_out = 0;
    pgraph->vertices[i].num_edges_in = 0;
  }

  if (dir == DIRECTED){

    /* formamos la lista de adyacencia */
    for (i = 0; i < gr[1]; i++){

      if (pgraph->vertices[gr[3 * i + 2]].edges == NULL){

	/* if empty */
	if ((pgraph->vertices[gr[3 * i + 2]].edges = new_edge(3 * i + 2, EDGE_ONE, gr)) == NULL){
	  printf("Error generate_graph: error new_edge\n");
	  free(pgraph->vertices);
	  free(pgraph);
	  return NULL;
	}

	pgraph->vertices[gr[3 * i + 2]].num_edges_out++;

      } else {

	/* not mpty insert in first position */
	if((pedge = new_edge(3 * i + 2, EDGE_ONE, gr)) == NULL){
	  printf("Error generate_graph: error new_edge\n");
	  free(pgraph->vertices);
	  free(pgraph);
	  return NULL;
	}
	
	pedge->next = pgraph->vertices[gr[3 * i + 2]].edges;

	pgraph->vertices[gr[3 * i + 2]].edges = pedge;

	pgraph->vertices[gr[3 * i + 2]].num_edges_out++;
      }

    }

    for (i = 0;i < gr[0]; i++){
      pedge = pgraph->vertices[i].edges;
      while (pedge != NULL){
	pgraph->vertices[pedge->dest].num_edges_in++;
	pedge = pedge->next;
      }
    }
  } else { /* no rirected */

    for (i = 0; i < gr[1]; i++){

      if (pgraph->vertices[gr[3 * i + 2]].edges == NULL) {

	if((pgraph->vertices[gr[3 * i + 2]].edges = new_edge(3 * i + 2, EDGE_ONE, gr)) == NULL){
	  printf("Error generate_graph: error new_edge\n");
	  free(pgraph->vertices);
	  free(pgraph);
	  return NULL;
	}
	
      } else {

	if( (pedge = new_edge(3 * i + 2, EDGE_ONE, gr)) == NULL){
	  printf("Error generate_graph: error new_edge\n");
	  free(pgraph->vertices);
	  free(pgraph);
	  return NULL;
	}
	
	pedge->next = pgraph->vertices[gr[3 * i + 2]].edges;
	pgraph->vertices[gr[3 * i + 2]].edges = pedge;

      } /* else */ 

      if (pgraph->vertices[gr[3 * i + 3]].edges == NULL){

	if((pgraph->vertices[gr[3 * i + 3]].edges = new_edge(3 * i + 2, EDGE_TWO, gr)) == NULL){
	  printf("Error generate_graph: error new_edge\n");
	  free(pgraph->vertices);
	  free(pgraph);
	  return NULL;
	}
      } else {

	if((pedge = new_edge(3 * i + 2, EDGE_TWO, gr)) == NULL){
	   printf("Error generate_graph: error new_edge\n");
	   free(pgraph->vertices);
	   free(pgraph);
	   return NULL;
	}
	
	pedge->next = pgraph->vertices[gr[3 * i + 3]].edges;
	
	pgraph->vertices[gr[3 * i + 3]].edges = pedge;

      }

    }
  }

  return pgraph;
}

/************************************************************************/
/* Function: new_edge                                                   */
/* Input: int n_edge : edge position in array gr                        */
/*        int type: create one in edge or out edge                      */
/*        int *gr: integer array representing the graph                 */
/* Output: *edge                                                        */
/************************************************************************/
edge *new_edge(int n_edge, int type, int *gr){

  edge *pedge = NULL;

  pedge = (edge *) malloc (sizeof(edge));

  if (pedge == NULL){
    printf("Error new_edge: no memory\n");
    return NULL;
  }

  if (type == EDGE_ONE) {
    pedge->orig = gr[n_edge];
    pedge->dest = gr[n_edge + 1];
    pedge->weight = gr[n_edge + 2];
    pedge->next = NULL;
  } else if (type == EDGE_TWO) {
    pedge->dest = gr[n_edge];
    pedge->orig = gr[n_edge + 1];
    pedge->weight = gr[n_edge + 2];
    pedge->next = NULL;
  }
  return pedge;
}

/******************************************************/
/* Function: free_grah                                */
/*                                                    */
/******************************************************/

void free_graph(graph *g)
{
  int i = 0;
  edge *pedge = NULL;

  for (i = 0; i < g->n_vertices; i++){
    while (g->vertices[i].edges != NULL) {
      pedge = g->vertices[i].edges;
      g->vertices[i].edges = g->vertices[i].edges->next;
      free(pedge);
    }
  }
  free(g->vertices);
  free (g);
  return;
}

/********************************************************/
/* Funcion: graph_to_char                               */
/*                                                      */
/********************************************************/

char *graph_to_char (graph *g){

  char *str = NULL;
  int size = 0, i = 0, num_vertices = 0;
  int num_edges = 0, count = 0, count2 = 0;
  int block_edge = 0, num = 0;
  edge *pedge = NULL;

  if ( g == NULL ) {
    printf("Error: empty graph\n");
    return NULL;
  }

  /* get number of chars needed for the vertices*/
  num_vertices = g->n_vertices;
  while (num_vertices >= 1){
    num_vertices = num_vertices/10;
    count++;
  }

  /* same for edges */
  num_edges = g->n_edges;
  while (num_edges >= 1){
    num_edges = num_edges/10;
    count2++;
  }

  /* block edge = size_vert_orig + size_vert_dest + weight(=10) */
  block_edge = (2 * count) + 10;  
  
  size = ( g->n_edges * block_edge  ) + (count + count2);

  str = (char *) malloc (size * sizeof(char));
  
  /* insert first element in the main string */
  num = sprintf(str,"%d\n",g->n_vertices);
  
  num += sprintf(&(str[num]),"%d\n",g->n_edges);
   
  /* fill the graph*/
  for (i = 0 ; i < g->n_vertices; i++){
    pedge = g->vertices[i].edges;
    while (pedge != NULL){
      num += sprintf(&(str[num]),"%d ",pedge->orig);
      num += sprintf(&(str[num]),"%d ",pedge->dest);
      num += sprintf(&(str[num]),"%d\n",pedge->weight);

      pedge = pedge->next;
    }
  }

  return str;
}


graph *regular_graph(int n_vertices)
{
  int *res;
  int n_edges, i;
  graph *g;

  if (n_vertices < 0) return 0;

  n_edges = n_vertices < 5 ? n_vertices : 2 * n_vertices;

  res = malloc(sizeof(int)*(n_edges* 3 + 2));

  res[0] = n_vertices;
  res[1] = n_edges;

  for(i = 0; i < n_vertices; i++) {
    res[i* 3 + 2] = i;
    res[i* 3 + 3] = (i + 1)%n_vertices;
    res[i* 3 + 4] = 1 + (int) (100.0 * rand() / (RAND_MAX + 1.0));
  }

  for(i = 0; i < n_vertices; i++) {
    res[(i + n_vertices) * 3 + 2] = i;
    res[(i + n_vertices) * 3 + 3] = (i + 2) % n_vertices;
    res[(i + n_vertices) * 3 + 4] = 1 + (int) (100.0 * rand()/(RAND_MAX + 1.0));
  }

  g = generate_graph(res,DIRECTED);
  free(res);

  return g;
}


/********************************************************/
/* Function: dijkstra                                    */
/*                                                      */
/********************************************************/

int dijkstra(int start, int *distances, int *previous, graph *g)
{

  p_queue *pq = NULL;
  int *checked = NULL, num_vertices = 0, i = 0, count = 0;
  int vertex = 0, vertex2 = 0;
  element *pelement = NULL;
  edge *pedge = NULL;


  if ( g == NULL ) {
    printf("Error dijkstra: no graph\n");
    return 0;
  }

  num_vertices = g->n_vertices;

  /* Generate pq from the num of vertices*/
  if ((pq = generate_pq (num_vertices * 5)) == NULL ) {
    printf("Error dijkstra : error generate_pq\n");
    return 0;
  }

  /* array of visited nodes */
  checked = (int *) malloc (num_vertices * sizeof(int));
  if (checked == NULL){
    printf("Error dijkstra: no memory\n");
    free_pq(pq);
    return 0;
  }

  /* Initialize tables */
  for (i = 0; i < num_vertices; i++){
    checked[i] = NO_VISITED;
    distances[i] = INT_MAX/2;
    previous[i] = -1;
  }

  /* Inicializamos la distancia de fuente */
  distances[start] = 0;



  /* Metemos el vertice en la cola */
  if ( insert_element_pq(start,0,pq) == 0 ) {
    printf("Error dijkstra: error insert pq\n");
    free_pq(pq);
    free(checked);
    return 0;
  }

  /* aux element for management */
  pelement = (element *) malloc(sizeof(element));
  if (pelement == NULL){
    printf("Error dijkstra: no memory \n");
    free_pq(pq);
    free(checked);
    return 0;
  }


  count = 0;

  while ((empty_pq(pq) == 0) && (count < num_vertices) ) {

    if ( get_first_pq(pelement,pq) == 0){
      printf("Error dijkstra: error pop elemnt\n");
      free_pq(pq);
      free(checked);
      free (pelement);
      return 0;
    }

    vertex = (int) pelement->data;

    /* change vertex status */
    if (checked[vertex] == NO_VISITED){
      checked[vertex] = VISITED;
      count++;
    }

    pedge = g->vertices[vertex].edges;
    while (pedge != NULL){

      vertex2 = pedge->dest;
      if (distances[vertex2] > (distances[vertex] + pedge->weight)) {

	previous[vertex2] = vertex;
	distances[vertex2] = distances[vertex] + pedge->weight;

	if ( insert_element_pq (vertex2,distances[vertex2],pq) == 0){
	  printf("Error en dijkstra: insert pq\n");
	  free_pq(pq);
	  free(checked);
	  free (pelement);
	  return 0;
	}
      }

	pedge = pedge->next;

    }
  }

  /* check if graph is connected in previous*/
  count = 0;
  for (i = 0; i < num_vertices; i++){
    if (previous[i] == -1)
      count++;
  }

  if(count > 1){
    printf("Error dijkstr: graph no conected\n");
    free_pq(pq);
    free(checked);
    free (pelement);
    return 0;
  }
  
  free_pq(pq);
  free(checked);
  free (pelement);  

  return 1;
}


/***************************************************************/
/* Funstion: cmtp_floyd_warshall                               */
/*                                                             */
/***************************************************************/

int** cmtp_floyd_warshall(graph *g)

{
  int i = 0, j = 0, k = 0, num_vertices = 0;
  int **matrix=NULL;

  if (g == NULL){
    printf("Error F-W: no graph\n");
    return NULL;
  }
  
  num_vertices =  g->n_vertices;
  
  matrix = (int **) malloc (num_vertices * sizeof(int));
  if (matrix == NULL) {
    printf("Error F-W: no  memory\n");
    return NULL;
  }

  for (i = 0; i < num_vertices; i++){
    matrix[i] = (int*) malloc (num_vertices * sizeof (int));
    
    if (matrix[i] == NULL){
      printf("Error F-W: no memory\n");
      for(j = 0 ; j < i; j++)
	free(matrix[j]);
      free(matrix);
      return NULL;
    }
    
  }

  for (i = 0; i < num_vertices; i++){
    for (j = 0; j < num_vertices; j++){

      if (i == j)
	matrix [i][j]= 0; /* init distances */
      else
	if ((matrix[i][j] = get_cost(g, i, j)) == -2){
	  printf("Error F-W: error get?cost\n");
	  for (i = 0; i < num_vertices; i++){
	    free(matrix[i]);
	  } 
	  free(matrix);
	}
    }
  }

  for (k = 0; k < num_vertices; k++){
    for (j = 0; j < num_vertices; j++){
      for (i = 0; i < num_vertices; i++) {
	matrix[i][j] = min(matrix[i][j], matrix[i][k] + matrix[k][j]);
      }
    }
  }

  return matrix;

}

int get_cost (graph *g, int vertex1, int vertex2)
{
  edge *pedge = NULL;
  int cost = INT_MAX / 2;

  if ( g == NULL ) {
    printf("Error get_cost: no  graph\n");
    return -2;
  }

  if ((vertex1 > g->n_vertices) || (vertex2 > g->n_vertices)) {
    printf("Error get_cost: vertices dont exist\n");
    return -2;
  }

  pedge = g->vertices[vertex1].edges;
  while (pedge != NULL){
    if((pedge->orig == vertex1) && (pedge->dest == vertex2) )
      cost = pedge->weight;

    pedge = pedge->next;
  }


  return cost;

}

int min (int n1, int n2)
{

  if (n1 < n2)
    return n1;
  else
    return n2;
}


int** cmtp_dijkstra(graph *g)
{
  int **matrix = NULL;
  int num_vertices = 0, i = 0, j = 0;
  int *dist = NULL, *prev = NULL;

  if (g == NULL){
    printf("Error cmtp_dijkstra: no grah\n");
    return NULL;
  }

  dist = malloc(sizeof(int) * g->n_vertices);
  prev = malloc(sizeof(int) * g->n_vertices);
  if (prev == NULL || dist == NULL) {
    printf("Error cmtp_dikjstra:  memory\n");
    if (dist) free(dist);
    if (prev) free(prev);
    free_graph(g);
    return 0;
  }

  num_vertices =  g->n_vertices;
  
  matrix = (int **) malloc (num_vertices * sizeof(int));
  if (matrix == NULL) {
    printf("Error cmtp_dikjstra:  memory\n");
    free(prev);
    free(dist);
    return NULL;
    
  }

  for (i = 0; i < num_vertices; i++){
    matrix[i] = (int*) malloc (num_vertices * sizeof (int));
    if (matrix[i] == NULL ){
      printf("Error cmtp_dikjstra:  memory\n");
      for(j = 0 ; j < i; j++)
	free(matrix[j]);
      free(matrix);
      free(prev);
      free(dist);
      return NULL;
    }
    
  }

  for (i = 0; i < num_vertices; i++){
    if(dijkstra(i, dist, prev, g) == 0){
      printf("Error cmtp_dijkstra: error dijkstra\n");
      for (i = 0; i < num_vertices; i++){
	free(matrix[i]);
      } 
      free(matrix);
      free(dist);
      free(prev);
      return NULL;
    }
	

    for (j = 0; j < num_vertices; j++){
      if (i == j)
	matrix[i][j] = 0;
      else
	matrix[i][j] = dist[j];


    }
  }

  free(dist);
  free(prev);
  return matrix;

}

graph *generate_graph_dir(int n_vertices, int n_edges, int min_pond, int max_pond)
{
  int *gr, *outs, *dest;
  int w, p;
  int aux;
  int i, iv, irv;
  int inic, incr, limit_edges, iters;
  graph *g;

  if (n_edges > n_vertices * (n_vertices - 1) || n_vertices < 0 || n_edges < 0)
  {
      return 0;
  }

  gr = malloc(sizeof(int) * (3 * n_edges + 2));
  dest = malloc(sizeof(int) * (n_vertices - 1));
  outs = malloc(sizeof(int) * n_vertices);

  if (gr == 0 || dest == 0 || outs == 0) {
      if (gr) free(gr);
      if (dest) free(dest);
      if (outs) free(outs);
      return 0;
  }

  for (i = 0; i < n_vertices - 1; i++) {
    dest[i] = i;
  }

  if (2 * n_edges < n_vertices * (n_vertices - 1)) {
    inic = 0;
    incr = 1;
    limit_edges = n_vertices - 1;
    iters = n_edges;
  } else {
    inic = n_vertices - 1;
    incr = -1;
    limit_edges = 0;
    iters = n_vertices * (n_vertices-1) - n_edges;
  }

  for (i = 0; i < n_vertices; i++) {
    outs[i] = inic;
  }
  
  for (i = 0;i < iters; i++) {
     int a = random_between(0, n_vertices - 1);
     while (outs[a] == limit_edges) {
       a++;
       if (a == n_vertices) a = 0;
     }
     outs[a]+=incr;
  }


  gr[0] = n_vertices;
  gr[1] = n_edges;

  for (irv = 0, iv = 0; iv < n_vertices; iv++) {
    for (i = 0; i < outs[iv]; i++) {
      w = random_between(i, n_vertices - 2);
      aux = dest[w];
      dest[w] = dest[i];
      dest[i] = aux;
    }

    for (i = 0; i < outs[iv]; i++) {
      p = random_between(min_pond, max_pond);
      gr[2 + (irv + i) * 3] = iv;
      gr[3 + (irv + i) * 3] = dest[i]>=iv ? dest[i]+1 : dest[i];   
      gr[4 + (irv + i) * 3] = p;
    }
    irv += outs[iv];
  }

  free(outs);
  free(dest);

  g = generate_graph(gr, DIRECTED);
  free(gr);

  return g;
}


int random_between(int min, int max)
{
    double range = max - min + 1;
    return min + (int)(range * rand() / (RAND_MAX + 1.0));
}


graph *read_graph(char *file_name)
{
  FILE *f;
  int *gr;
  int v, w, p;
  int i, n_vertices, n_edges, res;
  graph *g;

  f = fopen(file_name, "rt");
  if (f == NULL) {
      return 0;
  }

  res = fscanf(f, "%d \n", &n_vertices);
  res += fscanf(f, "%d\n", &n_edges);
  if (res != 2 || n_edges > n_vertices * (n_vertices - 1) || n_vertices < 0 || n_edges < 0)
  {
      fclose(f);
      return 0;
  }

  gr = malloc(sizeof(int) * (3 * n_edges + 2));
  if (gr == 0) {
      fclose(f);
      return 0;
  }

  gr[0] = n_vertices;
  gr[1] = n_edges;

  for (i = 0; i < n_edges; i++) {
      res = fscanf(f, "%d %d %d \n", &v, &w, &p);
      if (res != 3) {
          free(gr);
          fclose(f);
          return 0;
      }
      gr[2 + i * 3] = v;
      gr[2 + i * 3 + 1] = w;
      gr[2 + i * 3 + 2] = p;
  }

  g = generate_graph(gr, DIRECTED);

  free(gr);
  fclose(f);

  return g;
}


int TSPNN(graph* g, int origin, int* path)
{
    int i, *visited = NULL, v, w;
    edge *r_NN = NULL;

    if ((g == NULL) || (origin < 0) || (origin >= g->n_vertices) || (path == NULL)){
        printf("Error args TSPNN\n");
        return PATH_NOT_FOUND;
    }

    visited = (int*) malloc (g->n_vertices * sizeof(int));
    if (visited == NULL){
        printf("Error TSPNN memory\n");
        return PATH_NOT_FOUND;
    }

    for (i = 0; i < g->n_vertices; i++)
      visited[i] = NO_VISITED;

    visited[origin] = VISITED;

    path[0] = origin;

    w = origin;

    for (i = 1; i < g->n_vertices; i++){
        r_NN = get_vertex_NN(w, g, visited);
        if (r_NN == NULL){
            printf("Error get_vertex_NN\n");
            free (visited); visited = NULL;
            return PATH_NOT_FOUND;
        }
        v = r_NN->dest;

        visited[v] = VISITED;

        path[i]= v;

        w = v;
    }

    free(visited); visited = NULL;

    return PATH_FOUND;
}

edge* get_vertex_NN(int origin, graph *g, int *visited)
{
    edge *r = NULL, *r_NN = NULL;

    if ((g == NULL) || (origin < 0) || (origin >= g->n_vertices) || (visited == NULL)){
        printf("Error args obtener_vertice_NN\n");
        return NULL;
    }

    r = g->vertices[origin].edges;

    r_NN = r;

    while (r != NULL){
      if ((r->weight < r_NN->weight) && (visited[r->dest] != VISITED))
            r_NN = r;
      r = r->next;
    }

    return r_NN;
}


int find_path_euler(graph* g, int* path)

{

  int i = 0, vert_ini = 0;
  
  if (g == NULL){
    printf("Error find_path_euler: args\n");
    return 0;
  }

  if (path == NULL){
    printf("Error find_path_euler: args\n");
    return 0;
  }

  for (i = 0; i < g->n_vertices; i++){
    if ( g->vertices[i].num_edges_out >  g->vertices[i].num_edges_in){
      vert_ini = i;
      path[0] = i;
    }
  }

  if (BP_CE(g, vert_ini, path,1) == 1){
    return 1;
  }
  else
    return 0;
}

int BP_CE (graph *g, int vertex, int *path, int position)
{
  int count = 0;
  edge *edge_aux = NULL;

  if (g == NULL) {
      printf("Error BP: args\n");
      return -1;
  }

  if (path == NULL) {
      printf("Error BP: args\n");
      return -1;
  }
  
  if (g->vertices[vertex].num_edges_out == 0){
    path[position] = vertex;
    if (check_path(g, path, position) == 1){
      return 1;
    } else
      return 0;
    
  } else {
    while (count != g->vertices[vertex].num_edges_out) {
      edge_aux = (g->vertices[vertex]).edges;
      if ( g->vertices[edge_aux->dest].num_edges_in == 0)
	edge_aux = (g->vertices[vertex].edges)->next;
      
      if (edge_aux == NULL){
	return 0;
	
      } else {
	g->vertices[vertex].num_edges_out--;
	g->vertices[edge_aux->dest].num_edges_in--;
	path[position] = edge_aux->dest;
	position++;
	count++;
	
	if ( BP_CE(g, edge_aux->dest, path, position) == 1)
	  return 1;
      }
    }

      return 0;
    }
}

int check_path(graph *g, int* path, int position)
{
  int i = 0,control;
  
  if (g == NULL)  {
    printf ("Error check_path: args\n");
    return -1;
  }

  if (path == NULL) {
    printf ("Error check_path: args\n");
    return -1;
  }
	      
  for(i = 0;i < position; i++) {
      
    if ((g->vertices[path[i]].num_edges_in == 0) && (g->vertices[path[i]].num_edges_out == 0)) {
      control = 0;
    }else {
      control = 1;
      break;
    }
  }
  
  if (control == 0)
    return 1;
  else 
    return 0;
}
