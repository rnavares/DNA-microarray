/**
 * File : pq.h                         
 * Ricardo Navares
 *
 *  Priority Queues prototypes
 *
 */

#ifndef __PQ_H__
#define __PQ_H__

typedef struct {  
  int   weight;     
  int data;    
} element;

typedef struct {  
  int size;  
  int index;     
  element *elements; 
} p_queue;

p_queue *generate_pq(int n_elements);
void free_pq(p_queue *pq);
int empty_pq(p_queue *pq);
int insert_element_pq(int data, int weight, p_queue *pq);
int get_first_pq(element *el, p_queue *pq);

#endif



