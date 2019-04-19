# 每天学点C++知识：复制粘贴代码千万要小心 - 文章 - 伯乐在线
本文由 [伯乐在线](http://blog.jobbole.com) - [JingerJoe](http://www.jobbole.com/members/JingerJoe) 翻译，[至秦](http://www.jobbole.com/members/applevip) 校稿。未经许可，禁止转载！
英文出处：[Andrey Karpov](http://cpphints.com/hints/2)。欢迎加入[翻译组](https://github.com/jobbole/translation-project)。
有个国外团队检测了 200 多个 C/C++ 开源项目，包括了 Php、Qt 和 Linux 内核等知名项目。于是他们每天分享一个错误案例，并给出相应建议。本篇案例来自 Audacity 源代码。
### 错误代码：

C++
```
sampleCount VoiceKey::OnBackward (....) {
  ...
  int atrend = sgn(buffer[samplesleft - 2]-
                   buffer[samplesleft - 1]);                          
  int ztrend = sgn(buffer[samplesleft - WindowSizeInt-2]-
                   buffer[samplesleft - WindowSizeInt-2]);
  ...
}
```
### 解释：
“buffer[samplesleft – WindowSizeInt-2]” 该表达式是减去其自身。这个错误是由于复制粘贴代码引起的，该程序员复制了一行代码，却忘记把 2 改为 1 了。
### 正确的代码：

C++
```
int ztrend = sgn(buffer[samplesleft - WindowSizeInt-2]-
                 buffer[samplesleft - WindowSizeInt-1]);
```
### 建议：
复制粘贴代码时千万要小心。
不建议大家拒绝使用复制粘贴的方法，毕竟它确实很有用，使用起来太方便了。但用的时候千万要小心，不要着急。
俗话说有备无患。记住复制粘贴代码可能会导致大量的错误。看看这些通过 V501 诊断发现 bug 的例子中，有一半的错误都是由于使用复制粘贴方法导致的，另外一半是因为拼写错误。当然关于这个话题，我们另一篇文章专门写到它。
当你复制代码并进行修改的时候，时刻要记得检查你是否完全做对了！千万别偷懒！
另外，我们还会讨论更多关于复制粘贴的话题，我打赌你还没意识到这个问题影响的范围有多广，但是我保证会让你印象深刻。
