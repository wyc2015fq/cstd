# C/C++ 指针和数组 - 墨流觞的博客 - CSDN博客





2018年12月05日 13:50:30[墨氲](https://me.csdn.net/dss_dssssd)阅读数：39标签：[c																[指针](https://so.csdn.net/so/search/s.do?q=指针&t=blog)](https://so.csdn.net/so/search/s.do?q=c&t=blog)
个人分类：[c](https://blog.csdn.net/dss_dssssd/article/category/8499802)








## 1. 数组 c++

##### 声明：

一维数组： `type arrayName [ arraySize ];`

多维数组： `type name[size1][size2]...[sizeN];`

比如结构体数组的初始化和访问：

```
#include <iostream>
#include<stdlib.h>
using namespace std;


struct Share {
	long int share_price;
	char time_date[20];
};


int main()
{
	
	//一维数组
	Share one_D_share[] = { {100, "2017\\05\\31_09:00"}, {200,"2017\\05\\31_09:01"} };

	for (size_t index = 0; index < 2; ++index) {
		cout << one_D_share[index].share_price<<'\t' << one_D_share[index].time_date << endl;
	}

	//二维数组
	Share two_D_share[2][2] = { {{100, "2017\05\31_09:00"}, {200,"2017\\05\31_09:01"}},{{300, "2017\05\31_09:02"}, {400,"2017\05\31_09:04"}} };

	for (size_t i = 0; i < 2; ++i)
	for(size_t j = 0; j < 2; ++j){
		cout << two_D_share[i][j].share_price << two_D_share[i][j].time_date << endl;
	}
	
	//对部分元素赋值，为魏阙赋值的部分整数位0，字符为'\0'
	Share share_3[3] = { {100, "2017\\05\\31_09:00"}, {200,"2017\\05\\31_09:01"} };

	for (size_t index = 0; index < 3; ++index) {
		cout << share_3[index].share_price << '\t' << share_3[index].time_date << endl;
	}


	system("pause");
	return 0;
}
```

## 2. 指针

### 1. C中指针的概念

数据存放在内存中，而不同类型的数据占用的字节数不同，比如int占用4个字节。为了正确的访问到这些数据，必须为每个字节编号，比如旅馆中的房间号，根据编号可以准确的找到某个字节。

一下是4G内存每个字节的编号(十六进制表示：)
![在这里插入图片描述](https://img-blog.csdnimg.cn/20181205104448595.png)
内存中字节的编号称为**地址（Address）或指针（Pointer）**。地址从 0 开始依次增加，对于 32 位环境，程序能够使用的内存为 4GB，最小的地址为 0，最大的地址为 0XFFFFFFFF。

```
#include <stdio.h>
#include <stdlib.h>

void Function() {

}

int main() {
	int a = 16;
	char str[20] = "C语言指针";
	int arr[] = { 10, 100, 1000 };
	printf("%#X, %#X, %#X, %#X\n", &a, &str, &arr, &Function);
	printf("%#x, %#X, %#X, %#X\n", a, str, arr, Function);
	system("pause");
	return 0;

	

}
```

out:

> 
0X58FEAC, 0X58FE90, 0X58FE7C, 0XFA1348

0x10, 0X58FE90, 0X58FE7C, 0XFA1348


`%#x`是16进制输出

**一切都是地址**

CPU访问内存需要的是地址，而非变量名和函数名，在编译和链接成可执行程序后，变量名和函数名都会被替换成地址。

比如：假设int类型 a, b, c的内存地址为 0X1000, 0x1004, 0x1008,则减法运算`c=a-b；`

在执行过程中转换为：
$$(0x1008)=(0X1000)-( 0x1004)$$,其中`()`为取值操作，表达式的意思是，取出地址0X1000，0X1004中的值，相减赋值到0X1008的内存地址中。
需要注意的是：比如在上的胡代码示例中，**变量名代表的是数据本身，而函数名，数组名，字符串名表示的是首地址**

### 2.C指针

如果想在程序中访问数据的内存地址，只能需要指针变量了。

在C语言中们可以使用一个变量来存放指针(地址)，该变量称为**指针变量**，值为某一数据的地址，该数据可以使数组，字符串，函数，另一普通变量火指针变量。

##### 定义指针变量

`datatype *name`或者`datatype *name=value`
`*`代表是一个指针变量，`datatype`为该指针所指向数据的数据类型。比如：

```
int *ip1=NULL;
int a = 100;
ip1 = &a;
```

其中，`&`为取地址运算符

内存指向模型：![在这里插入图片描述](https://img-blog.csdnimg.cn/20181205111154798.png)

inp指向地址0x1000，然后根据类型为int，**则向后取4个字节的地址，作为数据块**，最终会得到a的数据。

指针可以向普通变量一样多次写入和修改

```
//定义int和char普通变量
int a=0, b=1;
char c='a', d='b';
//定义指针变量
int *ip1 = &a;
char *cp2 = &c;

//修该指针变量
ip1 = &b;
cp2=&c;
/*
与下列修改方法的不同：
*ip1 = b;
*cp2 = d;

指向的地址未变，更改了对应地址的内容，
而上一个是更改了指向的地址，也就是更改了指针变量本身的内容
*/
```

在声明变量是必须带`*`，而在给指针变量赋值是不需要，实际上`*ip`表示的是解引用，意思是取出ip所指向地址的数据

修改过程中，指针变化情况：
![在这里插入图片描述](https://img-blog.csdnimg.cn/20181205114942758.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L2Rzc19kc3Nzc2Q=,size_16,color_FFFFFF,t_70)
指针变量可以连续定义：注意每一变量前都要有`*`
`int *a, *b, *c; //a、b、c 的类型都是 int*`

而如果写成下面的形式，只有a为指针 变量，b,c为int类型的变量。
`int *a, b,c;`
##### 解引用获取数据

格式： `*pointer;`

这里的`*`为**指针运算符**，来取出某个地址上的数据：

```
int a = 20;
	int *ip = &a;
	printf("%d, %d\n", a, *ip);
```

out:

> 
20, 20


![在这里插入图片描述](https://img-blog.csdnimg.cn/20181205115825141.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L2Rzc19kc3Nzc2Q=,size_16,color_FFFFFF,t_70)

处理流程为，在程序被编译和连接后，a,ip被替换为相应的地址0x1001和0x2007,使用*ip时，先通过地址0x2007获得ip的值，0x1001，也就是a的地址，然后再通过这个值取得变量a的数据，共进行两次运算；而使用a的话，只需要通过0x1001直接取得它的数据，只要一步计算。

##### 修改内存数据

```
int a = 20, b = 200, c = 2000;
	int *ip = &a;
	*ip = b;
	c = *ip;

	printf("%d, %d,%d,%d\n", a, b,c,*ip);
```

out:

> 
200, 200,200,200


*ip代表的是a中的数据，等价于a。可以将另外一份数据赋值给它，也可以将它赋值给别的数据。

**注意：**
`*`在不同的场景下有不同的作用：在变量**定义**中，表明是指针变量和普通变量。而在**使用**指针变量是，前面加`*`号表示获取指针指向的数据

更复杂的指针变量赋值：

```
int x, y, *px = &x, *py = &y;
y = *px + 5;  //表示把x的内容加5并赋给y，*px+5相当于(*px)+5
y = ++*px;  //px的内容加上1之后赋给y，++*px相当于++(*px)
y = *px++;  //相当于y=(*px)++
py = px;  //把一个指针的值赋给另一个指针
```

##### 关于*和&的谜题

假设int类型的变量a，pa是指向它的指针，则

*&a: 理解为 *(&a),表示先取a的地址相当于pa，在对地址解引用，最表表示的是a

&*pa: 理解为&(*pa),表示先解引用pa,相当于a，在取a的地址，最终表示的pa
##### `*`的总结
- 表示乘法，用于算数运算中
- 在**定义**指针变量时使用，以和普通变量区分
- 在**使用**指针变量时，表示解引用指针指向的数据

### 3.指针变量的运算

指针时一个用数值表示的地址，因此可以对指针执行算数： ++,–,+,-和比较运算==，<, >

**值得注意的是**，每次+或-,地址的增加或减少字节数与定义指针变量的类型有关:

```
int    a = 10,   *pa = &a, *paa = &a;
    double b = 99.9, *pb = &b;
    char   c = '@',  *pc = &c;
    //最初的值
    printf("&a=%#X, &b=%#X, &c=%#X\n", &a, &b, &c);
    printf("pa=%#X, pb=%#X, pc=%#X\n", pa, pb, pc);
    //加法运算
    pa++; pb++; pc++;
    printf("pa=%#X, pb=%#X, pc=%#X\n", pa, pb, pc);
    //减法运算
    pa -= 2; pb -= 2; pc -= 2;
    printf("pa=%#X, pb=%#X, pc=%#X\n", pa, pb, pc);
    //比较运算
    if(pa == paa){
        printf("equally\n");
    }else{
        printf("not equallyd\n");
    }
    return 0;
```

out:

> 
&a=0X28FF44, &b=0X28FF30, &c=0X28FF2B

pa=0X28FF44, pb=0X28FF30, pc=0X28FF2B

pa=0X28FF48, pb=0X28FF38, pc=0X28FF2C

pa=0X28FF40, pb=0X28FF28, pc=0X28FF2A

not equally
从运算结果来看，pa、pb、pc 每次加 1，它们的地址分别增加 4、8、1个字节，这与他们数据类型所占的字节相同。

如果两个指针指向的是相关的变量，可以做比较运算，最常用的是利用算数和比较运算进行数组访问。

```
int a[10] = { 0,1,2,3,4,5,6,7,8,9 };
	int *p;
	for (p = a; p < a + 10; ++p)
	{
		printf("%#x,%d\n", p, *p);
	}
```

out:

> 
0x96f920,0

0x96f924,1

0x96f928,2

0x96f92c,3

0x96f930,4

0x96f934,5

0x96f938,6

0x96f93c,7

0x96f940,8

0x96f944,9
### 4. 指针和const关键字

以下两种声明等效：

```
const int *ip_a;
int const *ip_a;
```

代表的是指针指向的数据是常量。不能更改ip_a所指向数据的值，比如`*ip_a=42`出错。

而

```
int *const ip_b
```

表示指针(地址)本身是常量，你可以改变ip_b指向的值，比如`*ip_b=42;`但是却不可以改变ip_b本身指向的地址，也就是自己的值，比如`ip+b++`出错。
**注意：** 数组的首地址就是常量指针，其指向的地址不可改变，比如`int a[10];`，`a++`出错

### 5.数组指针 array pointer

**数组在内存区域中是一块连续的内存块**

定义：

```
int arr[] = {1,2,3,4,5};
int *p=arr; // 第一种
int *p = &arr[0];//第二种
```

数组指针指向是数组中某一具体的元素，而非整个数组，所以数组指针的类型和数组元素的类型相同，在代码中，p指向的数组元素是int类型，所以p的类型是`int*`。

数组的遍历方式：

```
int arr[] = { 0,1,2,3,4};
	int len = sizeof(arr) / sizeof(int);
	int *p=arr;
	//指针遍历1
	printf("指针遍历1\n");
	for (; p < arr + len; ++p)
	{
		
		printf("%#x,%d\n", p, *p);
	}
	//指针遍历2
	p = arr;
	printf("指针遍历2\n");
	for (int i=0; i < len; ++i)
	{
		
		printf("%#x,%d\n", p + i, *(p + i));
	}
	//下标遍历
	p = arr;
	printf("下标遍历\n");
	for (int i = 0; i < len; ++i)
	{
		
		printf("%#x,%d\n", p + i, p[i]); //等价于printf("%#x,%d\n", a + i, a[i])
	}
	//逆向遍历，反向输出
	

	printf("反向遍历\n");
	for (p = arr + len - 1; p >= arr; --p)
	{

		printf("%#x,%d\n", p, *p);
	}
```

out:

> 
指针遍历1

0x6ffae8,0

0x6ffaec,1

0x6ffaf0,2

0x6ffaf4,3

0x6ffaf8,4

指针遍历2

0x6ffae8,0

0x6ffaec,1

0x6ffaf0,2

0x6ffaf4,3

0x6ffaf8,4

下标遍历

0x6ffae8,0

0x6ffaec,1

0x6ffaf0,2

0x6ffaf4,3

0x6ffaf8,4

反向遍历

0x6ffaf8,4

0x6ffaf4,3

0x6ffaf0,2

0x6ffaec,1

0x6ffae8,0
**注意：**

数组在内存中只是数组元素的简单排列，没有开始和结束标志，在求数组的长度时不能使用`sizeof(p) / sizeof(int)`，因为 p 只是一个指向 int 类型的指针，编译器并不知道它指向的到底是一个整数还是一系列整数（数组），所以 sizeof§ 求得的是 p 这个指针变量本身所占用的字节数，而不是整个数组占用的字节数。

**所以在访问整个数组时，需要有数组长度参数，否则将无法访问数组。这在数组作为函数参数时尤其要注意，因为只是传入数组首地址。所以还要传入长度参数**

```
void printf_arr(int arr[],int len) {
	printf("通过sizeof获得的长度：%d\n", sizeof(arr) / sizeof(int));
	for (int i = 0; i < len; ++i) {
		printf("%d\t", arr[i]);
		}
	}
	int arr[] = { 0,1,2,3,4};
	int len = sizeof(arr) / sizeof(int);
	printf_arr(arr, len);
```

out:

> 
通过sizeof获得的长度：1

0       1       2       3       4


### 6. 指针数组

顾名思义，指针数组就是数组中的元素为指针。

定义：

```
const int MAX=3;
int *p[MAX]
```

在这里，把 ptr 声明为一个数组，由 MAX 个整数指针组成。因此，ptr 中的每个元素，都是一个指向 int 类型的指针
- 简单的，指向三个int值

```
int a = 1, b = 2, c = 3;

	int *ptr[3];
	//赋值为整数的地址
	ptr[0] = &a;
	ptr[1] = &b;
	ptr[2] = &c;
	for (int i = 0; i < 3; i++)
	{
		printf("Value of var[%d] = %d\n", i, *ptr[i]);
	}
```

out:

> 
Value of var[0] = 1

Value of var[1] = 2

Value of var[2] = 3- 每个指针指向int类型的数组，最终ptr指向的是二维数组

```
int arr1[3] = { 0,1,2 };
	int arr2[2] = { 9,99 };
	int arr3[5] = { 1,10,100,1000,10000 };
	int *ptr[3];
	ptr[0] = arr1;
	ptr[1] = arr2;
	ptr[2] = arr3;

	int length[] = { 3, 2, 5 };

	for (int i = 0; i < 3; ++i) {
		
		
		for (int j = 0; j < length[i]; ++j) {
			//printf("%d ", *(ptr[i] + j)); 与下列等价
			printf("%d ", ptr[i][j]);
		}
		printf("\n");
	}
```

out:

> 
0 1 2

9 99

1 10 100 1000 10000
内存模型：
![在这里插入图片描述](https://img-blog.csdnimg.cn/20181205134913491.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L2Rzc19kc3Nzc2Q=,size_16,color_FFFFFF,t_70)
### 7 字符数组和字符串

用来存放字符的数组称为字符数组，例如

```
char a[10]; //一维数组
char b[5][10];  //二维字符数组
char c[20]={'c', '  ', 'p', 'r', 'o', 'g', 'r', 'a','m'};  // 给部分数组元素赋值
char d[]={'c', ' ', 'p', 'r', 'o', 'g', 'r', 'a', 'm' };  //对全体元素赋值时可以省去长度
```

字符数组使劲上是一系列字符的集合，也就是**字符串（String）**,但是在C语言中，没有专门的String类型，使用字符数组来存放字符串
- 定义
- 
可以直接将字符串赋值给字符数组

```
char str[30] = {"c.biancheng.net"};
char str[30] = "c.biancheng.net";  //这种形式更加简洁，实际开发中常用
```

- 
也可以不指定数组长度

```
char str[] = {"c.biancheng.net"};
	char str[] = "c.biancheng.net";  //这种形式更加简洁，实际开发中常用
```


**注意：** 字符数组只有在定义时才能将整个字符串一次性地赋值，一旦定义完成后，只能一个字符一个字符的赋值。

```
char str[4];
str = "ab1"; //错误

//一下赋值方式正确
str[0] = 'a'; str[1]='b';str[2]='1';
```

##### 字符串结束标志(重点)

在C语言中，字符串总是以`\0`作为结尾，`\0`也被称为字符串结束标志

> 
`\0`是ASCII码表中的第0个字符，英文为`NUL`,中文成文“空字符”，在C语言中唯一的效果就是作为字符串结束标志


C语言在处理字符串式，会从前往后逐个扫描字符，当遇到`\0`时认为到达字符串的末尾，处理结束。如果没有`\0`，意味着永远到达不了字符串的结尾。

**由""包裹的字符串会在末尾自动添加’\0’**，所以会额外占据一个字节位置。

下图演示了“C program”在内存中的存储情况：
![在这里插入图片描述](https://img-blog.csdnimg.cn/20181205173617353.png)
**注意:**
- 逐个字符的给数组赋值并不会自动添加`\0`,
- 用字符数组存储字符串式，要注意为`\0`多留一个字节
`char str[7] = "abc123";`

"abc123"看起来只包含了 6 个字符，我们却将 str 的长度定义为 7，就是为了能够容纳最后的’\0’。如果将 str 的长度定义为 6，它就无法容纳’\0’了。
> 
当字符串长度大于数组长度时，有些较老或者不严格的编译器并不会报错，甚至连警告都没有，这就为以后的错误埋下了伏笔，读者自己要多多注意。


最好的办法是：**在创建数组时将所有的元素初始化为0**

在数组中存储A-Z：

```
#include <stdio.h>
int main(){
    char str[30] = {0};  //将所有元素都初始化为 0，或者说 '\0'
    char c;
    int i;
    for(c=65,i=0; c<=90; c++,i++){
        str[i] = c;
    }
    printf("%s\n", str);
   
    return 0;
}
```

**注意**：在函数内部定义的变量，数组，结构体，共用体等都成为**局部数据**，在很多编译器下，局部数据的初始化通常是随机初始化为任意值，而非0值。

### 8.指针访问字符数组

指针访问字符数组和指针访问普通数组完全一样，不在赘述

处理上述7中讲述的**字符数组**，还有一种字符串，直接使用指针指向字符串，

```
char *str="I want to learn c";
```

字符串中的所有字符在内存中为连续排列，str指向字符串的第0个字符，通常将第0个字符的地址称为**字符串的首地址**

下面演示如何输出字符串：

```
#include <stdio.h>
#include <string.h>
int main(){
    char *str="I want to learn c";
    int len = strlen(str);
   
    //直接输出字符串
    printf("%s\n", str);
    //使用*(str+i)
    for(int i=0; i<len; i++){
        printf("%c", *(str+i));
    }
    printf("\n");
    //使用str[i]
    for(int i=0; i<len; i++){
        printf("%c", str[i]);
    }
    printf("\n");
    return 0;
}
```

一下是字符数组的输出：

```
#include <stdio.h>
#include <string.h>
int main(){
    char str[] = "I want to learn c";
    int len = strlen(str);
    //直接输出字符串
    printf("%s\n", str);
    //每次输出一个字符
    for(int i=0; i<len; i++){
        printf("%c", str[i]);
    }
    printf("\n");
    return 0;
}
```

上述两者在输出方式是很相似，但是两者最本质的区别是**在内存中的存储区域不同**

*字符数组存储在全局数据区或栈区，而第二种形式的字符串存储在常量区，*而全局数据区和栈区的字符串（也包括其他数据）有读取和写入的权限，而常量区的字符串（也包括其他数据）只有读取权限，没有写入权限

这就意味着：
- `char *str="dsdsd";`使用指针定义的字符串为常量字符串，只可读取。不可更改字符串内容
- `char srt[]="dsdsd";`这种字符数组可以读取和修改

### 9. 指针作为函数参数

用指针变量作为函数参数可以将函数外部的地址传递带函数内部，使得在函数内部可以操作函数外部的数据，并且这些数据不会随着函数的结束而被销毁。

像数组、字符串、动态分配的内存等都是一系列数据的集合，没有办法通过一个参数全部传入函数内部，只能传递它们的指针，在函数内部通过指针来影响这些数据集合。

而对于整数、小数、字符等基本类型数据的操作有时也必须要借助指针，比如交换两个变量的值

```
#include <stdio.h>
void swap(int *p1, int *p2){
    int temp;  //临时变量
    temp = *p1;
    *p1 = *p2;
    *p2 = temp;
}
int main(){
    int a = 66, b = 99;
    swap(&a, &b);
    printf("a = %d, b = %d\n", a, b);
    return 0;
}
```

实际上将a和b的地址重新拷贝然后分别给p1，p2.这样*p1,*p2就是提取地址上的数据，进行数据交换。函数运行结束后p1,p2销毁，但是外部地址上的数据已经交换了。

##### 数组作为函数参数

数组是一系列数据的集合，无法通过参数将它们一次性传递到函数内部，如果希望在函数内部操作数组，必须传递数组指针。

定义max_and_min函数找到数组中的最大最小值

```
#include <stdio.h>
#include <stdlib.h>
void max_and_min(int * arr, int *max, int *min, int len) {
	*max = *min = arr[0];
	for (int i = 1; i < len; ++i) {
		if (*max < arr[i]) *max = arr[i];
		else if (*min > arr[i]) *min = arr[i];
	}
}
int main(){
	int max = 0, min = 0;
	int num[6] = { 1, 2, 3,0, -1, 2 };
	max_and_min(num, &max, &min, 6);
	printf("Max value is %d!\n", max);
	printf("Min value is %d!\n", min);


	system("pause");
	return 0;

	

}
```

out:

> 
Max value is 3!

Min value is -1!


也可以采用：
`void max_and_min(int arr[6], int *max, int *min, int len)`
`void max_and_min(int arr[], int *max, int *min, int len)`
两者都不会创建一个数组出来，编译器也不会为它们分配内存，实际的数组是不存在的，它们最终还是会转换为`int *arr`这样的指针。这就意味着，两种形式都不能将数组的所有元素都传递进来，大家还得使用数组指针

不管使用哪种方式传递数组，都不能在函数内部求得数组长度，因为 intArr 仅仅是一个指针，而不是真正的数组，所以必须要额外增加一个参数来传递数组长度。

**C语言为什么不允许直接传递数组的所有元素，而必须传递数组指针呢？**

**参数的传递本质上是一次赋值的过程，赋值就是对内存进行拷贝。所谓内存拷贝，是指将一块内存上的数据复制到另一块内存上。**

对于像 int、float、char 等基本类型的数据，它们占用的内存往往只有几个字节，对它们进行内存拷贝非常快速。而数组是一系列数据的集合，数据的数量没有限制，可能很少，也可能成千上万，对它们进行内存拷贝有可能是一个漫长的过程，会严重拖慢程序的效率，所以，C语言没有从语法上支持数据集合的直接赋值。

除了C语言，C++、Java、Python 等其它语言也禁止对大块内存进行拷贝，在底层都使用类似指针的方式来实现。

### 10 函数返回指针

C语言支持函数返回指针

定义：
`typename * functionName()`
但是使用指针作为函数返回值需要注意一点，函数运行结束后会销毁内部定义的所有局部数据，包括局部变量，局部数组和形式参数，所以函数返回的指针尽量不要指向这些数据。否则可能会在后续使用过程中可能会引发意向不到的错误。

```
#include <stdio.h>
int *func(){
    int n = 100;
    return &n;
}
int main(){
    int *p = func(), n;
    printf("c语言\n");
    n = *p;
    printf("value = %d\n", n);
    return 0;
}
```

当指向这段代码时，n将不是100，在函数结束后，n失去对该块内存的使用权限，程序的其他代码可以任意使用该块内存，因而导致代码出错。

而如果实在函数内定义static静态变量，则可以，因为其存储在静态数据区，并不想局部变量一样存储在栈区，不会因为函数结束就销毁其内存区域。因而可以正常返回其地址。

将代码中的`int n = 100;`改为`static int n = 100;`即可。

### 11.二级指针

定义：
`typename **p1;`

![在这里插入图片描述](https://img-blog.csdnimg.cn/20181206152951101.png)

代码：
```
int a =100;
int *ip1 = &a;
int **ip2 = &ip1;
```

### 12. 二维数组

二维数组在概念上是二维的，有行和列，但在内存中所有的数组元素都是连续排列的
`int a[3][4] = { {0, 1, 2, 3}, {4, 5, 6, 7}, {8, 9, 10, 11} };`

在内存中，a 的分布是一维线性的，整个数组占用一块连续的内存：
![在这里插入图片描述](https://img-blog.csdnimg.cn/20181206153620160.png)
C语言中的二维数组是按行排列的，也就是先存放 a[0] 行，再存放 a[1] 行，最后存放 a[2] 行；每行中的 4 个元素也是依次存放。数组 a 为 int 类型，每个元素占用 4 个字节，整个数组共占用 4×(3×4) = 48 个字节。

C语言允许把一个二维数组分解成多个一维数组来处理。对于数组 a，它可以分解成三个一维数组，即 a[0]、a[1]、a[2]。每一个一维数组又包含了 4 个元素，例如 a[0] 包含 a[0][0]、a[0][1]、a[0][2]、a[0][3]。

假设数组 a 中第 0 个元素的地址为 1000，那么每个一维数组的首地址如下图所示
![在这里插入图片描述](https://img-blog.csdnimg.cn/20181206153703444.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L2Rzc19kc3Nzc2Q=,size_16,color_FFFFFF,t_70)
先看一下以下定义：
`int (*p)[4] = a;`

括号中的`*`表明 `p`是一个指针，它指向一个数组，数组的类型为`int [4]`，这正是`a`所包含的每个一维数组的类型

**注意：**
`[ ]`的优先级高于`*`，`( )`是必须要加的，如果赤裸裸地写作`int *p[4]`，那么应该理解为`int *(p[4])`，p 就成了一个指针数组，而不是二维数组指针。

对指针进行加法（减法）运算时，它前进（后退）的步长与**它指向的数据类型有关**，p 指向的数据类型是int [4]，那么p+1就前进 4×4 = 16 个字节，p-1就后退 16 个字节，这正好是数组 a 所包含的每个一维数组的长度。也就是说，p+1会使得指针指向二维数组的下一行，p-1会使得指针指向数组的上一行。

**数组名a在表达式中也会内转换为和p等价的指针**

访问二维数组：
- 
p指向数组 a 的开头，也即第 0 行；p+1前进一行，指向第 1 行。

- 
*(p+1)表示取地址上的数据，也就是整个第 1 行数据。注意是一行数据，是多个数据，不是第 1 行中的第 0 个元素


```
int a[3][4] = { {0, 1, 2, 3}, {4, 5, 6, 7}, {8, 9, 10, 11} };
	int(*p)[4] = a;
	printf("%d\n", sizeof(*(p + 1)));
	printf("%d\n", sizeof(*(a + 1)));
```

out:

> 
16

16


3）*(p+1)+1表示第 1 行第 1 个元素的地址

*(p+1)单独使用时表示的是第 1 行数据，放在**表达式中**会被转换为第 1 行数据的首地址，也就是第 1 行第 0 个元素的地址；就像一维数组的名字，在**定义时或者和 sizeof、& 一起使用时才表示整个数组**，出现在表达式中就会被转换为指向数组第 0 个元素的指针

```
int a[3][4] = { {0, 1, 2, 3}, {4, 5, 6, 7}, {8, 9, 10, 11} };
	int(*p)[4] = a;
	//printf("%d\n", sizeof(*(p + 1)));
	//printf("%d\n", sizeof(*(a + 1)));
	printf("%#x\n", *(p + 1));
	printf("%#x\n", *(p + 1) + 1);
	
	printf("%#x\n", *(p + 2));
```

out:

> 
0xddfb44

0xddfb48

0xddfb54
4)*(*(p+1)+1)表示第 1 行第 1 个元素的值

根据以上结论，可以退出一下等价关系：

```
a+i == p+i
a[i] == p[i] == *(a+i) == *(p+i)
a[i][j] == p[i][j] == *(a[i]+j) == *(p[i]+j) == *(*(a+i)+j) == *(*(p+i)+j)
```

二维数组遍历方式：

```
int a[3][4] = { {0, 1, 2, 3}, {4, 5, 6, 7}, {8, 9, 10, 11} };
	int(*p)[4] = a;
	int i, j;
	//下标访问
	for (i = 0; i < 3; i++) {
		for (j = 0; j < 4; j++) printf("%2d  ", a[i][j]);
		printf("\n");
	}
	//指针访问1
	for (i = 0; i < 3; i++) {
		for (j = 0; j < 4; j++) printf("%2d  ", a[i][j]);
		printf("\n");
	}
	//指针访问2

	for (i = 0; i < 3; i++) {
		for (j = 0; j < 4; j++) printf("%2d  ", *(*(p+i)+j));
		printf("\n");
	}
	//指针访问3
	for (i = 0; i < 3; i++) {
		for (j = 0; j < 4; j++) printf("%2d  ", *(p[i] + j));
		printf("\n");
	}
```

### 13.函数指针

函数在内存中占据一段连续的内存区域，函数名在表达式中也被转换为函数所在内存区域的首地址，和数组名类似。可以将函数的首地址赋值给指针变量，然后可以通过指针变量就可以调用该函数，这种指针称为函数指针。

定义：

`returnType (*pointerName)(param list);`

returnType 为函数返回值类型，pointerNmae 为指针名称，param list 为函数参数列表。参数列表中可以同时给出参数的类型和名称，也可以只给出参数的类型，省略参数的名称，这一点和**函数声明**类似，但是注意**指针参数列表和要指向的函数参数列表要完全一样**

```
int add(int, int);
int main() {
	int a = 10, b = 5;
	int(*padd)(int, int) = add;
	int addition = (*padd)(a, b);
	printf("addition of a and b:%d", addition);
	system("pause");
	return 0;

	

}

int add(int a , int b) {
	return a + b;
}
```

out:

> 
addition of a and b:15


### 14 总结：

![常见指针变量的定义](https://img-blog.csdnimg.cn/20181206160908932.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L2Rzc19kc3Nzc2Q=,size_16,color_FFFFFF,t_70)

### 15 动态分配内存

##### c
- malloc(size) 分配内存
- free() 释放指针

$size = n * sizeof(typename)$

比如要分配长度16的double数组：

```
int n = 16;
	double *arr = NULL;//声明，并指向NULL，避免野指针
	arr = (double *)malloc(n * sizeof(double));
	if (!arr) {
		printf("分配失败！！！");
	}
	//赋值
	for (int i = 0; i < 16; i++) {
		arr[i] = i;
	}
	for (int i = 0; i < 16; i++) {
		printf("%f\n", arr[i]);
	}
	free(arr);//释放指针
```

##### c++
- new 动态申请
- delete 释放内存

在c++中，malloc函数依然存在，但是推荐使用new关键字，因为与malloc相比，new不仅分配了内存，还创建了对象。
- 
`指针变量名=new 类型(初始化)`

- 
`delete 指针名`

new表达式的操作序列为：在自有存储区(栈区)为对象或变量分贝内存，然后使用括号中的值初始化该对象或变量。 new表达式是调用库中操作符new()完成的

比如`int *pi=new int(0);`

而当pi指向的对象或变量的声明周期结束时，需要释放pi所指向的目标所占用的内存空间：
`delete pi;`

注意的是，释放了pi所指向的内存空间，但是并未释放指针pi本身，也就是说pi本省所占用的内存空间并未释放。

c++ vs有保护机制
几种初始化方式：

```
//1
int *p=NULL;
p=new int(100);
//2
int *p=new int;
*p=100;
//3
int *p=new int[100];
```

##### 关于数组delete释放需要注意：

一次只能释放申请的一维的数组地址，如果是二维数组，要先释放列在释放行
- 一维数组的的申请释放
`指针变量名=new 类型名[长度]；`
`delete []指针变量名`
```
int n;
	char *pc;
	cout << "请输入动态数组的元素个数" << endl;
	cin >> n;
	pc = new char[n];//申请25个字符，可以装下12个汉字和1个结束符
	strcpy(pc, "自由存储区内存的动态分配");
	cout << pc << endl;
	delete[]pc;
```
- 二维动态数组的创建和释放
**加上变量名理解！！！**

```
int **array
// 假定数组第一维长度为 m， 第二维长度为 n

// 动态分配空间
array = new int *[m];//分配m行的内存空间，指向各行的指针数组,每一个元素指向一行
for( int i=0; i<m; i++ )
{
    array[i] = new int [n] ;//为每一行分配内存空间
}
//赋值
for(int i=0;i<m;++i)
	for(int j=0;j<n;++j){
	array[i][j] = i * n + j;
	} 

//显示
for(int i=0;i<m;++i){
	for(int j=0;j<n;++j){
		cout<<array[i][j] <<' ';
	}
	cout<<endl;
 }
	

//释放
for( int i=0; i<m; i++ )
{
    delete [] arrar[i];
}
delete [] array;
```

指向结构体/类：

```
struct Share {
	long int share_price;
	string time_date;
};


int main()
{
    //动态创建结构体指针
	Share *pS = new Share;
	pS->share_price = 100;
	pS->time_date = "2017_09_10 09:10";

	//定义结构体数组
	Share *pSa = new Share[2];
	//赋值
	for (int i = 0; i < 2; ++i) {
		pSa->share_price = (i + 1) * 100;
		pSa->time_date = "2017_09_10";
	}
	}
```

参考：
- 指针 [http://c.biancheng.net/cpp/html/87.html](http://c.biancheng.net/cpp/html/87.html)
- 动态分配内存[https://blog.csdn.net/qq_29924041/article/details/54897204](https://blog.csdn.net/qq_29924041/article/details/54897204)
- [http://www.runoob.com/cplusplus/cpp-dynamic-memory.html](http://www.runoob.com/cplusplus/cpp-dynamic-memory.html)
































