# 嵌入式 C 语言的可变参数表函数的设计 - DoubleLi - 博客园






首先在介绍可变参数表函数的设计之前，我们先来介绍一下最经典的可变参数表printf函数的实现原理。
一、printf函数的实现原理
在C/C++中，对函数参数的扫描是从后向前的。C/C++的函数参数是通过压入堆栈的方式来给函数传参数的（堆栈是一种先进后出的数据结构），最先压入的参数最后出来，在计算机的内存中，数据有2块，一块是堆，一块是栈（函数参数及局部变量在这里），而栈是从内存的高地址向低地址生长的，控制生长的就是堆栈指针了，最先压入的参数是在最上面，就是说在所有参数的最后面，最后压入的参数在最下面，结构上看起来是第一个，所以最后压入的参数总是能够被函数找到，因为它就在堆栈指针的上方。printf的第一个被找到的参数就是那个字符指针，就是被双引号括起来的那一部分，函数通过判断字符串里控制参数的个数来判断参数个数及数据类型，通过这些就可算出数据需要的堆栈指针的偏移量了，下面给出printf("%d,%d",a,b);（其中a、b都是int型的）的汇编代码



**[cpp]**[view plain](http://blog.csdn.net/hackbuteer1/article/details/7558979)[copy](http://blog.csdn.net/hackbuteer1/article/details/7558979)



- .section  
- .data  
- string out = "%d,%d"  
- push b  
- push a  
- push $out  
- call printf  



你会看到，参数是最后的先压入栈中，最先的后压入栈中，参数控制的那个字符串常量是最后被压入的，所以这个常量总是能被找到的。
二、可变参数表函数的设计
      标准库提供的一些参数的数目可以有变化的函数。例如我们很熟悉的printf，它需要有一个格式串，还应根据需要为它提供任意多个“其他参数”。这种函数被称作“具有变长度参数表的函数”，或简称为“变参数函数”。我们写程序中有时也可能需要定义这种函数。要定义这类函数，就必须使用标准头文件<stdarg.h>，使用该文件提供的一套机制，并需要按照规定的定义方式工作。本节介绍这个头文件提供的有关功能，它们的意义和使用，并用例子说明这类函数的定义方法。
      C中变长实参头文件stdarg.h提供了一个数据类型va-list和三个宏（va-start、va-arg和va-end），用它们在被调用函数不知道参数个数和类型时对可变参数表进行测试，从而为访问可变参数提供了方便且有效的方法。va-list是一个char类型的指针，当被调用函数使用一个可变参数时，它声明一个类型为va-list的变量，该变量用来指向va-arg和va-end所需信息的位置。下面给出va_list在C中的源码：



**[cpp]**[view plain](http://blog.csdn.net/hackbuteer1/article/details/7558979)[copy](http://blog.csdn.net/hackbuteer1/article/details/7558979)



- typedef char *  va_list;  



     void va-start(va-list ap,lastfix)是一个宏，它使va-list类型变量ap指向被传递给函数的可变参数表中的第一个参数，在第一次调用va-arg和va-end之前，必须首先调用该宏。va-start的第二个参数lastfix是传递给被调用函数的最后一个固定参数的标识符。va-start使ap只指向lastfix之外的可变参数表中的第一个参数，很明显它先得到第一个参数内存地址，然后又加上这个参数的内存大小，就是下个参数的内存地址了。下面给出va_start在C中的源码：



**[cpp]**[view plain](http://blog.csdn.net/hackbuteer1/article/details/7558979)[copy](http://blog.csdn.net/hackbuteer1/article/details/7558979)



- #define _INTSIZEOF(n)   ( (sizeof(n) + sizeof(int) - 1) & ~(sizeof(int) - 1) )  
- #define va_start(ap,v)  ( ap = (va_list)&v + _INTSIZEOF(v) )   //得到可变参数中第一个参数的首地址  



      type va-arg(va-list ap,type)也是一个宏，其使用有双重目的，第一个是返回ap所指对象的值，第二个是修改参数指针ap使其增加以指向表中下一个参数。va-arg的第二个参数提供了修改参数指针所必需的信息。在第一次使用va-arg时，它返回可变参数表中的第一个参数，后续的调用都返回表中的下一个参数，下面给出va_arg在C中的源码：



**[cpp]**[view plain](http://blog.csdn.net/hackbuteer1/article/details/7558979)[copy](http://blog.csdn.net/hackbuteer1/article/details/7558979)



- #define va_arg(ap,type)    ( *(type *)((ap += _INTSIZEOF(type)) - _INTSIZEOF(type)) )    //将参数转换成需要的类型，并使ap指向下一个参数  



     在使用va-arg时，要注意第二个参数所用类型名应与传递到堆栈的参数的字节数对应，以保证能对不同类型的可变参数进行正确地寻址，比如实参依次为char型、char * 型、int型和float型时，在va-arg中它们的类型则应分别为int、char *、int和double.
     void va-end(va-list ap)也是一个宏，该宏用于被调用函数完成正常返回，功能就是把指针ap赋值为0，使它不指向内存的变量。下面给出va_end在C中的源码：



**[cpp]**[view plain](http://blog.csdn.net/hackbuteer1/article/details/7558979)[copy](http://blog.csdn.net/hackbuteer1/article/details/7558979)



- #define va_end(ap)      ( ap = (va_list)0 )  



     va-end必须在va-arg读完所有参数后再调用，否则会产生意想不到的后果。特别地，当可变参数表函数在程序执行过程中不止一次被调用时，在函数体每次处理完可变参数表之后必须调用一次va-end，以保证正确地恢复栈。
    一个变参数函数至少需要有一个普通参数，其普通参数可以具有任何类型。在函数定义中，这种函数的最后一个普通参数除了一般的用途之外，还有其他特殊用途。下面从一个例子开始说明有关的问题。
假设我们想定义一个函数sum，它可以用任意多个整数类型的表达式作为参数进行调用，希望sum能求出这些参数的和。这时我们应该将sum定义为一个只有一个普通参数，并具有变长度参数表的函数，这个函数的头部应该是（函数原型与此类似）：
int sum(int n, ...)
我们实际上要求在函数调用时，从第一个参数n得到被求和的表达式个数，从其余参数得到被求和的表达式。在参数表最后连续写三个圆点符号，说明这个函数具有可变数目的参数。凡参数表具有这种形式（最后写三个圆点），就表示定义的是一个变参数函数。注意，这样的三个圆点只能放在参数表最后，在所有普通参数之后。
下面假设函数sum里所用的va_list类型的变量的名字是vap。在能够用vap访问实际参数之前，必须首先用宏a_start对这个变量进行初始化。宏va_start的类型特征可以大致描述为：
va_start(va_list vap, 最后一个普通参数)
在函数sum里对vap初始化的语句应当写为：

va_start(vap, n); 相当于  char *vap= (char *)&n + sizeof(int);
此时vap正好指向n后面的可变参数表中的第一个参数。


在完成这个初始化之后，我们就可以通过另一个宏va_arg访问函数调用的各个实际参数了。宏va_arg的类型特征可以大致地描述为：
类型 va_arg(va_list vap, 类型名)
在调用宏va_arg时必须提供有关实参的实际类型，这一类型也将成为这个宏调用的返回值类型。对va_arg的调用不仅返回了一个实际参数的值（“当前”实际参数的值），同时还完成了某种更新操作，使对这个宏va_arg的下次调用能得到下一个实际参数。对于我们的例子，其中对宏va_arg的一次调用应当写为：
v = va_arg(vap, int);
这里假定v是一个有定义的int类型变量。
在变参数函数的定义里，函数退出之前必须做一次结束动作。这个动作通过对局部的va_list变量调用宏va_end完成。这个宏的类型特征大致是：
void va_end(va_list vap);
三、栈中参数分布以及宏使用后的指针变化说明如下:
![](http://my.csdn.net/uploads/201205/12/1336813017_9601.png)
下面是函数sum的完整定义，从中可以看到各有关部分的写法：



**[cpp]**[view plain](http://blog.csdn.net/hackbuteer1/article/details/7558979)[copy](http://blog.csdn.net/hackbuteer1/article/details/7558979)



- #include<iostream>  
- using namespace std;  
- #include<stdarg.h>  
- 
- int sum(int n,...)  
- {  
- int i , sum = 0;  
- va_list vap;  
-     va_start(vap , n);     //指向可变参数表中的第一个参数  
- for(i = 0 ; i < n ; ++i)  
-         sum += va_arg(vap , int);     //取出可变参数表中的参数，并修改参数指针vap使其增加以指向表中下一个参数  
-     va_end(vap);    //把指针vap赋值为0  
- return sum;  
- }  
- int main(void)  
- {  
- int m = sum(3 , 45 , 89 , 72);  
-     cout<<m<<endl;  
- return 0;  
- }  



这里首先定义了va_list变量vap，而后对它初始化。循环中通过va_arg取得顺序的各个实参的值，并将它们加入总和。最后调用va_end结束。
下面是调用这个函数的几个例子：
k = sum(3, 5+8, 7, 26*4);
m = sum(4, k, k*(k-15), 27, (k*k)/30);
函数sum中首先定义了可变参数表指针vap，而后通过va_start ( vap, n )取得了参数表首地址（赋值给了vap），其后的for循环则用来遍历可变参数表。这种遍历方式与我们在数据结构教材中经常看到的遍历方式是类似的。
　　函数sum看起来简洁明了，但是实际上printf的实现却远比这复杂。sum函数之所以看起来简单，是因为：
　　1、sum函数可变参数表的长度是已知的，通过num参数传入；
　　2、sum函数可变参数表中参数的类型是已知的，都为int型。
　　而printf函数则没有这么幸运。首先，printf函数可变参数的个数不能轻易的得到，而可变参数的类型也不是固定的，需由格式字符串进行识别（由%f、%d、%s等确定），因此则涉及到可变参数表的更复杂应用。
在这个函数中，需通过对传入的格式字符串（首地址为lpStr）进行识别来获知可变参数个数及各个可变参数的类型，具体实现体现在for循环中。譬如，在识别为%d后，做的是va_arg ( vap, int )，而获知为%l和%lf后则进行的是va_arg ( vap, long )、va_arg ( vap, double )。格式字符串识别完成后，可变参数也就处理完了。
在编写和使用具有可变数目参数的函数时，有几个问题值得注意。
第一：调用va_arg将更新被操作的va_list变量（如在上例的vap），使下次调用可以得到下一个参数。在执行这个操作时，va_arg并不知道实际有几个参数，也不知道参数的实际类型，它只是按给定的类型完成工作。因此，写程序的人应在变参数函数的定义里注意控制对实际参数的处理过程。上例通过参数n提供了参数个数的信息，就是为了控制循环。标准库函数printf根据格式串中的转换描述的数目确定实际参数的个数。如果这方面信息有误，函数执行中就可能出现严重问题。编译程序无法检查这里的数据一致性问题，需要写程序的人自己负责。在前面章节里，我们一直强调对printf等函数调用时，要注意格式串与其他参数个数之间一致性，其原因就在这里。
第二：编译系统无法对变参数函数中由三个圆点代表的那些实际参数做类型检查，因为函数的头部没有给出这些参数的类型信息。因此编译处理中既不会生成必要的类型转换，也不会提供类型错误信息。考虑标准库函数printf，在调用这个函数时，不但实际参数个数可能变化，各参数的类型也可能不同，因此不可能有统一方式来描述它们的类型。对于这种参数，C语言的处理方式就是不做类型检查，要求写程序的人保证函数调用的正确性。
假设我们写出下面的函数调用：
k = sum(6, 2.4, 4, 5.72, 6, 2);

编译程序不会发现这里参数类型不对，需要做类型转换，所有实参都将直接传给函数。函数里也会按照内部定义的方式把参数都当作整数使用。编译程序也不会发现参数个数与6不符。这一调用的结果完全由编译程序和执行环境决定，得到的结果肯定不会是正确的。


四、简单的练习




问题1：可变长参数的获取
　　有这样一个具有可变长参数的函数，其中有下列代码用来获取类型为float的实参：
　　va_arg (argp, float);
　　这样做可以吗？
　　答案与分析：
　　不可以。在可变长参数中，应用的是"加宽"原则。也就是float类型被扩展成double；char、 short类型被扩展成int。因此，如果你要去可变长参数列表中原来为float类型的参数，需要用va_arg(argp, double)。对char和short类型的则用va_arg(argp, int)。
　　问题2：定义可变长参数的一个限制
　　为什么我的编译器不允许我定义如下的函数，也就是可变长参数，但是没有任何的固定参数？



**[cpp]**[view plain](http://blog.csdn.net/hackbuteer1/article/details/7558979)[copy](http://blog.csdn.net/hackbuteer1/article/details/7558979)



- int f(...)  
- {  
-     ......  
-     ......  
-     ......  
- }  



答案与分析：
　　不可以。这是ANSI C 所要求的，你至少得定义一个固定参数。这个参数将被传递给va_start()，然后用va_arg()和va_end()来确定所有实际调用时可变长参数的类型和值。
      问题3：如何判别可变参数函数的参数类型？
函数形式如下：



**[cpp]**[view plain](http://blog.csdn.net/hackbuteer1/article/details/7558979)[copy](http://blog.csdn.net/hackbuteer1/article/details/7558979)



- void fun(char *str ,...)  
- {  
-     ......  
-     ......  
-     ......  
- }  



若传的参数个数大于1，如何判别第2个以后传参的参数类型？？？
答案与分析：
这个是没有办法判断的，例如printf( "%d%c%s ",   ....)是通过格式串中的%d、 %c、 %s来确定后面参数的类型，其实你也可以参考这种方法来判断不定参数的类型。
最后，奉献上自己写的一个printf函数



**[cpp]**[view plain](http://blog.csdn.net/hackbuteer1/article/details/7558979)[copy](http://blog.csdn.net/hackbuteer1/article/details/7558979)



- #include<stdio.h>  
- #include<stdarg.h>  
- 
- void myitoa(int n, char str[], int radix)  
- {  
- int i , j , remain;  
- char tmp;  
-     i = 0;  
- do  
-     {  
-         remain = n % radix;  
- if(remain > 9)  
-             str[i] = remain  - 10 + 'A';  
- else  
-             str[i] = remain + '0';  
-         i++;  
-     }while(n /= radix);  
-     str[i] = '\0';  
- 
- for(i-- , j = 0 ; j <= i ; j++ , i--)  
-     {  
-         tmp = str[j];  
-         str[j] = str[i];  
-         str[i] = tmp;  
-     }  
- 
- }  
- 
- void myprintf(const char *format, ...)  
- {  
- char c, ch, str[30];  
- va_list ap;  
- 
-     va_start(ap, format);  
- while((c = *format))  
-     {  
- switch(c)  
-         {  
- case '%':  
-             ch = *++format;  
- switch(ch)  
-             {  
- case 'd':  
-                 {  
- int n = va_arg(ap, int);  
-                     myitoa(n, str, 10);  
-                     fputs(str, stdout);  
- break;  
-                 }  
- case 'x':  
-                 {  
- int n = va_arg(ap, int);  
-                     myitoa(n, str, 16);  
-                     fputs(str, stdout);  
- break;  
-                 }  
- case 'f':  
-                 {  
- double f = va_arg(ap, double);  
- int n;  
-                     n = f;  
-                     myitoa(n, str, 10);  
-                     fputs(str, stdout);  
-                     putchar('.');  
-                     n = (f - n) * 1000000;  
-                     myitoa(n, str, 10);  
-                     fputs(str, stdout);  
- break;  
-                 }  
- case 'c':  
-                 {  
-                     putchar(va_arg(ap, int));  
- break;  
-                 }  
- case 's':  
-                 {  
- char *p = va_arg(ap, char *);  
-                     fputs(p, stdout);  
- break;  
-                 }  
- case '%':  
-                 {  
-                     putchar('%');  
- break;  
-                 }  
- default:  
-                 {  
-                     fputs("format invalid!", stdout);  
- break;  
-                 }  
-             }  
- break;  
- default:  
-             putchar(c);  
- break;  
-         }  
-         format++;  
-     }  
-     va_end(ap);  
- }  
- 
- int main(void)  
- {  
-     myprintf("%d, %x, %f, %c, %s, %%,%a\n", 10, 15, 3.14, 'B', "hello");  
- return 0;  
- }  
- 













