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

#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <time.h>
#include "quicksorts/unstable-qsort.h"

#define MAX(x, y) ((x) < (y) ? (y) : (x))

// FIXME #define MAX_SZ 10000000ULL
#define MAX_SZ 10000ULL

#define CHECK(expr)                             \
  if (expr)                                     \
    {}                                          \
  else                                          \
    check_failed (__FILE__, __LINE__)

static void
check_failed (const char *file, unsigned int line)
{
  fprintf (stderr, "CHECK failed at %s:%u\n", file, line);
  exit (1);
}

static int
random_int (int m, int n)
{
  return m + (int) (drand48 () * (n - m + 1));
}

long double
get_clock (void)
{
  return ((long double) clock ()) / CLOCKS_PER_SEC;
}

static int
intcmp (const void *px, const void *py)
{
  const int x = *((const int *) px);
  const int y = *((const int *) py);
  return ((x < y) ? -1 : ((x > y) ? 1 : 0));
}

static void
initialize_array (int *p, size_t n,
                  void (*init) (size_t i, int *x))
{
  for (size_t i = 0; i != n; i += 1)
    init (i, &p[i]);
}

static void
copy_array (int *dst, int *src, size_t n)
{
  for (size_t i = 0; i != n; i += 1)
    dst[i] = src[i];
}

static void
test_arrays_with_int_keys (void (*init) (size_t i, int *x))
{
  int *p1 = malloc (MAX_SZ * sizeof (int));
  int *p2 = malloc (MAX_SZ * sizeof (int));
  int *p3 = malloc (MAX_SZ * sizeof (int));

  for (size_t sz = 0; sz <= MAX_SZ; sz = MAX (1, 10 * sz))
    {
      initialize_array (p1, sz, init);
#if 0
      printf ("unsorted ---------------------------\n");
      for (size_t i = 0; i != sz; i += 1)
        printf ("%d\n", p1[i]);
#endif

      copy_array (p2, p1, sz);
      const long double t21 = get_clock ();
      qsort (p2, sz, sizeof (int), intcmp);
      const long double t22 = get_clock ();
      const long double t2 = t22 - t21;
#if 0
      printf ("qsort ------------------------------\n");
      for (size_t i = 0; i != sz; i += 1)
        printf ("%d\n", p2[i]);
#endif

      copy_array (p3, p1, sz);
      const long double t31 = get_clock ();
      unstable_qsort (p3, sz, sizeof (int), intcmp);
      const long double t32 = get_clock ();
      const long double t3 = t32 - t31;
#if 0
      printf ("unstable_qsort ---------------------\n");
      for (size_t i = 0; i != sz; i += 1)
        printf ("%d\n", p3[i]);
#endif

      for (size_t i = 0; i != sz; i += 1)
        CHECK (p2[i] == p3[i]);

      printf ("  qsort:%Lf  unstable_qsort:%Lf  %zu\n", t2, t3, sz);
    }

  free (p1);
  free (p2);
  free (p3);
}

static void
init_with_random_int (size_t i, int *x)
{
  *x = random_int (-1000, 1000);
}

static void
test_random_arrays_with_int_keys (void)
{
  printf ("Random arrays:\n");
  test_arrays_with_int_keys (init_with_random_int);
}

static void
init_with_index (size_t i, int *x)
{
  *x = (int) i;
}

static void
test_presorted_arrays_with_int_keys (void)
{
  printf ("Pre-sorted arrays:\n");
  test_arrays_with_int_keys (init_with_index);
}

static void
init_with_neg_of_index (size_t i, int *x)
{
  *x = -((int) i);
}

static void
test_reverse_presorted_arrays_with_int_keys (void)
{
  printf ("Reverse pre-sorted arrays:\n");
  test_arrays_with_int_keys (init_with_neg_of_index);
}

int
main (int argc, char *argv[])
{
  test_random_arrays_with_int_keys ();
  test_presorted_arrays_with_int_keys ();
  test_reverse_presorted_arrays_with_int_keys ();
  return 0;
}
