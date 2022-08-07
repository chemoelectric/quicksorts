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

#define QUICKSORTS__UNSTABLE_QUICKSORT__INSERTION_SORT(PFX)     \
  QUICKSORTS_COMMON__INSERTION_SORT                             \
  (PFX, QUICKSORTS__UNSTABLE_QUICKSORT__MAKE_AN_ORDERED_PREFIX)

#define QUICKSORTS__UNSTABLE_QUICKSORT__MOVE_RIGHTWARDS(PFX)    \
  do                                                            \
    {                                                           \
      while (PFX##p_left != PFX##p_pivot &&                     \
             QUICKSORTS__UNSTABLE_QUICKSORT__LT                 \
             ((void *) PFX##p_left, (void *) PFX##p_pivot))     \
        PFX##p_left += PFX##elemsz;                             \
    }                                                           \
  while (0)

#define QUICKSORTS__UNSTABLE_QUICKSORT__MOVE_LEFTWARDS(PFX)     \
  do                                                            \
    {                                                           \
      while (PFX##p_right != PFX##p_pivot &&                    \
             QUICKSORTS__UNSTABLE_QUICKSORT__LT                 \
             ((void *) PFX##p_pivot, (void *) PFX##p_right))    \
        PFX##p_right -= PFX##elemsz;                            \
    }                                                           \
  while (0)

#define QUICKSORTS__UNSTABLE_QUICKSORT__PARTITION(PFX)                  \
  do                                                                    \
    {                                                                   \
      PFX##i_pivot =                                                    \
        quicksorts_common__random_size_t_below (PFX##nmemb);            \
      PFX##p_pivot = PFX##arr + (PFX##elemsz * PFX##i_pivot);           \
      char *PFX##p_left = PFX##arr;                                     \
      char *PFX##p_right =                                              \
        PFX##arr + (PFX##elemsz * (PFX##nmemb - 1));                    \
      do                                                                \
        {                                                               \
          QUICKSORTS__UNSTABLE_QUICKSORT__MOVE_RIGHTWARDS (PFX);        \
          QUICKSORTS__UNSTABLE_QUICKSORT__MOVE_LEFTWARDS (PFX);         \
          if (PFX##p_left != PFX##p_right)                              \
            {                                                           \
              char *const PFX##p1 = PFX##p_left;                        \
              char *const PFX##p2 = PFX##p_right;                       \
              QUICKSORTS_COMMON__SWAP (PFX);                            \
              if (PFX##p_left == PFX##p_pivot)                          \
                {                                                       \
                  /* Keep the pivot between p_left and p_right. */      \
                  size_t PFX##half_diff =                               \
                    (((uintptr_t) PFX##p_right -                        \
                      (uintptr_t) PFX##p_left) / (PFX##elemsz << 1));   \
                  PFX##p_pivot =                                        \
                    PFX##p_right - (PFX##elemsz * PFX##half_diff);      \
                  PFX##i_pivot =                                        \
                    ((uintptr_t) PFX##p_pivot - (uintptr_t) PFX##arr)   \
                    / PFX##elemsz;                                      \
                  char *const PFX##p1 = PFX##p_pivot;                   \
                  char *const PFX##p2 = PFX##p_right;                   \
                  QUICKSORTS_COMMON__SWAP (PFX);                        \
                  PFX##p_left += PFX##elemsz;                           \
                }                                                       \
              else if (PFX##p_right == PFX##p_pivot)                    \
                {                                                       \
                  /* Keep the pivot between p_left and p_right. */      \
                  size_t PFX##half_diff =                               \
                    (((uintptr_t) PFX##p_right -                        \
                      (uintptr_t) PFX##p_left) / (PFX##elemsz << 1));   \
                  PFX##p_pivot =                                        \
                    PFX##p_left + (PFX##elemsz * PFX##half_diff);       \
                  PFX##i_pivot =                                        \
                    ((uintptr_t) PFX##p_pivot - (uintptr_t) PFX##arr)   \
                    / PFX##elemsz;                                      \
                  char *const PFX##p1 = PFX##p_left;                    \
                  char *const PFX##p2 = PFX##p_pivot;                   \
                  QUICKSORTS_COMMON__SWAP (PFX);                        \
                  PFX##p_right -= PFX##elemsz;                          \
                }                                                       \
              else                                                      \
                {                                                       \
                  PFX##p_left += PFX##elemsz;                           \
                  PFX##p_right -= PFX##elemsz;                          \
                }                                                       \
            }                                                           \
        }                                                               \
      while (PFX##p_left != PFX##p_right);                              \
    }                                                                   \
  while (0)

#ifndef QUICKSORTS__UNSTABLE_QUICKSORT__SMALL
// /* FIXME: TUNE THIS */ /* FIXME: TUNE THIS */ /* FIXME: TUNE THIS */ /* FIXME: TUNE THIS */ /* FIXME: TUNE THIS */
#define QUICKSORTS__UNSTABLE_QUICKSORT__SMALL 80
#endif

#define QUICKSORTS__UNSTABLE_QUICKSORT__QUICKSORT(PFX)                  \
  do                                                                    \
    {                                                                   \
      if (2 <= PFX##nmemb)                                              \
        {                                                               \
          size_t PFX##i_pivot;                                          \
          char *PFX##p_pivot;                                           \
                                                                        \
          QUICKSORTS_COMMON__STK_MAKE (PFX);                            \
          QUICKSORTS_COMMON__STK_PUSH (PFX, PFX##arr, PFX##nmemb);      \
          do                                                            \
            {                                                           \
              QUICKSORTS_COMMON__STK_POP (PFX);                         \
              if (PFX##nmemb <= QUICKSORTS__UNSTABLE_QUICKSORT__SMALL)  \
                QUICKSORTS__UNSTABLE_QUICKSORT__INSERTION_SORT (PFX);   \
              else                                                      \
                {                                                       \
                  QUICKSORTS__UNSTABLE_QUICKSORT__PARTITION (PFX);      \
                                                                        \
                  /* Push the larger part of the partition first. */    \
                  /* Otherwise the stack may overflow.            */    \
                                                                        \
                  size_t PFX##n_le = PFX##i_pivot;                      \
                  size_t PFX##n_ge = PFX##nmemb - 1 - PFX##i_pivot;     \
                  if (PFX##n_le == 0)                                   \
                    QUICKSORTS_COMMON__STK_PUSH                         \
                      (PFX, PFX##p_pivot + PFX##elemsz, PFX##n_ge);     \
                  else if (PFX##n_le < PFX##n_ge)                       \
                    {                                                   \
                      QUICKSORTS_COMMON__STK_PUSH                       \
                        (PFX, PFX##p_pivot + PFX##elemsz, PFX##n_ge);   \
                      QUICKSORTS_COMMON__STK_PUSH                       \
                        (PFX, PFX##arr, PFX##n_le);                     \
                    }                                                   \
                  else if (PFX##n_ge == 0)                              \
                    QUICKSORTS_COMMON__STK_PUSH                         \
                      (PFX, PFX##arr, PFX##n_le);                       \
                  else                                                  \
                    {                                                   \
                      QUICKSORTS_COMMON__STK_PUSH                       \
                        (PFX, PFX##arr, PFX##n_le);                     \
                      QUICKSORTS_COMMON__STK_PUSH                       \
                        (PFX, PFX##p_pivot + PFX##elemsz, PFX##n_ge);   \
                    }                                                   \
                }                                                       \
            }                                                           \
          while (PFX##stk_depth != 0);                                  \
        }                                                               \
    }                                                                   \
  while (0)

#endif /* QUICKSORTS__UNSTABLE_QUICKSORT_H__HEADER_GUARD__ */
