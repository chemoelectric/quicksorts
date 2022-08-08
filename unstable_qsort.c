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

#define LT(x, y)                                            \
  (quicksorts__unstable_quicksort__compar ((x), (y)) < 0)

#define SMALL_SIZE 80
#define SMALL_SORT QUICKSORTS__UNSTABLE_QUICKSORT__INSERTION_SORT
#define PIVOT_SELECTION QUICKSORTS_COMMON__PIVOT_MEDIAN_OF_THREE

void
unstable_qsort (void *base, size_t nmemb, size_t size,
                int (*quicksorts__unstable_quicksort__compar)
                (const void *, const void *))
{
  UNSTABLE_QUICKSORT_CONFIGURABLE (base, nmemb, size, LT,
                                   SMALL_SIZE, SMALL_SORT,
                                   PIVOT_SELECTION);
}
