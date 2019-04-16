# Linux c 开发 - 指针 - 自娱自乐的代码人，专注Java、c、php - CSDN博客





2012年10月18日 16:15:27[initphp](https://me.csdn.net/initphp)阅读数：2063标签：[语言																[c																[数据结构																[float																[exchange](https://so.csdn.net/so/search/s.do?q=exchange&t=blog)
个人分类：[Linux c 开发系列](https://blog.csdn.net/initphp/article/category/2609039)

所属专栏：[Linux c 开发系列](https://blog.csdn.net/column/details/mylinuxc.html)](https://so.csdn.net/so/search/s.do?q=float&t=blog)




指针是用来指向一个内存块的地址的值。可以看一张图来理解指针：

![](https://img-blog.csdn.net/20150120104044708?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvaW5pdHBocA==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)

64位机器：

char ：1个字节

char*(即指针变量): 8个字节

short int : 2个字节

int:  4个字节

unsigned int : 4个字节

float:  4个字节

double:   8个字节

long:   8个字节

long long:  8个字节

unsigned long:  8个字节 



1. 使用*符号定义一个指针：



```cpp
int *p = NULL;
```


2. &符号为取地址符：



```cpp
int x = 100;
p = &x; //取变量x的地址值
```


3. *符号为取值符号：



```cpp
int x = 100;
p = &x;
printf("值：%d", *p); //*p取值
```




### 1. int类型指针

```cpp
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>

int main(void) {
    int a = 100;
    int *p = NULL; //定义一个int类型指针
    p = &a; //&号取地址值，&a就是取a的地址值
    printf("a的值：%d\n", *p); //*是取值符号，*p就是取a的值
    *p = 1001; //改变a的值为1001
    printf("a值被改变：%d\n", a); //输出a的值
    printf("a的地址值：%d\n", p); //直接p 即地址值
}
```
输出：
```
[admin@localhost test_c]$ ./point 
a的值：100
a值被改变：1001
a的地址值：1389256820
```


### 2. 字符类型指针



```cpp
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>

int main(void) {
    char x = 'a';
    char *p = NULL; //定义一个字符指针
    p = &x; //取x变量的地址值
    printf("x的值：%c\n", *p); //输出x的值
    printf("x的地址值：%d\n", p); //输出x的地址
    *p = 'z'; //将x变量的值改变为z
    printf("x被改变：%c\n", x); //输出x的值
}
```
输出：
```
[admin@localhost test_c]$ ./point 
x的值：a
x的地址：31265799
x被改变：z
```




### 3. 数组类型指针

数组本身就是指向第一个数组元素地址的指针。

可以看一副图：

![](https://img-blog.csdn.net/20150120114303130?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvaW5pdHBocA==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)

上图定义了一个int[10]的数组，内存块中会开辟10个int类型的块给这个数组使用。而每一个内存块占4个字节（64位系统），其中数组实际上指向的是a[0] 的内存块的值。

因为知道了a[0]的地址值之后，因为数组是连续的内存块地址，就可以通过 *(a + n) 这样的方式或者 a[n]这样的方式来读取数组中的元素了。

下面看一个字符数组的例子：



```
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>

int main(void) {
    char strs[5] = { 'a', 'b', 'c', 'd', 'e'};  
    char *pstrs = NULL;  //定义一个字符数组指针
    pstrs = strs;  //数组本身就是指向数组元素第一个值的内存的地址
    printf("数组第一个值:%c 数组第二个值：%c", *pstrs, *(pstrs + 1));  
    //如果指针指向地址改变，则指向数组的值也改变  
    pstrs++;  
    printf("\n数组第一个值:%c", *pstrs);  
    printf("\n数组也可以通过下标方式获取值：%c", strs[3]);  
    //还可以把数组变量直接当初指针来操作
    int i;  
    printf("\n循环输入：");  
    for (i = 0; i < sizeof(strs) / sizeof(char); i++) {   
        printf("\n:%c", *(strs + i));  
    }  
}
```
输出：
```
[admin@localhost test_c]$ ./point 
数组第一个值:a 数组第二个值：b
数组第一个值:b
数组也可以通过下标方式获取值：d
循环输入：
:a
:b
:c
:d
:e
```




### 4. 字符串类型指针

定义一个字符串：



```cpp
char str[] = "woshishen";
```
字符串相当于结尾会带\0的一个字符数组



我们可以通过**strlen**方法来获取字符串的长度。通过**sizeof**方法，则可以获取字符数组的长度。



```cpp
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>

int main(void) {
    char str[] = "woshishen333"; //字符数组本身就是一个指针，指向字符数组的第一个位置
    printf("长度:%d\n", strlen(str)); //字符串的长度
    printf("实际的字符数组长度，结尾带\\0，长度：%d", sizeof(str));
}
```
输出：
```
[admin@localhost test_c]$ ./point 
长度:12
实际的字符数组长度，结尾带\0，长度：13
```




定义的字符串本身就是字符数组，而数组本身就是指向第一个数组元素地址的指针。

下面可以看一个字符串指针例子：



```cpp
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>

int main(void) {
    char str[] = "woshishen"; //字符数组本身就是一个指针，指向字符数组的第一个位置
    char *p = NULL; //定义一个字符串指针
    p = str;  //因为字符串是数组，而数组本身就是指向第一个数组元素的指针，所以不需要用&符号取值

    //字符串输出。因为字符串本身末尾会自动带上\0，所以直接通过%s就可以输出字符串
    //而如果是字符数组，因为数组中没有\0符号，所以拿着字符数组直接%s来输出就会有问题
    printf("字符串的值：%s\n", p); 
    printf("字符串中的第一个值：%c\n", *p); //字符串第一个值
    printf("字符串中的第二个值：%c\n", *(p + 1)); //字符串第二个值
    p++;
    printf("改变字符串指针指向位置，指向第二个字符开始：%s\n", p); 
}
```

输出：

```
[admin@localhost test_c]$ ./point 
字符串的值：woshishen
字符串中的第一个值：w
字符串中的第二个值：o
改变字符串指针指向位置，指向第二个字符开始：oshishen
```

**注：如果是 char *x = "sadasddsa";  则x指向的字符串被系统当做静态常量，不能通过指针修改字符串里面的字符内容。**




### 5. 常量指针和指针常量

1. 常量指针。常量指针不可以修改指针所指向的地址值。





```cpp
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>

int main(void) {
    //常量指针常量指向一个地址后，不能被改变地址，但是可以改变地址
    int x = 100;
    int y = 200;
    int * const p = &x;
    *p = 200;
    //p = &y; //如果重新指向y的地址，则会编译出错
    printf("\n常量指针值：%d", *p);
}
```
输出：





```
[admin@localhost test_c]$ ./point 
常量指针值：200
```


2. 指针常量。指针常量不可以修改指针所指向内存地址的值。





```cpp
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>

int main(void) {
    //指针常量，不可以修改指针指向的内存地址的值
    int x = 100;
    int y = 200;
    int const *p = &x;
    //*p = 200; //不可以修改之
    p = &y;  //可以修改指向的地址
    printf("\n常量指针值：%d", *p);
}
```
输出：





```
[admin@localhost test_c]$ ./point 

常量指针值：200
```





### 5. 函数参数是指针



函数的入参为指针，则函数中可以修改该内存地址上的值。



```cpp
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>

//传入指针
void swap(int *a, int *b) {  
    if (*a > *b) {  
        int temp;  
        temp = *a;  
        *a = *b;  
        *b = temp;  
    }  
}  

int main(void) {
    int xxx = 20, yyy = 10;  
    int *pxxx = NULL, *pyyy = NULL;  
    pxxx = &xxx;  
    pyyy = &yyy;  
    swap(pxxx, pyyy); //如果xxx > yyy 则变量值交换位置  
    printf("\nXXX:%d YYY:%d", xxx, yyy);  
}
```
输出：





```
[admin@localhost test_c]$ ./point 

XXX:10 YYY:20
```






### 5. 指针函数和函数指针

1. 指针函数，本身上还是一个函数，只不过**返回值是指针类型**的。



```cpp
int *func(int x, int y);
```

```cpp
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>

//指针函数，返回值是int类型的一个指针
int *get(int *a) {
    *a = *a * 100;
    return a;
}  

int main(void) {
    int a = 100;
    int *b = &a;
    int *c = get(b); //返回的指针用 int *c来接
    printf("指针函数的值：%d", *c);
}
```
输出：
```
[admin@localhost test_c]$ ./point 
指针函数的值：10000
```


2. 函数指针。函数指针是指向函数的指针变量，即本质是一个**指针变量**。





```cpp
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>

//普通函数
int sum(int a, int b) {
    return a + b;
}  

int main(void) {
    int (* sum_p) (int x, int y); //声明一个函数指针
    int x = 100;
    int y = 200;
    //函数指针指向 sum这个函数
    //或是使用 sum_p = &sum
    sum_p = sum; 
    //也可以使用 int z = (* sum_p)(x, y)
    int z =  sum_p (x, y);
    printf("\n函数指针的值：%d", z);
}
```
输出：
```
[admin@localhost test_c]$ ./point 

函数指针的值：300
```


### 6. 结构体指针

定义一个结构体：



```cpp
//定义一个结构体  
typedef struct student {  
    char *username;  
    int age;  
} STU;
```


下面是一个结构体指针的例子：





```cpp
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h> 
#include <string.h>

//定义一个结构体  
typedef struct student {  
    char *username;  
    int age;  
} STU;  

int main(void) {
    STU *p = NULL; //定义一个结构体指针
    p = (STU *) malloc(sizeof(STU)); //分配一个结构体内存，p指向这个内存地址
    p->username = "zhuli"; 
    p->age = 100;
    printf("结构体数据：用户名%s 用户年龄%d", p->username, p->age);
}
```
也可以这样定义结构体，不过一般倾向于使用**typedef**来定义结构体





```cpp
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h> 
#include <string.h>

//定义一个结构体  
struct student {  
    char *username;  
    int age;  
};  

int main(void) {
    struct student *p = NULL; //定义一个结构体指针
    p = (struct student *) malloc(sizeof(struct student)); //分配一个结构体内存，p指向这个内存地址
    p->username = "zhuli"; 
    p->age = 100;
    printf("结构体数据：用户名%s 用户年龄%d", p->username, p->age);
}
```
输出：





```
[admin@localhost test_c]$ ./point 
结构体数据：用户名zhuli 用户年龄100
```](https://so.csdn.net/so/search/s.do?q=数据结构&t=blog)](https://so.csdn.net/so/search/s.do?q=c&t=blog)](https://so.csdn.net/so/search/s.do?q=语言&t=blog)




