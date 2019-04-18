# 采用分离链接法的HashTable的实现 - Orisun - 博客园







# [采用分离链接法的HashTable的实现](https://www.cnblogs.com/zhangchaoyang/articles/2631067.html)





首先给出一个对字符串比较好的散列函数，在有些地方把该算法称为“均匀哈希算法”。

//提供一个对string进行散列的函数
int hashString(const string &str){
	string s;
	if(str.size()>1024)		//如果str太长，则只取前1024个字符
		s=str.substr(0,1024);
	else
		s=str;
	int rect=0;
	for(int i=0;i<s.size();++i)
		rect=rect*37+s[i];		//Horner法则
	return rect;
}


这个散列函数可能会溢出，导致返回值为负数。注意这里选择的数字是37,好在哪里我也无法证明，至少可以看到素数在散列函数中十分有用。

再好的散列函数也会产生冲突（collision），而且冲突还时有发生，所以一个HashTable必须考虑冲突如何解决。方法有：
- 分离链接法（separate chaining），后面会详细介绍。
- 线性探测法。就用采用散列函数h0发现有冲突时就采用h1，如果还有冲突就采用h2……其中hi(x)=[ h(x) + f(i) ] mod tableSize。 f(i)为i的线性函数，通常取f(i)=i。
- 平方探测法。与线性探测法法类似， f(i)为i的二次函数，通常取f(i)=i^2。
- 双散列。f(i)=i·h2(x)。

分离链接法就是把冲突的元素存放在一个链表里。当然除链表外也可采用其他数据结构，如二叉查找树、甚至是另外一个散列表，它们的查找速度都比链表要快。但是我们期望的是散列函数足够地好、槽足够地多，所以对应的链表都应该很短，不值得去尝试更复杂的结构。

