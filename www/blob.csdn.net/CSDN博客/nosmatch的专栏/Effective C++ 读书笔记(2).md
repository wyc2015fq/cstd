# Effective C++ 读书笔记(2) - nosmatch的专栏 - CSDN博客
2013年07月07日 17:10:14[nosmatch](https://me.csdn.net/HDUTigerkin)阅读数：515
个人分类：[读书笔记](https://blog.csdn.net/HDUTigerkin/article/category/1432055)
条款二：尽量以const,enum,inline替换#define
> 
对于条款二，可以理解为能在编译器期间做的事情不要放到预编译器中处理，预编译器是对程序编译之前的一个预先的
处理，不会检查对代码最任何的检出，这样如果在编译期间发现问题，如果问题是我们自己直接造成的那可能会很快的定位，想反
的如果问题在预编译期间做了一些处理，那样在编译中出现的问题可能会显示预编译中的问问题，这样对问题定位不方便，例如：
在程序中定义宏#define PI 3.1415，如果编译过程中出现错误，错误显示中会直接显示3,1415，而不会对PI符号有所说明，如果宏是
自己定义的还好，如果是引用的其他头文件中的定义，那真的不是很好查的！对于条款二，主要有以下两点说明：
> 
1.尽量以const、enum定义来替换#define 定义。
> 
在头文件中以#define定义的常量在预编译期间直接对对应的符号进行替换，没有内存的申请，这也是#define可以放到头
文件中的原因，对于const常量定义中，它将常量直接放入到符号表中，不会申请固定的内存，除非对该常量有内存的引用，这也就
是为什么const定义的常量能像#defie一样放到头文件中，在const常量的使用中主要常量重叠的出现，相关知识自行查阅！
> 
在类常量成员的定义中，可以直接赋予初值，例如：
```cpp
class GamePlayer{
private:
           static const int NumTurns = 6;
           int score[NumTurns];
           .....
}
```
> 
上面中NumTurns是常量的声明而不是定义式，在后面的使用中如果只是使用该常量并没有用到该常量的地址，就可以不
用再对该常量做定义，否组需要如下的定义：
> 
```cpp
const int GamePlayer::NumTurns;
```
对应的在有些编译器中是不支持在声明常量的时候直接赋值的，例如：
```cpp
class GamePlay{
private:static const int NumTurns;
             .......
}
```
后面接着定义：
const  int  GamePlqy::NumTurns=6;
此时你就不能通过NumTurns来定义数组score了，我们可以借用enum的定义在其中修改为enum{NumTurns = 6};
2.对于函数宏的定义，在C语言中经常可以看到一些宏的定义，例如经典的：
```cpp
#define max(a,b) f((a) >(b) ? (a) : (b))
```
虽然在每个变量中都是添加了(),但是还是可能出现不确定的错误，自己查！相对应的替代方案就是采用inline函数来替
换：
> 
```cpp
template<typename T>
inline const T& max(const T& a,const T& b){
      return f(a>b?a:b);
}
```
其中用到了template模板函数，之所以用引用时为了自定义的类型，如果只是内置类型，可以不需要引用！

> 
> 
