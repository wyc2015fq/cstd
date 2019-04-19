# 每天学点C++知识：为什么错误代码能正常工作 - 文章 - 伯乐在线
本文由 [伯乐在线](http://blog.jobbole.com) - [柒柒](http://www.jobbole.com/members/chen1991) 翻译，[至秦](http://www.jobbole.com/members/applevip) 校稿。未经许可，禁止转载！
英文出处：[Andrey Karpov](http://cpphints.com/hints/35)。欢迎加入[翻译组](https://github.com/jobbole/translation-project)。
有个国外团队检测了 200 多个 C/C++ 开源项目，包括了 Php、Qt 和 Linux 内核等知名项目。于是他们每天分享一个错误案例，并给出相应建议。本篇案例来自 Miranda NG 源码。
### 错误代码：

C++
```
#define MF_BYCOMMAND 0x00000000L
void CMenuBar::updateState(const HMENU hMenu) const
{
  ....
  ::CheckMenuItem(hMenu, ID_VIEW_SHOWAVATAR,
    MF_BYCOMMAND | dat->bShowAvatar ? MF_CHECKED : MF_UNCHECKED);
  ....
}
```
### 问题解释：
我想提出一个发人深思的主题来讨论：有时候，我们会看到完全不正确的代码却能工作得很好！
经验丰富的程序猿不会对这个问题感到惊讶（这又是另一个故事），但对那些最近刚开始学习 C/C++ 的人来说，这个问题也许有点让人困惑。那么，今天我们就来看看这样一个示例。
在上面显示的代码中，必须在调用 CheckMenuItem() 时设置标志位；然后，先看 bShowAvatar 是否为 true，如果成立MF_BYCOMMAND 就和 MF_CHECKED 按位或运算，反之则和 MF_UNCHECKED 做按位或运算。就这么简单！

C++
```
MF_BYCOMMAND | dat->bShowAvatar ? MF_CHECKED : MF_UNCHECKED
```
在上面的代码中，这位程序猿很自然地选择[三元](http://cpphints.com/hints/1)运算符来表示这个过程（这个运算符是 if-then-else 的简便版）：

C++
```
MF_BYCOMMAND | dat->bShowAvatar ? MF_CHECKED : MF_UNCHECKED
```
问题是，按位或运算符 | 比运算符 ?: 的优先级更高（详细看 [C/C++ 运算优先级](http://www.viva64.com/en/t/0064/)）。很明显这里有两个错误。
第一个错误是条件变了，不再是“dat->bShowAvatar”，而变成了“MF_BYCOMMAND | dat->bShowAvatar”。
第二个错误是只选择了一个标志位 —— MF_CHECKED 或 MF_UNCHECKED，标志位 MF_BYCOMMAND 不见了。
尽管存在这些错误，这条代码仍然能正常工作！这纯粹是因为运气。这位程序猿很走运，因为 [MF_BYCOMMAND](https://msdn.microsoft.com/ru-ru/library/windows/desktop/ms647619%28v=vs.85%29.aspx) 等于 0x0000000L。
当标志位 MF_BYCOMMAND 等于 0，也不会对代码有任何影响。可能某些经验丰富的程序猿已然明了，但万一这里有新手，我还是详细解释一下其中的缘由。
先来看看加了括号的正确表达式：

C++
```
MF_BYCOMMAND | (dat->bShowAvatar ? MF_CHECKED : MF_UNCHECKED)
```
用数值替换宏：

C++
```
0x00000000L | (dat->bShowAvatar ? 0x00000008L : 0x00000000L)
```
如果运算符“|”两边的操作数之一为0，那我们可以将表达式简化为：

C++
```
dat->bShowAvatar ? 0x00000008L : 0x00000000L
```
现在来进一步看看错误代码的变体：

C++
```
MF_BYCOMMAND | dat->bShowAvatar ? MF_CHECKED : MF_UNCHECKED
```
用数值代替宏：

C++
```
0x00000000L | dat->bShowAvatar ? 0x00000008L : 0x00000000L
```
在子表达式“0x0000000L | dat->bShowAvatar”中，运算符 | 的操作数之一为0，将表达式简化为：

C++
```
dat->bShowAvatar ? 0x00000008L : 0x00000000L
```
结果就是，我们得到了相同的表达式，这就是错误代码能正确工作的原因。又发生了一个编程奇迹。
### 正确代码这么写：
有很多种方法可以修正这段代码，其中一个方法就是加上圆括号，另外一个就是增加一个中间变量。用大家熟悉的操作符“if”在这里也会有帮助：

C++
```
if (dat->bShowAvatar)
  ::CheckMenuItem(hMenu, ID_VIEW_SHOWAVATAR,MF_BYCOMMAND | MF_CHECKED);
else
  ::CheckMenuItem(hMenu, ID_VIEW_SHOWAVATAR,MF_BYCOMMAND | MF_UNCHECKED);
```
我真的不是强调非用这种方式修正代码不可。这样也许更易阅读，但稍微有点长，所以这更多的是个人偏好。
### 建议：
我的建议很简单 —— 设法避免复杂的表达式，尤其是在使用三元运算符的时候。还有，别忘了圆括号。
要记住，操作符 “?:”很危险！有时你会忘记它的优先级比较低，并且容易写出错误的表达式。人们常常在塞满一个字符串时使用这个操作符，因此尽量不要那么做。
