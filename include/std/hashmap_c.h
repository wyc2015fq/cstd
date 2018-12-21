// HashMap
//这是在通用链表的基础上实现的映射，关于链表的实现参见：http://blog.csdn.net/swwlqw/article/details/22498833。
//注意映射中只存储了key和value的指针，没有储存实际的数据。
//对于新的key类型来说，需要自定义HashCode函数和equal函数。
//在HashSet的实现中给出了几个常见的hashCode函数和equal函数。参见：http://blog.csdn.net/swwlqw/article/details/22664129。

#ifndef MYHASHMAP_H_INCLUDED
#define MYHASHMAP_H_INCLUDED
#include "list_c.h"
#include "hashset_c.h"
 
 
typedef struct entry
{
    void * key;
    void * value;
} Entry;
 
typedef struct myHashMap
{
    int size;   //大小
    int initialCapacity; //初始容量
    float loadFactor;   //加载因子
    int (*hashCode)(void *key);
    int (*equal)(void *key1,void *key2);
    MyList ** entryList;
} MyHashMap;
 
typedef struct myHashMapEntryIterator
{
    int index;       //第几个链表
    MyHashMap *map;
    MyNode *current;
    int count;        //第几个数据
} MyHashMapEntryIterator;
 
//创建HashMap
MyHashMap *createMyHashMap(int (*hashCode)(void *key),int (*equal)(void *key1,void *key2));
 
//使用全部参数创建HashMap
MyHashMap *createMyHashMapForAll(int initialCapacity,float loadFactor,int (*hashCode)(void *key),int (*equal)(void *key1,void *key2));
 
//释放HashMap
void freeMyHashMap(MyHashMap * map);
 
//是否包含某个key
int myHashMapContainsKey(MyHashMap *const map,void * const key);
 
//增加一条映射
void myHashMapPutData(MyHashMap *const map,void * const key,void * const value);
 
//通过key得到数据，如果没有数据则返回null
void* myHashMapGetDataByKey(MyHashMap * const map,void *const key);
 
//数据的容量
int myHashMapGetSize(const MyHashMap * const map);
 
//创建Entry迭代器
MyHashMapEntryIterator* createMyHashMapEntryIterator( MyHashMap *const map);
 
//释放Entry迭代器
void freeMyHashMapEntryIterator(MyHashMapEntryIterator* iterator);
 
//Entry迭代器是否有下一个
int myHashMapEntryIteratorHasNext(MyHashMapEntryIterator* iterator);
 
//遍历下一个Entry元素
Entry* myHashMapEntryIteratorNext(MyHashMapEntryIterator* iterator);
 
//删除一条数据，返回是否删除成功
int myHashMapRemoveDataByKey(MyHashMap *const map,void * const key);
 
//遍历
void myHashMapOutput(MyHashMap *map, void(*pt)(Entry*));
 
#endif // MYHASHMAP_H_INCLUDED

#include <stdlib.h>
 
//某条Entry链表上是否包含某个key值。
Entry* listContainsEntry(MyList * list, void * key,
		int(*equal)(void *key1, void *key2)) {
	MyListIterator* it = createMyListIterator(list);
	while (myListIteratorHasNext(it)) {
		Entry * entry = (Entry *) (myListIteratorNext(it));
		if (entry->key == key || (equal != NULL && (*equal)(entry->key, key))) {
			return entry;
		}
	}
	freeMyListIterator(it);
	return NULL;
}
 
void rebuildMyHashMap(MyHashMap * map) {
	int newSize = map->initialCapacity * 2;
	MyList **newentryList = (MyList **) malloc(sizeof(MyList*) * newSize);
	for (int i = 0; i < newSize; i++) {
		newentryList[i] = createMyList();
	}
	MyHashMapEntryIterator* it = createMyHashMapEntryIterator(map);
	while (myHashMapEntryIteratorHasNext(it)) {
		Entry * entry = myHashMapEntryIteratorNext(it);
		int hasCode = (*(map->hashCode))(entry->key);
		hasCode %= newSize;
		if (hasCode < 0)
			hasCode += newSize;
		myListInsertDataAtLast(newentryList[hasCode], entry);
	}
	freeMyHashMapEntryIterator(it);
	for (int i = 0; i < map->initialCapacity; i++) {
		freeMyList(map->entryList[i]);
	}
	free(map->entryList);
	map->entryList = newentryList;
	map->initialCapacity = newSize;
}
 
//创建HashMap
MyHashMap *createMyHashMap(int(*hashCode)(void *key),
		int(*equal)(void *key1, void *key2)) {
	MyHashMap *re = (MyHashMap *) malloc(sizeof(MyHashMap));
	re->size = 0;
	re->loadFactor = DEFAULT_LOAD_FACTOR;
	re->initialCapacity = DEFAULT_INITIAL_CAPACITY;
	re->entryList = (MyList **) malloc(sizeof(MyList*) * re->initialCapacity);
	re->hashCode = hashCode;
	re->equal = equal;
	for (int i = 0; i < re->initialCapacity; i++) {
		re->entryList[i] = createMyList();
	}
	return re;
}
 
//使用全部参数创建HashMap
MyHashMap *createMyHashMapForAll(int initialCapacity, float loadFactor,
		int(*hashCode)(void *key), int(*equal)(void *key1, void *key2)) {
	MyHashMap *re = createMyHashMap(hashCode, equal);
	re->initialCapacity = initialCapacity;
	re->loadFactor = loadFactor;
	return re;
}
 
//是否包含某个key
int myHashMapContainsKey(MyHashMap * const map, void * const key) {
	int hasCode = (*(map->hashCode))(key);
	hasCode %= map->initialCapacity;
	if (hasCode < 0)
		hasCode += map->initialCapacity;
	Entry * re = listContainsEntry(map->entryList[hasCode], key, map->equal);
	return re != NULL;
}
 
