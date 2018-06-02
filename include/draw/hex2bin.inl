#include "stdio.h"
#include "string.h"

int hex2bin(const char* hexstr, char* binstr)
{
  int lenhex = strlen(hexstr);
  int lenbin = 4 * lenhex;
  int i, j, bin10, t;
  const char* hexcode = "0123456789ABCDEF";
  int lencode = strlen(hexcode);

  for (i = 0; i < lenhex; ++i) {
    for (j = 0; j < lencode; j++) {
      if (hexstr[i] == hexcode[j]) {
        bin10 = j;

        for (t = 3; t >= 0; --t) {
          binstr[4 * i + t] = (bin10 & 1) + '0';
          bin10 >>= 1;
        }

        break;
      }
    }
  }

  return 0;
}

int hex2bin_table(const char* hexstr, char* binstr)
{
  static const char* bin_table[] = {
    "0000",
    "0001",
    "0010",
    "0011",
    "0100",
    "0101",
    "0110",
    "0111",

    "1000", //8
    "1001",
    "1010",
    "1011",
    "1100",
    "1101",
    "1110",
    "1111",
  };
  static int inited = 0;
  static unsigned char hexmap2dec[256] = {0};
  int i;
  int* outbin = (int*)binstr;
  int slen = strlen(hexstr);

  if (0 == inited) {
    const char* hexcode = "0123456789ABCDEF";

    for (i = 0; hexcode[i]; ++i) {
      hexmap2dec[hexcode[i]] = i;
    }

    inited = 1;
  }

  for (i = 0; i < slen; ++i) {
    const char* sbin = bin_table[hexmap2dec[hexstr[i]]];
    outbin[i] = *(int*)sbin;
  }

  binstr[4 * slen] = 0;
  return 0;
}

int test_hex2bin()
{
  char* hexstr = "ABC022683160ABDF";
  char binstr[256] = "";
  char binstr2[256] = "";
  hex2bin(hexstr, binstr);
  hex2bin_table(hexstr, binstr2);
  ASSERT(0 == strcmp(binstr, binstr2));
  printf("%s\n", binstr);
  return 0;
}