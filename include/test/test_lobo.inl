// 卜红丽 的面试题
#include <stdio.h>
#include <stdlib.h>
void* align_ptr(void* ptr, unsigned int align)
{
  int iptr = (int)ptr;
  iptr = (iptr + align - 1) / align;
  iptr *= align;
  return (void*)iptr;
}
void* aligned_malloc(unsigned int size, unsigned int align)
{
  int n = (size + align - 1 + sizeof(char*)) / align;
  char* ptr0 = (char*)malloc(n * align);
  char** ptr1 = (char**)align_ptr(ptr0 + sizeof(char*), align);
  ptr1[-1] = ptr0;
  return (void*)ptr1;
}
void aligned_free(void* ptr)
{
  char** ptr1 = (char**)ptr;
  char* ptr0 = ptr1[-1];
  free(ptr0);
}
int test_aligned_malloc()
{
  void* aa = aligned_malloc(1000, 7);
  aligned_free(aa);
  return 0;
}
#include <assert.h>
#include <string.h>
static const char* binstr[16] = {
  "0000",//0
  "0001",//1
  "0010",//2
  "0011",//3
  "0100",//4
  "0101",//5
  "0110",//6
  "0111",//7
  "1000",//8
  "1001",//9
  "1010",//10
  "1011",//11
  "1100",//12
  "1101",//13
  "1110",//14
  "1111",//15
};
void f_u8(unsigned char a, char* str, int n)
{
  int* s4 = (int*)str;
  s4[0] = *(int*)binstr[a >> 4];
  s4[1] = *(int*)binstr[a & 0xf];
  return ;
}
// 十进制转二进制字符串（字符串高位在前）
void f(int a, char* str, int n)
{
  assert(n > 32);
  // 查表算法
  f_u8((a >> 24), str, 8);
  f_u8((a >> 16) & 0xff, str + 8, 8);
  f_u8((a >> 8) & 0xff, str + 16, 8);
  f_u8(a & 0xff, str + 24, 8);
  str[32] = '\0';
  return ;
}
int test_f()
{
  char buf[256] = {0};
  f(0x12345678, buf, 256);
  assert(0 == strcmp(buf, "00010010001101000101011001111000"));
  return 0;
}
// 1维积分 要求 countof(sum)==n+1
int sumcnt(int n, const int* list, int* sum)
{
  int i;
  sum[0] = 0;
  for (i = 0; i < n; ++i) {
    sum[i + 1] = sum[i] + list[i];
  }
  return 0;
}
// 最大和连续子序列
// 返回0表示失败 返回1表示成功
int sublist_maxsum(int n, const int* list, int* pmaxsum)
{
  if (n > 0) {
    int* cnt = (int*)malloc((n + 1) * n);
    int i, j, maxs = list[0];
    sumcnt(n, list, cnt);
    for (i = 0; i < n; ++i) {
      for (j = i + 1; j <= n; ++j) {
        int s = cnt[j] - cnt[i];
        if (s > maxs) {
          maxs = s;
        }
      }
    }
    free(cnt);
    if (pmaxsum) {
      *pmaxsum = maxs;
    }
    return 1;
  }
  // 失败 n必须>0
  return 0;
}
int test_sublist_maxsum()
{
  int list[] = { -2, 11, -4, 13, -5, -2};
  int mm = 0;
  sublist_maxsum(6, list, &mm);
  return 0;
}
int test_lobo()
{
  test_aligned_malloc();
  return 0;
}

