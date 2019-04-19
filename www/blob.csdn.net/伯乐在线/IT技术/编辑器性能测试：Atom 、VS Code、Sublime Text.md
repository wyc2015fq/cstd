# 编辑器性能测试：Atom 、VS Code、Sublime Text - 文章 - 伯乐在线
本文由 [伯乐在线](http://blog.jobbole.com) - [刘唱](http://www.jobbole.com/members/liuchang) 翻译，[黄利民](http://www.jobbole.com/members/huanglimin) 校稿。未经许可，禁止转载！
英文出处：[xinhong](https://blog.xinhong.me/post/sublime-text-vs-vscode-vs-atom-performance-dec-2016/)。欢迎加入[翻译组](https://github.com/jobbole/translation-project)。
说到文本编辑器，我用 Sublime Text 已经差不多有三年了，也一直没遇到什么问题。在一两年前，我尝试过用 Atom，其活跃的社区 (GitHub！我超爱GitHub) 留给我很深刻的印象。Atom 的 UI 设计尤其吸引我，但我仍然坚持使用 Sublime Text 的原因就是因为性能问题：即使在 Atom 1.0 版本发布之后，Atom 还是太慢了。
这几天，我在 Google 上搜索 “Sublime Text vs Atom 2016” 的文章，想看看 Atom 有没有什么显著的改善。然后我就看到了 Visual Studio Code。VS Code 也和 Atom 一样，是使用 Web 技术创建的，但有评论人说 VS Code 的速度要更快。所以我想做一个小测试，看看这几种编辑器的性能差异。
## 编辑器版本
|**Editor**|**Version**|
|----|----|
|[Sublime Text](https://www.sublimetext.com/)|3 beta, build 3126|
|[Atom](https://atom.io/)|1.12.7|
|[Visual Studio Code](https://code.visualstudio.com/)|1.8.1|
|TextEdit|1.12 (329)|
## 方法
用于测试硬件：MacBook Pro 2016 13-inch 带 Touch Bar， (2.9 GHz Intel Core i5 CPU, 8GB 2133 MHz LPDDR3 RAM, 运行macOS Sierra 10.12.2)。所有可见的程序都关闭了。使用 macOS 自带的TextEdit 作为参照。
### Launch Time 启动时间
每种编辑器都是从 Dock 中以点击图标的方式启动的。我记录了从点击图标到第一个窗口完全加载的这个过程的时长。
### Window Open Time 窗口打开的时间
首先启动每种编辑器，随后关闭所有窗口。我记录了从 Dock 菜单（或其他等价菜单）点击“新窗口”到第一个窗口完全加载的时长。
### Files Open Time 打开文件的时间
首先启动每种编辑器，打开一个窗口。将一个文件从 Finder 拖拽到编辑器的窗口中。我记录了从释放文件到文件完全加载的时长。
### Files Generating 文件生成
使用下面这段 Python 脚本分别生成包含 1万行、100万行、1 百万行和 1 千万行文字的四个文件，文件大小分别是 370KB、3.7MB、37MB 和 370MB。

Python
```
template = '''
#include <iostream>
int main() {
    return 0;
}
/*
%s
*/
'''
string = 'abcdefghijklmnopqrstuvwxyz1234567890n'
with open('test-10k.cpp', 'w') as f:
    f.write(template % (string * 10000,))
with open('test-100k.cpp', 'w') as f:
    f.write(template % (string * 100000,))
with open('test-1m.cpp', 'w') as f:
    f.write(template % (string * 1000000,))
with open('test-10m.cpp', 'w') as f:
    f.write(template % (string * 10000000,))
```
### Files Open Time 文件打开时间
首先打开每种编辑器，加载一个文件。我使用活动监视器 (Activity Monitor) 将所有进程占用的内存加在一起。这个过程中使用的文件是 “文件打开时间” 测试中大小为 370KB 的那个文件。
## 结果
### 启动时间（单位：秒）
![](http://ww1.sinaimg.cn/large/e5298966gw1fbl74xcwvyj20rs0h1aav.jpg)
注：
- TextEdit 在启动的时候并没有打开编辑窗口，而是打开了一个选择文件的窗口。
### 窗口打开时间（单位：秒）
![](http://ww3.sinaimg.cn/large/e5298966gw1fbl74xpm9mj20rs0gzq3r.jpg)
注：
- TextEdit 打开窗口的时候有一个弹出的动画，这会拖慢一点点时间
### 文件打开的时间（单位：秒）
![](http://ww1.sinaimg.cn/large/e5298966gw1fbl74y3j0aj20rs0l2gmt.jpg)
注：
- Atom 打不开“1百万行”的文件，在大约 40 秒后会报告“崩溃”。
- Visual Studio Code 不允许我打开 “1千万行” 的文件，会报 “文件过大”
- Atom 在打开 “10万行” 的文件时不能保持语法高亮。
- Visual Studio Code在打开 “1百万行” 的文件时不能保持语法高亮。
- TextEdit 没有语法高亮功能。
- TextEdit 在打开文件时有一个弹出动画，会拖慢一点点时间。
### 内存使用情况（单位：MB）
![](http://ww1.sinaimg.cn/large/e5298966gw1fbl74yvrvkj20rs0gxwfo.jpg)
## 结论
Atom 和 VS Code 的表现相比于 Sublime Text 和 TextEdit 要明显差一些：启动和打开窗口的时间很明显地会慢几秒钟，而且会占用更多的内存。
在打开文件和 RAM 使用这些方面，Visual Studio Code 要优于 Atom，它能处理更大的文件，而且速度比 Atom 更快。在我测试 3.7MB 的这个文件时，Visual Studio Code 可以在一秒内打开，但是 Atom 要用超过两秒的时间。
Sublime Text 仍然是速度之王，Visual Studio Code 排第二，Atom 倒数第一。
题外话：从这个月开始，我要用 Visual Studio Code 来代替 Sublime Text了，Visual Studio Code 看起来更好玩。
> 
**打赏支持我翻译更多好文章，谢谢！**
[打赏译者](#rewardbox)
#### 打赏支持我翻译更多好文章，谢谢！
![](http://jbcdn2.b0.upaiyun.com/2016/07/acb658caeaa7401d5d7d20f28a1eab52.png)
