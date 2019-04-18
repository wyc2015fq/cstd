# GDB常用命令与技巧（超好用的图形化gdbgui） - Likes的博客 - CSDN博客
2019年01月09日 10:44:44[Lailikes](https://me.csdn.net/songchuwang1868)阅读数：409
**目录**
[一、常用命令](#%E4%B8%80%E3%80%81%E5%B8%B8%E7%94%A8%E5%91%BD%E4%BB%A4)
[二、调试脚本](#%E4%BA%8C%E3%80%81%E8%B0%83%E8%AF%95%E8%84%9A%E6%9C%AC)
[三、图像界面](#%E4%B8%89%E3%80%81%E5%9B%BE%E5%83%8F%E7%95%8C%E9%9D%A2)
# 一、常用命令
|**命令**|**命令缩写**|**命令说明**|
|----|----|----|
|list|l|显示多行源代码|
|break|b|设置断点|
|break if|b if|当满足某个条件时停止|
|delete|d|删除断点|
|disable||禁用断点|
|enable||允许断点|
|info|i|描述程序状态，比如： i break 显示有哪些断点|
|run|r|开始运行程序|
|display|disp|跟踪查看某个变量，每次停下来都显示其值|
|print|p|打印内部变量值|
|watch||监视变量值新旧的变化|
|step|s|执行下一条语句，如果该语句为函数调用，则进入函数执行第一条语句|
|next|n|执行下一条语句,如果该语句为函数调用,不会进入函数内部执行(即不会一步步地调试函数内部语句）|
|continue|c|继续程序的运行，直到遇到下一个断点|
|finish||如果进入了某个函数，返回到调用调用它的函数，jump out|
|set var name = v||设置变量的值|
|backtrace|bt|查看函数调用信息（堆栈）|
|start|st|开始执行程序，在main函数中的第一条语句前停下|
|frame|f|查看栈帧，比如 frame 1 查看1号栈帧|
|quit|q|离开gdb|
|edit||在gdb中进行编辑|
|whatis||查看变量的类型|
|search||搜索源文件中的文本|
|file||装入需要调试的程序|
|kill|k|终止正在调试的程序|
常见问题：在文件组织比较复杂的代码中，打断点时需要指出相对路径，若是想打断点到函数名，必须给出完整的命名空间
# 二、调试脚本
优点：
- 自己定义一些方便的命令，比如打印STL容器中的内容
- 不用每次打开gdb都要重新打一次断点
使用方法：
   一般有三种方法：[https://blog.csdn.net/cnsword/article/details/16337031](https://blog.csdn.net/cnsword/article/details/16337031)
好用的调试脚本推荐：
- 查看STL容器：[http://www.yolinux.com/TUTORIALS/src/dbinit_stl_views-1.03.txt](http://www.yolinux.com/TUTORIALS/src/dbinit_stl_views-1.03.txt)
- gdb init：[https://github.com/gdbinit/Gdbinit/blob/master/gdbinit](https://github.com/gdbinit/Gdbinit/blob/master/gdbinit)
# 三、图像界面
### 1、gdb自带的简易图像界面tui
        使用方法：
```bash
gdb --tui a.out
```
         效果：
![](https://img-blog.csdnimg.cn/2019010909554850.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3NvbmdjaHV3YW5nMTg2OA==,size_16,color_FFFFFF,t_70)
使用方法详见另外一篇博客：
[https://blog.csdn.net/xu415/article/details/19021759](https://blog.csdn.net/xu415/article/details/19021759)
# 2、gdbgui
将调试的gui在浏览器中显示
[https://gdbgui.com/](https://gdbgui.com/)
- 安装：[https://github.com/cs01/gdbgui](https://github.com/cs01/gdbgui)
        a、安装pipx    
`curl https://raw.githubusercontent.com/cs01/pipx/master/get-pipx.py | python3`
        b、安装gdbgui
`pipx install gdbgui`- 使用
`gdbgui -p 10001 -r "a.out"`
-p 10001表示设定的端口是10001，-r 表示是远程remote调试模式（即调试界面不在Linux机上）
运行成功如图所示
![](https://img-blog.csdnimg.cn/20190109142712959.png)
之后通过Chrome浏览器连接相应的地址，需要强调的是可能需要关闭Linux上的防火墙，效果如下：
![](https://img-blog.csdnimg.cn/20190109142933462.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3NvbmdjaHV3YW5nMTg2OA==,size_16,color_FFFFFF,t_70)
上面是漂亮的图形化界面，右边是各种监视窗口，下面是gdb命令行
