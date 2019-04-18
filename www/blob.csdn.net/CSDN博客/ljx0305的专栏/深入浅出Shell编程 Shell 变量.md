# 深入浅出Shell编程: Shell 变量 - ljx0305的专栏 - CSDN博客
2009年05月16日 16:21:00[ljx0305](https://me.csdn.net/ljx0305)阅读数：520标签：[shell																[编程																[parameters																[path																[command																[bash](https://so.csdn.net/so/search/s.do?q=bash&t=blog)](https://so.csdn.net/so/search/s.do?q=command&t=blog)](https://so.csdn.net/so/search/s.do?q=path&t=blog)](https://so.csdn.net/so/search/s.do?q=parameters&t=blog)](https://so.csdn.net/so/search/s.do?q=编程&t=blog)](https://so.csdn.net/so/search/s.do?q=shell&t=blog)
个人分类：[Linux程序及使用](https://blog.csdn.net/ljx0305/article/category/394700)
## 深入浅出Shell编程: Shell 变量 
- [深入浅出Shell编程: Shell 变量](http://writeblog.csdn.net/#sah0/bGHXe04E)- [1 系统变量](http://writeblog.csdn.net/#saTCMgWpQkMsQ)
- [2 Shell用户变量](http://writeblog.csdn.net/#sage56.oDPQ6s)- [2.1 基础](http://writeblog.csdn.net/#saRRyAxcKK57E)
- [2.2 使用技巧](http://writeblog.csdn.net/#sa2VfSkwSg/.o)
- [2.3 shell 中的数组](http://writeblog.csdn.net/#saty0eeyV9npg)
- [3 shell 环境变量](http://writeblog.csdn.net/#sa2jlRvOybTRg)
先不要管Shell的版本，来看看Shell 变量，在Shell中有三种变量：系统变量，环境变量，用户变量。其中用户变量在编程过程中使用最多，系统变量在对参数判断和命令返回值判断会使用，环境变量主要是在程序运行的时候需要设置。 
### 1 系统变量 
Shell常用的系统变量并不多，但却十分有用，特别是在做一些参数检测的时候。下面是Shell常用的系统变量 
|[表示方法](http://writeblog.csdn.net/twiki/bin/view.pl/Main/SSVariableIntro?sortcol=0;table=1;up=0#sorted_table)|[描述](http://writeblog.csdn.net/twiki/bin/view.pl/Main/SSVariableIntro?sortcol=1;table=1;up=0#sorted_table)|
|----|----|
|$n|$1 表示第一个参数，$2 表示第二个参数 ...|
|$#|命令行参数的个数|
|$0|当前程序的名称|
|$?|前一个命令或函数的返回码|
|$*|以"参数1 参数2 ... " 形式保存所有参数|
|$@|以"参数1" "参数2" ... 形式保存所有参数|
|$$|本程序的(进程ID号)PID|
|$!|上一个命令的PID|
其中使用得比较多得是 $n $# $0 $? ,看看下面的例子： 
#!/bin/sh#This file is used to explain the shell system variable.echo "the number of parameter is $# ";echo "the return code of last command is $?";
echo "the script name is $0 ";
echo "the parameters are $* ";
echo "/$1 = $1 ; /$2 = $2 ";
下面是运行结果： 
-bash-2.05b$ ./chapter2.1.sh winter stlchina
the number of parameter is 2 
the return code of last command is 0
the script name is ./chapter2.1.sh 
the parameters are winter stlchina 
$1 = winter ; $2 = stlchina 
这个例子太简单了，一点也不实用，下面来个实用的，如果你看不懂，没有关系，后面的内容会有详细解释。 
#!/bin/shif [ $# -ne 2 ] ; thenecho "Usage: $0 string file";
exit 1;
figrep $1 $2 ;
if [ $? -ne 0 ] ; thenecho "Not Found /"$1/" in $2";
exit 1;
fiecho "Found /"$1/" in $2";
上面的例子中使用了$0 $1 $2 $# $? 等变量，下面是程序的解释： 
- 判断运行参数个数，如果不等于2，显示使用"用法帮助", 其中 $0 表示就是脚本自己。 
- 用grep 在$2 文件中查找$1 字符串。 
- 判断前一个命令运行后的返回值(一般成功都会返回0, 失败都会返回非0)。 
- 如果没有成功显示没找到相关信息，否则显示找到了。 
- 其中/"表示转义，在"" 里面还需要显示"号，则需要加上转义符/" . 
下面运行的例子： 
./chapter2.2.sh usage chapter2.2.sh 
Not Found "usage" in chapter2.2.sh
-bash-2.05b$ ./chapter2.2.sh Usage chapter2.2.sh      
echo "Usage: $0 string file";
Found "Usage" in chapter2.2.sh
### 2 Shell用户变量 
#### 2.1 基础 
不管系统变量有多少，对于需求来说，总是不够的。用户变量是最常用到的变量，使用也十分简单。 
用户定义的变量必须由字母数字及下划线组成,并且变量名的第一个字符不能为数字, 与其它UNIX名字一样,变量名是大小写敏感的. 对于用户变量,用户可按如下方式赋值: 
name="Winter"
在引用变量时,需在前面加$符号，用户也可以在变量间进行相互赋值,如: 
name="Winter"
WINTER=$name
echo "Hello $WINTER !"
输出结果应该很清楚：Hello Winter ! 
这里需要注意一点：变量和'='之间不要有空格，'='和赋值也不要有空格，否则shell不会认为变量被定义。掌握了基本的使用方法，你可以完全开始你的编程工作了。不过有时候需要未雨绸缪，下面介绍用户变量的一些技巧。 
#### 2.2 使用技巧 
也可以用变量和其他字符组成新的字,这时可能需要把变量用{}括起,如: 
SAT=Satur
echo Today is ${SAT}day
输出结果是： Today is Saturday 
有时候为了避免变量名和别的字符产生混淆，你最好养成习惯把变量名用{}括起来。 
对于未赋值的变量, Shell以空值对待, 用户也可以用unset命令清除给变量赋的值.看一个例子： 
#!/bin/shecho "a=$a" ;
a=2
echo "a=$a" ;
unset a
echo "a=$a" ;
先猜猜结果是什么？ 
-bash-2.05b$ ./test.sh 
a=
a=2
a=
如果你懂C++，你应该知道有个变量修饰符"const"，用于避免程序一不小心对变量进行修改。在shell中，对于用户变量，你可以使用同样的修饰符"readonly", 如果我把上面的例子修改成这样: 
#!/bin/shecho "a=$a" ;
#下面增加了readonly
readonly a=2    
echo "a=$a" ;
unset a
echo "a=$a" ;
其结果当然会不一样了： 
-bash-2.05b$ ./test.sh 
a=
a=2
a=2
#### 2.3 shell 中的数组 
shell变量中还能设置数组，但是不同的shell版本有不同数组赋值方法，而bourne shell 中不支持数组方式。因此，如果不是十分需要，还是建议你不要使用数组。若你的数据结构十分复杂，必须要使用数组，那么我建议你还是选择别的语言吧，shell不是万能的。 
shell有两种赋值方式，第一种是直接用下标赋值： 
name[0]="Tom"
name[1]="Tomy"
name[2]="John"
...
另一种方式对于不同的shell版本不一样。bash中赋值： 
#!/usr/local/bin/bash
name=("Tom" "Tomy" "John")
for i in 0  1 2
doecho $i:${name[$i]} ;
done;
上面两种赋值方式达到的效果一样。另外，你看见访问数组元素的方法了吗？使用${name[index]}的方式。注意第一行使用的是#!/usr/local/bin/bash, 和以前有些不一样哦。其输出结果是： 
-bash-2.05b$ ./test.sh    
0:Tom
1:Tomy
2:John
### 3 shell 环境变量 
shell 环境变量是所有shell 程序都会接受的参数。shell程序运行时，都会接收一组变量，这组变量就是环境变量。常用的环境变量： 
|名称|描述|
|----|----|
|PATH|命令搜索路径,以冒号为分隔符.注意与DOS下不同的是, 当前目录不在系统路径里|
|HOME|用户home目录的路径名,是cd命令的默认参数|
|COLUMNS|定义了命令编辑模式下可使用命令行的长度|
|EDITOR|默认的行编辑器|
|VISUAL|默认的可视编辑器|
|FCEDIT|命令fc使用的编辑器|
|HISTFILE|命令历史文件|
|HISTSIZE|命令历史文件中最多可包含的命令条数|
|HISTFILESIZE|命令历史文件中包含的最大行数|
|IFS|定义SHELL使用的分隔符|
|LOGNAME|用户登录名|
|MAIL|指向一个需要SHELL监视其修改时间的文件.当该文件修改后, SHELL将发消息You hava mail给用户|
|MAILCHECK|SHELL检查MAIL文件的周期,单位是秒|
|MAILPATH|功能与MAIL类似.但可以用一组文件,以冒号分隔,每个文件后可跟一个问号和一条发向用户的消息|
|SHELL|SHELL的路径名|
|TERM|终端类型|
|TMOUT|SHELL自动退出的时间,单位为秒,若设为0则禁止SHELL自动退出|
|PROMPT_COMMAND|指定在主命令提示符前应执行的命令|
|PS1|主命令提示符|
|PS2|二级命令提示符,命令执行过程中要求输入数据时用|
|PS3|select的命令提示符|
|PS4|调试命令提示符|
|MANPATH|寻找手册页的路径,以冒号分隔|
|LD_LIBRARY_PATH|寻找库的路径,以冒号分隔|
这些变量，要关注的最多的就是PATH, 其重要性不要我说了吧？ 
如果你希望把你定义的变量让其他所有的shell程序都能使用，也就是定义新的环境变量。你只要使用export关键词就可以了。例如： 
export MY_NAME=Winter
export PATH=/home/winter/bin:$PATH
上面的程序中，第一行输出MY_NAME变量，第二行是在环境变量PATH中增加一个路径/home/winter/bin 。如果你希望这些设置在你登陆unix/linux都有效，你需要把他们加入到你的shell启动脚本中, 如果是使用bash 
~/.bash_profile
其他版本你看一眼就知道了，在你的home目录下，以"."开头的文件，一般都会隐藏起来，你需要使用'ls -al'命令来显示。
引用：[http://www.stlchina.org/twiki/bin/view.pl/Main/SSVariableIntro](http://www.stlchina.org/twiki/bin/view.pl/Main/SSVariableIntro)
