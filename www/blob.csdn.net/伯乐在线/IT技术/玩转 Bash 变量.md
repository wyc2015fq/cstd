# 玩转 Bash 变量 - 文章 - 伯乐在线
原文出处： [spacewander](https://segmentfault.com/a/1190000002539169)
PS : 注意本文讨论的是Bash，而不一定是/bin/sh所链接的那个shell。这里出现的所有代码片段，默认在顶上都添加了`#!/bin/bash`。
## 一门自带混淆的语言


```
while (( $# )); do
    case $1 in
        -a*)
            # Error checking
            [[ ${1#-a} ]] || { echo "bash: ${FUNCNAME[0]}: `$1': missing"
                "number specifier" 1>&2; return 1; }
            printf %d "${1#-a}" &> /dev/null || { echo "bash:"
                "${FUNCNAME[0]}: `$1': invalid number specifier" 1>&2
            return 1; }
            # Assign array of -aN elements
            [[ "$2" ]] && unset -v "$2" && eval $2=("${@:3:${1#-a}}") &&
                shift $((${1#-a} + 2)) || { echo "bash: ${FUNCNAME[0]}:"
                "`$1${2+ }$2': missing argument(s)" 1>&2; return 1; }
            ;;
        -v)
            # Assign single value
            [[ "$2" ]] && unset -v "$2" && eval $2="$3" &&
                shift 3 || { echo "bash: ${FUNCNAME[0]}: $1: missing"
                "argument(s)" 1>&2; return 1; }
            ;;
        *)
            echo "bash: ${FUNCNAME[0]}: $1: invalid option" 1>&2
            return 1 ;;
    esac
done
```
如果你觉得阅读上面的Bash代码，就像阅读段子一样顺畅，那么是时候关掉这个页面，去做别的更有意义的行为，比如去喝个水什么的。
如果你觉得上面的Bash代码犹如鬼画符，并且实际生活中不得不面对它，那么就看下去吧。
# Bash变量操作
正式开始正文内容。
想要在一篇文章里，讲述要看懂开篇代码所需的所有知识点，这是不自量力的行为。因此，本文将讲且仅讲Bash中操作变量的方法。所以，即使你看完了这篇文章，你多半还是看不懂开篇的代码。
不过看完这篇文章之后，你对Bash的变量操作会有更为深入的认识。而且更重要的是，Bash之于你，不再是怎么也看不清摸不透。下一次要写脚本的时候，你也将更加坚定地下定决心 —— 人生苦短，我用Python/Ruby。
## 严格意义上的Bash变量类型
Bash变量只有两种类型，字符串和数组。不过从严格意义上，Bash没有变量类型。Bash中的变量，在运行的时候会被展开成其对应的值（字符串）。你可以把它看做C/C++中的宏定义，或者一些模板语言中的占位符。
一般情况下，变量通过`=`赋值，注意`=`两边不要留空格。有些好孩子，已经养成了符号两端留空格的习惯，结果当开始写Bash的时候，他们抓狂了。
要想访问变量，只需在变量名前面添加`$`，解释器就会对它进行展开。如果该变量并不存在，解释器会把它展开成“”。


```
me=spacewander
echo $me
echo $who
```
## 来自命令行的你
作为脚本语言，第一要义当然是要随时随地获取到命令行输入啦。
在Bash中，使用`$1`可以获取命令行输入的第一个参数，`$2`可以获取命令行输入的第2个参数，`$3`可以获取命令行输入的第……
你看，$1到$10000的用法就这么交代完了。Bash还是挺有逻辑的嘛。
顺便一提，`$0`获取的脚本的名字（其实就是其他语言中的第0个参数），`$@`获取所有的参数，`$#`获取参数的数目。记住`@`和`#`这两个符号，在Bash这一神秘的符文体系中，前者表示全部参数，后者表示参数的数目。
## 展开，然后Bomb！
假如Bash变量中含有空白字符，或者含有特殊字符，比如`*`，展开后会污染到外面的字符串，结果就是**Bomb**。
比如


