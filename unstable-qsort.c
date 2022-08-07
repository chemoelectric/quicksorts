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

#undef QUICKSORTS__UNSTABLE_QUICKSORT__LT
#define QUICKSORTS__UNSTABLE_QUICKSORT__LT(x, y) \
  (quicksorts__unstable_quicksort__compar ((x), (y)) < 0)

void
unstable_qsort (void *base, size_t nmemb, size_t size,
                int (*quicksorts__unstable_quicksort__compar)
                (const void *, const void *))
{
  /* FIXME : Put elembuf in the heap if ‘size’ is very large. */
  char quicksorts__unstable_qsort__elembuf[size];

  char *quicksorts__unstable_qsort__arr = base;
  size_t quicksorts__unstable_qsort__nmemb = nmemb;
  size_t quicksorts__unstable_qsort__elemsz = size;

  QUICKSORTS__UNSTABLE_QUICKSORT__QUICKSORT
    (quicksorts__unstable_qsort__);
}
