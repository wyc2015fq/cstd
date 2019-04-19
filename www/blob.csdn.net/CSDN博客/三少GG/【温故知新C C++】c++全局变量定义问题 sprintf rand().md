# 【温故知新C/C++】c++全局变量定义问题||sprintf||rand() - 三少GG - CSDN博客
2011年04月29日 04:47:00[三少GG](https://me.csdn.net/scut1135)阅读数：1320标签：[c++																[编译器																[fun																[random																[numbers																[service](https://so.csdn.net/so/search/s.do?q=service&t=blog)](https://so.csdn.net/so/search/s.do?q=numbers&t=blog)](https://so.csdn.net/so/search/s.do?q=random&t=blog)](https://so.csdn.net/so/search/s.do?q=fun&t=blog)](https://so.csdn.net/so/search/s.do?q=编译器&t=blog)](https://so.csdn.net/so/search/s.do?q=c++&t=blog)
今天在头文件里定义了static boost::asio::io_service G_IO_SERV;做全局变量，同时有几个用到这个全局变量的函数包含了这个头文件，在 
程序执行时，这个头文件被包含了几次就会调用几次构造函数，跟了下发现这几个生成的变量地址都不同（这个结论用屁股想也能想出来，肯定不同）。加了#ifndef也不行 
查书，发现static全局变量的有效域是本文件，定义最好还是放cpp里，头文件里只是声明，所以应该如下定义： 
GlobalVar.h 
extern boost::asio::io_service G_IO_SERV; 
/////////////////////////////////////////// 
GlobalVar.cpp 
boost::asio::io_service G_IO_SERV;
+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
造成LNK2005错误主要有以下几种情况： 
1．重复定义全局变量。可能存在两种情况： 
A、对于一些初学编程的程序员，有时候会以为需要使 用全局变量的地方就可以使用定义申明一下。其实这是错误的，全局变量是针对整个工程的。正确的应该是在一个CPP文件中定义如下：int g_Test;那么在使用的CPP文件中就应该使用：extern int g_Test即可，如果还是使用int g_Test，那么就会产生LNK2005错误，一般错误错误信息类似：AAA.obj error LNK2005 int book c？[book@@3HA](mailto:book@@3HA) already defined in BBB.obj。切记的就是不能给变量赋值否则还是会有LNK2005错误。 
这里需要的是“声明”，不是“定义”！根据C++标准的规定，一个变量是声明，必须同时满足两个条件，否则就是定义： 
(1)声明必须使用extern关键字；(2)不能给变量赋初值 
所以，下面的是声明: 
extern int a; 
下面的是定义 
int a; int a = 0; extern int a =0; 
B、对于那么编程不是那么严谨的程序员，总是在需要使用变量的文件中随意定义一个全局变量，并且对于变量名也不予考虑，这也往往容易造成变量名重复，而造成LNK2005错误。 
本文来自CSDN博客，转载请标明出处：[http://blog.csdn.net/scut1135/archive/2011/03/31/6292658.aspx](http://blog.csdn.net/scut1135/archive/2011/03/31/6292658.aspx)
+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
[http://blog.myspace.cn/e/403029127.htm](http://blog.myspace.cn/e/403029127.htm)
# C/C++定义全局变量/常量几种方法的区别
在讨论全局变量之前我们先要明白几个基本的概念： 
1. 编译单元(模块)：
      在IDE开发工具大行其道的今天，对于编译的一些概念很多人已经不再清楚了，很多程序员最怕的就是处理连接错误(LINK ERROR), 因为它不像编译错误那样可以给出你程序错误的具体位置，你常常对这种错误感到懊恼，但是如果你经常使用gcc，makefile等工具在linux或者嵌入式下做开发工作的话，那么你可能非常的理解编译与连接的区别！当在VC这样的开发工具上编写完代码，点击编译按钮准备生成exe文件时，VC其实做了两步工作，第一步，将每个.cpp(.c)和相应.h文件编译成obj文件；第二步，将工程中所有的obj文件进行LINK生成最终的.exe文件，那么错误就有可能在两个地方产生，一个是编译时的错误，这个主要是语法错误，另一个是连接错误，主要是重复定义变量等。我们所说的编译单元就是指在编译阶段生成的每个obj文件，一个obj文件就是一个编译单元，也就是说一个cpp(.c)和它相应的.h文件共同组成了一个编译单元，一个工程由很多个编译单元组成，每个obj文件里包含了变量存储的相对地址等 。
2. 声明与定义的区别
函数或变量在声明时，并没有给它实际的物理内存空间，它有时候可以保证你的程序编译通过， 但是当函数或变量定义的时候，它就在内存中有了实际的物理空间，如果你在编译模块中引用的外部变量没有在整个工程中任何一个地方定义的话， 那么即使它在编译时可以通过，在连接时也会报错，因为程序在内存中找不到这个变量！你也可以这样理解， 对同一个变量或函数的声明可以有多次，而定义只能有一次! 
3. extern的作用
      extern有两个作用，第一个,当它与" C" 一起连用时，如: extern " C" void fun(int a, int b); 则告诉编译器在编译fun这个函数名时按着C的规则去翻译相应的函数名而不是C++的， C++的规则在翻译这个函数名时会把fun这个名字变得面目全非，可能是fun@aBc_int_int#%$也可能是别的，这要看编译器的" 脾气" 了(不同的编译器采用的方法不一样)，为什么这么做呢，因为C++支持函数的重载啊，在这里不去过多的论述这个问题，如果你有兴趣可以去网上搜索，相信你可以得到满意的解释!
      当extern不与" C" 在一起修饰变量或函数时，如在头文件中: extern int g_Int;   它的作用就是声明函数或全局变量的作用范围的关键字，其声明的函数和变量可以在本模块活其他模块中使用，记住它是一个声明不是定义!也就是说B模块(编译单元)要是引用模块(编译单元)A中定义的全局变量或函数时，它只要包含A模块的头文件即可, 在编译阶段，模块B虽然找不到该函数或变量，但它不会报错，它会在连接时从模块A生成的目标代码中找到此函数。 
      如果你对以上几个概念已经非常明白的话，那么让我们一起来看以下几种全局变量/常量的使用区别: 
1. 用extern修饰的全局变量
      以上已经说了extern的作用，下面我们来举个例子,如:
      在test1.h中有下列声明:
      #ifndef TEST1H
      #define TEST1H
      extern char g_str[]; // 声明全局变量g_str
      void fun1(); 
      #endif
      在test1.cpp中
      #include " test1.h" 
      char g_str[] = " 123456" ; // 定义全局变量g_str
      void fun1()
      { 
              cout < < g_str < < endl; 
      } 
      以上是test1模块， 它的编译和连接都可以通过,如果我们还有test2模块也想使用g_str,只需要在原文件中引用就可以了
      #include " test1.h" 
      void fun2()
      { 
              cout < < g_str < < endl; 
      } 
      以上test1和test2可以同时编译连接通过，如果你感兴趣的话可以用ultraEdit打开test1.obj,你可以在里面着" 123456" 这个字符串,但是你却不能在test2.obj里面找到，这是因为g_str是整个工程的全局变量，在内存中只存在一份, test2.obj这个编译单元不需要再有一份了，不然会在连接时报告重复定义这个错误!
      有些人喜欢把全局变量的声明和定义放在一起，这样可以防止忘记了定义，如把上面test1.h改为
      extern char g_str[] = " 123456" ; // 这个时候相当于没有extern
      然后把test1.cpp中的g_str的定义去掉,这个时候再编译连接test1和test2两个模块时，会报连接错误，这是因为你把全局变量g_str的定义放在了头文件之后，test1.cpp这个模块包含了test1.h所以定义了一次g_str,而 test2.cpp也包含了test1.h所以再一次定义了g_str, 这个时候连接器在连接test1和test2时发现两个g_str。如果你非要把g_str的定义放在test1.h中的话，那么就把test2的代码中#include " test1.h" 去掉 换成:
      extern char g_str[]; 
      void fun2()
      { 
              cout < < g_str < < endl; 
      } 
      这个时候编译器就知道g_str是引自于外部的一个编译模块了，不会在本模块中再重复定义一个出来，但是我想说这样做非常糟糕，因为你由于无法在test2.cpp中使用#include " test1.h" , 那么test1.h中声明的其他函数你也无法使用了，除非也用都用extern修饰，这样的话你光声明的函数就要一大串，而且头文件的作用就是要给外部提供接口使用的，所以 请记住， 只在头文件中做声明，真理总是这么简单。 
2. 用static修饰的全局变量
      首先，我要告诉你static与extern是一对“水火不容”的家伙，也就是说extern和static不能同时修饰一个变量；其次，static修饰的全局变量声明与定义同时进行，也就是说当你在头文件中使用static声明了全局变量后，它也同时被定义了；最后，static修饰全局变量的作用域只能是本身的编译单元，也就是说它的“全局”只对本编译单元有效，其他编译单元则看不到它,如:
      test1.h:
      #ifndef TEST1H
      #define TEST1H
      static char g_str[] = " 123456" ; 
      void fun1(); 
      #endif 
      test1.cpp:
      #include " test1.h" 
      void fun1()
      { 
              cout < < g_str < < endl; 
      } 
      test2.cpp
      #include " test1.h" 
      void fun2()
      { 
              cout < < g_str < < endl; 
      } 
      以上两个编译单元可以连接成功, 当你打开test1.obj时，你可以在它里面找到字符串" 123456" , 同时你也可以在test2.obj中找到它们，它们之所以可以连接成功而没有报重复定义的错误是因为虽然它们有相同的内容，但是存储的物理地址并不一样，就像是两个不同变量赋了相同的值一样，而这两个变量分别作用于它们各自的编译单元。
      也许你比较较真，自己偷偷的跟踪调试上面的代码,结果你发现两个编译单元（test1, test2）的g_str的内存地址相同，于是你下结论static修饰的变量也可以作用于其他模块，但是我要告诉你，那是你的编译器在欺骗你，大多数编译器都对代码都有优化功能，以达到生成的目标程序更节省内存，执行效率更高，当编译器在连接各个编译单元的时候，它会把相同内容的内存只拷贝一份，比如上面的" 123456" , 位于两个编译单元中的变量都是同样的内容，那么在连接的时候它在内存中就只会存在一份了， 如果你把上面的代码改成下面的样子，你马上就可以拆穿编译器的谎言:
      test1.cpp:
      #include " test1.h" 
      void fun1()
      { 
              g_str[0] = 'a'; 
              cout < < g_str < < endl; 
      } 
      test2.cpp
      #include " test1.h" 
      void fun2()
      { 
              cout < < g_str < < endl; 
      } 
      void main()
      { 
              fun1(); // a23456
              fun2(); // 123456
      } 
      这个时候你在跟踪代码时，就会发现两个编译单元中的g_str地址并不相同，因为你在一处修改了它，所以编译器被强行的恢复内存的原貌，在内存中存在了两份拷贝给两个模块中的变量使用。 
      正是因为static有以上的特性，所以一般定义static全局变量时，都把它放在原文件中而不是头文件，这样就不会给其他模块造成不必要的信息污染，同样记住这个原则吧！
3 const修饰的全局常量 
      const修饰的全局常量用途很广，比如软件中的错误信息字符串都是用全局常量来定义的。const修饰的全局常量据有跟static相同的特性，即它们只能作用于本编译模块中，但是const可以与extern连用来声明该常量可以作用于其他编译模块中, 如
      extern const char g_str[]; 
      然后在原文件中别忘了定义:
      const char g_str[] = " 123456" ; 
      所以当const单独使用时它就与static相同，而当与extern一起合作的时候，它的特性就跟extern的一样了！所以对const我没有什么可以过多的描述，我只是想提醒你，const char* g_str = " 123456" 与 const char g_str[] = " 123465" 是不同的， 前面那个const 修饰的是char * 而不是g_str,它的g_str并不是常量，它被看做是一个定义了的全局变量（可以被其他编译单元使用）， 所以如果你像让char *g_str遵守const的全局常量的规则，最好这么定义const char* const g_str=" 123456" .
+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
sprintf
参数说明及应用举例　　sprintf格式的规格如下所示。[]中的部分是可选的。 
　　%[指定参数$][标识符][宽度][.精度]指示符 
　　若想输出**`%'本身时, 请这样`%%'处理。 **
　　1. 处理字符方向。负号时表示从后向前处理。 
　　2. 填空字元。 0 的话表示空格填 0；空格是内定值，表示空格就放着。 
　　3. 字符总宽度。为最小宽度。 
　　4. 精确度。指在小数点后的浮点数位数。 
　　=-=-=-=-=-=-=-=-=-=-=-=-=-=-=- 
### 转换字符
　　=-=-=-=-=-=-=-=-=-=-=-=-=-=-=- 
　　%% 印出百分比符号，不转换。 
　　%c 整数转成对应的 ASCII 字元。 
　　%d 整数转成十进位。 
　　%f 倍精确度数字转成浮点数。 
　　%o 整数转成八进位。 
　　%s 整数转成字符串。 
　　%x 整数转成小写十六进位。 
　　%X 整数转成大写十六进位。 
　　=-=-=-=-=-=-=-=-=-=-=-=-=-=-=- 
+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
rand（产生随机数）  
相关函数 
srand
 
表头文件 
#include<stdlib.h>
 
定义函数 
int rand(void)
 
函数说明 
rand()会返回一随机数值，范围在0至RAND_MAX 间。在调用此函数产生随机数前，必须先利用srand()设好随机数种子，如果未设随机数种子，rand()在调用时会自动设随机数种子为1。关于随机数种子请参考srand()。
 
返回值 
返回0至RAND_MAX之间的随机数值，RAND_MAX定义在stdlib.h，其值为2147483647。
 
范例 
/* 产生介于1 到10 间的随机数值，此范例未设随机数种子，完整的随机数产生请参考
srand（）*/
#include<stdlib.h>
main()
{
int i,j;
for(i=0;i<10;i++)
{
j=1+(int)(10.0*rand()/(RAND_MAX+1.0));
printf("%d ",j);
}
}
 
执行 
9 4 8 8 10 2 4 8 3 6
9 4 8 8 10 2 4 8 3 6
 
srand（设置随机数种子）  
相关函数 
rand
 
表头文件 
#include<stdlib.h>
 
定义函数 
void srand (unsigned int seed);
 
函数说明 
srand()用来设置rand()产生随机数时的随机数种子。参数seed必须是个整数，通常可以利用geypid()或time(0)的返回值来当做seed。如果每次seed都设相同值，rand()所产生的随机数值每次就会一样。
 
返回值 
范例 
/* 产生介于1 到10 间的随机数值，此范例与执行结果可与rand（）参照*/
#include<time.h>
#include<stdlib.h>
main()
{
int i,j;
srand((int)time(0));
for(i=0;i<10;i++)
{
j=1+(int)(10.0*rand()/(RAND_MAX+1.0));
printf(" %d ",j);
}
}
 
执行 
5 8 8 8 10 2 10 8 9 9
2 9 7 4 10 3 2 10 8 7rand()函数是产生随机数的一个随机函数。C语言里还有 srand()函数等。 
### 详述rand（）
　　(1)使用干函数首先应在开头包含头文件[stdlib.h](view/1347718.htm) 　　#include<stdlib.h> 　　(2)在标准的C库中函数rand()可以生成0~[RAND_MAX](view/3604994.htm)之间的一个随机数，其中RAND_MAX 是stdlib.h 中定义的一个整数，它与系统有关。 　　(3)rand()函数没有输入参数，直接通过表达式rand()来引用；例如可以用下面的语句来打印两个随机数： 　　printf("Random numbers are: %i %i/n",rand(),rand()); 　　(4)因为rand()函数是按指定的顺序来产生整数，因此每次执行上面的语句都打印相同的两个值，所以说C语言的随机并不是真正意义上的随机。 　　(5)为了使程序在每次执行时都能生成一个新序列的随机值，我们通常通过为随机数生成器提供一粒新的随机种子。函数 srand()(来自stdlib.h)可以为随机数生成器播散种子。只要种子不同rand()函数就会产生不同的随机数序列。srand()称为随机数生成器的初始化器。 
### 例子
　　文件名： rand_srand.c 　　/* This program generates and prints ten random integers between 1 and RAND_MAX*/ 　　#include <stdio.h> 　　#include <stdlib.h> 　　int main() 　　{ 　　unsigned int seed; /*申明初始化器的种子，注意是usigned int 型的*/ 　　int k; 　　printf("Enter a positive integer seed value: /n"); 　　scanf("%u",&seed); 　　srand(seed); 　　printf("Random Numbers are:/n"); 　　for(k = 1; k <= 10; k++) 　　{ 　　printf("%i",rand()); 　　printf("/n"); 　　} 　　return 0; 　　} 　　当提供的种子相同时，随机数序列也是相同的。而且当种子为1时，与不使用srand()函数时一样的，也就是说rand()函数默认情况下初始化种子值为1； 　　在stdlib.h 中这两个函数的原型是： 　　int rand(); 　　void srand (unsigned int); 　　srand(time(0)); i=rand(); 这样i就是一个真正意义上的随机数。 　　rand()产生伪随机数，srand函数提供种子，种子不同产生的随机数序列也不同，所以通常先调用srand函数** time(0)返回的是系统的时间（从1970.1.1午夜算起），单位：秒，**种子不同当然产生的随机数相同几率就很小了。
