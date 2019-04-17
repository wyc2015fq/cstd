# linux C编程（三）ANSI C中的类型限定词（const/volatile/restrict） - BryantLJ学习的地方 - CSDN博客





2016年08月24日 22:40:12[遍地流金](https://me.csdn.net/u012177034)阅读数：679








由第二节我们知道，一个变量有两种属性来描述，一种为数据类型（如int，char等），一种为存储属性（包括作用域，链接属性，生存周期 属性，由static和变量声明的地方决定）。C90标准增加了两个属性：不变性和易变性，C99标准增加了1个属性：restrict属性，用于编译器优化。 

另外，C99中规定类型限定符是幂等的，也即意味着**可以在一个声明中不止一次的使用同意限定词，多余的将被忽略**

如下面两种使用方式都是合理的：

```
const const const int n = 6;//合理

typedef const int zip;
const zip q = 8;//也是合理
```

本片文章着重讲述这三个类型限定词。

# const限定词

const用来限定一个变量不容许被改变。一定程度上能够增强系统的健壮性，使得一些逻辑错误在编译阶段被发现。

### const只读变量&常量

```
const int n = 5;
int a[n];
```

上述代码在使用ANSI C编译时会出错。原因： 

ANSI C规定数组的定义必须用常量指定大小，不能用只读变量。虽然根据编译过程和内存分配，这种使用是合理的（好像C++中就是合理的），但是ANSI不容许。

只读变量和常量的区别：常量，如’abc’，’5’等不存在于内存中；只读变量存放在内存中。 

常量可以通过宏和枚举变量定义，字符串常量也可以在程序中定义，为其他变量赋值，如下面的p：

```
char *p = "i am hungry";
p[0] = 'c';
```

上面的代码在编译时会出现内存的非法写入错误，因为”i am hungry”是一个常量，其存放在**只读存储区**，指针p指向该区域，为p[0]赋值即像p指向的内存区域写入是不被容许的。

### const限定的内容

const的最常用法：const char m;限定m不可变（**与char const m意义相等**）

```
typedef char * pStr;
char string[4]="abc";//注意用的string[4]，而不是3，因为"abc"默认最后面跟一个'\0'字符，故大小为4；但强制用string[3]="abc"编译时也不会报错
const char * p1=string;
const pStr p2 = string;
p1++;//正确
p2++;//错误
```

此例中p1定义时为const char *p1，说明限定*p1不可变，但p1是可变的；p2的定义为const pStr p2，说明限定的是p2不可变，因此p2++是错误的。（预处理时不会将pStr像宏一样替换）

### const & 指针

const修饰变量主要由两种方式，一种是在前，一种是在后。 

（1）const在前

```
const int value;//value不可变
const char *pContent;//*pContent不可变
const (char *)pContent;//pContent不可变，类型为char *
char * const pContent;//pContent不可变，类型为char *
const char * const pContent;//pContent与*pContent都不可变
```

（2）const在后

```
int const value;//value不可变
char const *pContent;//*pContent不可变
char * const pContent;//pContent不可变
const char * pContent;//*pContent不可变
char const * const pContent;//pContent和*pContent均不可变
```

### const & 函数

const可用来修饰：1.输入参数 2.函数返回值 3.函数体 

如下面的三个const
`const rValue func(const list_array)const;`
1.修饰输入参数： 
$\bullet$可以修饰指针传递的参数，避免在函数内部意外改动指针所指数据
`void StringCopy(char *strDestination,char * const strSource);`
$\bullet$可以修饰指针传递的参数，避免函数内部意外修改指针本身
`void swap(int * const p1,int * const p2);`
2.修饰函数返回值： 
$\bullet$修饰“指针返回值”，即指针不能被修改

```
//声明
const char * getString(void);
//调用，**返回值必须加const**
const char * Str= getString();
```

$\bullet$修饰“数值返回值”
`const int getNum(void);`
# volatile限定词

volatile告诉编译器改变量是“易变的”，即除了可能被程序改变以外还可被其他代理改变。典型的如单片机中的某些硬件外设寄存器变量，如外部中断标志寄存器随时可能倍外部信号改变。 

volatile存在的目的是为了控制程序的优化，如下面的程序：

```
int x;
...
val1 = x;
val2 = x;
...
```

本段程序中连续使用x为val1和val2赋值，若不加valtile来限定x，则编译器可能会在编译的时候对这句话进行优化：第一步将x移到寄存器里，赋值给val1，然后在继续赋值给x2，这样只从内存中取一次数据，效率会得到提升，但是如果x是非常容易收到其他因素变化的话，这种优化可能会产生错误。因此此时代码可以写为如下方式，此时程序就会完全按照本来的顺序执行，不会去优化与变量x相关的代码

```
volatile x;
...
val1 = x;
val2 = x;
```

volatile可以与const同时存在，如硬件系统的时钟，不容许软件对其修改，因为需设为const；同时时钟又是一直不断更新的，因此需设为volatile。
`const volatile int time;`
# restrict限定词

该限定符只适用于指针，表明该指针是访问一个数据对象的**唯一且初始**的方式。该类型只能限定满足要求（唯一且初始的访问该数据）的指针。

```cpp
int ar[10];
int *restrict restar = (int *)malloc(10*sizeof(int));//可以修饰restar，
int *par = ar;//不能修饰par，因为par指向的ar是通过ar[10]定义的，还有很多其他种访问的方式
```

一旦编译器被告知该指针是restrict类型，编译器就可以做一些假定，对某些代码进行相应的优化。非restrict类型就不能做相应的优化。