槽数为素数时，HashTable的性能会比较好，这就引用一个问题：如何确定一个数是素数？一种快速的判别方法起源于[费马小定理](http://baike.baidu.com/view/263807.htm)。

如果对于任意满足1 < b < p的b下式都成立：

 b^(p-1）≡1(mod p)

 则p必定是一个质数。


//模c下求a的b次方
int powermod(int a,int b,int c){
	if(b==0)
		return 1;
	if(b==1)
		return a%c;
	int t=powermod(a,b/2,c);
	if(b&1){		//如果b是奇数
		return t*t*a%c;
	}
	else{
		return t*t%c;
	}
}
//判断数p是否为素数
bool isPrime(int p){
	for(int i=0;i<100;i++){;
		if(powermod(rand()%(p-1)+1,p-1,p)!=1)
			return false;
	}
	return true;
}


　　假设一个散列表能容纳n个元素、具有m个槽，定义其装载因子(load factor)α为n/m。假定散列函数足够地好，任何一个元素散列到m个槽位的可能性是相同的，且与其他元素已被散列到什么位置上独立无关（这个假设称为简单一致散列simple uniform hashing）。则平均情况下查找一个元素是否在散列表中的时间复杂度是O(1+α)。所以装载因子α成为我们关注的焦点。


当一个HashTable太满后，发生冲突的概率就会大大增加。我们的策略是：当达到事先设定的装载因子时，就把槽位扩展成原先的2倍以上（取最小的素数）。这叫再散列。原先的HashTable完全释放，申请新的更大的空间，然后把已有的元素重新散列到新的HashTable。再散列开销很大，但我们期望的是发生再散列的次数很少。

template<typename HashObj>
void HashTable<HashObj>::rehash(){
	int oldSlot=getSlots();
	int newSlot=oldSlot*2;
	while(!isPrime((unsigned long)newSlot)){
		newSlot++;
	}
	
	vector<list<HashObj> > oldVector = table;
	for(int i=0;i<table.size();++i)
		table[i].clear();
	table.resize(newSlot);
	setSlots(newSlot);
	capacity=0;
	
	for(int i=0;i<oldVector.size();++i){
		typename list<HashObj>::iterator itr=oldVector[i].begin();
		while(itr!=oldVector[i].end())
			insert(*itr++);
	}
}


有一个语法点：注意17行，当使用含有模板类型的迭代器时，要在前面加一个typename。

再散列需要把之前散列过的key重新做一次散列，这称为非一致性哈希。一致性哈希要求提供一个hashtable,它能在节点加入离开时不会导致映射关系的重大变化。在某些实际应用中需要使用[一致性哈希](http://blog.csdn.net/chen77716/article/details/5949166)。

下面给出完整代码：

hash.h

#ifndef _HASH_H
#define _HASH_H

#include<vector>
#include<list>
#include<string>

using namespace std;

//HashObj类型的数据必须提供函数：int hash();operator ==
template<typename HashObj>
class HashTable{
private:
	int capacity;		//已容纳的元素的个数
	int slots;			//槽的个数（取素数散列性比较好）
	double alpha;		//装载因子
	vector<list<HashObj> > table;		//哈希表
	int myhash(const HashObj& ele);		//内部哈希函数，负责把数据映射到[0,slots-1]
	void rehash();		//当达到装载因子时，槽数扩大为原来的2倍以上（取最小的素数），重新进行再散列
	
public:
	//判断一个数据是否在HashTable中
	bool contain(const HashObj &ele);
	//插入一个元素到HashTable。插入成功则返回true，如果元素原先就存在于HashTable中，则返回false
	bool insert(const HashObj& ele);
	//从HashTable中删除一个元素。如果指定元素不在HashTable中，则删除失败，返回false
	bool remove(const HashObj &ele);
	HashTable(int slots=10007,double alpha=0.7):slots(slots),alpha(alpha){
		capacity=0;
		table.resize(slots);
	}
	int getSlots(){
		return slots;
	}
	void setSlots(int num){
		slots=num;
	}
};

//提供一个对string进行散列的函数
int hashString(const string &str){
	string s;
	if(str.size()>1024)		//如果str太长，则只取前1024个字符
		s=str.substr(0,1024);
	else
		s=str;
	int rect=0;
	for(int i=0;i<s.size();++i)
		rect=rect*37+s[i];		//Horner法则
	return rect;
}

//由于散列的槽数最好是素数，所以提供一个判别素数的函数
int fmod(int a, int b, int c)//快速模取幂  
{  
    if(b == 1) return a;  
    int t = fmod(a, b / 2, c);  
    t = (t * t) % c;  
    if(b & 1) t = (t * a) % c;  
    return t;  
}  
bool isPrime(int n)//米勒-拉宾算法  
{  
    for(int i = 0; i < 100; ++ i)  
    {  
        if(fmod(rand() % (n - 1) + 1, n - 1, n) != 1)//a的取值为[1,n-1]，a的值需要变化，所以用到随机函数  
            return false;  
    }  
    return true;  
}  

//提供一个满足HashObj要求的类
class Employee{
private:
	string name;
	int id;
public:
	Employee(string name="",int id=-1):name(name),id(id){}
	bool operator == (const Employee & obj) const{
		return this->name==obj.name;
	}
	int hash()const{
		return hashString(name);
	}
	void setName(string str){
		name=str;
	}
	void setId(int i){
		id=i;
	}
};

#endif

hash.cpp

#include<iostream>
#include<cassert>
#include<algorithm>
#include"hash.h"

template<typename HashObj>
int HashTable<HashObj>::myhash(const HashObj & ele){
	int rect=ele.hash();
	rect%=slots;
	if(rect<0)
		rect+=slots;
	return rect;
}

template<typename HashObj>
bool HashTable<HashObj>::contain(const HashObj & ele){
	int index=myhash(ele);
	const list<HashObj> & whichList=table[index];
	return find(whichList.begin(),whichList.end(),ele)!=whichList.end();
}

template<typename HashObj>
bool HashTable<HashObj>::insert(const HashObj & ele){
	int index=myhash(ele);
	list<HashObj> & whichList=table[index];
	if(find(whichList.begin(),whichList.end(),ele)!=whichList.end())
		return false;
	whichList.push_back(ele);
	capacity++;
	if(capacity*1.0/slots>alpha)
		rehash();
	return true;
}

template<typename HashObj>
bool HashTable<HashObj>::remove(const HashObj & ele){
	int index=myhash(ele);
	list<HashObj> & whichList=table[index];
	typename list<HashObj>::iterator itr=find(whichList.begin(),whichList.end(),ele);
	if(itr==whichList.end())
		return false;
	whichList.erase(itr);
	capacity--;
	return true;
}

template<typename HashObj>
void HashTable<HashObj>::rehash(){
	int oldSlot=getSlots();
	int newSlot=oldSlot*2;
	while(!isPrime((unsigned long)newSlot)){
		newSlot++;
	}
	
	vector<list<HashObj> > oldVector = table;
	for(int i=0;i<table.size();++i)
		table[i].clear();
	table.resize(newSlot);
	setSlots(newSlot);
	capacity=0;
	
	for(int i=0;i<oldVector.size();++i){
		typename list<HashObj>::iterator itr=oldVector[i].begin();
		while(itr!=oldVector[i].end())
			insert(*itr++);
	}
}

int main(){
	const int arrSize=9;
	HashTable<Employee> hashTable(7,1.0);		//刚开始设槽数为7
	string names[arrSize]={"hujintao","jiangzeming","heizeming",
							"chaogai","jingchengwu","liangchaowei",
							"weijiabao","zhamgsanbao","zengxiaoxian"};
	Employee employee[arrSize];
	for(int i=0;i<arrSize;++i){
		employee[i].setName(names[i]);
		employee[i].setId(i+1);
	}

	for(int i=0;i<arrSize;++i){
		hashTable.insert(employee[i]);
	}
	assert(hashTable.getSlots()==17);		//扩容后槽数应该为17
	for(int i=0;i<arrSize;++i){
		assert(hashTable.contain(employee[i]));
		hashTable.remove(employee[i]);
		assert(!hashTable.contain(employee[i]));
	}
	
	return 0;
}















