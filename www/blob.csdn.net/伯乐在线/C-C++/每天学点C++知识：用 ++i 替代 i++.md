# 每天学点C++知识：用 ++i 替代 i++ - 文章 - 伯乐在线
本文由 [伯乐在线](http://blog.jobbole.com) - [JingerJoe](http://www.jobbole.com/members/JingerJoe) 翻译，[黄小非](http://www.jobbole.com/members/huangxiaofei) 校稿。未经许可，禁止转载！
英文出处：[cpphints](http://cpphints.com/hints/31)。欢迎加入[翻译组](https://github.com/jobbole/translation-project)。
静态代码分析工具可简化编码过程，检测出错误并帮助修复。PVS-Studio 是一个用于 C/C++ 的静态代码分析工具。该团队检测了 200 多个 C/C++ 开源项目，包括了 Unreal Engine、Php、Haiku、Qt 和 Linux 内核等知名项目。于是他们每天分享一个错误案例，并给出相应建议。伯乐在线翻译组正在翻译这个系列，今天是第二篇。
这个 bug 是在 Unreal Engine 4 的源代码中发现的。
### 错误代码：

C++
```
void FSlateNotificationManager::GetWindows(
  TArray< TSharedRef<SWindow> >& OutWindows) const
{
  for( auto Iter(NotificationLists.CreateConstIterator());
       Iter; Iter++ )
  {
    TSharedPtr<SNotificationList> NotificationList = *Iter;
    ....
  }
}
```
### 解释：
如果不读标题的话，你可能很难发现这段代码里的问题。第一眼看上去这段代码完全正确，其实它并不完美。没错，我指的是后自增运算符 Iter++ 。 我们应该尽量使用前自增运算符而不是后自增运算符，即用 ++ Iter 代替 Iter++ 。 为什么要这么做，有什么有实际价值？下面我会详细解释。
### 正确代码：

C++
```
void FSlateNotificationManager::GetWindows(
  TArray< TSharedRef<SWindow> >& OutWindows) const
{
  for( auto Iter(NotificationLists.CreateConstIterator());
       Iter; ++Iter)
  {
    TSharedPtr<SNotificationList> NotificationList = *Iter;
    ....
  }
}
```
建议：
前缀和后缀形式之间的区别是众所周知的。我希望它们内部结构的区别（告诉了我们运算法则）大家也是清楚的。如果你有使用过运算符重载的话，肯定已经意识到了。没有用过的话，我在这儿简单地解释一下（用过运算符重载的可以跳过下面关于运算符重载的例子）。
前自增运算符改变了对象的状态并返回对象改变后的状态，不需要创建临时对象。下面是前自增运算符的例子：

C++
```
MyOwnClass& operator++()
{
  ++meOwnField;
  return (*this);
}
```
后自增运算符也改变了对象的状态但是返回的是对象改变前的状态，并且需要创建一个临时对象。下面是后自增运算符重载的例子：

C++
```
MyOwnClass operator++(int)
{
  MyOWnCLass tmp = *this;
  ++(*this);
  return tmp;
}
```
看到上面这段代码，你会发现有一个额外的操作，就是要创建一个临时对象，在实践中这点太重要了！
现在的编译器做代码优化的时候非常智能，如果没有用处，是不会随便创建临时对象的。这就是为什么在发布版中我们很难发现 i++ 和 ++ i 的区别。
但是在调试模式下进行程序调试的时候就是另一回事了，这时候你会看到性能上有很大差别。
举个例子，在[这篇](http://www.viva64.com/en/b/0093/)文章中，有一些例子可以估计调试版本中使用前自增和后自增运算符的代码运行时间，我们可以看到使用后缀形式所用时间几乎是前缀的四倍。
有人会说：”那又怎么样？反正发布版都是一样的。”，这种想法说对也对说不对也不对。通常我们会花更多的时间做单元测试和调试程序，所以大多数时间都在调试版本下工作，谁也不想浪费时间在那儿等吧？
关于“对于迭代器，我们是否应该用前自增运算符(++i)来代替后自增运算符(i++)？”这个问题，我想认真地回答: “是的，真应该这么做”。 你会发现在调试版本中速度大大提升。 如果迭代器很复杂的话，这么做的好处更是显而易见了。
参考资料（阅读推荐）：
- [Is it reasonable to use the prefix increment operator ++it instead of postfix operator it++ for iterators?](http://www.viva64.com/en/b/0093/)
- [Pre vs. post increment operator – benchmark](http://silviuardelean.ro/2011/04/20/pre-vs-post-increment-operator/)
这个错误是用静态代码分析工具 PVS-Studio 发现的，错误信息为：V803 性能下降。 如果iter是迭代器的话，使用前自增运算符会更高效，使用 ++iter 代替 iter++.
