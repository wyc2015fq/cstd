# Linux Shell从入门到删除根目录跑路指南 - 文章 - 伯乐在线
原文出处： [xrzs](https://my.oschina.net/leejun2005/blog/793916 )
shell 作为一门 linux 下使用广泛的系统语言，语法简单，上手容易，但是想要用好，少犯错误，也不是那么容易的一件事，可谓虽是居家旅行之良药，但也是杀人灭口之利器~
今天就来聊聊 linux 下一个常见的问题：如何避免误删目录。下文会详细的讲述不同的场景下误删目录，以及相应的解决方案。
1、变量为空导致误删文件


```
base_path=/usr/sbin
tmp_file=`cmd_invalid`
# rm -rf $base_path/$tmp_file
```
这种情况下如果 cmd 执行出错或者返回为空，后果将是灾难性的，那如何防范呢？
（1）利用 shell 的变量扩展功能，如果变量为空赋给默认值或者抛出异常退出脚本：


```
echo ${base_path:?var is empty}/${tmp_file:?var is empty}
-bash: tmp_file: var is empty
```
（2）人肉判断变量是否为空：


```
[[ ${tmp_file} == "" ]] && echo 1
1
[[ -z ${tmp_file} ]] && echo 1       
1
```
（3）如果变量未定义还可以开启 set 选项：


```
# cat a.sh
set -u
b=
echo $b
echo $a
echo 1
# bash a.sh
a.sh: line 4: a: unbound variable
```
2、路径含有空格导致误删文件
史上最经典的要数下面这个bumblebee项目了，这个项目本来不出名，不过，程序在其安装脚本install.sh里的一个bug让这个项目一下子成了全世界最瞩目的项目。
![](http://jbcdn2.b0.upaiyun.com/2017/01/30b2e48168845489647c42d2ded7c686.png)
那我们该如何防范这种问题呢？
（1）良好的编程习惯：变量加引号防止扩展


```
path="/usr/local /sbin"
# rm -rf $path
rm -rf "$path"
```
（2）对变量进行语义检查
比如检测是否含有空格等特殊字符，不通用，不推荐这么做
3、目录或文件含有特殊字符导致误删文件
![](http://jbcdn2.b0.upaiyun.com/2017/01/6f136eac47a7580f757b3b2abb327159.png)
![](http://jbcdn2.b0.upaiyun.com/2017/01/28abb0349eeaef57c7141bd0d86bcecc.jpg)


```
ll
总用量 8
drwxrwxr-x 2 work work 4096 11月 24 18:57 '~'
-rw-rw-r-- 1 work work   34 11月 24 19:49 a.sh
# rm -rf ~
```
那我们该如何防范这种问题呢？
（1）良好的编程习惯：变量加引号防止扩展


```
rm -rf "~"
```
（2）如果不确定，删除之前 echo 或 find 一下，看变量被扩展成啥了


```
echo rm -rf "~"
rm -rf ~
echo rm -rf ~ 
rm -rf /home/work
```
4、cd 切换目录失败，导致文件被误删


```
cd ooxx_path_not_exsit
rm -rf *.exe
```
恭喜这种情况下你的当前目录下匹配文件都会被误删，那我们该如何防范这种问题呢？
（1）使用逻辑短路操作


```
cd path && rm -rf *.exe
```
（2）检测 path 是否存在


```
[[ -d ~ ]] && echo 1
1
```
5、终极解决方案
不要使用 root 操作系统资源，这样至少不会删除系统文件。
6、在登录 shell 下使用友好的提示符
友好的命令提示符能时刻提醒操作者当前在哪个路径下，避免错误的路径下操作文件。
![](http://jbcdn2.b0.upaiyun.com/2017/01/b971ccc4c9ac305e23015278ec593b1e.jpg)
![](http://jbcdn2.b0.upaiyun.com/2017/01/5ca9d0feb9465f1d32bc81c72d8eae22.jpg)
OK，本文到此就结束了，列举了一些常见的case和解决方案，希望能对大家有所启发，如果有其他的 case 和建议也欢迎交流~
