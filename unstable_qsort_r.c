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

#include "quicksorts/unstable-qsort.h"
#include "quicksorts/unstable-quicksort.h"

#undef LT_PREDICATE
#define LT_PREDICATE(x, y)                              \
  (quicksorts__unstable_quicksort__compar               \
   ((x), (y), quicksorts__unstable_quicksort__env) < 0)

#ifndef PIVOT_SELECTION
#define PIVOT_SELECTION QUICKSORTS_COMMON__PIVOT_MEDIAN_OF_THREE
#endif

#ifndef SMALL_SIZE
#define SMALL_SIZE 80
#endif

#ifndef SMALL_SORT
#define SMALL_SORT QUICKSORTS__UNSTABLE_QUICKSORT__INSERTION_SORT
#endif

void
unstable_qsort_r (void *base, size_t nmemb, size_t size,
                  int (*quicksorts__unstable_quicksort__compar)
                  (const void *, const void *, void *),
                  void *quicksorts__unstable_quicksort__env)
{
  UNSTABLE_QUICKSORT_CONFIGURABLE (base, nmemb, size,
                                   LT_PREDICATE,
                                   PIVOT_SELECTION,
                                   SMALL_SIZE, SMALL_SORT);
}
