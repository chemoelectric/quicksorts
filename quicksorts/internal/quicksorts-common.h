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

#include <stdlib.h>
#include <string.h>

#if defined __GNUC__
#define QUICKSORTS_COMMON__MEMCPY __builtin_memcpy
#define QUICKSORTS_COMMON__MEMMOVE __builtin_memmove
#else
#define QUICKSORTS_COMMON__MEMCPY memcpy
#define QUICKSORTS_COMMON__MEMMOVE memmove
#endif

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

#define QUICKSORTS_COMMON__INSERTION_SORT(PFX, ARR, NMEMB, ELEMSZ,  \
                                          MAKE_AN_ORDERED_PREFIX)   \
  do                                                                \
    {                                                               \
      size_t PFX##nmemb = (NMEMB);                                  \
      if (PFX##nmemb > 1)                                           \
        {                                                           \
          size_t PFX##elemsz = (ELEMSZ);                            \
          char *PFX##arr = (void *) (ARR);                          \
          size_t PFX##pfx_len;                                      \
          MAKE_AN_ORDERED_PREFIX (PFX);                             \
          size_t PFX##i = PFX##pfx_len;                             \
          while (PFX##i != PFX##nmemb)                              \
            {                                                       \
              size_t PFX##pos;                                      \
              QUICKSORTS_COMMON__INSERTION_POSITION (PFX);          \
              size_t PFX##left = PFX##pos;                          \
              size_t PFX##right = PFX##i;                           \
              QUICKSORTS_COMMON__SUBCIRCULATE_RIGHT (PFX);          \
              PFX##i += 1;                                          \
            }                                                       \
        }                                                           \
    }                                                               \
  while (0)

#endif /* QUICKSORTS__INTERNAL__QUICKSORTS_COMMON_H__HEADER_GUARD__ */
