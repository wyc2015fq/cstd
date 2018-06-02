
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "RESearch.inl"

struct aaa : public CharacterIndexer {
  const char* a;
  aaa(const char* aa) {a=aa;}
  virtual char CharAt(int index) {
    return a[index];
  }
};

int main() {
  RESearch re;
  const char* str = "\\(fo.*\\)-\\1";
  const char* str2 = "foo-foo fo-fo fob-fob foobar-foobar";
  aaa aa(str2);
  int slen = strlen(str);
  int slen2 = strlen(str2);
  re.Compile(str, slen, 0, 0);
  int tt = re.Execute(aa, 0, slen2);
  return 0;
}