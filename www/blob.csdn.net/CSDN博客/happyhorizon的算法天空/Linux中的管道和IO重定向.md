# Linux中的管道和IO重定向 - happyhorizon的算法天空 - CSDN博客
2018年03月29日 21:32:16[lxy_Alex](https://me.csdn.net/happyhorizion)阅读数：87
# Linux中的管道和IO重定向
## 管道技术是什么?
管道是linux中进程之间的通信技术, 核心思想是先进先出(FRIST IN, FIRST OUT, FIFO), 管道的英文是pipeline, 这个词很生动地描述了数据和信息在进程之间的流动就像在水管中一样. 
## 管道应用
管道的符号是 “ | ”, 利用管道通信的基本格式是: 
`command1 | command2 | command3 | … | commandn`
在上述命令中, command1 的输出作为command2的输入, 并继续在command们中按照先后顺序流动. 到了commandn, 它的输出就会被直接显示在shell上. 
比如我们经常可以见到用管道来帮助我们寻找文件的语句:
`ls | grep data`
输出是当前路径下所有名称包含data的文件: 
```
data_folder                                            
test_data
```
利用管道,配合cat, more, sed, awk等显示和文字处理\提取的工具,可以有很多非常灵活的应用. 
比如在shell中逐行显示当前路径下的文件: `ls | more`
或者逐行显示某文件内容: `cat | more`
比如和sed配合, sed命令的标准格式是:
`sed [选项]... {脚本(如果没有其他脚本)} [输入文件]...`
我们可以用cat等类似命令将数据送入管道, 然后交给sed处理, 这时, sed的命令中就不必指定input了.例如:打印当前路径下的前5个文件名称: 
`ls -l | sed -n '1,5p'`
例如显示包含指定内容的行: 
`cat preprocess.py | sed -n '/import/p'`
得到输出: 
```java
import os
#import sys
import argparse
```
上述这个命令当然也可以不用管道来实现: `sed -n '/import/p' preprocess.p` 可以得到同样的结果. 管道更多地被应用在更复杂的情况下. 例如下面这个例子就结合了管道, 调用命令行工具进行变量赋值和sed. 
```bash
dl@DL~/$var1='Windows Pipeline is quite useful'
dl@DL:~$ replace=Linux
dl@DL:~$ var2=`echo $var1 | sed "s/Window/$replace/g"`
dl@DL::~$  echo $var2
Linuxs Pipeline is quite useful
```
和管道特别相近,也经常配合一起使用的技术是I/O重定向, 下面来谈谈这个.
## I/O重定向
## 什么是I/O重定向?
I/O重定向就是说将文件\命令\程序\脚本,甚至代码块的输出发送到另外一个文件/程序等作为输入.等等,这不是和管道差不多么? 没错, 实际上, 管道只是I/O重定向中的一种, 仅仅是命令之间的输出输入传递, 其他的重定向符号指定的是命令和系统标准输出输入之间的数据传递.  
这里必须要首先谈谈文件标识符(File Descriptor, FD),  当linux启动一个进程的时候,系统会自动为这个进程打开三个文件: 标准输入(FD=0), 标准输出(FD=1), 标准错误输出(FD=2). 下图中描述了这三者和shell命令之间的关系: 
![这里写图片描述](https://img-blog.csdn.net/201803292130167?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L2hhcHB5aG9yaXppb24=/font/5a6L5L2T/fontsize/10/fill/I0JBQkFCMA==/dissolve/20)
命令行工具和系统交互的过程中,经常用到的重定向符号还有: 
```
> filename将标准输出写到文件中
>> filename 将标准输入写入文件中, 如果文件内已经存在内容,则在文件末尾开始继续写入
<filename 将文件读入标准输入中
```
说明: <符号的用法
```
dl@DL:~$ wc -l .vimrc
410 .vimrc
dl@DL:~$ wc -l <.vimrc
410
```
这里用到了wc命令. 这条指令可以帮助我们计算文件的Byte数、字数或是列数，若不指定文件名称，或是所给予的文件名为“-”，则wc指令会从标准输入设备读取数据。
