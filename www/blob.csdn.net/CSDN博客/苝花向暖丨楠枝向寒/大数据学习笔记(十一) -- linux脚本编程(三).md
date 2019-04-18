# 大数据学习笔记(十一) -- linux脚本编程(三) - 苝花向暖丨楠枝向寒 - CSDN博客

2018年09月26日 22:45:18[苝花向暖丨楠枝向寒](https://me.csdn.net/weixin_40247263)阅读数：21标签：[for																[while																[du																[case																[if](https://so.csdn.net/so/search/s.do?q=if&t=blog)](https://so.csdn.net/so/search/s.do?q=case&t=blog)](https://so.csdn.net/so/search/s.do?q=du&t=blog)](https://so.csdn.net/so/search/s.do?q=while&t=blog)](https://so.csdn.net/so/search/s.do?q=for&t=blog)
个人分类：[大数据](https://blog.csdn.net/weixin_40247263/article/category/8073779)


**Table of Contents**

[扩展](#%E6%89%A9%E5%B1%95)

[练习题](#%E7%BB%83%E4%B9%A0%E9%A2%98)

[关键字](#%E5%85%B3%E9%94%AE%E5%AD%97)

[if](#if)

[while](#while)

[for](#for)

[练习题](#%E7%BB%83%E4%B9%A0%E9%A2%98)

# 扩展

**引号扩展**

![](https://img-blog.csdn.net/20180927094307889?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3dlaXhpbl80MDI0NzI2Mw==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)

双引号可以扩展参数，可以嵌套，单引号不可以

```bash
[root@node001 sh01]# echo "$a"
99
[root@node001 sh01]# echo '$a'
$a
```

**花括号扩展**

```bash
[root@node001 sh01]# cp /etc/profile  /etc/init.d/network  ./
```

可优化为如下形式 

```bash
[root@node001 sh01]# cp /etc/{profile,init.d/network} ./
```

**反引号扩展**

用反引号将一段程序括起来，其执行结果可以用来赋值给一个变量

```bash
[root@node001 sh01]# sxt=`echo "hello"`
[root@node001 sh01]# echo $sxt
hello
```

**小括号配合$ 扩展**

类似于反引号的功能

```bash
[root@node001 sh01]# sxt=$(echo $(echo "world"))
[root@node001 sh01]# echo $sxt
world
```

**逻辑与 和 逻辑或**

```bash
[root@node001 sh01]# ls / && echo ok
bin   dev  home  lib64       media  ooxx  proc  sbin     srv  tmp  var
boot  etc  lib   lost+found  mnt    opt   root  selinux  sys  usr
ok
[root@node001 sh01]# ls / || echo ok
bin   dev  home  lib64       media  ooxx  proc  sbin     srv  tmp  var
boot  etc  lib   lost+found  mnt    opt   root  selinux  sys  usr
```

**数学运算**

使用 let 对变量进行运算

```bash
[root@node001 sh01]# let c=$a+$b
[root@node001 sh01]# echo $c
2
```

下面使用两个小括号进行简化

```bash
[root@node001 sh01]# d=$((a+b))
[root@node001 sh01]# echo $d
2
[root@node001 sh01]# ((a++))
[root@node001 sh01]# echo $a
2
```

**test**

该命令可以判断很多内容

```bash
[root@node001 sh01]# test 3 -gt 2
[root@node001 sh01]# echo $?
0
[root@node001 sh01]# test 3 -gt 8
[root@node001 sh01]# echo $?
1
[root@node001 sh01]# test 3 -gt 8 && echo ok
[root@node001 sh01]# test 3 -lt 8 && echo ok
ok
```

简化版   --  [[  ]]  这种形式不推荐使用，移植性差，某些shell 不兼容

```bash
[root@node001 sh01]# [ 3 -lt 8 ] && echo ok
ok
[root@node001 sh01]# [[ 3 -lt 8 ]] && echo ok
ok
```

# 练习题

编写一个脚本，执行脚本时传入用户名作为参数。然后自动创建用户并设置密码。

```bash
[root@node001 sh01]# cat useradd.sh 
#! /bin/bash

[ ! $# -eq 1  ] && echo "args err" && exit 2
id $1 && echo "user exist" && exit 3
useradd $1 && echo $1 | passwd --stdin $1 >& /dev/null && echo "user add ok" && exit 0
echo "i don't know..." && exit 8
```

思路，判断是否只传递了一个参数，如果不是则提示错误信息，并停止执行返回状态码 2

id $1 是判断用户是否存在，用户存在该命令会返回状态码0，然后提示用户名已存在，停止执行并返回状态码 3

然后就是 添加用户，如果添加成功 就设置密码 ， 当我们输入 passwd 用户名 时 默认从交互命令行 接收用户输入的参数，使用 --stdin 可以使 passwd 命令从其输入流中接收参数。

       --stdin

              This option is used to indicate that passwd should  read  the  new

              password from standard input, which can be a pipe.

passwd还会导致屏幕输出结果，所以后面我们将 标准输出流 $1 重定向 到 /dev/null 这是一个废弃目录，可以理解为垃圾回收站。我们把不需要的内容放在这里不会占用空间。 然后再提示用户  用户添加成功 ，并退出 返回 状态码 0.

如果程序执行到了最后一行，就直接告诉用户我也不知道是什么问题。。并返回状态码 8

其实触发 最后一种情况的 或许是 非root用户下创建用户，然后提示权限不够。

# 关键字

## if

linux中的 if 跟 java 以及 python中的 if的区别就是 linux的if 语句块在最后结束的时候需要加上fi，而且java 的 if判断是 true或 false，python是 True 或 False 或 None，但是在linux中 判断是 0 或 其他，0就代表真 ， 其他就代表假。  而且linux中除了一切皆文件的说法，还有一切皆命令的说法，也就是一切的命令执行以后都会返回一个状态码。这个状态码不会 显示出来，需要我们通过 $? 去取值。但是如果写在 if 后面则会自动 判断返回的状态码是否为 0 。如下

```bash
[root@node001 sh01]# if ls / ;then echo ok;fi
bin   dev  home  lib64       media  ooxx  proc  sbin     srv  tmp  var
boot  etc  lib   lost+found  mnt    opt   root  selinux  sys  usr
ok
```

这里一定要改变 对return 传统的认知。 只有 return 0 ，if 才会认为它为true

```bash
[root@node001 sh01]# test(){
> return 3
> }
[root@node001 sh01]# if test ; then echo ok ;fi
[root@node001 sh01]# test(){ return 0; }
[root@node001 sh01]# if test ; then echo ok ;fi
ok
```

## while

while COMMANDS; do COMMANDS; done 

命令之间 以 分号隔开

```bash
[root@node001 /]# ll
total 94
dr-xr-xr-x.  2 root root        4096 Sep 22 12:08 bin
dr-xr-xr-x.  5 root root        1024 Sep 21 03:27 boot
drwxr-xr-x. 18 root root        3740 Sep 22 15:11 dev
drwxr-xr-x. 75 root root        4096 Sep 22 23:22 etc
drwxr-xr-x.  9 root root        4096 Sep 22 23:22 home
dr-xr-xr-x.  8 root root        4096 Sep 22 06:58 lib
dr-xr-xr-x. 10 root root       12288 Sep 22 12:08 lib64
drwx------.  2 root root       16384 Sep 21 03:25 lost+found
drwxr-xr-x.  2 root root        4096 Sep 23  2011 media
dr-xr-xr-x.  7 root root        4096 Mar 29  2017 mnt
drwxrwx---.  2 root groupshare  4096 Sep 22 05:17 ooxx
drwxr-xr-x.  3 root root        4096 Sep 22 07:33 opt
dr-xr-xr-x. 87 root root           0 Sep 21 06:00 proc
dr-xr-x---.  4 root root        4096 Sep 22 19:58 root
dr-xr-xr-x.  2 root root       12288 Sep 22 09:30 sbin
drwxr-xr-x.  7 root root           0 Sep 21 06:00 selinux
drwxr-xr-x.  2 root root        4096 Sep 23  2011 srv
drwxr-xr-x. 13 root root           0 Sep 21 06:00 sys
drwxrwxrwt.  3 root root        4096 Sep 22 23:22 tmp
drwxr-xr-x. 14 root root        4096 Sep 22 08:24 usr
drwxr-xr-x. 17 root root        4096 Sep 21 03:26 var
[root@node001 /]# cd ooxx/
[root@node001 ooxx]# ll
total 4
-rw-rw-r--. 1 zzh01 groupshare 22 Sep 22 05:17 zzh01.txt
[root@node001 ooxx]# while ls /ooxx;do echo "hello";rm -rf /ooxx ; done
zzh01.txt
hello
ls: cannot access /ooxx: No such file or directory
```

## for

**普通 for 循环**

```bash
[root@node001 /]# for ((i=1;i<5;i++));do echo $i ;done
1
2
3
4
```

**增强 for 循环 **

被双引号 括住的被认为是一个元素

```bash
[root@node001 /]# for i in zzh zzh1 zzh2 zzh3;do echo $i;done
zzh
zzh1
zzh2
zzh3
[root@node001 /]# for i in "zzh zzh1" zzh2 zzh3;do echo $i;done
zzh zzh1
zzh2
zzh3
```

**case**

语法 ： case .... esac  含义与其他语言的 switch ... case 类似

```bash
[root@node001 sh01]# cat sh06.sh 
#! /bin/bash
option=$1
case ${option} in
    -f) echo "param is -f"
       ;;
    -d) echo "param is -d"
       ;;
    *)
        echo "$0:usage:[-f ] | [ -d ]"
	exit 1
       ;;
esac
```

```bash
[root@node001 sh01]# ./sh06.sh -f
param is -f
[root@node001 sh01]# ./sh06.sh -d
param is -d
[root@node001 sh01]# ./sh06.sh 
./sh06.sh:usage:[-f ] | [ -d ]
[root@node001 sh01]# ./sh06.sh -a
./sh06.sh:usage:[-f ] | [ -d ]
```

总结 ： if 和 while 后面接的是命令 ， for 和 case 接的是的变量

# 练习题

编写一个脚本，传入一个目录作为参数，打印出目录下 占用空间最大的文件名

```bash
[root@node001 sh01]# cat sh07.sh 
#! /bin/bash
oldIFS=$IFS
IFS=$'\n'
flg=0
for i in ` du -a $1 | sort -nr`;do
	fileName=`echo $i | awk '{print $2}'`
	if [ -f $fileName ];then
		echo $fileName
		flg=1
		break
	fi

done

IFS=$oldIFS

if [ $flg -eq 0 ];then
	echo "not find" && return 2
else
	return 0
fi
```

解释：

        for 循环 首先会将 in 后面的 列表进行切割，默认是根据 空格、制表符、换行符 进行切割。

但是   ` du =a $1 | sort -nr ` 返回的内容 应只根据 换行符进行切割，所以我们应对IFS进行重新设置。

注意，设置的时候 是有固定格式的 ，不能只写 '\n' ，那样IFS 存的仅仅是一个字符串 必须在前面加上 $

其中 du -a 是列出 给定的目录下的所有文件所占用的空间大小以及文件名，其中总目录大小也会给出。

sort -nr 代表以数值的顺序倒序 排列，目的是将占用空间最大的放在最前面，从而减少for循环遍历的次数。

awk 用来取出 文件名部分, [ -f    ] 用来判断该文件是否存在，且是否真的是一个文件，如果是则返回true，说白了，主要目的就是判断是否是文件，从而淘汰目录。找到以后在控制台显示 文件名。

如果没有找到 则给出提示，注意在for循环使用完毕后，我们应及时将IFS改为原本的值。 

还有 如果我们是以  . 或 source 来执行脚本，一定要使用return ，不要使用 exit ，否则会直接退出登录。



