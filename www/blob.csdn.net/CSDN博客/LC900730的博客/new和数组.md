# new和数组 - LC900730的博客 - CSDN博客
2017年09月26日 17:41:37[lc900730](https://me.csdn.net/LC900730)阅读数：785
## new分配数组
```
int *pia=new int[get_size()];
//pia指向第一个int
//【】必须是整形，但是不必是常量
```
```
typedef int arrT[42];
    int *p=new arrT; //分配一个42个int型的数组
```
虽然称使用new T[]分配的内存为“动态数组”，但是这种叫法在某种程度上有误导。当使用new分配一个数组时候，我们并未得到数组类型的对象，而是得到一个数组元素类型的指针。new返回的是一个元素类型的指针。 
不能对动态数组使用begin、end，因为这些动态数组使用数组维度。
### 初始化方式
```
int *pia=new int[10];
int *pia2=new int[10]();
string *psa=new string[10];
string *psa2=new string[10]();
```
在新标准中，我们还可以使用元素初始化器的花括号列表：
```
//10个int分别用列表中对应的初始化器初始化
int *pia3=new int[10]{0,1,2,3,4,5,6,7,8,9};
//10个string,前4个给定初始化器初始化，剩余值初始化。
string *psa3=new string[10]{"a","an","the",string(3,'x')};
```
### 释放动态数组
```java
delete p;     //p指向一个动态分配的对象或为空
//数组元素逆序销毁，即最后一个先销毁，然后倒数第二个
delete [] pa;  //pa必须指向一个动态分配的数组或者为空
```
必须使用[]; 
在类型别名中定义一个数组类型，不需要[].但是在释放时候必须使用[]
```
typedef int arrT[42];
int *p =new arrT;
delete [] p;
```
### unique_ptr
可以管理new分配的数组的unique_ptr。 
为了使用一个unique_ptr管理动态数组，我们必须在对象类型之后跟一对方括号。
```
//up指向一个保护10个未初始化int的数组
//int[]指明指向一个int数组
unique_ptr<int []>up(new int [10]);
up.release();   //自动用delete[]销毁其指针
```
当unique_ptr指向的是数组时候，我们不能使用点和箭头。因为指向的是数组而不是单个对象。
```matlab
for(size_t i=0;i!=10;i++){
    up[i]=i;
}
```
