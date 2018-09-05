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

#include <assert.h>
#include <string.h>

#define d(i,j) dd[(i) * (m+2) + (j)]

using namespace std;

/* Binary search */
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

/* Returns the minimum of 2 ints */
int min2(int a, int b) {
  return (a < b) ? a : b;
}

/* Returns the maximum of 3 ints */
int max3(int a, int b, int c) {
  return max2(max2(a, b), c);
}

/* Returns the minimum of 3 ints */
int min3(int a, int b, int c) {
  return min2(min2(a, b), c);
}

/* Returns the maximum of 4 ints */
int max4(int a, int b, int c, int d) {
  return max2(max2(a, b), max2(c, d));
}

/* Returns the minimum of 4 ints */
int min4(int a, int b, int c, int d) {
  return min2(min2(a, b), min2(c, d));
}

/* Returns m if characters match, r otherwise */
int equal(char a, char b, int m, int r) {
  return (a == b) ? m : r;
}

/* Damerau Levenshtein Distance */
int damerau_levenshtein_distance(const char* s, const char* t, int n, int m) {

  int cost = 0;
  const int INFINITY = n + m;
  assert(s != NULL);
  assert(t != NULL);
  assert(n > 0);
  assert(m > 0);
  int* DA = (int*) calloc(256, sizeof(int));
  int* dd = (int*) calloc((n + 2) * (m + 2), sizeof(int));
  d(0,0) = INFINITY;

  for (int i = 0; i < n + 1; i++) {
    d(i + 1, 1) = i;
    d(i + 1, 0) = INFINITY;
  }

  for (int j = 0; j < m + 1; j++) {
    d(1, j + 1) = j;
    d(0, j + 1) = INFINITY;
  }

  for (int i = 1; i < n + 1; i++) {
    int DB = 0;
    for(int j = 1; j < m + 1; j++) {
      int i1 = DA[t[j-1]];
      int j1 = DB;
      cost = (s[i-1] == t[j-1]) ? 0 : 1;
      if(cost==0) {
        DB = j;
      }
      d(i+1, j+1) = min4(
        d(i,j)+cost,
        d(i + 1, j) + 1,
        d(i, j + 1) + 1,
        d(i1, j1) + (i - i1 - 1) + 1 + (j - j1 -1));
    }
    DA[s[i-1]] = i;
  }

  cost = d(n + 1, m + 1);
  free(dd);
  free(DA);
  return cost;
}

/* Array A of |f| + 1 columns and |t| + 1 rows */
int needleman_wunsch(char *f, char *t, int m, int i, int d, int r) {

  int j, k;
  int lf = strlen(f);
  int lt = strlen(t);
  int A[lt + 1][lf + 1];
  A[0][0] = 0;

  for (j = 1; j <= lt; j ++) {
    A[j][0] = j * i;
  }

  for (k = 1; k <= lf; k ++) {
    A[0][k] = k * d;
  }

  for (j = 1; j <= lt; j ++) {
    for (k = 1; k <= lf; k ++) {
      // choose min() if m < min(i,d,r): i.e. (m,i,d,r) = (0,1,1,1)
      // choose max() if m > max(i,d,r): i.e. (m,i,d,r) = (-1,1,1,1)
      if (m > max3(i, d, r)) {
        A[j][k] = max3(
          A[j][k - 1] + d,                                  // deletion
          A[j - 1][k] + i,                                  // insertion
          A[j - 1][k - 1] + equal(f[k - 1], t[j - 1], m, r) // replace or match
        );
      } else {
        A[j][k] = min3(
          A[j][k - 1] + d,                                  // deletion
          A[j - 1][k] + i,                                  // insertion
          A[j - 1][k - 1] + equal(f[k - 1], t[j - 1], m, r) // replace or match
        );
      }
    }
  }

  //debug
  // printf("\n");
  // for (j = 0; j <= lt; j ++) {
  //   for (k = 0; k <= lf; k ++) {
  //     printf("%2d ", A[j][k]);
  //   }
  //   printf("\n");
  // }

  return A[lt][lf];

}

