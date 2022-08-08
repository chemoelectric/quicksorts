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

#define QUICKSORTS__UNSTABLE_QUICKSORT__MAKE_AN_ORDERED_PREFIX(PFX, \
                                                               LT)  \
  do                                                                \
    {                                                               \
      PFX##pfx_len = 2;                                             \
      char *PFX##p = PFX##arr + (PFX##elemsz * 2);                  \
                                                                    \
      if (!(LT ((const void *) (PFX##arr + PFX##elemsz),            \
                (const void *) PFX##arr)))                          \
        {                                                           \
          /* Non-decreasing order. */                               \
          while (PFX##pfx_len < PFX##nmemb &&                       \
                 !(LT ((void *) PFX##p,                             \
                       (void *) (PFX##p - PFX##elemsz))))           \
            {                                                       \
              PFX##pfx_len += 1;                                    \
              PFX##p += PFX##elemsz;                                \
            }                                                       \
        }                                                           \
      else                                                          \
        {                                                           \
          /* Decreasing order. This branch sorts unstably. */       \
          while (PFX##pfx_len < PFX##nmemb &&                       \
                 !(LT ((void *) (PFX##p - PFX##elemsz),             \
                       (void *) PFX##p)))                           \
            {                                                       \
              PFX##pfx_len += 1;                                    \
              PFX##p += PFX##elemsz;                                \
            }                                                       \
          quicksorts_common__reverse_prefix                         \
            (PFX##arr, PFX##pfx_len, PFX##elemsz);                  \
        }                                                           \
    }                                                               \
  while (0)

#define QUICKSORTS__UNSTABLE_QUICKSORT__INSERTION_SORT(PFX, LT,     \
                                                       SMALL_SIZE)  \
  QUICKSORTS_COMMON__INSERTION_SORT                                 \
  (PFX, LT, SMALL_SIZE,                                             \
   QUICKSORTS__UNSTABLE_QUICKSORT__MAKE_AN_ORDERED_PREFIX)

#define QUICKSORTS__UNSTABLE_QUICKSORT__GAP_PASS(PFX, LT, GAP)      \
  do                                                                \
    {                                                               \
      const size_t PFX##gap = (GAP);                                \
      for (size_t PFX##i = PFX##gap;                                \
           PFX##i < PFX##nmemb;                                     \
           PFX##i += 1)                                             \
        {                                                           \
          char *PFX##p_right = PFX##arr + (PFX##elemsz * PFX##i);   \
          size_t PFX##j = PFX##i;                                   \
          while (PFX##gap <= PFX##j &&                              \
                 (LT (PFX##p_right,                                 \
                      (PFX##arr +                                   \
                       (PFX##elemsz * (PFX##j - PFX##gap))))))      \
            PFX##j -= PFX##gap;                                     \
          char *PFX##p_left = PFX##arr + (PFX##elemsz * PFX##j);    \
          quicksorts_common__subcirculate_right_with_gap            \
            (PFX##p_left, PFX##p_right, PFX##elemsz, PFX##gap);     \
        }                                                           \
    }                                                               \
  while (0)

#define QUICKSORTS__UNSTABLE_QUICKSORT__SHELL_SORT(PFX, LT,         \
                                                   SMALL_SIZE)      \
  do                                                                \
    {                                                               \
      /* The famous gap sequence of Marcin Ciura and */             \
      /* Roman Dovgopol: https://oeis.org/A102549    */             \
      if ((SMALL_SIZE) >= 1750 && PFX##nmemb >= 1750)               \
        QUICKSORTS__UNSTABLE_QUICKSORT__GAP_PASS(PFX, LT, 1750);    \
      if ((SMALL_SIZE) >= 701 && PFX##nmemb >= 701)                 \
        QUICKSORTS__UNSTABLE_QUICKSORT__GAP_PASS(PFX, LT, 701);     \
      if ((SMALL_SIZE) >= 301 && PFX##nmemb >= 301)                 \
        QUICKSORTS__UNSTABLE_QUICKSORT__GAP_PASS(PFX, LT, 301);     \
      if ((SMALL_SIZE) >= 132 && PFX##nmemb >= 132)                 \
        QUICKSORTS__UNSTABLE_QUICKSORT__GAP_PASS(PFX, LT, 132);     \
      if ((SMALL_SIZE) >= 57 && PFX##nmemb >= 57)                   \
        QUICKSORTS__UNSTABLE_QUICKSORT__GAP_PASS(PFX, LT, 57);      \
      if ((SMALL_SIZE) >= 23 && PFX##nmemb >= 23)                   \
        QUICKSORTS__UNSTABLE_QUICKSORT__GAP_PASS(PFX, LT, 23);      \
      if ((SMALL_SIZE) >= 10 && PFX##nmemb >= 10)                   \
        QUICKSORTS__UNSTABLE_QUICKSORT__GAP_PASS(PFX, LT, 10);      \
      if ((SMALL_SIZE) >= 4 && PFX##nmemb >= 4)                     \
        QUICKSORTS__UNSTABLE_QUICKSORT__GAP_PASS(PFX, LT, 4);       \
      QUICKSORTS__UNSTABLE_QUICKSORT__GAP_PASS(PFX, LT, 1);         \
    }                                                               \
  while (0)

#define QUICKSORTS__UNSTABLE_QUICKSORT__MOVE_RIGHTWARDS(PFX, LT)    \
  do                                                                \
    {                                                               \
      while (PFX##p_left != PFX##p_pivot &&                         \
             (LT ((void *) PFX##p_left, (void *) PFX##p_pivot)))    \
        PFX##p_left += PFX##elemsz;                                 \
    }                                                               \
  while (0)

#define QUICKSORTS__UNSTABLE_QUICKSORT__MOVE_LEFTWARDS(PFX, LT)     \
  do                                                                \
    {                                                               \
      while (PFX##p_right != PFX##p_pivot &&                        \
             (LT ((void *) PFX##p_pivot, (void *) PFX##p_right)))   \
        PFX##p_right -= PFX##elemsz;                                \
    }                                                               \
  while (0)

#define QUICKSORTS__UNSTABLE_QUICKSORT__PARTITION(PFX, LT,              \
                                                  PIVOT_SELECTION)      \
  do                                                                    \
    {                                                                   \
      PIVOT_SELECTION (PFX##arr, PFX##nmemb, PFX##elemsz, LT,           \
                       PFX##i_pivot);                                   \
      PFX##p_pivot = PFX##arr + (PFX##elemsz * PFX##i_pivot);           \
      char *PFX##p_left = PFX##arr;                                     \
      char *PFX##p_right =                                              \
        PFX##arr + (PFX##elemsz * (PFX##nmemb - 1));                    \
      do                                                                \
        {                                                               \
          QUICKSORTS__UNSTABLE_QUICKSORT__MOVE_RIGHTWARDS (PFX, LT);    \
          QUICKSORTS__UNSTABLE_QUICKSORT__MOVE_LEFTWARDS (PFX, LT);     \
          if (PFX##p_left != PFX##p_right)                              \
            {                                                           \
              quicksorts_common__elem_swap                              \
                (PFX##p_left, PFX##p_right, PFX##elemsz);               \
              if (PFX##p_left == PFX##p_pivot)                          \
                {                                                       \
                  /* Keep the pivot between p_left and p_right. */      \
                  size_t PFX##half_diff =                               \
                    (PFX##p_right - PFX##p_left) / (PFX##elemsz << 1);  \
                  PFX##p_pivot =                                        \
                    PFX##p_right - (PFX##elemsz * PFX##half_diff);      \
                  PFX##i_pivot =                                        \
                    (PFX##p_pivot - PFX##arr) / PFX##elemsz;            \
                  quicksorts_common__elem_swap                          \
                    (PFX##p_pivot, PFX##p_right, PFX##elemsz);          \
                  PFX##p_left += PFX##elemsz;                           \
                }                                                       \
              else if (PFX##p_right == PFX##p_pivot)                    \
                {                                                       \
                  /* Keep the pivot between p_left and p_right. */      \
                  size_t PFX##half_diff =                               \
                    (PFX##p_right - PFX##p_left) / (PFX##elemsz << 1);  \
                  PFX##p_pivot =                                        \
                    PFX##p_left + (PFX##elemsz * PFX##half_diff);       \
                  PFX##i_pivot =                                        \
                    (PFX##p_pivot - PFX##arr) / PFX##elemsz;            \
                  quicksorts_common__elem_swap                          \
                    (PFX##p_left, PFX##p_pivot, PFX##elemsz);           \
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

#define QUICKSORTS__UNSTABLE_QUICKSORT__QUICKSORT(PFX, LT, SMALL_SIZE,  \
                                                  SMALL_SORT,           \
                                                  PIVOT_SELECTION)      \
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
              if (PFX##nmemb <= (SMALL_SIZE))                           \
                {                                                       \
                  SMALL_SORT (PFX, LT, SMALL_SIZE);                     \
                }                                                       \
              else                                                      \
                {                                                       \
                  QUICKSORTS__UNSTABLE_QUICKSORT__PARTITION             \
                    (PFX, LT, PIVOT_SELECTION);                         \
                                                                        \
                  /* Push the larger part of the partition first. */    \
                  /* Otherwise the stack may overflow.            */    \
                                                                        \
                  size_t PFX##n_le = PFX##i_pivot;                      \
                  size_t PFX##n_ge = PFX##nmemb - 1 - PFX##i_pivot;     \
                  if (PFX##n_le == 0)                                   \
                    {                                                   \
                      QUICKSORTS_COMMON__STK_PUSH                       \
                        (PFX, PFX##p_pivot + PFX##elemsz, PFX##n_ge);   \
                    }                                                   \
                  else if (PFX##n_le < PFX##n_ge)                       \
                    {                                                   \
                      QUICKSORTS_COMMON__STK_PUSH                       \
                        (PFX, PFX##p_pivot + PFX##elemsz, PFX##n_ge);   \
                      QUICKSORTS_COMMON__STK_PUSH                       \
                        (PFX, PFX##arr, PFX##n_le);                     \
                    }                                                   \
                  else if (PFX##n_ge == 0)                              \
                    {                                                   \
                      QUICKSORTS_COMMON__STK_PUSH                       \
                        (PFX, PFX##arr, PFX##n_le);                     \
                    }                                                   \
                  else                                                  \
                    {                                                   \
                      QUICKSORTS_COMMON__STK_PUSH                       \
                        (PFX, PFX##arr, PFX##n_le);                     \
                      QUICKSORTS_COMMON__STK_PUSH                       \
                        (PFX, PFX##p_pivot + PFX##elemsz, PFX##n_ge);   \
                    }                                                   \
                }                                                       \
              }                                                         \
          while (PFX##stk_depth != 0);                                  \
        }                                                               \
    }                                                                   \
  while (0)

#define UNSTABLE_QUICKSORT_CONFIGURABLE(BASE, NMEMB, ELEMSZ, LT,    \
                                        SMALL_SIZE, SMALL_SORT,     \
                                        PIVOT_SELECTION)            \
  do                                                                \
    {                                                               \
      char *quicksorts__unstable_quicksort__arr =                   \
        (void *) (BASE);                                            \
      size_t quicksorts__unstable_quicksort__nmemb =                \
        (size_t) (NMEMB);                                           \
      size_t quicksorts__unstable_quicksort__elemsz =               \
        (size_t) (ELEMSZ);                                          \
                                                                    \
      QUICKSORTS__UNSTABLE_QUICKSORT__QUICKSORT                     \
        (quicksorts__unstable_quicksort__,                          \
         LT, SMALL_SIZE, SMALL_SORT, PIVOT_SELECTION);              \
    }                                                               \
  while (0)

#endif /* QUICKSORTS__UNSTABLE_QUICKSORT_H__HEADER_GUARD__ */
