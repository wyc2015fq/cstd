# linux常用指令-整理（不断完善） - 博客堂 - CSDN博客





2017年08月23日 17:18:23[最小森林](https://me.csdn.net/u012052268)阅读数：397










- [常用指令](#常用指令)- [1操作文件的指令](#1操作文件的指令)
- [2文件夹指令](#2文件夹指令)
- [3打包压缩相关命令](#3打包压缩相关命令)
- [4关机重启机器](#4关机重启机器)
- [5Linux软件包管理](#5linux软件包管理)- [1离线](#51离线)
- [2在线](#52在线)
- [3 sh](#53-sh)

- [6vim使用](#6vim使用)
- [7用户及用户组管理](#7用户及用户组管理)
- [8更改文件的用户及用户组](#8更改文件的用户及用户组)
- [9文件权限管理](#9文件权限管理)
- [10Linux管道](#10linux管道)
- [11系统管理命令](#11系统管理命令)





# 常用指令

## 1操作文件的指令

```
touch  创建空文件
echo   创建带有内容的文件。
cat    查看文件内容
cp     拷贝
mv     移动或重命名
rm     删除文件
       -r  递归删除，可删除子目录及文件
       -f  强制删除
find   在文件系统中搜索某文件
wc     统计文本中行数、字数、字符数
grep   在文本文件中查找某个字符串
ln     创建链接文件
more、less   分页显示文本文件内容
head、tail   显示文件头、尾内容
ctrl+alt+F1  命令行全屏模式
```

## 2文件夹指令

```
cd     切换目录   
ls     显示文件或目录
       -l  列出文件详细信息l(list)
       -a  列出当前目录下所有文件及目录，包括隐藏的a(all)
mkdir  创建目录
       -p  创建目录，若无父目录，则创建p(parent) 
rmdir  删除空目录 
rm -r  删除该目录（包括里面的东西）
tree   树形结构显示目录，需要安装tree包
pwd    显示当前目录
```

## 3打包压缩相关命令

```
tar:     打包压缩
   -c    归档文件
   -x    解压缩文件
   -z    gzip压缩文件
   -j    bzip2压缩文件
   -v    显示压缩或解压缩过程 v(view)
   -f    使用档名
```

例：

```
tar -cvf /home/abc.tar /home/abc        只打包，不压缩
tar -zcvf /home/abc.tar.gz /home/abc    打包，并用gzip压缩！
tar -jcvf /home/abc.tar.bz2 /home/abc   打包，并用bzip2压缩
这三个命令：
/home/abc.tar指的是你压缩好之后的文件放在哪里；abc.tar就默认当前路径。
/home/abc 指的是 你要压缩的文件夹是哪个，注意我上次写home，结果把整个home给压缩了，所以一定要写好文件名，可以不写绝对路径，但是最后你想压缩的文件夹名abc一定要写上。 


当然，如果想解压缩，就直接替换上面的命令tar  -cvf  /   tar  -zcvf  /  tar  -jcvf 中的“c” 换成“x” 就可以了。

常见解压：
tar -zxvf /home/abc.tar.gz /home/abc
```

## 4关机/重启机器

```
shutdown
    -r      关机重启
    -h      关机不重启
    now     立刻关机
halt        关机
reboot      重启
```

## 5Linux软件包管理

### 5.1离线

dpkg (Debian Package)管理工具，软件包名以.deb后缀。这种方法适合系统不能联网的情况下。

比如安装tree命令的安装包，先将tree.deb传到Linux系统中。再使用如下命令安装。

```bash
sudo dpkg -i tree_1.5.3-1_i386.deb  安装软件
sudo dpkg -r tree             卸载软件
```

### 5.2在线

APT（Advanced Packaging Tool）高级软件工具。这种方法适合系统能够连接互联网的情况。

```
sudo apt-get install tree     安装tree
sudo apt-get remove tree      卸载tree （sudo apt-get remove --purge tree      干净的卸载tree）
sudo apt-get update           更新软件
sudo apt-get upgrade
```

### 5.3 .sh

有些时候，下载的包是.sh结尾的，直接bash安装。

这是因为.sh是shell的文件格式，他把一些复杂的shell安装命令过程封装，我们只需要运行即可。 

这类软件一半卸载的时候，直接把安装包删除，然后再把path删除即可。

## 6vim使用

vim三种模式：命令模式、插入模式、编辑模式。分别使用ESC或 i 或：来切换模式。 

命令模式下：

```
:q            退出
:q!           强制退出
:wq           保存并退出
:set number   显示行号
:set nonumber 隐藏行号
/apache       在文档中查找apache 按n跳到下一个，shift+n上一个
yyp           复制光标所在行，并粘贴
h(左移一个字符←)、j(下一行↓)、k(上一行↑)、l(右移一个字符→)
```

## 7用户及用户组管理

```
/etc/passwd    存储用户账号
/etc/group     存储组账号
/etc/shadow    存储用户账号的密码
/etc/gshadow   存储用户组账号的密码
useradd        添加用户名
userdel        删除用户名
adduser        添加用户名
groupadd       添加组名
groupdel       删除组名
passwd root    给root设置密码
su root
su – root
/etc/profile   系统环境变量
bash_profile   用户环境变量
.bashrc        用户环境变量
su user        切换用户，加载配置文件.bashrc
su – user      切换用户，加载配置文件/etc/profile ，加载bash_profile
```

## 8更改文件的用户及用户组
`sudo chown [-R] owner[:group] {File|Directory}`
例如：还以jdk-7u21-linux-i586.tar.gz为例。属于用户hadoop，组hadoop 

要想切换此文件所属的用户及组。可以使用命令。
`sudo chown root:root jdk-7u21-linux-i586.tar.gz`
更改anaconda目录的用户为xqt（原来是root）否则会导致安装不上，权限不够。

```bash
sudo chown -R xqt:xqt anaconda3/
```

## 9文件权限管理

三种基本权限

```
R   读  数值表示为4
W   写  数值表示为2
X   可执行 数值表示为1
所以权利全开是数字7
```

例如：jdk-7u21-linux-i586.tar.gz文件的权限为-rw-rw-r–

```
第一个字符“-”表示普通文件；这个位置还可能会出现“l”链接；“d”表示目录
第二三四个字符“rw-”表示当前所属用户的权限。     所以用数值表示为4+2=6
第五六七个字符“rw-”表示当前所属组的权限。        所以用数值表示为4+2=6
第八九十个字符“r–”表示其他用户权限。                 所以用数值表示为2
所以操作此文件的权限用数值表示为662?
```

例如：有一个文件filename，权限为“-rw-r—-x” ,将权限值改为”-rwxrw-r-x”，用数值表示为765

```bash
sudo chmod u+x g+w o+r filename
或者
sudo chmod 765 filename
```

## 10Linux管道

将一个命令的标准输出作为另一个命令的标准输入。也就是把几个命令组合起来使用，后一个命令除以前一个命令的结果。

例：grep -r “close” /home/* | more 在home目录下所有文件中查找，包括close的文件，并分页输出。

## 11系统管理命令

```
stat     显示指定文件的详细信息，比ls更详细
who      显示在线登陆用户
whoami   显示当前操作用户
hostname 显示主机名
uname    显示系统信息
top      动态显示当前耗费资源最多进程信息
ps       显示瞬间进程状态 ps -aux
du       查看目录大小 du -h /home带有单位显示目录信息
df       查看磁盘大小 df -h 带有单位显示磁盘信息
ifconfig 查看网络情况
ping     测试网络连通
netstat  显示网络状态信息
man      命令不会用了，找男人? 如：man ls
clear    清屏
alias    对命令重命名 如：alias showmeit=”ps -aux” ，另外解除使用unaliax showmeit
kill     杀死进程，可以先用ps 或 top命令查看进程的id，然后再用kill命令杀死进程。
```

学习中遇到什么再补充~



