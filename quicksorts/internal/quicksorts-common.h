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

#ifndef QUICKSORTS_COMMON__ELEMBUF_SIZE
#define QUICKSORTS_COMMON__ELEMBUF_SIZE 128
#endif

#if defined __GNUC__
#define quicksorts_common__inline \
  __attribute__((__always_inline__)) static inline
#else
#define quicksorts_common__inline static inline
#endif

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
/* Pivot selection mechanisms.                                      */

#define QUICKSORTS_COMMON__PIVOT_RANDOM(BASE, NMEMB, SIZE, LT,  \
                                        RESULT)                 \
  do                                                            \
    {                                                           \
      RESULT = quicksorts_common__random_size_t_below (NMEMB);  \
    }                                                           \
  while (0)

#define QUICKSORTS_COMMON__PIVOT_MIDDLE(BASE, NMEMB, SIZE, LT,  \
                                        RESULT)                 \
  do                                                            \
    {                                                           \
      RESULT = ((NMEMB) >> 1);                                  \
    }                                                           \
  while (0)

#define QUICKSORTS_COMMON__PIVOT_MEDIAN_OF_THREE(BASE, NMEMB, SIZE, \
                                                 LT, RESULT)        \
  do                                                                \
    {                                                               \
      size_t nmemb = (size_t) (NMEMB);                              \
      if (nmemb <= 2)                                               \
        {                                                           \
          RESULT = (size_t) 0;                                      \
        }                                                           \
      else                                                          \
        {                                                           \
          char *const arr = (void *) (BASE);                        \
          const size_t elemsz = (size_t) (SIZE);                    \
                                                                    \
          const size_t i_first = 0;                                 \
          const size_t i_middle = nmemb >> 1;                       \
          const size_t i_last = nmemb - 1;                          \
                                                                    \
          const char *p_first = arr;                                \
          const char *p_middle = arr + (elemsz * i_middle);         \
          const char *p_last = arr + (elemsz * i_last);             \
                                                                    \
          const bool middle_lt_first =                              \
            (bool) LT (p_middle, p_first);                          \
          const bool last_lt_first =                                \
            (bool) LT (p_last, p_first);                            \
          if (middle_lt_first != last_lt_first)                     \
            {                                                       \
              RESULT = i_first;                                     \
            }                                                       \
          else                                                      \
            {                                                       \
              const bool middle_lt_last =                           \
                (bool) LT (p_middle, p_last);                       \
              if (middle_lt_first != middle_lt_last)                \
                {                                                   \
                  RESULT = i_middle;                                \
                }                                                   \
              else                                                  \
                {                                                   \
                  RESULT = i_last;                                  \
                }                                                   \
            }                                                       \
        }                                                           \
    }                                                               \
  while (0)

/*------------------------------------------------------------------*/

/* Swap two elements. */
quicksorts_common__inline void
quicksorts_common__elem_swap (char *p1, char *p2, size_t elemsz)
{
  char elembuf[QUICKSORTS_COMMON__ELEMBUF_SIZE];

  if (elemsz <= (QUICKSORTS_COMMON__ELEMBUF_SIZE))
    {
      QUICKSORTS_COMMON__MEMCPY (elembuf, p1, elemsz);
      QUICKSORTS_COMMON__MEMCPY (p1, p2, elemsz);
      QUICKSORTS_COMMON__MEMCPY (p2, elembuf, elemsz);
    }
  else
    {
      for (size_t i = 0;
           i != elemsz;
           i +=
             (((elemsz - i) < (QUICKSORTS_COMMON__ELEMBUF_SIZE)) ?
              (elemsz - i) : (QUICKSORTS_COMMON__ELEMBUF_SIZE)))
        {
          const size_t blocksz =
            ((elemsz - i) < (QUICKSORTS_COMMON__ELEMBUF_SIZE)) ?
            (elemsz - i) : (QUICKSORTS_COMMON__ELEMBUF_SIZE);
          QUICKSORTS_COMMON__MEMCPY (elembuf, p1, blocksz);
          QUICKSORTS_COMMON__MEMCPY (p1, p2, blocksz);
          QUICKSORTS_COMMON__MEMCPY (p2, elembuf, blocksz);
          p1 += blocksz;
          p2 += blocksz;
        }
    }
}

