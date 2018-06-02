Quque.h文件
#ifndef QUQUE_H_INCLUDED
#define QUQUE_H_INCLUDED
#include "ac.h"
struct struct_quque
{
  int q[MAX_STATE];
  int num;
};
typedef struct struct_quque quque;
void init_bfs_quque(quque** bfs_quque);
int is_empty(quque* bfs_quque);
void in_quque(int n, quque* bfs_quque);
int out_quque(quque* bfs_quque);
#endif // QUQUE_H_INCLUDED
Quque.c文件
#include "quque.h"
#include <stdlib.h>
void init_bfs_quque(quque** bfs_quque)
{
  (*bfs_quque) = (quque*)malloc(sizeof(quque));
  (*bfs_quque)->num = 0;
}
int is_empty(quque* bfs_quque)
{
  return bfs_quque->num;
}
void in_quque(int n, quque* bfs_quque)
{
  int i = 0;
  for (i = bfs_quque->num; i > 0; i--)
  {
    bfs_quque->q[i] = bfs_quque->q[i - 1];
  }
  bfs_quque->q[0] = n;
  bfs_quque->num++;
}
int out_quque(quque* bfs_quque)
{
  bfs_quque->num--;
  return bfs_quque->q[bfs_quque->num];
}
Ac.h文件
#ifndef AC_H_INCLUDED
#define AC_H_INCLUDED
#define MAX_STATE 100 //自动机最大状态数
#define MAX_SYMBOL 256 //匹配的字符数可以匹配所有的ASCII码
#define MAX_MODE 4 //最大模式串数
#define MODE_LEN 10
int DFD[MAX_STATE][MAX_SYMBOL]; //状态转移表--DFA包括转向函数和失效函数
int F[MAX_STATE] ; //失败函数
struct output_t
{
  int flag;
  char str[MODE_LEN];
} output[MAX_STATE];
int father[MAX_STATE];
unsigned int statecount; //总状态数-1
unsigned int modecount; //模式串数-1
struct struct_quque* bfs_quque;
void printAC();
void init_ac();
void go();
void fail();
void output_str(int state, int j);
void prec();
int AC(unsigned char* s, int http_len);
#endif // AC_H_INCLUDED
Ac.c文件
#include "ac.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "quque.h"
void printAC()
{
  int i = 0;
  int j = 0;
  printf("DFD\n");
  for (i = 0; i < MAX_STATE; i++)
  {
    for (j = 0; j < MAX_SYMBOL; j++)
    {
      printf("%d ", DFD[i][j]);
      if (j == MAX_SYMBOL - 1)
      {
        printf("\n");
      }
    }
  }
  printf("F\n");
  for (i = 0; i < MAX_STATE; i++)
  {
    printf("%d ", F[i]);
  }
  printf("\noutput\n");
  for (i = 0; i < MAX_STATE; i++)
  {
    printf("%d %s\n", output[i].flag, output[i].str);
  }
}
void init_ac()
{
  int i, j;
  statecount = 1;
  modecount = 0;
  for (i = 0 ; i < MAX_STATE; i++)
  {
    output[i].flag = -1;
    output[i].str[0] = '\0';
    F[i] = 0;
    father[i] = 0;
    for (j = 0 ; j < MAX_SYMBOL; j++)
    {
      DFD[i][j] = 0;
    }
  }
}
void go()
{
  int c;
  int currentstate ;
  char str[MODE_LEN];
  int start = 1;
  int temp = 0;
  getchar();
  printf("input patter strings,interval with blank and end with enter:\n");
  while ((c = getchar()) != '\n')
  {
    if (c != ' ')
    {
      if (start)
      {
        start = 0;
        modecount++;
        currentstate = 0;
        temp = 0;
      }
      str[temp++] = c;
      if (DFD[currentstate][c] == 0)
      {
        DFD[currentstate][c] = statecount;
        father[statecount] = currentstate;
        currentstate = statecount;
        statecount++;
      }
      else
      {
        currentstate = DFD[currentstate][c];
      }
    }
    else
    {
      output[currentstate].flag = 1;
      str[temp] = '\0';
      strcpy(output[currentstate].str, str);
      start = 1;
      if (modecount == MAX_MODE)
      {
        printf("Max patter string number is: %d", MAX_MODE);
        goto END;
      }
    }
  }
  output[currentstate].flag = 1;
  str[temp] = '\0';
  strcpy(output[currentstate].str, str);
  return;
END:
  printf("Press any key to continue...");
  c = getch();
  return;
}
void fail()
{
  int i, j, t;
  int ch;
  init_bfs_quque(&bfs_quque);
  in_quque(0, bfs_quque);
  while (is_empty(bfs_quque))
  {
    t = out_quque(bfs_quque);
    if (t)
    {
      for (i = 0; i < MAX_SYMBOL; i++)
      {
        if (DFD[father[t]][i] == t)
        {
          ch = i;
          break;
        }
      }
      j = t;
      while ((j = F[father[j]]) != 0)
      {
        if (DFD[j][ch])
        {
          F[t] = DFD[j][ch];
          break;
        }
      }
      if (j == 0)
      {
        F[t] = (DFD[j][ch] == t) ? 0 : DFD[j][ch];
      }
    }
    for (i = 0 ; i < MAX_SYMBOL; i++)
    {
      if (DFD[t][i] != 0)
      {
        in_quque(DFD[t][i], bfs_quque);
      }
    }
  }
}
void output_str(int state, int j)
{
  do
  {
    if (output[state].str[0])
    {
      printf("%d %s\n", j - strlen(output[state].str) + 1, output[state].str);
    }
  }
  while ((state = F[state]) != 0);
}
void prec()
{
  printf("init ac\n");
  //初始化变量
  init_ac();
  //建立转向函数
  go();
  //建立失效函数
  fail();
  //打印自动机
  //printAC();
}
int AC(unsigned char* s, int http_len)
{
  int j = -1;
  int flag = 0;
  int currentstate = 0;
  int state = 0;
  int i = 0;
  while (j++ < http_len)
  {
    if (s[j] < MAX_SYMBOL)
    {
      state = DFD[currentstate][s[j]];
      if (state == 0)
      {
        while (currentstate && (!state))
        {
          state = DFD[F[currentstate]][s[j]];
          currentstate = state;
        }
      }
      else
      {
        currentstate = state;
      }
      if (output[currentstate].flag == 1)
      {
        output_str(currentstate, j);
        flag = 1;
      }
    }
  }
  return flag;
}
Actest.c文件
#include "ac.h"
int main(int argc, char** argv)
{
  char str[100];
  scanf("%s", str);
  prec();
  AC(str, strlen(str));
}

