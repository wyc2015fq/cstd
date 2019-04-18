# 大数据学习笔记(十) -- linux脚本编程(二) - 苝花向暖丨楠枝向寒 - CSDN博客

2018年09月26日 15:22:08[苝花向暖丨楠枝向寒](https://me.csdn.net/weixin_40247263)阅读数：31标签：[IO流																[重定向																[变量](https://so.csdn.net/so/search/s.do?q=变量&t=blog)](https://so.csdn.net/so/search/s.do?q=重定向&t=blog)](https://so.csdn.net/so/search/s.do?q=IO流&t=blog)
个人分类：[大数据](https://blog.csdn.net/weixin_40247263/article/category/8073779)


**Table of Contents**

[IO流](#IO%E6%B5%81)

[重定向](#%E9%87%8D%E5%AE%9A%E5%90%91)

[输出重定向](#%E8%BE%93%E5%87%BA%E9%87%8D%E5%AE%9A%E5%90%91)

[输入重定向](#%E8%BE%93%E5%85%A5%E9%87%8D%E5%AE%9A%E5%90%91)

[变量](#%E5%8F%98%E9%87%8F)

# IO流

首先说明：我用的是xshell5连接的虚拟机实现在windows下操作linux操作系统。

一切的命令执行都设计 io 操作，我在命令行下输入一个命令，这个命令会传递给xshell5然后通过socket与linux系统交互，linux执行命令以后返回结果再给xshell5,最后展示给我们，上述所说的过程可以通过以下命令看到

```bash
[root@node001 pts]# echo $$
21502
[root@node001 pts]# ps -ef | grep 21502
root      21502  21498  0 11:29 pts/0    00:00:00 -bash
root      21966  21502  0 14:56 pts/0    00:00:00 ps -ef
root      21967  21502  0 14:56 pts/0    00:00:00 grep 21502
[root@node001 pts]# ps -ef | grep 21498
root      21498  16819  0 11:29 ?        00:00:06 sshd: root@pts/0 
root      21502  21498  0 11:29 pts/0    00:00:00 -bash
root      21969  21502  0 14:58 pts/0    00:00:00 grep 21498
[root@node001 pts]# cd /proc/$$
[root@node001 21502]# cd fd
[root@node001 fd]# ll
总用量 0
lrwx------. 1 root root 64 9月  22 14:33 0 -> /dev/pts/0
lrwx------. 1 root root 64 9月  22 14:33 1 -> /dev/pts/0
lrwx------. 1 root root 64 9月  22 14:33 2 -> /dev/pts/0
lrwx------. 1 root root 64 9月  22 14:33 255 -> /dev/pts/0
```

解释一下，我首先查看了当前bash的进程号，然后根据进程号查该进程，然后再去查询当前bash的父进程，发现该bash进程是通过 sshd: root@pts/0 创建出来的，sshd就是 xshell 与 linux的连接协议，它通过文件描述符 0 绑定了一个socket，我们再去 

etc/当前bash进程号目录/fd 文件夹下 可以看到当前bash 的输入流 0、输出流 1 和 错误输出流 2 。它们都指向了 /dev/pts/0 ，dev/pts 文件夹下面的文件名字类似于   0、1、2 这种的都代表 连接虚拟机的终端，我这里指向的是 0 ，代表的是当前我使用的这个xshell终端，我们这里可以再通过xshell打开一个终端

然后通过

```bash
[root@node001 dev]# echo "aaaaaa" > /dev/pts/1
```

在 新打开的终端中会受到内容

```bash
[root@node001 pts]# aaaaaa
```

这也就验证了 我上面所说的。

也就是说默认情况下我们的命令输入和输出都是 xshell。但是我们如果不想让该内容输出到xshell的屏幕上，我们可以对其输出流进行重定向。

# 重定向

## 输出重定向

我们 通过 >   将 ll的输出内容重定向到文件当中， 如果多次使用 > 将输出内容重定向到一个文件当中，并不会发生内容追加，而是覆盖。

```bash
[root@node001 ~]# ll 1> ll.txt
[root@node001 ~]# cat ll.txt 
总用量 134996
-rw-r--r--.  1 root  root    8816567 9月  22 06:43 apache-tomcat-7.0.61.tar.gz
-rw-r--r--.  1 root  root        141 9月  22 13:42 awk.txt
-rwxr-xr-x.  1 root  root        217 9月  22 13:43 command.awk
-rw-r--r--.  1 root  root        175 9月  21 22:37 grep
-rw-r--r--.  1 root  root         65 9月  21 23:48 grep.txt
-r--r--r--.  1 root  root     854072 9月  22 09:41 httpd-2.2.15-59.el6.centos.x86_64.rpm
-rw-r--r--.  1 root  root        157 9月  22 00:52 ifcfg-eth0
-rw-r--r--.  1 root  root        884 9月  22 00:09 inittab
-rw-r--r--.  1 root  root  126857158 9月  22 06:43 jdk-7u67-linux-x64.rpm
-rw-r--r--.  1 root  root          0 9月  22 15:24 ll.txt
-rw-r--r--.  1 root  root        963 9月  22 02:24 passwd
-rw-r--r--.  1 root  root       1796 9月  21 20:20 profile
drwxr-xr-x.  2 root  root       4096 9月  22 13:12 sh01
-rw-r--r--.  1 root  root         28 9月  22 00:09 sort.txt
drwxr-xr-x. 12 50469 users      4096 9月  22 07:28 tengine-2.1.0
-rw-r--r--.  1 root  root    1653240 9月  22 06:43 tengine-2.1.0.tar.gz
-rw-r--r--.  1 root  root         22 9月  21 20:25 zzh.txt
```

```bash
[root@node001 ~]# echo "hello world" 1> ll.txt 
[root@node001 ~]# cat ll.txt 
hello world
```

使用 >> 可以实现 内容追加

```bash
[root@node001 ~]# echo "hello world1" 1>> ll.txt 
[root@node001 ~]# cat ll.txt 
hello world
hello world1
```

 将输出流重定向同时将错误输出流重定向

```bash
[root@node001 ~]# ls / /error 1>ls.txt 2>error.txt
[root@node001 ~]# cat ls.txt 
/:
bin
boot
dev
etc
home
lib
lib64
lost+found
media
mnt
ooxx
opt
proc
root
sbin
selinux
srv
sys
tmp
usr
var
[root@node001 ~]# cat error.txt 
ls: cannot access /error: No such file or directory
[root@node001 ~]#
```

通过下面的方式可以将标准输出和错误输出整合

用 2>&1 的方式可以将错误输出重定向到标准输出。

注意   1.大于号 与 "&" 之间没有空格

          2.冲顶向的顺序是从左向右绑定，所以下面的顺序反过来不行

```bash
[root@node001 ~]# ls / /text 1>> all.text 2>&1
[root@node001 ~]# cat all.text 
ls: cannot access /text: No such file or directory
/:
bin
boot
dev
etc
home
lib
lib64
lost+found
media
mnt
ooxx
opt
proc
root
sbin
selinux
srv
sys
tmp
usr
var
```

下面是简写方式

```bash
[root@node001 ~]# ls / /text >& all.txt
[root@node001 ~]# cat all.txt 
ls: cannot access /text: No such file or directory
/:
bin
boot
dev
etc
home
lib
lib64
lost+found
media
mnt
ooxx
opt
proc
root
sbin
selinux
srv
sys
tmp
usr
var
```

或者下面这种

```bash
[root@node001 ~]# ls / /text &> all.txt
```

## 输入重定向
-   <<<   将没有换行的字符串 放入输入流中
-   <<     将多行字符放入输入流中，但最终只会读取第一行，且遇到 << 后面的字符串停止
-   <       将文本放入输入流中

将read的输入流重定向为 从现有的字符串获取 使用  <<<

```bash
[root@node001 ~]# read aaa 0<<<"hello"
[root@node001 ~]# echo $aaa
hello
```

一般read的输入流 遇到 回车时 结束接收，我们可以更改输入流停止接收的标识  使用 <<

```bash
[root@node001 ~]# read aaa 0<<Bye
> ZZH
> AHASDL
> SDAS
> F
> SDSA
> DSAW
> Bye
[root@node001 ~]# echo $aaa
ZZH
```

在文本中也可以使用重定向

```bash
[root@node001 sh01]# cat sh02.sh 
cat 0<<OOXX
SLKJFKLSJFSKLDLKJF
SKLDJFLKSDFLKJ
SKDJFLKDJSLAKDAS
DSADCASDQ
DSADQWWDSAF
DSADSADQW
DSADQW
OOXX

echo "ok...you know me"
[root@node001 sh01]# . sh02.sh 
SLKJFKLSJFSKLDLKJF
SKLDJFLKSDFLKJ
SKDJFLKDJSLAKDAS
DSADCASDQ
DSADQWWDSAF
DSADSADQW
DSADQW
ok...you know me
```

下面是与 百度简历socket连接，并将输入输出流重定向到 该socket同时 将文件描述符设置为8，然后就可以向百度发送请求，并获取返回来的数据。

exec :

简单来说 内存分为3个区域，堆、栈、代码段，原本代码段里存的是bash，但是当我们用 exec 去执行一个命令，比如说exec ls ，那么 exec 会用 ls 的代码段替换掉 bash的代码段，然后将 ls代码段 压入栈，cpu再去栈中读取时就会读取到 ls的代码段，然后执行命令，执行命令以后会有一个exit，由于这是bash已经不存在了，所以 会退出连接。

但是 exec 还可以用于 开启一个新的io流并绑定socket用法如下，这种是不会退出连接的。

```bash
[root@node001 ~]# exec 8<> /dev/tcp/www.baidu.com/80
[root@node001 ~]# cd /proc/$$/fd
[root@node001 fd]# ll
total 0
lrwx------. 1 root root 64 Sep 22 17:31 0 -> /dev/pts/3
lrwx------. 1 root root 64 Sep 22 17:39 1 -> /dev/pts/3
lrwx------. 1 root root 64 Sep 22 17:39 2 -> /dev/pts/3
lrwx------. 1 root root 64 Sep 22 17:39 255 -> /dev/pts/3
lrwx------. 1 root root 64 Sep 22 17:39 8 -> s
[root@node001 fd]# exec 8<> /dev/tcp/www.baidu.com/80
[root@node001 fd]# echo -e "GET / HTTP/1.0\n" 1>&8
[root@node001 fd]# cat 0<&8
```

# 变量

变量分为全局变量，本地变量

**全局变量  --  函数内声明，函数外可取到**

```bash
[root@node001 fd]# text(){
> a=99
> echo $a
> }
[root@node001 fd]# text
99
[root@node001 fd]# echo $a
99
```

**本地变量  --  函数内声明，函数外不可取到**

```bash
[root@node001 fd]# text1(){
> local b=99
> echo $b
> }
[root@node001 fd]# text1
99
[root@node001 fd]# echo $b

[root@node001 fd]#
```

观察下面的代码 

```bash
[root@node001 sh01]# cat sh03.sh 

echo $1
echo $2
echo $12

echo "-------"
echo $#
echo $*
echo $@
[root@node001 sh01]# source sh03.sh a b c 4 5 6 7 8 9 0 x y z
a
b
a2
-------
13
a b c 4 5 6 7 8 9 0 x y z
a b c 4 5 6 7 8 9 0 x y z
```

```bash
[root@node001 sh01]# ls /
bin   dev  home  lib64       media  ooxx  proc  sbin     srv  tmp  var
boot  etc  lib   lost+found  mnt    opt   root  selinux  sys  usr
[root@node001 sh01]# echo $?
0
[root@node001 sh01]# ls /error
ls: cannot access /error: No such file or directory
[root@node001 sh01]# echo $?
2
```

可发现
- $1 和 $2 代表位置参数，$12 被bash理解为 $1 然后拼接一个字符 '2' ，正确的 写法应为 ${12}
- $# 代表传递的参数的个数
- $*  和 $@ 代表 传递的所有参数
- $? 代表上一个命令的执行状态。 0 代表正常，其它代表错误
- 
**$*	所有的位置参数,被作为一个单词.	注意:"$*"必须被""引用.	$@	与$*同义,但是每个参数都是一个独立的""引用字串,这就意味着参数被完整地传递,	并没有被解释和扩展.这也意味着,每个参数列表中的每个参数都被当成一个独立的	单词.	注意:"$@"必须被引用.**

**$@ $* 只在被双引号包起来的时候才会有差异	双引号括起来的情况：	$*将所有的参数认为是一个字段	$@以IFS（默认为空格）来划分字段，如果空格在“”里面，不划分。采用LS的脚本运行./test 1 "2 3" 4   来发现差异	没有括起来的情况是$@和$*一样的，见到IFS就划分字段。还是采用LS的脚本运行./test 1 "2 3" 4   来发现差异**

**体现如下**

```bash
[root@node001 sh01]# cat sh05.sh 
# /bin/bash

echo $#
echo $@
echo $*

for i in "$*";do echo $i;done

for i in "$@";do
	echo $i
done

[root@node001 sh01]# ./sh05.sh a b c d e
5
a b c d e
a b c d e
a b c d e
a
b
c
d
e
```

**数组**

通过 (a b c) 的方式来声明一个数组，直接打印数组只会输出第一个元素，通过索引位来获取里面的值。* 和 @ 代表输出所有的值

```bash
[root@node001 ~]# array=(a b c)
[root@node001 ~]# echo $array 
a
[root@node001 ~]# echo ${array[1]} 
b
[root@node001 ~]# echo ${array[0]} 
a
[root@node001 ~]# echo ${array[*]} 
a b c
[root@node001 ~]# echo ${array[@]} 
a b c
```

**管道**

往常我们使用管道都是2个或多个命令的结合使用，前面的命令的输出会通过管道到达后一个命令的输入流。

其实管道会 同时开辟 2个或多个新bash进程 去执行管道左右的命令。这两个bash IO流是互通的，最终的结果再返回到父bash进程显示

所以下面的  a=22 是在新的bash进程进行复制的，当命令执行完毕回到原本的bash进程中，查看 变量a的值依旧是9

```bash
[root@node001 ~]# a=9
[root@node001 ~]# echo $a
9
[root@node001 ~]# a=22 | echo ok
ok
[root@node001 ~]# echo $a
9
```

**export**

将当前进程中变量导出以后子进程中就可以访问到该变量的值

注意：下面的实例中 以./文件名 的方式运行文件会 新开启一个bash子进程

```bash
[root@node001 sh01]# cat sh04.sh 
#! /bin/bash
echo $a
[root@node001 sh01]# ll
total 16
-rw-r--r--. 1 root root  40 Sep 22 13:12 sh01.sh
-rw-r--r--. 1 root root 132 Sep 22 17:14 sh02.sh
-rw-r--r--. 1 root root  68 Sep 22 18:13 sh03.sh
-rw-r--r--. 1 root root  21 Sep 22 19:56 sh04.sh
[root@node001 sh01]# chmod +x sh04.sh 
[root@node001 sh01]# ll
total 16
-rw-r--r--. 1 root root  40 Sep 22 13:12 sh01.sh
-rw-r--r--. 1 root root 132 Sep 22 17:14 sh02.sh
-rw-r--r--. 1 root root  68 Sep 22 18:13 sh03.sh
-rwxr-xr-x. 1 root root  21 Sep 22 19:56 sh04.sh
[root@node001 sh01]# ./sh04.sh 

[root@node001 sh01]# export a
[root@node001 sh01]# ./sh04.sh 
9
```

但是 在 子进程运行过程中对该变量做修改不会影响到 父进程中的变量值，在父进程中做修改也不会影响到子进程的变量值。

**测试一**

如下：

子进程打印被导出的变量值，然后在子进程中修改该变量值，在子进程结束之前打印父进程的变量值。

其中  ./sh04.sh & 代表后台运行该文本，如果不加& 在子进程执行期间 无法操作 父bash进程

```bash
[root@node001 sh01]# cat sh04.sh 
#! /bin/bash
echo $a
echo "----"
a=99
sleep 20
echo $a
[root@node001 sh01]# ./sh04.sh &
[1] 22241
[root@node001 sh01]# 1
----

[root@node001 sh01]# echo $a
1
[root@node001 sh01]# 99

[1]+  Done                    ./sh04.sh
[root@node001 sh01]# echo $a
1
```

**测试二**

如下：

子进程打印一次变量值，睡20s，在这期间父进程修改变量值且不到export ，20s过后子进程再次打印变量值

```bash
[root@node001 sh01]# cat sh04.sh 
#! /bin/bash
echo $a
echo "----"
sleep 20
echo $a
```

```bash
[root@node001 sh01]# a=6
[root@node001 sh01]# ./sh04.sh &
[1] 22260
[root@node001 sh01]# 6
----

[root@node001 sh01]# a=8
[root@node001 sh01]# 6

[1]+  Done                    ./sh04.sh
```

可以发现 ， 在子进程运行的过程中我们将 原本的 a=6 ，在父进程中 改为 a=8，对子进程没有影响

但是注意，此时如果再次执行一次该命令的话，会发现 子进程中的 a 变成了 8

```bash
[root@node001 sh01]# ./sh04.sh &
[1] 22262
[root@node001 sh01]# 8
----
8

[1]+  Done                    ./sh04.sh
```

说明：父进程中的 a指向了 6，当export以后，使子进程的a也指向了6，子进程运行过程中发现 a指向了6所以第一次输出6，在睡眠的20s过程中，我们将父进程指向的a改成了8，那么为什么20s过后子进程的值还是6呢? 有人说父进程将 a=6 改为 a=8 实际上是在内存中新开辟了一个空间存放8，然后使原本指向6的变量a，去指向8了，而子进程的a还是指向6，所以不受影响，但我不这么认为，因为根据我上面测试的结果，当我再次通过 ./sh04.sh 运行脚本时，第一次打印出来a的值是8不是6了，而在这期间我也并没有将变量a重新export。 

所以我个人的理解为 在脚本执行的一次过程当中，只有第一次见到变量时它才会从父进程寻找该变量的引用，而从这时开始直到运行结束，它不会再次去通过变量a的指针去寻找内存中所存的值。而是一直将它认为是原本的值，除非你在文本内部将变量值进行改变。

其实即使我上面使用 export 进行重新导出，子进程中变量a的值同样不会受到影响，这也就验证了我上面所说，一旦找准了，在运行过程中，不会再去根据指针重新寻找。

使用export的测试如下

```bash
[root@node001 sh01]# ./sh04.sh &
[1] 22264
[root@node001 sh01]# 8
----

[root@node001 sh01]# a=9
[root@node001 sh01]# export a
[root@node001 sh01]# echo $a
9
[root@node001 sh01]# 8

[1]+  Done                    ./sh04.sh
```

**$$ 的特殊性**

我们之前说过 管道 会开启新的进程去执行左右两边的命令，下面的程序案例说 应该会打印出子进程的 pid，但结果却是当前父进程的pid

```bash
[root@node001 sh01]# echo $$
22194
[root@node001 sh01]# echo $$ | more
22194
```

原因： $$ 的优先级 比管道的优先级更高，bash看到 $$ 就执行，执行以后再执行的管道。


