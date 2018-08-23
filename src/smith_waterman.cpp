#include <stdio.h>
#include <string.h>

#define D -1
#define I -1
#define M 1
#define R -1

/* Array A of |f| + 1 columns and |t| + 1 rows */
int smith_waterman(int A[f + 1][t + 1]) {

  int lf = strlen(f);
  int lt = strlen(t);
  A[0][0] = 0;

  for (j = 1; j <= lt; j ++) {
    A[j][0] = 0;
  }

  for (k = 1; k <= lf; k ++) {
    A[0][k] = 0;
  }

  for (j = 1; j <= lt; j ++) {
    for (k = 1; k <= lf; k ++) {
      A[j][k] = max4(
        0,
        A[j][k - 1] + d,                            // deletion
        A[j - 1][k] + i,                            // insertion
        A[j - 1][k - 1] + equal(f[k - 1], t[j - 1]) // replace or match
      );
    }
  }

  return A[lt][lf];

}

/* Returns the maximum of 4 ints */
int max4(int a, int b, int c, int d) {
  return max2(max2(a, b), max2(c, d));
}

/* Returns the maximum of 2 ints */
int max2(int a, int b) {
  return (a > b) ? a : b;
}

/* Returns m if characters match, r otherwise */
int equal(int a, int b) {
  return (a == b) ? M : 0;
}
