/*
  Copyright © 2022 Barry Schwartz

  This program is free software: you can redistribute it and/or
  modify it under the terms of the GNU General Public License, as
  published by the Free Software Foundation, either version 3 of the
  License, or (at your option) any later version.

  This program is distributed in the hope that it will be useful, but
  WITHOUT ANY WARRANTY; without even the implied warranty of
  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
  General Public License for more details.

  You should have received copies of the GNU General Public License
  along with this program. If not, see
  <https://www.gnu.org/licenses/>.
*/

#ifndef QUICKSORTS__INTERNAL__QUICKSORTS_COMMON_H__HEADER_GUARD__
#define QUICKSORTS__INTERNAL__QUICKSORTS_COMMON_H__HEADER_GUARD__

#include <assert.h>
#include <limits.h>
#include <stdatomic.h>
#include <stdbool.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>

#define quicksorts_common__inline static inline

#if defined __GNUC__
#define QUICKSORTS_COMMON__MEMSET __builtin_memset
#define QUICKSORTS_COMMON__MEMCPY __builtin_memcpy
#define QUICKSORTS_COMMON__MEMMOVE __builtin_memmove
#define QUICKSORTS_COMMON__BSWAP64 __builtin_bswap64
#else
#define QUICKSORTS_COMMON__MEMSET memset
#define QUICKSORTS_COMMON__MEMCPY memcpy
#define QUICKSORTS_COMMON__MEMMOVE memmove
#define QUICKSORTS_COMMON__BSWAP64(x)               \
  ((((x) & UINT64_C (0x00000000000000FF)) << 56) |  \
   (((x) & UINT64_C (0x000000000000FF00)) << 40) |  \
   (((x) & UINT64_C (0x0000000000FF0000)) << 24) |  \
   (((x) & UINT64_C (0x00000000FF000000)) << 8) |   \
   (((x) & UINT64_C (0x000000FF00000000)) >> 8) |   \
   (((x) & UINT64_C (0x0000FF0000000000)) >> 24) |  \
   (((x) & UINT64_C (0x00FF000000000000)) >> 40) |  \
   (((x) & UINT64_C (0xFF00000000000000)) >> 56))
#endif

/*------------------------------------------------------------------*/
/* Spinlocks for random number generator seeds.                     */

typedef struct
{
  /* Use unsigned integers, so they will wrap around when they
     overflow. */
  atomic_size_t active;
  atomic_size_t available;
} quicksorts_common__spinlock_t;

quicksorts_common__inline void
quicksorts_common__obtain_spinlock
(quicksorts_common__spinlock_t *lock)
{
  size_t my_ticket =
    atomic_fetch_add_explicit (&lock->available, 1,
                               memory_order_seq_cst);
  bool done = false;
  while (!done)
    {
      size_t active_ticket =
        atomic_load_explicit (&lock->active, memory_order_seq_cst);
      done = (my_ticket == active_ticket);
      /* if (!done)                        */
      /*   optionally_put_a_pause_here (); */
    }
  atomic_thread_fence (memory_order_seq_cst);
}

quicksorts_common__inline void
quicksorts_common__release_spinlock
(quicksorts_common__spinlock_t *lock)
{
  atomic_thread_fence (memory_order_seq_cst);
  (void) atomic_fetch_add_explicit (&lock->active, 1,
                                    memory_order_seq_cst);
}

/*------------------------------------------------------------------*/
/* A simple linear congruential generator.                          */

/* The multiplier LCG_A comes from Steele, Guy; Vigna, Sebastiano (28
   September 2021). "Computationally easy, spectrally good multipliers
   for congruential pseudorandom number generators".
   arXiv:2001.05304v3 [cs.DS] */
#define QUICKSORTS_COMMON__LCG_A (UINT64_C (0xf1357aea2e62a9c5))

/* LCG_C must be odd. (The value 1 may get optimized to an increment
   instruction.) */
#define QUICKSORTS_COMMON__LCG_C (UINT64_C (1))

extern quicksorts_common__spinlock_t quicksorts_common__seed_lock;
extern uint64_t quicksorts_common__seed;

