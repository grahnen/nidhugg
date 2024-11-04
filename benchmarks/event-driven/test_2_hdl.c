#include <stdio.h>
#include <stdatomic.h>
#include <pthread.h>
#include <assert.h>
#include "qthread.h"

//N! traces
#ifndef N
#  warning "N was not defined; defining it as 5"
#  define N 5
#endif

qthread_t h1, h2;
atomic_int a;

void __VERIFIER_assume(intptr_t);

void r(void *_id) {
    atomic_int l = atomic_load_explicit(&a, memory_order_seq_cst);
}

void m1(void *_id){
    atomic_store_explicit(&a, 1, memory_order_seq_cst);
    qthread_post_event(h2, &r, NULL);
}

void m2(void *_id){
    atomic_store_explicit(&a, 2, memory_order_seq_cst);
    qthread_post_event(h1, &r, NULL);
}

void *handler_func(void *i){
  qthread_exec();
  return 0;
}

int main(){
    qthread_create(&h1, &handler_func, NULL);
    qthread_start(h1);
    qthread_create(&h2, &handler_func, NULL);
    qthread_start(h2);
  //for (int i = 0; i < N; i++){
    qthread_post_event(h1, &m1, NULL);
    qthread_post_event(h2, &m2, NULL);
  //}
}
