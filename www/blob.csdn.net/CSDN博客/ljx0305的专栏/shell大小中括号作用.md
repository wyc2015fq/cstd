# shell大小中括号作用 - ljx0305的专栏 - CSDN博客
2012年11月07日 16:22:47[ljx0305](https://me.csdn.net/ljx0305)阅读数：636
个人分类：[Linux](https://blog.csdn.net/ljx0305/article/category/364133)

Shell的强大是毋庸置疑的，方便了我们也迷惑了我们，比如这些杂七杂八的括号，一向自认聪明的我也傻傻分不清了，今天好哈的整理一下。
     在Shell中的小括号,大括号结构和有括号的变量，命令的用法如下:
1.${var} 
2.$(cmd) 
3.()和{} 
4.${var:-string},${var:+string},${var:=string},${var:?string} 
5.$((exp)) 
6.$(var%pattern),$(var%%pattern),$(var#pattern),$(var##pattern)
     现在来一一详细介绍：
     １）Shell中变量的原形 
      这个最常见的变量形式就是$var，打印var用命令
      echo $var
      可是这里有个问题：当你要显示变量值加随意的字符(如$varAA)时，就会出错。系统会认为整个varAA是一个变量，这时就可以用一个大括号来限定变量名称的范围，如${var}AA,这样就好了。
     2）命令替换$(cmd)
      命令替换$(cmd)和符号`cmd`(注意这不是单引号，在美式键盘上，`是ESC下面的那个键)有相同之处.以echo $(ls)来说明整个替换过程：shell扫描一遍命令行，发现了$(cmd)结构，便将$(cmd)中的cmd执行一次，得到其标准输出，再将此输出放到原来命令echo $(ls)中的$(ls)位置，即替换了$(ls),再执行echo命令。如下：
1
$ ls
2
a b c
3
$ echo $(ls)
4
a b c
5
$ echo `ls`
6
a b c
     3）一串的命令执行()和{}
      ()和{}都是对一串的命令进行执行，但有所区别：
A,()只是对一串命令重新开一个子shell进行执行
B,{}对一串命令在当前shell执行
C,()和{}都是把一串的命令放在括号里面，并且命令之间用;号隔开
D,()最后一个命令可以不用分号
E,{}最后一个命令要用分号
F,{}的第一个命令和左括号之间必须要有一个空格
G,()里的各命令不必和括号有空格
H,()和{}中括号里面的某个命令的重定向只影响该命令，但括号外的重定向则影响到括号里的所有命令
     4）几种特殊的替换结构：${var:-string},${var:+string},${var:=string},${var:?string}
      A,${var:-string}和${var:=string}:若变量var为空，则用在命令行中用string来替换${var:-string}，否则变量var不为空时，则用变量var的值来替换${var:-string}；对于${var:=string}的替换规则和${var:-string}是一样的，所不同之处是${var:=string}若var为空时，用string替换${var:=string}的同时，把string赋给变量var： ${var:=string}很常用的一种用法是，判断某个变量是否赋值，没有的话则给它赋上一个默认值。
      B. ${var:+string}的替换规则和上面的相反，即只有当var不是空的时候才替换成string，若var为空时则不替换或者说是替换成变量 var的值，即空值。(因为变量var此时为空，所以这两种说法是等价的)
      C,${var:?string}替换规则为：若变量var不为空，则用变量var的值来替换${var:?string}；若变量var为空，则把string输出到标准错误中，并从脚本中退出。我们可利用此特性来检查是否设置了变量的值。
      补充扩展：在上面这五种替换结构中string不一定是常值的，可用另外一个变量的值或是一种命令的输出。
     5）POSIX标准的扩展计算:$((exp))
      这种计算是符合C语言的运算符，也就是说只要符合C的运算符都可用在$((exp))，甚至是三目运算符。注意：这种扩展计算是整数型的计算，不支持浮点型.若是逻辑判断，表达式exp为真则为1,假则为0。
     6）四种模式匹配替换结构：${var%pattern},${var%%pattern},${var#pattern},${var##pattern}
     第一种模式：${variable%pattern}，这种模式时，shell在variable中查找，看它是否一给的模式pattern结尾，如果是，就从命令行把variable中的内容去掉右边最短的匹配模式
     第二种模式： ${variable%%pattern}，这种模式时，shell在variable中查找，看它是否一给的模式pattern结尾，如果是，就从命令行把variable中的内容去掉右边最长的匹配模式
     第三种模式：${variable#pattern} 这种模式时，shell在variable中查找，看它是否一给的模式pattern开始，如果是，就从命令行把variable中的内容去掉左边最短的匹配模式
     第四种模式： ${variable##pattern} 这种模式时，shell在variable中查找，看它是否一给的模式pattern结尾，如果是，就从命令行把variable中的内容去掉右边最长的匹配模式
     这四种模式中都不会改变variable的值，其中，只有在pattern中使用了*匹配符号时，%和%%，#和##才有区别。结构中的pattern支持通配符，*表示零个或多个任意字符，?表示零个或一个任意字符，[...]表示匹配中括号里面的字符，[!...]表示不匹配中括号里面的字符
01
[root@root shell]# var=testcase
02
[root@root shell]# echo $var
03
testcase
04
[root@root shell]# echo ${var%s*e} 从最右边删除最短匹配
05
testca
06
[root@root shell]# echo $var
07
testcase
08
[root@root shell]# echo ${var%%s*e} 从最右边删除最长匹配
09
te
10
[root@root shell]# echo $var  变量没有改变
11
testcase
12
[root@root shell]# echo ${var#?e} 从最左边删除最短匹配
13
stcase
14
[root@root shell]# echo $var
15
testcase
16
[root@root shell]# echo ${var#*e}  从最左边删除最短匹配
17
stcase
18
[root@root shell]# echo $var
19
testcase
20
[root@root shell]# echo ${var##*e} 从最左边删除最长匹配，即删除所有
21
[root@root shell]# echo $var
22
testcase
23
[root@root shell]# echo ${var##*s} 从最左边删除最长匹配
24
e
25
[root@root shell]# echo $var
26
testcase
27
[root@root shell]# echo ${var#test} 删除test
28
case
29
[root@root shell]# echo $var
30
testcase
31
[root@root shell]# echo ${var#tests} 没有匹配
32
testcase
转自网络：
[http://taotaocoder.blog.163.com/blog/static/200228274201110306203483/](http://taotaocoder.blog.163.com/blog/static/200228274201110306203483/)
引用:[http://blog.chinaunix.net/uid-20104120-id-3286408.html](http://blog.chinaunix.net/uid-20104120-id-3286408.html)
