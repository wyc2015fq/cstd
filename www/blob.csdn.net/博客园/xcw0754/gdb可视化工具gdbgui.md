# gdb可视化工具gdbgui - xcw0754 - 博客园
# [gdb可视化工具gdbgui](https://www.cnblogs.com/xcw0754/p/10125179.html)
gdbgui是一款很强大的gdb可视化工具，基于浏览器调试很方便。后台是python写的，目前使用起来还有些bug，不过已经很满足了。
官网 `https://gdbgui.com`
Github `https://github.com/cs01/gdbgui`
源码里面有个example目录，下面有很多例子。
### 开始
先来一张图看看效果
![](https://img2018.cnblogs.com/blog/641737/201812/641737-20181215232609766-299333757.png)
顶部输入path后敲回车或者`Load Binary`就能加载文件。
顶部右边有几个按钮，依次是开始、继续、暂停、逐步、进入函数、离开函数。
显示相应的汇编语言，可以点击`fetch disassembly`，然后会出现`attr`和`intel`可以选。
重新加载文件，可以点击`reload file`。
显示目录树，可以点击`show filesystem`。
底栏可以输入gdb的命令，显示结果在控制台中。
右部有很多栏：
```
threads     显示函数栈
local variables    本地变量，可实时观察变量值
expressions    可以输入表达式，比如变量名、逻辑表达式、式子等等
Tree    结构体对象之间的关系，很直观实用
memory    显示内存中的数据
breakpoints    断点
signals    发送信号
registers    所有寄存器
```
我觉得这个工具好用的地方就是操作方便，断点只需点一下行号，还能手动输入gdb命令，`memory`只要点一点就能显示出来，也支持手动输入查询。比较特色的是Tree，可以帮助理解复杂的数据结构。最最方便之处就是它是基于web的，在无GUI的系统也可以用得上。

