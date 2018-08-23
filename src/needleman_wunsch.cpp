#include <stdio.h>
#include <string.h>

#define D -1
#define I -1
#define M 1
#define R -1

unsigned int edit_distance(const std::string& s1, const std::string& s2) {

  const size_t len1 = s1.size();
  const size_t len2 = s2.size();
  vector<vector<unsigned int>> d(len1 + 1, vector<unsigned int>(len2 + 1));

  d[0][0] = 0;

  for (unsigned int i = 1; i <= len1; i ++) {
    d[i][0] = i;
  }

  for (unsigned int i = 1; i <= len2; i ++) {
    d[0][i] = i;
  }

  for (unsigned int i = 1; i <= len1; i++) {
    for (unsigned int j = 1; j <= len2; j ++) {
      d[i][j] = min({
        d[i - 1][j] + 1,
        d[i][j - 1] + 1,
        d[i - 1][j - 1] + (s1[i-1] == s2[j-1] ? 0 : 1)});
    }
  }

  return d[len1][len2];
}

/* Array A of |f| + 1 columns and |t| + 1 rows */
int needleman_wunsch(int A[f + 1][t + 1]) {

  int lf = strlen(f);
  int lt = strlen(t);
  A[0][0] = 0;

  for (j = 1; j <= lt; j ++) {
    A[j][0] = j * i;
  }

  for (k = 1; k <= lf; k ++) {
    A[0][k] = k * d;
  }

  for (j = 1; j <= lt; j ++) {
    for (k = 1; k <= lf; k ++) {
      A[j][k] = max3(
        A[j][k - 1] + d,                            // deletion
        A[j - 1][k] + i,                            // insertion
        A[j - 1][k - 1] + equal(f[k - 1], t[j - 1]) // replace or match
      );
    }
  }

  return A[lt][lf];

}

/* Returns the maximum of 3 ints */
int max3(int a, int b, int c) {
  return max2(max2(a, b), c);
}

/* Returns the maximum of 2 ints */
int max2(int a, int b) {
  return (a > b) ? a : b;
}

/* Returns m if characters match, r otherwise */
int equal(int a, int b) {
  return (a == b) ? M : 0;
}
