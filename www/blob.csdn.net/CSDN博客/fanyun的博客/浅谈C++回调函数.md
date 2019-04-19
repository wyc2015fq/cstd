# 浅谈C++回调函数 - fanyun的博客 - CSDN博客
2018年02月28日 22:09:30[老樊Lu码](https://me.csdn.net/fanyun_01)阅读数：3424
所属专栏：[C/C++/C#开发实战365](https://blog.csdn.net/column/details/c-plus-plus-01.html)
  1.什么是回调函数？
       回调函数就是一个通过函数指针调用的函数。如果你把函数的指针（地址）作为参数传递给另一个函数，当这个指针被用来调用其所指向的函数时，我们就说这是回调函数。回调函数不是由该函数的实现方直接调用，而是在特定的事件或条件发生时由另外的一方调用的，用于对该事件或条件进行响应。
      简单总结：什么是回调函数？——就是由声明函数的类来调用的函数叫做回调函数。普通函数可以让任何类调用。“回调”的主语是谁？——声明“回调函数”的那个类。Block、委托、通知、回调函数，它们虽然名字不一样，但是原理都一样，都是“回调机制”的思想的具体实现！
2.回调函数的机制
      (1)定义回调函数原型；(2)提供函数实现的一方在初始化的时候，将回调函数注册给调用者；(3)当特定的事件或条件发生的时候，调用者使用函数指针调用函数对事件进行处理。
3.回调函数的意义
       因为可以把调用者和被调用者分开，所以调用者不关心谁是被调用者。它只需知道存在一个具有特定原型和限制条件的被调用函数。简而言之，回调函数就是允许用户把需要调用的方法的指针作为参数传递给一个函数，以便该函数在处理相似事件的时候可以灵活的使用不同的方法。
4.回调函数的作用
       想知道回调函数在实际中有什么作用？先假设有这样一种情况：我们要编写一个库，它提供了某些排序算法的实现（如冒泡排序、快速排序、shell排序、shake排序等等），为了能让库更加通用，不想在函数中嵌入排序逻辑，而让使用者来实现相应的逻辑；或者，能让库可用于多种数据类型（int、float、string），此时，该怎么办呢？可以使用函数指针，并进行回调。
       回调可用于通知机制。例如，有时要在A程序中设置一个计时器，每到一定时间，A程序会得到相应的通知，但通知机制的实现者对A程序一无所知。那么，就需一个具有特定原型的函数指针进行回调，通知A程序事件已经发生。实际上，API使用一个回调函数SetTimer()来通知计时器。如果没有提供回调函数，它还会把一个消息发往程序的消息队列。
        再比如，假设有A、B两个类。
（1）A类有多种形态，要在B类中实现回调函数。如假设A类是网络请求开源类ASIHttpRequest，它可能请求成功，也可能请求失败。这个时候，B类就要针对以上两个情况，作不同的处理。
（2）A类的形态由B类决定时，要在B类中实现回调函数。如UITableView类就会提供很多回调函数（iOS专业术语称“委托”方法）
（3）A类需要向B类传递数据时，可以在B类中实现回调函数（A类一般是数据层比较耗时的操作类）。如举的那个发工资的例子。在实际编程中，这样的机制有个好处就是可以提升用户的操作体验。比如用户从X页面跳转到Y页面，需要向网络请求数据，而且比较耗时，那我们怎么办？有三种方案：第一种就是在X页面展示一个旋转指示器，当收到网络传回的数据时，在展现Y页面。第二种就是使用回调函数。用户从X页面直接跳转到Y页面，Y页面需要到数据让数据层去执行，当收到数据时，再在Y页面展现。第三种就是在Y页面中开启多线程。让一个子线程专门到后台去取数据。综合来说，第二种更加简介易懂，而且代码紧凑。
         不管怎么说，回调函数是继承自C语言的。在C++中，应只在与C代码建立接口或与已有的回调接口打交道时，才使用回调函数。除了上述情况，在C++中应使用虚方法或仿函数（functor），而不是回调函数。
5.代码示例
```cpp
typedef int(__stdcall*CompareFunction)(constbyte*,constbyte*）
```
     它就是回调函数的类型，负责用同样的参数形式将参数传递给相应的具体元素比较函数。另外，通过它，两个不同的排序算法，可以调用和具体元素相关的比较函数，实现和元素类型无关的排序：Bubblesort（）和Quicksort（），这两个方法都用同样的参数原型，但实现了不同的排序算法。
```cpp
void DLLDIR__stdcallBubblesort(byte*array,intsize,intelem_size,CompareFunctioncmpFunc);
void DLLDIR__stdcallQuicksort(byte*array,intsize,intelem_size,CompareFunctioncmpFunc);
```
这两个函数接受以下参数：
·byte * array：指向元素数组的指针（任意类型）。
·int size：数组中元素的个数。
·int elem_size：数组中一个元素的大小，以字节为单位。
·CompareFunction cmpFunc：带有上述原型的指向回调函数的指针。
这两个函数都会对数组进行某种排序，但每次都需决定两个元素哪个排在前面，而函数中有一个回调函数，其地址是作为一个参数传递进来的。对编写者来说，不必介意函数在何处实现，或它怎样被实现的，所需在意的只是两个用于比较的元素的地址，并返回以下的某个值（库的编写者和使用者都必须遵守这个约定）：
·-1：如果第一个元素较小，那它在已排序好的数组中，应该排在第二个元素前面。
·0：如果两个元素相等，那么它们的相对位置并不重要，在已排序好的数组中，谁在前面都无所谓。
·1：如果第一个元素较大，那在已排序好的数组中，它应该排第二个元素后面。
基于以上约定，函数Bubblesort（）的实现如下，Quicksort（）就稍微复杂一点：
```cpp
void DLLDIR__stdcall Bubblesort(byte*array,intsize,intelem_size,cmpFunc)
{
for(inti=0;i<size;i++)
{
for(intj=0;j<size-i-1;j++)
{
//回调比较函数
if(1==(*cmpFunc)(array+j*elem_size,array+(j+1)*elem_size))
{
//两个相比较的元素相交换
byte* temp=newbyte[elem_size];
memcpy(temp,array+j*elem_size,elem_size);
memcpy(array+j*elem_size,array+(j+1)*elem_size,elem_size);
memcpy(array+(j+1)*elem_size,temp,elem_size);
delete[]temp;
}
}
}
}
```
注意：因为实现中使用了memcpy（），所以函数在使用的数据类型方面，会有所局限。
对使用者来说，必须有一个回调函数，其地址要传递给Bubblesort（）函数。下面有二个简单的示例，一个比较两个整数，而另一个比较两个字符串：
```cpp
int__stdcall CompareInts(constbyte*velem1,constbyte*velem2)
{
int elem1=*(int*)velem1;
int elem2=*(int*)velem2;
if(elem1<elem2)
return-1;
if(elem1>elem2)
return1;
return0;
}
int __stdcall CompareStrings(constbyte*velem1,constbyte*velem2)
{
const char* elem1=(char*)velem1;
const char* elem2=(char*)velem2;
return strcmp(elem1,elem2);
}
```
下面另有一个程序，用于测试以上所有的代码，它传递了一个有5个元素的[数组](https://baike.baidu.com/item/%E6%95%B0%E7%BB%84)给Bubblesort（）和Quicksort（），同时还传递了一个指向回调函数的指针。（使用byte类型需包含头文件windows.h，或：
```cpp
typedef unsignedchar bute;
int main(intargc,char*argv[])
{
int i;
int array[]={5432,4321,3210,2109,1098};
cout<<"BeforesortingintswithBubblesort\n";
for(i=0;i<5;i++)
cout<<array[i]<<'\n';
Bubblesort((byte*)array,5,sizeof(array[0]),&CompareInts);
cout<<"Afterthesorting\n";
for(i=0;i<5;i++)
cout<<array[i]<<'\n';
const char str[5][10]={"estella","danielle","crissy","bo","angie"};
cout<<"BeforesortingstringswithQuicksort\n";
for(i=0;i<5;i++)
cout<<str[i]<<'\n';
Quicksort((byte*)str,5,10,&CompareStrings);
cout<<"Afterthesorting\n";
for(i=0;i<5;i++)
cout<<str[i]<<'\n';
return0;
}
```
如果想进行降序排序（大元素在先），就只需修改回调函数的代码，或使用另一个回调函数，这样编程起来灵活性就比较大了。
上面的代码中，可在函数原型中找到__stdcall，因为它以双下划线打头，所以它是一个特定于编译器的扩展，说到底也就是微软的实现。任何支持开发基于Win32的程序都必须支持这个扩展或其等价物。以__stdcall标识的函数使用了标准调用约定，为什么叫标准约定呢，因为所有的Win32 API（除了个别接受可变参数的除外）都使用它。标准调用约定的函数在它们返回到调用者之前，都会从堆栈中移除掉参数，这也是Pascal的标准约定。但在C/C++中，调用约定是调用者负责清理堆栈，而不是被调用函数；为强制函数使用C/C++调用约定，可使用__cdecl。另外，可变参数函数也使用C/C++调用约定。
Windows操作系统采用了标准调用约定（Pascal约定），因为其可减小代码的体积。这点对早期的Windows来说非常重要，因为那时它运行在只有640KB内存的电脑上。
如果你不喜欢__stdcall，还可以使用CALLBACK宏，它定义在windef.h中：
```cpp
#define CALLBACK__stdcallor
#define CALLBACKPASCAL//而PASCAL在此被#defined成__stdcall
```
作为回调函数的C++方法
因为平时很可能会使用到C++编写代码，也许会想到把回调函数写成类中的一个方法，但先来看看以下的代码：
```cpp
class CCallbackTester
{
public:
int CALLBACKCompareInts(constbyte*velem1,constbyte*velem2);
};
Bubblesort((byte*)array,5,sizeof(array[0]),&CCallbackTester::CompareInts);
```
如果使用微软的编译器，将会得到下面这个编译错误：
errorC2664:’Bubblesort’:cannotconvertparameter4from’int(__stdcallCCallbackTester::*)(constunsignedchar*,constunsigne）
这是因为非静态成员函数有一个额外的参数：this指针，这将迫使你在成员函数前面加上static。
6.使用回调函数的好处
（1）可以让实现方，根据回调方的多种形态进行不同的处理和操作。（ASIHttpRequest）
（2）可以让实现方，根据自己的需要定制回调方的不同形态。（UITableView）
（3）可以将耗时的操作隐藏在回调方，不影响实现方其它信息的展示。
（4）让代码的逻辑更加集中，更加易读。
