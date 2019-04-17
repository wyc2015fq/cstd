# c++指针（二） - 墨流觞的博客 - CSDN博客





2017年02月21日 13:41:30[墨氲](https://me.csdn.net/dss_dssssd)阅读数：132标签：[c++																[指针																[c](https://so.csdn.net/so/search/s.do?q=c&t=blog)
个人分类：[c++](https://blog.csdn.net/dss_dssssd/article/category/6737792)





## c++指针（二）

一.变量： 

    c++中变量包含两部分内容：存储变量的地址（位置），该地址上存储的值（内容）

```cpp
int num = 20;
    cout << "num的位置："<<&num << endl;
    cout<<"num的内容："<<num;
```

输出:

```
num的位置：012FFB28
num的内容：20
```

![模型解释](https://img-blog.csdn.net/20170221130632857?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvZHNzX2Rzc3NzZA==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)

解释num += 1;语句的执行过程： 

    首先源程序经过编译器处理后，将num变量分配到指定的内存地址上，在执行该语句是，首先根据num找到对应的内存地址（0x112FFB28）按照int形4字节的规则解析0x112FFB28~0x112FFB41存储的数据，即取出num的值20，执行增量+1操作，再将得到的数据放回原地址上。此种按变量的地址直接存储变量的方法称为“直接访问”方式。存储变量的内存空间的首地址称为该变量的地址。 

    二.指针 ——间接访问方式 

    注意：指针也是变量，自然也具有变量的属性：内容和位置 

    指针具体的说是存储另一个对象的地址的变量- 内容（值）：另一个对象的地址
- 
位置（地址）：存储该指针变量的地址

1.声明：

`类型 * 变量名1（，*变量名2，... ...）`
解释：

1>.类型：明确指针指向的数据类型，指针只存放变量的首地址，必须指定变量所占的内存和数据是如何组织的，比如4字节整型数，4字节浮点型和更复杂的类和结构体型，类型向编译器表明了解析方式。

```
int num = 0x00ff00ff;
    cout << "num的位置："<<&num << endl<<"num的内容："<<num;
    int *pint = &num;
    short *pshort = (short*)&num;
    cout << "*pint=" << *pint << endl;
    cout << "*pshort =" << *pshort << endl;
    float *pfloat =(float*) &num;
    cout << "解析为浮点数：" << *pfloat << endl;
```

输出：

```
num的位置：001DFEA4
num的内容：16711935
*pint=16711935
*pchar=255
解析为浮点数：2.34184e-38
```

short占两个字节，故编译器在解析*pshort时，只解析001DFEA4和001DFEA5两个字节的内容，故255，浮点数与整数的解析方式不同，故同样的地址，同样的字节数，但解析后的数据也不同 

2>.在一行语句中同事声明多个指针变量是，一定按照上述声明方式声明，即每一个指针变量需要一个指针变量说明符‘*’，比如 

int *lp1,*lp2 

而int *lp1,lp2,则lp1为指针，lp2为整型变量 

2.  赋值初始化：
**1>声明时接着赋值初始化**
**2>声明时不初始化，而在使用时在初始化**

```
int num = 20;
    int *ptr = &num;

    int *ptr2;
    ptr2 = &num;
    cout << "Value of num:" << num << ';' << "Address of num:" << &num << endl;
    cout << "Value of *ptr:" << *ptr<< "Value of ptr:" << ptr << ';' << "Address of ptr:" << &ptr << endl;
    cout << "Value of *ptr2:" << *ptr2 << "Value of ptr2:" << ptr2 << ';' << "Address of ptr2:" << &ptr2 << endl;
```

输出：

```
Value of num:20;Address of num:00EFFB20
Value of *ptr:20Value of ptr:00EFFB20;Address of ptr:00EFFB14
Value of *ptr2:20Value of ptr2:00EFFB20;Address of ptr2:00EFFB08
```

**使用前一定要初始化指针，否则会遇到非常隐秘的错误。**

指针赋值的另一种形式： 

    当然可以初始化指针为一个具体数值，但要使用强制类型转换
```
/*这种机会基本没有，事前根本不会知道编译器将变量存放在内存中哪    个位置*/
    int *ptr3;
    ptr3 = (int *)0x0029b028;
```

-指针四种赋值类型
- 0值常量式：

```
const int p = 0;
//常变量
int *pt = p;
//0
pt = p;
//宏定义的形式，NULL在编译时替换为0
pt = NULL;
```
- 
类型匹配的对象地址

- 
同类型的有效指针


```
int num =20;
int *p1 = &num;
int *p2;
//将p2赋值给p1
p2 = p1;
```
- 一对象末的下一个地址

```
//数组中指针操作
int size = 4;
int arr[] = {1,2,3,4};
int *ptr = arr;
ptr = arr + 4; // 指向数组最后一个元素4的下一个地址
```

3.与指针相关的两个操作符： 

‘&’取地址运算符：作用于内存中一个可寻址的数据（如变量，对象，数组元素等）获得该变量的地址。运算结果不可作左值 

 ‘*’间接引用运算符，作用于内存类型的变量，访问指针所指向的内存地址中的数据。运算结果可以作为左值，来修改所指向对象的值 

4.指针的加减法: 

    1>指针加上一个整数n（任意整数），加n的结果等于原来的地址加上n*指针所指向的对象所占的总字节数，同理减法，得到的地址为原来的地址加上n*指针所指向的对象所占的总字节数： 

    newAddress = oldAddress +n*针所指向的对象所占的总字节数 

    newAddress = oldAddress -n*针所指向的对象所占的总字节数 

2>一个指针减去另一个指针，得到一个整数，用在数组中，来得到两个数组元素之间的间隔
```cpp
int num = 20;

    int *ptr = &num;
    short *ptrs = (short*)&num;
    cout << "address of num:" << ptr<<'\n'<<"address of (ptr+1):"<<ptr + 1<< endl<<"address of (ptr-1):"<<ptr -1<<endl;
    cout << "address of num:" << ptrs << '\n' << "address of (ptrs+1):" << ptrs + 1 << endl << "address of (ptrs-1):" << ptrs - 1;
    int arr[4] = { 0,1,2,3 };
    int *pa = arr,*pa2=&arr[2];
    cout <<endl<< " 数组中2和0的索引号差：" << pa2 - pa << endl;
```

输出：

```
address of num:00AFFB88
address of (ptr+1):00AFFB8C
address of (ptr-1):00AFFB84
address of num:00AFFB88
address of (ptrs+1):00AFFB8A
address of (ptrs-1):00AFFB86
数组中2和0的索引号差：2
```

可看出ptr指向4字节整数，则ptr+1得到的地址为加4个字节 

同样ptrs指向2字节短整数，则ptrs+1得到的地址为加2个字节

程序：

```cpp
int i = 15, j, *p, *q;
    p = &i;
    cout << "p指向的值：" << *p << endl<<endl;
    *p = 20;
    cout << "p指向的值：" << *p << endl << "i的值：" << i << endl << endl;
    j = 2 * (*p);
    cout << "j的值：" << j << endl << endl;
    q = &i;
    cout << "p存贮的地址值：" << p << endl ;
    cout << "q存贮的地址值：" << q << endl ;
    cout << "p指向的值：" << *p << endl ;
    cout << "q指向的值：" << *q << endl << endl;
    *p = *q - 1;
    cout << "p指向的值：" << *p << endl ;
    cout << "q指向的值：" << *q << endl << endl;
    q = &j;
    cout << "q存贮的地址值：" << q << endl;
    cout << "q指向的值：" << *q << endl << endl;
    *p = *q - 1;
    cout << "p存贮的地址值：" << p << endl;
    cout << "q存贮的地址值：" << q << endl;
    cout << "p指向的值：" << *p << endl;
    cout << "q指向的值：" << *q << endl << endl;
    *(&i) = 255;
    cout<<"i的值："<<i<<endl;
```

输出：

```cpp
p指向的值：15

p指向的值：20
i的值：20

j的值：40

p存贮的地址值：0057F79C
q存贮的地址值：0057F79C
p指向的值：20
q指向的值：20

p指向的值：19
q指向的值：19

q存贮的地址值：0057F790
q指向的值：40

p存贮的地址值：0057F79C
q存贮的地址值：0057F790
p指向的值：39
q指向的值：40

i的值：255
```

画图解释： 

在使用指针时一定注意画图表明关系 
![解释](https://img-blog.csdn.net/20170221205632278?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvZHNzX2Rzc3NzZA==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)
![这里写图片描述](https://img-blog.csdn.net/20170221210029862?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvZHNzX2Rzc3NzZA==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)

![这里写图片描述](https://img-blog.csdn.net/20170221205853154?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvZHNzX2Rzc3NzZA==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)

![这里写图片描述](https://img-blog.csdn.net/20170221210117613?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvZHNzX2Rzc3NzZA==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)

图像来自《数据结构与算法——c++》 

区分： 

1.
```
int num;
int * const ptr=&num;
```

ptr是指向num的指针常量，意味着ptr的值，即指向的地址是不可变化的：

```
int num2;
//错误，指针所指向的地址不可更改，指针的内容不可变
ptr = &num2
//正确，指针所指向对象的的内容是可以变的
*ptr=2
```

关键：指向的地址是不可改变的，但是指向的对象的值是可以改变 

2.

```
int n = 20;
    int m = 10;
    const int *p = &n;
    //赋值正确
    p = &m;
    //赋值错误
    *p = 10;
    //正确
    n =30;
    /*意义：
    禁止通过指针间接修改所指向内容的值，起到保护作用
    但可以通过变量名直接修改*/
```

p是一个指向整型常量的指针类型，也就是说指针指向对象的值是不可变的，但指向对象的位置是可以变得。

**禁止通过指针间接修改所指向内容的值，起到保护作用**
**但可以通过变量名直接修改**

区别： 

指针常量：指针指向的地址（即指针的内容）不会改变，保护指针 

 形式：      `类型* const  变量名`

常量指针：保护所指向对象的值不会通过指针间接修改，保护指针所指向的对象 

 形式：         `const  类型* 变量名 或 类型 const* 变量名`
注意： 

c++中不允许指针取数字的地址，但可以取常量的地址

```
const double PI = 3.14159;
//允许,但在vs2015中需要使用指针常量或强制类型转化
//1.const double *p =&PI,此时不可通过指针修改PI的值
//2.double*p = (double*)
double *p = &PI;
//不允许
double *p2=&1.21;
```

```cpp
const int p = 23;
    const int *pt = &p;


    int *ptr =(int*) &p;
    *ptr = 6;
    //cout << p;
    cout << ptr << ' '<< *ptr<<endl;
    cout << &p <<' '<< p;
```

输出：

```
00AFF930 6
00AFF930 23
```

常量在常量存储区和栈各有一个p，而栈为副本，通过指针修改站上的副本，并未修改常量存储区，而p直接访问访问的是常量存储区，间接访问访问的是栈的地址即副本的内容。](https://so.csdn.net/so/search/s.do?q=指针&t=blog)](https://so.csdn.net/so/search/s.do?q=c++&t=blog)




