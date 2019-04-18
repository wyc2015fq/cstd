# shell脚本：如何记录计算时长以及如何保存日志文件 - happyhorizon的算法天空 - CSDN博客
2018年12月16日 01:10:04[lxy_Alex](https://me.csdn.net/happyhorizion)阅读数：338
python和matlab都有非常友好的记录时间的方式，且不说python的time，datetime工具包，matlab的tic，toc命令简单好记，都是程序时间很好的记录工具，可以帮助后续代码的持续优化和改善。但是shell脚本如何记录运行了多长时间呢？这要从shell脚本的`date`命令说起。
## date命令
date是shell的时间工具，其调用方式非常简单。
基本格式： date [选项] … [+格式]
在shell控制台上直接输入`date`得到的是：
```
2018年12月15日 星期六 23时46分04秒 CST
```
常见的date命令调用方式有：
```
echo `date +%s`
```
返回从1970年1月1日00:00:00以来所经过的秒数: 1544888892， 或者还可以这样调用：
```
echo `date +%Y/%D:%H:%M:%S`
2018/12/15/18:23:50:42
```
date还支持很多格式，具体可参考文后“date命令详解”
## shell脚本的基本运算
虽然大多数时候都会把复杂的计算放在python脚本或者matlab脚本中，但是shell脚本中偶尔免不要做一些运算。
### 代数运算
|运算符|说明|举例|
|----|----|----|
|=|赋值|a=$b 将把变量 b 的值赋给 a|
|+|加法|`expr $a + $b` 结果为 30。|
|-|减法|`expr $a - $b` 结果为 -10。|
|*|乘法|`expr $a \* $b` 结果为  200。|
|/|除法|`expr $b / $a` 结果为 2。|
|%|取余|`expr $b % $a` 结果为 0。|
除了expr符号以外，数学运算同样可以使用(()) 。例如：
```
((1+2))
(((2+3)*4))
```
但是要想取到运算结果，需要用$引用
```
a=$((1+2))
```
还可以用bc这个shell脚本工具，比如后面记录计算时长的例子。
## shell中记录运算时长
```
stime=`date %s`
sleep 60
etime=`date +%s`
s=`echo "scale=0; ($etime - $stime)%60" | bc`
m=`echo "scale=0; ($etime - $stime)/60%60" | bc`
h=`echo "scale=0; ($etime - $stime)/60/60" | bc`
```
## 保存日志文件
在做批量实验室，例如跑批量MRI的预处理，我们会写好shell脚本，然后启动脚本，等所有作业执行完再去看结果，但是这些执行时的信息如何保存下来到文件中呢？下面这个命令可以完成这个任务
```
sh batchjob.sh 2>&1 | tee mylog.log
```
其中sh [batchjob.sh](http://batchjob.sh)：表示要执行的shell脚步；0，1，2：在linux分别表示标准输入、标准输出和标准错误信息输出。
参考：
date命令详解：[https://www.cnblogs.com/kimbo/p/7102203.html](https://www.cnblogs.com/kimbo/p/7102203.html)
shell脚本数学运算很详细的博客： [https://www.jb51.net/article/134292.htm](https://www.jb51.net/article/134292.htm)
