# C文件的格式化工具（astyle） - xcw0754 - 博客园
# [C文件的格式化工具（astyle）](https://www.cnblogs.com/xcw0754/p/6236264.html)
### 工具简介
astyle用于格式化 C，C++，C#, Java等文件，方便将代码风格转变成预期的效果。如改变缩进方式、括号风格等等。
### 工具的安装
环境 ubuntu 14.04
`$apt-get install astyle`
### 工具的简单使用
格式化单个文件
`$astyle xxx.c`
或者批量格式化
`$astyle x1.c x2.c x3.c x5.c`
指令敲完后，成功则提示`格式化 xxx`，其他情况见打印信息。
astyle会在格式化之前将文件备份在`同名文件.orig`中，再格式化。
### 设置默认输出格式
文件指定方式
`$HOME/.astylerc`
环境变量方式
`ARTISTIC_STYLE_OPTIONS`
命令行方式(常用，具体见下面命令行options)
`--options`
若没有指定输出格式，那么会默认以4个空格为缩进进行格式化。
### 命令行options
`astyle [options] < Original > Beautified`
> 
括号风格
-A10 推荐一下这种风格，名曰"One True Brace Style"，函数的大括号各独占一行，函数内的左大括号紧跟行尾。
括号的风格很多，有A1到A12，都可以尝试一下效果。
> 
缩进风格
-s# 以空格作为缩进，#号可以代替一个数值，用于设置每层的缩进长度，范围是2 and 20。默认-s4
-t# 以tab作为缩进，#号可以代替一个数值，用于设置每层的缩进长度，范围是2 and 20。
> 
缩进选项
-C 结构体、class中的public和protected等的缩进。
-S switch的case的缩进。
-K switch的case的缩进，默认是这种，case和switch同列的。
-N 为namespace块添加缩进，对java文件无效。
-Y C++的注释方式//加上一个空格
> 
padding
-f if、for、while的上面都空一行。
-F if、for、while的上面都空一行，else、catch的前后括号都写在一行。
-p 操作符左右带空格，一旦使用就无法缩回去了。
-P 圆括号之后之前都加一个空格。
-H if、for、while之后都加一个空格。
-U 取消-H。
-xe 删除空行。
-k# #可以替换1、2、3，表示将C/C++中的*、&、^放在类型与变量名的中间的左边、中间、右边。
> 
格式
-e 将else if打断，拆成两行来写。
-j 为没有带括号的if、for、while之后的一行加上一对括号。
-J 与-j相反。
-O 不拆开写在一行的块。
-o 让复杂的表达式仍保留在一行。
-c 将tab转成空格。
--mode=c 用C/C++模式。
--mode=cs 用C#模式。
--mode=java 用Java模式。
> 
其他选项
--exclude="###" 将文件或者目录忽略，不进行处理。比如--exclude="foo bar.cpp"
-q 安静模式
-Q 只显示已经格式化的文件
> 
推荐指令
`astyle -A10 -j -O -o -c -k3 -H -S -Y foo.c`

