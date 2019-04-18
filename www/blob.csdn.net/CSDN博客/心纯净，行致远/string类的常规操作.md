# string类的常规操作 - 心纯净，行致远 - CSDN博客





2018年12月03日 10:02:54[吉大秦少游](https://me.csdn.net/zhanshen112)阅读数：42








# 1、定义

string数组的定义有三种：
`String arr[] = new String[10]; //创建一个长度为10的String 类型数组。`

`String arr[] = {"张三","李四"};`

`String[] arr = new String[10];`
# 2、求string类数组的长度（元素个数）

**sizeof(数组名) / sizeof(数组名[0])**

**整个数组的长度/第一个元素的长度(长度是固定的)**

**sizeof其作用是返回一个对象或类型所占的内存字节数，在windows下string是28**

例子

string A[] = { "Hello "};

sizeof(A)的结果为28，sizeof(A[0])的结果为28

string A[] = { "Hello " , "Hello World"};

sizeof(A)的结果为56，sizeof(A[0])的结果为28

string A[] = { "qwertyuiopasdfghjklzxcvbnmqwertyuiopasdfghjklzxcvbnm" , "Hello World","Hello"};

sizeof(A)的结果为84，sizeof(A[0])的结果还是为28(字符数量超过28)


# 3、获取C/C++字符串、字符数组长度

**在C/C++中常用的获取字符串长度或者字符串数组长度的函数有sizeof()length()strlen()size()**

其中strlen(str)和str.length()和str.size()都可以用来求字符串的长度

str.length()和str.size()是用于求string类对象的成员函数

strlen(str) 是用于求字符串数组的长度，其参数是char*

辨析strlen() 与 sizeof() 的区别
**strlen(char*)**

函数求是字符串的实际长度，它可以用来获取动态实际字符数组的长度，是从开始到遇到第一个“\0”，如果只是定义没有赋予初始值，这个结果是不确定的，它会从数组的首地址开始一直找下去，直到遇到“\0”停止查找。
**sizeof()**

求所占总空间的字节数，静态的，跟初始状态字符数组的大小有关系，大小等于初始时字符数组的大小或者等于初始时字符数组的大小+1

在C++中，如果定义的是字符串数组的话，那么如果想获取数组的长度，只能用sizeof(数组名)，而不能用strlen(str)
**example**

```cpp
char str[20]="0123456789";
int   a=strlen(str); //a=10;strlen 计算字符串的长度，以\0'为字符串结束标记。
int   b=sizeof(str); //b=20;sizeof 计算的则是分配的数组str[20] 所占的内存空间的大小，不受里面存储的内容影响
```



```cpp
char *str1="absde";
char str2[]="absde";
char str3[8]={'a',};
char ss[] = "0123456789";

输出：

sizeof(str1)=4;
sizeof(str2)=6;
sizeof(str3)=8;
sizeof(ss)=11
```

 首先说明一点，char类型占一个字节，所以sizeof(char)是1，这点要理解

str1是一个指针，只是指向了字符串”absde”而已。所以sizeof(str1)不是字符串占的空间也不是字符数组占的空间，而是一个字符型指针占的空间。所以sizeof(str1)=sizeof(char*)=4，在C/C++中一个指针占4个字节

str2是一个字符型数组。C/C++规定，对于一个数组，返回这个数组占的总空间，所以sizeof(str2)取得的是字符串”absde”占的总空间。”absde”中，共有a b s d e \0六个字符，所以str2数组的长度是6，所以sizeof(str2)=6*sizeof(char)=6

str3已经定义成了长度是8的数组，所以sizeof(str3)为8

str4和str2类似，’0’ ‘1’ … ‘9’加上’\0’共11个字符，所以ss占的空间是8

总之，对于指针，sizeof操作符返回这个指针占的空间，一般是4个字节；而对于一个数组，sizeof返回这个数组所有元素占的总空间。char*与char[]容易混淆，一定要分清，而且char*=”aaa”的写法现在不被提倡，应予以避免

**而strlen不区分是数组还是指针，就读到\0为止返回长度。而且strlen是不把\0计入字符串的长度的。**

# **一、sizeof**

> 
sizeof(...)是运算符，在头文件中typedef为unsigned int，其值在编译时即计算好了，参数可以是数组、指针、类型、对象、函数等。



它的功能是：获得保证能容纳实现所建立的最大对象的字节大小。



由于在编译时计算，因此sizeof不能用来返回动态分配的内存空间的大小。实际上，用sizeof来返回类型以及静态分配的对象、结构或数组所占的空间，返回值跟对象、结构、数组所存储的内容没有关系。



具体而言，当参数分别如下时，sizeof返回的值表示的含义如下：
***数组——编译时分配的数组空间大小；指针——存储该指针所用的空间大小（存储该指针的地址的长度，是长整型，应该为4）；类型——该类型所占的空间大小；对象——对象的实际占用空间大小；函数——函数的返回类型所占的空间大小。函数的返回类型不能是void。***


# 二、strlen

> 
strlen(...)是函数，要在运行时才能计算。参数必须是字符型指针（char*）。当数组名作为参数传入时，实际上数组就退化成指针了。

它的功能是：返回字符串的长度。该字符串可能是自己定义的，也可能是内存中随机的，该函数实际完成的功能是从代表该字符串的第一个地址开始遍历，直到遇到结束符NULL。返回的长度大小不包括NULL。


# 三、举例：

> 
eg1、char arr[10] = "What?";

          int len_one = strlen(arr);

          int len_two = sizeof(arr);

          cout << len_one << " and " << len_two << endl;

输出结果为：5 and 10

点评：sizeof返回定义arr数组时，编译器为其分配的数组空间大小，不关心里面存了多少数据。strlen只关心存储的数据内容，不关心空间的大小和类型。

eg2、char * parr = new char[10];

          int len_one = strlen(parr);

          int len_two = sizeof(parr);

          int len_three = sizeof(*parr);

          cout << len_one << " and " << len_two << " and " << len_three << endl;

输出结果：23 and 4 and 1

点评：第一个输出结果23实际上每次运行可能不一样，这取决于parr里面存了什么（从parr[0]开始知道遇到第一个NULL结束）；第二个结果实际上本意是想计算parr所指向的动态内存空间的大小，但是事与愿违，sizeof认为parr是个字符指针，因此返回的是该指针所占的空间（指针的存储用的是长整型，所以为4）;第三个结果，由于*parr所代表的是parr所指的地址空间存放的字符，所以长度为1。


## string 类的常见应用

可以参考该链接，获取更为详细的介绍

C++使用内置的数据类型string来处理字符串会很方便，可以完全代替C语言中的char数组和char指针。

使用string类需要包含头文件string，同时string 类型的变量结尾没有“\0”，其本质是一个string类，因此可以通过调用string类中成员函数length()函数，如：

> 
string s = "hello world";

int len = s.lenght();

cout<<len<<endl;


运行结果为：11

由于string变量的末尾没有“\0”字符，所以length()返回的是字符串的真实长度，而不是长度+1

在C++中可以用string类来代替C中的char数组形式的字符串，但是有些情况下是需要转换成C语言风格的，如以下代码：

> 
#include <stdio.h>

#include <math.h>

#define MAX 51

int main(){

    char s1 [MAX];

    char s2 [MAX];

    scanf("%s%s",s1,s2);

    int len = strlen(s1);

    for (int i=0;i<len;i++){

        if(s1[i]==s2[i])

            printf("1");

        else

            printf("0");

    }

    printf("\n");

    return 0;

}


这里需要一个个的访问实际数组中的元素，当时如果利用C++中的sizeof函数时，获取的是数组的固定空间，而不是实际的动态空间。所以这里只能用strlen（str）来获取char数组的实际长度（也即是用户输入的字符串的长度），如果用sizeof（str）所得到的长度大小为51。

string类型的字符串转换成char数组字符串

> 
string filename = "test.txt";

ifstream in ;

in.open(filename.c_str());


为了使用文件打开函数，必须将string类型的变量转换为字符串数组

> 
#ifndef _ITERATOR_DEBUG_LEVEL

#define _ITERATOR_DEBUG_LEVEL 0

#else

#undef _ITERATOR_DEBUG_LEVEL

#define _ITERATOR_DEBUG_LEVEL 0

#endif

#include <iostream>

#include <string>

//string字符串->c字符串转换

void main()

{

    //string -> char*

    string s1 = "far away";

    const char* c = s1.c_str();

    printf("%s\n",c);

    //

    string s2 = c;

    //string->char[]

    //从string中赋值字符到char[]

    char arr[50] = {0};

    s1.copy(arr,4,0);

    cout << arr << endl;

    system("pause");

}


输出结果：

far away

far

请按任意键继续…

## string字符串的输入输出

string类重载了输入输出的运算符，用”>>”进行输入，用”<<”进行输出

> 
#include <iostream>

#include <string>

using namespace std;

int main(){

    string s;

    cin >> s;  //输入字符串

    cout << s << endl;  //输出字符串

    system("pause");

    return 0;

}


运行结果：

c plus

c

请按任意键继续…



虽然我们输入了两个由空格隔开的”c plus’,但是只输出了一个，这是因为输入的运算符“>>”默认会忽略空格，遇到空格就认为输入结束，所以最后输入的plus没有被存储到变量里面。

## 访问字符串中的字符

string 字符串也可以像字符串数组一样按照下标来访问其中的每一个字符。string 字符串的起始下标仍是从 0 开始。请看下面的代码：

> 
#include <string>

using namespace std;

//string遍历

void main(){

    string s1 = "abcdefg";

    //1、数组方式

    cout << "数组方式:" << endl;

    for (int i = 0; i < s1.length(); i++){

        cout <<s1[i] << endl;

    }

    //2、迭代方式

    cout << "迭代方式:" << endl;

    for (string::iterator it = s1.begin(); it != s1.end(); it++){

        cout<<*it<< endl;

    }

    system("pause");

}


输出结果：

数组方式:

a

b

c

d

e

f

g

迭代方式:

a

b

c

d

e

f

g

请按任意键继续…

## 字符串的拼接

有了string 类，我们可以使用”+“ 或者”+=“运算符来直接拼接字符串，非常方便，再也不需要使用C语言中的strcat(),strcopy(),malloc()等函数来拼接字符串了，再也不用担心空间不够溢出了。

下面来看列子：

> 
//字符串拼接

void main()

{

    string s1 = "alan";

    string s2 = "xiho";

    //1.

    string s3 = s1 + s2;

    string s4 = " wahaha";

    //2.

    s3.append(s4);

    cout << s3 << endl;

    system("pause");

}


输出结果为：

alanxiho wahaha

请按任意键继续…

## 四、参考资料：

Sizeof与Strlen的区别与联系(转）

1.sizeof操作符的结果类型是size_t，它在头文件中typedef为unsigned int类型。

该类型保证能容纳实现所建立的最大对象的字节大小。

2.sizeof是算符，strlen是函数。

3.sizeof可以用类型做参数，strlen只能用char*做参数，且必须是以”\0”结尾的。

sizeof还可以用函数做参数，比如：

short f();

printf(“%d\n”, sizeof(f()));

输出的结果是sizeof(short)，即2。

4.数组做sizeof的参数不退化，传递给strlen就退化为指针了。

> 
5.大部分编译程序 在编译的时候就把sizeof计算过了 是类型或是变量的长度这就是sizeof(x)可以用来定义数组维数的原因

char str[20]=”0123456789”;

int a=strlen(str); //a=10;

int b=sizeof(str); //而b=20;


6.strlen的结果要在运行的时候才能计算出来，时用来计算字符串的长度，不是类型占内存的大小。

7.sizeof后如果是类型必须加括弧，如果是变量名可以不加括弧。这是因为sizeof是个操作符不是个函数。

8.当适用了于一个结构类型时或变量， sizeof 返回实际的大小，

当适用一静态地空间数组， sizeof 归还全部数组的尺寸。

sizeof 操作符不能返回动态地被分派了的数组或外部的数组的尺寸

9.数组作为参数传给函数时传的是指针而不是数组，传递的是数组的首地址，

如：

fun(char [8])

fun(char [])

都等价于 fun(char *)

在C++里参数传递数组永远都是传递指向数组首元素的指针，编译器不知道数组的大小

如果想在函数内知道数组的大小， 需要这样做：

进入函数后用memcpy拷贝出来，长度由另一个形参传进去

> 
fun(unsiged char *p1, int len)

{

unsigned char* buf = new unsigned char[len+1]

memcpy(buf, p1, len);

}


我们能常在用到 sizeof 和 strlen 的时候，通常是计算字符串数组的长度

看了上面的详细解释，发现两者的使用还是有区别的，从这个例子可以看得很清楚：

> 
char str[20]=”0123456789”;

int a=strlen(str); //a=10; >>>> strlen 计算字符串的长度，以结束符 0x00 为字符串结束。

int b=sizeof(str); //而b=20; >>>> sizeof 计算的则是分配的数组 str[20] 所占的内存空间的大小，不受里面存储的内容改变。


上面是对静态数组处理的结果，如果是对指针，结果就不一样了

char* ss = “0123456789”;

sizeof(ss) 结果 4 ＝＝＝》ss是指向字符串常量的字符指针，sizeof 获得的是一个指针的之所占的空间,应该是长整型的，所以是4

sizeof(*ss) 结果 1 ＝＝＝》*ss是第一个字符 其实就是获得了字符串的第一位’0’ 所占的内存空间，是char类型的，占了 1 位

strlen(ss)= 10 >>>> 如果要获得这个字符串的长度，则一定要使用 strlen



