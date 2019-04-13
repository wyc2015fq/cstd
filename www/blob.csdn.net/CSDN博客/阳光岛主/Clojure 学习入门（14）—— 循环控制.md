
# Clojure 学习入门（14）—— 循环控制 - 阳光岛主 - CSDN博客

2014年01月06日 23:04:21[阳光岛主](https://me.csdn.net/sunboy_2050)阅读数：13200所属专栏：[Clojure 学习总结](https://blog.csdn.net/column/details/learn-clojure.html)



Clojure 基于函数的流程控制
repeatedly 字面意思为重复函数。一般的用法如下：user=> (repeatedly5\#(rand-int11))
(68266)
重复产生5次随机数。rand-int 11表示0至11的随机数。
这里顺便提一下repeat函数：repeat函数接受一/两个常量参数，用法如下：user=> (repeat5(int(rand-int100)))
(3030303030)
当repeat、repeatedly函数只接受一个参数时（即没有重复次数的参数），需要配合take来中止，否则会产生内存溢出的错误。如下：user=> (repeatedly \#(rand-int11))
OutOfMemoryError Java heap space  java.util.Arrays.copyOf (Arrays.java:2882)
user=> (take5(repeatedly \#(rand-int11)))
(107028)

**iterate：**
迭代函数形式如下：
(iterate f v)
相当于：
while(true) { v = f(v) }
所以一般要配合(take n sequence)来中止：user=> (take10(iterate inc5)) ; (567891011121314)
user=> (take10(iterate \#(+ %5)5)) ; (5101520253035404550)
user=> (take10(iterate \#(* %2)2)) ; (2481632641282565121024)

基于序列的流程控制
dotimes 会执行给定的表达式一定次数, 一个本地binding会被给定值：从0到一个给定的数值。如下：user=> (dotimes [num3] (println"number:"(inc num)))
number:1
number:2
number:3
nil

doseq和for的语法是一样的，只不过for返回lazy seq而doseq接受任意数量的表达式, 以有副作用的方式执行它们, 并且返回niluser=> (doseq [i (range10)] (println i))
0
1
2
3
4
5
6
7
8
9
nil
user=> (doseq [i [123] j [1020]] (println i"-"j))
1-10
1-20
2-10
2-20
3-10
3-20
nil

for、doseq都支持遍历多个集合(最右边的最快)，同时还可以用:when 和 :while来过滤。user=> (for[x (range37)] (* x x))
(9162536)
user=> (for[x [012345] :let [y (* x3)] :when (even? y)] y)
(0612)
上面第二条语句可理解为：先定义x为集合，再使用let将y赋值为x*3，最后判断y是否为偶数，偶数才返回


