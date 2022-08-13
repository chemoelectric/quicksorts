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

#define MAX_SZ 100000ULL

#if __GNUC__
#define STRCMP __builtin_strcmp
#else
#define STRCMP strcmp
#endif

typedef const char *sortkind_t;
#define sortkind_eq(A, B) (STRCMP ((A), (B)) == 0)

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

typedef struct {
  char s[BIG_SIZE];
} big_t;

static inline bool
big_t_lt (const big_t *x, const big_t *y)
{
  return (STRCMP (x->s, y->s) < 0);
}

static int
big_t_cmp (const void *x, const void *y)
{
  return (STRCMP (((const big_t *) x)->s,
                  ((const big_t *) y)->s));
}

static void
initialize_array (big_t *p, size_t n)
{
  for (size_t i = 0; i != n; i += 1)
    {
      for (size_t j = 0; j < BIG_SIZE; j += 1)
        p[i].s[j] = (char) random_int ('0', '9');
      p[i].s[BIG_SIZE - 1] = '\0';
    }
}

static void
copy_array (big_t *dst, big_t *src, size_t n)
{
  memcpy (dst, src, n * sizeof (big_t));
}

static void
unstable_random_insertion_typed_big (big_t *base, size_t nmemb)
{
  UNSTABLE_QUICKSORT_TYPED_7ARGS
    (big_t, base, nmemb, big_t_lt,
     QUICKSORTS_COMMON__PIVOT_RANDOM,
     80, QUICKSORTS__UNSTABLE_QUICKSORT__INSERTION_SORT__TYPED);
}

static void
unstable_median3_insertion_typed_big (big_t *base, size_t nmemb)
{
  UNSTABLE_QUICKSORT_TYPED_7ARGS
    (big_t, base, nmemb, big_t_lt,
     QUICKSORTS_COMMON__PIVOT_MEDIAN_OF_THREE,
     80, QUICKSORTS__UNSTABLE_QUICKSORT__INSERTION_SORT__TYPED);
}

static void
unstable_random_shell_typed_big (big_t *base, size_t nmemb)
{
  UNSTABLE_QUICKSORT_TYPED_7ARGS
    (big_t, base, nmemb, big_t_lt,
     QUICKSORTS_COMMON__PIVOT_RANDOM,
     350, QUICKSORTS__UNSTABLE_QUICKSORT__SHELL_SORT__TYPED);
}

static void
unstable_median3_shell_typed_big (big_t *base, size_t nmemb)
{
  UNSTABLE_QUICKSORT_TYPED_7ARGS
    (big_t, base, nmemb, big_t_lt,
     QUICKSORTS_COMMON__PIVOT_MEDIAN_OF_THREE,
     350, QUICKSORTS__UNSTABLE_QUICKSORT__SHELL_SORT__TYPED);
}

static void
test_arrays (sortkind_t sortkind)
{
  big_t *p1 = malloc (MAX_SZ * sizeof (big_t));
  big_t *p2 = malloc (MAX_SZ * sizeof (big_t));
  big_t *p3 = malloc (MAX_SZ * sizeof (big_t));

  for (size_t sz = 0; sz <= MAX_SZ; sz = MAX (1, 10 * sz))
    {
      initialize_array (p1, sz);
#if 0
      printf ("unsorted ---------------------------\n");
      for (size_t i = 0; i != sz; i += 1)
        printf ("%s\n", p1[i].s);
#endif

      copy_array (p2, p1, sz);
      const long double t21 = get_clock ();
      qsort (p2, sz, sizeof (big_t), big_t_cmp);
      const long double t22 = get_clock ();
      const long double t2 = t22 - t21;
#if 0
      printf ("system qsort -----------------------\n");
      for (size_t i = 0; i != sz; i += 1)
        printf ("%s\n", p2[i].s);
#endif

      copy_array (p3, p1, sz);
      long double t31;
      long double t32;
      if (sortkind_eq (sortkind,
                       "unstable-random-insertion-typed-big"))
        {
          t31 = get_clock ();
          unstable_random_insertion_typed_big (p3, sz);
          t32 = get_clock ();
        }
      else if (sortkind_eq (sortkind,
                            "unstable-median3-insertion-typed-big"))
        {
          t31 = get_clock ();
          unstable_median3_insertion_typed_big (p3, sz);
          t32 = get_clock ();
        }
      else if (sortkind_eq (sortkind,
                            "unstable-random-shell-typed-big"))
        {
          t31 = get_clock ();
          unstable_random_shell_typed_big (p3, sz);
          t32 = get_clock ();
        }
      else if (sortkind_eq (sortkind,
                            "unstable-median3-shell-typed-big"))
        {
          t31 = get_clock ();
          unstable_median3_shell_typed_big (p3, sz);
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
        printf ("%s\n", p3[i].s);
#endif

      for (size_t i = 0; i != sz; i += 1)
        {
#if 0
          printf ("i = %zu  p2[i] = %s,  p3[i] = %s\n",
                  i, p2[i].s, p3[i].s);
#endif
          for (size_t j = 0; j != BIG_SIZE; j += 1)
            CHECK (p2[i].s[j] == p3[i].s[j]);
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
