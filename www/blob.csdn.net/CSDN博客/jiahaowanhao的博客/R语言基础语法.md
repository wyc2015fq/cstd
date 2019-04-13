
# R语言基础语法 - jiahaowanhao的博客 - CSDN博客


2018年02月23日 13:56:58[数据分析技术](https://me.csdn.net/jiahaowanhao)阅读数：84标签：[R语言																](https://so.csdn.net/so/search/s.do?q=R语言&t=blog)


[R语言基础语法](http://cda.pinggu.org/view/24820.html)
学习一个新编程语言的惯例是什么？，当然是编写一个“Hello，World！”，下面我们将演示如何使用R编程编写一个“Hello，World！”程序。根据需要，您可以在R命令提示符下编程，也可以使用R脚本文件编写程序。
R命令提示符
当安装好了R开发环境以后，那么通过在命令提示符下键入以下命令，就可以启动R命令提示符 -
$ R
Shell
或者，在Windows系统下 -
C:\Users\Administrator> R
Shell
这将启动R解释器，您将获得一个提示>您可以在以下开始编写程序 -
![](http://cda.pinggu.org/uploadfile/image/20180223/20180223065736_33670.png)
上图程序中，第一个语句定义了一个字符串变量str，然后分配一个字符串“Hello，World~！”，下一个语句print()用于打印存储在变量str中的值。
R脚本文件
通常，我们通过在脚本文件中编写R代码，然后再通过名为Rscript的R解释器的帮助下，在命令提示符下执行这些脚本。 所以下面演示如何在一个名为test.R的文本中编写代码，如下 -
\# My first program in R Programming
myString <- "Hello, World!"
print ( myString)
将上述代码保存在test.R文件中，并在Linux命令提示符下执行，如下所示。即使您使用Windows或其他系统，语法将保持不变。
$ Rscript test.R
Shell
当我们运行上述程序时，会产生以下结果(Windows)
C:\Users\Administrator>F:
F:>cd worksp\R
F:\worksp\R>dir
2017/08/22  21:52    <DIR>          .
2017/08/22  21:52    <DIR>          ..
2017/08/22  21:53                85 test.R
1 个文件             85 字节
2 个目录 53,158,862,848 可用字节
F:\worksp\R>Rscript test.R
[1] "Hello, World!"
F:\worksp\R>

注释
注释就像在R程序中帮助文本，并且在执行实际程序时被解释器忽略。单个注释在语句的开头使用\#写成如下：
\# My first program in R Programming
R
R不支持多行注释，但可以执行以下操作：
if(FALSE) {
"This is a demo for multi-line comments and it should be put inside either a single
OR double quote"
}
myString <- "Hello, World!"
print ( myString)

虽然上述注释由R解释器执行，但不会干扰您的实际编程代码。所以我们可以把要注释的内容放入单引号或双引号中。

