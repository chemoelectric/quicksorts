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
#include <stdbool.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>
#include <time.h>
#include "quicksorts/unstable-quicksort.h"

#define MAX_SZ 1000000ULL

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

#define BIG_SIZE 2048

static inline bool
string_lt (const void *x, const void *y)
{
  return (strcmp (x, y) < 0);
}

static void
initialize_array (char *p, size_t n)
{
  memset (p, '\0', BIG_SIZE * n);
  for (size_t i = 0; i != n; i += 1)
    sprintf (p + (i * BIG_SIZE), "%zu", i);
}

static void
copy_array (char *dst, char *src, size_t n)
{
  memcpy (dst, src, n * BIG_SIZE);
}

static void
unstable_random_insertion_big (void *base, size_t nmemb)
{
  UNSTABLE_QUICKSORT_7ARGS
    (base, nmemb, BIG_SIZE, string_lt,
     QUICKSORTS_COMMON__PIVOT_RANDOM,
     80, QUICKSORTS__UNSTABLE_QUICKSORT__INSERTION_SORT);
}

static void
unstable_random_shell_big (void *base, size_t nmemb)
{
  UNSTABLE_QUICKSORT_7ARGS
    (base, nmemb, BIG_SIZE, string_lt,
     QUICKSORTS_COMMON__PIVOT_RANDOM,
     350, QUICKSORTS__UNSTABLE_QUICKSORT__SHELL_SORT);
}

static void
test_arrays (sortkind_t sortkind)
{
  char *p1 = malloc (MAX_SZ * BIG_SIZE);
  char *p2 = malloc (MAX_SZ * BIG_SIZE);
  char *p3 = malloc (MAX_SZ * BIG_SIZE);

  for (size_t sz = 0; sz <= MAX_SZ; sz = MAX (1, 10 * sz))
    {
      initialize_array (p1, sz);
#if 0
      printf ("unsorted ---------------------------\n");
      for (size_t i = 0; i != sz; i += 1)
        printf ("%s\n", p1[i]);
#endif

      copy_array (p2, p1, sz);
      const long double t21 = get_clock ();
      qsort (p2, sz, BIG_SIZE,
             (int (*) (const void *, const void *)) strcmp);
      const long double t22 = get_clock ();
      const long double t2 = t22 - t21;
#if 0
      printf ("system qsort -----------------------\n");
      for (size_t i = 0; i != sz; i += 1)
        printf ("%s\n", p2[i]);
#endif

      copy_array (p3, p1, sz);
      long double t31;
      long double t32;
      if (sortkind_eq (sortkind, "unstable-random-insertion-big"))
        {
          t31 = get_clock ();
          unstable_random_insertion_big (p3, sz);
          t32 = get_clock ();
        }
      else if (sortkind_eq (sortkind, "unstable-random-shell-big"))
        {
          t31 = get_clock ();
          unstable_random_shell_big (p3, sz);
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
        printf ("%s\n", &p3[i * BIG_SIZE]);
#endif

      for (size_t i = 0; i != sz; i += 1)
        {
#if 0
          printf ("i = %zu  p2[i] = %s,  p3[i] = %s\n",
                  i, &p2[i * BIG_SIZE], &p3[i * BIG_SIZE]);
#endif
          for (size_t j = 0; j != BIG_SIZE; j += 1)
            CHECK (p2[(i * BIG_SIZE) + j] == p3[(i * BIG_SIZE) + j]);
        }

      printf ("  qsort:%Lf  ours:%Lf  %zu\n", t2, t3, sz);
    }

  free (p1);
  free (p2);
  free (p3);
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
  test_arrays (sortkind);
  return 0;
}
