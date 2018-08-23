#include <stdio.h>
#include <ctype.h>
#include <string.h>
#include <stdlib.h>
#include <getopt.h>

string concat(vector<string> words, int start, int end) {
  string s;
  int i;
  for (i = start; i < end; i ++) {
    s += ((i > start) ? " " : "") + words[i];
  }
  return s;
}

void n_gram(string str, int n) {
  vector<string> ngrams;
  stringstream words(str);
  string segment;
  vector<string> seglist;

  while (getline(words, segment, ' ')) {
    seglist.push_back(segment);
  }

  int i;
  for (i = 0; i < seglist.size() -n + 1; i ++) {
    ngrams.push_back(concat(seglist, i , i + n));
  }
  for (i = 0; i < ngrams.size(); i ++) {
    cout << ngrams[i] << endl;
  }
  // return ngrams;
}
