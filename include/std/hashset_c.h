// HashSet
//对于新的数据类型来说，需要自定义HashCode函数和equal函数。
//下面还给出了几个常见的hashCode函数和equal函数。

#ifndef __HASHSET_C_H__
#define __HASHSET_C_H__

#include "hashcode_c.h"

#ifndef MYHASHSET_H_INCLUDED
#define MYHASHSET_H_INCLUDED


typedef struct myHashSet {
  int size; //大小
  int initialCapacity; //初始容量
  float loadFactor; //加载因子
  int (*hashCode)(void* data);
  int (*equal)(void* data1, void* data2);
  MyList** dataList;
} MyHashSet;

typedef struct myHashSetIterator {
  int index; //第几个链表
  MyHashSet* set;
  MyNode* current;
  int count; //第几个数据
} MyHashSetIterator;

//创建HashSet
MyHashSet* createMyHashSet(int (*hashCode)(void* data), int (*equal)(void* data1, void* data2));

//使用全部参数创建HashSet
MyHashSet* createMyHashSetForAll(int initialCapacity, float loadFactor, int (*hashCode)(void* data), int (*equal)(void* data1, void* data2));

//释放HashSet
void freeMyHashSet(MyHashSet* set);

//是否包含某个data
int myHashSetContains(MyHashSet* const set, void* const data);

//增加一条数据,返回是否添加成功
int myHashSetAddData(MyHashSet* const set, void* const data);

//数据的容量
int myHashSetGetSize(const MyHashSet* const set);

//创建迭代器
MyHashSetIterator* createMyHashSetIterator(MyHashSet* const set);

//释放迭代器
void freeMyHashSetIterator(MyHashSetIterator* iterator);

//迭代器是否有下一个
int myHashSetIteratorHasNext(MyHashSetIterator* iterator);

//遍历下一个元素
void* myHashSetIteratorNext(MyHashSetIterator* iterator);

//删除一条数据，返回是否删除成功
int myHashSetRemoveData(MyHashSet* const set, void* const data);

//将第二个Set的全部对象加入到第一个，返回增加的个数
int myHashSetAddAllSet(MyHashSet* set, MyHashSet* other);

//复制HashSet
MyHashSet* myHashSetCopy(MyHashSet* set);

//遍历
void myHashSetOutput(MyHashSet* set, void(*pt)(void*));

#endif // MYHASHSET_H_INCLUDED

#include <stdlib.h>

//创建HashSet
MyHashSet* createMyHashSet(int(*hashCode)(void* data), int(*equal)(void* data1, void* data2))
{
  MyHashSet* re = (MyHashSet*)malloc(sizeof(MyHashSet));
  re->size = 0;
  re->loadFactor = DEFAULT_LOAD_FACTOR;
  re->initialCapacity = DEFAULT_INITIAL_CAPACITY;
  re->dataList = (MyList**) malloc(sizeof(MyList*) * re->initialCapacity);
  re->hashCode = hashCode;
  re->equal = equal;
  for (int i = 0; i < re->initialCapacity; i++) {
    re->dataList[i] = createMySearchList(equal);
  }
  return re;
}

//使用全部参数创建HashSet
MyHashSet* createMyHashSetForAll(int initialCapacity, float loadFactor, int(*hashCode)(void* data), int(*equal)(void* data1, void* data2))
{
  MyHashSet* re = createMyHashSet(hashCode, equal);
  re->initialCapacity = initialCapacity;
  re->loadFactor = loadFactor;
  return re;
}

//释放HashSet
void freeMyHashSet(MyHashSet* set)
{
  for (int i = 0; i < set->initialCapacity; i++) {
    freeMyList(set->dataList[i]);
  }
  free(set->dataList);
  free(set);
}

//是否包含某个data
int myHashSetContains(MyHashSet* const set, void* const data)
{
  int hasCode = (*(set->hashCode))(data);
  hasCode %= set->initialCapacity;
  if (hasCode < 0) {
    hasCode += set->initialCapacity;
  }
  int re = myListFindDataIndex(set->dataList[hasCode], data);
  return re > -1;
}

void rebuildMyHashSet(MyHashSet* set)
{
  int newSize = set->initialCapacity * 2;
  MyList** newdataList = (MyList**) malloc(sizeof(MyList*) * newSize);
  for (int i = 0; i < newSize; i++) {
    newdataList[i] = createMyList();
  }
  MyHashSetIterator* it = createMyHashSetIterator(set);
  while (myHashSetIteratorHasNext(it)) {
    void* data = myHashSetIteratorNext(it);
    int hasCode = (*(set->hashCode))(data);
    hasCode %= newSize;
    myListInsertDataAtLast(newdataList[hasCode], data);
  }
  freeMyHashSetIterator(it);
  for (int i = 0; i < set->initialCapacity; i++) {
    freeMyList(set->dataList[i]);
  }
  free(set->dataList);
  set->dataList = newdataList;
  set->initialCapacity = newSize;
}