/* Array A of |f| + 1 columns and |t| + 1 rows */
int smith_waterman(char *f, char *t, int m, int i, int d, int r) {

  if (m == 0) {
    m = -1;
  }

  int j, k, score = 0;
  int lf = strlen(f);
  int lt = strlen(t);
  int A[lt + 1][lf + 1];
  A[0][0] = 0;

  for (j = 1; j <= lt; j ++) {
    A[j][0] = 0;
  }

  for (k = 1; k <= lf; k ++) {
    A[0][k] = 0;
  }

  for (j = 1; j <= lt; j ++) {
    for (k = 1; k <= lf; k ++) {
      if (m > max3(i, d, r)) {
        A[j][k] = max4(
          0,
          A[j][k - 1] + d,                       // deletion
          A[j - 1][k] + i,                       // insertion
          A[j - 1][k - 1] + equal(f[k - 1], t[j - 1], m, r) // replace or match
        );
        if (A[j][k] > score) {
          score = A[j][k];
        }
      } else {
        A[j][k] = min4(
          0,
          A[j][k - 1] + d,                       // deletion
          A[j - 1][k] + i,                       // insertion
          A[j - 1][k - 1] + equal(f[k - 1], t[j - 1], m, r) // replace or match
        );
        if (A[j][k] < score) {
          score = A[j][k];
        }
      }

    }
  }

  //debug
  // printf("\n");
  // for (j = 0; j <= lt; j ++) {
  //   for (k = 0; k <= lf; k ++) {
  //     printf("%2d ", A[j][k]);
  //   }
  //   printf("\n");
  // }

  return score;

}

/* Auxiliary Function*/
string concat(vector<char> seglist, int start, int end) {
  string s;
  int i;
  for (i = start; i < end; i ++) {
    s += seglist.at(i);
  }
  return s;
}

/* N-Grams */
vector<string> n_grams(string str, int n) {
  vector<string> ngrams;
  vector<char> seglist(str.begin(), str.end());
  string hash = "#";

  if (str.size() > 0) {
    ngrams.push_back(hash + str[0]);
    for (int i = 0; i < seglist.size() - n + 1; i ++) {
      ngrams.push_back(concat(seglist, i, i + n));
    }
    ngrams.push_back(str[str.size() - 1] + hash);
  }

  return ngrams;
}

/* Array A of |f| + 1 columns and |t| + 1 rows */
int global_levenshtein_distance(char *f, char *t) {
  return needleman_wunsch(f, t, 0, 1, 1, 1);
}

/* Array A of |f| + 1 columns and |t| + 1 rows */
int local_levenshtein_distance(char *f, char *t) {
  return smith_waterman(f, t, 0, 1, 1, 1);
}

/* Neighbourhood Search */
vector<string> neighbourhood_search(string word, vector<string> dict, int n) {

  char *s = new char[word.length() + 1];
  strcpy(s, word.c_str());

  vector<string> neighbours;
  for (int i = 0; i < dict.size(); i ++) {
    //cout << entry << endl;
    char *t = new char[dict[i].length() + 1];
    strcpy(t, dict[i].c_str());
    if (global_levenshtein_distance(s, t) == n) {
      neighbours.push_back(dict[i]);
    }
    delete t;
  }

  delete s;
  return neighbours;

}

