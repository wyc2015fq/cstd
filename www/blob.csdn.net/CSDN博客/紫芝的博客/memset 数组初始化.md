# memset 数组初始化 - 紫芝的博客 - CSDN博客





2018年04月07日 20:31:44[紫芝](https://me.csdn.net/qq_40507857)阅读数：937









```cpp
void *memset(void *s, int ch, size_t n)
```
1.功能是：


将s所指向的某一块内存中的前n个字节的内容全部设置为ch指定的ASCII值，
 第一个值为指定的内存地址，块的大小由第三个参数指定，


这个函数通常为新申请的内存做初始化工作，
 其返回值为指向s的指针。


2.注意：这边是对前n个字节进行赋值，而且还是ASCII的形式，
那就是说，对于字符数组，可以将其初始化为任意一个字符，
那对于其他基础类型的数组呢？


3.因为memset按字节对内存块进行初始化,

所以他无法对int数组初始化除了0和-1以外的其他数 




```cpp
#include<iostream>
#include<bits/stdc++.h>
using namespace std;
int main()
{
	int a[20],b[20],c[20],d[20];
	memset(a,0,sizeof(a));//riht,初始化为 0 
	memset(b,-1,sizeof(b));//right,初始化为 -1
	memset(c,1,sizeof(c));          //wrong   
    memset(d,2,sizeof(d));          //wrong  
	
	cout<<"a:"<<endl;  
    for(int i=0;i<20;i++) cout<<a[i]<<endl;
    cout<<endl<<endl;
    
	cout<<"b:"<<endl;  
    for(int i=0;i<20;i++) cout<<b[i]<<endl; 
	cout<<endl<<endl;
	 
    cout<<"c:wrong"<<endl; 
    for(int i=0;i<20;i++) cout<<c[i]<<endl;
	cout<<endl<<endl;
	 
	cout<<"d:wrong"<<endl;  
    for(int i=0;i<20;i++) cout<<d[i]<<endl; 
    cout<<endl<<endl;


    return 0;  
}
```






