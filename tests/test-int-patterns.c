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
#include <string.h>
#include <time.h>
#include "quicksorts/unstable-qsort.h"
#include "quicksorts/unstable-quicksort.h"

#define MAX_SZ 10000000ULL

typedef const char *sortkind_t;
#define sortkind_eq(A, B) (strcmp ((A), (B)) == 0)

#define MAX(x, y) ((x) < (y) ? (y) : (x))

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

static int
intcmp (const void *px, const void *py)
{
  const int x = *((const int *) px);
  const int y = *((const int *) py);
  return ((x < y) ? -1 : ((x > y) ? 1 : 0));
}

static int
intcmp_r (const void *px, const void *py, void *env)
{
  CHECK (*(int *) env == 1234);
  const int x = *((const int *) px);
  const int y = *((const int *) py);
  return ((x < y) ? -1 : ((x > y) ? 1 : 0));
}

static inline int
int_lt (const void *px, const void *py)
{
  return (*(const int *) px < *(const int *) py);
}

static void
unstable_insertion_random (void *base, size_t nmemb)
{
  UNSTABLE_QUICKSORT_CONFIGURABLE
    (base, nmemb, sizeof (int), int_lt,
     80, QUICKSORTS__UNSTABLE_QUICKSORT__INSERTION_SORT,
     QUICKSORTS_COMMON__PIVOT_RANDOM);
}

static void
test_arrays_with_int_keys (sortkind_t sortkind,
                           void (*init) (size_t i, int *x))
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
      printf ("system qsort -----------------------\n");
      for (size_t i = 0; i != sz; i += 1)
        printf ("%d\n", p2[i]);
#endif

      copy_array (p3, p1, sz);
      long double t31;
      long double t32;
      if (sortkind_eq (sortkind, "unstable_qsort"))
        {
          t31 = get_clock ();
          unstable_qsort (p3, sz, sizeof (int), intcmp);
          t32 = get_clock ();
        }
      else if (sortkind_eq (sortkind, "unstable_qsort_r"))
        {
          int env_val = 1234;
          void *env = &env_val;
          t31 = get_clock ();
          unstable_qsort_r (p3, sz, sizeof (int), intcmp_r, env);
          t32 = get_clock ();
        }
      else if (sortkind_eq (sortkind, "unstable-insertion-random"))
        {
          t31 = get_clock ();
          unstable_insertion_random (p3, sz);
          t32 = get_clock ();
        }
      else
        {
          printf ("Invalid command-line argument.\n");
          exit (1);
        }
      const long double t3 = t32 - t31;
#if 0
      printf ("my sort ----------------------------\n");
      for (size_t i = 0; i != sz; i += 1)
        printf ("%d\n", p3[i]);
#endif

      for (size_t i = 0; i != sz; i += 1)
        {
#if 0
          printf ("i = %zu  p2[i] = %d,  p3[i] = %d\n",
                  i, p2[i], p3[i]);
#endif
          CHECK (p2[i] == p3[i]);
        }

      printf ("  qsort:%Lf  ours:%Lf  %zu\n", t2, t3, sz);
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
test_random_arrays_with_int_keys (sortkind_t sortkind)
{
  printf ("Random arrays:\n");
  test_arrays_with_int_keys (sortkind, init_with_random_int);
}

static void
init_with_index (size_t i, int *x)
{
  *x = (int) i;
}

static void
test_presorted_arrays_with_int_keys (sortkind_t sortkind)
{
  printf ("Pre-sorted arrays:\n");
  test_arrays_with_int_keys (sortkind, init_with_index);
}

static void
init_with_neg_of_index (size_t i, int *x)
{
  *x = -((int) i);
}

static void
test_reverse_presorted_arrays_with_int_keys (sortkind_t sortkind)
{
  printf ("Reverse pre-sorted arrays:\n");
  test_arrays_with_int_keys (sortkind, init_with_neg_of_index);
}

static void
init_with_sign_reversing_random_ints (size_t i, int *x)
{
  int r = random_int (-1000, 1000);
  *x = ((i & 1) == 0) ? r : (-r);
}

static void
test_sign_reversal_random_arrays_with_int_keys (sortkind_t sortkind)
{
  printf ("Sign-reversal random arrays:\n");
  test_arrays_with_int_keys
    (sortkind, init_with_sign_reversing_random_ints);
}

static void
init_with_a_constant_int (size_t i, int *x)
{
  *x = 1;
}

static void
test_constant_arrays_with_int_keys (sortkind_t sortkind)
{
  printf ("Constant arrays:\n");
  test_arrays_with_int_keys (sortkind, init_with_a_constant_int);
}

static void
init_with_a_sign_reversing_constant_int (size_t i, int *x)
{
  *x = ((i & 1) == 0) ? 1 : (-1);
}

static void
test_sign_reversal_constant_arrays_with_int_keys (sortkind_t sortkind)
{
  printf ("Sign-reversal constant arrays:\n");
  test_arrays_with_int_keys
    (sortkind, init_with_a_sign_reversing_constant_int);
}

int
main (int argc, char *argv[])
{
  if (argc <= 1)
    {
      printf ("This program expects a command-line argument.\n");
      exit (1);
    }
  sortkind_t sortkind = argv[1];
  test_random_arrays_with_int_keys (sortkind);
  test_presorted_arrays_with_int_keys (sortkind);
  test_reverse_presorted_arrays_with_int_keys (sortkind);
  test_sign_reversal_random_arrays_with_int_keys (sortkind);
  test_constant_arrays_with_int_keys (sortkind);
  test_sign_reversal_constant_arrays_with_int_keys (sortkind);
  return 0;
}
