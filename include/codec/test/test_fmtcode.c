
#include <stdio.h>


#include "findfile.h"
int fmtcode(char* s, char* send, char* d, char** pd);

void do_any(const TCHAR* spec, fileinfo_t* info, void* userdata) {
  // printf("%s %s\n", spec, fileinfo_filename(*info));
  char infile[256];
  char outfile[256];
  slipdir(spec, infile);
  strcat(infile, fileinfo_filename(*info));
  slipdir(spec, outfile);
  ///strcat(outfile, "__");
  strcat(outfile, fileinfo_filename(*info));
  findtxtfile(infile, outfile, fmtcode);
}

int main() {
  findfile("F:\\public\\bin\\文本工具集\\fmtcode\\*.cpp", do_any, 1, 0);
  //test_fmtcode();
  return 0;
}