//增加一条数据,返回是否添加成功
int myHashSetAddData(MyHashSet* const set, void* const data)
{
  int hasCode = (*(set->hashCode))(data);
  hasCode %= set->initialCapacity;
  if (hasCode < 0) {
    hasCode += set->initialCapacity;
  }
  int re = myListFindDataIndex(set->dataList[hasCode], data);
  if (re == -1) {
    myListInsertDataAtLast(set->dataList[hasCode], data);
    (set->size)++;
    if (set->size > set->initialCapacity * set->loadFactor) {
      rebuildMyHashSet(set);
    }
    return 1;
  }
  return 0;
}

//数据的容量
int myHashSetGetSize(const MyHashSet* const set)
{
  return set->size;
}

//创建迭代器
MyHashSetIterator* createMyHashSetIterator(MyHashSet* const set)
{
  MyHashSetIterator* re = (MyHashSetIterator*) malloc(
                            sizeof(MyHashSetIterator));
  re->count = 0;
  re->index = 0;
  re->set = set;
  re->current = set->dataList[0]->first;
  return re;
}

//释放迭代器
void freeMyHashSetIterator(MyHashSetIterator* iterator)
{
  free(iterator);
}

//迭代器是否有下一个
int myHashSetIteratorHasNext(MyHashSetIterator* iterator)
{
  return iterator->count < iterator->set->size;
}

//遍历下一个元素
void* myHashSetIteratorNext(MyHashSetIterator* iterator)
{
  (iterator->count)++;
  while (!(iterator->current)) {
    (iterator->index)++;
    iterator->current = iterator->set->dataList[iterator->index]->first;
  }
  void* re = iterator->current->data;
  iterator->current = iterator->current->next;
  return re;
}

//删除一条数据，返回是否删除成功
int myHashSetRemoveData(MyHashSet* const set, void* const data)
{
  int hasCode = (*(set->hashCode))(data);
  hasCode %= set->initialCapacity;
  if (hasCode < 0) {
    hasCode += set->initialCapacity;
  }
  int re = myListRemoveDataObject(set->dataList[hasCode], data);
  if (re) {
    (set->size)--;
  }
  return re;
}

//将第二个Set的全部对象加入到第一个，返回增加的个数
int myHashSetAddAllSet(MyHashSet* set, MyHashSet* other)
{
  int ssize = set->size;
  MyHashSetIterator* it = createMyHashSetIterator(other);
  while (myHashSetIteratorHasNext(it)) {
    myHashSetAddData(set, myHashSetIteratorNext(it));
  }
  freeMyHashSetIterator(it);
  int re = set->size - ssize;
  return re;
}

//复制HashSet
MyHashSet* myHashSetCopy(MyHashSet* set)
{
  MyHashSet* re = createMyHashSetForAll(set->initialCapacity, set->loadFactor, set->hashCode, set->equal);
  myHashSetAddAllSet(re, set);
  return re;
}

//遍历
void myHashSetOutput(MyHashSet* set, void(*pt)(void*))
{
  MyHashSetIterator* it = createMyHashSetIterator(set);
  while (myHashSetIteratorHasNext(it)) {
    pt(myHashSetIteratorNext(it));
  }
  freeMyHashSetIterator(it);
}

//（4）测试文件

/*************************
*** File main.c
*** test for MyHashSet
**************************/
#include <stdio.h>
#include <stdlib.h>


int test_hashset_c()
{
  char* strs[] = {
    "abc",
    "qq",
    "hello",
    "abc",
    "lmy",
    "ab",
    "qq",
    "lqw",
    "sww",
    "lqw"
  };
  int S = countof(strs);
  //创建集合需要指定两个函数，hashCode函数和equal函数。
  MyHashSet* set = createMyHashSet(myHashCodeString, myEqualString);
  //插入数据
  for (int i = 0; i < S; i++) {
    myHashSetAddData(set, strs[i]);
  }
  //输出大小
  printf("size=%d\n", myHashSetGetSize(set));
  //测试删除
  myHashSetRemoveData(set, "qq");
  myHashSetRemoveData(set, "ab");
  myHashSetRemoveData(set, "qwert");
  //输出大小
  printf("after remove size=%d\n", myHashSetGetSize(set));
  //遍历
  MyHashSetIterator* it = createMyHashSetIterator(set);
  while (myHashSetIteratorHasNext(it)) {
    char* pp = (char*)myHashSetIteratorNext(it);
    puts(pp);
  }
  //释放遍历器
  freeMyHashSetIterator(it);
  //释放集合
  freeMyHashSet(set);
  return 0;
}


#endif // __HASHSET_C_H__
