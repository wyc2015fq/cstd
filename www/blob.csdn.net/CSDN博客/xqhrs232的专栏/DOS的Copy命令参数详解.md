# DOS的Copy命令参数详解 - xqhrs232的专栏 - CSDN博客
2014年08月15日 16:35:11[xqhrs232](https://me.csdn.net/xqhrs232)阅读数：4144
原文地址::[http://zhongsion.blog.163.com/blog/static/22646272008478255656/](http://zhongsion.blog.163.com/blog/static/22646272008478255656/)
相关文章
1、copy_百度百科----[http://baike.baidu.com/view/596598.htm?fr=aladdin](http://baike.baidu.com/view/596598.htm?fr=aladdin)
2、 DOS命令大全：复制命令详解----[http://www.feiesoft.com/windows/cmd/copy.htm](http://www.feiesoft.com/windows/cmd/copy.htm)
Copy命令参数详解
**[问题]:**Copy命令参数都有哪些呢？Copy命令作用是什么？
**[解决]:**Copy命令作用及参数详解
作用：将一个或多个文件复制到另一个位置。
COPY [/V] [/N] [/Y | /-Y] [/Z] [/A | /B ] source [/A | /B]
[+ source [/A | /B] [+ ...]][destination [/A | /B]]
source       指定要复制的文件。
/A           表示 ASCII 文本文件。
/B           表示二进制文件。
destination  为新文件指定目录和/或文件名。
/V           验证新文件是否正确写入。
/N           复制非 8.3 名称的文件时，使用短文件名（如果可用）。
/Y           不使用确认是否要覆盖现有目标文件的提示。
/-Y          使用确认是否要覆盖现有目标文件的提示。
/Z           用可重新启动模式复制网络文件。
开关 /Y 可以在 COPYCMD 环境变量中预置。
可以在命令行上使用 /-Y 来覆盖它。默认为
使用覆盖提示，除非是从批处理脚本内执行
COPY 命令。
要追加文件，请为目标指定单个文件，而为
源指定多个文件（使用通配符或 file1+file2+file3 格式）。
**[实践]:**以上理论在windows 2003 的Dos(5.2.3790版本)中实践过。

//==================================================================================================================
备注：：
1》怎么看一个DOS的参数-----在CMD的DOS窗口内输入DOS命令+/？就可以了，比如Copy命令就是---------copy/?
