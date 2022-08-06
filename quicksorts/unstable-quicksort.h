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

#ifndef QUICKSORTS__UNSTABLE_QUICKSORT_H__HEADER_GUARD__
#define QUICKSORTS__UNSTABLE_QUICKSORT_H__HEADER_GUARD__

#include <stdlib.h>
#include <string.h>

#define QUICKSORTS__UNSTABLE_QUICKSORT__SWAP(PFX)                   \
  do                                                                \
    {                                                               \
      size_t PFX##i = PFX##elemsz;                                  \
      for (size_t PFX##i = 0; PFX##i < PFX##elemsz; PFX##i += 1)    \
        {                                                           \
          char PFX##tmp = PFX##p1[PFX##i];                          \
          PFX##p1[PFX##i] = PFX##p2[PFX##i];                        \
          PFX##p2[PFX##i] = PFX##tmp;                               \
        }                                                           \
    }                                                               \
  while (0)

#define QUICKSORTS__UNSTABLE_QUICKSORT__REVERSE_PREFIX(PFX)     \
  do                                                            \
    {                                                           \
      char *PFX##p_left = PFX##arr;                             \
      char *PFX##p_right =                                      \
        PFX##p_left + ((PFX##pfx_len - 1) * PFX##elemsz);       \
      while (PFX##p_left < PFX##p_right)                        \
        {                                                       \
          char *PFX##p1 = PFX##p_left;                          \
          char *PFX##p2 = PFX##p_right;                         \
          QUICKSORTS__UNSTABLE_QUICKSORT__SWAP (PFX);           \
          PFX##p_left += PFX##elemsz;                           \
          PFX##p_right -= PFX##elemsz;                          \
        }                                                       \
    }                                                           \
  while (0)

#define QUICKSORTS__UNSTABLE_QUICKSORT__MAKE_AN_ORDERED_PREFIX(PFX)     \
  do                                                                    \
    {                                                                   \
      PFX##pfx_len = 2;                                                 \
      char *PFX##p = PFX##arr + (PFX##elemsz * 2);                      \
                                                                        \
      if (!(QUICKSORTS__UNSTABLE_QUICKSORT__LT                          \
            ((const void *) (PFX##arr + PFX##elemsz),                   \
             (const void *) PFX##arr)))                                 \
        {                                                               \
          /* Non-decreasing order. */                                   \
          while (PFX##pfx_len < PFX##nmemb &&                           \
                 !(QUICKSORTS__UNSTABLE_QUICKSORT__LT                   \
                   ((void *) PFX##p, (void *) (PFX##p - PFX##elemsz)))) \
            {                                                           \
              PFX##pfx_len += 1;                                        \
              PFX##p += PFX##elemsz;                                    \
            }                                                           \
        }                                                               \
      else                                                              \
        {                                                               \
          /* Decreasing order. This branch sorts unstably. */           \
          while (PFX##pfx_len < PFX##nmemb &&                           \
                 !(QUICKSORTS__UNSTABLE_QUICKSORT__LT                   \
                   ((void *) (PFX##p - PFX##elemsz), (void *) PFX##p))) \
            {                                                           \
              PFX##pfx_len += 1;                                        \
              PFX##p += PFX##elemsz;                                    \
            }                                                           \
          QUICKSORTS__UNSTABLE_QUICKSORT__REVERSE_PREFIX (PFX);         \
        }                                                               \
    }                                                                   \
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
#define QUICKSORTS__UNSTABLE_QUICKSORT__INSERTION_POSITION(PFX)         \
  do                                                                    \
    {                                                                   \
      size_t PFX##j = 0;                                                \
      size_t PFX##k = PFX##i - 1;                                       \
                                                                        \
      while (PFX##j != PFX##k)                                          \
        {                                                               \
          /* Ceiling of the midway point: */                            \
          size_t PFX##h = PFX##k - ((PFX##k - PFX##j) >> 1);            \
                                                                        \
          if (QUICKSORTS__UNSTABLE_QUICKSORT__LT                        \
              ((void *) (PFX##arr + (PFX##elemsz * PFX##i)),            \
               (void *) (PFX##arr + (PFX##elemsz * PFX##h))))           \
            PFX##k = PFX##h - 1;                                        \
          else                                                          \
            PFX##j = PFX##h;                                            \
        }                                                               \
                                                                        \
      if (PFX##j != 0)                                                  \
        PFX##pos = PFX##j + 1;                                          \
      else if (QUICKSORTS__UNSTABLE_QUICKSORT__LT                       \
               ((void *) (PFX##arr + (PFX##elemsz * PFX##i)),           \
                (void *) PFX##arr))                                     \
        PFX##pos = 0;                                                   \
      else                                                              \
        PFX##pos = 1;                                                   \
    }                                                                   \
  while (0)

#define QUICKSORTS__UNSTABLE_QUICKSORT__SUBCIRCULATE_RIGHT(PFX)         \
  do                                                                    \
    {                                                                   \
      if (PFX##left != PFX##right)                                      \
        {                                                               \
          char *PFX##p_left = PFX##arr + (PFX##elemsz * PFX##left);     \
          char *PFX##p_right = PFX##arr + (PFX##elemsz * PFX##right);   \
          for (size_t PFX##bytenum = 0;                                 \
               PFX##bytenum < PFX##elemsz;                              \
               PFX##bytenum += 1)                                       \
            {                                                           \
              char *PFX##pl = PFX##p_left + PFX##bytenum;               \
              char *PFX##pr = PFX##p_right + PFX##bytenum;              \
                                                                        \
              char PFX##tmp = *PFX##pr;                                 \
                                                                        \
              for (char *PFX##p = PFX##pr;                              \
                   PFX##p != PFX##pl;                                   \
                   PFX##p -= PFX##elemsz)                               \
                *PFX##p = *(PFX##p - PFX##elemsz);                      \
                                                                        \
              *PFX##pl = PFX##tmp;                                      \
            }                                                           \
        }                                                               \
    }                                                                   \
  while (0)

#define QUICKSORTS__UNSTABLE_QUICKSORT__INSERTION_SORT(PFX, ARR, NMEMB, \
                                                       ELEMSZ)          \
  do                                                                    \
    {                                                                   \
      size_t PFX##nmemb = (NMEMB);                                      \
      if (PFX##nmemb > 1)                                               \
        {                                                               \
          size_t PFX##elemsz = (ELEMSZ);                                \
          char *PFX##arr = (void *) (ARR);                              \
          size_t PFX##pfx_len;                                          \
          QUICKSORTS__UNSTABLE_QUICKSORT__MAKE_AN_ORDERED_PREFIX (PFX); \
          size_t PFX##i = PFX##pfx_len;                                 \
          while (PFX##i != PFX##nmemb)                                  \
            {                                                           \
              size_t PFX##pos;                                          \
              QUICKSORTS__UNSTABLE_QUICKSORT__INSERTION_POSITION (PFX); \
              size_t PFX##left = PFX##pos;                              \
              size_t PFX##right = PFX##i;                               \
              QUICKSORTS__UNSTABLE_QUICKSORT__SUBCIRCULATE_RIGHT (PFX); \
              PFX##i += 1;                                              \
            }                                                           \
        }                                                               \
    }                                                                   \
  while (0)

#endif /* QUICKSORTS__UNSTABLE_QUICKSORT_H__HEADER_GUARD__ */
