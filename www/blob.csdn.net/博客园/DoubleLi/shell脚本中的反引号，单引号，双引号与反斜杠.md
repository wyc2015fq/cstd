# shell脚本中的反引号，单引号，双引号与反斜杠 - DoubleLi - 博客园






转自：http://blog.sina.com.cn/s/blog_6561ca8c0102we2i.html



反引号位 (`)经常被忽略，而且容易与单引号弄混。它位于键盘的Tab键的上方、1键的左方。单引号(')位于Enter键的左方。在Linux中反引号起着命令替换的作用。命令替换是指shell能够将一个命令的标准输出插在一个命令行中任何位置，将反引号中的字符串做为命令来执行，我们在用shell编程时经常用的到，将系统命令的执行结果赋给一个变量。如下，shell会执行反引号中的date命令，把结果插入到echo命令显示的内容中。

> echo The date is `date`
the date is 2015年 11月 25日 星期三 16:18:56 CST
shell中有两种方法作命令替换，除了反引号还可以用$(…)结构括起来，$(…)格式受到POSIX标准支持，也利于嵌套。但反引号内不能再引用反引号，而$()中可以引用反引号。


单引号、双引号用于用户把带有空格的字符串赋值给变量的分界符。
> str="Today is Monday"
> echo $str
Today is Monday
如果没有单引号或双引号，shell会把空格后的字符串解释为命令。
> str=Today is Monday
bash: is: command not found


单引号和双引号的区别。单引号告诉shell忽略所有特殊字符，而双引号忽略大多数，但不包括①美元符号②反引号③反斜杠，这3种特殊字符将不被忽略。 不忽略美元符号意味着shell在双引号内部可进行变量名替换。
> testvalue=100
> echo 'The testvalue is $testvalue'
The testvalue is $testvalue
> echo "The testvalue is $testvalue"
The testvalue is 100


反斜杠一般用作转义字符，或称逃脱字符。linux如果echo要让转义字符发生转义功能，就要使用-e选项，且转义字符要使用双引号
echo -e “\n”
反斜杠的另一种作用，就是当反斜杠用于一行的最后一个字符时，shell把行尾的反斜杠作为续行，这种结构在分几行输入长命令时经常使用。