/* Calculates scores of Damerau-Levenshtein Distance */
void calculate_damerau_levenshtein_distance(vector<string> dict, vector<string> correct, vector<string> misspell) {

  int accuracy_denominator = misspell.size();
  int accuracy_numerator = 0;
  int recall_denominator = misspell.size();
  int recall_numerator = 0;
  int precision_denominator = 0;
  int precision_numerator = 0;

  for (int i = 0; i < misspell.size(); i ++) {
    vector<string> matches;
    vector<int> scores;
    int min_score = 999999;
    char *s = new char[misspell[i].length() + 1];
    strcpy(s, misspell[i].c_str());

    for (int j = 0; j < dict.size(); j ++) {
      char *t = new char[dict[j].length() + 1];
      strcpy(t, dict[j].c_str());
      int score = damerau_levenshtein_distance(s, t, strlen(s), strlen(t));
      scores.push_back(score);
      if (score < min_score) {
        min_score = score;
      }
      delete t;
    }

    for (int j = 0; j < scores.size(); j ++) {
      if (scores[j] == min_score) {
        matches.push_back(dict[j]);
      }
    }

    cout << misspell[i] << ", " << correct[i] << ": ";
    precision_denominator += matches.size();
    if (matches.size() > 0 && matches[0] == correct[i]) {
      accuracy_numerator ++;
    }
    for (int j = 0; j < matches.size(); j ++) {
      if (matches[j] == correct[i]) {
        precision_numerator ++;
        recall_numerator ++;
      }
      cout << matches[j] << ",";
    }
    cout << endl;

    delete s;
  }

  double accuracy = (accuracy_denominator == 0) ? 0 : (double)accuracy_numerator / (double)accuracy_denominator;
  double precision = (precision_denominator == 0) ? 0 : (double)precision_numerator / (double)precision_denominator;
  double recall = (recall_denominator == 0) ? 0 : (double)recall_numerator / (double)recall_denominator;

  cout << "accuracy = " << accuracy * 100 << "%" << endl;
  cout << "precision = " << precision * 100 << "%" << endl;
  cout << "recall = " << recall * 100 << "%" << endl;

}

/* Calculates scores of Global Edit Distance using Levenshtein Distance parameters */
void calculate_global_levenshtein_distance(vector<string> dict, vector<string> correct, vector<string> misspell) {

  int accuracy_denominator = misspell.size();
  int accuracy_numerator = 0;
  int recall_denominator = misspell.size();
  int recall_numerator = 0;
  int precision_denominator = 0;
  int precision_numerator = 0;

  for (int i = 0; i < misspell.size(); i ++) {
    vector<string> matches;
    vector<int> scores;
    int min_score = 999999;
    char *s = new char[misspell[i].length() + 1];
    strcpy(s, misspell[i].c_str());

    for (int j = 0; j < dict.size(); j ++) {
      char *t = new char[dict[j].length() + 1];
      strcpy(t, dict[j].c_str());
      int score = global_levenshtein_distance(s, t);
      scores.push_back(score);
      if (score < min_score) {
        min_score = score;
      }
      delete t;
    }

    for (int j = 0; j < scores.size(); j ++) {
      if (scores[j] == min_score) {
        matches.push_back(dict[j]);
      }
    }

    cout << misspell[i] << ", " << correct[i] << ": ";
    precision_denominator += matches.size();

    if (matches.size() > 0 && matches[0] == correct[i]) {
      accuracy_numerator ++;
    }

    for (int j = 0; j < matches.size(); j ++) {
      if (matches[j] == correct[i]) {
        precision_numerator ++;
        recall_numerator ++;
      }
      cout << matches[j] << ",";
    }
    cout << endl;
    delete s;
  }

  double accuracy = (accuracy_denominator == 0) ? 0 : (double)accuracy_numerator / (double)accuracy_denominator;
  double precision = (precision_denominator == 0) ? 0 : (double)precision_numerator / (double)precision_denominator;
  double recall = (recall_denominator == 0) ? 0 : (double)recall_numerator / (double)recall_denominator;

  cout << "accuracy = " << accuracy * 100 << "%" << endl;
  cout << "precision = " << precision * 100 << "%" << endl;
  cout << "recall = " << recall * 100 << "%" << endl;

}

