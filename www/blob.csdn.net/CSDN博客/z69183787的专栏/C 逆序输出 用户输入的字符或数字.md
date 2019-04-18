# C 逆序输出 用户输入的字符或数字 - z69183787的专栏 - CSDN博客
2012年10月29日 15:08:05[OkidoGreen](https://me.csdn.net/z69183787)阅读数：1207
个人分类：[其他语言-C++](https://blog.csdn.net/z69183787/article/category/2175767)
```cpp
#include <iostream>
using namespace std;
void main()
{
	//本程序中的以下数据没有涉及到写的权限,因此声明为只读权限
/*
	const int n[]={100,200,300,400,500};
	const int len_n=sizeof(n)/4;
	const int *pHead=&n[0];//保存数组的第一个元素的地址,指向数组的第一个元素
	const int *pTail=&n[len_n-1];//保存数组的最后一个元素的地址
	const int *p=NULL;//初始化指针为NULL
	cout<<*pHead<<endl;//100
	cout<<*pTail<<endl;//500
	//让指针指向最后一个元素
	p=pTail;
	cout<<*p<<endl;//500
	//把指针指向第一个元素
	++p;
	p=pHead;
	cout<<*p<<endl;//100
	//指针遍历元素,因为当前指针已指向pHead,因此我不再初始化指针p为pHead
	//分析:当前p指向pHead,即本程序中的元素"100",可以直接输出这个元素,
	//接着移动指针指向"200",输出"200"后,移动指针指向"300",...
	while((p-1)!=pTail)//100 200 300 400 500
	{
		cout<<*p<<" ";//输出当前指针所指向的元素
		++p;//往生移动指针指向下一个元素
	}
	cout<<endl;
	//当然,我们很容易就作逆序遍历
	//500 400 300 200 100
	p=pTail;
	while((p+1)!=pHead)
	{
		cout<<*p<<" ";
		--p;
	}
	cout<<endl;
*/
int i,k;
	i=0;
	char a[50],*p,*q;
	printf("输入字符串回车结束:\n");
	while(1)
	{ 
		scanf("%c",&a[i]);
		if(a[i]=='\n')
			break;
		i++;
	}
	k=i;
	cout<<k;
	printf("输入字符串为:\n");
	for(i=0;i<k;i++)
		printf("%c",a[i]);
	printf("\n");
	q = a+k-1;
//	cout<<*q;
	printf("交换后字符串为:");
	for(p=a,q=a+k-1;q>=p;q--){
		cout<<"ddddddd"<<endl;
		printf("%c",*q);
	}
	int j[10] = {1,2,3,4,5,6};
	int *pp = j;
	cout<<*(pp+1)<<endl<<sizeof(j)/sizeof(int);
}
```
