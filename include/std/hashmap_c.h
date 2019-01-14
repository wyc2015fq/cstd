// HashMap
//这是在通用链表的基础上实现的映射，关于链表的实现参见：http://blog.csdn.net/swwlqw/article/details/22498833。
//注意映射中只存储了key和value的指针，没有储存实际的数据。
//对于新的key类型来说，需要自定义HashCode函数和equal函数。
//在HashSet的实现中给出了几个常见的hashCode函数和equal函数。参见：http://blog.csdn.net/swwlqw/article/details/22664129。

#include "inttypes_c.h"
#include "algo.h"
#include "list.h"
#include "hashcode_c.h"
#include "mempool_c.h"

typedef struct kvnode {
  size_t hash;
  void* key;
  void* value;
  struct kvnode* next;
} kvnode;

typedef int(*cmp_fun_t)(const void* key1, const void* key2);
typedef size_t (*hash_fun_t)(void* key);
typedef void (*setkv_fun_t)(kvnode* kv, void* key, void* value);
typedef void (*delkv_fun_t)(kvnode* kv);

typedef struct hashmap_t {
  hash_fun_t hash;
  cmp_fun_t cmp;
  setkv_fun_t setkv;
  delkv_fun_t delkv;
  size_t count;         //大小
  size_t bucket_size;    //初始容量
  kvnode** bucket;
} hashmap_t;

//某条kvnode链表上是否包含某个key值。
kvnode** list_find(kvnode** it, size_t h, const void* key, cmp_fun_t cmp)
{
  for (; *it; it = &((*it)->next)) {
    if ((*it)->key == key || (h == (*it)->hash && 0 == cmp((*it)->key, key))) {
      return it;
    }
  }
  return it;
}
void list_free(kvnode** it)
{
  kvnode* p = *it;
  *it = 0;
  for (; p; ) {
    kvnode* next = p->next;
    free(p);
    p = next;
  }
}

void hashmap_resize(hashmap_t* map)
{
  static const float DEFAULT_LOAD_FACTOR = 0.75f;
  if (map->count == 0 || map->count > map->bucket_size * DEFAULT_LOAD_FACTOR) {
    static const size_t DEFAULT_INITIAL_CAPACITY = 1 << 4; // aka 16
    static const size_t MAXIMUM_CAPACITY = 1 << 30;
    size_t new_bucket_size = map->bucket_size * 2;
    //new_bucket_size = MAX(new_bucket_size, aa);
    new_bucket_size = MAX(new_bucket_size, DEFAULT_INITIAL_CAPACITY);
    new_bucket_size = MIN(new_bucket_size, MAXIMUM_CAPACITY);
    kvnode** new_bucket = (kvnode**) malloc(sizeof(kvnode*) * new_bucket_size);
    memset(new_bucket, 0, (sizeof(kvnode*) * new_bucket_size));
    for (size_t i = 0; i < map->bucket_size; ++i) {
      kvnode* it = map->bucket[i];
      for (; it; ) {
        size_t hasCode = it->hash % new_bucket_size;
        kvnode* next = it->next;
        it->next = NULL;
        LIST_ADDFRONT1(new_bucket[hasCode], it);
        it = next;
      }
    }
    if (map->bucket) {
      free(map->bucket);
    }
    map->bucket = new_bucket;
    map->bucket_size = new_bucket_size;
  }
}

//增加一条映射
kvnode** hashmap_put_or_get(hashmap_t* map, void* key, bool put)
{
  size_t h = map->hash ? map->hash(key) : myHashCodeDefault(key);
  hashmap_resize(map);
  size_t hasCode = h % map->bucket_size;
  kvnode** first = map->bucket + hasCode;
  kvnode** re = list_find(first, h, key, map->cmp);
  if ((*re) == NULL && put) {
    kvnode* node = (kvnode*) malloc(sizeof(kvnode));
    memset(node, 0, sizeof(*node));
    node->hash = h;
    LIST_ADDFRONT1((*first), node);
    map->count++;
    re = first;
  }
  return re;
}

//是否包含某个key
kvnode** hashmap_find(hashmap_t* map, void* key)
{
  kvnode** re = hashmap_put_or_get(map, key, false);
  return re;
}