/* Calculates scores of Local Edit Distance using Levenshtein Distance parameters */
void calculate_local_levenshtein_distance(vector<string> dict, vector<string> correct, vector<string> misspell) {

  int accuracy_denominator = misspell.size();
  int accuracy_numerator = 0;
  int recall_denominator = misspell.size();
  int recall_numerator = 0;
  int precision_denominator = 0;
  int precision_numerator = 0;

  for (int i = 0; i < misspell.size(); i ++) {
    vector<string> matches;
    vector<int> scores;
    int min_score = 999999;
    char *s = new char[misspell[i].length() + 1];
    strcpy(s, misspell[i].c_str());

    for (int j = 0; j < dict.size(); j ++) {
      char *t = new char[dict[j].length() + 1];
      strcpy(t, dict[j].c_str());
      int score = local_levenshtein_distance(s, t);
      scores.push_back(score);
      if (score < min_score) {
        min_score = score;
      }
      delete t;
    }

    for (int j = 0; j < scores.size(); j ++) {
      if (scores[j] == min_score) {
        matches.push_back(dict[j]);
      }
    }

    cout << misspell[i] << ", " << correct[i] << ": ";
    precision_denominator += matches.size();

    if (matches.size() > 0 && matches[0] == correct[i]) {
      accuracy_numerator ++;
    }

    for (int j = 0; j < matches.size(); j ++) {
      if (matches[j] == correct[i]) {
        precision_numerator ++;
        recall_numerator ++;
      }
      cout << matches[j] << ",";
    }
    cout << endl;
    delete s;
  }

  double accuracy = (accuracy_denominator == 0) ? 0 : (double)accuracy_numerator / (double)accuracy_denominator;
  double precision = (precision_denominator == 0) ? 0 : (double)precision_numerator / (double)precision_denominator;
  double recall = (recall_denominator == 0) ? 0 : (double)recall_numerator / (double)recall_denominator;

  cout << "accuracy = " << accuracy * 100 << "%" << endl;
  cout << "precision = " << precision * 100 << "%" << endl;
  cout << "recall = " << recall * 100 << "%" << endl;

}

/* Calculates scores of N-Gram */
void calculate_n_gram(vector<string> dict, vector<string> correct, vector<string> misspell, int n) {

  int accuracy_denominator = misspell.size();
  int accuracy_numerator = 0;
  int recall_denominator = misspell.size();
  int recall_numerator = 0;
  int precision_denominator = 0;
  int precision_numerator = 0;

  for (int i = 0; i < misspell.size(); i ++) {
    vector<int> nscore;
    vector<string> ngrams;

    int min_score = 999999;
    for (int j = 0; j < dict.size(); j ++) {

      string str_s(misspell[i]);
      string str_t(dict[j]);
      vector<string> ns = n_grams(str_s, n);
      vector<string> nt = n_grams(str_t, n);
      vector<string> ni;
      int score = 0;

      // debug
      // cout << "A    : ";
      // for (int i = 0; i < ns.size(); i ++) {
      //   cout << ns[i] << " ";
      // }
      // cout << endl << "B    : ";
      // for (int i = 0; i < nt.size(); i ++) {
      //   cout << nt[i] << " ";
      // }

      sort(ns.begin(), ns.end());
      sort(nt.begin(), nt.end());
      set_intersection(ns.begin(), ns.end(), nt.begin(), nt.end(), back_inserter(ni));

      // debug
      // cout << endl << "A ∩ B: ";
      // for (int i = 0; i < ni.size(); i ++) {
      //   cout << ni[i] << " ";
      // }
      // cout << endl;

      score = ns.size() + nt.size() - (2 * ni.size());
      if (score < min_score) {
        min_score = score;
      }
      nscore.push_back(score);
    }

    for (int j = 0; j < nscore.size(); j ++) {
      if (nscore[j] == min_score) {
        ngrams.push_back(dict[j]);
      }
    }

    cout << misspell[i] << ", " << correct[i] << ": ";
    precision_denominator += ngrams.size();

    if (ngrams.size() > 0 && ngrams[0] == correct[i]) {
      accuracy_numerator ++;
    }

    for (int j = 0; j < ngrams.size(); j ++) {
      if (ngrams[j] == correct[i]) {
        precision_numerator ++;
        recall_numerator ++;
      }
      cout << ngrams[j] << ",";
    }
    cout << endl;
  }

  double accuracy = (accuracy_denominator == 0) ? 0 : (double)accuracy_numerator / (double)accuracy_denominator;
  double precision = (precision_denominator == 0) ? 0 : (double)precision_numerator / (double)precision_denominator;
  double recall = (recall_denominator == 0) ? 0 : (double)recall_numerator / (double)recall_denominator;

  cout << "accuracy = " << accuracy * 100 << "%" << endl;
  cout << "precision = " << precision * 100 << "%" << endl;
  cout << "recall = " << recall * 100 << "%" << endl;
}

