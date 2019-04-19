# 每天学点C++知识：尽可能使用枚举类 - 文章 - 伯乐在线
本文由 [伯乐在线](http://blog.jobbole.com) - [JingerJoe](http://www.jobbole.com/members/JingerJoe) 翻译，[LongQi](http://www.jobbole.com/members/LongQI) 校稿。未经许可，禁止转载！
英文出处：[cpphints](http://cpphints.com/hints/14)。欢迎加入[翻译组](https://github.com/jobbole/translation-project)。
静态代码分析工具可简化编码过程，检测出错误并帮助修复。PVS-Studio 是一个用于 C/C++ 的静态代码分析工具。该团队检测了 200 多个 C/C++ 开源项目，包括了 Unreal Engine、Php、Haiku、Qt 和 Linux 内核等知名项目。于是他们每天分享一个错误案例，并给出相应建议。伯乐在线翻译组正在翻译这个系列，今天是第一篇。
下面这个 Bug 是在 Source SDK 的源代码中发现的。
### 错误代码：
这种错误的例子代码量都非常大，我尽可能地选取其中最小的一部分，但是很抱歉，代码看起来依旧很冗长。

C++
```
enum PhysGunPickup_t
{
  PICKED_UP_BY_CANNON,
  PUNTED_BY_CANNON,
  PICKED_UP_BY_PLAYER,
};
enum PhysGunDrop_t
{
  DROPPED_BY_PLAYER,
  THROWN_BY_PLAYER,
  DROPPED_BY_CANNON,
  LAUNCHED_BY_CANNON,
};
void CBreakableProp::OnPhysGunDrop(...., PhysGunDrop_t Reason)
{
  ....
  if( Reason == PUNTED_BY_CANNON )
  {
    PlayPuntSound(); 
  }
  ....
}
```
### 解释：
Reason 变量是属于枚举类型 PhysGunDrop_t，却用它和属于另一个枚举类型的常量作比较，这种比较显然是个逻辑错误。
但是这种 bug 模式很普遍，我甚至在像 Clang、TortoiseGit 和 Linux Kernel 这种项目中都有碰到过。
为什么会如此频繁？因为在标准C++中，枚举类型本来就不是类型安全的。到底什么该跟什么做比较，很容易让人混淆。
### 正确代码：
我不太确定这段代码的正确版本应该是什么样的，我猜想 PUNTED_BY_CANNON 应该用 DROPPED_BY_CANNON 或者 LAUNCHED_BY_CANNON 来替代。此处就用 LAUNCHED_BY_CANNON 来代替了。

C++
```
if( Reason == LAUNCHED_BY_CANNON )
{
  PlayPuntSound(); 
}
```
### 建议：
如果你是用C++写代码，还没碰到过这种bug，算你幸运；我强烈建议你从现在开始在代码中使用“枚举类”。
对于C++11中的一些新特性，我没有太多信心。就拿auto关键字来说吧，我相信如果频繁使用的话，会有很多坏处的。 我是这么看的：比起写代码，程序员会花更多的时间阅读代码，所以我们必须确保程序的可读性很强。 C语言中， 所有变量都必须在函数的一开始就声明，那么在函数的中间或者末尾编辑代码时，没那么容易推测出某个Alice变量到底是什么意思。这也是为什么变量的命名规范会这么多样化？ 例如，前缀命名法PfAlice就代表指向浮点数的指针。
C++中你可以随时随地声明变量，这是一种很好的编码风格。因而使用前后缀命名也不再那么受欢迎了。接着auto关键字出现了，直接导致程序员又开始使用各种各样很难理解的构造形式， 诸如 auto Alice= FOO(); 之类的。 Alice？谁是TM的Alice？（为保留原文本意，此处翻译稍有不文明）
很抱歉，又偏离我们的主题了。 我想告诉大家的是一些新的特性都有它的好坏两面性。 但是对于“枚举类”， 我坚信使用它有百利而无一害。
在使用枚举类的时候，必须明确指出指定的常量属于哪个枚举类型，以免在代码中出现错误。使用枚举类更新后的代码如下：

C++
```
enum class PhysGunDrop_t
{
  DROPPED_BY_PLAYER,
  THROWN_BY_PLAYER,
  DROPPED_BY_CANNON,
  LAUNCHED_BY_CANNON,
};
void CBreakableProp::OnPhysGunDrop(...., PhysGunDrop_t Reason)
{
  ....
  if( Reason == PhysGunDrop_t::LAUNCHED_BY_CANNON )
  {
    PlayPuntSound(); 
  }
  ....
}
```
说真的，修复旧代码的确会有一定困难，但我强烈推荐你们现在就开始在代码中使用枚举类，你的项目定会从中受益。
我觉得在这儿详细介绍枚举类没有多大意义，下面的一些链接可供大家参考学习，从中你会了解到关于C++11这个出色的新特性的所有细节。
- Wikipedia. C++11. [Strongly typed enumerations](https://en.wikipedia.org/wiki/C++11).
- Cppreference. [Enumeration declaration](http://en.cppreference.com/w/cpp/language/enum).
- StackOverflow. [Why is enum class preferred over plain enum?](http://stackoverflow.com/questions/18335861/why-is-enum-class-preferred-over-plain-enum)
这个错误是用静态代码分析工具 PVS-Studio 检测到的，错误信息为：V556 对不同枚举类型的值进行比较：Reason == PUNTED_BY_CANNON。
