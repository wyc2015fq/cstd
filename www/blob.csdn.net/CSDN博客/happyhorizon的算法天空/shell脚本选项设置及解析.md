# shell脚本选项设置及解析 - happyhorizon的算法天空 - CSDN博客
2018年05月24日 11:21:54[lxy_Alex](https://me.csdn.net/happyhorizion)阅读数：2386
## shell脚本的选项
比较复杂的shell脚本通常都会支持多个选项及其输入. 通常形式为`-[a-z,A-Z] [filename/settings]`. 在控制台输入脚本名称时,还会显示出帮助信息.
### 显示帮助
可以定义一个Usage() {}函数作为脚本的帮助函数, 当帮助信息比较简单的时候,可以echo命令实现, 当参数比较多的时候,可以用cat工具配合EOF符号来定义大段的说明文字. 例如:
```
Usage() 
{ 
	cat <<EOF 
	bet <input> <output> [options]
	-d debug (do not delete temporary intermediate images) 
	EOF  
	exit 1 
} 
# 显示帮助 
[ "$1" = "" ] && Usage
```
这里bet命令的调用格式定义为:`bet <input> <output> [options]`,这也是一种常见的shell脚本参数格式. 当options比较多的时候,shell脚本没有python里面argpase这样的工具包, 应该如何解析这些参数设置呢? 这时候还是要具体问题具体分析. 实际上,shell脚本对参数的读取和解析也是非常灵活的.
### 简单的选项参数
如果参数只是类似于-a, -N这种简单的标识符. 可以用简单的if …then … fi 语句解析:
```
if [ $1 = -a ] ; then 
	 REG="-a" 
	 HOWLONG=5 
fi
```
### 复杂的选项参数
如果参数后面还有具体的配置文件名称或者选项,也就是说 -a, -N这类前缀后续还有具体的设置字符串, 例如参数说明如下:
```
-T <minutes>          Estimated job length in minutes, used to auto-set queue name
  -q <queuename>        Possible values for <queuename> are "verylong.q", "long.q" 
                        and "short.q". See below for details
                        Default is "long.q".
  -a <arch-name>        Architecture [e.g., darwin or lx24-amd64]
  -p <job-priority>     Lower priority [0:-1024] default = 0                 
  -M <email-address>    Who to email, default = `whoami`@fmrib.ox.ac.uk 
  -j <jid>              Place a hold on this task until job jid has completed
  -t <filename>         Specify a task file of commands to execute in parallel
  -N <jobname>          Specify jobname as it will appear on queue
  -R <RAM>              Max total RAM to use for job (integer in MB)
  -l <logdirname>       Where to output logfiles
  -m <mailoptions>      Change the SGE mail options, see qsub for details
  -z <output>           If <output> image or file already exists, do nothing and exit
  -F                    Use flags embedded in scripts to set SGE queuing options
  -s <pename>,<threads> Submit a multi-threaded task - requires a PE (<pename>) to be
                        configured for the requested queues.
                        <threads> specifies the number of threads to run
  -v                    Verbose mode.
```
那么可以采用这样的方式:
```
nargs=$# 
if [ $nargs -eq 0 ] ; then 
	 usage 
fi 
set -- `getopt T:q:a:p:M:j:t:z:N:R:Fvm:l:s: $*` 
result=$? 
if [ $result != 0 ] ; then 
	echo "What? Your arguments make no sense!" 
fi 
if [ $nargs -eq 0 ] || [ $result != 0 ] ; 
	then usage 
fi
```
上面脚本片段中, 最重要的是如下一段:
```
set -- `getopt T:q:a:p:M:j:t:z:N:R:Fvm:l:s: $*`
```
这一句很关键. 其中用到了两个命令行工具: getopt和set. getopt的调用方法是这样的:
```
getopt options optstring parameters
```
这里的optstring是`T:q:a:p:M:j:t:z:N:R:Fvm:l:s:`带值选项要在字母后加冒号（:）,可以将类似功能的前缀符号放在一起, 就像这句里的"Fvm" .如果参数中包含无效参数，那么会给出错误信息.
当getopt将参数格式化好了之后，由set替换掉命令行原始参数。例如:
```
#!/bin/bash
set -- `getopt a:b:c "$@"`
echo "param = $@"
while [ -n "$1" ]
do
    case "$1" in 
     -a) echo "found option a and param = $2" 
         shift ;;
     -b) echo "found option b and param = $2"
         shift ;;
     -c) echo "found option c, no param." ;;
     --) ;;
     *) echo "what's this?" 
        break ;;
    esac
    shift
done
```
保存文件名称为getopt_test. 得到的输出如下:
```
$ getopt_test -a test -b test -e
getopt: invalid option -- 'e'
param = -a test -b test --
found option a and param = test
found option b and param = test
```
最后, 附上shell脚本中特殊符号的含义
## Shell脚本中`$0、$?、$!、$$、$*、$#、$@`等的意义
$$
Shell本身的PID（ProcessID）
$!
Shell最后运行的后台Process的PID
$?
最后运行的命令的结束代码（返回值）
$-
使用Set命令设定的Flag一览
$*所有参数列表。如"$*“用「”」括起来的情况、以"$1 $2 … $n"的形式输出所有参数。
$@所有参数列表。如"$@“用「”」括起来的情况、以"$1" “$2" … "$n” 的形式输出所有参数。
$#
添加到Shell的参数个数
$0
Shell本身的文件名
$1～$n
添加到Shell的各参数值。$1是第1参数、$2是第2参数…。
## 延伸阅读:
getopt：命令行选项、参数处理: [https://blog.csdn.net/tdmyl/article/details/24714297](https://blog.csdn.net/tdmyl/article/details/24714297)
《Linux命令行与shell脚本编程大全》 第十三章 学习笔记: [https://blog.csdn.net/su1216/article/details/10019221](https://blog.csdn.net/su1216/article/details/10019221)
