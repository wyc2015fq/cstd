# Shell 脚本浮点运算 - 文章 - 伯乐在线
本文作者： [伯乐在线](http://blog.jobbole.com) - [answer](http://www.jobbole.com/members/answer) 。未经作者许可，禁止转载！
欢迎加入伯乐在线 [专栏作者](http://blog.jobbole.com/99322)。
本文将介绍几种Linux下通过Shell脚本进行浮点数计算的方法。
## Why
Bash Shell本身不具备处理浮点计算的能力, 如`expr`命令只支持整数运算 :


```
#!/bin/bash
a=59
b=60
expr $a / $b
```
运行结果 :


```
$ ./cal.sh
0
$
```
## Plan A
使用`bc`进行处理。
代码 :


```
#!/bin/bash
a=59
b=60
echo "scale=4; $a / $b" | bc
```
运行结果 :


```
$ ./bc.sh
.9833
$
```
> 
`scale`表示结果的小数精度。
## Plan B
使用`awk`进行处理。
代码 :


```
#!/bin/bash
a=59
b=60
awk 'BEGIN{printf "%.2f\n",('$a'/'$b')}'
```
运行结果 :


```
$ ./awk.sh
0.98
$
```
## Compare
使用bc :
![bc](http://o9zpdspb3.bkt.clouddn.com/bc.png)
使用awk :
![awk](http://o9zpdspb3.bkt.clouddn.com/awk.png)
> 
可以看出使用`awk`的效率更高，特别是运算次数比较大时。
> 
**打赏支持我写出更多好文章，谢谢！**
[打赏作者](#rewardbox)
#### 打赏支持我写出更多好文章，谢谢！
任选一种支付方式
![](http://jbcdn2.b0.upaiyun.com/2016/05/a0fe2e7173dbb8e9958f604115b14e47.png)![](http://jbcdn2.b0.upaiyun.com/2016/05/a1c7df04a021538432631eedf6eed47e.jpg)
