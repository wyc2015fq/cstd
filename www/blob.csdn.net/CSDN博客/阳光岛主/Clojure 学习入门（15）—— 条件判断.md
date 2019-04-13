
# Clojure 学习入门（15）—— 条件判断 - 阳光岛主 - CSDN博客

2014年01月08日 23:12:00[阳光岛主](https://me.csdn.net/sunboy_2050)阅读数：14504所属专栏：[Clojure 学习总结](https://blog.csdn.net/column/details/learn-clojure.html)



一、标准的流程控制
if：
将一个判断表达式作为它的第一个参数进行求值。如果求值为true，那么就返回它的第二个参数（相当于“then”子句）的求值结果。如果结果为false（包括nil）就返回第三个参数的求值结果（相当于“else”子句），前提是有提供第三个参数并且不为空。
user=> (defn is-small [number] (if(< number100)"yes""no"))
\#'user/is-small
user=> (is-small50)
"yes"
user=> (is-small110)
"no"

if条件中除了false和nil，其他都为true：user=> (iftrue"true")
"true"
user=> (if0"true")
"true"
user=> (if"""true")
"true"
user=> (ifnil"true")
nil
user=> (iffalse"true")
nil

if-not：
跟 if 的用法相同，但是作用是相反的。当逻辑为false的时候会去计算第二个参数的值，为true的时候才计算第三个参数的值user=> (if-not (zero?0)"no""yes")
"yes"
user=> (if(not (zero?0))"no""yes")
"yes"

if-let：
if-let宏接受两个参数，第一个参数为绑定变量，第二个参数为表达式。并根据第二个表达式参数返回的值确定执行then、else语句。user=> (defnif-let-test [arg] (if-let [x arg]"true""false"))
\#'user/if-let-test
user=> (if-let-test1)
"true"
user=> (if-let-test nil)
"false"
user=> (if-let-testfalse)
"false"

when:
when没有else子句，如果when后面第一个参数为true，则执行条件后的所有语句，否则返回nil。user=> (whenfalse(println"is true")"return true")
nil
user=> (whentrue(println"is true")"return true")
istrue
"return true"
user=> (def has-value (whentrue(println"hello world")"returned value"))
hello world
\#'user/has-value
user=> has-value
"returned value"

when-not：
when-not与when类似，只是第一个参数返回false，才执行后面所有语句，否则返回nil。user=> (when-notfalse(println"is true")"return true")
istrue
"return true"
user=> (when-nottrue(println"is true")"return true")
nil

when-let：
when-let与if-let类似，只有绑定变量值不是false、nil时，才执行后面所有语句，否则直接返回nil。user=> (when-let [atrue] (println"true")"return true")
true
"return true"
user=> (when-let [afalse] (println"true"))
nil
user=> (when-let [a nil] (println"true"))
nil

cond：
cond 可以有任意个“判断/表达式”对，作为它的参数。如果满足第一个判断，就执行第一个判断对应的表达式。如果没有满足第一个条件，就会尝试后面的判断表达式，以此类推。如果一个都没有满足，那么返回 nil 除非你用一个 :else 关键字放在最后来抓住剩下的所有可能性。cond类似于java中的switch..case..default语句，如：user=> (defn f [n] (cond (< n0)"<0"(< n10)"<10":else">=10"))
\#'user/f
user=> (f -2)
"<0"
user=> (f2)
"<10"
user=> (f10)
">=10"

case：
case可以简单理解为java中switch的case，如下user=> (let [mystr"hello"];首先绑定mystr的值为hello
(casemystr
""0
"hello"(count mystr)));case用于匹配mystr的值
5
user=> (let [mystr"no match"]
(casemystr
""0
"hello"(count mystr)
"default")) ;最后一个表达式只有匹配不成功时才执行
"default"

case可以用列表一次匹配多个值：user=> (defn f [x] (casex
(510)"*5"
(369)"*3"
"others"))
\#'user/f
user=> (f5)
"*5"
user=> (f10)
"*5"
user=> (f6)
"*3"
user=> (f1)
"others"

do：
do执行多条语句，返回最后一条语句值user=> (def v (do(println123) (println321) -1))
123
321
\#'user/v
user=> v
-1

loop、recur：
如果递归的层次太深的话，那么可能会产生内存不足的情况。所以一些编程语言利用 “tail call optimization” (TCO)的技术来解决这个问题。在Clojure里面避免这个问题的一个办法是使用loop 和recur。;定义递归语句完成10+9+……1=55
user=> (loop [sum0cnt10] (if(= cnt0) sum (recur (+ cnt sum) (dec cnt))))
55

loop/recur 组合把一个看似递归的调用变成一个迭代 — 迭代不需要占用栈空间。 loop special form 跟let special form 类似的地方是它们都会建立一个本地binding，但是同时它也建立一个递归点， 而这个递归点就是recur的参数里面的那个函数。loop给这些binding一个初始值。对recur 的调用使得程序的控制权返回给loop 并且给那些本地binding赋了新的值。给recur传递的参数一定要和loop所创建的binding的个数一样。同样recur只能出现在loop这个special form的最后一行


