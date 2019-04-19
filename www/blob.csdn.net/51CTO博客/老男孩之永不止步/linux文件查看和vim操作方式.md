# linux文件查看和vim操作方式-老男孩之永不止步-51CTO博客
![linux文件查看和vim操作方式](https://s1.51cto.com/images/blog/201904/04/21babc2d24b100463532ab3ea719e28f.jpg?x-oss-process=image/watermark,size_16,text_QDUxQ1RP5Y2a5a6i,color_FFFFFF,t_100,g_se,x_10,y_10,shadow_90,type_ZmFuZ3poZW5naGVpdGk=)
![linux文件查看和vim操作方式](https://s1.51cto.com/images/blog/201904/04/17854d50fcbb5916a6a3a58373885830.jpg?x-oss-process=image/watermark,size_16,text_QDUxQ1RP5Y2a5a6i,color_FFFFFF,t_100,g_se,x_10,y_10,shadow_90,type_ZmFuZ3poZW5naGVpdGk=)
vim的三种模式：
![linux文件查看和vim操作方式](https://s1.51cto.com/images/blog/201904/07/3d70a24c5d303ea3447f7a892c51af01.png?x-oss-process=image/watermark,size_16,text_QDUxQ1RP5Y2a5a6i,color_FFFFFF,t_100,g_se,x_10,y_10,shadow_90,type_ZmFuZ3poZW5naGVpdGk=)
普通模式
```
[root@GYJ ~]# vi 1.txt
gyjgyjgyj123456789
~                                                                                   
~                                                                                   
~                                                                                   
~                                                                                   
~
```
编辑模式(-- INSERT --  插入的意思）
```
[root@GYJ ~]# vi 1.txt
gyjgyjgyj123456789
~                                                                                   
~                                                                                   
~                                                                                   
~                                                                                                                                                                                                                                                                                                                                                                                                                                    
-- INSERT --
```
命令行模式(看最底行，可以输入命令）
```
[root@GYJ ~]# vi 1.txt
gyjgyjgyj123456789
~                                                                                   
~                                                                                   
~                                                                                   
~                                                                                   
~                                                                                                                                                                      
:
```
set nu
显示行号（命令模式下输入）
```
[root@GYJ ~]# vi 1.txt
      1 g
      2 y
      3 j
      4 g
      5 y
      6 j
      7 gyjgyj123456789
~                                                                                   
~                                                                                   
~                                                                                                                                                               
-- INSERT --
```
set nonu
取消行号
```
[root@GYJ ~]# vi 1.txt
g
y
j
g
y
j
gyjgyj123456789
~                                                                                   
~                                                                                                                                                                                                                                          
:set nonu
```
cat
查看文件
```
[root@GYJ ~]# ls
1  2  anaconda-ks.cfg
[root@GYJ ~]# cat 1
123
456
[root@GYJ ~]#
```
cat 文件  文件  >>文件
合并文件到另一个文件（这个文件可以不存在）
```
[root@GYJ ~]# ls
1  2  anaconda-ks.cfg
[root@GYJ ~]# cat 1
123
456
[root@GYJ ~]# cat 2
789
111
[root@GYJ ~]# cat 1 2 >> 3
[root@GYJ ~]# ls
1  2  3  anaconda-ks.cfg
[root@GYJ ~]# cat 3
123
456
789
111
[root@GYJ ~]# cat 1 2 >>3
[root@GYJ ~]# cat 3
123
456
789
111
123
456
789
111
[root@GYJ ~]#
```
cat -n 文件，-b不显示空行的行号
显示行号
```
[root@GYJ ~]# cat -n 3
     1  123
     2  456
     3  789
     4  111
     5  123
     6  456
     7  789
     8  111
[root@GYJ ~]# cat -b 3
     1  123
     2  456
     3  789
     4  111
     5  123
     6  456
     7  789
     8  111
[root@GYJ ~]#
```
cat >>文件<<EOF
追加编辑(编辑完最后一行开头输入大写EOF，回车退出）
```
[root@GYJ ~]# cat 3
123
456
789
111
123
456
789
111
[root@GYJ ~]# cat >> 3 << EOF
> gyj
> GGYYJJ
> EOF
[root@GYJ ~]# cat 3
123
456
789
111
123
456
789
111
gyj
GGYYJJ
[root@GYJ ~]#
```
echo
输入文本到屏幕
```
[root@GYJ ~]# echo 1 2 3
1 2 3
[root@GYJ ~]# echo 1/2/3
1/2/3
[root@GYJ ~]# echo {1..5}
1 2 3 4 5
[root@GYJ ~]# echo 1{1..5}
11 12 13 14 15
[root@GYJ ~]# echo 1{1..5}.6
11.6 12.6 13.6 14.6 15.6
[root@GYJ ~]#
```
echo >文件
清空文件
[```
root@GYJ ~]# cat 1
123
[root@GYJ ~]# echo > 1
[root@GYJ ~]# cat 1
[root@GYJ ~]# 
```
echo 追加内容>>文件
给文件追加内容
[```
root@GYJ ~]# cat 1
123
[root@GYJ ~]# echo      456 >>1
[root@GYJ ~]# cat 1
123
456
[root@GYJ ~]#
```
seq
生成竖行序列
```
[root@GYJ ~]# seq 1 5
1
2
3
4
5
[root@GYJ ~]# seq 1 1 5
1
2
3
4
5
[root@GYJ ~]# seq 1 2 5
1
3
5
[root@GYJ ~]# seq 1 3 5
1
4
[root@GYJ ~]# seq 1 4 5
1
5
[root@GYJ ~]# seq 1 5 5
1
[root@GYJ ~]#
```
seq -w  10
以数字的位数，排列竖行序列
```
[root@GYJ ~]# seq -w 10
01
02
03
04
05
06
07
08
09
10
[root@GYJ ~]# seq -w 5
1
2
3
4
5
[root@GYJ ~]# seq -w 100
001
002
003
004
005
...(太长省略）
099
100
```
more：分页显示内容
f：下一页
b：上一页
q：退出（查看到文件底部自动退出）
less：分页显示内容
f：下一页
b：上一页
q：Q 退出（查看到文件底部，不退出）
-N：显示行号
/要搜索的内容  搜索查找 n查找下一个  N 查找上一个
head：显示文档头部
-n：显示文档前几行
[root@oldboy-59 ~]# head test.txt -n1
#! /bin/bash
[root@oldboy-59 ~]# head -n1 test.txt
#! /bin/bash
tail：显示文档尾部
-n：显示文档后几行
-f：显示文档后几行（实时更新）
tail -n5 test.txt 
tail  -n1 test.txt -n2  以后面的优先
tailf = tail -f  查看实时的日志情况
别名 alias
系统默认给rm mv cp 定义了别名
alias 查看已定义的别名
alias rm='命令'
alias rm='echo command bny...'
坑：.bashrc 要注释里面的别名 rm
unalias 取消别名
设置网卡名称为别名eth0
alias eth0='vim /etc/sysconfig/network-scripts/ifcfg-eth0'
