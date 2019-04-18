# 【STL学习】自己动手C++编程实现hash table（散列表） - 江南烟雨 - CSDN博客
2013年04月01日 15:52:03[江南烟雨](https://me.csdn.net/xiajun07061225)阅读数：5208
SGI STL中散列表采用链接法解决冲突。结构中维护了一个vector，vector中每一个元素称为一个桶（bucket），它包含的是一个链表的第一个节点。
下面代码展示了自己编程实现的hash table，C++模板类封装。
如有错误，还请包涵和指正（E-Mail：xiajunhust@gmail.com）！
HashTable.h：
```cpp
//《STL源码剖析》5.7 hashtable编程实现
//Author：江南烟雨
//E-Mail：xiajunhust@gmail.com
#include <iostream>
#include <algorithm>
#include <vector>
//hash table中链表节点数据结构定义
template <class ValueType>
struct __hashtable_node{
	__hashtable_node *next;
	ValueType val;
};
static const int __stl_num_primes = 28;//表格大小(28个质数)
//28个质数
static const unsigned long __stl_prime_list[__stl_num_primes] = {
	53,         97,           193,         389,       769,
	1543,       3079,         6151,        12289,     24593,
	49157,      98317,        196613,      393241,    786433,
	1572869,    3145739,      6291469,     12582917,  25165843,
	50331653,   100663319,    201326611,   402653189, 805306457, 
	1610612741, 3221225473ul, 4294967291ul
};
//得出28个质数中不小于n的那个质数
inline unsigned long __get_next_prime(unsigned long n)
{
	const unsigned long *first = __stl_prime_list;
	const unsigned long *last = __stl_prime_list + __stl_num_primes;
	const unsigned long *pos = std::lower_bound(first,last,n);
	return pos == last ? *(last - 1) : *pos;
}
template <class T1,class T2>
void construct(T1 *p,const T2 &value)
{
	new (p) T1(value);
}
template <class T>
void destroy(T* pointer)
{
	pointer->~T();//调用析构函数
}
//hash函数定义
//都是仿函数
template <class KeyType>
struct hash{};
//字符串要进行映射
inline size_t __stl_hash_string(const char *s)
{
	unsigned long h = 0;
	for (;*s;++s)
	{
		h = 5 * h + *s;
	}
	return size_t(h);
}
//类模板显示特化定义
template <>
struct hash<int>{
	size_t operator()(int x) const {return x;}
};
template <>
struct hash<char *>{
	size_t operator()(const char *s) const {return __stl_hash_string(s);}
};
template <>
struct hash<const char *>{
	size_t operator()(const char *s) const {return __stl_hash_string(s);}
};
template <>
struct hash<char>{
	size_t operator()(char s) const {return s;}
};
template <>
struct hash<unsigned char>{
	size_t operator()(unsigned char s) const {return s;}
};
template <>
struct hash<signed char>{
	size_t operator()(signed char s) const {return s;}
};
// C++ Standard 规定，每一个 Adaptable Unary Function 都必须继承此类别
template <class Arg,class Result>
struct unary_function{
	typedef Arg argument_type;
	typedef Result result_type;
};
// C++ Standard 规定，每一个 Adaptable Binary Function 都必须继承此类别
template <class Arg1,class Arg2,class Result>
struct binary_function{
	typedef Arg1 first_argument_type;
	typedef Arg2 second_argument_type;
	typedef Result result_type;
};
//从节点中取出键值的仿函数定义
//identity function；任何数值通过此函数式后，不会发生任何改变
template <class T>
struct identity:public unary_function<T,T>
{
	const T& operator()(const T& x) const{return x;}
};
//判断键值是否相等的仿函数定义
template <class T>
struct equal_to:public binary_function<T,T,bool>{
	bool operator()(const T& x,const T& y) const{return x == y;}
};
//比较字符串是否相等的仿函数
struct eqstr{
	bool operator()(const char *s1,const char *s2)const
	{
		return strcmp(s1,s2) == 0;
	}
};
//hash table数据结构定义
//模板参数：
//ValueType：节点的实值型别
//KeyType：节点的键值型别
//HashFcn：hash function的函数型别
//ExtractKey：从节点中取出键值的方法
//EqualKey：判断键值是否相同
template <class ValueType,class KeyType,class HashFcn,class ExtractKey,class EqualKey>
class HashTableClass{
public:
	typedef struct __hashtable_node<ValueType> node;//hash table内部链表节点定义
	//hash table没有提供默认构造函数
	HashTableClass(size_t n,
		const HashFcn &hf,
		const EqualKey &eql,
		const ExtractKey &ext)
		:hasher(hf),equals(eql),get_key(ext),num_elements(0)
	{
		initialize_buckets(n);
	}
	HashTableClass(size_t n,
		const HashFcn &hf,
		const EqualKey &eql)
		:hasher(hf),equals(eql),get_key(ExtractKey()),num_elements(0)
	{
		initialize_buckets(n);
	}
	HashTableClass(const HashTableClass &ht)
		:hasher(ht.hasher),equals(ht.equals),get_key(ht.get_key),num_elements(0)
	{
		copy_from(&ht);
	}
	~HashTableClass(){clear();};
	//赋值操作符
	HashTableClass& operator= (const HashTableClass &ht)
	{
		if (&ht != this)
		{
			clear();
			hasher = ht.hasher;
			equals = ht.equals;
			get_key = ht.get_key;
			copy_from(&ht);
		}
	}
	//返回元素个数
	size_t size(){return num_elements;}
	//最大元素数目
	size_t max_size(){return size_t(-1);}
	//返回bucket vector大小
	size_t bucket_count(){return buckets.size();}
	//返回bucket vector可能的最大值
	size_t max_bucket_count(){return __stl_prime_list[__stl_num_primes - 1];}
	//插入元素，不允许重复
	std::pair<node *,bool> insert_unique(const ValueType &obj);
	//插入元素，允许重复
	node* insert_equal(const ValueType &obj);
	//打印所有节点
	void printAllNodes();
	//遍历所有buckets
	void printAllBuckets();
	//查找某一键值的节点
	std::pair<node*,bool> find(const KeyType &key);
	//判断某一值出现的次数
	size_t count(const KeyType &key);
	//整体删除
	void clear();
	//复制hash表
	void copy_from(const HashTableClass *ht);
private:
	std::vector<node*> buckets;//由桶组成的vector
	size_t num_elements;//总的元素个数
	HashFcn hasher;
	ExtractKey get_key;
	EqualKey equals;
	//节点配置和释放函数
	node* new_node(const ValueType &obj)
	{
		node *tempNode = new node;
		tempNode->next = NULL;
		try
		{
			construct(&tempNode->val,obj);
		}
		catch (...)
		{
			delete tempNode;
			throw;
			return NULL;
		}
		return tempNode;
	}
	void delete_node(node *n)
	{
		destroy(&n->val);
		delete n;
	}
	//初始化buckets vector
	void initialize_buckets(size_t n);
	//返回最接近n并大于等于n的质数
	size_t next_size(size_t n)const{return __get_next_prime(n);}
	//判断是否需要扩充buckets vector，如有需要则进行扩充
	void resize(size_t num_elements_hint);
	//判断元素落在哪个bucket
	//提供两个版本
	//版本一：只接受实值
	size_t bkt_num(const ValueType &obj) const{return bkt_num_key(get_key(obj));}
	//版本二：接受实值和buckets个数
	size_t bkt_num(const ValueType &obj,size_t n) const {return bkt_num_key(get_key(obj),n);}
	//版本一：只接受键值
	size_t bkt_num_key(const KeyType &Key) const{return hasher(Key) % (buckets.size());}
	//版本二：接受键值和buckets个数
	size_t bkt_num_key(const KeyType &Key,size_t n) const {return hasher(Key) % n;}
	//在不需要重新分配bucket vector的情况下插入元素，元素不允许重复
	std::pair<node *,bool> insert_unique_noresize(const ValueType &obj);
	//在不需要重新分配bucket vector的情况下插入元素，元素不允许重复
	node* insert_equal_noresize(const ValueType &obj);
};
template <class ValueType,class KeyType,class HashFcn,class ExtractKey,class EqualKey>
std::pair<typename HashTableClass<ValueType,KeyType,HashFcn,ExtractKey,EqualKey>::node *,bool> 
HashTableClass<ValueType,KeyType,HashFcn,ExtractKey,EqualKey>::insert_unique(const ValueType &obj)
{
	resize(num_elements + 1);
	return insert_unique_noresize(obj);
}
template <class ValueType,class KeyType,class HashFcn,class ExtractKey,class EqualKey>
typename HashTableClass<ValueType,KeyType,HashFcn,ExtractKey,EqualKey>::node *
HashTableClass<ValueType,KeyType,HashFcn,ExtractKey,EqualKey>::insert_equal(const ValueType &obj)
{
	resize(num_elements + 1);
	return insert_equal_noresize(obj);
}
template <class ValueType,class KeyType,class HashFcn,class ExtractKey,class EqualKey>
void HashTableClass<ValueType,KeyType,HashFcn,ExtractKey,EqualKey>::printAllNodes()
{
	cout << endl;
	cout << "Current node in hash table : " << endl;
	for (size_t i = 0;i < buckets.size();++i)
	{
		typename HashTableClass<ValueType,KeyType,HashFcn,ExtractKey,EqualKey>::node* curNode = buckets[i];
		while(curNode)
		{
			cout << curNode->val << " ";
			curNode = curNode->next;
		}
	}
	cout << endl;
}
template <class ValueType,class KeyType,class HashFcn,class ExtractKey,class EqualKey>
void HashTableClass<ValueType,KeyType,HashFcn,ExtractKey,EqualKey>::printAllBuckets()
{
	cout << endl;
	cout << "Current buckets in hash table : " << endl;
	for (size_t i = 0;i < buckets.size();++i)
	{
		typename HashTableClass<ValueType,KeyType,HashFcn,ExtractKey,EqualKey>::node* curNode = buckets[i];
		if(NULL == curNode)
			cout << "buckets[" << i << "] is empty!" << endl;
		else
		{
			size_t count = 0;
			while(curNode)
			{
				++count;
				curNode = curNode->next;
			}
			cout << "buckets[" << i << "] has " << count << " elements : ";
			curNode = buckets[i];
			while(curNode)
			{
				cout << curNode->val << " ";
				curNode = curNode->next;
			}
			cout << endl;
		}
	}
}
template <class ValueType,class KeyType,class HashFcn,class ExtractKey,class EqualKey>
std::pair<typename HashTableClass<ValueType,KeyType,HashFcn,ExtractKey,EqualKey>::node *,bool> 
HashTableClass<ValueType,KeyType,HashFcn,ExtractKey,EqualKey>::find(const KeyType &key)
{
	size_t bucket_index = bkt_num_key(key);
	node*first = buckets[bucket_index];
	while(first)
	{
		if (equals(key,get_key(first->val)))
		{
			cout << "find the element " << key << " success" << endl;
			return std::pair<typename HashTableClass<ValueType,KeyType,HashFcn,ExtractKey,EqualKey>::node *,bool>(first,true);
		}
		first = first->next;
	}
	cout << "cannot find the element " << key << endl;
	return std::pair<typename HashTableClass<ValueType,KeyType,HashFcn,ExtractKey,EqualKey>::node *,bool>(NULL,false);
}
template <class ValueType,class KeyType,class HashFcn,class ExtractKey,class EqualKey>
size_t HashTableClass<ValueType,KeyType,HashFcn,ExtractKey,EqualKey>::count(const KeyType &key)
{
	size_t bucket_index = bkt_num_key(key);
	node*first = buckets[bucket_index];
	size_t num = 0;
	while(first)
	{
		if (equals(key,get_key(first->val)))
		{
			++num;
		}
		first = first->next;
	}
	cout << "The element " << key << " appears " << num << " times" << endl;
	return num;
}
template <class ValueType,class KeyType,class HashFcn,class ExtractKey,class EqualKey>
void HashTableClass<ValueType,KeyType,HashFcn,ExtractKey,EqualKey>::clear()
{
	for (size_t i = 0;i < buckets.size();++i)
	{
		node* first = buckets[i];
		//删除bucket list中的每个节点
		while(first)
		{
			node *next = first->next;
			delete_node(first);
			first = next;
		}
		buckets[i] = 0;
	}
	//总元素个数置0
	num_elements = 0;
	//vector并未释放掉空间（自动回收）
}
template <class ValueType,class KeyType,class HashFcn,class ExtractKey,class EqualKey>
void HashTableClass<ValueType,KeyType,HashFcn,ExtractKey,EqualKey>::copy_from(const HashTableClass *ht)
{
	buckets.clear();//清除已有vector
	//使得bucket vector空间和对方相同
	buckets.reserve(ht->buckets.size());
	//插入n个元素，null
	buckets.insert(buckets.end(),ht->buckets.size(),(typename HashTableClass<ValueType,KeyType,HashFcn,ExtractKey,EqualKey>::node *)0);
	for (size_t i = 0;i < ht->buckets.size();++i)
	{
		if (const node *cur = ht->buckets[i])
		{
			node *tempNode = new_node(cur->val);
			buckets[i] = tempNode;
			for (node *next = cur->next;next;next = next->next)
			{
				tempNode->next = new_node(next->val);
				tempNode = tempNode->next;
			}
		}
	}
	num_elements = ht->num_elements;
}
template <class ValueType,class KeyType,class HashFcn,class ExtractKey,class EqualKey>
void HashTableClass<ValueType,KeyType,HashFcn,ExtractKey,EqualKey>::initialize_buckets(size_t n)
{
	const size_t n_buckets = next_size(n);
	buckets.reserve(n_buckets);
	buckets.insert(buckets.end(),n_buckets,(node*)0);
}
template <class ValueType,class KeyType,class HashFcn,class ExtractKey,class EqualKey>
void HashTableClass<ValueType,KeyType,HashFcn,ExtractKey,EqualKey>::resize(size_t num_elements_hint)
{
	//buckets vector重建与否的标准：
	//比较新的总元素个数和原buckets vector大小
	const size_t old_n_vector = buckets.size();
	//需要重新配置
	if (num_elements_hint > old_n_vector)
	{
		const size_t n = next_size(num_elements_hint);
		if (n > old_n_vector)//有可能vector已到达最大
		{
			vector<node*> tempVec(n,(node *)0);
			for (size_t bucketIndex = 0;bucketIndex < old_n_vector;++bucketIndex)
			{
				node *first = buckets[bucketIndex];//指向节点对应之串行的起始节点
				while(first)
				{
					//计算节点落在哪一个新的bucket内
					size_t new_bucket_index = bkt_num_key(first->val,n);
					buckets[bucketIndex] = first->next;
					first->next = tempVec[new_bucket_index];
					tempVec[new_bucket_index] = first;
					first = buckets[bucketIndex];
				}
			}
			//交换新旧两个bucket vector
			buckets.swap(tempVec);
		}
	}
}
template <class ValueType,class KeyType,class HashFcn,class ExtractKey,class EqualKey>
std::pair<typename HashTableClass<ValueType,KeyType,HashFcn,ExtractKey,EqualKey>::node *,bool> 
HashTableClass<ValueType,KeyType,HashFcn,ExtractKey,EqualKey>::insert_unique_noresize(const ValueType &obj)
{
	size_t bucket_index = bkt_num(obj);
	node *first = buckets[bucket_index];
	//搜索当前链表
	for (node *curNode = first;curNode;curNode = curNode->next)
	{
		if(equals(get_key(obj),get_key(curNode->val)))
			return pair<typename HashTableClass<ValueType,KeyType,HashFcn,ExtractKey,EqualKey>::node*,bool>(curNode,false);
	}
	node *tempNode = new_node(obj);
	tempNode->next = first;
	buckets[bucket_index] = tempNode;
	++num_elements;
	return pair<typename HashTableClass<ValueType,KeyType,HashFcn,ExtractKey,EqualKey>::node*,bool>(tempNode,true);
}
template <class ValueType,class KeyType,class HashFcn,class ExtractKey,class EqualKey>
typename HashTableClass<ValueType,KeyType,HashFcn,ExtractKey,EqualKey>::node*
HashTableClass<ValueType,KeyType,HashFcn,ExtractKey,EqualKey>::insert_equal_noresize(const ValueType &obj)
{
	size_t bucket_index = bkt_num(obj);
	node *first = buckets[bucket_index];
	for (node *curNode = first;curNode;curNode = curNode->next)
	{
		//发现与链表中的某键值相等，马上插入，然后返回
		if (equals(get_key(obj),get_key(curNode->val)))
		{
			node *tempNode = new_node(obj);
			tempNode->next = curNode->next;
			curNode->next = tempNode;
			++num_elements;
			return tempNode;
		}
	}
	//如果没发现键值相等的元素
	node *tempNode = new_node(obj);
	//将其插入链表头部
	tempNode->next = first;
	buckets[bucket_index] = tempNode;
	++num_elements;
	return tempNode;
}
```
HashTable.cpp：
```cpp
#include "HashTable.h"
using namespace std;
int main()
{
	HashTableClass<int,int,hash<int>,identity<int>,equal_to<int> > *hashTableObj = new HashTableClass<int,int,hash<int>,identity<int>,equal_to<int> >(50,hash<int>(),equal_to<int>());
	cout << "Hash Table size : " << hashTableObj->size() << endl;
	cout << "Hash Table bucket count : " << hashTableObj->bucket_count() << endl;
	cout << "Hash Table max bucket count : " << hashTableObj->max_bucket_count() << endl;
	hashTableObj->insert_unique(59);
	hashTableObj->insert_unique(63);
	hashTableObj->insert_unique(108);
	hashTableObj->insert_unique(2);
	hashTableObj->insert_unique(53);
	hashTableObj->insert_unique(55);
	cout << "Hash Table size : " << hashTableObj->size() << endl;
	hashTableObj->printAllNodes();
	hashTableObj->printAllBuckets();
	//继续插入元素，使总元素个数达到54个
	for(int i = 0;i <= 47;++i)
		hashTableObj->insert_equal(i);
	cout << endl;
	cout << "Hash Table size : " << hashTableObj->size() << endl;
	cout << "Hash Table bucket count : " << hashTableObj->bucket_count() << endl;
	hashTableObj->printAllNodes();
	hashTableObj->printAllBuckets();
	hashTableObj->find(2);
	hashTableObj->count(2);
	HashTableClass<int,int,hash<int>,identity<int>,equal_to<int> > *hashTableObj2 = new HashTableClass<int,int,hash<int>,identity<int>,equal_to<int> >(20,hash<int>(),equal_to<int>());
	cout << "Hash Table 2 size : " << hashTableObj2->size() << endl;
	cout << "Hash Table 2 bucket count : " << hashTableObj2->bucket_count() << endl;
	cout << "Hash Table 2 max bucket count : " << hashTableObj2->max_bucket_count() << endl;
	hashTableObj2->copy_from(hashTableObj);
	cout << "Hash Table 2 size : " << hashTableObj2->size() << endl;
	cout << "Hash Table 2 bucket count : " << hashTableObj2->bucket_count() << endl;
	cout << "Hash Table 2 max bucket count : " << hashTableObj2->max_bucket_count() << endl;
	HashTableClass<int,int,hash<int>,identity<int>,equal_to<int> > hashTableObj3(*hashTableObj2);
	cout << "Hash Table 3 size : " << hashTableObj3.size() << endl;
	cout << "Hash Table 3 bucket count : " << hashTableObj3.bucket_count() << endl;
	cout << "Hash Table 3 max bucket count : " << hashTableObj3.max_bucket_count() << endl;
	HashTableClass<char *,char *,hash<char *>,identity<char*>,eqstr > *hashTableObjString = new HashTableClass<char *,char *,hash<char *>,identity<char*>,eqstr >(20,hash<char *>(),eqstr());
	hashTableObjString->insert_unique("jun");
	hashTableObjString->insert_unique("hust");
	cout << "Hash Table hashTableObjString size : " << hashTableObjString->size() << endl;
	cout << "Hash Table hashTableObjString bucket count : " << hashTableObjString->bucket_count() << endl;
	cout << "Hash Table hashTableObjString max bucket count : " << hashTableObjString->max_bucket_count() << endl;
	hashTableObjString->printAllNodes();
	//hashTableObjString->printAllBuckets();
	hashTableObjString->find("juu");
	delete hashTableObj;
	delete hashTableObj2;
	delete hashTableObjString;
	return 0;
}
```
运行结果（VS2008+Win7）：
![](https://img-my.csdn.net/uploads/201304/01/1364803020_2324.jpg)
![](https://img-my.csdn.net/uploads/201304/02/1364872754_2269.jpg)
