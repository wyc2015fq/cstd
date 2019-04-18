# 精解C++的switch语句 - gauss的专栏 - CSDN博客
2013年01月04日 00:15:44[gauss](https://me.csdn.net/mathlmx)阅读数：399
入门书籍对switch语句的介绍相对较浅，我也因此而产生了很多想当然的误解。为解惑而写了以下一小篇精解switch语句，相信会对很多朋友有所帮助，同时顺便补充一些相关知识。
先抛出个题目，见下程序：
//原代码出自《C语言参考手册（原书第5版）》
//为了表达我的意图，特做了部分改动
switch(x)
{
default:
if(prime(x))
{
case 2: case 3: case 5: case 7:
process_prime(x);
}
else
{
case 4: case 6: case 8: case 9: case 10:
process_composite(x);
}
}
你能说出它如何执行吗？
switch语句的格式为：
switch(条件)语句
　　其中，条件的类型可以是整数类型，枚举类型，或者类类型（但该类需要有单一的转换到整数类型或（可以是字符类型，但不能是浮点类型、字符串、指针类型等），语句部分不一定非得是一条复合语句。因此，switch("123"[2]+(int)3.1);是条合法的switch语句，switch(j)case 5:i++;也是条合法的switch语句。如果switch的语句部分是一条非复合语句，则其内定义的变量作用域，效果上等同于该条语句加上了{}。如int
 i=3;switch(i)int i=4;，相当于int i=3;switch(i){int i=4;}，因此这并不会导致同一局部域下的重复定义错误。
如果条件为类类型，则该类内要有一个用户定义的类型转换操作符重载函数。如下边代码：
#include <iostream>
using namespace std;
class CTest
{
public:
operator int(){cout<<"int"<<endl;return data;}
operator char(){cout<<"int"<<endl;return static_cast<char>(data);}
CTest(int i):data(i){}
CTest(char c):data(c){}
private:
int data;
};
int main()
{
CTest x1(3);
CTest x2('5');
switch(x1)
{
case '5':break;
case 3:break;
}
return 0;
}
在VC++6下，编译器会报如下错误信息：
error C2450: switch expression of type 'class CTest' is illegal Ambiguous user-defined-conversion
因为类型转换函数有两个都是整数类型，编译器无法判断该去调用哪一个进行转换。
　　条件也可以是int i=3这样的初始化，其结果就是i的值。其作用域从声明处开始，直至switch语句的结束。可以理解成在switch外再加上了{}。int i=3,j=4这样的多个初始化，在C++标准中，不是条件。
　　语句部分，可以出现多个case标号以及一个default标号，它们的出现顺序随意。一个case标号或default标号，与属于其上层最近的switch语句，如：
switch(i)
{
case 1:
case 2:
if(a>5)
{
case 3:
case 4:
b=4;
switch(j)
{
case 5:
default:;
}
default:;
}
}
中，case 3:、case 4:，以及最后一个default:，属于外层switch，虽然它们在if语句内。注意在上面的代码中，}前的最后一个标号，后面至少要出现一条语句，因此如果没有内容的情况下，也至少要以一个空语句;作为结束。
case标号后为一个整数类型的常量表达式，因此int i=3;switch(i){case 3:;}合法，而int i=3;switch(3){case i:;}不合法，因为case i:的i不是个静态表达式。如果将int i=3;换成const int i=3;则后者在C++中就合法了，但在C中仍然不合法。原因是C和C++对const的处理不同，在C中，const限定的量是不能直接去修改的，但它本身并不是常量表达式；在C++中，const限定的量，如果其值能在编译时确定，则其可出现在必须使用常量表达式之处。
　　同一个switch的各个case标号的值不能够相互重复。要注意的是，case标号在实现中是有上限的：C89标准要求至少257个，这保证了ASCII被switch列举一遍。
　　虽然要求case标号是常量表达式，看起来似乎不是很零活方便（比如对比VB的Select Case），但是这样的设计可以保证更高的效率，而效率则是C和C++最为看重的因素。因为case标号的值是编译时可确定的整数类型，又因为其不可有重复，因此编译器可以进行优化。比如以下代码：
switch(b)
{
case 0: ...
case 1: ...
...
case 255: ...
}
不用被翻译成
if(b==0)
...
else if(b==1)
...
else if(b==255)
...
这样当b为255的情况，将最慢被执行到。而如果编译器对256个数字进行了优化，它可以根据比较的频率及重要性，产生这样的代码：
if(b<128)
{
if(b<64)
...
}
else
{
if(b<192)
...
}
这样的折半法，会减少每个比较经历的步数，在一个多层循环中的switch语句的执行效率会因此而得到提高。当然，前提是你要知道什么时候适合用它比较好，如命令行参数的解析。如果条件相对复杂，就使用if else而不是switch。
　　对switch比较常见的误解，就是把switch理解成if，把：
switch(b)
{
case 1: ... ;break;
case 2: ... ;break;
case 3: ... ;break;
default: ... ;break;
}
理解成它其实就是：
if(b==1)
{
...
}
else if(b==2)
{
...
}
else if(b==3)
{
...
}
else
{
...
}
没错，以上两段代码执行的效果的确等价，但是：
switch(b)
{
case 1: ...
case 2: ...
case 3: ...
default: ...
}
并不等价于：
if(b==1)
{
...
}
else if(b==2)
{
...
}
else if(b==3)
{
...
}
else
{
...
} //如果代码如此生成，则不仅不使用break的分支要多生成一条goto，而且对条件值的比较，也分散到了多个地方，低效！
　　如果每个标号后面，没有加break，则switch代码的执行会发生下落。上面的switch代码应该理解成：
//伪代码
{
if(b==1)goto case 1;
else if(b==2)goto case 2;
else if(b==3)goto case 3;
else goto default;
goto end_of_switch //如果没有default的话
{
case 1: ...
case 2: ...
case 3: ...
default: ...
end_of_switch:
}
}
而switch语句内最外层的break本身，作用就相当于伪代码的goto end_of_switch。这解释了各标号后的代码执行顺序，进而明白下落的具体原理。它也解释了标号顺序为何可以随意。
switch的执行方式：switch对条件求值，然后对所有的case标号（如果有的话）进行比较（可能被优化过比较顺序，或者被比较的条件值一直放在某个寄存器中，用CMP这样的汇编指令以提高效率），符合哪个就像goto一样跳到指定的标号。如果没成功，就会去goto default。如果程序没写default，则跳到switch语句下一条的位置。case标号和default标号，本身与普通标号类似，并不会在其出现处，设置else if进行条件判断，因此也不会影响下落。比较并转移到相应标号的代码，可以理解成完全是switch(条件)自己一句所生成的。
　　回到文章开始的那个题目，我们将其转换成对应的伪代码：
{
if(x==2)goto case 2; //实际汇编代码可能做优化
else if(x==3)goto case 3;
else if(x==5)goto case 5;
else if(x==7)goto case 7;
else if(x==4)goto case 4;
else if(x==6)goto case 6;
else if(x==8)goto case 8;
else if(x==9)goto case 9;
else if(x==10)goto case 10;
else goto default;
goto end_of_switch; //此句无用，有default时无产生的必要
{
default:
if(!prime(x))goto part_of_else;//为了方便理解，将if语句也转换了
case 2:
case 3:
case 5:
case 7:
process_prime(x);
goto end_of_if;
part_of_else:
case 4:
case 6:
case 8:
case 9:
case 10:
process_composite(x);
end_of_if:;
end_of_switch:;
}
}
　　该程序的if部分，用prime()函数来求x值是否为质数，如果是则调用process_prime()，否则去调用process_composite()。但是，其他部分的因素，在于大多数情况下，要判断的数<=10。如果对<=10的情况，也要完全靠调用prime()，则无疑会降低效率。为此，该switch会先行对2至10范围内的情况进行判断，并跳转相应位置进行处理。如果不在2至10范围的，则跳转到default，往下再调用prime进行判断。
当switch(x)int i=3;
　　最后要强调的问题是，C++中goto不能从前往后跳过变量定义，因此switch内出现的变量定义语句，最好放在复合语句{}中包起来。
请看以下代码：
switch(x)
{
case 0: f0(); break;
case 1:CTest i(3); f1(); break;
default: fdef();
}
其伪代码为：
{
if(x==0)goto case 0;
else if(x==1)goto case 1;
else goto default;
{
case 0:
f0();goto end_of_switch;
case 1:
CTest i(3);
f1();
goto end_of_switch;
default:
fdef();
end_of_switch:
CTest::~CTest();
}
}
当x为0或1时，不会有问题，问题是当转到default时，相当于从外层直接跳过了i的构造，然后在default自己的代码执行完毕后，还要执行i的析构。改成：
switch(x)
{
case 0: f0(); break;
case 1:{CTest i(3); f1(); break;}
default: fdef();
}
则i的作用域被限制在{}中，不会再出现未构造就析构的问题。
