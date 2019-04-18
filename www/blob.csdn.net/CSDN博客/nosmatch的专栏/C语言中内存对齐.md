# C语言中内存对齐 - nosmatch的专栏 - CSDN博客
2011年07月27日 20:38:44[nosmatch](https://me.csdn.net/HDUTigerkin)阅读数：1011
- #include <stdio.h>
- 
- void print()  
- {  
-   printf("hello world!\n");  
-   return ;  
- }  
- void main()  
- {  
-   printf("%d\n",sizeof(print()));   
-   return ;  
- }  
#include <stdio.h>void print(){  printf("hello world!\n");  return ;}void main(){  printf("%d\n",sizeof(print()));   return ;}
这段代码在linux环境下我采用gcc编译是没有任何问题的，对于void类型，其长度为1，但是如果我们在vc6下面运行的话话就会出现illegal sizeof operand错误，所以我们称之为操作符更加的准确些，既然是操作符，那么我们来看看它的几种使用方式：
1、sizeof( object );            // sizeof( 对象 )；
2、 sizeof( type_name ); // sizeof( 类型 )；
3、sizeof object;               // sizeof 对象; 通常这种写法我们在代码中都不会使用，所以很少见到。
下面来看段代码加深下印象：
#include <stdio.h>
- 
- void main()  
- {  
-     int i;  
-     printf("sizeof(i):\t%d\n",sizeof(i));  
-     printf("sizeof(4):\t%d\n",sizeof(4));   
-     printf("sizeof(4+2.5):\t%d\n",sizeof(4+2.5));   
-     printf("sizeof(int):\t%d\n",sizeof(int));   
-     printf("sizeof 5:\t%d\n",sizeof 5);   
-     return ;  
- }  
#include <stdio.h>void main(){int i;printf("sizeof(i):\t%d\n",sizeof(i));printf("sizeof(4):\t%d\n",sizeof(4)); printf("sizeof(4+2.5):\t%d\n",sizeof(4+2.5)); printf("sizeof(int):\t%d\n",sizeof(int)); printf("sizeof 5:\t%d\n",sizeof 5); return ;}
运行结果为：
![](http://hi.csdn.net/attachment/201107/25/0_1311560827cfDD.gif)
[view plain](#)[copy to clipboard](#)[print](#)[?](#)
- sizeof(i):      4  
- sizeof(4):      4  
- sizeof(4+2.5):  8  
- sizeof(int):    4  
- sizeof 5:       4  
- Press any key to continue  
sizeof(i):      4sizeof(4):      4sizeof(4+2.5):  8sizeof(int):    4sizeof 5:       4Press any key to continue
从运行结果我们可以看出上面的几种使用方式，实际上，sizeof计算对象的大小也是转换成对对象类型的计算，也就是说，同种类型的不同对象其sizeof值都是一样的。从给出的代码中我们也可以看出sizeof可以对一个表达式求值，编译器根据表达式的最终结果类型来确定大小，但是一般不会对表达式进行计算或者当表达式为函数时并不执行函数体。如：
[view plain](#)[copy to clipboard](#)[print](#)[?](#)
- #include <stdio.h>
- int print()  
- {  
-     printf("Hello bigloomy!");  
-     return 0;  
- }  
- void main()  
- {  
-     printf("sizeof(print()):\t%d\n",sizeof(print()));   
-     return ;  
- }  
#include <stdio.h>int print(){printf("Hello bigloomy!");return 0;}void main(){printf("sizeof(print()):\t%d\n",sizeof(print())); return ;}
运行结果为：
![](http://hi.csdn.net/attachment/201107/25/0_1311561483hz0A.gif)
[view plain](#)[copy to clipboard](#)[print](#)[?](#)
- sizeof(print()):        4  
- Press any key to continue  
sizeof(print()):        4Press any key to continue
从结果我们可以看出print()函数并没有被调用。
 接下来我们来看看linux内核链表里的一个宏：
#define offsetof(TYPE, MEMBER) ((size_t) &((TYPE *)0)->MEMBER)
对这个宏的讲解我们大致可以分为以下4步进行讲解：
1、( (TYPE *)0 ) 0地址强制 "转换" 为 TYPE结构类型的指针；
2、((TYPE *)0)->MEMBER   访问TYPE结构中的MEMBER数据成员；
3、&( ( (TYPE *)0 )->MEMBER)取出TYPE结构中的数据成员MEMBER的地址；
4、(size_t)(&(((TYPE*)0)->MEMBER))结果转换为size_t类型。
宏offsetof的巧妙之处在于将0地址强制转换为 TYPE结构类型的指针，TYPE结构以内存空间首地址0作为起始地址，则成员地址自然为偏移地址。可能有的读者会想是不是非要用0呢？当然不是，我们仅仅是为了计算的简便。也可以使用是他的值，只是算出来的结果还要再减去该数值才是偏移地址。来看看下面的代码：
[view plain](#)[copy to clipboard](#)[print](#)[?](#)
- #include <stdio.h> 
- 
- #define offsetof(TYPE, MEMBER) ((size_t) &((TYPE *)4)->MEMBER) 
- 
- typedefstruct stu1  
- {     
- int a;  
- int b;  
- }stu1;  
- 
- void main()  
- {  
-     printf("offsetof(stu1,a):\t%d\n",offsetof(stu1,a)-4);   
-     printf("offsetof(stu1,b):\t%d\n",offsetof(stu1,b)-4);   
- }  
#include <stdio.h>#define offsetof(TYPE, MEMBER) ((size_t) &((TYPE *)4)->MEMBER)typedef struct stu1{int a;int b;}stu1;void main(){printf("offsetof(stu1,a):\t%d\n",offsetof(stu1,a)-4); printf("offsetof(stu1,b):\t%d\n",offsetof(stu1,b)-4); }
运行结果为：
![](http://hi.csdn.net/attachment/201107/25/0_1311574968KVnG.gif)
[view plain](#)[copy to clipboard](#)[print](#)[?](#)
- offsetof(stu1,a):       0  
- offsetof(stu1,b):       4  
- Press any key to continue
offsetof(stu1,a):       0offsetof(stu1,b):       4Press any key to continue
为了让读者加深印象，我们这里在代码中没有使用0，而是使用的4，所以在最终计算出的结果部分减去了一个4才是偏移地址，当然实际使用中我们都是用的是0。
懂了上面的宏offsetof之后我们再来看看下面的代码：
[view plain](#)[copy to clipboard](#)[print](#)[?](#)
- #include <stdio.h> 
- 
- #define offsetof(TYPE, MEMBER) ((size_t) &((TYPE *)0)->MEMBER) 
- 
- typedefstruct stu1  
- {     
- int a;  
- char b[1];  
- int c;  
- }stu1;  
- 
- void main()  
- {  
-     printf("offsetof(stu1,a):\t%d\n",offsetof(stu1,a));   
-     printf("offsetof(stu1,b):\t%d\n",offsetof(stu1,b));   
-     printf("offsetof(stu1,c):\t%d\n",offsetof(stu1,c));   
-     printf("sizeof(stu1)    :\t%d\n",sizeof(stu1));   
- }  
#include <stdio.h>#define offsetof(TYPE, MEMBER) ((size_t) &((TYPE *)0)->MEMBER)typedef struct stu1{int a;char b[1];int c;}stu1;void main(){printf("offsetof(stu1,a):\t%d\n",offsetof(stu1,a)); printf("offsetof(stu1,b):\t%d\n",offsetof(stu1,b)); printf("offsetof(stu1,c):\t%d\n",offsetof(stu1,c)); printf("sizeof(stu1)    :\t%d\n",sizeof(stu1)); }
运行结果为：
![](http://hi.csdn.net/attachment/201107/25/0_13115754580wDX.gif)
[view plain](#)[copy to clipboard](#)[print](#)[?](#)
- offsetof(stu1,a):       0  
- offsetof(stu1,b):       4  
- offsetof(stu1,c):       8  
- sizeof(stu1)    :       12  
- Press any key to continue
offsetof(stu1,a):       0offsetof(stu1,b):       4offsetof(stu1,c):       8sizeof(stu1)    :       12Press any key to continue
对于字节对齐不了解的读者可能有疑惑的是c的偏移量怎么会是8和结构体的大小怎么会是12呢？因该是sizeof(int)+sizeof(char)+sizeof(int)=9。其实这是编译器对变量存储的一个特殊处理。为了提高CPU的存储速度，编译器对一些变量的起始地址做了对齐处理。在默认情况下，编译器规定各成员变量存放的起始地址相对于结构的起始地址的偏移量必须为该变量的类型所占用的字节数的倍数。现在来分析下上面的代码，如果我们假定a的起始地址为0，它占用了4个字节，那么接下来的空闲地址就是4，是1的倍数，满足要求，所以b存放的起始地址是4，占用一个字节。接下来的空闲地址为5，而c是int变量，占用4个字节，5不是4的整数倍，所以向后移动，找到离5最近的8作为存放c的起始地址，c也占用4字节，所以最后使得结构体的大小为12。现在我们再来看看下面的代码：
[view plain](#)[copy to clipboard](#)[print](#)[?](#)
- #include <stdio.h> 
- 
- typedefstruct stu1  
- {     
- char array[7];  
- }stu1;  
- 
- typedefstruct stu2  
- {     
- double fa;  
- }stu2;  
- 
- typedefstruct stu3  
- {     
-     stu1 s;  
- char str;  
- }stu3;  
- 
- typedefstruct stu4  
- {     
-     stu2 s;  
- char str;   
- }stu4;  
- 
- void main()  
- {  
-     printf("sizeof(stu1)    :\t%d\n",sizeof(stu1));   
-     printf("sizeof(stu2)    :\t%d\n",sizeof(stu2));  
-     printf("sizeof(stu3)    :\t%d\n",sizeof(stu3));  
-     printf("sizeof(stu4)    :\t%d\n",sizeof(stu4));  
- }  
#include <stdio.h>typedef struct stu1{char array[7];}stu1;typedef struct stu2{double fa;}stu2;typedef struct stu3{stu1 s;char str;}stu3;typedef struct stu4{stu2 s;char str; }stu4;void main(){printf("sizeof(stu1)    :\t%d\n",sizeof(stu1)); printf("sizeof(stu2)    :\t%d\n",sizeof(stu2));printf("sizeof(stu3)    :\t%d\n",sizeof(stu3));printf("sizeof(stu4)    :\t%d\n",sizeof(stu4));}
运行结果为：
![](http://hi.csdn.net/attachment/201107/25/0_131158048511yX.gif)
[view plain](#)[copy to clipboard](#)[print](#)[?](#)
- sizeof(stu1)    :       7  
- sizeof(stu2)    :       8  
- sizeof(stu3)    :       8  
- sizeof(stu4)    :       16  
- Press any key to continue
sizeof(stu1)    :       7sizeof(stu2)    :       8sizeof(stu3)    :       8sizeof(stu4)    :       16Press any key to continue
分析下上面我们的运行结果，重点是struct stu3和struct stu4，在struct stu3中使用的是一个字节对齐，因为在stu1和stu3中都只有一个char类型，在struct stu3中我们定义了一个stu1类型的 s，而stu1所占的大小为7，所以加上加上接下来的一个字节str，sizeof(stu3)为8。在stu4中，由于我们定义了一个stu2类型的s，而s是一个double类型的变量，占用8字节，所以接下来在stu4中采用的是8字节对齐。如果我们此时假定stu4中的s从地址0开始存放，占用8个字节，接下来的空闲地址就是8，根据我们上面的讲解可知刚好可以在此存放str。所以变量都分配完空间后stu4结构体所占的字节数为9，但9不是结构体的边界数，也就是说我们要求分配的字节数为结构体中占用空间最大的类型所占用的字节数的整数倍，在这里也就是double类型所占用的字节数8的整数倍，所以接下来还要再分配7个字节的空间，该7个字节的空间没有使用，由编译器自动填充，没有存放任何有意义的东西。 
当然我们也可以使用预编译指令#pragma pack (value)来告诉编译器，使用我们指定的对齐值来取代缺省的。接下来我们来看看一段代码。
[view plain](#)[copy to clipboard](#)[print](#)[?](#)
- #include <stdio.h> 
- 
- #pragma pack (1) /*指定按1字节对齐*/ 
- 
- typedefunion stu1  
- {     
- char str[10];  
- int b;  
- }stu1;  
- 
- #pragma pack () /*取消指定对齐，恢复缺省对齐*/ 
- 
- typedefunion stu2  
- {     
- char str[10];  
- int b;  
- }stu2;  
- 
- void main()  
- {  
-     printf("sizeof(stu1)    :\t%d\n",sizeof(stu1));   
-     printf("sizeof(stu2)    :\t%d\n",sizeof(stu2));   
- }  
#include <stdio.h>#pragma pack (1) /*指定按1字节对齐*/typedef union stu1{char str[10];int b;}stu1;#pragma pack () /*取消指定对齐，恢复缺省对齐*/typedef union stu2{char str[10];int b;}stu2;void main(){printf("sizeof(stu1)    :\t%d\n",sizeof(stu1)); printf("sizeof(stu2)    :\t%d\n",sizeof(stu2)); }
 运行结果为：
![](http://hi.csdn.net/attachment/201107/25/0_1311594831oHW5.gif)
[view plain](#)[copy to clipboard](#)[print](#)[?](#)
- sizeof(stu1)    :       10  
- sizeof(stu2)    :       12  
- Press any key to continue
sizeof(stu1)    :       10sizeof(stu2)    :       12Press any key to continue
现在来分析下上面的代码。由于之前我们一直都在使用struct，所以在这里我们特地例举了一个union的代码来分析下，我们大家都知道union的大小取决于它所有的成员中占用空间最大的一个成员的大小。由于在union  stu1中我们使用了1字节对齐，所以对于stu1来说占用空间最大的是char str[10]类型的数组,，其值为10。为什么stu1为10而stu2却是12呢？因为在stu2的上面我们使用了#pragma pack () ，取消指定对齐，恢复缺省对齐。所以由于stu2其中int类型成员的存在，使stu2的对齐方式变成4字节对齐，也就是说，stu2的大小必须在4的对界上，换句话说就是stu2的大小要是4的整数倍，所以占用的空间变成了12。