//通过key得到数据，如果没有数据则返回null
void* hashmap_get(hashmap_t* map, void* key)
{
  kvnode** re = hashmap_put_or_get(map, key, false);
  if (*re) {
    return (*re)->value;
  }
  return NULL;
}
void* hashmap_put(hashmap_t* map, void* key, void* value)
{
  kvnode** re = hashmap_put_or_get(map, key, true);
  void* old_value = (*re)->value;
  if (map->setkv) {
    map->setkv(*re, key, value);
  } else {
    (*re)->key = key;
    (*re)->key = value;
  }
  return old_value;
}

//删除一条数据，返回是否删除成功
void hashmap_del(hashmap_t* map, void* key)
{
  kvnode** re = hashmap_put_or_get(map, key, false);
  if (*re) {
    kvnode* p = *re;
    if (map->delkv) {
      map->delkv(p);
    }
    LIST_DEL1(*re);
    map->count--;
    free(p);
  }
}

//释放HashMap
void hashmap_free(hashmap_t* map)
{
  for (size_t i = 0; i < map->bucket_size; i++) {
    list_free(map->bucket + i);
  }
  free(map->bucket);
  memset(map, 0, sizeof(*map));
}

//遍历
void hashmap_foreach(hashmap_t* map, void(*pt)(kvnode*))
{
  for (size_t i = 0; i < map->bucket_size; ++i) {
    kvnode* it = map->bucket[i];
    for (; it; it = it->next) {
      pt(it);
    }
  }
}

void mySetKV(kvnode* kv, void* key, void* value)
{
  if (key && kv->key == NULL) {
    kv->key = strdup((char*)key);
  }
  kv->value = value;
}

void myDelKV(kvnode* kv)
{
  free(kv->key);
}

void myprintKV(kvnode* kv)
{
  printf("%s(%d)\n", (char*)kv->key, (int)(kv->value));
}

#if 1


/*************************
*** File main.c
*** test for hashmap_t
**************************/
#include <stdio.h>
#include <stdlib.h>
//#include "myEqual.h"
#include "inttypes_c.h"
#include "hashcode_c.h"
#include "utime.h"
#include <map>
#include <string>
//#include "hashmap_t.h"

int test_hashmap()
{
  if (0) {
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
    size_t S = countof(strs);
    int*  data = (int*)malloc(sizeof(int) * S);
    for (int i = 0; i < S; i++) {
      data[i] = i;
    }
    //创建映射需要指定两个函数，hashCode函数和equal函数。
    hashmap_t map[1] = {{myHashCodeString, myCmpString, mySetKV, myDelKV, 0}};
    //插入数据
    for (int i = 0; i < S; i++) {
      hashmap_put(map, strs[i], (void*)i);
    }
    //输出大小
    printf("size=%d\n", map->count);
    //测试删除
    hashmap_del(map, "qq");
    hashmap_del(map, "ab");
    hashmap_del(map, "qwert");
    //输出大小
    printf("after remove size=%d\n", map->count);
    //遍历
    hashmap_foreach(map, myprintKV);
    //释放遍历器
    //释放映射
    hashmap_free(map);
    //释放数据
    free(data);
  }
  int n = 100000;
  if (0) {
    //创建映射需要指定两个函数，hashCode函数和equal函数。
    //using namespace std;
    std::map<std::string, int> map2;
    char buf[100] = {0};
    int k = 3;
    {
      utime_start(_start_time);
      for (int i = 0; i < n; i++) {
        _snprintf(buf, 100, "%d", i);
        map2[buf] = i;
        //printf("%d\n", i);
      }
      printf("after remove count=%d\n", map2.size());
      for (int i = 0; i < n; i++) {
        _snprintf(buf, 100, "%d", i);
        map2.erase(buf);
      }
      printf("%lf\n", utime_elapsed(_start_time));
    };
    printf("after remove count=%d\n", map2.size());
  }
  if (1) {
    //创建映射需要指定两个函数，hashCode函数和equal函数。
    hashmap_t map[1] = {{myHashCodeString, myCmpString, mySetKV, myDelKV, 0}};
    char buf[100] = {0};
    int k = 3;
    {
      utime_start(_start_time);
      for (int i = 0; i < n; i++) {
        _snprintf(buf, 100, "%d", i);
        hashmap_put(map, buf, (void*)i);
        //printf("%d\n", i);
      }
      printf("after remove count=%d\n", map->count);
      for (int i = 0; i < n; i++) {
        _snprintf(buf, 100, "%d", i);
        hashmap_del(map, buf);
      }
      printf("%lf\n", utime_elapsed(_start_time));
    };
    printf("after remove count=%d\n", map->count);
    hashmap_free(map);
  }
  return 0;
}


#endif