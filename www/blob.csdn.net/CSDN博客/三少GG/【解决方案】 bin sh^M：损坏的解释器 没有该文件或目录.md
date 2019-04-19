# 【解决方案】/bin/sh^M：损坏的解释器: 没有该文件或目录 - 三少GG - CSDN博客
2011年06月16日 18:36:00[三少GG](https://me.csdn.net/scut1135)阅读数：2056
## [/bin/sh^M：损坏的解释器: 没有该文件或目录 【解决方案】](http://www.cnblogs.com/freakshow/archive/2011/01/10/1932059.html)
同事丢给我一个**.sh文件 运行之，报上面的错误，ubuntu论坛上有专门的答案，记录下来，以备后忘。
问题原因：脚本的编码问题，windows下编辑的文本copy到linux下就带这个^M结尾。 
解决方案：用 vim 打开文件，然后执行冒号命令：
**代码:**
:set ff=unix
:wq 
./configure 不能执行：
解决方法： configure文件，右键属性，权限-->允许执行！！！
