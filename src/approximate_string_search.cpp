/*
 * File    : approximate_string_search.cpp
 * Title   : Approximate String Search
 * Author  : 736901
 * Created : 19/08/2018
 * Purpose : COMP90049 2018S2 Assignment 1
 */

#include <algorithm>
#include <cstring>
#include <ctime>
#include <fstream>
#include <iostream>
#include <sstream>
#include <vector>

#include <string.h>

using namespace std;

#define INSERT -1
#define DELETE -1
#define REPLACE -1
#define MATCH 1

// binary search
int binary_search(int A[], int l, int r, int x) {
  if (r > 0) {
    int m = l + (r - l) / 2;
    if (A[m] == x) {
      return m;
    }
    if (A[m] > x) {
      return binary_search(A, l, m - 1, x);
    }
    return binary_search(A, m + 1, r, x);
  }
  return -1;
}

/* Returns the maximum of 2 ints */
int max2(int a, int b) {
  return (a > b) ? a : b;
}

/* Returns the maximum of 3 ints */
int max3(int a, int b, int c) {
  return max2(max2(a, b), c);
}

/* Returns the maximum of 4 ints */
int max4(int a, int b, int c, int d) {
  return max2(max2(a, b), max2(c, d));
}

/* Returns m if characters match, r otherwise */
int equal(char a, char b) {
  return (a == b) ? MATCH : REPLACE;
}

/* Returns m if characters match, r otherwise */
int equal2(char a, char b, int m, int r) {
  return (a == b) ? m : r;
}

/* Array A of |f| + 1 columns and |t| + 1 rows */
int needleman_wunsch(char *f, char *t) {

  int j, k;
  int lf = strlen(f);
  int lt = strlen(t);
  int A[lf + 1][lt + 1];
  A[0][0] = 0;

  for (j = 1; j <= lt; j ++) {
    A[j][0] = j * INSERT;
  }

  for (k = 1; k <= lf; k ++) {
    A[0][k] = k * DELETE;
  }

  for (j = 1; j <= lt; j ++) {
    for (k = 1; k <= lf; k ++) {
      A[j][k] = max3(
        A[j][k - 1] + DELETE,                       // deletion
        A[j - 1][k] + INSERT,                       // insertion
        A[j - 1][k - 1] + equal(f[k - 1], t[j - 1]) // replace or match
      );
    }
  }

  //debug
  printf("\n");
  for (j = 0; j <= lt; j ++) {
    for (k = 0; k <= lf; k ++) {
      printf("%2d ", A[j][k]);
    }
    printf("\n");
  }

  return A[lt][lf];

}

/* Array A of |f| + 1 columns and |t| + 1 rows */
int smith_waterman(char *f, char *t) {

  int j, k, greatest = 0;
  int lf = strlen(f);
  int lt = strlen(t);
  int A[lf + 1][lt + 1];
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
        A[j][k - 1] + DELETE,                       // deletion
        A[j - 1][k] + INSERT,                       // insertion
        A[j - 1][k - 1] + equal(f[k - 1], t[j - 1]) // replace or match
      );
      if (A[j][k] > greatest) {
        greatest = A[j][k];
      }
    }
  }

  //debug
  printf("\n");
  for (j = 0; j <= lt; j ++) {
    for (k = 0; k <= lf; k ++) {
      printf("%2d ", A[j][k]);
    }
    printf("\n");
  }

  return greatest;

}

string concat(vector<char> seglist, int start, int end) {
  string s;
  int i;
  for (i = start; i < end; i ++) {
    s += seglist.at(i);
  }
  return s;
}

vector<string> n_grams(string str, int n) {
  vector<string> ngrams;
  vector<char> seglist(str.begin(), str.end());
  /*
  // convert seglist from vector<char> to vector<string>

  stringstream words(str);
  string segment;
  vector<string> seglist;

  while (getline(words, segment, ' ')) {
    seglist.push_back(segment);
  }
  */

  string hash = "#";
  ngrams.push_back(hash + str.front());
  for (int i = 0; i < seglist.size() - n + 1; i ++) {
    ngrams.push_back(concat(seglist, i, i + n));
  }
  ngrams.push_back(str.back() + hash);

  return ngrams;
}

