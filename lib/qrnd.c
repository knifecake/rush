/*------------------------------------------------------------------
   Project:  General C utilities
   File:     qrnd.h
   Rev.      2.0
   Date:     Apr. 26, 2011

   (C) Simone Santini, 2011

   Revisions
   1.0       Mar. 3, 2011
       Initial version
   2.0       Apr. 26, 2011
       Added a new generator that generates the numbers [0,...n-1]
       once each without repetitions.


   This series of functions implement several pseudo-random number
   generators. All these generators are based on the same generator
   of pseudo-random integer numbers in the range 0 to 2^32-1, called
   i_rnd

   Based on this, two generators are built: the first (f_rnd) generates
   double precision numbers in the range [0,1], the second (n_rnd) 
   generates numbers with a normal distribution (with mean 0 and unitary
   variance).

   In order to use these generators, it is necessary to initialize them
   with an integer "seed". The initialization function (r_init) will return
   a state, that will be used in the call to the other generators.
   It is posible to use the same state for intermingled calls to all the
   generators, or to use different states. That is, the following two
   fragments  of code are both legal:

   rnd_state *s = r_init(12345);
   for (i=0; i<100; i++) {
      long int a = i_rnd(s);
      double f = f_rnd(s);
      double n = n_rnd(s);
   }

   and

   rnd_state *si = r_init(12345);
   rnd_state *sf = r_init(23456);
   rnd_state *sn = r_init(77665);
   for (i=0; i<100; i++) {
      long int a = i_rnd(si);
      double f = f_rnd(sf);
      double n = n_rnd(sn);
   }

  -------------------------------------------------------------------
*/

#include "qrnd.h"
#include <stdlib.h>


#define DRY_RUNS  10

/*
  Private function: basic pseudo-random number generator
*/
static long _basic_rnd(rnd_state *s) {
  long k;
  k = s->seed/s->q;
  s->seed = s->a*(s->seed-k*s->q)-k*s->r;
  if (s->seed < 0) s->seed += s->m;
  return s->seed;
}



/*
  Inititlization function: given a seed initializes a state
  and returns a pointer to it.
*/
rnd_state *r_init(long seed) {
  int i;
  rnd_state *s = (rnd_state *) malloc(sizeof(rnd_state));
  
  s->a = 16807L;
  s->m = 2147483647L;
  s->q = 127773;
  s->r = 2836;
  
  s->mask = 123459876L;
  s->seed = seed;

  s->stored = 0;

  for (i=0; i<DRY_RUNS; i++)
    _basic_rnd(s);

  for (i=0; i<NSTORE; i++)
    s->store[i] = _basic_rnd(s);
  s->lstidx = s->store[0]*(NSTORE-1)/(s->m-1);

  return s;
}

/*
  Finalizes the use of the generator liberating the memoty used
  by the state
*/
void r_end(rnd_state *s) {
  free (s);
}


/*
  Function that returns a pseudo-random integer number
*/
long i_rnd(rnd_state *s) {
  long out;
  s->seed ^= s->mask;
  
  out = s->store[s->lstidx];
  s->store[s->lstidx] = _basic_rnd(s);
  s->lstidx = s->store[s->lstidx]*NSTORE/(s->m-1);
  if (s->lstidx >= NSTORE) s->lstidx = NSTORE-1;
  s->seed ^= s->mask;
  return out;
}
  
/*
  Function that returns a pseudo-random floating point number
  in the interval [0,1]
*/
double f_rnd(rnd_state *s) {
  long q = i_rnd(s);
  return ((double) q)/((double) (s->m-1));
}

/*
  Function that returns a pseudo-random floating point number
  with a normal distribution.
*/
double n_rnd(rnd_state *s) {
  if (s->stored) {
    s->stored = 0;
    return s->sval;
  }
  else {
    double v1, v2, r, fac;
    r = 2.0;
    while( r > 1.0 || r == 0.0) {
      v1 = 2*f_rnd(s)-1;
      v2 = 2*f_rnd(s)-1;
      r = v1*v1+v2*v2;
    }
    fac = sqrt(-2*log(r)/r);
    s->sval = v1*fac;
    s->stored = 1;
    return v2*fac;
  }
}



/*
  Initializes the generator of unique random number in the range
  [0,..n-1]. Returns a structure of type r_norep_state.
  
  Parameters
  name         type           content
  seed         long           the seed for teh random number generator;
  n            long           the value n that defines the range [0,..n-1]
                              in which the numbers have to be returned.
*/
r_norep_state *r_norep_init(long seed, long n) {
  if (n <= 0) return NULL;

  r_norep_state *r = (r_norep_state *) malloc(sizeof(r_norep_state));
  r->n = n;
  r->left = n;
  r->val = (long *) malloc(sizeof(long)*r->n);
  for (long k=0; k<r->n; k++) {
    r->val[k] = k;
  }
  r->rst = r_init(seed);
  return r;
}


/*
  Releases a structure of type r_norep_state. After this function
  has been called, the structure can no longer be used to generate
  random numbers.
*/
void r_norep_end(r_norep_state *r) {
  r_end(r->rst);
  free(r->val);
  free(r);
}


/*
  Returns a random number in a given range without repetitions. The
  function receives a structure of type r_norep_state that has been
  created with a parameter n. The function can be called at most n
  times and every time it will return a different number in the
  interval [0,..n-1]. All numbers will be returned, and nonumber will
  be returned twice. If the function is called more than n times, it
  will return -1 to indicate an incorrect call.
*/
long r_norep_rnd(r_norep_state *r) {
  if (r->left <= 0) return -1;
  if (r->left == 1) {
    r->left--;
    return r->val[0];
  }

  long q = i_rnd(r->rst) % r->left;
  long ret = r->val[q];
  r->val[q] = r->val[r->left-1];
  r->left--;
  return ret;
}
