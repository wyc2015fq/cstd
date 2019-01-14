c语言实现通用数据结构（二）：通用队列
2014年03月30日 23: 15: 08 swwlqw 阅读数：1508 标签： c语言 数据结构 队列  更多
个人分类： c语言实现通用数据结构
这是在通用链表的基础上实现的队列，关于链表的实现参见：http://blog.csdn.net/swwlqw/article/details/22498833。

注意队列中只存储了指针，没有储存实际的数据。



头文件 myQueue.h


#ifndef MYQUEUE_H_INCLUDED
#define MYQUEUE_H_INCLUDED

#include "myList.h"

typedef MyList MyQueue;

//创建队列
MyQueue* createMyQueue();

//释放队列
void freeMyQueue(MyQueue* queue);

//插入
void myQueueAdd(MyQueue* const queue, void* const data);

//删除
void* myQueueRemove(MyQueue* const queue);

//取得队头
void* myQueueGetTop(const MyQueue* const queue);

#endif // MYQUEUE_H_INCLUDED


源文件 myQueue.c


#include "myQueue.h"

//创建队列
MyQueue* createMyQueue()
{
  return createMyList();
}

//释放队列
void freeMyQueue(MyQueue* queue)
{
  freeMyList(queue);
}

//插入
void myQueueAdd(MyQueue* const queue, void* const data)
{
  myListInsertDataAtLast(queue, data);
}

//删除
void* myQueueRemove(MyQueue* const queue)
{
  return myListRemoveDataAtFirst(queue);
}

//取得队头
void* myQueueGetTop(const MyQueue* const queue)
{
  return myListGetDataAtFirst(queue);
}



测试文件 main.c

/*************************
*** File main.c
*** test for MyQueue
**************************/
#include <stdio.h>
#include <stdlib.h>
#include "myQueue.h"

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
  //创建队列
  MyQueue* queue = createMyQueue();
  //插入数据
  myQueueAdd(queue, &data[0]);
  myQueueAdd(queue, &data[2]);
  myQueueAdd(queue, &data[6]);
  //测试删除
  while (myListGetSize(queue)) {
    A* pp = myQueueRemove(queue);
    printf("%d[%c] ", pp->i, pp->c);
  }
  puts("");
  //释放链表
  freeMyQueue(queue);
  //释放数据
  free(data);
  return 0;
}


