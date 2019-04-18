# 读《C++ Primer（第三版）》的一些疑问（不断更新） - 江南烟雨 - CSDN博客
2011年07月10日 20:57:59[江南烟雨](https://me.csdn.net/xiajun07061225)阅读数：2348
【1】（Page94）代码：
```cpp
int staff_size=27;
//错误：非const变量
double salaries[staff_size];
```
但是编译器并不报错？这是为什么？
解答：
用Dev C++编译能通过，但是用VS2008便报错，看来编译器很关键啊~~~~
【2】（Page104）
```cpp
typedef char *cstring;
extern const cstring cstr;
```
请问，cstr的类型是什么？
解答：cstr是指向字符的const指针。cstring是指针类型，指向char。const修饰cstring，就是修饰指针。
【3】（Page133）代码：
```cpp
//一个string的大小与它所指的字符串的长度无关；
string st1(“foobar”);
string st2(“a mighty oak”);
```
sizeof(st1)、sizeof(st2)都是12（具体数值和编译器有关）。
解答：
sizeof(str)返回的是一个string对象本身所占的内存的大小，不管你的string里面存储了多大的文本，这个值都是固定的。要取得string对象的长度，可以用str.length()。
【4】（Page135）下列语句哪些是非法的或错误的？
```cpp
（a）vector<string> svec(10);
（b）vector<string> *pvec1=new vector<string>(10);
（c）vector<string> *pvec1=new vector<string>(10);
（d）vector <string> *pv1=&svec;
（i）delete pv1;
```
b和c有什么区别？各是对的还是错误的？
还有i到底是正确的还是错误的？
解答：
vector<string> *pvec1=new vector<string>(10)是分配一个vector容器，里面有10个string元素，而vector<string> *pvec1=new vector<string>(10)数组，数组含有vector <string>10个元素。
i中，是合法的，编译器无法侦测出任何错误。但其实是错误的，因为我们不能对一个“不是通过new表达式分配的指针”施以delete操作。
 【5】（Page142）下列bitset对象的声明哪些是错误的？
（a）bitset<64> bitvec(32);
（d）bitset<32>bv;bitset<16> bv16(bv);
 解答：
（a）正确。64是size_t的一个合法值，bitvec将使用64位中的32个。
？
【6】（Page224）下列iterator的用法哪些是错误的？
```cpp
const vector<int>  ivec;
vector<string> svec;
list<int> ilist;
```
（c）vector<string>::iterator it=&svec[0];
解答：
错误。iterator必须借由begin()来初始化，而不是借由vector的某个取值操作符来初始化。
