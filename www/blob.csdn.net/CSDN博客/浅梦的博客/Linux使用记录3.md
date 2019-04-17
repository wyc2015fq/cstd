# Linux使用记录3 - 浅梦的博客 - CSDN博客





2017年03月05日 15:04:41[浅梦s](https://me.csdn.net/u012151283)阅读数：212








# Bash Shell脚本

## 功能

![这里写图片描述](https://img-blog.csdn.net/20170305145035265?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvdTAxMjE1MTI4Mw==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)

1. Shell脚本介绍 
**shell**是一个提供用户窗口终端接口的命令行**解释器**，它也可以用来运行脚本。 

例如，在命令行中敲入以下命令`find . -name "*.c" -ls`和执行一个包含如下内容的脚本完成的功能是一样的。
```bash
#!/bin/bash
find . -name "*.c" -ls
```

第一行以`#!`开头的包含用来执行文件的命令行解释器的完整路径`/bin/bash`。解释器的选择取决于使用的脚本语言。 

2. Command Shell Choices 

常用的解释器包括 

- /usr/bin/perl 

- /bin/bash 

- /bin/csh 

- /usr/bin/python 

- /bin/sh 

Linux提供了很多shells,可用的shells存在`/etc/shells`文件中。大部分Linux用户使用默认的**bash**shell，对于一些UNIX用户可能会选择其他的shell。 

3. bash 脚本 

现在编写一个简单的**bash**脚本。有两种方式 

第一种
```bash
$ cat > exscript.sh
#!/bin/bash
echo "HELLO"
echo "WORLD"
```

按下回车和CTRL-D保存文件。 

第二种 

创建文件并用编辑器编写，最后使用命令`chmod +x exscript.sh`添加可执行权限。（`chmod +x`可以让所有用户拥有执行权限）。 

4. 使用bash脚本交互的例子 

创建一个能够提示用户输入变量，并存储打印该变量的值的脚本。通过在变量名前加`$`符号可以引用变量。
```bash
#!/bin/bash
# Interactive reading of variables
echo "ENTER YOUR NAME"
read sname
# Display of variable values
echo $sname
```

`#`开头的行是作为注释存在的。 

5. 返回值 

所有的shell脚本在完成执行后都会生成一个返回值。返回值可以通过`exit`来设置。返回值允许一个进程监视另一个进程的退出状态，在父子进程中经常使用。 

6. 查看返回值 

在文件前命令`ls`，返回值存储在`$?`环境变量中。应用程序通常会将返回值翻译成可理解的消息提供给用户。
## 语法
- 基本语法和特殊字符
|字符|描述|
|----|----|
|`#`|添加注释，除了用于脚本开始的`\#`或`#!`|
|`\`|用于行尾，将同一条指令分成多行|
|`;`|表示之后跟随的是一条新指令，同行分隔指令|
|`$`|表明之后跟随的是一个变量|

2. I/O重定向 
`>`和`>>`，`<`

3. 内置Shell指令 
![这里写图片描述](https://img-blog.csdn.net/20170305151953620?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvdTAxMjE1MTI4Mw==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)

4. 脚本参数|参数|含义|
|----|----|
|`$0`|脚本名|
|`$1`|第一个参数|
|`$2,$3,etc`|第二，三个参数|
|`$*`|所有参数|
|`$#`|参数个数|

5. 指令替代 

将一条指令的结果作为另一条指令的一部分，将内部指令用`()`或`$()`包括。例如： 
`$ cf /lib/modules/$(uname -r)/`

6. 环境变量 

输出`$ echo $PATH`

设置`$ MYCOLOR=blue`设置不需要`$`

通过`env`,`set`,`printenv`指令可以获取环境变量列表。 

7. 导出变量 

默认情况下，在脚本中创建的变量只能在该脚本的后续步骤中使用。任何子进程(sub-shells)都不能访问这些变量。要想让这些变量在子进程中使用，使用**export**表达式`export VAR=value`或 
`VAR=value;export VAR`

While child processes are allowed to modify the value of exported variables, the parent will not see any changes; exported variables are not shared, but only copied. 

8. 函数 

在脚本中使用函数需要两个步骤，1.声明函数2.调用函数 

声明语法：
```bash
function_name (){
    command...
}
```

使用`$1`，`$2`来获取函数参数。

## 结构
- 
if表达式 

压缩表示

```bash
if TEST-COMMANDS; then CONSEQUENT-COMMANDS;fi
```

更通用的定义

```bash
if condition
then
       statements
else
       statements
fi
```

- 
使用if表达式


```bash
if [ -f /etc/passwd ]
then
    echo "/etc/passwd exists."
fi
```

使用`[]`或`[[]]`来表示测试条件。 

3. 文件测试 
**bash**提供了一系列可用于if表达式的文件测试条件。|条件|含义|
|----|----|
|`-e file`|检查文件存在|
|`-d file`|检查文件是否为目录|
|`-f file`|检查是否是常规文件(不是链接文件，设备结点，目录)|
|`-s file`|检查文件大小是否非0|
|`-g file`|检查文件是否有`sgid`set|
|`-u file`|检查文件是否有`suid`set|
|`-r file`|检查文件是否可读|
|`-w file`|检查文件是否可写|
|`-x file`|检查文件是否可执行|

通过`man 1 test`命令查看完整的文件测试条件。 

4. 字符串测试 

使用`==`

5. 数字测试|操作符|含义|
|----|----|
|`-eq`|等于|
|`-ne`|不等于|
|`-gt`|大于|
|`-lt`|小于|
|`-ge`|大于等于|
|`-le`|小于等于|

对比语法`exp1 -op exp2`

6. 算术表达式 

- 使用**expr**工具，
```bash
expr 8 + 8
echo $(expr 8 + 8)
```

注意要有**空格**

- 使用`$((...))`语法，这是Shell内建的格式 
`echo $((8+8))`

- 使用内建命令`let`
`let x=(8+8);echo $x`

在现代shell脚本中，最好用`var=$((...))`替换**expr**
# 高级Bash脚本

## 字符串处理
|操作符|含义|
|----|----|
|[[string1 > string2]]|Compares the sorting order of string1 and string2.|
|[[string1 == string2]]|Compares the characters in string1 with the characters in string2.|
|myLen1=`${#string1}`|保存字符串长度|

提取字符串 
`${string:0:1}`从0到1 
`${string#*.}`提取所有.号后的字符
## 布尔表达式

`&&`,`||`,`!`

## Case语句

```bash
case expression in
   pattern1) execute commands;;
   pattern2) execute commands;;
   pattern3) execute commands;;
   pattern4) execute commands;;
   * )       execute some default commands or nothing ;;
esac
```

## 循环结构

```bash
for variable-name in list
do
    execute one iteration for each item in the
            list until the list is finished
done
```

```bash
while condition is true
do
    Commands for execution
    ----
done
```

```bash
until condition is false
do
    Commands for execution
    ----
done
```

## 脚本调试

在**bash**shell中，以调试模式运行脚本`bash -x ./script_file`

- It traces and prefixes each command with the + character. 

- It displays each command before executing it. 

- It can debug only selected parts of a script (if desired) with:
```bash
set -x    # turns on debugging
...
set +x    # turns off debugging
```

## 重定向错误到文件和屏幕

在UNIX/Linux中，所有运行的程序都会被分配一个打开文件流。
|文件流|描述|文件描述符|
|----|----|----|
|stdin|标准输入，默认为键盘/终端|0|
|stdout|标准输出，默认为屏幕|1|
|stderr|表准错误输出|2|

使用重定向可以将**stdout**和**stderr**的输出流保存到一个或多个不同的文件中供之后分析。

## 一些额外的有用的技术
- 创建临时文件和目录 

使用**touch**创建临时文件容易被别人访问到。 

使用**mktemp**创建随机文件。 
`XXXXXXXX`将被**mktemp**工具用随机字符替换来确保文件不会被轻易访问到。|命令|用途|
|----|----|
|`TEMP=$(mktemp /tmp/tempfile.XXXXXXXX)`|创建一个临时文件|
|`TEMPDIR=$(mktemp -d /tmp/tempdir.XXXXXXXX)`|创建临时目录|

2. 用/dev/null丢弃输出 
![这里写图片描述](https://img-blog.csdn.net/20170305171609927?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvdTAxMjE1MTI4Mw==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)

3. 随机数和数据 

使用`$RANDOM`生成随机数。
# 进程

## 进程和进程属性介绍
- 进程类型
|进程类型|描述|例子|
|----|----|----|
|交互进程Interactive Process||bash,fireforx,top|
|批处理进程Batch Process||updatedb|
|守护进程Daemons||httpd,xinetd,sshd|
|线程||gnome-terminal,firefox|
|内核线程||kswapd0,migration,ksoftirqd|

![这里写图片描述](https://img-blog.csdn.net/20170305172126040?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvdTAxMjE1MTI4Mw==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)

2. 进程安排和状态 
**running**正在执行，或等待分配（时间片）来运行。 
**sleep**等待事件发生 
**zombie**子进程完成，而父进程没有得知这个状态。它并不存在但是仍然会出现在系统进程列表里。 

3. 进程和线程ID 

PID1表示**init**进程。|ID类型|描述|
|----|----|
|PID|唯一进程ID号|
|PPID|父进程ID|
|TID|线程ID，对于单线程进程来说就是PID。多与多线程，每个线程共享相同PID，但是有不同的TID|

4. 用户和组ID 
![这里写图片描述](https://img-blog.csdn.net/20170305173157309?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvdTAxMjE1MTI4Mw==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)

5. 优先级 
![这里写图片描述](https://img-blog.csdn.net/20170305173327216?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvdTAxMjE1MTI4Mw==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)
## 列出进程
- ps指令(System V 风格) 
`ps`会列出当前shell中所有运行进程。 
`ps -u`列出特定用户的进程信息。 
`ps -ef`详细列出所有系统进程。 
`ps -eLf` 进一步将每一个线程列出在一行。- ps指令(BSD 风格) 
`ps aux`列出所有用户的所有进程 
`ps axo`列出想要看的指定属性，如stat,priority,pid,pcpu,comm等。- 进程树 
`pstree`列出进程树 
`kill -SIGKILL <pid>` or `kill -9 <pid>`终止进程。只能终止自己的进程，root才能终止其他用户的进程。- top命令 
`top`可以实时地监控进程，输入`q`退出。 
![这里写图片描述](https://img-blog.csdn.net/20170305191816527?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvdTAxMjE1MTI4Mw==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)

top命令各行内容 
- **top**命令的首行展示了一个系统当前情况的概览。 
- 系统启动时间
- 登陆用户数量
- 平均负载load average 

平均复杂决定了系统的繁忙程度。一个CPU的负载是1.00表示满载但是没有过载。如果超过1.00表示进程在竞争CPU时间。如果平均负载非常高，可能意味着系统有问题，such as a runaway process (a process in a non-responding state)。
- 第二行展示了进程总数，运行，睡眠，停止，僵尸进程数量。
- 第三行展示了CPU时间是如何分成用户(us)和内核(uy)的。 

The percentage of user jobs running at a lower priority (niceness - ni) is then listed. Idle mode (id) should be low if the load average is high, and vice versa. The percentage of jobs waiting (wa) for I/O is listed. Interrupts include the percentage of hardware (hi) vs. software interrupts (si). Steal time (st) is generally used with virtual machines, which has some of its idle CPU time taken for other uses.
- 第四行和第五行展示了内存使用情况，分为物理内存(RAM)和交换空间。一旦物理内存耗尽，系统会开始使用**swap**空间作为外部存储池，但是访问速度会比内存慢很多，会降低系统性能。如果系统经常开始使用交换区，可以增加更多交换区空间。但是，通常先考虑增大更多的物理内存。
- top命令输出的进程列表 

默认情况下，进程根据最高CPU使用量排序。其他信息包括： 
- Process Identification Number (PID)
- Process owner (USER)
- Priority (PR) and nice values (NI)
- Virtual (VIRT), physical (RES), and shared memory (SHR)
- Status (S)
- Percentage of CPU (%CPU) and memory (%MEM) used
- Execution time (TIME+)
- Command (COMMAND)
- top下的交互命令 
**top**运行情况下可以使用单个字母来改变行为。
|命令|输出|
|----|----|
|t|显示或隐藏统计信息(2,3行)|
|m|显示或隐藏内存信息(4,5行)|
|A|按最高资源消耗排序进程|
|r|更改指定进程优先级|
|k|终止指定进程|
|f|进入top配置界面|
|o|交互式选择一个新的排序|

## 进程指标和进程控制
- 平均负载 

可以通过`w`,`top`,`uptime`命令查看平均负载。
- 解释平均负载
- 后台和前台进程 

默认情况下进程在前台执行。在指令后面添加后缀`&`可以让指令在后台运行。 

使用`CTRL-Z`挂起一个前台进程或`CTRL-C`中止前台进程。使用`bg`和`fg`可以让进程在后台和前台运行。- 任务管理 
**jobs**工具可以列出当前终端下所有在后台运行的任务。显示任务ID,状态state,指令名。 
`jobs -l`提供相同的信息包括后台任务的PID。 

后台任务和终端窗口是联系的，所以如果log off,**jobs**工具家ing不会显示从该窗口启动的任务。
## 计划未来进程

使用`at`工具可以在指定时间执行指令。 
![这里写图片描述](https://img-blog.csdn.net/20170305202624328?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvdTAxMjE1MTI4Mw==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)

1. cron 
![这里写图片描述](https://img-blog.csdn.net/20170305202721731?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvdTAxMjE1MTI4Mw==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)

2. sleep 
![这里写图片描述](https://img-blog.csdn.net/20170305203311629?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvdTAxMjE1MTI4Mw==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)
# 通用应用




























