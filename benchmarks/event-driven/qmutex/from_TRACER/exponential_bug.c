/* Copyright (C) 2020 Magnus Lång, 2018 Tuan Phong Ngo
 * This benchmark is part of Nidhugg
 */

/* Example in Figure 2 in the PLDI 2015 paper: 
   https://dl.acm.org/citation.cfm?id=2737975
*/

#include <assert.h>
#include <stdint.h>
#include <stdatomic.h>
#include <pthread.h>
#include "../inc/qmutex.h"

#ifndef N
#  warning "N was not defined, assuming 3"
#  define N 3
#endif

// shared variables
atomic_int x, y;
qmutex lock;

static void *thr1cs(void *arg) {
  atomic_store_explicit(&x, 1, memory_order_seq_cst);
  for (int i=0; i<N; i++) {
    atomic_store_explicit(&y, 1, memory_order_seq_cst);
  }
  return arg;
}
void *thr1(void *arg)
{
  qmutex_delegate(&lock, thr1cs, arg);

  qmutex_delegate(&lock, thr1cs, arg);

  return NULL;
}

static void *thr2cs(void *arg) {
  atomic_store_explicit(&x, 0, memory_order_seq_cst);
  return arg;
}
void *thr2(void *arg)
{
  int _y;

  qmutex_delegate(&lock, thr2cs, arg);

  if (atomic_load_explicit(&x, memory_order_seq_cst) > 0) {
    _y = atomic_load_explicit(&y, memory_order_seq_cst);
    atomic_store_explicit(&y, _y+1, memory_order_seq_cst);
    atomic_store_explicit(&x, 2, memory_order_seq_cst);
  }

  qmutex_delegate(&lock, thr2cs, arg);

  if (atomic_load_explicit(&x, memory_order_seq_cst) > 0) {
    _y = atomic_load_explicit(&y, memory_order_seq_cst);
    atomic_store_explicit(&y, _y+1, memory_order_seq_cst);
    atomic_store_explicit(&x, 2, memory_order_seq_cst);
  }

  return NULL;
}


void *thr3(void *arg)
{
	
	if (atomic_load_explicit(&x, memory_order_seq_cst) > 1) {
		if (atomic_load_explicit(&y, memory_order_seq_cst) == 3) {
			//assert(0);			
			return NULL;
		} else {
			atomic_store_explicit(&y, 2, memory_order_seq_cst);
		}
	}

	if (atomic_load_explicit(&x, memory_order_seq_cst) > 1) {
		if (atomic_load_explicit(&y, memory_order_seq_cst) == 3) {
			//assert(0);
			return NULL;
		} else {
			atomic_store_explicit(&y, 2, memory_order_seq_cst);
		}
	}

	return NULL;
}

int main(int argc, char *argv[])
{

	pthread_t t1, t2, t3;
	
	atomic_init(&x, 0);
	atomic_init(&y, 0);
	
    qmutex_init (&lock);

	pthread_create(&t1, NULL, thr1, NULL);
	pthread_create(&t2, NULL, thr2, NULL);
	pthread_create(&t3, NULL, thr3, NULL);

	return 0;
}
