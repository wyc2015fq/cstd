# 哈希表（散列表）—Hash表解决地址冲突 C语言实现 - 在思索中前行！ - CSDN博客





2014年10月17日 14:13:43[_Tham](https://me.csdn.net/txl16211)阅读数：2776








哈希表（Hash table，也叫散列表），是根据关键码值(Key value)而直接进行访问的数据结构。也就是说，它通过把关键码值映射到表中一个位置来访问记录，以加快查找的速度。具体的介绍网上有很详细的描述，如[闲聊哈希表](http://www.tektalk.org/2010/03/02/%E9%97%B2%E8%81%8A%E5%93%88%E5%B8%8C%E8%A1%A8-%EF%BC%88%E4%B8%8A%EF%BC%89/) ，这里就不再累述了；

哈希表在像Java、C#等语言中是与生俱来的。可是在C的世界中，似乎只有自己动手，丰衣足食；在网上google了一把，大致有几个版本，我会一一来分析对比；

首先先来交代一下哈希表实现中需要注意的一些概念：

[（主要参考：这里）](http://blog.csdn.net/dreamcs/article/details/5764444)

## **1、哈希函数**




也叫散列函数，即：根据key,计算出key对应记录的储存位置

position = f(key)


散列函数满足以下的条件：


1、对输入值运算，得到一个固定长度的摘要(Hash value)；


2、不同的输入值可能对应同样的输出值；


以下的函数都可以认为是一个散列函数：


f(x) = x mod 16; (1)


f(x) = (x2 + 10) * x; (2)


f(x) = (x | 0×0000FFFF) XOR (x >> 16); (3)


不过，仅仅满足上面这两条的函数，作为散列函数，还有不足的地方。我们还希望散列函数满足下面几点：


1、散列函数的输出值尽量接近均匀分布；


2、x的微小变化可以使f(x)发生非常大的变化，即所谓“雪崩效应”([Avalanche effect](http://en.wikipedia.org/wiki/Avalanche_effect))；


上面两点用数学语言表示，就是：


1， 输出值y的分布函数F(y)=y/m, m为散列函数的最大值。或记为y~U[0, m]


2，|df(x)/dx| >> 1；


从上面两点，大家看看，前面举例的三个散列函数，哪个更好呢？对了，是第三个：


f(x) = (x | 0×0000FFFF) XOR (x >> 16);


它很完美地满足“好的散列函数”的两个附加条件。

**2、哈希冲突（Hash collision）**


也就是两个不同输入产生了相同输出值的情况。首先，哈希冲突是无法避免的，因此，哈希算法的选择直接决定了哈希冲突发送的概率；同时必须要对哈希冲突进行处理，方法主要有以下几种：


1， 链地址法


链地址法：对Hash表中每个Hash值建立一个冲突表，即将冲突的几个记录以表的形式存储在其中


2， 开放地址法


下面就来看看每种方法的具体实现吧：


链地址法：

举例说明： 设有 8 个元素 { a,b,c,d,e,f,g,h } ，采用某种哈希函数得到的地址分别为： {0 ， 2 ， 4 ， 1 ， 0 ， 8 ， 7 ， 2} ，当哈希表长度为 10 时，采用链地址法解决冲突的哈希表如下图所示。 

![](http://pic002.cnblogs.com/images/2012/34831/2012011309101439.jpg)

## 链地址法实现Hash冲突优化


```cpp
#include "stdafx.h"
#include <string.h>
#include <stdio.h>
#include <stdlib.h>

typedef struct _node{
    char *name;
    char *desc;
    struct _node *next;
}node;

#define HASHSIZE 101
static node* hashtab[HASHSIZE];

void inithashtab(){
    int i;
    for(i=0;i<HASHSIZE;i++)
        hashtab[i]=NULL;
}

unsigned int hash(char *s){
    unsigned int h=0;
    for(;*s;s++)
        h=*s+h*31;
    return h%HASHSIZE;
}

node* lookup(char *n){
    unsigned int hi=hash(n);
    node* np=hashtab[hi];
    for(;np!=NULL;np=np->next){
        if(!strcmp(np->name,n))
            return np;
    }
    
    return NULL;
}

char* m_strdup(char *o){
    int l=strlen(o)+1;
    char *ns=(char*)malloc(l*sizeof(char));
    strcpy(ns,o);
    if(ns==NULL)
        return NULL;
    else
        return ns;
}

char* get(char* name){
    node* n=lookup(name);
    if(n==NULL)
        return NULL;
    else
        return n->desc;
}

int install(char* name,char* desc){
    unsigned int hi;
    node* np;
    if((np=lookup(name))==NULL){
        hi=hash(name);
        np=(node*)malloc(sizeof(node));
        if(np==NULL)
            return 0;
        np->name=m_strdup(name);
        if(np->name==NULL) return 0;
        np->next=hashtab[hi];
        hashtab[hi]=np;
    }
    else
        free(np->desc);
    np->desc=m_strdup(desc);
    if(np->desc==NULL) return 0;
    
    return 1;
}

/* A pretty useless but good debugging function,
which simply displays the hashtable in (key.value) pairs
*/
void displaytable(){
    int i;
    node *t;
    for(i=0;i<HASHSIZE;i++){
        if(hashtab[i]==NULL)
            printf("()");
        else{
            t=hashtab[i];
            printf("(");
            for(;t!=NULL;t=t->next)
                printf("(%s.%s) ",t->name,t->desc);
            printf(".)");
        }
    }
}

void cleanup(){
    int i;
    node *np,*t;
    for(i=0;i<HASHSIZE;i++){
        if(hashtab[i]!=NULL){
            np=hashtab[i];
            while(np!=NULL){
                t=np->next;
                free(np->name);
                free(np->desc);
                free(np);
                np=t;
            }
        }
    }
}

main(){
    int i;
    char* names[]={"name","address","phone","k101","k110"};
    char* descs[]={"Sourav","Sinagor","26300788","Value1","Value2"};
    
    inithashtab();
    for(i=0;i<5;i++)
        install(names[i],descs[i]);
    
    printf("Done");
    printf("If we didnt do anything wrong..""we should see %s",get("k110"));
    
    install("phone","9433120451");
    
    printf("Again if we go right, we have %s and %s",get("k101"),get("phone"));
    
    /*displaytable();*/
    cleanup();
    return 0;
}
```

## Hash方法2

第2种解决哈希冲突的做法，即为每个Hash值，建立一个Hash桶(Bucket)，桶的容量是固定的，也就是只能处理固定次数的冲突，如1048576个Hash桶，每个桶中有4个表项(Entry)，总计4M个表项。其实这两种的实现思路雷同，就是对Hash表中每个Hash值建立一个冲突表，即将冲突的几个记录以表的形式存储在其中；

废话不多说，上代码和图示基本能说明清楚：

完整的代码，请看：[这里](http://pokristensson.com/strmap.html)，一位圣安德鲁斯大学的讲师：[KRISTENSSON](http://pokristensson.com/index.html)博客

这里截取几个主要的片段：

主要的数据结构：



```cpp
struct Pair {
    char *key;
    char *value;
};

struct Bucket {
    unsigned int count;
    Pair *pairs;
};

struct StrMap {
    unsigned int count;
    Bucket *buckets;
};
```





主要的函数：


put：

```cpp
int sm_put(StrMap *map, const char *key, const char *value)
{
    unsigned int key_len, value_len, index;
    Bucket *bucket;
    Pair *tmp_pairs, *pair;
    char *tmp_value;
    char *new_key, *new_value;

    if (map == NULL) {
        return 0;
    }
    if (key == NULL || value == NULL) {
        return 0;
    }
    key_len = strlen(key);
    value_len = strlen(value);
    /* Get a pointer to the bucket the key string hashes to */
    index = hash(key) % map->count;
    bucket = &(map->buckets[index]);
    /* Check if we can handle insertion by simply replacing
     * an existing value in a key-value pair in the bucket.
     */
    if ((pair = get_pair(bucket, key)) != NULL) {
        /* The bucket contains a pair that matches the provided key,
         * change the value for that pair to the new value.
         */
        if (strlen(pair->value) < value_len) {
            /* If the new value is larger than the old value, re-allocate
             * space for the new larger value.
             */
            tmp_value = realloc(pair->value, (value_len + 1) * sizeof(char));
            if (tmp_value == NULL) {
                return 0;
            }
            pair->value = tmp_value;
        }
        /* Copy the new value into the pair that matches the key */
        strcpy(pair->value, value);
        return 1;
    }
    /* Allocate space for a new key and value */
    new_key = malloc((key_len + 1) * sizeof(char));
    if (new_key == NULL) {
        return 0;
    }
    new_value = malloc((value_len + 1) * sizeof(char));
    if (new_value == NULL) {
        free(new_key);
        return 0;
    }
    /* Create a key-value pair */
    if (bucket->count == 0) {
        /* The bucket is empty, lazily allocate space for a single
         * key-value pair.
         */
        bucket->pairs = malloc(sizeof(Pair));
        if (bucket->pairs == NULL) {
            free(new_key);
            free(new_value);
            return 0;
        }
        bucket->count = 1;
    }
    else {
        /* The bucket wasn't empty but no pair existed that matches the provided
         * key, so create a new key-value pair.
         */
        tmp_pairs = realloc(bucket->pairs, (bucket->count + 1) * sizeof(Pair));
        if (tmp_pairs == NULL) {
            free(new_key);
            free(new_value);
            return 0;
        }
        bucket->pairs = tmp_pairs;
        bucket->count++;
    }
    /* Get the last pair in the chain for the bucket */
    pair = &(bucket->pairs[bucket->count - 1]);
    pair->key = new_key;
    pair->value = new_value;
    /* Copy the key and its value into the key-value pair */
    strcpy(pair->key, key);
    strcpy(pair->value, value);
    return 1;
}
```

get：



```cpp
int sm_get(const StrMap *map, const char *key, char *out_buf, unsigned int n_out_buf)
{
    unsigned int index;
    Bucket *bucket;
    Pair *pair;

    if (map == NULL) {
        return 0;
    }
    if (key == NULL) {
        return 0;
    }
    index = hash(key) % map->count;
    bucket = &(map->buckets[index]);
    pair = get_pair(bucket, key);
    if (pair == NULL) {
        return 0;
    }
    if (out_buf == NULL && n_out_buf == 0) {
        return strlen(pair->value) + 1;
    }
    if (out_buf == NULL) {
        return 0;
    }
    if (strlen(pair->value) >= n_out_buf) {
        return 0;
    }
    strcpy(out_buf, pair->value);
    return 1;
}
```

哈希函数：





```cpp
/*
 * Returns a hash code for the provided string.
 */
static unsigned long hash(const char *str)
{
    unsigned long hash = 5381;
    int c;

    while (c = *str++) {
        hash = ((hash << 5) + hash) + c;
    }
    return hash;
}
```




大致的思路是这样的：

![](http://pic002.cnblogs.com/images/2012/34831/2012011610493340.png)



首先哈希桶的个数是固定的，有用户构建的时候输入，一旦构建，个数就已经固定；查找的时候首先将key值通过哈希函数获取哈希值，根据哈希值获取到对应的哈希桶，然后遍历哈希桶内的pairs数组获取；



这两种实现方法看似比较类似，但也有差异：


基于哈希桶的情况下，由于Hash桶容量的限制，所以，有可能发生Hash表填不满的情况，也就是，虽然Hash表里面还有空位，但是新建的表项由于冲突过多，而不能装入Hash表中。不过，这样的实现也有其好处，就是查表的最大开销是可以确定的，因为最多处理的冲突数是确定的，所以算法的时间复杂度为O(1)+O(m)，其中m为Hash桶容量。


而另一种通过链表的实现，由于Hash桶的容量是无限的，因此，只要没有超出Hash表的最大容量，就能够容纳新建的表项。但是，一旦发生了Hash冲突严重的情况，就会造成Hash桶的链表过长，大大降低查找效率。在最坏的情况下，时间复杂度退化为O(n)，其中n为Hash表的总容量。当然，这种情况的概率小之又小，几乎是可以忽略的。

## **[哈希表的C实现（三）](http://www.cnblogs.com/xiekeli/archive/2012/01/17/2324433.html)**

## 网上流传最具传奇色彩的莫过于暴雪公司的魔兽文件打包管理器里的hashTable的实现了；在冲突方面的处理方面，采用线性探测再散列。在添加和查找过程中进行了三次哈希，第一个哈希值用来查找，后两个哈希值用来校验，这样可以大大减少冲突的几率。

## 在网上找了相关代码，但不知道其来源是否地道：

## StringHash.h


```cpp
#include <StdAfx.h>
#include <string>

using namespace std;

#pragma once

#define MAXTABLELEN 1024    // 默认哈希索引表大小 
//////////////////////////////////////////////////////////////////////////  
// 哈希索引表定义  
typedef struct  _HASHTABLE
{  
    long nHashA;  
    long nHashB;  
    bool bExists;  
}HASHTABLE, *PHASHTABLE ;  

class StringHash
{
public:
    StringHash(const long nTableLength = MAXTABLELEN);
    ~StringHash(void);
private:  
    unsigned long cryptTable[0x500];  
    unsigned long m_tablelength;    // 哈希索引表长度  
    HASHTABLE *m_HashIndexTable; 
private:
    void InitCryptTable();                                               // 对哈希索引表预处理 
    unsigned long HashString(const string &lpszString, unsigned long dwHashType); // 求取哈希值      
public:
    bool Hash(string url);
    unsigned long Hashed(string url);    // 检测url是否被hash过
};
```
StringHash.cpp


```cpp
#include "StdAfx.h"
#include "StringHash.h"

StringHash::StringHash(const long nTableLength /*= MAXTABLELEN*/)
{
    InitCryptTable();  
    m_tablelength = nTableLength;  
    //初始化hash表
    m_HashIndexTable = new HASHTABLE[nTableLength];  
    for ( int i = 0; i < nTableLength; i++ )  
    {  
        m_HashIndexTable[i].nHashA = -1;  
        m_HashIndexTable[i].nHashB = -1;  
        m_HashIndexTable[i].bExists = false;  
    }          
}

StringHash::~StringHash(void)
{
    //清理内存
    if ( NULL != m_HashIndexTable )  
    {  
        delete []m_HashIndexTable;  
        m_HashIndexTable = NULL;  
        m_tablelength = 0;  
    }  
}

/************************************************************************/
/*函数名：InitCryptTable
/*功  能：对哈希索引表预处理  
/*返回值：无
/************************************************************************/
void StringHash::InitCryptTable()  
{   
    unsigned long seed = 0x00100001, index1 = 0, index2 = 0, i;  

    for( index1 = 0; index1 < 0x100; index1++ )  
    {   
        for( index2 = index1, i = 0; i < 5; i++, index2 += 0x100 )  
        {   
            unsigned long temp1, temp2;  
            seed = (seed * 125 + 3) % 0x2AAAAB;  
            temp1 = (seed & 0xFFFF) << 0x10;  
            seed = (seed * 125 + 3) % 0x2AAAAB;  
            temp2 = (seed & 0xFFFF);  
            cryptTable[index2] = ( temp1 | temp2 );   
        }   
    }   
}  

/************************************************************************/
/*函数名：HashString
/*功  能：求取哈希值   
/*返回值：返回hash值
/************************************************************************/
unsigned long StringHash::HashString(const string& lpszString, unsigned long dwHashType)  
{   
    unsigned char *key = (unsigned char *)(const_cast<char*>(lpszString.c_str()));  
    unsigned long seed1 = 0x7FED7FED, seed2 = 0xEEEEEEEE;  
    int ch;  

    while(*key != 0)  
    {   
        ch = toupper(*key++);  

        seed1 = cryptTable[(dwHashType << 8) + ch] ^ (seed1 + seed2);  
        seed2 = ch + seed1 + seed2 + (seed2 << 5) + 3;   
    }  
    return seed1;   
}  

/************************************************************************/
/*函数名：Hashed
/*功  能：检测一个字符串是否被hash过
/*返回值：如果存在，返回位置；否则，返回-1
/************************************************************************/
unsigned long StringHash::Hashed(string lpszString)  

{   
    const unsigned long HASH_OFFSET = 0, HASH_A = 1, HASH_B = 2;  
    //不同的字符串三次hash还会碰撞的几率无限接近于不可能
    unsigned long nHash = HashString(lpszString, HASH_OFFSET);  
    unsigned long nHashA = HashString(lpszString, HASH_A);  
    unsigned long nHashB = HashString(lpszString, HASH_B);  
    unsigned long nHashStart = nHash % m_tablelength,  
    nHashPos = nHashStart;  

    while ( m_HashIndexTable[nHashPos].bExists)  
    {   
        if (m_HashIndexTable[nHashPos].nHashA == nHashA && m_HashIndexTable[nHashPos].nHashB == nHashB)   
            return nHashPos;   
        else   
            nHashPos = (nHashPos + 1) % m_tablelength;  

        if (nHashPos == nHashStart)   
            break;   
    }  

    return -1; //没有找到  
}  

/************************************************************************/
/*函数名：Hash
/*功  能：hash一个字符串 
/*返回值：成功，返回true；失败，返回false
/************************************************************************/
bool StringHash::Hash(string lpszString)
{  
    const unsigned long HASH_OFFSET = 0, HASH_A = 1, HASH_B = 2;  
    unsigned long nHash = HashString(lpszString, HASH_OFFSET);  
    unsigned long nHashA = HashString(lpszString, HASH_A);  
    unsigned long nHashB = HashString(lpszString, HASH_B);  
    unsigned long nHashStart = nHash % m_tablelength, 
        nHashPos = nHashStart;  

    while ( m_HashIndexTable[nHashPos].bExists)  
    {   
        nHashPos = (nHashPos + 1) % m_tablelength;  
        if (nHashPos == nHashStart) //一个轮回  
        {  
            //hash表中没有空余的位置了,无法完成hash
            return false;   
        }  
    }  
    m_HashIndexTable[nHashPos].bExists = true;  
    m_HashIndexTable[nHashPos].nHashA = nHashA;  
    m_HashIndexTable[nHashPos].nHashB = nHashB;  

    return true;  
}
```



关于其中的实现原理，我觉得没有比 [inside MPQ](http://sfsrealm.hopto.org/inside_mopaq/chapter2.htm)说得清楚的了，于是用我蹩脚的E文，将该文的第二节翻译了一遍（将原文和译文都贴出来，请高手指正）：


原理

Most of the advancements throughout the history of computers have been because of particular problems which required solving. In this chapter, we'll take a look at some of these problems and their solutions as they pertain to the MPQ format.

贯穿计算机发展历史，大多数进步都是源于某些问题的解决，在这一节中，我们来看一看与MPQ 格式相关问题及解决方案；

Hashes 哈希表

Problem: You have a very large array of strings. You have another string and need to know if it is already in the list. You would probably begin by comparing each string in the list with the string other, but when put into application, you would find that this method is far too slow for practical use. Something else must be done. But how can you know if the string exists without comparing it to all the other strings?

问题：你有一个很大的字符串数组，同时，你另外还有一个字符串，需要知道这个字符串是否已经存在于字符串数组中。你可能会对数组中的每一个字符串进行比较，但是在实际项目中，你会发现这种做法对某些特殊应用来说太慢了。必须寻求其他途径。那么如何才能在不作遍历比较的情况下知道这个字符串是否存在于数组中呢？

Solution: Hashes. Hashes are smaller data types (i.e. numbers) that represent other, larger, data types (usually strings). In this scenario, you could store hashes in the array with the strings. Then you could compute the hash of the other string and compare it to the stored hashes. If a hash in the array matches the new hash, the strings can be compared to verify the match. This method, called indexing, could speed things up by about 100 times, depending on the size of the array and the average length of the strings.

解决方案：哈希表。哈希表是通过更小的数据类型表示其他更大的数据类型。在这种情况下，你可以把哈希表存储在字符串数组中，然后你可以计算字符串的哈希值，然后与已经存储的字符串的哈希值进行比较。如果有匹配的哈希值，就可以通过字符串比较进行匹配验证。这种方法叫索引，根据数组的大小以及字符串的平均长度可以约100倍。


```cpp
unsigned long HashString(char *lpszString)
{   
    unsigned long ulHash = 0xf1e2d3c4;        
    while (*lpszString != 0)    
    {        
        ulHash <<= 1;       
        ulHash += *lpszString++;      
    }   
    return ulHash;
}
```


The previous code function demonstrates a very simple hashing algorithm. The function sums the characters in the string, shifting the hash value left one bit before each character is added in. Using this algorithm, the string "arr\units.dat" would hash to 0x5A858026, and "unit\neutral\acritter.grp" would hash to 0x694CD020. Now, this is, admittedly, a very simple algorithm, and it isn't very useful, because it would generate a relatively predictable output, and a lot of collisions in the lower range of numbers. Collisions are what happen when more than one string hash to the same value.

上面代码中的函数演示了一种非常简单的散列算法。这个函数在遍历字符串过程中，将哈希值左移一位，然后加上字符值；通过这个算法，字符串"arr\units.dat" 的哈希值是0x5A858026，字符串"unit\neutral\acritter.grp" 的哈希值是0x694CD020；现在，众所周知的，这是一个基本没有什么实用价值的简单算法，因为它会在较低的数据范围内产生相对可预测的输出，从而可能会产生大量冲突（不同的字符串产生相同的哈希值）。


The MPQ format, on the other hand, uses a very complicated hash algorithm (shown below) to generate totally unpredictable hash values. In fact, the hashing algorithm is so effective that it is called a one-way hash. A one-way hash is a an algorithm that is constructed in such a way that deriving the original string (set of strings, actually) is virtually impossible. Using this particular algorithm, the filename "arr\units.dat" would hash to 0xF4E6C69D, and "unit\neutral\acritter.grp" would hash to 0xA26067F3.

MPQ格式，使用了一种非常复杂的散列算法（如下所示），产生完全不可预测的哈希值，这个算法十分有效，这就是所谓的单向散列算法。通过单向散列算法几乎不可能通过哈希值来唯一的确定输入值。使用这种算法，文件名 "arr\units.dat" 的哈希值是0xF4E6C69D，"unit\neutral\acritter.grp" 的哈希值是 0xA26067F3。

```cpp
unsigned long HashString(char *lpszFileName, unsigned long dwHashType)
{   
    unsigned char *key = (unsigned char *)lpszFileName;   
    unsigned long seed1 = 0x7FED7FED, seed2 = 0xEEEEEEEE;   
    int ch;

    while(*key != 0)       
    {      
        ch = toupper(*key++);   
        seed1 = cryptTable[(dwHashType << 8) + ch] ^ (seed1 + seed2);       
        seed2 = ch + seed1 + seed2 + (seed2 << 5) + 3;       
    }   
    return seed1;  
}
```



Hash Tables 哈希表


Problem: You tried using an index like in the previous sample, but your program absolutely demands break-neck speeds, and indexing just isn't fast enough. About the only thing you could do to make it faster is to not check all of the hashes in the array. Or, even better, if you could only make one comparison in order to be sure the string doesn't exist anywhere in the array. Sound too good to be true? It's not.

问题：您尝试在前面的示例中使用相同索引，您的程序一定会有中断现象发生，而且不够快 。如果想让它更快，您能做的只有让程序不去查询数组中的所有散列值。或者 您可以只做一次对比就可以得出在列表中是否存在字符串。 听起来不错，真的么？  不可能的啦


Solution: A hash table. A hash table is a special type of array in which the offset of the desired string is the hash of that string. What I mean is this. Say that you make that string array use a separate array of fixed size (let's say 1024 entries, to make it an even power of 2) for the hash table. You want to see if the new string is in that table. To get the string's place in the hash table, you compute the hash of that string, then modulo (division remainder) that hash value by the size of that table. Thus, if you used the simple hash algorithm in the previous section, "arr\units.dat" would hash to 0x5A858026, making its offset 0x26 (0x5A858026 divided by 0x400 is 0x16A160, with a remainder of 0x26). The string at this location (if there was one) would then be compared to the string to add. If the string at 0x26 doesn't match or just plain doesn't exist, then the string to add doesn't exist in the array. The following code illustrates this:

解决：一个哈希表就是以字符串的哈希值作为下标的一类数组。我的意思是，哈希表使用一个固定长度的字符串数组（比如1024，2的偶次幂）进行存储；当你要看看这个字符串是否存在于哈希表中，为了获取这个字符串在哈希表中的位置，你首先计算字符串的哈希值，然后哈希表的长度取模。这样如果你像上一节那样使用简单的哈希算法，字符串"arr\units.dat" 的哈希值是0x5A858026,偏移量0x26（0x5A858026 除于0x400等于0x16A160，模0x400等于0x26）。因此，这个位置的字符串将与新加入的字符串进行比较。如果0X26处的字符串不匹配或不存在，那么表示新增的字符串在数组中不存在。下面是示意的代码：

```cpp
int GetHashTablePos(char *lpszString, SOMESTRUCTURE *lpTable, int nTableSize)
{   
    int nHash = HashString(lpszString), nHashPos = nHash % nTableSize;       
    if (lpTable[nHashPos].bExists && !strcmp(lpTable[nHashPos].pString, lpszString))       
        return nHashPos;   
    else        
        return -1; //Error value   
}
```


Now, there is one glaring flaw in that explanation. What do you think happens when a collision occurs (two different strings hash to the same value)? Obviously, they can't occupy the same entry in the hash table. Normally, this is solved by each entry in the hash table being a pointer to a linked list, and the linked list would hold all the entries that hash to that same value.

上面的说明中存在一个刺眼的缺陷。当有冲突（两个不同的字符串有相同的哈希值）发生的时候怎么办？显而易见的，它们不能占据哈希表中的同一个位置。通常的解决办法是为每一个哈希值指向一个链表，用于存放所有哈希冲突的值；


MPQs use a hash table of filenames to keep track of the files inside, but the format of this table is somewhat different from the way hash tables are normally done. First of all, instead of using a hash as an offset, and storing the actually filename for verification, MPQs do not store the filename at all, but rather use three different hashes: one for the hash table offset, two for verification. These two verification hashes are used in place of the actual filename. Of course, this leaves the possibility that two different filenames would hash to the same three hashes, but the chances of this happening are, on average, 1:18889465931478580854784, which should be safe enough for just about anyone.

MPQs使用一个存放文件名的哈希表来跟踪文件内部，但是表的格式与通常方法有点不同，首先不像通常的做法使用哈希值作为偏移量，存储实际的文件名。MPQs 根本不存储文件名，而是使用了三个不同的哈希值：一个用做哈希表偏移量，两个用作核对。这两个核对的哈希值用于替代文件名。当然从理论上说存在两个不同的文件名得到相同的三个哈希值，但是这种情况发送的几率是：1:18889465931478580854784,这应该足够安全了。


The other way that an MPQ's hash table differs from the conventional implementation is that instead of using a linked list for each entry, when a collision occurs, the entry will be shifted to the next slot, and the process repeated until a free space is found. Take a look at the following illustrational code, which is basically the way a file is located for reading in an MPQ:

MPQ's的哈希表的实现与传统实现的另一个不同的地方是，相对与传统做法（为每个节点使用一个链表，当冲突发生的时候，遍历链表进行比较），看一下下面的示范代码，在MPQ中定位一个文件进行读操作：

```cpp
int GetHashTablePos(char *lpszString, MPQHASHTABLE *lpTable, int nTableSize)
{   
    const int HASH_OFFSET = 0, HASH_A = 1, HASH_B = 2;    
    int nHash = HashString(lpszString, HASH_OFFSET),nHashA = HashString(lpszString, HASH_A),nHashB = HashString(lpszString, HASH_B), nHashStart = nHash % nTableSize,nHashPos = nHashStart;
    while (lpTable[nHashPos].bExists)
    {
        if (lpTable[nHashPos].nHashA == nHashA && lpTable[nHashPos].nHashB == nHashB)
            return nHashPos;

        else
            nHashPos = (nHashPos + 1) % nTableSize;
            if (nHashPos == nHashStart)
                break;
    }
    return -1; //Error value

}
```



However convoluted that code may look,  the theory behind it isn't difficult. It basically follows this process when looking to read a file:


Compute the three hashes (offset hash and two check hashes) and store them in variables.


Move to the entry of the offset hash


Is the entry unused? If so, stop the search and return 'file not found'.


Do the two check hashes match the check hashes of the file we're looking for? If so, stop the search and return the current entry.


Move to the next entry in the list, wrapping around to the beginning if we were on the last entry.


Is the entry we just moved to the same as the offset hash (did we look through the whole hash table?)? If so, stop the search and return 'file not found'.


Go back to step 3.

无论代码看上去有多么复杂，其背后的理论并不难。读一个文件的时候基本遵循下面这样一个过程：

1、计算三个哈希值（一个哈希偏移量和两个验证值）并保存到变量中；

2、移动到哈希偏移量对应的值；

3、对应的位置是否尚未使用？如果是，则停止搜寻，并返回“文件不存在”；

4、这两个验证值是否与我们要找的字符串验证值匹配，如果是，停止搜寻，并返回当前的节点；

5、移动到下一个节点，如果到了最后一个节点则返回开始；

6、Is the entry we just moved to the same as the offset hash (did we look through the whole hash table?)? If so, stop the search and return 'file not found'.

7、回到第3步；


If you were paying attention, you might have noticed from my explanation and sample code is that the MPQ's hash table has to hold all the file entries in the MPQ. But what do you think happens when every hash-table entry gets filled? The answer might surprise you with its obviousness: you can't add any more files. Several people have asked me why there is a limit (called the file limit) on the number of files that can be put in an MPQ, and if there is any way around this limit. Well, you already have the answer to the first question. As for the second; no, you cannot get around the file limit. For that matter, hash tables cannot even be resized without remaking the entire MPQ from scratch. This is because the location of each entry in the hash table may well change due to the resizing, and we would not be able to derive the new position because the position is the hash of the file name, and we may not know the file name.

如果您注意的话，您可能已经从我们的解释和示例代码注意到，MPQ的哈希表已经将所有的文件入口放入MPQ中；那么当哈希表的每个项都被填充的时候，会发生什么呢？答案可能会让你惊讶：你不能添加任何文件。有些人可能会问我为什么文件数量上有这样的限制（文件限制），是否有办法绕过这个限制？就此而言，如果不重新创建MPQ 的项，甚至无法调整哈希表的大小。这是因为每个项在哈希表中的位置会因为跳闸尺寸而改变，而我们无法得到新的位置，因为这些位置值是文件名的哈希值，而我们根本不知道文件名是什么。















