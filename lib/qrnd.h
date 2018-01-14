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

#ifndef ____RANDOM___INCLUDED_____
#define ____RANDOM___INCLUDED_____

#include <math.h>


#define NSTORE   32





/*
  State returned by the initialization routine. The fields seed, a, m,
  q, r, mask, store and lastidx are used by the integer generator (see
  s874 for documentation on the meaning of these fields), the fields
  stored and sval are used only by the normal distribution generator.
*/
typedef struct {
  long seed;
  long a;
  long m;
  long q;
  long r;
  long mask;  /* This is a trick to scramble the seed (see s874) */
  long store[NSTORE];
  int  lstidx;

  int stored;
  double sval;
} rnd_state;


/*
  This is the state returned by the function r_norep_init, which
  initializes a random number generators that, called n times, returns
  all the numbers in [0,..n-1] in random order without repetitions. 

  This structure has the following elements:

  Name        type                content
  n           long             the number n of different numbers that
                               will be returned.
  left        long             at any given moment, the number of values
                               that still have to be returned. If the function
			       r_norep_rnd has been called k times with this
			       strucure, then left = n-k
  val         long *           an array with the numbers that haven't been
                               generated yet. Only the first "left" entries
			       are valid at any time
  rst         rnd_state *      a structure for the generation of random numbers


  Created for Rev. 2.0
*/
typedef  struct {
  long  n;
  long  left;
  long  *val;
  rnd_state *rst;
} r_norep_state;


/*
  Initializes the generator of unique random number in the range
  [0,..n-1]. Returns a structure of type r_norep_state.
  
  Parameters
  name         type           content
  seed         long           the seed for teh random number generator;
  n            long           the value n that defines the range [0,..n-1]
                              in which the numbers have to be returned.
*/
r_norep_state *r_norep_init(long seed, long n);


/*
  Releases a structure of type r_norep_state. After this function
  has been called, the structure can no longer be used to generate
  random numbers.
*/
void r_norep_end(r_norep_state *r);


/*
  Returns a random number in a given range without repetitions. The
  function receives a structure of type r_norep_state that has been
  created with a parameter n. The function can be called at most n
  times and every time it will return a different number in the
  interval [0,..n-1]. All numbers will be returned, and nonumber will
  be returned twice. If the function is called more than n times, it
  will return -1 to indicate an incorrect call.
*/
long r_norep_rnd(r_norep_state *r);


/*
  Inititlization function: given a seed initializes a state
  and returns a pointer to it.
*/
rnd_state *r_init(long seed);


/*
  Finalizes the use of the generator liberating the memoty used
  by the state
*/
void r_end(rnd_state *s);


/*
  Function that returns a pseudo-random integer number
*/
long i_rnd(rnd_state *s);
  
/*
  Function that returns a pseudo-random floating point number
  in the interval [0,1]
*/
double f_rnd(rnd_state *s);

/*
  Function that returns a pseudo-random floating point number
  with a normal distribution.
*/
double n_rnd(rnd_state *s);

#endif
