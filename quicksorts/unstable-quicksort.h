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

#define QUICKSORTS__UNSTABLE_QUICKSORT__MAKE_AN_ORDERED_PREFIX__TYPED(PFX, T, LT) \
  do                                                                    \
    {                                                                   \
      PFX##pfx_len = 2;                                                 \
      T *PFX##p = PFX##arr + 2;                                         \
                                                                        \
      if (!(LT (PFX##arr + 1, PFX##arr)))                               \
        {                                                               \
          /* Non-decreasing order. */                                   \
          while (PFX##pfx_len < PFX##nmemb &&                           \
                 !(LT (PFX##p, PFX##p - 1)))                            \
            {                                                           \
              PFX##pfx_len += 1;                                        \
              PFX##p += 1;                                              \
            }                                                           \
        }                                                               \
      else                                                              \
        {                                                               \
          /* Decreasing order. This branch sorts unstably. */           \
          while (PFX##pfx_len < PFX##nmemb &&                           \
                 !(LT (PFX##p - 1, PFX##p)))                            \
            {                                                           \
              PFX##pfx_len += 1;                                        \
              PFX##p += 1;                                              \
            }                                                           \
          QUICKSORTS_COMMON__REVERSE_PREFIX__TYPED                      \
            (PFX, T, PFX##arr, PFX##pfx_len);                           \
        }                                                               \
    }                                                                   \
  while (0)

#define QUICKSORTS__UNSTABLE_QUICKSORT__INSERTION_SORT(PFX, BASE,   \
                                                       NMEMB,       \
                                                       ELEMSZ, LT,  \
                                                       SMALL_SIZE)  \
  do                                                                \
    {                                                               \
      char *PFX##insertion_sort__arr = (void *) (BASE);             \
      size_t PFX##insertion_sort__nmemb = (size_t) (NMEMB);         \
      size_t PFX##insertion_sort__elemsz = (size_t) (ELEMSZ);       \
      QUICKSORTS_COMMON__INSERTION_SORT                             \
        (PFX##insertion_sort__, LT, SMALL_SIZE,                     \
         QUICKSORTS__UNSTABLE_QUICKSORT__MAKE_AN_ORDERED_PREFIX);   \
    }                                                               \
  while (0)

#define QUICKSORTS__UNSTABLE_QUICKSORT__INSERTION_SORT__TYPED(PFX, T,   \
                                                              BASE,     \
                                                              NMEMB,    \
                                                              LT,       \
                                                              SMALL_SIZE) \
  do                                                                    \
    {                                                                   \
      T *PFX##insertion_sort__arr = (T *) (BASE);                       \
      size_t PFX##insertion_sort__nmemb = (size_t) (NMEMB);             \
      QUICKSORTS_COMMON__INSERTION_SORT__TYPED                          \
        (PFX##insertion_sort__, T, LT, SMALL_SIZE,                      \
         QUICKSORTS__UNSTABLE_QUICKSORT__MAKE_AN_ORDERED_PREFIX__TYPED); \
    }                                                                   \
  while (0)

#define QUICKSORTS__UNSTABLE_QUICKSORT__GAP_PASS(PFX, LT, GAP)          \
  do                                                                    \
    {                                                                   \
      const size_t PFX##gap = (GAP);                                    \
      const size_t PFX##chargap = PFX##elemsz * PFX##gap;               \
      char *PFX##p_lstop = PFX##arr + PFX##chargap;                     \
      char *PFX##p_rstop = PFX##arr + (PFX##elemsz * PFX##nmemb);       \
      for (char *PFX##p_right = PFX##p_lstop;                           \
           PFX##p_right != PFX##p_rstop;                                \
           PFX##p_right += PFX##elemsz)                                 \
        {                                                               \
          char *PFX##p_left = PFX##p_right;                             \
          while (PFX##p_lstop <= PFX##p_left &&                         \
                 (LT ((const void *) PFX##p_right,                      \
                      (const void *) (PFX##p_left - PFX##chargap))))    \
            PFX##p_left -= PFX##chargap;                                \
          quicksorts_common__subcirculate_right_with_gap                \
            (PFX##p_left, PFX##p_right, PFX##elemsz, PFX##gap);         \
        }                                                               \
    }                                                                   \
  while (0)

#define QUICKSORTS__UNSTABLE_QUICKSORT__GAP_PASS__TYPED(PFX, T, LT, \
                                                        GAP)        \
  do                                                                \
    {                                                               \
      const size_t PFX##gap = (GAP);                                \
      T *PFX##p_lstop = PFX##arr + PFX##gap;                        \
      T *PFX##p_rstop = PFX##arr + PFX##nmemb;                      \
      for (T *PFX##p_right = PFX##p_lstop;                          \
           PFX##p_right != PFX##p_rstop;                            \
           PFX##p_right += 1)                                       \
        {                                                           \
          T *PFX##p_left = PFX##p_right;                            \
          while (PFX##p_lstop <= PFX##p_left &&                     \
                 (LT (PFX##p_right, PFX##p_left - PFX##gap)))       \
            PFX##p_left -= PFX##gap;                                \
          QUICKSORTS_COMMON__SUBCIRCULATE_RIGHT_WITH_GAP__TYPED     \
            (PFX, T, PFX##p_left, PFX##p_right, PFX##gap);          \
        }                                                           \
    }                                                               \
  while (0)

#define QUICKSORTS__UNSTABLE_QUICKSORT__SHELL_SORT(PFX, BASE,       \
                                                   NMEMB, ELEMSZ,   \
                                                   LT, SMALL_SIZE)  \
  do                                                                \
    {                                                               \
      char *PFX##shell_sort__arr = (void *) (BASE);                 \
      size_t PFX##shell_sort__nmemb = (size_t) (NMEMB);             \
      size_t PFX##shell_sort__elemsz = (size_t) (ELEMSZ);           \
                                                                    \
      /* The famous gap sequence of Marcin Ciura and */             \
      /* Roman Dovgopol: https://oeis.org/A102549    */             \
      if ((SMALL_SIZE) >= 1750 && PFX##shell_sort__nmemb >= 1750)   \
        QUICKSORTS__UNSTABLE_QUICKSORT__GAP_PASS                    \
          (PFX##shell_sort__, LT, 1750);                            \
      if ((SMALL_SIZE) >= 701 && PFX##shell_sort__nmemb >= 701)     \
        QUICKSORTS__UNSTABLE_QUICKSORT__GAP_PASS                    \
          (PFX##shell_sort__, LT, 701);                             \
      if ((SMALL_SIZE) >= 301 && PFX##shell_sort__nmemb >= 301)     \
        QUICKSORTS__UNSTABLE_QUICKSORT__GAP_PASS                    \
          (PFX##shell_sort__, LT, 301);                             \
      if ((SMALL_SIZE) >= 132 && PFX##shell_sort__nmemb >= 132)     \
        QUICKSORTS__UNSTABLE_QUICKSORT__GAP_PASS                    \
          (PFX##shell_sort__, LT, 132);                             \
      if ((SMALL_SIZE) >= 57 && PFX##shell_sort__nmemb >= 57)       \
        QUICKSORTS__UNSTABLE_QUICKSORT__GAP_PASS                    \
          (PFX##shell_sort__, LT, 57);                              \
      if ((SMALL_SIZE) >= 23 && PFX##shell_sort__nmemb >= 23)       \
        QUICKSORTS__UNSTABLE_QUICKSORT__GAP_PASS                    \
          (PFX##shell_sort__, LT, 23);                              \
      if ((SMALL_SIZE) >= 10 && PFX##shell_sort__nmemb >= 10)       \
        QUICKSORTS__UNSTABLE_QUICKSORT__GAP_PASS                    \
          (PFX##shell_sort__, LT, 10);                              \
      if ((SMALL_SIZE) >= 4 && PFX##shell_sort__nmemb >= 4)         \
        QUICKSORTS__UNSTABLE_QUICKSORT__GAP_PASS                    \
          (PFX##shell_sort__, LT, 4);                               \
      QUICKSORTS__UNSTABLE_QUICKSORT__GAP_PASS                      \
        (PFX##shell_sort__, LT, 1);                                 \
    }                                                               \
  while (0)

#define QUICKSORTS__UNSTABLE_QUICKSORT__SHELL_SORT__TYPED(PFX, T,       \
                                                          BASE,         \
                                                          NMEMB, LT,    \
                                                          SMALL_SIZE)   \
  do                                                                \
    {                                                               \
      T *PFX##shell_sort__arr = (T *) (BASE);                       \
      size_t PFX##shell_sort__nmemb = (size_t) (NMEMB);             \
                                                                    \
      /* The famous gap sequence of Marcin Ciura and */             \
      /* Roman Dovgopol: https://oeis.org/A102549    */             \
      if ((SMALL_SIZE) >= 1750 && PFX##shell_sort__nmemb >= 1750)   \
        QUICKSORTS__UNSTABLE_QUICKSORT__GAP_PASS__TYPED             \
          (PFX##shell_sort__, T, LT, 1750);                         \
      if ((SMALL_SIZE) >= 701 && PFX##shell_sort__nmemb >= 701)     \
        QUICKSORTS__UNSTABLE_QUICKSORT__GAP_PASS__TYPED             \
          (PFX##shell_sort__, T, LT, 701);                          \
      if ((SMALL_SIZE) >= 301 && PFX##shell_sort__nmemb >= 301)     \
        QUICKSORTS__UNSTABLE_QUICKSORT__GAP_PASS__TYPED             \
          (PFX##shell_sort__, T, LT, 301);                          \
      if ((SMALL_SIZE) >= 132 && PFX##shell_sort__nmemb >= 132)     \
        QUICKSORTS__UNSTABLE_QUICKSORT__GAP_PASS__TYPED             \
          (PFX##shell_sort__, T, LT, 132);                          \
      if ((SMALL_SIZE) >= 57 && PFX##shell_sort__nmemb >= 57)       \
        QUICKSORTS__UNSTABLE_QUICKSORT__GAP_PASS__TYPED             \
          (PFX##shell_sort__, T, LT, 57);                           \
      if ((SMALL_SIZE) >= 23 && PFX##shell_sort__nmemb >= 23)       \
        QUICKSORTS__UNSTABLE_QUICKSORT__GAP_PASS__TYPED             \
          (PFX##shell_sort__, T, LT, 23);                           \
      if ((SMALL_SIZE) >= 10 && PFX##shell_sort__nmemb >= 10)       \
        QUICKSORTS__UNSTABLE_QUICKSORT__GAP_PASS__TYPED             \
          (PFX##shell_sort__, T, LT, 10);                           \
      if ((SMALL_SIZE) >= 4 && PFX##shell_sort__nmemb >= 4)         \
        QUICKSORTS__UNSTABLE_QUICKSORT__GAP_PASS__TYPED             \
          (PFX##shell_sort__, T, LT, 4);                            \
      QUICKSORTS__UNSTABLE_QUICKSORT__GAP_PASS__TYPED               \
        (PFX##shell_sort__, T, LT, 1);                              \
    }                                                               \
  while (0)

#define QUICKSORTS__UNSTABLE_QUICKSORT__MOVE_RIGHTWARDS(PFX, LT)    \
  do                                                                \
    {                                                               \
      while (PFX##p_left != PFX##p_right &&                         \
             (LT ((const void *) PFX##p_left,                       \
                  (const void *) PFX##p_pivot)))                    \
        PFX##p_left += PFX##elemsz;                                 \
    }                                                               \
  while (0)

#define QUICKSORTS__UNSTABLE_QUICKSORT__MOVE_LEFTWARDS(PFX, LT) \
  do                                                            \
    {                                                           \
      while (PFX##p_left != PFX##p_right &&                     \
             (LT ((const void *) PFX##p_pivot,                  \
                  (const void *) PFX##p_right)))                \
        PFX##p_right -= PFX##elemsz;                            \
    }                                                           \
  while (0)

#define QUICKSORTS__UNSTABLE_QUICKSORT__MOVE_RIGHTWARDS__TYPED(PFX, T,  \
                                                               LT)      \
  do                                                                    \
    {                                                                   \
      while (PFX##p_left != PFX##p_right &&                             \
             (LT (PFX##p_left, PFX##p_pivot)))                          \
        PFX##p_left += 1;                                               \
    }                                                                   \
  while (0)

#define QUICKSORTS__UNSTABLE_QUICKSORT__MOVE_LEFTWARDS__TYPED(PFX, T,   \
                                                              LT)       \
  do                                                                    \
    {                                                                   \
      while (PFX##p_left != PFX##p_right &&                             \
             (LT (PFX##p_pivot, PFX##p_right)))                         \
        PFX##p_right -= 1;                                              \
    }                                                                   \
  while (0)

#define QUICKSORTS__UNSTABLE_QUICKSORT__PARTITION(PFX, LT,              \
                                                  PIVOT_SELECTION)      \
  do                                                                    \
    {                                                                   \
      PIVOT_SELECTION (PFX##arr, PFX##nmemb, PFX##elemsz, LT,           \
                       PFX##i_pivot);                                   \
      PFX##p_pivot = PFX##arr + (PFX##elemsz * PFX##i_pivot);           \
                                                                        \
      /* Put the pivot in the middle, so it will be as near to */       \
      /* other elements as possible.                           */       \
      char *PFX##p_middle =                                             \
        PFX##arr + ((PFX##nmemb >> 1) * PFX##elemsz);                   \
      quicksorts_common__elem_swap                                      \
        ((void *) PFX##p_pivot, (void *) PFX##p_middle, PFX##elemsz);   \
      PFX##p_pivot = PFX##p_middle;                                     \
                                                                        \
      char *PFX##p_left = PFX##arr;                                     \
      char *PFX##p_right = PFX##arr + ((PFX##nmemb - 1) * PFX##elemsz); \
                                                                        \
      QUICKSORTS__UNSTABLE_QUICKSORT__MOVE_RIGHTWARDS (PFX, LT);        \
      QUICKSORTS__UNSTABLE_QUICKSORT__MOVE_LEFTWARDS (PFX, LT);         \
                                                                        \
      while (PFX##p_left != PFX##p_right)                               \
        {                                                               \
          quicksorts_common__elem_swap                                  \
            ((void *) PFX##p_left, (void *) PFX##p_right,               \
             PFX##elemsz);                                              \
                                                                        \
          /* The pivot’s position may have been changed by the */       \
          /* swap.                                             */       \
          if (PFX##p_pivot == PFX##p_left)                              \
            PFX##p_pivot = PFX##p_right;                                \
          else if (PFX##p_pivot == PFX##p_right)                        \
            PFX##p_pivot = PFX##p_left;                                 \
                                                                        \
          PFX##p_left += PFX##elemsz;                                   \
                                                                        \
          if (PFX##p_left != PFX##p_right)                              \
            PFX##p_right -= PFX##elemsz;                                \
                                                                        \
          QUICKSORTS__UNSTABLE_QUICKSORT__MOVE_RIGHTWARDS (PFX, LT);    \
          QUICKSORTS__UNSTABLE_QUICKSORT__MOVE_LEFTWARDS (PFX, LT);     \
        }                                                               \
                                                                        \
      /* Put the pivot between the two parts of the partition. */       \
      if (LT ((const void *) PFX##p_pivot,                              \
              (const void *) PFX##p_right))                             \
        {                                                               \
          if (PFX##p_pivot < PFX##p_right)                              \
            {                                                           \
              quicksorts_common__elem_swap                              \
                ((void *) PFX##p_pivot,                                 \
                 (void *) (PFX##p_right - PFX##elemsz),                 \
                 PFX##elemsz);                                          \
              PFX##p_pivot = PFX##p_right - PFX##elemsz;                \
            }                                                           \
          else                                                          \
            {                                                           \
              quicksorts_common__elem_swap                              \
                ((void *) PFX##p_pivot,                                 \
                 (void *) PFX##p_right,                                 \
                 PFX##elemsz);                                          \
              PFX##p_pivot = PFX##p_right;                              \
            }                                                           \
        }                                                               \
      else                                                              \
        {                                                               \
          if (PFX##p_right < PFX##p_pivot)                              \
            {                                                           \
              quicksorts_common__elem_swap                              \
                ((void *) PFX##p_pivot,                                 \
                 (void *) (PFX##p_right + PFX##elemsz),                 \
                 PFX##elemsz);                                          \
              PFX##p_pivot = PFX##p_right + PFX##elemsz;                \
            }                                                           \
          else                                                          \
            {                                                           \
              quicksorts_common__elem_swap                              \
                ((void *) PFX##p_pivot,                                 \
                 (void *) PFX##p_right,                                 \
                 PFX##elemsz);                                          \
              PFX##p_pivot = PFX##p_right;                              \
            }                                                           \
        }                                                               \
                                                                        \
      PFX##i_pivot = (PFX##p_pivot - PFX##arr) / PFX##elemsz;           \
    }                                                                   \
  while (0)

#define QUICKSORTS__UNSTABLE_QUICKSORT__PARTITION__TYPED(PFX, T, LT,    \
                                                         PIVOT_SELECTION) \
  do                                                                    \
    {                                                                   \
      PIVOT_SELECTION (PFX##arr, PFX##nmemb, sizeof (T), LT,            \
                       PFX##i_pivot);                                   \
      PFX##p_pivot = PFX##arr + PFX##i_pivot;                           \
                                                                        \
      /* Put the pivot in the middle, so it will be as near to */       \
      /* other elements as possible.                           */       \
      T *PFX##p_middle = PFX##arr + (PFX##nmemb >> 1);                  \
      QUICKSORTS_COMMON__ELEM_SWAP__TYPED                               \
        (PFX, T, PFX##p_pivot, PFX##p_middle);                          \
      PFX##p_pivot = PFX##p_middle;                                     \
                                                                        \
      T *PFX##p_left = PFX##arr;                                        \
      T *PFX##p_right = PFX##arr + (PFX##nmemb - 1);                    \
                                                                        \
      QUICKSORTS__UNSTABLE_QUICKSORT__MOVE_RIGHTWARDS__TYPED            \
        (PFX, T, LT);                                                   \
      QUICKSORTS__UNSTABLE_QUICKSORT__MOVE_LEFTWARDS__TYPED             \
        (PFX, T, LT);                                                   \
                                                                        \
      while (PFX##p_left != PFX##p_right)                               \
        {                                                               \
          QUICKSORTS_COMMON__ELEM_SWAP__TYPED                           \
            (PFX, T, PFX##p_left, PFX##p_right);                        \
                                                                        \
          /* The pivot’s position may have been changed by the */       \
          /* swap.                                             */       \
          if (PFX##p_pivot == PFX##p_left)                              \
            PFX##p_pivot = PFX##p_right;                                \
          else if (PFX##p_pivot == PFX##p_right)                        \
            PFX##p_pivot = PFX##p_left;                                 \
                                                                        \
          PFX##p_left += 1;                                             \
                                                                        \
          if (PFX##p_left != PFX##p_right)                              \
            PFX##p_right -= 1;                                          \
                                                                        \
          QUICKSORTS__UNSTABLE_QUICKSORT__MOVE_RIGHTWARDS__TYPED        \
            (PFX, T, LT);                                               \
          QUICKSORTS__UNSTABLE_QUICKSORT__MOVE_LEFTWARDS__TYPED         \
            (PFX, T, LT);                                               \
        }                                                               \
                                                                        \
      /* Put the pivot between the two parts of the partition. */       \
      if (LT (PFX##p_pivot, PFX##p_right))                              \
        {                                                               \
          if (PFX##p_pivot < PFX##p_right)                              \
            {                                                           \
              QUICKSORTS_COMMON__ELEM_SWAP__TYPED                       \
                (PFX, T, PFX##p_pivot, PFX##p_right - 1);               \
              PFX##p_pivot = PFX##p_right - 1;                          \
            }                                                           \
          else                                                          \
            {                                                           \
              QUICKSORTS_COMMON__ELEM_SWAP__TYPED                       \
                (PFX, T, PFX##p_pivot, PFX##p_right);                   \
              PFX##p_pivot = PFX##p_right;                              \
            }                                                           \
        }                                                               \
      else                                                              \
        {                                                               \
          if (PFX##p_right < PFX##p_pivot)                              \
            {                                                           \
              QUICKSORTS_COMMON__ELEM_SWAP__TYPED                       \
                (PFX, T, PFX##p_pivot, PFX##p_right + 1);               \
              PFX##p_pivot = PFX##p_right + 1;                          \
            }                                                           \
          else                                                          \
            {                                                           \
              QUICKSORTS_COMMON__ELEM_SWAP__TYPED                       \
                (PFX, T, PFX##p_pivot, PFX##p_right);                   \
              PFX##p_pivot = PFX##p_right;                              \
            }                                                           \
        }                                                               \
                                                                        \
      PFX##i_pivot = PFX##p_pivot - PFX##arr;                           \
    }                                                                   \
  while (0)

#define QUICKSORTS__UNSTABLE_QUICKSORT__QUICKSORT(PFX, BASE, NMEMB,     \
                                                  ELEMSZ, LT,           \
                                                  PIVOT_SELECTION,      \
                                                  SMALL_SIZE,           \
                                                  SMALL_SORT)           \
  do                                                                    \
    {                                                                   \
      char *PFX##arr = (void *) (BASE);                                 \
      size_t PFX##nmemb = (size_t) (NMEMB);                             \
      size_t PFX##elemsz = (size_t) (ELEMSZ);                           \
                                                                        \
      if (0 < PFX##elemsz && 2 <= PFX##nmemb)                           \
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
                  SMALL_SORT (PFX, PFX##arr, PFX##nmemb, PFX##elemsz,   \
                              LT, (SMALL_SIZE));                        \
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
                  if (PFX##n_le < PFX##n_ge)                            \
                    {                                                   \
                      QUICKSORTS_COMMON__STK_PUSH                       \
                        (PFX, PFX##p_pivot + PFX##elemsz, PFX##n_ge);   \
                      if (PFX##n_le != 0)                               \
                        QUICKSORTS_COMMON__STK_PUSH                     \
                          (PFX, PFX##arr, PFX##n_le);                   \
                    }                                                   \
                  else                                                  \
                    {                                                   \
                      QUICKSORTS_COMMON__STK_PUSH                       \
                        (PFX, PFX##arr, PFX##n_le);                     \
                      if (PFX##n_ge != 0)                               \
                        QUICKSORTS_COMMON__STK_PUSH                     \
                          (PFX, PFX##p_pivot + PFX##elemsz, PFX##n_ge); \
                    }                                                   \
                }                                                       \
            }                                                           \
          while (PFX##stk_depth != 0);                                  \
        }                                                               \
    }                                                                   \
  while (0)

#define QUICKSORTS__UNSTABLE_QUICKSORT__QUICKSORT__TYPED(PFX, T, BASE, \
                                                         NMEMB, LT,     \
                                                         PIVOT_SELECTION, \
                                                         SMALL_SIZE,    \
                                                         SMALL_SORT)    \
  do                                                                    \
    {                                                                   \
      T *PFX##arr = (T *) (BASE);                                       \
      size_t PFX##nmemb = (size_t) (NMEMB);                             \
                                                                        \
      if (0 < sizeof (T) && 2 <= PFX##nmemb)                            \
        {                                                               \
          size_t PFX##i_pivot;                                          \
          T *PFX##p_pivot;                                              \
                                                                        \
          QUICKSORTS_COMMON__STK_MAKE (PFX);                            \
          QUICKSORTS_COMMON__STK_PUSH (PFX, PFX##arr, PFX##nmemb);      \
          do                                                            \
            {                                                           \
              QUICKSORTS_COMMON__STK_POP (PFX);                         \
              if (PFX##nmemb <= (SMALL_SIZE))                           \
                {                                                       \
                  SMALL_SORT (PFX, T, PFX##arr, PFX##nmemb, LT,         \
                              (SMALL_SIZE));                            \
                }                                                       \
              else                                                      \
                {                                                       \
                  QUICKSORTS__UNSTABLE_QUICKSORT__PARTITION__TYPED      \
                    (PFX, T, LT, PIVOT_SELECTION);                      \
                                                                        \
                  /* Push the larger part of the partition first. */    \
                  /* Otherwise the stack may overflow.            */    \
                                                                        \
                  size_t PFX##n_le = PFX##i_pivot;                      \
                  size_t PFX##n_ge = PFX##nmemb - 1 - PFX##i_pivot;     \
                  if (PFX##n_le < PFX##n_ge)                            \
                    {                                                   \
                      QUICKSORTS_COMMON__STK_PUSH                       \
                        (PFX, PFX##p_pivot + 1, PFX##n_ge);             \
                      if (PFX##n_le != 0)                               \
                        QUICKSORTS_COMMON__STK_PUSH                     \
                          (PFX, PFX##arr, PFX##n_le);                   \
                    }                                                   \
                  else                                                  \
                    {                                                   \
                      QUICKSORTS_COMMON__STK_PUSH                       \
                        (PFX, PFX##arr, PFX##n_le);                     \
                      if (PFX##n_ge != 0)                               \
                        QUICKSORTS_COMMON__STK_PUSH                     \
                          (PFX, PFX##p_pivot + 1, PFX##n_ge);           \
                    }                                                   \
                }                                                       \
            }                                                           \
          while (PFX##stk_depth != 0);                                  \
        }                                                               \
    }                                                                   \
  while (0)

#define UNSTABLE_QUICKSORT_7ARGS(BASE, NMEMB, ELEMSZ, LT,           \
                                 PIVOT_SELECTION,                   \
                                 SMALL_SIZE, SMALL_SORT)            \
  do                                                                \
    {                                                               \
      QUICKSORTS__UNSTABLE_QUICKSORT__QUICKSORT                     \
        (quicksorts__unstable_quicksort__,                          \
         (BASE), (NMEMB), (ELEMSZ),                                 \
         LT, PIVOT_SELECTION, (SMALL_SIZE), SMALL_SORT);            \
    }                                                               \
  while (0)

#define UNSTABLE_QUICKSORT_TYPED_7ARGS(T, BASE, NMEMB, LT,      \
                                       PIVOT_SELECTION,         \
                                       SMALL_SIZE, SMALL_SORT)  \
  do                                                            \
    {                                                           \
      QUICKSORTS__UNSTABLE_QUICKSORT__QUICKSORT__TYPED          \
        (quicksorts__unstable_quicksort__typed__, T,            \
         (BASE), (NMEMB), LT, PIVOT_SELECTION,                  \
         (SMALL_SIZE), SMALL_SORT);                             \
    }                                                           \
  while (0)

#ifndef UNSTABLE_QUICKSORT__DEFAULT__PIVOT_SELECTION
#define UNSTABLE_QUICKSORT__DEFAULT__PIVOT_SELECTION    \
  QUICKSORTS_COMMON__PIVOT_MEDIAN_OF_THREE
#endif

#ifndef UNSTABLE_QUICKSORT__DEFAULT__SMALL_SIZE
#define UNSTABLE_QUICKSORT__DEFAULT__SMALL_SIZE 80
#endif

#ifndef UNSTABLE_QUICKSORT__DEFAULT__SMALL_SORT
#define UNSTABLE_QUICKSORT__DEFAULT__SMALL_SORT     \
  QUICKSORTS__UNSTABLE_QUICKSORT__INSERTION_SORT
#endif

#ifndef UNSTABLE_QUICKSORT__DEFAULT__SMALL_SORT__TYPED
#define UNSTABLE_QUICKSORT__DEFAULT__SMALL_SORT__TYPED  \
  QUICKSORTS__UNSTABLE_QUICKSORT__INSERTION_SORT__TYPED
#endif

#define UNSTABLE_QUICKSORT_4ARGS(BASE, NMEMB, ELEMSZ, LT)   \
  do                                                        \
    {                                                       \
      QUICKSORTS__UNSTABLE_QUICKSORT__QUICKSORT             \
        (quicksorts__unstable_quicksort__,                  \
         (BASE), (NMEMB), (ELEMSZ), LT,                     \
         UNSTABLE_QUICKSORT__DEFAULT__PIVOT_SELECTION,      \
         (UNSTABLE_QUICKSORT__DEFAULT__SMALL_SIZE),         \
         UNSTABLE_QUICKSORT__DEFAULT__SMALL_SORT);          \
    }                                                       \
  while (0)

#define UNSTABLE_QUICKSORT_TYPED_4ARGS(T, BASE, NMEMB, LT)  \
  do                                                        \
    {                                                       \
      QUICKSORTS__UNSTABLE_QUICKSORT__QUICKSORT__TYPED      \
        (quicksorts__unstable_quicksort__typed__,           \
         T, (BASE), (NMEMB), LT,                            \
         UNSTABLE_QUICKSORT__DEFAULT__PIVOT_SELECTION,      \
         (UNSTABLE_QUICKSORT__DEFAULT__SMALL_SIZE),         \
         UNSTABLE_QUICKSORT__DEFAULT__SMALL_SORT__TYPED);   \
    }                                                       \
  while (0)

#define UNSTABLE_QUICKSORT UNSTABLE_QUICKSORT_4ARGS
#define UNSTABLE_QUICKSORT_TYPED UNSTABLE_QUICKSORT_TYPED_4ARGS

#endif /* QUICKSORTS__UNSTABLE_QUICKSORT_H__HEADER_GUARD__ */
