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

/* #if defined __GNUC__ */
/* #define QUICKSORTS__UNSTABLE_QUICKSORT__MEMSET __builtin_memset */
/* #define QUICKSORTS__UNSTABLE_QUICKSORT__MEMCPY __builtin_memcpy */
/* #else */
/* #define QUICKSORTS__UNSTABLE_QUICKSORT__MEMSET memset */
/* #define QUICKSORTS__UNSTABLE_QUICKSORT__MEMCPY memcpy */
/* #endif */

#define QUICKSORTS__UNSTABLE_QUICKSORT__SWAP(PFX, P1, P2, ELEMSZ)   \
  do                                                                \
    {                                                               \
      char *PFX##p1 = (P1);                                         \
      char *PFX##p2 = (P2);                                         \
      size_t PFX##n = (ELEMSZ);                                     \
      do                                                            \
        {                                                           \
          char PFX##tmp = *PFX##p1;                                 \
          *PFX##p1 = *PFX##p2;                                      \
          *PFX##p2 = PFX##tmp;                                      \
          PFX##n -= 1;                                              \
        }                                                           \
      while (PFX##n != 0);                                          \
    }                                                               \
  while (0)

#define QUICKSORTS__UNSTABLE_QUICKSORT__REVERSE_ARRAY(PFX, ARR, N, ELEMSZ) \
  do                                                                    \
    {                                                                   \
      PFX##p_left = (ARR);                                              \
      PFX##p_right = (ARR) + (((N) - 1) * (ELEMSZ));                    \
      while (PFX##p_left < PFX##p_right)                                \
        {                                                               \
          QUICKSORTS__UNSTABLE_QUICKSORT__SWAP                          \
            (PFX, PFX##p_left, PFX##p_right, (ELEMSZ));                 \
          PFX##p_left += (ELEMSZ);                                      \
          PFX##p_right -= (ELEMSZ);                                     \
        }                                                               \
    }                                                                   \
  while (0)

#define QUICKSORTS__UNSTABLE_QUICKSORT__MAKE_AN_ORDERED_PREFIX(PFX, ARR, N, ELEMSZ, \
                                                               QUQ__LT__, \
                                                               QUQ__PREFIX_LENGTH__) \
  do                                                                    \
    {                                                                   \
      size_t PFX##pfx_len = 2;                                          \
      if (!QUQ__LT__ ((ARR) + (ELEMSZ), (ARR)))                         \
        {                                                               \
          /* Non-decreasing order. */                                   \
          while (PFX##pfx_len < N &&                                    \
                 !QUQ__LT__ ((ARR) + ((ELEMSZ) * PFX##pfx_len),         \
                             (ARR) + (((ELEMSZ) - 1) * PFX##pfx_len)))  \
            PFX##pfx_len += 1;                                          \
        }                                                               \
      else                                                              \
        {                                                               \
          /* Decreasing order. This branch sorts unstably. */           \
          while (PFX##pfx_len < N &&                                    \
                 !QUQ__LT__ ((ARR) + (((ELEMSZ) - 1) * PFX##pfx_len),   \
                             (ARR) + ((ELEMSZ) * PFX##pfx_len)))        \
            PFX##pfx_len += 1;                                          \
          QUICKSORTS__UNSTABLE_QUICKSORT__REVERSE_ARRAY                 \
            (PFX, (ARR), PFX##pfx_len, (ELEMSZ));                       \
        }                                                               \
      QUQ__PREFIX_LENGTH__ = PFX##pfx_len;                              \
    }                                                                   \
  while (0)



/* fn {a  : vt@ype} */
/* insertion_position */
/*           {n      : int} */
/*           {i      : pos | i < n} */
/*           {p_arr  : addr} */
/*           (pf_arr : !array_v (a, p_arr, n) >> _ | */
/*            p_arr  : ptr p_arr, */
/*            i      : size_t i) */
/*     :<> [j : nat | j <= i] */
/*         size_t j = */
/*   (* */
/*     A binary search. */

/*     References: */

/*       * H. Bottenbruch, "Structure and use of ALGOL 60", Journal of */
/*         the ACM, Volume 9, Issue 2, April 1962, pp.161-221. */
/*         https://doi.org/10.1145/321119.321120 */

/*         The general algorithm is described on pages 214 and 215. */

/*       * https://en.wikipedia.org/w/index.php?title=Binary_search_algorithm&oldid=1062988272#Alternative_procedure */
/*   *) */
/*   let */
/*     fun */
/*     loop {j, k : int | 0 <= j; j <= k; k < i} */
/*          .<k - j>. */
/*          (arr : &array (a, n), */
/*           j   : size_t j, */
/*           k   : size_t k) */
/*         :<> [j1 : nat | j1 <= i] */
/*             size_t j1 = */
/*       if j <> k then */
/*         let */
/*           val h = k - half (k - j) (* (j + k) ceildiv 2 *) */
/*          in */
/*           if array_element_lt<a> {n} (arr, i, h) then */
/*             loop (arr, j, pred h) */
/*           else */
/*             loop (arr, h, k) */
/*         end */
/*       else if j <> i2sz 0 then */
/*         succ j */
/*       else if array_element_lt<a> {n} (arr, i, i2sz 0) then */
/*         i2sz 0 */
/*       else */
/*         i2sz 1 */
/*   in */
/*     loop (!p_arr, i2sz 0, pred i) */
/*   end */


#endif /* QUICKSORTS__UNSTABLE_QUICKSORT_H__HEADER_GUARD__ */
