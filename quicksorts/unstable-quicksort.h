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
#include <quicksorts/internal/quicksorts-common.h>

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
          QUICKSORTS_COMMON__REVERSE_PREFIX (PFX);                      \
        }                                                               \
    }                                                                   \
  while (0)

#define QUICKSORTS__UNSTABLE_QUICKSORT__INSERTION_SORT(PFX, ARR, NMEMB, \
                                                       ELEMSZ)          \
  QUICKSORTS_COMMON__INSERTION_SORT(PFX, (ARR), (NMEMB), (ELEMSZ),      \
    QUICKSORTS__UNSTABLE_QUICKSORT__MAKE_AN_ORDERED_PREFIX)

#endif /* QUICKSORTS__UNSTABLE_QUICKSORT_H__HEADER_GUARD__ */
