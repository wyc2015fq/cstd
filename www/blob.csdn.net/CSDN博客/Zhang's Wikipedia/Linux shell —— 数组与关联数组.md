
# Linux shell —— 数组与关联数组 - Zhang's Wikipedia - CSDN博客


2018年05月06日 19:42:01[Inside_Zhang](https://me.csdn.net/lanchunhui)阅读数：104



## 1. 关联数组
使用declare -A（declare 的用法请使用 help 进行查看，help declare） 进行声明关联数组变量；
$ declare -A fruits_price
$ fruits_price=([apple]='$100' [orange]='$150')
列出关联数组的索引（也就是 key）：
$ echo ${!fruits_price[*]}
$ echo ${!fruits_price[@]}
## 2. 序列数组
seq 方法创建
基本用法：
$ a_num_seq = ($seq 5)
$ echo $a_num_seq
1 2 3 4 5
a_num_seq 得到是字符串，不同之处以空格分隔开。在linux 里面，可以把它看作是 list. 可以通过for…in 循环读取。
$ for i in $a_num_list; do echo $i; done;
1
2
3
4
5
生成 array，只需在$(seq 5)外再套一层()
$ a_num_seq = ($(seq 5))
$ echo $a_num_seq
1
$ echo ${\#a_num_seq[@]}
5		\# 得到其长度信息
使用 {begin…end}
注意 begin 和 and 之间是两个小数点，而不是三个；
$ echo {1..10}
1 2 3 4 5 6 7 8 9 10
$ for i in {1..5}; do echo $i; done;
1
2
3
4
5
性能比较
$ time echo {1..100000}real    0m18.758s
user    0m0.068s
sys     0m0.012s
$ time echo $(seq 100000)real    0m20.064s
user    0m0.068s
sys     0m0.012s


