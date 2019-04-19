# Modern C++(六)强类型枚举enum - xcyl的口袋 - CSDN博客
2016年11月03日 00:05:31[xcyl](https://me.csdn.net/cai6811376)阅读数：588
我们都知道在C++11前的枚举类型是默认为int类型，不是强类型的，所以毫无关系的两个枚举类型可以进行比较
```
enum EnumA
{
    A = 1,
    B = 2
};
enum EnumB
{
    C = 1,
    D = 2
};
if (EnumA::A == EnumB::C)
{
    //...
}
```
并且还隐藏着另外一个问题，由于枚举类型的元素是完全暴露出来的，其作用域内不可以有同名元素，比如
```
enum EnumC
{
    E = 1,
    F = 2
};
enum EnumD
{
    E = 1,
    G = 2
};
//重定义，无法通过编译，因为EnumC::E与EnumD::E被认定为同一变量
```
在C++11的标准中解决了这个问题，拥有了强类型枚举，从名字我们知道这里解决了弱类型的问题，我们看看是如何使用的
```
enum class EnumA
{
    A = 1,
    B = 2
};
enum class EnumB
{
    C = 1,
    D = 2
};
```
我们再对EnumA和EnumB中的元素进行比较
```
if (EnumA::A == EnumB::C)
{
    //...
}
//编译错误~
```
随之，元素同名问题也解决了
```
enum class EnumC
{
    E = 1,
    F = 2
};
enum class EnumD
{
    E = 1,
    G = 2
};
//通过编译~
```
同时，他们也将无法隐形转换为int类型 `EnumC::E = 5` 将无法通过编译，不过仍然兼容以前的用法。并且也无法直接使用E，G，因为他们分别属于各自枚举类型作用域中。
其他特性
enum并支持指定枚举类型的数据类型，默认类型为int
```
enum EnumA : int {};
enum class EnumB : long {};
```
由于可以指定enum的数据类型，所以enum支持前置声明啦~但必须指定类型
```
enum class Enum : char;
void Test(Enum e)
{
    //...
}
enum class Enum : char
{
    A = 'A',
    B = 'B'
};
```
## 获取更多信息
邮件：[support@easydarwin.org](mailto:support@easydarwin.org)
WEB：[www.easydarwin.org](http://www.easydarwin.org)
Copyright © EasyDarwin.org 2012-2016
![EasyDarwin](http://www.easydarwin.org/skin/easydarwin/images/wx_qrcode.jpg)
