# 彻底搞清C/C++中一维数组，二维数组，指针，数组指针和指针数组以及指向指针的指针，行地址和列地址之间的关系 - 文章 - 伯乐在线
原文出处： [zhangyulin54321](http://blog.csdn.net/zhangyulin54321/article/details/7843531)
如果配合http://wenku.baidu.com/view/acf838ef856a561252d36fe3.html去看的话下面的内容比较容易理解
从一道题目说起。


```
#include <iostream>
using namespace std;
int main()
{
int ia[3][4]={0,1,2,3,4,5,6,7,8,9,10,11};
typedef int int_array[4];
for (int_array *p =ia;p!= ia+3;++p)
{
for (int *q=*p;q!=*p+4;++q)
{
cout<<*q<<" ";
}
cout<<endl;
}
return 0;
}
```
#### 一、先解释 typedef int int_array[4];
关键字typedef用来自定义数据类型，这是所有教材都这样讲的，但不要理解为新创建了一个数据类型，而是将已有的一个类型赋予个新名称而已，即起一个别名。
具体对这个语句来说，别名就是：int_array。而[4]不属于名字，而表示一种已有的数据类型，即：给一个大小为4的int数组取一个别名为int_array。
那如何知道是这样定义的呢？很简单。
首先，int a[4];这可是常见的定义格式。再在其前面添加关键字typedef，变成 typedef int a[4];最后将数组名a改为自己想要的一个别名int_array即可。注意：原本的a本意是数组名，属于变量范畴，而int_array则是新数据类型名（即别名），本质不一样了哦。祥见谭浩强的那本经典教材。
#### 二、语句 int_array *p =ia; 的含义
首先，它是一个定义语句，即用自定义的数据类型int_array来定义一个该类型的一个指针变量ia。
ia是一个什么东东呢？它是一个二维数组名。
对于一维数组a，我们有：&a[0]等价于a，即都表示该一维数组首元地址。
那么，对于二维数组这个性质还成立吗？没错，同样成立。即：
&ia[0]等价于ia。
所以，int_array *p =ia;与int_array *p =&ia[0];是等价的。
但是，&a[0]与&ia[0]含义是不一样的。前者代表了一维数组中首元地址，而后者则代表二维数组中第一行的行首地址。
行首地址与行首元地址，它们的值用cout输出来那肯定是一样的。但它们与指针的操作扯上关系时，就不一样了，前者以行为基本单位，后者以一个元素为基本单位，切记。
现在我们应该明白了，语句int_array *p =ia;的作用是定义p后，并初始化p，即用p来指向二维数组的第一行（整个这一行），即ia[0]，不是第一行的首元ia[0][0]哦。当然，对p这样初始化是正确的，因为p要指向的正是大小为4的一维数组，而二维数组ia的每一行正好就是4个元素。
ia[0]可认为是首行的数组名。ia[1]、ia[2]类推。
#### 三、语句 ++p 的含义
由上可知，p既然首先指向第一行ia[0]，那么（p+1）不就指向第二行ia[1]吗？正是如此。
于是外循环的终止条件就应该是不存在的第四行，即ia[3]，所以终止条件就是：p!= ia+3。
#### 四、语句 int *q=*p; 的含义
如上所述，先定义整型指针q，并初始化为*p。
*p是什么意思？
前面已得到：p 被初始化为ia，即&ia[0]。那么*p就代表ia[0]。
即：p存储的是首行地址，于是，*p就直接代表了该行，即整个这一行。
而前面已经明确的讲了ia[0]代表的是首行的数组名，当然它是一维的。而一维数组名不就代表了这一行的首元地址吗？于是，就有
q=*p等价于q=ia[0]，也等价于q=&ia[0][0]。
再于是，q指向了一个一维数组的首元。切记，不能说q指向了一个一维数组。再再于是，++q就表示&ia[0][1]。当然了，再执行一遍++q，就表示&ia[0][2]了。
再解释一下*p+4。
刚刚讲了*p直接代表了某一行，即ia[0]或ia[1]或ia[2]，也讲了这些ia[0]或ia[1]或ia[2]就代表该行的行数组名。当然，都是一维的。
回忆一下，一维数组名加一个数字代表什么呢？例如a是一个一维数组名，a+4表示什么呢？答案是：&a[4]，即该一维数组的第五个元素a[4]的地址。
所以*p+4表示：p所指那一行的第五个元素，当然这对于本题来说是不存在的，所以就做为内循环的终止条件咯。
*q代表什么呢？
q指向的是一个具体的元素，那么*q就直接代表了该元素的内存空间。那么，cout它就是输出该元素的值。
为了能更好地理解数组指针，与普通指针及二级指针的区别，下面举例说明一下。
例如：{int a[4][5];int (*p)[5]=a;}这里a是个二维数组的数组名，相当于一个二级指针常量；p是一个指针变量，它指向包含5个int元素的一维数组，此时p的增量以一维数组长度为单位；p+i是二维数组a的i行的起始地址，*(p+2)+3表示a数组2行3列元素地址（第一行为0行，第一列为0列），*（*(p+2)+3）表示a[2][3]的值。
#### 再看一道题
一道面试题引发的问题，首先要知道[]的优先级高于*，题目：
char **p,a[6][8]; 问p=a是否会导致程序在以后出现问题？为什么？
直接用程序说明：


```
#include<stdio.h>
void main()
{
char **p,a[6][8];
p = a;
printf（"\n"）;
}
```
编译，然后就会发现通不过，报错：错误 1 error C2440: “=”: 无法从“char [6][8]”转换为“char **”
于是乎，我看了下《C专家编程》里10.5节—使用指针向函数传递一个多维数组。
方法一，函数是 void fun(int arr[2][3]); 这种方法只能处理2行3列的int型数组。
方法二，可以省略第一维的长度。函数是 void fun(int arr[][3]);这种方式虽然限制宽松了一些，但是还是只能处理每行是3个整数长度的数组或者写成这种形式 void fun(int (*arr)[3]);这是一个数组指针或者叫行指针，arr和*先结合使得arr成为一个指针，这个指针指向具有3个int类型数据的数组。
方法三，创建一个一维数组，数组中的元素是指向其他东西的指针，也即二级指针。函数是 int fun(int **arr);这种方法可以动态处理各行各列不一样长度的数据。
注意：只有把二维数组改成一个指向向量的指针数组的前提下才可以这么做！比如下面的程序可以正常输出abc：
注意：数组指针又叫指向一维数组的指针，数据类型 (*指针变量)[一维数组维数] eg:int (*p)[4];指向指针的指针(二级指针) eg int **pp;
pp就是行地址，*pp就是列地址;pp+1还是行地址，*pp+1还是列地址。


```
#include <iostream> 
using namespace std; 
void test(char **ptr) 
{ 
for(;(strcmp(*ptr,"NULL"))!=0;ptr=ptr+1)
cout << *(ptr) << endl; 
} 
int main() 
{ 
char *p[] = {"abcsdad", "def", "ghiffaghjjkjkkk","NULL"}; 
test(p); 
return 0; 
}
```
又如:
///////////////!!!!!!!!!!!!!!!!!!!!!!!!!!!!二维数组访问方式如下!!!!!!!////////////////////////////////////////////////////
（1） 方法一


```
int b[3][3] ={{1,2,3},{4,5,6},{7,8,9}};
int j,k,(*q)[3];
q=b;
for(j=0; j<3; j++)
{
for(k=0; k<3; k++)
{
//cout << *(*(b+j)+k) << " ";
cout << *(*(q+j)+k) << " ";
}
}
cout << endl;
char name[][17]={"Follow me","BASIC","Great Wall","FORTRAN","Computer desighn","ok"};
char (*p)[17];
for(p=name;strcmp(*p,"ok")!=0;p++)
{
cout<<*p<<endl;
}
```
（2）方法二


```
char *name[]={"Follow me","BASIC","Great Wall","FORTRAN","Computer desighn"}; 
char **p; 
int i; 
for(i=0;i<5;i++) 
{p=name+i; 
printf("%s\n",*p); 
}
```
还可以改改：


```
char *name[]={"Follow me","BASIC","Great Wall","FORTRAN","Computer desighn"}; 
char **p; 
int i; 
for(i=0;i<sizeof(name)/4;i++) 
{p=name+i; 
//name+i是行指针，name[i]是列指针，&name[i]是行指针,而p本身也是行指针。
//等价于p=&name[i];
//cout<<sizeof(name)<<endl;
//cout<<(name[i])<<endl;//输出的结果和*p一样，但表示的本质不一样，name[i]本身就是列指针，p是行指针，*p后就变成了列指针
printf("%s\n",*p); 
}
```
与下面的比较一下


```
#include<iostream>
#include<string.h>
#include<stdio.h>
using namespace std;
#define dim(x) sizeof(x) / sizeof(x[0])
void main()
{
char name[][17] = {
"Follow me",
"BASIC",
"Great Wall",
"FORTRAN",
"Computer desighn",
};
char (*p)[17] = name;
int nCount = dim(name);
while (nCount-- > 0)
{
cout << *p++ << endl;
}
}
```
结果都是一样的，但我们最好习惯用char*name[]这样指针数组的定义，因为char name[][17]虽然行可以任意，但列却不能任意。只有指针数组才能实现任意的行和列。
还比较一下对int 型的二维数组的输出：


```
#include<iostream>
#include<string.h>
#include<stdio.h>
using namespace std;
#define dim(x) sizeof(x) / sizeof(x[0])
#define dam(x) sizeof(x) / sizeof(int)
void main()
{
int aArray[]={1,2,3,4};
int bArray[]={5,6,7,8,9,10};
int cArray[]={11,12,14,15,16,17,18,19,20,21,22};
int *name[] = {
aArray,
bArray,
cArray,
};
int **p = name;
int aColum=dam(aArray);
int bColum=dam(bArray);
int cColum=dam(cArray);
int nCount=sizeof(name)/4;
int i ,flag;
for(i=0;i<nCount;i++)
{ int *j=name[i];
flag=i;
switch(flag)
{
case 0:
for(;j<name[i]+aColum;j++)//name[i]列指针，而name+i是行指针,而int *指向的是列指针，而name[i]+j还是列指针
cout<<*j<<" ";
cout<<endl;
break;
case 1:
for(;j<name[i]+bColum;j++)
cout<<*j<<" ";
cout<<endl;
break;
case 2:
for(;j<name[i]+cColum;j++)
cout<<*j<<" ";
cout<<endl;
break;
}
}
}
```
（3）方法三
用一维指针遍历二维数组


```
int a[3][4]={0,1,2,3,4,5,6,7,8,9,10,11};
int *p;
for(p=a[0];p<a[0]+12;p++)
cout<<*p<<endl;
```
看看 对一维数组的访问


```
int aq[12]={1,2,3,4,5};
int (*p)[12];//数组指针也即行地址
p=&aq;//行地址
cout<<p<<endl;
cout<<*p<<endl;
for(int i=0;i<5;i++)
cout<<*(p[0]+i)<<endl;//注意p是行地址，套上[]这个就是列地址了p[i]
```
与下面的比较


```
int aq[12]={1,2,3,4,5};
int *p;//列地址
p=aq;
for(int i=0;i<5;i++)
cout<<*(p+i)<<endl;
```
也可定义这样的，只要记住指针数组中的内容是指针即地址就行


```
int a[]={12,12,34,5};
int b[]={13,17};
int c[]={123,56,4};
int *ia[3]={a,b,c};
```
在《C专家编程》10.3节的小启发里讲的很透彻：（以下这段文字及对比一定要认真分析！）
很有用的二维数组知识，看看二维数组！！！！！！！！！！！！！！！！！


```
int a[3][4]={0,1,2,3,4,5,6,7,8,9,10,11};
cout<<a+1<<endl; 
cout<<&a[1]<<endl;
cout<<a[1]<<endl;
cout<<*(a+1)<<endl;
cout<<&a[1][0]<<endl;
```
这五句输出的结果值是一样的，都是地址值，但是表示的含义不一样。
所以我总结的二维数组一些性质：
a+1表示行地址 而前面加了个*号就表示列地址了 如*（a+1）。
a[1]表示列地址，前面加个&号就表示行地址了，如&a[1].
&a[1][0]表示列地址，注意当* & [] 这三个符号组合出现的次数为偶数次的话就表示真正的内容值，出现奇数次就表示该内容的地址。
但是请注意& 和()不能组合到一起，所以&（a+1）是错误的。
行地址前加*就表示列地址了，列地址前加*号就表示取内容值，而列地址前加&表示行地址了。所以只有行地址前加*号才能变列地址，加&报错，而列地址前加&变行地址，
加*就取得值了，所以对二维数组来说只有列地址前加*号，才能得到内容。如果是行地址，这先要转化为列地址，才能得内容。而行地址套上[]符号就变成列地址了如
a->行地址 a[0]->列地址.总之一点只有当成为列地址后再在前面加*才能取到真正的内容.
看看如下内容：


```
#include<iostream>
#include<string.h>
#include<stdio.h>
using namespace std;
void main()
{
char name[][17] = {
"Follow me",
"BASIC",
"Great Wall",
"FORTRAN",
"Computer desighn",
"ok"};
char (*p)[17] = name;
for(;(**p)!='\0';p++)
cout<<*p<<endl;
}
```
对这个的改进，不要输出最后的乱码


```
#include<iostream>
#include<string.h>
#include<stdio.h>
using namespace std;
#define dim(x) sizeof(x) / sizeof(x[0])
void main()
{
char name[][17] = {
"Follow me",
"BASIC",
"Great Wall",
"FORTRAN",
"Computer desighn",
"ok"};
char (*p)[17] = name;
int nCount = dim(name);
while (nCount-- > 0)
{
cout << *p++ << endl;
}
}
```
就不会输出最后的乱码了，哈哈哈哈！！！！！！！！！！！！！！！！！！！！！！
再看看输出int的二维数组


```
#include<iostream>
#include<string.h>
#include<stdio.h>
using namespace std;
#define dim(x) sizeof(x) / sizeof(x[0])
void main()
{
int name[][4] = {
{1,2,3,4},
{5,6,7,8},
{9,10,11,12},
};
int (*p)[4] = name;
int nRow = dim(name);//3行
int nColum=(sizeof(name[0])/sizeof(int));//4列
for (int j=0;j<nRow;j++)
{for(int i=0;i<nColum;i++)
cout << *(*(p+j)+i);//因为p是数组指针，是行指针，而p+j还是行指针，*p后就是列指针，而*p+i还是列指针
cout<<endl;
}
}
```
数组和指针参数是如何被编译器修改的？
“数组名被改写成一个指针参数”规则并不是递归定义的。数组的数组会被改写成“数组的指针”，而不是“指针的指针”：
实参 所匹配的形参
数组的数组 char c[8][10]; char (*)[10]; 数组指针
指针数组 char *c[10]; char **c; 指针的指针
数组指针(行指针) char (*c)[10]; char (*c)[10]; 不改变
指针的指针 char **c; char **c; 不改变
下面再看一个网友的一段分析相当给力的代码：


```
#include "stdafx.h" 
#include <iostream> 
using namespace std; 
int _tmain(int argc, _TCHAR* argv[]) 
{ 
int arr1[3]; 
int arr2[3]; 
int arr3[3]; 
int * ptr; 
// ptr1是一个指向 int [3] 的指针，即ptr的类型和&arr1的类型是一样的，注意：arr1指向的内存区域定长 
int ptr1[3][3]={{1,2,3},{1,2,3},{1,2,3}}; 
// ptr2是一个指向 int * 的指针，即ptr2的类型和&ptr是一样的，注意：ptr指向的内存区域不定长 
int * ptr2[3]={arr1,arr2,arr3}; 
// ptr3是一个指向 int [3] 的指针，即ptr3的类型和&arr1的类型是一样的，注意：arr1指向的内存区域定长 
int(* ptr3)[3]=&arr1; 
ptr3=ptr1; // 没错，他们的类型相同 
// ptr3=ptr2;//error 无法从“int *[3]”转换为“int (*)[3] 
// ptr4是一个指向 int * 的指针，即ptr4的类型和&ptr是一样的，注意：ptr指向的内存区域不定长 
int ** ptr4; 
//ptr4=&arr1; //error 无法从“int (*)[3]”转换为“int ** 
ptr4=ptr2; // 没错，他们的类型相同 
//ptr4=ptr3; // error 无法从“int (*)[3]”转换为“int ** 
return 0; 
}
```
再看一个有意思的，看看对指针的掌握情况，“行地址”和“列地址”的区别
记住 一维数组的数组名是列地址，二维数组名是行地址，虽然数组名都是地址，但本质不一样，行地址始终要转化为列地址，才能得到内容值
转化规则：行->列：在行前加*号，列->转化->行，在列前加&。


```
int a[]={12,34,54};
int aa[][3]={{13,4,5},{5,6,3}};
int (*pp)[3];//pp为行地址，如果是int **rr；则rr为列地址
int **rr;
pp=a;//为什么这样不可以，因为一维数组名为列地址，而pp为行地址，本质不一样，按照上面的转化规则，加上&就行
pp=&a;//为什么这样就行
pp=aa;//都是行地址
pp=&aa;//错误，为什么这样就不行,不能从int(*)[2][3]转化为int(*)[3]
pp=&aa[0];//为什么这样可以，aa[0]是列地址加上&就是行地址
```
再看下面


```
int a[]={12,34,54};
int *r=a;//int *为列指针，
int **rr;//int **好像为行指针，但是如 rr=&a不行，所以rr不是行地址
rr=&r;
for(int i=0;i<sizeof(a)/4;i++)
cout<<*(*rr+i)<<endl;//*rr就等于r
```
#### 五、总结
数组指针和二维数组有关，你可以到定义了数组指针后将二维数组名赋值给他；指针数组和指向指针的指针有关，你可以将两者赋值。
