c语言实现通用数据结构（三）：通用椎栈
2014年03月31日 13: 25: 36 swwlqw 阅读数：932
这是在通用链表的基础上实现的椎栈，关于链表的实现参见：http://blog.csdn.net/swwlqw/article/details/22498833。

这里所说的椎栈就是指的栈。

注意椎栈中只存储了指针，没有储存实际的数据。



头文件：


/*************************
*** File myStack.h
**************************/
#ifndef MYSTACK_H_INCLUDED
#define MYSTACK_H_INCLUDED

#include "myList.h"
typedef MyList MyStack;

//创建堆栈
MyStack* createMyStack();

//释放堆栈
void freeMyStack(MyStack* stack);

//加入
void myStackAdd(MyStack* const stack, void* const data);

//删除
void* myStackRemove(MyStack* const stack);

//取得堆栈头
void* myStackGetTop(const MyStack* const stack);

#endif // MYSTACK_H_INCLUDED


源文件


/*************************
*** File myStack.c
**************************/
#include "myStack.h"

//创建堆栈
MyStack* createMyStack()
{
  return createMyList();
}

//释放堆栈
void freeMyStack(MyStack* stack)
{
  freeMyList(stack);
}

//加入
void myStackAdd(MyStack* const stack, void* const data)
{
  myListInsertDataAtFirst(stack, data);
}

//删除
void* myStackRemove(MyStack* const stack)
{
  return myListRemoveDataAtFirst(stack);
}

//取得堆栈头
void* myStackGetTop(const MyStack* const stack)
{
  return myListGetDataAtFirst(stack);
}


测试文件


/*************************
*** File main.c
*** test for MyStack
**************************/
#include <stdio.h>
#include <stdlib.h>
#include "myStack.h"

typedef struct a {
  int i;
  char c;
} A;


int main()
{
  const int S = 10;
  //创建并初始化数据
  A* data = malloc(sizeof(A) * S);
  for (int i = 0; i < S; i++) {
    data[i].i = i;
    data[i].c = (char)('A' + i);
  }
  //创建椎栈
  MyStack* stack = createMyStack();
  //插入数据
  myStackAdd(stack, &data[0]);
  myStackAdd(stack, &data[2]);
  myStackAdd(stack, &data[6]);
  //测试删除
  while (myListGetSize(stack)) {
    A* pp = myStackRemove(stack);
    printf("%d[%c] ", pp->i, pp->c);
  }
  puts("");
  //释放椎栈
  freeMyStack(stack);
  //释放数据
  free(data);
  return 0;
}