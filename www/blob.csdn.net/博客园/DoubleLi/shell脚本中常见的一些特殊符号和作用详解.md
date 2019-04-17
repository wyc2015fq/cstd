# shell脚本中常见的一些特殊符号和作用详解 - DoubleLi - 博客园






在编写Shell脚本时，我们需要会用到各种各样的特殊符号，通过这些特殊符号可以使我们编写的代码更加简洁和高效，这里给大家汇总下：

**1、{} 大括号：用法一：通配符扩展**eg: ls 
my_{finger,toe}s
这条命令相当于如下命令的组合：
ls my_fingers my_toes
eg: mkdir 
{userA,userB,userC}-{home,bin,data}
我们将得到 userA-home, userA-bin, userA-data, 
userB-home, userB-bin,userB-data,userC-home, userC-bin, userC-data，这几个目录。

**用法二：可用于语句块的构造**，语句之间用回车隔开。如果你想在某些使用单个语句的地方（比如在AND或OR列表中）使用多条语句，你可以把它们括在花括号{}中来构造一个语句块。
eg:
{
grep 
-v “$cdcatnum” $strack_file > $temp_file
cat $temp_file > 
$strack_file
echo
cat -n file1
} (注：以上大括号中的四句命令够成了一个语句块)

**用法三：参数扩展**${name:-default} 
使用一个默认值（一般是空值）来代替那些空的或者没有赋值的变量name；
${name:=default}使用指定值来代替空的或者没有赋值的变量name；
${name:?message}如果变量为空或者未赋值，那么就会显示出错误信息并中止脚本的执行同时返回退出码1。
${#name} 
给出name的长度
${name%word} 从name的尾部开始删除与word匹配的最小部分，然后返回剩余部分
${name%%word} 
从name的尾部开始删除与word匹配的最长部分，然后返回剩余部分
${name#word} 
从name的头部开始删除与word匹配的最小部分，然后返回剩余部分
${name##word} 
从name的头部开始删除与word匹配的最长部分，然后返回剩余部分
（注，name为变量名，word为要匹配的字符串）
用法三在处理字符串和未知变量时，是很有用的。

**2、[] 中括号： 用法一：通配符扩展：**允许匹配方括号中任何一个单个字符
eg: ls 
/[eh][to][cm]*
相当于执行 ls /etc /home（若有/eom目录，就相当于会执行ls /etc /home 
/eom）
注：在mkdir命令下不能扩展

**用法二：用于条件判断符号：**[]符号可理解为指向test命令的一个软链接，所以其用法可完全参照test，将test位置替换为[便可。
eg: 
if [ "$?" != 0 ] 等价于 if test “$?” != 0
then echo “Executes error”

**3、`command` 反引号：`**command`与$(command)的含义相同，都是返回当前执行命令的结果 


eg: #!/bin/sh
for file in $(ls f*.sh);do
lpr $file
done
exit 
0

该例实现了扩展f*.sh给出所有匹配模式的文件的名字。

**4、'string' 单引号 和 “string” 双引号 **
双引号：如果想在定义的变量中加入空格，就必须使用单引号或双引号，
单、双引号的区别在于双引号转义特殊字符而单引号不转义特殊字符
eg: 
$ heyyou=home
$ echo ‘$heyyou'
$ $heyyou （$没有转义）
eg: $ heyyou=home
$ 
echo “$heyyou”
$ home （很明显，$转义了输出了heyyou变量的值）

**5、$# 它的作用是告诉你引用变量的总数量是多少 **
$$ 
它的作用是告诉你shell脚本的进程号；
$* 以一个单字符串显示所有的脚本传递的参数。等价于$1 $2 $3…….；
$@ 
与$*基本类似（参见序号7），但在数组赋值时有些不同；
$? 前一个命令的退出码；
$- 显示shell使用的当前选项；
$! 
最后一个后台运行的进程ID号。

**6、$((…))语法：对括号内的表达式求值 **eg:
#!/bin/sh
x=0
hile [ "$x" -ne 10 ];do
echo 
$x
x=$(($x+1))
done
exit 0

**7、shell中几种特殊的参数变量的引用**

$1、$2、$3……${10}、${11}、${12}……：表示脚本传入的的各个参数，注意当需表示两位数以后的参数时数字要用花括号括起。
$@ 
列出所有的参数，各参数用空格隔开
$*: 列出所有的参数，各参数用环境变量IFS的第一个字符隔开

**8、命令列表： **
AND列表 statement1 && statement2 
&& statement3 && …:只有在前面所有的命令都执行成功的情况下才执行后一条命令
OR列表 
statement1 || statement2 || statement3 || 
…:允许执行一系列命令直到有一条命令成功为止，其后所有命令将不再被执行
eg:#!/bin/sh
touch file_one
rm -f 
file_two
if [ -f file_one ] && echo “hello” && [ -f file_two 
] && echo ” there”
then
echo “in if”
else
echo “in 
else”
fi
exit 0
上例的输出为：
hello
in 
else
关于AND列表与OR列表，在逻辑判断中很使用，下面就举一个其最常用的例子：
[ condition ] && 
command for true || command for false:
当条件为真时，执行commandfor true 
,当条件为假时，执行command for false

**9、: 冒号：内建空指令，返回值为0**eg: $ :
$ echo $?
$ 
0
while: (该语句结构可实现一个无限循环)

**10、; 分号： 在 shell 中，担任”连续指令”功能的符号就是”分号”**eg:cd ~/backup 
; mkdir startup ; cp ~/.* startup/.

**11、# 井号： 表示符号后面的是注解文字，不会被执行；**
* 
匹配文件名中的任何字符，包括字符串；
？ 匹配文件名中的任何单个字符。
~ 代表使用者的 home 目录

**12、\ 倒斜线：**放在指令前，有取消 aliases（别名） 
的作用；放在特殊符号前，则该特殊符号的作用消失；放在指令的最末端，表示指令连接下一行（使得回车符无效，只起换行作用）

**13、! 感叹号：**通常它代表反逻辑的作用，譬如条件侦测中，用 != 来代表”不等于”

**14、** 次方运算：**两个星号在运算时代表 “次方” 的意思

eg:let 
“sus=2**3″
echo “sus = $sus”
$ sus = 8 -









