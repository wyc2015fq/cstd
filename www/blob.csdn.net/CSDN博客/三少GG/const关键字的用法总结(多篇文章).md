# const关键字的用法总结(多篇文章) - 三少GG - CSDN博客
2012年02月29日 11:38:32[三少GG](https://me.csdn.net/scut1135)阅读数：822
**1.    const 用于定义常量，定义时必须进行初始化，一经定义不能修改。**
**2.    const用作函数参数：**
**1） 指针参数：**
const 在 * 的左边，表示该指针的数据不能改变，但是地址可以改变。
例如：void fun(constint *p)
{
*p = 10;     //非法，不能改变指针p的数据
int num(20);
p = #   //合法，可以改变p指向的地址，同时改变了p的数据，此时*p = 20
}
const 在 * 的右边，表示该指针的地址不能改变，但是数据可以改变。
例如：void fun(int *const p)
{
*p = 10;     //合法，改变了指针p的数据
int num(20);
p = #   //非法，不能改变p指向的地址
}
**2） 引用参数：**
const 作为引用参数时，只可以将其值赋给其他变量，不能修改其数值。
例如：void fun(constint &p)
{
int i;
i = p;     //合法，将 p的数值赋给了变量i
p = 10;   //非法，不能改变p的数值
}
**3.    const 用作类的成员函数**
1）const 修饰类的成员函数时，必须写在函数的后面。
例如：class Test;
voidTest::fun(void) const
2）const成员函数可以调用类中const成员函数，不能调用非const成员函数。
例如：voidTest::gun(void);
voidTest::hun(void) const
void Test::fun(void)const
{
gun( );     //非法，不能调用非const成员函数
hun( );     //合法，可以调用const成员函数
}
3）const 对象可以调用类中const成员函数，但是不能调用非const成员函数。
例如： voidTest::gun(void);
voidTest::fun(void) const;
const Test A;
A.gun( )     //非法，不能调用非const成员函数
A.fun( );     //合法，可以调用const成员函数
4）类中非const 对象和非const 成员函数都可以调用const成员函数。
例如： voidTest::fun(void) const;
voidTest::gun(void)
{
Fun( );    //合法，可以调用const成员函数
}
Test A;
A.fun( );     //合法，可以调用const成员函数
5）总结：类的对象和成员函数之间调用关系作如下表格。
|对象调用成员函数|成员函数调用成员函数| | | | |
|----|----|----|----|----|----|
|对象|成员函数|合法/非法|调用者|被调用者|合法/非法|
|const|const|合法|const|const|合法|
|const|non-const|非法|const|non-const|非法|
|non-const|const|合法|non-const|const|合法|
|non-const|non-const|合法|non-const|non-const|合法|
**4.    const 用作函数的返回值**
const 用作函数的返回值时，必须写在函数的前面。表示函数的返回值不能被修改。
例如：const intfun(int i);
该函数返回了一个整形值，这个值不能被修改。
*********************************************************************************************************
const给人的第一印象就是定义常量。
(1)const用于定义常量。
例如：const intN = 100;const int M = 200;
这样程序中只要用到 N、M 就分别代表为整型100、200，N、M 为一常量，在程序中不可改变。
但有人说他编程时从来不用const定义常量。我相信。但他是不懂得真正的编程艺术，用const定义常量不仅能方便我们编程而且能提高程序的清晰性。你是愿意看到程序中100、200 满天飞，还是愿意只看到简单清晰的N、M。相信有没有好处你慢慢体会。
还有人说他不用const定义常量，他用#define宏定义常量。可以。但不知道你有没有发现有时#define宏并没有如你所愿在定义常量。下面我们比较比较const和#define。
1。const定义常量是有数据类型的，而#define宏定义常量却没有。
这样const定义的常量编译器可以对其进行数据静态类型安全检查，而#define宏定义的常量却只是进行简单的字符替换，没有类型安全检查，且有时还会产生边际效应(不如你愿处)。所谓边际效应举例如下：
#define N 100
#define M 200 + N
当程序中使用 M*N 时，原本想要 100 * （200+ N ）的却变成了 100 * 200 + N。
2。有些调试程序可对const进行调试，但不对#define进行调试。
3。当定义局部变量时，const作用域仅限于定义局部变量的函数体内。但用#define时其作用域不仅限于定义局部变量的函数体内，而是从定义点到整个程序的结束点。但也可以用#undef取消其定义从而限定其作用域范А?
光用const定义常量，并不能起到其强大的作用。const还可修饰函数形式参数、返回值和类的成员函数等。从而提高函数的健壮性。因为const修饰的东西能受到c/c++的静态类型安全检查机制的强制保护，防止意外的修改。
(2)const修饰函数形式参数
形式参数有输入形式参数和输出形式参数。参数用于输出时不能加const修饰，那样会使函数失去输出功能。因为const修饰的东西是不能改变的。
const只能用于修饰输入参数。
谈const只能用于修饰输入参数之前先谈谈C++函数的三种传递方式。
C++函数的三种传递方式为：值传递、指针传递和引用传递。简单举例说明之，详细说明请参考别的资料。
值传递：
void fun(int x){
x += 5; //修改的只是y在栈中copy x
}
void main(void){
int y = 0;
fun(y);
cout<<"y = "< } 
指针传递：
void fun(int *x){
*x += 5;//修改的是指针x指向的内存单元值
}
void main(void){
int y = 0;
fun(&y);
cout<<<<"y = "<}
引用传递：
void fun(int &x){
x += 5;//修改的是x引用的对象值 &x = y; 
}
void main(void){
int y = 0;
fun(y);
cout<<<<"y = "<}
看了传递方式后我们继续来谈"const只能用于修饰输入参数"的情况。
当输入参数用"值传递"方式时，我们不需要加const修饰，因为用值传递时，函数将自动用实际参数的拷贝初始化形式参数，当在函数体内改变形式参数时，改变的也只是栈上的拷贝而不是实际参数。
但要注意的是，当输入参数为ADT/UDT（用户自定义类型和抽象数据类型）时，应该将"值传递"改为"const &传递"，目的可以提高效率。
例如：
void fun(A a);//效率底。函数体内产生A类型的临时对象用于复制参数 a，但是临时对象的
//构造、复制、析构过程都将消耗时间。
void fun(A const &a);//提高效率。用"引用传递"不需要产生临时对象，省了临时对象的 
//构造、复制、析构过程消耗的时间。但光用引用有可能改变a,所以加const
当输入参数用"指针传递"方式时，加const修饰可防止意外修改指针指向的内存单元，起到保护作用。
例如：
void funstrcopy(char *strdest,const char *strsrc)//任何改变strsrc指向的内存单元，
//编译器都将报错
const不仅可以保护指针的内存单元，也可以保护指针本身，防止其地址改变。
例如：
void funstrcopy(char *strdest,const char *const strsrc)
(3)const修饰函数的返回值
如给"指针传递"的函数返回值加const，则返回值不能被直接修改，**且该返回值只能被赋值给加const修饰的同类型指针。**例如：
const char *GetChar(void){};
赋值 char *ch= GetChar();//错误constchar *ch = GetChar();//正确 
(4)const修饰类的成员函数(函数定义体)
任何不会修改数据成员的函数都应用const修饰，这样当不小心修改了数据成员或调用了非const成员函数时，编译器都会报错。
const修饰类的成员函数形式为：intGetCount(void)const; 
**********************************************************************************************
关键字：Const，Const函数，Const变量，函数后面的Const
看到const 关键字，C++程序员首先想到的可能是const 常量。这可不是良好的条件反射。如果只知道用const 定义常量，那么相当于把火药仅用于制作鞭炮。const 更大的魅力是它可以修饰函数的参数、返回值，甚至函数的定义体。
const 是constant 的缩写，“恒定不变”的意思。被const 修饰的东西都受到强制保护，可以预防意外的变动，能提高程序的健壮性。所以很多C++程序设计书籍建议：“Use const whenever you need”。
1.用const 修饰函数的参数
如果参数作输出用，不论它是什么数据类型，也不论它采用“指针传递”还是“引用传递”，都不能加const 修饰，否则该参数将失去输出功能。const 只能修饰输入参数：
如果输入参数采用“指针传递”，那么加const 修饰可以防止意外地改动该指针，起到保护作用。
例如StringCopy 函数：
void StringCopy(char *strDestination,const char *strSource);
其中strSource 是输入参数，strDestination 是输出参数。给strSource 加上const修饰后，如果函数体内的语句试图改动strSource 的内容，编译器将指出错误。
如果输入参数采用“值传递”，由于函数将自动产生临时变量用于复制该参数，该输入参数本来就无需保护，所以不要加const 修饰。
例如不要将函数void Func1(int x) 写成void Func1(const int x)。同理不要将函数void Func2(A a) 写成void Func2(const A a)。其中A 为用户自定义的数据类型。
对于非内部数据类型的参数而言，象void Func(A a) 这样声明的函数注定效率比较底。因为函数体内将产生A 类型的临时对象用于复制参数a，而临时对象的构造、复制、析构过程都将消耗时间。
为了提高效率，可以将函数声明改为void Func(A &a)，因为“引用传递”仅借用一下参数的别名而已，不需要产生临时对象。但是函数void Func(A &a) 存在一个缺点：
“引用传递”有可能改变参数a，这是我们不期望的。解决这个问题很容易，加const修饰即可，因此函数最终成为void Func(const A &a)。
以此类推，是否应将void Func(int x) 改写为void Func(const int &x)，以便提高效率？完全没有必要，因为内部数据类型的参数不存在构造、析构的过程，而复制也非常快，“值传递”和“引用传递”的效率几乎相当。
问题是如此的缠绵，我只好将“const &”修饰输入参数的用法总结一下。
对于非内部数据类型的输入参数，应该将“值传递”的方式改为“const 引用传递”，目的是提高效率。例如将void Func(A a) 改为void Func(const A &a)。
对于内部数据类型的输入参数，不要将“值传递”的方式改为“const 引用传递”。否则既达不到提高效率的目的，又降低了函数的可理解性。例如void Func(int x) 不应该改为void Func(const int &x)。
**********************************************************************************
const 函数与const 参数
一个成员函数为const的确切含义是什么？有两种主要的看法：数据意义上的const（bitwise constness）和概念意义上的const（conceptual constness）。
bitwise constness的坚持者认为，当且仅当成员函数不修改对象的任何数据成员（静态数据成员除外）时，即不修改对象中任何一个比特(bit)时，这个成员函数才是const的。
实际上，bitwise constness正是c++对const问题的定义，const成员函数不被允许修改它所在对象的任何一个数据成员。
利用c++标准组织针对这类情况专门提供的有关const问题的另一个可选方案。此方案使用了关键字mutable，当对非静态数据成员运用mutable时，这些成员的“bitwise constness”限制就被解除：
class string {
public:
...   // same as above
private:
char *data;
mutable size_t datalength;      // 这些数据成员现在
                                  // 为mutable；他们可以在
mutable bool lengthisvalid;     // 任何地方被修改，即使
                                  // 在const成员函数里
};
size_t string::length() const
{
if (!lengthisvalid) {
   datalength = strlen(data);    // 现在合法
   lengthisvalid = true;         // 同样合法
}
return datalength;
}
mutable在处理“bitwise-constness限制”问题时是一个很好的方案，但它被加入到c++标准中的时间不长，所以有的编译器可能还不支持它。如果是这样，就不得不倒退到c++黑暗的旧时代去，在那儿，生活很简陋，const有时可能会被抛弃。
还有一种情况下，通过类型转换消除const会既有用又安全。这就是：将一个const对象传递到一个取非const参数的函数中，同时你又知道参数不会在函数内部被修改的情况时。第二个条件很重要，因为对一个只会被读的对象（不会被写）消除const永远是安全的，即使那个对象最初曾被定义为const。
例如，已经知道有些库不正确地声明了象下面这样的strlen函数：
size_t strlen(char *s);
strlen当然不会去修改s所指的数据——至少我一辈子没看见过。但因为有了这个声明，对一个const char *类型的指针调用这个函数时就会不合法。为解决这个问题，可以在给strlen传参数时安全地把这个指针的const强制转换掉：
const char *klingongreeting ="nuqneh"; // "nuqneh"即"hello"
                                        // 
size_t length =
strlen(const_cast<char*>(klingongreeting));
但不要滥用这个方法。只有在被调用的函数（比如本例中的strlen）不会修改它的参数所指的数据时，才能保证它可以正常工作。