//增加一条映射
void myHashMapPutData(MyHashMap * const map, void * const key,
		void * const value) {
	int hasCode = (*(map->hashCode))(key);
	hasCode %= map->initialCapacity;
	if (hasCode < 0)
		hasCode += map->initialCapacity;
	Entry * re = listContainsEntry(map->entryList[hasCode], key, map->equal);
	if (re == NULL) {
		Entry * entry = (Entry*) malloc(sizeof(Entry));
		entry->key = key;
		entry->value = value;
		myListInsertDataAtLast(map->entryList[hasCode], entry);
		(map->size)++;
		if (map->size > map->initialCapacity * map->loadFactor) {
			rebuildMyHashMap(map);
		}
	} else {
		re->value = value;
	}
}
 
//通过key得到数据，如果没有数据则返回null
void* myHashMapGetDataByKey(MyHashMap * const map, void * const key) {
	int hasCode = (*(map->hashCode))(key);
	hasCode %= map->initialCapacity;
	if (hasCode < 0)
		hasCode += map->initialCapacity;
	Entry * re = listContainsEntry(map->entryList[hasCode], key, map->equal);
	if (re == NULL) {
		return NULL;
	}
	return re->value;
}
 
//数据的容量
int myHashMapGetSize(const MyHashMap * const map) {
	return map->size;
}
 
//创建Entry迭代器
MyHashMapEntryIterator* createMyHashMapEntryIterator(MyHashMap * const map) {
	MyHashMapEntryIterator* re = (MyHashMapEntryIterator*) malloc(
			sizeof(MyHashMapEntryIterator));
	re->count = 0;
	re->index = 0;
	re->map = map;
	re->current = map->entryList[0]->first;
	return re;
}
 
//释放Entry迭代器
void freeMyHashMapEntryIterator(MyHashMapEntryIterator* iterator) {
	free(iterator);
}
 
//Entry迭代器是否有下一个
int myHashMapEntryIteratorHasNext(MyHashMapEntryIterator* iterator) {
	return iterator->count < iterator->map->size;
}
 
//遍历下一个Entry元素
Entry* myHashMapEntryIteratorNext(MyHashMapEntryIterator* iterator) {
	(iterator->count)++;
	while (!(iterator->current)) {
		(iterator->index)++;
		iterator->current = iterator->map->entryList[iterator->index]->first;
	}
	Entry * re = (Entry *) iterator->current->data;
	iterator->current = iterator->current->next;
	return re;
}
 
//删除一条数据，返回是否删除成功
int myHashMapRemoveDataByKey(MyHashMap * const map, void * const key) {
	int hasCode = (*(map->hashCode))(key);
	hasCode %= map->initialCapacity;
	if (hasCode < 0)
		hasCode += map->initialCapacity;
	MyListIterator* it = createMyListIterator(map->entryList[hasCode]);
	int re = 0;
	while (myListIteratorHasNext(it)) {
		Entry * entry = (Entry *) (myListIteratorNext(it));
		if ((*(map->equal))(entry->key, key)) {
			myListRemoveDataAt(map->entryList[hasCode], it->count - 1);
			re = 1;
			(map->size)--;
			break;
		}
	}
	freeMyListIterator(it);
	return re;
}
 
void myFree(Entry * p){
    free(p);
}
 
//释放HashMap
void freeMyHashMap(MyHashMap * map) {
	myHashMapOutput(map, myFree);
	for (int i = 0; i < map->initialCapacity; i++) {
		freeMyList(map->entryList[i]);
	}
	free(map->entryList);
	free(map);
}
 
//遍历
void myHashMapOutput(MyHashMap *map, void(*pt)(Entry*)) {
	MyHashMapEntryIterator* iterator = createMyHashMapEntryIterator(map);
	while (myHashMapEntryIteratorHasNext(iterator)) {
		pt(myHashMapEntryIteratorNext(iterator));
	}
	freeMyHashMapEntryIterator(iterator);
}



/*************************
*** File main.c
*** test for MyHashMap
**************************/
#include <stdio.h>
#include <stdlib.h>
//#include "myEqual.h"
//#include "myHashCode.h"
//#include "myHashMap.h"
 
 
int test_hashmap()
{  
  char* strs[]=
  {
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
    int*  data = (int*)malloc(sizeof(int)* S);
    for (int i=0; i<S; i++)
    {
        data[i]=i;
    }
 
    //创建映射需要指定两个函数，hashCode函数和equal函数。
    MyHashMap * map = createMyHashMap(myHashCodeString, myEqualString);
 
    //插入数据
    for (int i=0; i<S; i++)
    {
        myHashMapPutData(map, strs[i], &data[i]);
    }
 
    //输出大小
    printf("size=%d\n",myHashMapGetSize(map));
 
    //测试删除
    myHashMapRemoveDataByKey(map,"qq");
    myHashMapRemoveDataByKey(map,"ab");
    myHashMapRemoveDataByKey(map,"qwert");
 
    //输出大小
    printf("after remove size=%d\n",myHashMapGetSize(map));
 
    //遍历
    MyHashMapEntryIterator * it = createMyHashMapEntryIterator(map);
    while(myHashMapEntryIteratorHasNext(it))
    {
        Entry * pp= myHashMapEntryIteratorNext(it);
        char * key = (char*)pp->key;
        int* value = (int*)pp->value;
        printf("%s(%d)\n", key, *value);
    }
    //释放遍历器
    freeMyHashMapEntryIterator(it);
 
    //释放映射
    freeMyHashMap(map);
 
    //释放数据
    free(data);
    return 0;
}

