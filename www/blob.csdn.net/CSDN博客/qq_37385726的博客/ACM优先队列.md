# ACM优先队列 - qq_37385726的博客 - CSDN博客





2018年05月22日 21:42:07[啧啧啧biubiu](https://me.csdn.net/qq_37385726)阅读数：109








![](https://img-blog.csdn.net/20180522213921477?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3FxXzM3Mzg1NzI2/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)








```cpp
#include<iostream>
using namespace std;
#include<queue>
#include<cstdio>

struct Node
{
	char op;
	int num;
	Node(char operation,int number=0)
	{
		op=operation;
		num=number;
	}
	bool operator < (const Node & a) const
    {
        return num>a.num;
    }
	
};


int main(void)
{
	int n;
	while(~scanf("%d",&n))
	{
		priority_queue<Node> pqueue;
		char tempch;
		int tempnum;
		for(int i=0;i<n;i++)
		{
			getchar();
			scanf("%c",&tempch);
			if(tempch=='Q')
			{
				printf("%d\n",pqueue.top().num);
			}
			else if(tempch=='D')
			{
				pqueue.pop();
			}
			else
			{
				scanf("%d",&tempnum);
				pqueue.push(Node(tempch,tempnum));
			}
		}
	}
	return 0;
}
```

#######转#######



## 优先队列的头文件&&声明

首先，你需要

```cpp
#include<queue>
using namespace std;
```
- 1
- 2

这两个头文件。

其次，一个优先队列声明的基本格式是： 
priority_queue<结构类型> 队列名;
比如：

```
priority_queue <int> i;
priority_queue <double> d;
```
- 1
- 2

不过，我们最为常用的是这几种：

```cpp
priority_queue <node> q;
//node是一个结构体
//结构体里重载了‘<’小于符号
priority_queue <int,vector<int>,greater<int> > q;
//不需要#include<vector>头文件
//注意后面两个“>”不要写在一起，“>>”是右移运算符
priority_queue <int,vector<int>,less<int> >q;
```
- 1
- 2
- 3
- 4
- 5
- 6
- 7

我们将在下文来讲讲这几种声明方式的不同。

## 优先队列的基本操作

与队列的基本操作如出一辙。 
如果想要了解请[点击这里](http://blog.csdn.net/c20182030/article/details/70171231)，看关于队列的介绍。

以一个名为q的优先队列为例。

```php
q.size();//返回q里元素个数
q.empty();//返回q是否为空，空则返回1，否则返回0
q.push(k);//在q的末尾插入k
q.pop();//删掉q的第一个元素
q.top();//返回q的第一个元素
q.back();//返回q的末尾元素
```
- 1
- 2
- 3
- 4
- 5
- 6

## 优先队列的特性

上文已经说过了，自动排序。 
怎么个排法呢？ 
在这里介绍一下：

### 默认的优先队列（非结构体结构）
`priority_queue <int> q;`- 1

这样的优先队列是怎样的？让我们写程序验证一下。

```
#include<cstdio>
#include<queue>
using namespace std;
priority_queue <int> q;
int main()
{
    q.push(10),q.push(8),q.push(12),q.push(14),q.push(6);
    while(!q.empty())
        printf("%d ",q.top()),q.pop();
}
```
- 1
- 2
- 3
- 4
- 5
- 6
- 7
- 8
- 9
- 10

程序大意就是在这个优先队列里依次插入10、8、12、14、6，再输出。 
结果是什么呢？ 
`14 12 10 8 6`
也就是说，它是按从大到小排序的！

### 默认的优先队列（结构体，重载小于）

先看看这个结构体是什么。

```
struct node
{
    int x,y;
    bool operator < (const node & a) const
    {
        return x<a.x;
    }
};
```
- 1
- 2
- 3
- 4
- 5
- 6
- 7
- 8

这个node结构体有两个成员，x和y，它的小于规则是x小者小。 
再来看看验证程序：

```
#include<cstdio>
#include<queue>
using namespace std;
struct node
{
    int x,y;
    bool operator < (const node & a) const
    {
        return x<a.x;
    }
}k;
priority_queue <node> q;
int main()
{
    k.x=10,k.y=100; q.push(k);
    k.x=12,k.y=60; q.push(k);
    k.x=14,k.y=40; q.push(k);
    k.x=6,k.y=80; q.push(k);
    k.x=8,k.y=20; q.push(k);
    while(!q.empty())
    {
        node m=q.top(); q.pop();
        printf("(%d,%d) ",m.x,m.y);
    }
}
```
- 1
- 2
- 3
- 4
- 5
- 6
- 7
- 8
- 9
- 10
- 11
- 12
- 13
- 14
- 15
- 16
- 17
- 18
- 19
- 20
- 21
- 22
- 23
- 24
- 25

程序大意就是插入(10,100),(12,60),(14,40),(6,20),(8,20)这五个node。 
再来看看它的输出： 
`(14,40) (12,60) (10,100) (8,20) (6,80)`

它也是按照重载后的小于规则，从大到小排序的。

### less和greater优先队列

还是以int为例，先来声明：

```cpp
priority_queue <int,vector<int>,less<int> > p;
priority_queue <int,vector<int>,greater<int> > q;
```
- 1
- 2

话不多说，上程序和结果：

```cpp
#include<cstdio>
#include<queue>
using namespace std;
priority_queue <int,vector<int>,less<int> > p;
priority_queue <int,vector<int>,greater<int> > q;
int a[5]={10,12,14,6,8};
int main()
{
    for(int i=0;i<5;i++)
        p.push(a[i]),q.push(a[i]);

    printf("less<int>:")
    while(!p.empty())
        printf("%d ",p.top()),p.pop();  

    pritntf("\ngreater<int>:")
    while(!q.empty())
        printf("%d ",q.top()),q.pop();
}
```
- 1
- 2
- 3
- 4
- 5
- 6
- 7
- 8
- 9
- 10
- 11
- 12
- 13
- 14
- 15
- 16
- 17
- 18
- 19

结果： 

```
less<int>:14 12 10 8 6 
greater<int>:6 8 10 12 14
```


所以，我们可以知道，less是从大到小，greater是从小到大。