int n_gram(char s[], char t[], int n) {
  string str_s(s);
  string str_t(t);
  vector<string> ns = n_grams(str_s, n);
  vector<string> nt = n_grams(str_t, n);
  vector<string> ni;

  // debug
  cout << "A    : ";
  for (int i = 0; i < ns.size(); i ++) {
    cout << ns[i] << " ";
  }
  cout << endl << "B    : ";
  for (int i = 0; i < nt.size(); i ++) {
    cout << nt[i] << " ";
  }

  sort(ns.begin(), ns.end());
  sort(nt.begin(), nt.end());
  set_intersection(ns.begin(), ns.end(), nt.begin(), nt.end(), back_inserter(ni));

  // debug
  cout << endl << "A ∩ B: ";
  for (int i = 0; i < ni.size(); i ++) {
    cout << ni[i] << " ";
  }
  cout << endl;

  int score = ns.size() + nt.size() - (2 * ni.size());
  return score;
}

/* Array A of |f| + 1 columns and |t| + 1 rows */
int levenshtein_distance(char *t, char *f) {
  int m = 0;
  int i = 1;
  int d = 1;
  int r = 1;

  int j, k;
  int lf = strlen(f);
  int lt = strlen(t);
  int A[lf + 1][lt + 1];
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
        A[j][k - 1] + d,                       // deletion
        A[j - 1][k] + i,                       // insertion
        A[j - 1][k - 1] + equal2(f[k - 1], t[j - 1], m, r) // replace or match
      );
    }
  }

  return A[lt][lf];
}

void neighbourhood_search(char *s, std::fstream& dict, int n) {
  string entry;
  vector<string> neighbours;
  while (getline(dict, entry)) {
    char *t = new char[entry.length() + 1];
    strcpy(t, entry.c_str());
    if (levenshtein_distance(s, t) == n) {
      neighbours.push_back(entry);
      cout << entry << endl;
    }
  }

  cout << neighbours.size() << " neighbours found" << endl;
/*
  // binary search stuff
  int A = {2, 3, 4, 10, 40};
  int n = sizeof(A) / sizeof(A[0]);
  int x = 10;
  int result = binary_search(A, 0, n - 1, x);
*/
}

/* Main function */
int main(int argc, char **argv) {

  // Start time
  clock_t tick, tock;
  tick = clock();

  // Check for correct number of arguments
  if (argc != 4) {
    cerr << "usage: " << argv[0] << " <dict> <correct> <misspell>" << endl;
    cerr << "example: " << argv[0] << " ../data/dict.txt ../data/wiki_correct.txt ../data/wiki_misspell.txt" << endl;
    cerr << "example: " << argv[0] << " ../data/dict.txt ../data/birkbeck_correct.txt ../data/birkbeck_misspell.txt" << endl;
    return 1;
  }

  // Open files
  fstream dict;
  dict.open(argv[1], ios::in);
  if (not dict || not dict.is_open()) {
    cerr << "file: " << argv[1] << " failed to open" << endl;
    return 1;
  }

  fstream correct;
  correct.open(argv[2], ios::in);
  if (not correct || not correct.is_open()) {
    cerr << "file: " << argv[2] << " failed to open" << endl;
    return 1;
  }

  fstream misspell;
  misspell.open(argv[3], ios::in);
  if (not misspell || not misspell.is_open()) {
    cerr << "file: " << argv[3] << " failed to open" << endl;
    return 1;
  }
/*
  string word;
  string entry;

  int lines = 0;
  if (misspell.is_open() && dict.is_open()) {
    //while (getline(misspell, word)) {
      while (getline(dict, entry)) {
        //char *wordstr = new char[word.length() + 1];
        //strcpy(wordstr, word.c_str());
        word = "doge";
        char wordstr[] = "doge";
        char *entrystr = new char[entry.length() + 1];
        strcpy(entrystr, entry.c_str());
        //needleman_wunsch(wordstr, entrystr);
        //smith_waterman(wordstr, entrystr);
        n_gram(wordstr, entrystr, 2);
        //cout << word << ", " << entry << ", " << n_gram(wordstr, entrystr, 2) << endl;
        lines ++;
        if (lines % 10000 == 0) {
          cout << lines << endl;
        }
      }
    //}
  }

  // Close files
  dict.close();
  correct.close();
  misspell.close();

*/

  char s[] = "lended";
  char t[] = "deaden";
  int ged = needleman_wunsch(s, t);
  cout << "global edit distance: " << ged << "\n" << endl;
  int led = smith_waterman(s, t);
  cout << "local edit distance: " << led << "\n" << endl;
  int n = 2;
  int ngram = n_gram(s, t, n);
  cout << n << "-gram: " << ngram << "\n" << endl;
  //neighbourhood_search(s, dict, 3);


  // End time
  tock = clock();
  float diff = ((float) tock - (float) tick) / CLOCKS_PER_SEC;
  cout << "Time: " << diff << "s" << endl;

}
