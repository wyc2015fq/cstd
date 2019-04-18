# 关于sizeof关键字的用法  转载 - ljx0305的专栏 - CSDN博客
2007年10月08日 16:19:00[ljx0305](https://me.csdn.net/ljx0305)阅读数：896
一、sizeof的概念　 
　　sizeof是C语言的一种单目操作符，如C语言的其他操作符++、--等。它并不是函数。sizeof操作符以字节形式给出了其操作数的存储大小。操作数可以是一个表达式或括在括号内的类型名。操作数的存储大小由操作数的类型决定。　 
二、sizeof的使用方法　 
　　1、用于数据类型　 
　　sizeof使用形式：sizeof（type）　 
　　数据类型必须用括号括住。如sizeof（int）。　 
　　2、用于变量　 
　　sizeof使用形式：sizeof（var_name）或sizeof　var_name　 
　　变量名可以不用括号括住。如sizeof　(var_name)，sizeof　var_name等都是正确形式。带括号的用法更普遍，大多数程序员采用这种形式。　 
　　注意：sizeof操作符不能用于函数类型，不完全类型或位字段。不完全类型指具有未知存储大小的数据类型，如未知存储大小的数组类型、未知内容的结构或联合类型、void类型等。　 
　　如sizeof(max)若此时变量max定义为int　max(),sizeof(char_v)　若此时char_v定义为char　char_v　[MAX]且MAX未知，sizeof(void)都不是正确形式。　 
三、sizeof的结果　 
　　sizeof操作符的结果类型是size_t，它在头文件 
中typedef为unsigned　int类型。该类型保证能容纳实现所建立的最大对象的字节大小。　 
　　1、若操作数具有类型char、unsigned　char或signed　char，其结果等于1。　 
　　ANSI　C正式规定字符类型为1字节。　 
　　2、int、unsigned　int　、short　int、unsigned　short　、long　int　、unsigned　long　、float、double、long　double类型的sizeof　在ANSI　C中没有具体规定，大小依赖于实现，一般可能分别为2、2、2、2、4、4、4、8、10。　 
　　3、当操作数是指针时，sizeof依赖于编译器。例如Microsoft　C/C++7.0中，near类指针字节数为2，far、huge类指针字节数为4。一般Unix的指针字节数为4。　 
　　4、当操作数具有数组类型时，其结果是数组的总字节数。　 
　　5、联合类型操作数的sizeof是其最大字节成员的字节数。结构类型操作数的sizeof是这种类型对象的总字节数，包括任何垫补在内。　 
　　让我们看如下结构：　 
　　struct　{char　b;　double　x;}　a;　 
　　在某些机器上sizeof（a）=12，而一般sizeof（char）+　sizeof（double）=9。　 
　　这是因为编译器在考虑对齐问题时，在结构中插入空位以控制各成员对象的地址对齐。如double类型的结构成员x要放在被4整除的地址。　 
　　6、如果操作数是函数中的数组形参或函数类型的形参，sizeof给出其指针的大小。　 
四、sizeof与其他操作符的关系　 
　　sizeof的优先级为2级，比/、%等3级运算符优先级高。它可以与其他操作符一起组成表达式。如i*sizeof（int）；其中i为int类型变量。　 
五、sizeof的主要用途　 
　　1、sizeof操作符的一个主要用途是与存储分配和I/O系统那样的例程进行通信。例如：　 
　　void　*malloc（size_t　size）,　 
　　size_t　fread(void　*　ptr,size_t　size,size_t　nmemb,FILE　*　stream)。　 
　　2、sizeof的另一个的主要用途是计算数组中元素的个数。例如：　 
　　void　*　memset（void　*　s,int　c,sizeof(s)）。　 
六、建议　 
　　由于操作数的字节数在实现时可能出现变化，建议在涉及到操作数字节大小时用sizeof来代替常量计算。
本文主要包括二个部分，第一部分重点介绍在VC中，怎么样采用sizeof来求结构的大小，以及容易出现的问题，并给出解决问题的方法，第二部分总结出VC中sizeof的主要用法。 
1、 sizeof应用在结构上的情况 
请看下面的结构： 
struct MyStruct 
{ 
double dda1; 
char dda; 
int type 
}; 
对结构MyStruct采用sizeof会出现什么结果呢？sizeof(MyStruct)为多少呢？也许你会这样求： 
sizeof(MyStruct)=sizeof(double)+sizeof(char)+sizeof(int)=13 
但是当在VC中测试上面结构的大小时，你会发现sizeof(MyStruct)为16。你知道为什么在VC中会得出这样一个结果吗？ 
其实，这是VC对变量存储的一个特殊处理。为了提高CPU的存储速度，VC对一些变量的起始地址做了“对齐”处理。在默认情况下，VC规定各成员变量存放的起始地址相对于结构的起始地址的偏移量必须为该变量的类型所占用的字节数的倍数。下面列出常用类型的对齐方式(vc6.0,32位系统)。 
类型 
对齐方式（变量存放的起始地址相对于结构的起始地址的偏移量） 
Char 
偏移量必须为sizeof(char)即1的倍数 
int 
偏移量必须为sizeof(int)即4的倍数 
float 
偏移量必须为sizeof(float)即4的倍数 
double 
偏移量必须为sizeof(double)即8的倍数 
Short 
偏移量必须为sizeof(short)即2的倍数 
各成员变量在存放的时候根据在结构中出现的顺序依次申请空间，同时按照上面的对齐方式调整位置，空缺的字节VC会自动填充。同时VC为了确保结构的大小为结构的字节边界数（即该结构中占用最大空间的类型所占用的字节数）的倍数，所以在为最后一个成员变量申请空间后，还会根据需要自动填充空缺的字节。 
下面用前面的例子来说明VC到底怎么样来存放结构的。 
struct MyStruct 
{ 
double dda1; 
char dda; 
int type 
}； 
为上面的结构分配空间的时候，VC根据成员变量出现的顺序和对齐方式，先为第一个成员dda1分配空间，其起始地址跟结构的起始地址相同（刚好偏移量0刚好为sizeof(double)的倍数），该成员变量占用sizeof(double)=8个字节；接下来为第二个成员dda分配空间，这时下一个可以分配的地址对于结构的起始地址的偏移量为8，是sizeof(char)的倍数，所以把dda存放在偏移量为8的地方满足对齐方式，该成员变量占用sizeof(char)=1个字节；接下来为第三个成员type分配空间，这时下一个可以分配的地址对于结构的起始地址的偏移量为9，不是sizeof(int)=4的倍数，为了满足对齐方式对偏移量的约束问题，VC自动填充3个字节（这三个字节没有放什么东西），这时下一个可以分配的地址对于结构的起始地址的偏移量为12，刚好是sizeof(int)=4的倍数，所以把type存放在偏移量为12的地方，该成员变量占用sizeof(int)=4个字节；这时整个结构的成员变量已经都分配了空间，总的占用的空间大小为：8+1+3+4=16，刚好为结构的字节边界数（即结构中占用最大空间的类型所占用的字节数sizeof(double)=8）的倍数，所以没有空缺的字节需要填充。所以整个结构的大小为：sizeof(MyStruct)=8+1+3+4=16，其中有3个字节是VC自动填充的，没有放任何有意义的东西。 
下面再举个例子，交换一下上面的MyStruct的成员变量的位置，使它变成下面的情况： 
struct MyStruct 
{ 
char dda; 
double dda1; 
int type 
}； 
这个结构占用的空间为多大呢？在VC6.0环境下，可以得到sizeof(MyStruc)为24。结合上面提到的分配空间的一些原则，分析下VC怎么样为上面的结构分配空间的。（简单说明） 
struct MyStruct 
{ 
char dda;//偏移量为0，满足对齐方式，dda占用1个字节； 
double dda1;//下一个可用的地址的偏移量为1，不是sizeof(double)=8 
//的倍数，需要补足7个字节才能使偏移量变为8（满足对齐 
//方式），因此VC自动填充7个字节，dda1存放在偏移量为8 
//的地址上，它占用8个字节。 
int type；//下一个可用的地址的偏移量为16，是sizeof(int)=4的倍 
//数，满足int的对齐方式，所以不需要VC自动填充，type存 
//放在偏移量为16的地址上，它占用4个字节。 
}；//所有成员变量都分配了空间，空间总的大小为1+7+8+4=20，不是结构 
//的节边界数（即结构中占用最大空间的类型所占用的字节数sizeof 
//(double)=8）的倍数，所以需要填充4个字节，以满足结构的大小为 
//sizeof(double)=8的倍数。 
所以该结构总的大小为：sizeof(MyStruc)为1+7+8+4+4=24。其中总的有7+4=11个字节是VC自动填充的，没有放任何有意义的东西。 
VC对结构的存储的特殊处理确实提高CPU存储变量的速度，但是有时候也带来了一些麻烦，我们也屏蔽掉变量默认的对齐方式，自己可以设定变量的对齐方式。 
VC中提供了#pragma pack(n)来设定变量以n字节对齐方式。n字节对齐就是说变量存放的起始地址的偏移量有两种情况：第一、如果n大于等于该变量所占用的字节数，那么偏移量必须满足默认的对齐方式，第二、如果n小于该变量的类型所占用的字节数，那么偏移量为n的倍数，不用满足默认的对齐方式。结构的总大小也有个约束条件，分下面两种情况：如果n大于所有成员变量类型所占用的字节数，那么结构的总大小必须为占用空间最大的变量占用的空间数的倍数； 
否则必须为n的倍数。下面举例说明其用法。 
#pragma pack(push) //保存对齐状态 
#pragma pack(4)//设定为4字节对齐 
struct test 
{ 
char m1; 
double m4; 
int m3; 
}; 
#pragma pack(pop)//恢复对齐状态 
以上结构的大小为16，下面分析其存储情况，首先为m1分配空间，其偏移量为0，满足我们自己设定的对齐方式（4字节对齐），m1占用1个字节。接着开始为m4分配空间，这时其偏移量为1，需要补足3个字节，这样使偏移量满足为n=4的倍数（因为sizeof(double)大于n）,m4占用8个字节。接着为m3分配空间，这时其偏移量为12，满足为4的倍数，m3占用4个字节。这时已经为所有成员变量分配了空间，共分配了16个字节，满足为n的倍数。如果把上面的#pragma pack(4)改为#pragma pack(16)，那么我们可以得到结构的大小为24。（请读者自己分析） 
2、 sizeof用法总结 
在VC中，sizeof有着许多的用法，而且很容易引起一些错误。下面根据sizeof后面的参数对sizeof的用法做个总结。 
A． 参数为数据类型或者为一般变量。例如sizeof(int),sizeof(long)等等。这种情况要注意的是不同系统系统或者不同编译器得到的结果可能是不同的。例如int类型在16位系统中占2个字节，在32位系统中占4个字节。 
B． 参数为数组或指针。下面举例说明. 
int a[50]; //sizeof(a)=4*50=200; 求数组所占的空间大小 
int *a=new int[50];// sizeof(a)=4; a为一个指针，sizeof(a)是求指针 
//的大小,在32位系统中，当然是占4个字节。 
C． 参数为结构或类。Sizeof应用在类和结构的处理情况是相同的。但有两点需要注意，第一、结构或者类中的静态成员不对结构或者类的大小产生影响，因为静态变量的存储位置与结构或者类的实例地址无关。 
第二、没有成员变量的结构或类的大小为1，因为必须保证结构或类的每一 
个实例在内存中都有唯一的地址。 
下面举例说明， 
Class Test{int a;static double c};//sizeof(Test)=4. 
Test *s;//sizeof(s)=4,s为一个指针。 
Class test1{ };//sizeof(test1)=1; 
D． 参数为其他。下面举例说明。 
int func(char s[5]); 
{ 
cout< 
//数的参数在传递的时候系统处理为一个指针，所 
//以sizeof(s)实际上为求指针的大小。 
return 1; 
} 
sizeof(func(“1234”))=4//因为func的返回类型为int，所以相当于 
//求sizeof(int). 
以上为sizeof的基本用法，在实际的使用中要注意分析VC的分配变量的分配策略，这样的话可以避免一些错误。
转载自[：http://bbs.chinaunix.net/viewthread.php?tid=844212&extra=page%3D1](http://bbs.chinaunix.net/viewthread.php?tid=844212&extra=page%3D1)
