# C++程序员看过来，false < false的设定恰当吗？ - 文章 - 伯乐在线
本文由 [伯乐在线](http://blog.jobbole.com) - [sapphire](http://www.jobbole.com/members/sapphire000) 翻译，[黄利民](http://www.jobbole.com/members/huanglimin) 校稿。未经许可，禁止转载！
英文出处：[stackoverflow.com](http://stackoverflow.com/questions/26143976/is-the-operation-false-true-well-defined)。欢迎加入[翻译组](https://github.com/jobbole/translation-project)。
### 本文摘编自 StackOverflow 上的一个热门问答贴，duncan 提问：
C++中是否规范地定义了：
- 布尔参数中的“小于（less than）运算符”，并且如果有的话，
- 四个参数排列的结果是什么？
换句话说，以下操作的结果是有准确定义的吗？


```
false < false
false < true
true < false
true < true
```
在我的设定下（Centos 7, GCC 4.8.2)，下面的代码得出了我期望的结果（鉴于C用0表示false，用1表示true）


```
false < false = false
false < true = true
true < false = false
true < true = false
```
虽然我敢肯定，大多数（甚至所有？）编译器都会给出相同的输出，但这是由C++的标准规定的吗？或者这是一个模糊点，但符合规范的编译器可以算出true是小于false的？

C++
```
#include <iostream>
const char * s(bool a)
{
  return (a ? "true" : "false");
}
void test(bool a, bool b)
{
  std::cout << s(a) << " < " << s(b) << " = " << s(a < b) << std::endl;
}
int main(int argc, char* argv[])
{
  test(false, false);
  test(false, true);
  test(true, false);
  test(true, true);
  return 0;
}
```
### 最佳回答来自 Shafik Yaghmour
该操作是根据C+ +标准明确界定的。
**细节：**
我们可以通过查找[C++规范](http://www.open-std.org/jtc1/sc22/wg21/docs/papers/2012/n3337.pdf)5.9节关系运算符的说明：
操作数必须有算数类型、枚举类型、或指针类型，又或是键入的std :: nullptr_t。该操作符<（小于），>（大于），<=（小于或等于），和> =（大于或等于）所有产生的结果均为false或true。结果为布尔型。
而布尔变量是在3.9.1基本类型中的算数类型。
**类型为BOOL，CHAR，char16_t，char32_t，wchar_t的，和有符号、无符号整型统称为整型。**
并且整数和浮点类型统称为算术类型。
此外，true和false是2.14.6布尔文字量中的布尔文字量：


```
boolean-literal:
    false
    true
```
回到5.9节来更深层地理解关系操作符的机制，它说：
通常的算数转化是在算数或枚举类型的操作数下执行的。
通常的算数转化在第五节中提及，它说：
 否则整形提升（4.5）应该在两个操作数上执行，
并且4.5节提到：
bool类型的prvalue可转换为int类型的prvalue，这时false变为0，true变为1。
所以表达式：


```
false < false
false < true
true < false
true < true
```
通过这样的规则会变成：


```
0 < 0
0 < 1
1 < 0
1 < 1
```
