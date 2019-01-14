
#ifndef MYHASHCODE_H_INCLUDED
#define MYHASHCODE_H_INCLUDED

#include <string.h>

#define HASHCODE_MULT 31


//源文件

/*************************
*** File myHashCode.c
**************************/
#include "strhash.h"



static size_t myHashCodeDefault(const void* p)
{
  size_t h = (size_t)p;
  return (h) ^ (h >> 16);
}

//string类型的hashCode
static size_t myHashCodeString(void* a)
{
  size_t re = 0;
  unsigned char* aa = (unsigned char*) a;
  return RSHash((char*)aa, strlen((char*)aa));
  while (*aa) {
    re += HASHCODE_MULT * *aa;
    aa++;
  }
  return re;
}

//默认的相等的方法
static int myCmpDefault(const void* a, const void* b)
{
  return a == b;
}

//int类型相等的方法
static int myCmpInt(const void* a, const void* b)
{
  int* aa = (int*) a;
  int* bb = (int*) b;
  return CC_CMP(*aa, *bb);
}

//char类型相等的方法
static int myCmpChar(const void* a, const void* b)
{
  char* aa = (char*) a;
  char* bb = (char*) b;
  return CC_CMP(*aa, *bb);
}

//string类型相等的方法
static int myCmpString(const void* a, const void* b)
{
  char* aa = (char*) a;
  char* bb = (char*) b;
  return strcmp(aa, bb);
}



#endif // MYHASHCODE_H_INCLUDED
