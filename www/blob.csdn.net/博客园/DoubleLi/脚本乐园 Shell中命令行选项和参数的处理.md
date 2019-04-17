# 脚本乐园 Shell中命令行选项和参数的处理 - DoubleLi - 博客园






在Linux的Shell中怎样处理tail -n 10 access.log这样的命令行选项呢？这是被别人问起的一个问题，好好学习了一下，进行总结如下：
在bash中，可以用以下三种方式来处理命令行参数，每种方式都有自己的应用场景。
**1.直接处理，依次对$1,$2,...,$n进行解析，分别手工处理；**
2.getopts来处理，单个字符选项的情况（如：-n 10 -f file.txt等选项）；
3.getopt，可以处理单个字符选项，也可以处理长选项long-option（如：--prefix=/home等）。
总结：一般小脚本手工处理也许就够了，getopts能处理绝大多数的情况，getopt较复杂、功能也更强大。

下面分别进行简单的说明：

**1.直接手工处理位置参数**
必须要要知道几个变量，
    *    $0 ：即命令本身，相当于C/C++中的argv[0]
    *    $1 ：第一个参数.
    *    $2, $3, $4 ... ：第2、3、4个参数，依次类推。
    *    $#  参数的个数，不包括命令本身
    *    $@ ：参数本身的列表，也不包括命令本身
    *    $* ：和$@相同，但"$*" 和 "$@"(加引号)并不同，"$*"将所有的参数解释成一个字符串，而"$@"是一个参数数组。
手工处理方式能满足多数的简单需求，配合shift使用也能构造出强大的功能，但处理复杂选项的时候建议用下面的两种方法。
给个实例吧(getargs.sh)：
|#!/bin/bashif [ $# -lt 1 ]; then    echo "error.. need args"    exit 1fiecho "commond is $0"echo "args are:"for arg in "$@"do    echo $argdone|
|----|


运行命令：./getargs.sh 11 22 cc
commond is ./getargs.sh
args are:
11
22
cc

**2.getopts （Shell内置命令）**
处理命令行参数是一个相似而又复杂的事情，为此，C提供了getopt/getopt_long等函数，C++的boost提供了Options库，在shell中，处理此事的是getopts和getopt.
先说一下getopts/getopt的区别吧，getopt是个外部binary文件，而getopts是shell builtin。
|[admin@intlqa142055x ~]$ type getoptgetopt is /usr/bin/getopt[admin@intlqa142055x ~]$ type getoptsgetopts is a shell builtin|
|----|


getopts不能直接处理长的选项（如：--prefix=/home等）
关于getopts的使用方法，可以man bash  搜索getopts
getopts有两个参数，第一个参数是一个字符串，包括字符和“：”，每一个字符都是一个有效的选项，如果字符后面带有“：”，表示这个字符有自己的参数。getopts从命令中获取这些参数，并且删去了“-”，并将其赋值在第二个参数中，如果带有自己参数，这个参数赋值在“OPTARG”中。提供getopts的shell内置了OPTARG这个变变，getopts修改了这个变量。
这里变量$OPTARG存储相应选项的参数，而$OPTIND总是存储原始$*中下一个要处理的元素位置。
while getopts ":a:bc" opt  #第一个冒号表示忽略错误；字符后面的冒号表示该选项必须有自己的参数
代码实例(getopts.sh)：
|echo $*while getopts ":a:bc" optdo        case $opt in                a ) echo $OPTARG                    echo $OPTIND;;                b ) echo "b $OPTIND";;                c ) echo "c $OPTIND";;                ? ) echo "error"                    exit 1;;        esacdoneecho $OPTINDshift $(($OPTIND - 1))#通过shift $(($OPTIND - 1))的处理，$*中就只保留了除去选项内容的参数，可以在其后进行正常的shell编程处理了。echo $0echo $*|
|----|


执行命令：./getopts.sh -a 11 -b -c
-a 11 -b -c
11
3
b 4
c 5
5
./getopts.sh

**3.getopt****（一个外部工具）**
具体用用法可以 man getopt
#-o表示短选项，两个冒号表示该选项有一个可选参数，可选参数必须紧贴选项，如-carg 而不能是-c arg
#--long表示长选项
简单举个例子吧(getopt.sh)：
|#!/bin/bash# A small example program for using the new getopt(1) program.# This program will only work with bash(1)# An similar program using the tcsh(1) script. language can be found# as parse.tcsh# Example input and output (from the bash prompt):# ./parse.bash -a par1 'another arg' --c-long 'wow!*\?' -cmore -b " very long "# Option a# Option c, no argument# Option c, argument `more'# Option b, argument ` very long '# Remaining arguments:# --> `par1'# --> `another arg'# --> `wow!*\?'# Note that we use `"$@"' to let each command-line parameter expand to a# separate word. The quotes around `$@' are essential!# We need TEMP as the `eval set --' would nuke the return value of getopt.#-o表示短选项，两个冒号表示该选项有一个可选参数，可选参数必须紧贴选项#如-carg 而不能是-c arg#--long表示长选项#"$@"在上面解释过# -n:出错时的信息# -- ：举一个例子比较好理解：#我们要创建一个名字为 "-f"的目录你会怎么办？# mkdir -f #不成功，因为-f会被mkdir当作选项来解析，这时就可以使用# mkdir -- -f 这样-f就不会被作为选项。TEMP=`getopt -o ab:c:: --long a-long,b-long:,c-long:: \     -n 'example.bash' -- "$@"`if [ $? != 0 ] ; then echo "Terminating..." >&2 ; exit 1 ; fi# Note the quotes around `$TEMP': they are essential!#set 会重新排列参数的顺序，也就是改变$1,$2...$n的值，这些值在getopt中重新排列过了eval set -- "$TEMP"#经过getopt的处理，下面处理具体选项。while true ; do        case "$1" in                -a|--a-long) echo "Option a" ; shift ;;                -b|--b-long) echo "Option b, argument \`$2'" ; shift 2 ;;                -c|--c-long)                        # c has an optional argument. As we are in quoted mode,                        # an empty parameter will be generated if its optional                        # argument is not found.                        case "$2" in                                "") echo "Option c, no argument"; shift 2 ;;                                *)  echo "Option c, argument \`$2'" ; shift 2 ;;                        esac ;;                --) shift ; break ;;                *) echo "Internal error!" ; exit 1 ;;        esacdoneecho "Remaining arguments:"for arg do   echo '--> '"\`$arg'" ;done|
|----|


运行命令：./getopt.sh --b-long abc -a -c33 remain
Option b, argument `abc'
Option a
Option c, argument `33'
Remaining arguments:
--> `remain'


参考资料：
http://www.cnblogs.com/FrankTan/archive/2010/03/01/1634516.html
http://www.orczhou.com/index.php/2010/04/linux-shell-getopt/
http://blog.csdn.net/flowingflying/archive/2010/01/03/5126066.aspx









