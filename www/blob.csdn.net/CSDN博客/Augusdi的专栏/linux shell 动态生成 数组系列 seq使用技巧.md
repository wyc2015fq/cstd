
# linux shell 动态生成 数组系列 seq使用技巧 - Augusdi的专栏 - CSDN博客


2015年04月26日 00:01:42[Augusdi](https://me.csdn.net/Augusdi)阅读数：1757个人分类：[Shell																](https://blog.csdn.net/Augusdi/article/category/2575289)


﻿﻿
如果对linux shell 数组不是很熟悉的话，请看上一篇文章：[l](http://www.cnblogs.com/chengmo/archive/2010/09/30/1839632.html)**inux shell 数组建立及使用技巧**,这篇文章主要讲是动态生成数组系列。方法应该很多，我这里主要以一个求和计算的题目为例进行分析。
**题目：请用linux shell 写一段脚本，实现从1..1000中所有偶数的和值。**
**方法一：**
> 通过while 循环得到需要的结果：

> start=1;

> total=0;

> while [ $start -le 1000 ];do

> [[ $(($start%2)) == 0 ]]&&total=$(($total+$start));

> start=$(($start+1));

> done;

> echo $total;

> [chengmo@centos5 ~]$ start=1;total=0;while [ $start -le 1000 ];do    [[ $(($start%2)) == 0 ]]&&total=$(($total+$start));   start=$(($start+1));done;echo $total;

> 250500

> 以上运行结果是：249500,在linux shell 中，”;”作为命令行分隔符。如果大家对于$(()) 运算符号不是很理解，可以查看：
> linux shell 实现 四则运算（整数及浮点） 简单方法
> ，如果对于：[[]] [] 符号，可以参考另外一篇文章
> [linux shell 逻辑运算符、逻辑表达式详解](http://www.cnblogs.com/chengmo/archive/2010/10/01/1839942.html)
> 。

**方法二：**
> 通过 for 循环得到结果：

> start=0;

> total=0;

> for i in $(seq $start 2 1000); do

> total=$(($total+$i));

> done;

> echo $total;

> [chengmo@centos5 ~]$ start=0;total=0;for i in $(seq $start 2 1000); do    total=$(($total+$i));done;echo $total;

> 250500

> 上面语句已经代码方面明显优于方法一，而且性能方面表现也很好。下面比较就可以发现：


> 比较性能：

> [chengmo@centos5 ~]$ time (start=0;total=0;for i in $(seq $start 2 1000); do    total=$(($total+$i));done;echo $total;)              250500

> real    0m0.016s

> user    0m0.012s

> sys     0m0.003s

> [chengmo@centos5 ~]$ time (start=1;total=0;while [ $start -le 1000 ];do    [[ $(($start%2)) == 0 ]]&&total=$(($total+$start));   start=$(($start+1));done;echo $total;)

> 250500

> real    0m0.073s

> user    0m0.069s

> sys     0m0.004s

> 方法一耗时 是方法二的 6倍！

**seq 使用：**
> seq [OPTION]... LAST

> seq [OPTION]... FIRST LAST

> seq [OPTION]... FIRST INCREMENT LAST

> [chengmo@centos5 ~]$ seq 1000   ‘起始默认是 1，间隔默认也是1

> [chengmo@centos5 ~]$seq 2 1000  ‘间隔默认是1

> [chengmo@centos5 ~]$seq 1 3 10    '从1开始，到10 间隔为3 结果是：1 4 7 10

> 说明：默认间隔是“空格” 如果想换成其它的可以带参数：-s

> [chengmo@centos5 ~]$seq -s'\#' 1 3 10

> 1\#4\#7\#10

> 应用技巧：
生成连续数组系列：

> [chengmo@centos5 ~]$ a=($(seq  1 3 10))

> [chengmo@centos5 ~]$ echo ${a[1]}

> 4

> [chengmo@centos5 ~]$ echo ${a[@]}

> 1 4 7 10

生成连续相同字符：

> [chengmo@centos5 ~]$ seq -s '\#' 30 | sed -e 's/[0-9]*//g'

> \#\#\#\#\#\#\#\#\#\#\#\#\#\#\#\#\#\#\#\#\#\#\#\#\#\#\#\#\#

> 上面例子：通过加入间隔字符‘＃’后，替换掉数字，　生成连续相同字符’\#’，这个在以后书写中还是有不少帮助。


> [http://www.cnblogs.com/chengmo/archive/2010/09/30/1839668.html](http://www.cnblogs.com/chengmo/archive/2010/09/30/1839668.html)




