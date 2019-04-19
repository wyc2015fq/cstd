# shell之xargs与-exec与管道的区别你造吗？ - Koma Hub - CSDN博客
2019年03月10日 15:17:11[Koma_Wong](https://me.csdn.net/Rong_Toa)阅读数：8
个人分类：[Shell](https://blog.csdn.net/Rong_Toa/article/category/7346222)
**Table of Contents**
[1、xargs作用：](#1%E3%80%81xargs%E4%BD%9C%E7%94%A8%EF%BC%9A)
[2、使用模式：](#2%E3%80%81%E4%BD%BF%E7%94%A8%E6%A8%A1%E5%BC%8F%EF%BC%9A)
[3、xargs常用选项](#3%E3%80%81xargs%E5%B8%B8%E7%94%A8%E9%80%89%E9%A1%B9)
[4、xargs与管道|的区别](#4%E3%80%81xargs%E4%B8%8E%E7%AE%A1%E9%81%93%7C%E7%9A%84%E5%8C%BA%E5%88%AB)
[5、xargs与-exec的区别](#5%E3%80%81xargs%E4%B8%8E-exec%E7%9A%84%E5%8C%BA%E5%88%AB)
### 1、xargs作用：
（1）将前一个命令的标准输出传递给下一个命令，作为它的参数，xargs的默认命令是echo，空格是默认定界符
（2）将多行输入转换为单行
### 2、使用模式：
`front command | xargs -option later command`
front command： 前一个命令
-option： xargs的选项
later command： 后一个命令
一般与管道“|”结合使用
### 3、xargs常用选项
-n： 指定一次处理的参数个数
-d： 自定义参数界定符
-p： 询问是否运行 later command 参数
-t ： 表示先打印命令，然后再执行
-i ： 逐项处理
…更多参数查看man xargs
### 4、xargs与管道|的区别
管道“|” 用来将前一个命令的标准输出传递到下一个命令的标准输入。
xargs 将前一个命令的标准输出传递给下一个命令，作为它的参数。
可见，标准输入与命令参数是不同的。个人理解，命令参数就是直接跟在命令后面的，标准输入可以是键盘，文件等。
所以，管道符 | 所传递给程序的不是简单地在程序名后面输入的参数，它们会被程序内部的读取功能如scanf和gets等接收，而xargs则是将内容作为普通的参数传递给程序，相当于直接跟在命令后面。况且，有些命令是不接受标准输入的，比如kill，rm等命令。
总结：管道符后不加xargs相当于先将xargs后面的命令回车执行一下再从键盘里输入管道符前面命令执行的结果内容
加上xargs 相当于执行命令（xargs后面的命令）后，直接从键盘输入管道符前面命令执行的结果内容再回车，即管道前面的结果直接跟在xargs 后面的命令后面
再总结一下，就是回车的先后顺序不太一样。
### 5、xargs与-exec的区别
**-exec**：
{}表示命令的参数即为所找到的文件,以；表示comman命令的结束。
\是转义符，因为分号在命令中还有它用途，所以就用一个\来限定表示这是一个分号而不是表示其它意思。
**-ok**：
和 -exec 的作用相同，格式也一样，只不过以一种更为安全的模式来执行该参数
所给出的shell给出的这个命令之前，都会给出提示，让用户来确定是否执行。
**xargs将参数一次传给echo**，即执行：echo begin ./xargs.txt ./args.txt
```
find . -name '*.txt' -type f | xargs echo begin
```
**exec一次传递一个参数**，即执行：echo begin ./xargs.txt;echo begin ./args.txt
```
find . -name '*.txt' -type f -exec echo begin {} \;
```
xargs 要结合管道来完成
```
格式：find [option] express |xargs command
```
很明显，exec是对每个找到的文件执行一次命令，除非这单个的文件名超过了几k，否则不会出现命令行超长出报错的问题。
而xargs是把所有找到的文件名一股脑的转给命令。当文件很多时，这些文件名组合成的命令行参数很容易超长，导致命令出错。
另外， find | xargs 这种组合在处理有空格字符的文件名时也会出错，因为这时执行的命令已经不知道哪些是分割符、哪些是文件名中的空格！ 而用exec则不会有这个问题。
总结
相比之下，也不难看出各自的缺点
> - 1、exec 每处理一个文件或者目录，它都需要启动一次命令，效率不好;
- 2、exec 格式麻烦，必须用 {} 做文件的代位符，必须用 \; 作为命令的结束符，书写不便。
- 3、xargs 不能操作文件名有空格的文件；
- 4、exec参数是一个一个传递的，传递一个参数执行一次命令；xargs一次将参数传给命令，可以使用-n控制参数个数。
- 5、exec文件名有空格等特殊字符也能处理；xargs不能处理特殊文件名，如果想处理特殊文件名需要特殊处理
综上，如果要使用的命令支持一次处理多个文件，并且也知道这些文件里没有带空格的文件，那么使用 xargs比较方便; 否则，就要用 exec了。
**转自微信：**[shell之xargs与-exec与管道的区别你造吗？](http://mp.weixin.qq.com/s?__biz=MzA4NzQzMzU4Mg==&mid=2652923816&idx=1&sn=0ceed44639fe65c80ae6918a87d89166&chksm=8bed4fa9bc9ac6bff9fdf4573624a0c5b79547890085641e307ea93940bbd8b3d3face09118b&mpshare=1&scene=1&srcid=0305rwPkPSiwaGUhvPCVOFUN#rd)