/* Swap two typed elements. */
#define QUICKSORTS_COMMON__ELEM_SWAP__TYPED(PFX, T, P1, P2) \
  do                                                        \
    {                                                       \
      T *PFX##swap__p1 = (T *) (P1);                        \
      T *PFX##swap__p2 = (T *) (P2);                        \
      T tmp = *PFX##swap__p1;                               \
      *PFX##swap__p1 = *PFX##swap__p2;                      \
      *PFX##swap__p2 = tmp;                                 \
    }                                                       \
  while (0)

/* Reverse a prefix of pfx_len >= 2. */
quicksorts_common__inline void
quicksorts_common__reverse_prefix (char *arr, size_t pfx_len,
                                   size_t elemsz)
{
  char *p_left = arr;
  char *p_right = p_left + ((pfx_len - 1) * elemsz);
  do
    {
      quicksorts_common__elem_swap (p_left, p_right, elemsz);
      p_left += elemsz;
      p_right -= elemsz;
    }
  while (p_left < p_right);
}

/* Reverse a prefix of pfx_len >= 2. */
#define QUICKSORTS_COMMON__REVERSE_PREFIX__TYPED(PFX, T, ARR,   \
                                                 PFX_LEN)       \
  do                                                            \
    {                                                           \
      const size_t PFX##reverse_prefix__pfx_len =               \
        (size_t) (PFX_LEN);                                     \
      T *PFX##reverse_prefix__p_left = (T *) (ARR);             \
      T *PFX##reverse_prefix__p_right =                         \
        (PFX##reverse_prefix__p_left +                          \
         PFX##reverse_prefix__pfx_len - 1);                     \
      do                                                        \
        {                                                       \
          QUICKSORTS_COMMON__ELEM_SWAP__TYPED                   \
            (PFX##__reverse_prefix__, T,                        \
             PFX##reverse_prefix__p_left,                       \
             PFX##reverse_prefix__p_right);                     \
          PFX##reverse_prefix__p_left += 1;                     \
          PFX##reverse_prefix__p_right -= 1;                    \
        }                                                       \
      while (PFX##reverse_prefix__p_left                        \
             < PFX##reverse_prefix__p_right);                   \
    }                                                           \
  while (0)

#if 1
/**/
/*
  The insertion position is found by a binary search.

  References:

  * H. Bottenbruch, "Structure and use of ALGOL 60", Journal of the
    ACM, Volume 9, Issue 2, April 1962, pp.161-221.
    https://doi.org/10.1145/321119.321120

    The general algorithm is described on pages 214 and 215.

  * https://en.wikipedia.org/w/index.php?title=Binary_search_algorithm&oldid=1062988272#Alternative_procedure
*/
#define QUICKSORTS_COMMON__INSERTION_POSITION(PFX, LT)              \
  do                                                                \
    {                                                               \
      char *PFX##pj = PFX##arr;                                     \
      char *PFX##pk = PFX##pi - PFX##elemsz;                        \
                                                                    \
      while (PFX##pj != PFX##pk)                                    \
        {                                                           \
          /* Ceiling of the midway point: */                        \
          char *PFX##ph =                                           \
            PFX##pk - (PFX##elemsz *                                \
                       (((PFX##pk - PFX##pj) / PFX##elemsz) >> 1)); \
                                                                    \
          if (LT ((void *) PFX##pi, (void *) PFX##ph))              \
            PFX##pk = PFX##ph - PFX##elemsz;                        \
          else                                                      \
            PFX##pj = PFX##ph;                                      \
        }                                                           \
                                                                    \
      if (PFX##pj != PFX##arr)                                      \
        PFX##pos = PFX##pj + PFX##elemsz;                           \
      else if (LT ((void *) PFX##pi, (void *) PFX##arr))            \
        PFX##pos = PFX##arr;                                        \
      else                                                          \
        PFX##pos = PFX##arr + PFX##elemsz;                          \
    }                                                               \
  while (0)
/**/
#else
/**/
/* Here is a position-finder for a *non-binary* (ordinary) insertion
   sort. */
#define QUICKSORTS_COMMON__INSERTION_POSITION(PFX, LT)  \
  do                                                    \
    {                                                   \
      char *PFX##pj = PFX##pi;                          \
      while (PFX##pj != PFX##arr &&                     \
             LT ((void *) PFX##pi,                      \
                 (void *) (PFX##pj - PFX##elemsz)))     \
        PFX##pj -= PFX##elemsz;                         \
      PFX##pos = PFX##pj;                               \
    }                                                   \
  while (0)
/**/
#endif

#define QUICKSORTS_COMMON__INSERTION_POSITION__TYPED(PFX, T, LT)    \
  do                                                                \
    {                                                               \
      T *PFX##pj = PFX##arr;                                        \
      T *PFX##pk = PFX##pi - 1;                                     \
                                                                    \
      while (PFX##pj != PFX##pk)                                    \
        {                                                           \
          /* Ceiling of the midway point: */                        \
          T *PFX##ph = PFX##pk - ((PFX##pk - PFX##pj) >> 1);        \
                                                                    \
          if (LT (PFX##pi, PFX##ph))                                \
            PFX##pk = PFX##ph - 1;                                  \
          else                                                      \
            PFX##pj = PFX##ph;                                      \
        }                                                           \
                                                                    \
      if (PFX##pj != PFX##arr)                                      \
        PFX##pos = PFX##pj + 1;                                     \
      else if (LT (PFX##pi, PFX##arr))                              \
        PFX##pos = PFX##arr;                                        \
      else                                                          \
        PFX##pos = PFX##arr + 1;                                    \
    }                                                               \
  while (0)

#if 1
/* One implementation of subcirculate_right. */
quicksorts_common__inline void
quicksorts_common__subcirculate_right (char *p_left, char *p_right,
                                       size_t elemsz)
{
  char elembuf[QUICKSORTS_COMMON__ELEMBUF_SIZE];

  if (p_left != p_right)
    {
      if (elemsz <= (QUICKSORTS_COMMON__ELEMBUF_SIZE))
        {
          QUICKSORTS_COMMON__MEMCPY (elembuf, p_right, elemsz);
          QUICKSORTS_COMMON__MEMMOVE (p_left + elemsz, p_left,
                                      p_right - p_left);
          QUICKSORTS_COMMON__MEMCPY (p_left, elembuf, elemsz);
        }
      else
        {
          for (size_t i = 0;
               i != elemsz;
               i +=
                 (((elemsz - i) < (QUICKSORTS_COMMON__ELEMBUF_SIZE)) ?
                  (elemsz - i) : (QUICKSORTS_COMMON__ELEMBUF_SIZE)))
            {
              const size_t blocksz =
                ((elemsz - i) < (QUICKSORTS_COMMON__ELEMBUF_SIZE)) ?
                (elemsz - i) : (QUICKSORTS_COMMON__ELEMBUF_SIZE);
              QUICKSORTS_COMMON__MEMCPY (elembuf, p_right, blocksz);
              for (char *p = p_right; p != p_left; p -= elemsz)
                QUICKSORTS_COMMON__MEMCPY (p, p - elemsz, blocksz);
              QUICKSORTS_COMMON__MEMCPY (p_left, elembuf, blocksz);
              p_right += blocksz;
              p_left += blocksz;
            }
        }
    }
}
#else
/* Another implementation of subcirculate_right. */
quicksorts_common__inline void
quicksorts_common__subcirculate_right (char *p_left, char *p_right,
                                       size_t elemsz)
{
  if (p_left != p_right)
    for (size_t i = 0; i != elemsz; i += 1)
      {
        char tmp = *p_right;
        for (char *p = p_right; p != p_left; p -= elemsz)
          *p = *(p - elemsz);
        *p_left = tmp;
        p_left += 1;
        p_right += 1;
      }
}
#endif

#define QUICKSORTS_COMMON__SUBCIRCULATE_RIGHT__TYPED(PFX, T,        \
                                                     P_LEFT,        \
                                                     P_RIGHT)       \
  do                                                                \
    {                                                               \
      T *PFX##subcircrt__p_left = (T *) (P_LEFT);                   \
      T *PFX##subcircrt__p_right = (T *) (P_RIGHT);                 \
      if (PFX##subcircrt__p_left != PFX##subcircrt__p_right)        \
        {                                                           \
          T PFX##subcircrt__tmp = *PFX##subcircrt__p_right;         \
          QUICKSORTS_COMMON__MEMMOVE                                \
            (PFX##subcircrt__p_left + 1,                            \
             PFX##subcircrt__p_left,                                \
             ((PFX##subcircrt__p_right - PFX##subcircrt__p_left)    \
              * sizeof (T)));                                       \
          *PFX##subcircrt__p_left = PFX##subcircrt__tmp;            \
        }                                                           \
    }                                                               \
  while (0)

#if 1
/* One implementation of subcirculate_right_with_gap. */
quicksorts_common__inline void
quicksorts_common__subcirculate_right_with_gap (char *p_left,
                                                char *p_right,
                                                size_t elemsz,
                                                size_t gap)
{
  char elembuf[QUICKSORTS_COMMON__ELEMBUF_SIZE];

  if (p_left != p_right)
    {
      const size_t chargap = elemsz * gap;

      if (elemsz <= (QUICKSORTS_COMMON__ELEMBUF_SIZE))
        {
          QUICKSORTS_COMMON__MEMCPY (elembuf, p_right, elemsz);
          if (gap == 1)
            QUICKSORTS_COMMON__MEMMOVE (p_left + elemsz, p_left,
                                        p_right - p_left);
          else
            for (char *p = p_right; p != p_left; p -= chargap)
              QUICKSORTS_COMMON__MEMCPY (p, p - chargap, elemsz);
          QUICKSORTS_COMMON__MEMCPY (p_left, elembuf, elemsz);
        }
      else
        {
          for (size_t i = 0;
               i != elemsz;
               i +=
                 (((elemsz - i) < (QUICKSORTS_COMMON__ELEMBUF_SIZE)) ?
                  (elemsz - i) : (QUICKSORTS_COMMON__ELEMBUF_SIZE)))
            {
              const size_t blocksz =
                ((elemsz - i) < (QUICKSORTS_COMMON__ELEMBUF_SIZE)) ?
                (elemsz - i) : (QUICKSORTS_COMMON__ELEMBUF_SIZE);
              QUICKSORTS_COMMON__MEMCPY (elembuf, p_right, blocksz);
              for (char *p = p_right; p != p_left; p -= chargap)
                QUICKSORTS_COMMON__MEMCPY (p, p - chargap, blocksz);
              QUICKSORTS_COMMON__MEMCPY (p_left, elembuf, blocksz);
              p_right += blocksz;
              p_left += blocksz;
            }
        }
    }
}
#else
/* Another implementation of subcirculate_right_with_gap. I have
   gotten bad performance from it, however. */
quicksorts_common__inline void
quicksorts_common__subcirculate_right_with_gap (char *p_left,
                                                char *p_right,
                                                size_t elemsz,
                                                size_t gap)
{
  if (p_left != p_right)
    {
      const size_t chargap = elemsz * gap;

      for (size_t i = 0; i != elemsz; i += 1)
        {
          char tmp = *p_right;
          for (char *p = p_right; p != p_left; p -= chargap)
            *p = *(p - chargap);
          *p_left = tmp;
          p_left += 1;
          p_right += 1;
        }
    }
}
#endif

#define QUICKSORTS_COMMON__SUBCIRCULATE_RIGHT_WITH_GAP__TYPED(PFX, T,   \
                                                              P_LEFT,   \
                                                              P_RIGHT,  \
                                                              GAP)      \
  do                                                                    \
    {                                                                   \
      T *PFX##subcrtgap__p_left = (T *) (P_LEFT);                       \
      T *PFX##subcrtgap__p_right = (T *) (P_RIGHT);                     \
      if (PFX##subcrtgap__p_left != PFX##subcrtgap_p_right)             \
        {                                                               \
          T PFX##subcrtgap__tmp = *PFX##subcrtgap_p_right;              \
          for (T *PFX##subcrtgap__p = PFX##subcrtgap__p_right;          \
               PFX##subcrtgap__p != PFX##subcrtgap__p_left;             \
               PFX##subcrtgap__p -= PFX##subcrtgap__gap)                \
            *PFX##subcrtgap__p =                                        \
              *(PFX##subcrtgap__p - PFX##subcrtgap__gap);               \
          *PFX##subcrtgap_p_left = PFX##subcrtgap__tmp;                 \
        }                                                               \
    }                                                                   \
  while (0)

#define QUICKSORTS_COMMON__INSERTION_SORT(PFX, LT, SMALL_SIZE,      \
                                          MAKE_AN_ORDERED_PREFIX)   \
  do                                                                \
    {                                                               \
      if (PFX##nmemb > 1)                                           \
        {                                                           \
          size_t PFX##pfx_len;                                      \
          MAKE_AN_ORDERED_PREFIX (PFX, LT);                         \
          size_t PFX##i = PFX##pfx_len;                             \
          while (PFX##i != PFX##nmemb)                              \
            {                                                       \
              char *PFX##pi = PFX##arr + (PFX##elemsz * PFX##i);    \
              char *PFX##pos;                                       \
              QUICKSORTS_COMMON__INSERTION_POSITION (PFX, LT);      \
              quicksorts_common__subcirculate_right                 \
                (PFX##pos, PFX##pi, PFX##elemsz);                   \
              PFX##i += 1;                                          \
            }                                                       \
        }                                                           \
    }                                                               \
  while (0)

#define QUICKSORTS_COMMON__INSERTION_SORT__TYPED(PFX, T, LT,            \
                                                 SMALL_SIZE,            \
                                                 MAKE_AN_ORDERED_PREFIX) \
  do                                                                    \
    {                                                                   \
      if (PFX##nmemb > 1)                                               \
        {                                                               \
          size_t PFX##pfx_len;                                          \
          MAKE_AN_ORDERED_PREFIX (PFX, T, LT);                          \
          size_t PFX##i = PFX##pfx_len;                                 \
          while (PFX##i != PFX##nmemb)                                  \
            {                                                           \
              T *PFX##pi = ((T *) PFX##arr) + PFX##i;                   \
              T *PFX##pos;                                              \
              QUICKSORTS_COMMON__INSERTION_POSITION__TYPED              \
                (PFX, T, LT);                                           \
              QUICKSORTS_COMMON__SUBCIRCULATE_RIGHT__TYPED              \
                (PFX, T, PFX##pos, PFX##pi);                            \
              PFX##i += 1;                                              \
            }                                                           \
        }                                                               \
    }                                                                   \
  while (0)

#define QUICKSORTS_COMMON__STK_MAKE(PFX)                    \
  typedef struct {void *p; size_t n;} PFX##stk_entry_t;     \
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