/* Calculates scores of Neighbourhood Search */
void calculate_neighbourhood_search(vector<string> dict, vector<string> correct, vector<string> misspell, int n) {

  vector<string> neighbours;
  int accuracy_denominator = misspell.size();
  int accuracy_numerator = 0;
  int recall_denominator = misspell.size();
  int recall_numerator = 0;
  int precision_denominator = 0;
  int precision_numerator = 0;

  for (int i = 0; i < misspell.size(); i ++) {
    neighbours = neighbourhood_search(misspell[i], dict, n);

    cout << misspell[i] << ", " << correct[i] << ": ";
    precision_denominator += neighbours.size();

    if (neighbours.size() > 0 && neighbours[0] == correct[i]) {
      accuracy_numerator ++;
    }

    for (int j = 0; j < neighbours.size(); j ++) {
      if (neighbours[j] == correct[i]) {
        precision_numerator ++;
        recall_numerator ++;
      }
      cout << neighbours[j] << ",";
    }
    cout << endl;
  }

  double accuracy = (accuracy_denominator == 0) ? 0 : (double)accuracy_numerator / (double)accuracy_denominator;
  double precision = (precision_denominator == 0) ? 0 : (double)precision_numerator / (double)precision_denominator;
  double recall = (recall_denominator == 0) ? 0 : (double)recall_numerator / (double)recall_denominator;

  cout << "accuracy = " << accuracy * 100 << "%" << endl;
  cout << "precision = " << precision * 100 << "%" << endl;
  cout << "recall = " << recall * 100 << "%" << endl;
}

/* Main function */
int main(int argc, char **argv) {

  // Start time
  clock_t tick, tock;
  tick = clock();

  // Check for correct number of arguments
  if (argc != 4) {
    cerr << "usage: " << argv[0] << " <dict_file> <correct> <misspell>" << endl;
    cerr << "example: " << argv[0] << " dict_file.txt wiki_correct.txt wiki_misspell.txt" << endl;
    cerr << "example: " << argv[0] << " dict_file.txt birkbeck_correct.txt birkbeck_misspell.txt" << endl;
    return 1;
  }

  // Open files
  fstream dict_file;
  dict_file.open(argv[1], ios::in);
  if (not dict_file || not dict_file.is_open()) {
    cerr << "file: " << argv[1] << " failed to open" << endl;
    return 1;
  }
  fstream correct_file;
  correct_file.open(argv[2], ios::in);
  if (not correct_file || not correct_file.is_open()) {
    cerr << "file: " << argv[2] << " failed to open" << endl;
    return 1;
  }
  fstream misspell_file;
  misspell_file.open(argv[3], ios::in);
  if (not misspell_file || not misspell_file.is_open()) {
    cerr << "file: " << argv[3] << " failed to open" << endl;
    return 1;
  }

  string entry;
  vector<string> dict;
  vector<string> correct;
  vector<string> misspell;

  // get items from dict
  if (dict_file.is_open()) {
    while (getline(dict_file, entry)) {
      dict.push_back(entry);
    }
  }
  // get items from correct
  if (correct_file.is_open()) {
    while (getline(correct_file, entry)) {
      correct.push_back(entry);
    }
  }
  // get items from misspell
  if (misspell_file.is_open()) {
    while (getline(misspell_file, entry)) {
      misspell.push_back(entry);
    }
  }

  // Close files
  dict_file.close();
  correct_file.close();
  misspell_file.close();

  // Ensure misspell and correct have the same lengths
  if (misspell.size() != correct.size()) {
    cout << "error: misspell and correct have different lengths" << endl;
    exit(1);
  }

  // processing
  calculate_n_gram(dict, correct, misspell, 2);

  // End time
  tock = clock();
  float diff = ((float) tock - (float) tick) / CLOCKS_PER_SEC;
  cout << "Time: " << diff << "s" << endl;

}