quicksorts_common__inline uint64_t
quicksorts_common__random_uint64 (void)
{
  quicksorts_common__obtain_spinlock (&quicksorts_common__seed_lock);

  const uint64_t old_seed = quicksorts_common__seed;

  /* The following operation is modulo 2**64, by virtue of standard C
     behavior for uint64_t. */
  quicksorts_common__seed =
    (QUICKSORTS_COMMON__LCG_A * old_seed) + QUICKSORTS_COMMON__LCG_C;

  quicksorts_common__release_spinlock (&quicksorts_common__seed_lock);

  /* Reverse the bytes, because least significant bits of LCG output
     tend to be bad. Indeed, the very least significant bit literally
     switches back and forth between 0 and 1. */
  return QUICKSORTS_COMMON__BSWAP64 (old_seed);
}

quicksorts_common__inline size_t
quicksorts_common__random_size_t_below (size_t n)
{
  const uint64_t v = quicksorts_common__random_uint64 ();
  return (size_t) (v % (uint64_t) n);
}

/*------------------------------------------------------------------*/

/* FIXME: TRY A SWAP THAT DOES NOT REQUIRE AN elembuf */
/* FIXME: TRY A SWAP THAT DOES NOT REQUIRE AN elembuf */
/* FIXME: TRY A SWAP THAT DOES NOT REQUIRE AN elembuf */
/* FIXME: TRY A SWAP THAT DOES NOT REQUIRE AN elembuf */
/* FIXME: TRY A SWAP THAT DOES NOT REQUIRE AN elembuf */
/* FIXME: TRY A SWAP THAT DOES NOT REQUIRE AN elembuf */
/* FIXME: TRY A SWAP THAT DOES NOT REQUIRE AN elembuf */
/* FIXME: TRY A SWAP THAT DOES NOT REQUIRE AN elembuf */
#define QUICKSORTS_COMMON__SWAP(PFX)            \
  do                                            \
    {                                           \
      QUICKSORTS_COMMON__MEMCPY                 \
        (PFX##elembuf, PFX##p1, PFX##elemsz);   \
      QUICKSORTS_COMMON__MEMCPY                 \
        (PFX##p1, PFX##p2, PFX##elemsz);        \
      QUICKSORTS_COMMON__MEMCPY                 \
        (PFX##p2, PFX##elembuf, PFX##elemsz);   \
    }                                           \
  while (0)

#define QUICKSORTS_COMMON__REVERSE_PREFIX(PFX)              \
  do                                                        \
    {                                                       \
      char *PFX##p_left = PFX##arr;                         \
      char *PFX##p_right =                                  \
        PFX##p_left + ((PFX##pfx_len - 1) * PFX##elemsz);   \
      while (PFX##p_left < PFX##p_right)                    \
        {                                                   \
          char *PFX##p1 = PFX##p_left;                      \
          char *PFX##p2 = PFX##p_right;                     \
          QUICKSORTS_COMMON__SWAP (PFX);                    \
          PFX##p_left += PFX##elemsz;                       \
          PFX##p_right -= PFX##elemsz;                      \
        }                                                   \
    }                                                       \
  while (0)

/*
  The insertion position is found by a binary search.

  References:

  * H. Bottenbruch, "Structure and use of ALGOL 60", Journal of the
    ACM, Volume 9, Issue 2, April 1962, pp.161-221.
    https://doi.org/10.1145/321119.321120

    The general algorithm is described on pages 214 and 215.

  * https://en.wikipedia.org/w/index.php?title=Binary_search_algorithm&oldid=1062988272#Alternative_procedure
*/
#define QUICKSORTS_COMMON__INSERTION_POSITION(PFX)              \
  do                                                            \
    {                                                           \
      size_t PFX##j = 0;                                        \
      size_t PFX##k = PFX##i - 1;                               \
                                                                \
      while (PFX##j != PFX##k)                                  \
        {                                                       \
          /* Ceiling of the midway point: */                    \
          size_t PFX##h = PFX##k - ((PFX##k - PFX##j) >> 1);    \
                                                                \
          if (QUICKSORTS__UNSTABLE_QUICKSORT__LT                \
              ((void *) (PFX##arr + (PFX##elemsz * PFX##i)),    \
               (void *) (PFX##arr + (PFX##elemsz * PFX##h))))   \
            PFX##k = PFX##h - 1;                                \
          else                                                  \
            PFX##j = PFX##h;                                    \
        }                                                       \
                                                                \
      if (PFX##j != 0)                                          \
        PFX##pos = PFX##j + 1;                                  \
      else if (QUICKSORTS__UNSTABLE_QUICKSORT__LT               \
               ((void *) (PFX##arr + (PFX##elemsz * PFX##i)),   \
                (void *) PFX##arr))                             \
        PFX##pos = 0;                                           \
      else                                                      \
        PFX##pos = 1;                                           \
    }                                                           \
  while (0)

#define QUICKSORTS_COMMON__SUBCIRCULATE_RIGHT(PFX)                      \
  do                                                                    \
    {                                                                   \
      if (PFX##left != PFX##right)                                      \
        {                                                               \
          char *PFX##p_left = PFX##arr + (PFX##elemsz * PFX##left);     \
          char *PFX##p_right = PFX##arr + (PFX##elemsz * PFX##right);   \
                                                                        \
          QUICKSORTS_COMMON__MEMCPY                                     \
            (PFX##elembuf, PFX##p_right, PFX##elemsz);                  \
                                                                        \
          QUICKSORTS_COMMON__MEMMOVE                                    \
            (PFX##p_left + PFX##elemsz, PFX##p_left,                    \
             PFX##elemsz * (PFX##right - PFX##left));                   \
                                                                        \
          QUICKSORTS_COMMON__MEMCPY                                     \
            (PFX##p_left, PFX##elembuf, PFX##elemsz);                   \
        }                                                               \
    }                                                                   \
  while (0)

#define QUICKSORTS_COMMON__INSERTION_SORT(PFX, MAKE_AN_ORDERED_PREFIX)  \
  do                                                                    \
    {                                                                   \
      if (PFX##nmemb > 1)                                               \
        {                                                               \
          size_t PFX##pfx_len;                                          \
          MAKE_AN_ORDERED_PREFIX (PFX);                                 \
          size_t PFX##i = PFX##pfx_len;                                 \
          while (PFX##i != PFX##nmemb)                                  \
            {                                                           \
              size_t PFX##pos;                                          \
              QUICKSORTS_COMMON__INSERTION_POSITION (PFX);              \
              size_t PFX##left = PFX##pos;                              \
              size_t PFX##right = PFX##i;                               \
              QUICKSORTS_COMMON__SUBCIRCULATE_RIGHT (PFX);              \
              PFX##i += 1;                                              \
            }                                                           \
        }                                                               \
    }                                                                   \
  while (0)

#define QUICKSORTS_COMMON__STK_MAKE(PFX)                    \
  typedef struct {char *p; size_t n;} PFX##stk_entry_t;     \
  PFX##stk_entry_t PFX##stk[CHAR_BIT * sizeof (size_t)];    \
  QUICKSORTS_COMMON__MEMSET                                 \
  (PFX##stk, 0,                                             \
   CHAR_BIT * sizeof (size_t) * sizeof (PFX##stk_entry_t)); \
  int PFX##stk_depth = 0

#define QUICKSORTS_COMMON__STK_PUSH(PFX, P, N)                  \
  do                                                            \
    {                                                           \
      assert (PFX##stk_depth < CHAR_BIT * sizeof (size_t));     \
      PFX##stk_depth += 1;                                      \
      PFX##stk_entry_t PFX##entry;                              \
      PFX##entry.p = (P);                                       \
      PFX##entry.n = (N);                                       \
      PFX##stk[(CHAR_BIT * sizeof (size_t)) - PFX##stk_depth] = \
        PFX##entry;                                             \
    }                                                           \
  while (0)

#define QUICKSORTS_COMMON__STK_POP(PFX)                             \
  do                                                                \
    {                                                               \
      assert (0 < PFX##stk_depth);                                  \
      PFX##stk_entry_t PFX##entry =                                 \
        PFX##stk[(CHAR_BIT * sizeof (size_t)) - PFX##stk_depth];    \
      PFX##arr = PFX##entry.p;                                      \
      PFX##nmemb = PFX##entry.n;                                    \
      PFX##stk_depth -= 1;                                          \
    }                                                               \
  while (0)

#endif /* QUICKSORTS__INTERNAL__QUICKSORTS_COMMON_H__HEADER_GUARD__ */