```
Oops='*'
# '*'解释成所有匹配的文件名
echo $Oops
# 所以需要加双引号括起来
echo "$Oops"
# 加单引号会怎样呢？
echo '$Oops'
```
上面的代码值得一试。
另外一种Bomb的可能是，变量后面需要接其它字符串，比如`$FRUITs`。如果想让解释器识别成`$FRUIT`变量，而不是`$FRUITs`，需要用花括号括起来，像`${FRUIT}s`
## 数组和关联数组
Bash中可以使用两种容器。
一种是数组，另一种是关联数组，类似于其他语言中的Map/Hash/Dict。
声明数组的常用语法： `declare -a ARY`或者`ARY=(1 2 3)`
声明关联数组的唯一语法： `declare -A MAP`
赋值的语法：
直接`ARY[N]=VALUE`，N可以是数字索引也可以是键。关联数组可以使用`MAP=([x]=a [y]=b)`进行多项赋值，注意这是赋值的语句而不是声明。
亲测数组中的索引不一定要按顺序来，你可以先给2和3上的元素赋值。（同样算是弱类型的Javascript也支持这种无厘头赋值，这算通病么？）
往现有数组批量添加元素：


```
ARY+=(a b c)
MAP+=([a]=1 [b]=2)
```
取值：


```
${ARY[INDEX]}
${MAP[KEY]}
```
注意花括号的使用
`${A[@]}` 展开成所有的变量，而获取数组长度使用 `${#A[@]}`
切片：
`${ARY[@]:N:M}` N是offset而M是length
返回索引，相当于keys()：
`${!MAP[@]}`
试试下面的代码：


```
declare -a ARY
declare -A MAP
MAP+=([a]=1 [b]=2)
ARY+=(a b c)
echo ${ARY[1]}
echo ${MAP[a]}
echo "${ARY[@]}"
echo "${MAP[@]}"
echo "${ARY[@]:0:1}"
echo ${#ARY[@]}
echo "${!MAP[@]}"
ARY[4]=a
echo ${ARY[@]}
echo ${ARY[3]}
```
## 变量（字符串）变换
Bash中的变量变换，大体是${变量[操作符]}的形式
### 大小写变换


```
HI=HellO
echo "$HI" # HellO
echo ${HI^} # HellO
echo ${HI^^} # HELLO
echo ${HI,} # hellO
echo ${HI,,} # hello
echo ${HI~} # hellO
echo ${HI~~} #hELLo
```
`^`大写，`,`小写， `~`大小写切换
重复一次只修改首字母，重复两次则应用于所有字母。
混着用会怎样？
`echo ${HI^,^} # HellO`
看来是不行的×_×
### 移除匹配的字符串
`%xx` 从后往前，开始匹配，移除匹配的内容
`%%xx` 跟上面的差不多，不过这是贪婪匹配
`#xx` 从前往后，开始匹配，移除匹配的内容
`##xx` 跟上面的差不多，不过这是贪婪匹配
这个比较难理解，不过看下面几个例子应该能明白了。


```
FILENAME=/home/spacewander/param.sh
echo ${FILENAME%/*} # /home/spacewander
echo ${FILENAME%%/*} #
echo ${FILENAME#*/} # home/spacewander/param.sh
echo ${FILENAME##*/} # param.sh
```
### 查找并替换
`/MATCH/VALUE` 替换第一个匹配的内容。
`//MATCH/VALUE` 替换匹配的内容


```
echo ${FILENAME/home/office} # /office/spacewander/param.sh
echo ${FILENAME//s/S} # /home/Spacewander/param.Sh
```
## 其它字符串操作
获取变量（字符串）长度：`${#FILENAME}`
字符串切片：跟数组切片是同样的语法，`${STR:offset:len}`


```
TEXT=这个程序充满了BUG!
echo ${TEXT:0:8}
echo ${TEXT:4}
# 你还可以使用负数作为offset，这时候就是从后往前算起。注意负数要用括号括起来，避免冲突。
echo ${TEXT:(-4)}
```
## 关于变量，其它的内容
Bash中有一项特性，你可以方便地检查某个变量是否设置了，如果没有设置，就赋予一个默认值。尤其在处理环境变量的时候，这项特性会让你感到欣慰。
语法是`${VAR:=VALUE}`或者`${VAR:=VALUE}`。此外，还有一个相似的语法，`${VAR:=VALUE}`和`${VAR：=VALUE}`。
下面展示下两者的区别


```
# expand to default variable
echo ${NULL-"Not null"} # Not null
echo ${NULL} #
# set default variable
echo ${NIL="Not nil"} # Not nil
echo ${NIL} # Not nil
```
可以看出，前者只是当变量不存在时，展开成指定的值。而后者在变量不存在时，将变量的值设置为指定值。
最后介绍一个，当目标变量不存在时，指定报错信息的语法。


```
echo ${TARGET?Not Found} # 当$TARGET不存在时，显示TARGET: Not Found，并结束程序。
```
