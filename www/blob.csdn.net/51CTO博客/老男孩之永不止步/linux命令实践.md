# linux命令实践-老男孩之永不止步-51CTO博客
7.pwd     显示当前所在目录
8.cd      切换目录/进入目录中
cd ~ =====  cd  进入当前用户家目录
cd ..  进入当前目录的上一级目录
cd .   进入当前目录
cd -   回到上一次目录
9.mkdir  创建目录，默认创建一层目录
make directory
-p  递归创建目录   
```
[root@oldboyedu-59 network-scripts]# mkdir /oldboy
[root@oldboyedu-59 network-scripts]# ls /oldboy/
[root@oldboyedu-59 network-scripts]# ls -l /oldboy/
total 0
```
```
[root@oldboyedu-59 ~]# mkdir /data/ole/li
mkdir: cannot create directory ‘/data/ole/li’: No such file or directory
[root@oldboyedu-59 ~]# mkdir -p /data/ole/li
```
10.ls    显示目录内容
list
参数    -l     显示详细信息，可以同时查看多个文件内容，输入具体路径
```
[root@oldboyedu-59 tmp]# ls
1.txt  1.txt.bak  kobe  kobe.bak
[root@oldboyedu-59 tmp]# ls -l kobe
total 0
[root@oldboyedu-59 tmp]# ls -l 1.txt
-rw-r--r-- 1 root root 0 Mar 28 17:18 1.txt
```
11.touch  创建文件和更改时间戳，可以递归创建文件，但是前面目录不存在会报错，需要前面的目录已经存在
```
[root@oldboyedu-59 tmp]# ls
[root@oldboyedu-59 tmp]# touch 1 2
[root@oldboyedu-59 tmp]# touch 1/2/3
touch: cannot touch ‘1/2/3’: Not a directory
```
```
[root@oldboyedu-59 tmp]# mkdir kobe
[root@oldboyedu-59 tmp]# touch kobe/24
[root@oldboyedu-59 tmp]# ls
1  2  kobe  mary
[root@oldboyedu-59 tmp]# cd kobe/
[root@oldboyedu-59 kobe]# ls
24
```
touch  更改时间戳
```
[root@oldboyedu-59 tmp]# ls -l
total 0
-rw-r--r-- 1 root root 0 Mar 28 17:17 1.txt
[root@oldboyedu-59 tmp]# touch 1.txt
[root@oldboyedu-59 tmp]# ls -l
total 0
-rw-r--r-- 1 root root 0 Mar 28 17:18 1.txt
```
12.echo  输出信息到屏幕，一般配合touch和mkdir创建文件或者目录使用
```
[root@oldboyedu-59 ~]# echo {1..10}
1 2 3 4 5 6 7 8 9 10
[root@oldboyedu-59 ~]# echo {a..z}
a b c d e f g h i j k l m n o p q r s t u v w x y z
```
```
[root@oldboyedu-59 ~]# echo oldboy{0..10}
oldboy0 oldboy1 oldboy2 oldboy3 oldboy4 oldboy5 oldboy6 oldboy7 oldboy8 oldboy9 oldboy10
[root@oldboyedu-59 ~]# echo oldboy{01..10}.txt
oldboy01.txt oldboy02.txt oldboy03.txt oldboy04.txt oldboy05.txt oldboy06.txt oldboy07.txt oldboy08.txt oldboy09.txt oldboy10.txt
```
```
[root@oldboyedu-59 tmp]# echo kobe {13..15}
kobe 13 14 15
```
```
[root@oldboyedu-59 ~]# touch {11..20}.txt
[root@oldboyedu-59 ~]# ls
11.txt  14.txt  17.txt  20.txt           hh
12.txt  15.txt  18.txt  aa               uu
13.txt  16.txt  19.txt  anaconda-ks.cfg
[root@oldboyedu-59 ~]# mkdir {01..10}
[root@oldboyedu-59 ~]# ls
01  05  09      13.txt  17.txt  aa
02  06  10      14.txt  18.txt  anaconda-ks.cfg
03  07  11.txt  15.txt  19.txt  hh
04  08  12.txt  16.txt  20.txt  uu
```
- cp  复制文件或者目录，默认无法复制目录
-r 递归复制，复制目录和目录的内容 
```
[root@oldboyedu-59 tmp]# cp kobe  /root
cp: omitting directory ‘kobe’-----忽略
[root@oldboyedu-59 tmp]# cp -r kobe /root
[root@oldboyedu-59 tmp]# cd /root
[root@oldboyedu-59 ~]# ls
01  05  09      12      14      16      18      20               c     hh
02  06  10      12.txt  14.txt  16.txt  18.txt  20.txt           d     kobe
```
cp的备份功能（文件和目录都可以）
```
[root@oldboyedu-59 ~]# cp /oldboy/oldboy.txt /oldboy/oldboy.txt.bak
[root@oldboyedu-59 ~]# cd /oldboy
[root@oldboyedu-59 oldboy]# ls 
alex  oldboy.txt  oldboy.txt.bak
```
```
[root@oldboyedu-59 tmp]# ls
1.txt  kobe
[root@oldboyedu-59 tmp]# cp kobe kobe.bak
cp: omitting directory ‘kobe’
[root@oldboyedu-59 tmp]# cp 1.txt 1.txt.bak
[root@oldboyedu-59 tmp]# ls
1.txt  1.txt.bak  kobe
[root@oldboyedu-59 tmp]# cp -r  kobe kobe.bak
[root@oldboyedu-59 tmp]# ls
1.txt  1.txt.bak  kobe  kobe.bak
```
