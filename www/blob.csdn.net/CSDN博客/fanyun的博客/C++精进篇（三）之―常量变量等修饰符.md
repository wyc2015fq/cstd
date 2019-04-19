# C++精进篇（三）之―常量变量等修饰符 - fanyun的博客 - CSDN博客
2016年03月25日 22:18:17[老樊Lu码](https://me.csdn.net/fanyun_01)阅读数：1162
所属专栏：[C/C++/C#开发实战365](https://blog.csdn.net/column/details/c-plus-plus-01.html)
**一、Const常量**
**1、Const定义：**
常类型是指使用类型修饰符const修饰的类型，常类型的变量或对象的值是不能被更新的。const 推出的初始目的，正是为了取代预编译指令，消除它的缺点，同时继承它的优点。
**2、Const作用：**
（1）可以定义const常量，具有不可变性。例如：               const int Max=100; int Array[Max]; <span style="color:#333333;">（</span><span style="color:#333333;">2</span><span style="color:#333333;">）便于进行类型检查，使编译器对处理内容有更多了解，消除了一些隐患。</span><span style="color:#333333;">例如：</span><span style="color:#333333;"> void f(const int i) { .........} </span><span style="color:#333333;">编译器就会知道</span><span style="color:#333333;">i</span><span style="color:#333333;">是一个常量，不允许修改。</span><span style="color:#333333;">（</span><span style="color:#333333;">3</span><span style="color:#333333;">）可以避免意义模糊的数字出现，同样可以很方便地进行参数的调整和修改。</span><span style="color:#333333;">同宏定义一样，可以做到不变则已，一变都变！如（</span><span style="color:#333333;">1</span><span style="color:#333333;">）中，如果想修改</span><span style="color:#333333;">Max</span><span style="color:#333333;">的内容，只需要：</span><span style="color:#333333;">const int Max=you want;</span><span style="color:#333333;">即可！</span><span style="color:#333333;"> </span><span style="color:#333333;">（</span><span style="color:#333333;">4</span><span style="color:#333333;">）可以保护被修饰的东西，防止意外的修改，增强程序的健壮性。</span><span style="color:#333333;">还是上面的例子，如果在函数体内修改了</span><span style="color:#333333;">i</span><span style="color:#333333;">，编译器就会报错；</span><span style="color:#333333;">例如：</span>void f(const int i) { i=10;//error! } <span style="color:#333333;">（</span><span style="color:#333333;">5</span><span style="color:#333333;">）</span><span style="color:#333333;">为函数重载提供了一个参考。例如如下代码：</span>
class A { ......
void f(int i) {......} //一个函数
void f(int i) const {......} //上一个函数的重载 ......
};
（6）可以节省空间，避免不必要的内存分配。例如如下代码：
#define PI 3.14159 //常量宏   
const doulbe Pi=3.14159; //此时并未将Pi放入ROM中 ......   
double i=Pi; //此时为Pi分配内存，以后不再分配！   
double I=PI; //编译期间进行宏替换，分配内存   
double j=Pi; //没有内存分配   
double J=PI; //再进行宏替换，又一次分配内存！   
const定义常量从汇编的角度来看，只是给出了对应的内存地址，而不是象#define一样给出的是立即数，所以，const定义的常量在程序运行过程中只有一份拷贝，而#define定义的常量在内存中有若干个拷贝。<span style="color:#333333;">（</span><span style="color:#333333;">7</span><span style="color:#333333;">）</span><span style="color:#333333;">提高了效率。</span><span style="color:#333333;">编译器通常不为普通</span><span style="color:#333333;">const</span><span style="color:#333333;">常量分配存储空间，而是将它们保存在符号表中，这使得它成为一个编译期间的常量，没有了存储与读内存的操作，使得它的效率也很高。</span>
**3、Const常用的几种情形：**
（1）修饰一般常量一般常量是指简单类型的常量。这种常量在定义时，修饰符const可以用在类型说明符前，也可以用在类型说明符后。<span style="color:#333333;">               </span><span style="color:#333333;">例如：</span><span style="color:#333333;"> int const x=2; </span><span style="color:#333333;">或</span><span style="color:#333333;"> const int x=2; </span><span style="color:#333333;">（</span><span style="color:#333333;">2</span><span style="color:#333333;">）修饰常数组</span><span style="color:#333333;">定义或说明一个常数组可采用如下格式：</span><span style="color:#333333;">               int const a[5]={1, 2, 3, 4, 5}; </span><span style="color:#333333;">               const int a[5]={1, 2, 3, 4, 5}; </span><span style="color:#333333;">（</span><span style="color:#333333;">3</span><span style="color:#333333;">）修饰常对象</span><span style="color:#333333;">常对象是指对象常量，定义格式如下：</span><span style="color:#333333;">   </span><span style="color:#333333;">               class A; </span><span style="color:#333333;">               const A a; </span>               A const a; <span style="color:#333333;">定义常对象时，同样要进行初始化，并且该对象不能再被更新，修饰符</span><span style="color:#333333;">const</span><span style="color:#333333;">可以放在类名后面，也可以放在类名前面。</span><span style="color:#333333;"> </span><span style="color:#333333;">（</span><span style="color:#333333;">4</span><span style="color:#333333;">）修饰常指针</span>               const int *A; //const修饰指向的对象，A可变，A指向的对象不可变               int const *A; //const修饰指向的对象，A可变，A指向的对象不可变               int *const A; //const修饰指针A， A不可变，A指向的对象可变               const int *const A;//指针A和A指向的对象都不可变<span style="color:#333333;">（</span><span style="color:#333333;">5</span><span style="color:#333333;">）修饰常引用</span><span style="color:#333333;">使用</span><span style="color:#333333;">const</span><span style="color:#333333;">修饰符也可以说明引用，被说明的引用为常引用，该引用所引用的对象不能被更新。</span><span style="color:#333333;">               </span><span style="color:#333333;">其定义格式如下：</span>               const double & v; <span style="color:#333333;">（</span><span style="color:#333333;">6</span><span style="color:#333333;">）修饰函数的常参数</span><span style="color:#333333;"> const</span><span style="color:#333333;">修饰符也可以修饰函数的传递参数，格式如下：</span>               void Fun(const int Var); 告诉编译器Var在函数体中的无法改变，从而防止了使用者的一些无意的或错误的修改。<span style="color:#333333;">（</span><span style="color:#333333;">7</span><span style="color:#333333;">）修饰函数的返回值：</span><span style="color:#333333;"> const</span><span style="color:#333333;">修饰符也可以修饰函数的返回值，是返回值不可被改变，格式如下：</span>               const int Fun1(); const MyClass Fun2(); <span style="color:#333333;">（</span><span style="color:#333333;">8</span><span style="color:#333333;">）修饰类的成员函数：</span><span style="color:#333333;"> const</span><span style="color:#333333;">修饰符也可以修饰类的成员函数，格式如下：</span>
class ClassName {   
public:   
int Fun() const; .....   
};   
这样，在调用函数Fun时就不能修改类里面的数据<span style="color:#333333;">（</span><span style="color:#333333;">9</span><span style="color:#333333;">）在另一连接文件中引用</span><span style="color:#333333;">const</span><span style="color:#333333;">常量</span>               extern const int i;//正确的引用               extern const int j=10;//错误！常量不可以被再次赋值，另外，还要注意，常量必须初始化！例如： const int i=5; （10）const修饰this指针this指针是个什么类型的？这要看具体情况：如果在非const成员函数中，this指针只是一个类类型的；如果在const成员函数中，this指针是一个const类类型的；如果在volatile成员函数中,this指针就是一个volatile类类型的。
**二、auto**
**1、使用迭代器：**
vector<vector<int> > v;
vector<vector<int> >::iterator it = v.begin(); 
**2、函数指针也同样, 类型声明很特别：**
int add(int x,int y){
    return x+y;
}
int main(){
    int (*func)(int,int) =add;
    cout<<func(1,2)<<endl;
}
**3、既然把v.begin()赋给it, 类型已经在编译期确定了，编译器知道正确的类型是什么，再加一个类型声明实在很繁琐。C++11有了auto。我们可以这样写：**
vector<vector<int>> v; // C++11 可以不用在'>>'之间加空格了！
auto it = v.begin();
auto func = add;
编译器会根据值的类型，推导出autob变量。类型的推导是在编译期就完成的，仍是静态类型，和脚本语言不同。实际上是一个语法糖。但由于C++对模板的大量使用，一个变量的类型有时过于复杂难以写出，这样的语法糖是必要的。
**三、decltype**
**1、有时候，我们需要让编译器根据表达式来确定数据类型，但又不初始化这个变量。**这个时候就可以使用decltype关键字：
decltype(f())sum = x;**2、函数f()并不会被调用，但是它的返回值类型会在这里被使用。**
decltype不同于auto，它会完整的保留const。
int i = 42, *p = &i, &r = i;
decltype(r+ 0) b;
decltype(*p)c;
decltype(r)是一个引用类型，而decltype(r + 0)是一个int类型。
decltype(*p)表示返回的将是一个引用，即int&。**3****、还需要注意括号的使用：**
decltype((i))d; // 错误的表达，d是int&，必须被初始化。
decltype(i)e;   // 正确的表达，e是int。注意：不加括号，返回的是变量类型；加了括号，返回的是表达式，赋值表达式会产生一个“=”左边的类型的引用。
**四、变量别名****1****、变量别名可以使用typedef：**
typedefdouble wages;也可以使用using:
using SI= Sales_item;**2、变量别名和const结合，会产生有趣的结果：**
typedefchar *pstring;
constpstring cstr = 0;
cstr是指向char类型的常量指针，因为const要和base type结合。**3****、这里，对象可以被更改，但cstr不可被更改。**
typedef不是define，不能直接替代变量的内容。
