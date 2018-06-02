
#include <string.h>
#include <stdio.h>
#include "md5.h"
#include "findfile.h"
#include "sort.h"
#include "macro.h"

int get_md5( const unsigned char* plainText, int len, char* digestString ) {
  int p = 0, i;
  unsigned char digest[ 16 ]; //the numerical value of the digest
  
  MD5_CTX context;
  MD5Init( &context );
  
  //the alternative to these ugly casts is to go into the RFC code and change the declarations
  MD5Update( &context, ( unsigned char* ) ( plainText ), len );
  MD5Final( ( digest ), &context );
  
  if (digestString) {
    //make a string version of the numeric digest value
    for ( i = 0; i < 16; i++ ) {
      sprintf( &digestString[ p ], "%02x", digest[ i ] );
      p += 2;
    }
  }
  return 1;
}

int get_md52( const unsigned char* plainText, int len, char* digestString ) {
  int p = 0, i;
  unsigned char digest[ 16 ]; //the numerical value of the digest
  
  MD5_CTX context;
  MD5Init( &context );
  
  //the alternative to these ugly casts is to go into the RFC code and change the declarations
  for (i=0; i<len; ++i) {
    MD5Update( &context, ( unsigned char* ) ( plainText )+i, 1 );
  }
  MD5Final( ( digest ), &context );
  
  if (digestString) {
    //make a string version of the numeric digest value
    for ( i = 0; i < 16; i++ ) {
      sprintf( &digestString[ p ], "%02x", digest[ i ] );
      p += 2;
    }
  }
  return 1;
}

int test_md5() {
  strlist_t slist;
  int i, flen;
  char* buf;
  char digestString[ 36 ];  //Null terminated string value of the digest expressed in hex digits
  get_file_list("F:\\code\\cstd\\*.*", &slist);
  for (i=0; i<slist.used; ++i) {
    buf = filemap(slist.strs[i], &flen);
    if (buf) {
      get_md5( buf, flen, digestString );
      printf("%s %s\n", digestString, slist.strs[i]);
      free(buf);
    } else {
      //printf("%s\n", slist.strs[i]);
    }
  }
  free_strlist(&slist);
  return 0;
}

#define fdd_fine_info_less(f1, f2)  ((f1).fsize < (f2).fsize)

int test_finddup() {
  strlist_t slist;
  int i;
  typedef struct fdd_fine_info_t { char* fname; int64 fsize; } fdd_fine_info_t;
  fdd_fine_info_t* fddfi;
  get_file_list(".\\*.*", &slist);
  fddfi = MALLOC(fdd_fine_info_t, slist.used);
  for (i=0; i<slist.used; ++i) {
    fddfi[i].fname = slist.strs[i];
    fddfi[i].fsize = get_file_size(slist.strs[i]);
  }
  QSORT(fddfi, fddfi+slist.used, fdd_fine_info_less, fdd_fine_info_t);
  for (i=0; i<slist.used; ++i) {
    char* buf = fddfi[i].fname;
    printf("%9d %s\n", (int)fddfi[i].fsize, buf);
  }
  SAFEFREE(fddfi);
  free_strlist(&slist);
  return 0;
}


int main() {
  test_md5();
  test_finddup();
  return 0;
}