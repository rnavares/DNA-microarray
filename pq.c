#include "pq.h"
#include <stdlib.h>
#include <limits.h>


p_queue *generate_pq(int n_elements)
{
  p_queue *pq;

  pq = malloc(sizeof(p_queue));
  if (!pq) return 0;

  pq->elements = malloc(sizeof(element) * (n_elements + 1));
  if (!pq->elements) {
    return 0;
  }

  pq->size = n_elements;
  pq->index = 0;
  pq->elements[0].weight = -INT_MAX;
  pq->elements[0].data = 0;

  return pq;
}

void free_pq(p_queue *pq)
{
  free(pq->elements);
  free(pq);
}

int empty_pq(p_queue *pq)
{
  return pq->index == 0;
}

int insert_element_pq(int data, int weight, p_queue *pq)
{
  int idx;

  if (pq->size<pq->index + 1)
  {
    return 0; 
  }

  pq->index++;
  idx = pq->index;
  while(pq->elements[idx/2].weight >= weight) {
      pq->elements[idx] = pq->elements[idx/2];
      idx = idx/2;
  }

  pq->elements[idx].data = data;
  pq->elements[idx].weight  = weight;
 
  return 1;
}

int get_first_pq(element *el, p_queue *pq)
{
  int idx;
  int least, last;

  if (empty_pq(pq)){
    return 0; 
  }

  (*el) = pq->elements[1];

  idx = 1;
  last = pq->index;
  pq->index--;
  while(idx*2 <= pq->index) {
      least = idx*2;
      if (least < pq->index) {
        if (pq->elements[idx*2].weight > pq->elements[idx*2+1].weight) {
            least++;
        }      
      }
      if (pq->elements[least].weight<pq->elements[last].weight)
      {
          pq->elements[idx] = pq->elements[least];
          idx = least;
      }
      else {
          break;
      }
  }
  pq->elements[idx] = pq->elements[last];

  return 1;
}



