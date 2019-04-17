# Hash算法 - PeterBishop - CSDN博客





2018年08月13日 23:13:57[PeterBishop0](https://me.csdn.net/qq_40061421)阅读数：72








# 哈希表（Hash Table）原理及其实现

# 原理

## 介绍

哈希表（Hash table，也叫散列表）， 是根据关键码值(Key value)而直接进行访问的数据结构。也就是说，它通过把关键码值映射到表中一个位置来访问记录，以加快查找的速度。这个映射函数叫做散列函数，存放记录的数组叫做散列表。

哈希表hash table(key，value) 的做法其实很简单，就是把Key通过一个固定的算法函数既所谓的哈希函数转换成一个整型数字，然后就将该数字对数组长度进行取余，取余结果就当作数组的下标，将value存储在以该数字为下标的数组空间里。

而当使用哈希表进行查询的时候，就是再次使用哈希函数将key转换为对应的数组下标，并定位到该空间获取value，如此一来，就可以充分利用到数组的定位性能进行数据定位。

哈希表最大的优点，就是把数据的存储和查找消耗的时间大大降低，几乎可以看成是常数时间；而代价仅仅是消耗比较多的内存。然而在当前可利用内存越来越多的情况下，用空间换时间的做法是值得的。另外，编码比较容易也是它的特点之一。 哈希表又叫做散列表，分为“开散列” 和“闭散列”。

我们使用一个下标范围比较大的数组来存储元素。可以设计一个函数（哈希函数， 也叫做散列函数），使得每个元素的关键字都与一个函数值（即数组下标）相对应，于是用这个数组单元来存储这个元素；也可以简单的理解为，按照关键字为每一 个元素“分类”，然后将这个元素存储在相应“类”所对应的地方。

但是，不能够保证每个元素的关键字与函数值是一一对应的，因此极有可能出现对于不同的元素，却计算出了相同的函数值，这样就产生了“冲突”，换句话说，就是把不同的元素分在了相同的“类”之中。后面我们将看到一种解决“冲突”的简便做法。 总的来说，“直接定址”与“解决冲突”是哈希表的两大特点。

## 哈希函数构造

就是映射函数构造，看某个元素具体属于哪一个类别。 

除余法： 选择一个适当的正整数 p ，令 h(k ) = k mod p ，这里， p 如果选取的是比较大的素数，效果比较好。而且此法非常容易实现，因此是最常用的方法。最直观的一种，上图使用的就是这种散列法，公式： 

index = value % 16 

学过汇编的都知道，求模数其实是通过一个除法运算得到的，所以叫“除法散列法”。

平方散列法 

求index是非常频繁的操作，而乘法的运算要比除法来得省时（对现在的CPU来说，估计我们感觉不出来），所以我们考虑把除法换成乘法和一个位移操作。公式： 

index = (value * value) >> 28 （ 右移，除以2^28。记法：左移变大，是乘。右移变小，是除）

数字选择法： 如果关键字的位数比较多，超过长整型范围而无法直接运算，可以选择其中数字分布比较均匀的若干位，所组成的新的值作为关键字或者直接作为函数值。

斐波那契（Fibonacci）散列法：平方散列法的缺点是显而易见的，所以我们能不能找出一个理想的乘数，而不是拿value本身当作乘数呢？答案是肯定的。 

1，对于16位整数而言，这个乘数是40503 

2，对于32位整数而言，这个乘数是2654435769 

3，对于64位整数而言，这个乘数是11400714819323198485 

这几个“理想乘数”是如何得出来的呢？这跟一个法则有关，叫黄金分割法则，而描述黄金分割法则的最经典表达式无疑就是著名的斐波那契数列，即如此形式的序列：0, 1, 1, 2, 3, 5, 8, 13, 21, 34, 55, 89, 144, 233,377, 610， 987, 1597, 2584, 4181, 6765, 10946，…。另外，斐波那契数列的值和太阳系八大行星的轨道半径的比例出奇吻合。 

对我们常见的32位整数而言，公式： 

index = (value * 2654435769) >> 28

### **解决冲突的方法（搜索的时候也是通过这个来找）**

通过哈希函数去计算哈希值，难免会有冲突的时候，解决冲突的方法有如下几种：
- 
开放定址法：依靠数组中的空位解决碰撞冲突
- 线性探测法：直接检测散列表的下一个位置（即索引值加1），如果仍冲突，继续；
- 二次探测法：即H + 1^2, H + 2^2, H + 3^2…
- 伪随机探测

- 
再哈希法：使用多个哈希函数，第一个冲突时，使用第二个哈希函数，直到不冲突为止；

- 链地址法：将所有哈希地址相同的关键字，都链接到同一个链表中；



```cpp
#include <stdio.h>
#include<string.h>
#include <stdlib.h>
#include <malloc.h>



#define MAX_SIZE        100

typedef int Element;

typedef struct hash_node
{
	Element data;
	struct hash_node *next;
}hash_node, *hnode, *hlist;

typedef struct hash_tab
{
	Element tsize;
	hash_node **tlist;
}hash_tab, *htable;


hnode init_node()
{
	hnode hn;
	if ((hn = (hnode)malloc(sizeof(hash_node))) == NULL)
	{
		fprintf(stderr, "Init hash list error %s: %d\n", __FILE__, __LINE__);
		return NULL;
	}
	return hn;
}

htable init_table(int size)
{
	int i;
	htable ht;
	if ((ht = (htable)malloc(sizeof(hash_tab))) == NULL)
	{
		fprintf(stderr, "Init hash table error %s: %d\n", __FILE__, __LINE__);
		return NULL;
	}
	ht->tsize = size;
	ht->tlist = (hlist *)malloc(sizeof(hlist) * size);
	for (i = 0; i<size; i++)
	{
		ht->tlist[i] = init_node();
		ht->tlist[i]->data = i;
		ht->tlist[i]->next = NULL;
	}
	return ht;
}

int hash_int(Element val, int size)
{
	int key = val % size;
	return key;
}

// Brian Kernighan & Dennis Ritchie' hash algorithm used in Java
long hash_string(Element *val, int size)
{
	long hash, ch;
	while ((ch = (long)*val++) != NULL)
	{
		hash = hash * 31 + ch;
	}
	return hash;
}

hnode search_hnode_int(htable ht, Element val)
{
	int key = hash_int(val, ht->tsize);
	hnode hn = ht->tlist[key];
	hn = hn->next;
	while (hn != NULL && hn->data != val)
		hn = hn->next;
	return hn;
}

hnode search_hnode_string(htable ht, Element *val)
{
	int key = hash_string(val, ht->tsize);
	hnode hn = ht->tlist[key];
	hn = hn->next;
	while (hn != NULL && strcmp((char*)hn->data, (char*)val))
		hn = hn->next;
	return hn;
}

// add hash node at the beginning of the hash list
void add_hnode(htable ht, Element val)
{
	int key = hash_int(val, ht->tsize);
	hnode hn = init_node();
	// hnode tail;
	if (search_hnode_int(ht, val) == NULL)
	{
		hn->data = val;
		hn->next = ht->tlist[key]->next;
		ht->tlist[key]->next = hn;
	}
}

void display_htable_simple(htable ht)
{
	int i;
	hnode hn;
	for (i = 0; i<ht->tsize; i++)
	{
		if (ht->tlist[i]->next == NULL)
			continue;
		// printf("node %2d: ", ht->tlist[i]->data);
		printf("node %2d: ", i);
		hn = ht->tlist[i]->next;
		while (hn)
		{
			printf("->%d ", hn->data);
			hn = hn->next;
		}
		printf("\n");
	}
}

void display_htable_all(htable ht)
{
	int i;
	hnode hn;
	for (i = 0; i<ht->tsize; i++)
	{
		printf("node %d: ", ht->tlist[i]->data);
		hn = ht->tlist[i]->next;
		while (hn)
		{
			printf("->%d ", hn->data);
			hn = hn->next;
		}
		printf("\n");
	}
}

/*
// get intersection of the given two arrays
void insersection(int *a, int *b, int lena, int lenb)
{
int i, j;
htable ht = init_table(lena+lenb);
for(i=0, j=0; i<lena, j<lenb; i++, j++)
{
add_hnode(ht, lena[i]);
add_hnode(ht, lena[j]);
}
}
*/

int main()
{
	htable ht = init_table(MAX_SIZE);
	// display_htable(ht);
	add_hnode(ht, 1);
	add_hnode(ht, 24);
	add_hnode(ht, 25);
	add_hnode(ht, 9);
	add_hnode(ht, 172);
	add_hnode(ht, 600);
	add_hnode(ht, 700);
	add_hnode(ht, 800);
	add_hnode(ht, 1025);
	add_hnode(ht, 800);
	display_htable_simple(ht);
	return 0;
}
```





