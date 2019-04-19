# 每天学点C++知识：不要节约代码行数 - 文章 - 伯乐在线
本文由 [伯乐在线](http://blog.jobbole.com) - [LynnShaw](http://www.jobbole.com/members/shaoai37) 翻译，[至秦](http://www.jobbole.com/members/applevip) 校稿。未经许可，禁止转载！
英文出处：[Andrey Karpov](http://cpphints.com/hints/15)。欢迎加入[翻译组](https://github.com/jobbole/translation-project)。
有个国外团队检测了 200 多个 C/C++ 开源项目，包括了 Php、Qt 和 Linux 内核等知名项目。于是他们每天分享一个错误案例，并给出相应建议。本篇案例来自 KDE4 源码。
### 错误代码：

C++
```
void LDAPProtocol::del( const KUrl &_url, bool )
{
  ....
  if ( (id = mOp.del( usrc.dn() ) == -1) ) {
    LDAPErr();
    return;
  }
  ret = mOp.waitForResult( id, -1 );
  ....
}
```
### 说明：
一些程序员竭尽全力想把更多的代码压缩到一行。他们尤其热衷于“if”条件语句，把赋值和比较一次性完成。
一个[典型](http://www.viva64.com/en/examples/V593/)的错误模式是使用 if (A = Foo() == Error) 这样的表达式。上面我们正在处理的实例代码正是这种错误。
比较操作的优先级高于赋值操作。这就是为什么”mOp.del( usrc.dn() ) == -1″ 比较会先执行，然后“true”(1) 或“false”(0) 值被赋给变量 id。
如果 mOp.del() 返回“-1”，函数会终止；否则，函数会持续运行，“id”变量会被赋予一个错误的值。它的结果总为 0。
### 正确代码：

C++
```
id = mOp.del(usrc.dn());
if ( id == -1 ) {
```
### 建议：
不要懒得去写多余的代码：毕竟复杂的表达式很难读懂。首先进行赋值，然后再比较。这样以后维护你代码的程序员也会轻松很多，也会减少出错的可能。
今天的这个小技巧看起来很微不足道，不过我希望它会帮到你，牢牢记住并迫使自己写干净整洁且正确的代码，而不是“看我有多专业！”的风格。
