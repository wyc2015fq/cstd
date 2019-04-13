
# Clojure 学习入门（13）—— binding - 阳光岛主 - CSDN博客

2014年01月05日 10:21:20[阳光岛主](https://me.csdn.net/sunboy_2050)阅读数：10290所属专栏：[Clojure 学习总结](https://blog.csdn.net/column/details/learn-clojure.html)



Clojure里面是不支持变量的。Binding跟变量有点像，但是在被赋值之前是不允许改的，包括：全局binding, 线程本地(thread local)binding， 以及函数内的本地binding， 以及一个表达式内部的binding。
**def**这个special form 定义一个全局的 binding，并且你还可以给它一个”root value” ，这个root value在所有的线程里面都是可见的，除非你给它赋了一个线程本地的值.`def。`也可以用来改变一个已经存在的binding的root value —— 但是这是不被鼓励的，因为这会牺牲不可变数据所带来的好处。
函数的参数是只在这个函数内可见的本地binding。
`let`这个special form 创建局限于一个 当前form的bindings. 它的第一个参数是一个vector, 里面包含名字-表达式的对子。表达式的值会被解析然后赋给左边的名字。这些binding可以在这个vector后面的表达式里面使用。这些binding还可以被多次赋值以改变它们的值，let命令剩下的参数是一些利用这个binding来进行计算的一些表达式。注意：如果这些表达式里面有调用别的函数，那么这个函数是无法利用let创建的这个binding的。
宏**binding**跟`let 类似`, 但是它创建的本地binding会暂时地覆盖已经存在的全局binding. 这个binding可以在创建这个binding的form以及这个form里面调用的函数里面都能看到。但是一旦跳出了这个`binding`那么被覆盖的全局binding的值会回复到之前的状态。
从 Clojure 1.3开始, binding只能用在 动态变量(dynamic var)上面了. 下面的例子演示了怎么定一个dynamic var。另一个区别是`let`是串行的赋值的, 所以后面的binding可以用前面binding的值, 而`binding`是不行的.
要被用来定义成新的、本地线程的、用binding来定义的binding有它们自己的命名方式：她们以星号开始，以星号结束。在这篇文章里面你会看到：`*command-line-args*`,`*agent*`,`*err*`,`*flush-on-newline*`,`*in*`,`*load-tests*`,`*ns*`,`*out*`,`*print-length*`,`*print-level*`and`*stack-trace-depth*`.要使用这些binding的函数会被这些binding的值影响的。比如给*out*一个新的binding会改变println函数的输出终端。
下面的例子介绍了`def`,`let`和`binding`的用法：

```python
; clojure connect to mysql
; ithomer.net
; 2013.12.10
(ns myClojure.syntax.binding)
(def ^:dynamic v 1)         ; v is a global binding
 
(defn f1 []
  (println "f1: v =" v))    ; global binding of 1
 
(defn f2 []
  (println "f2: before let v =" v)       ; global binding
  (let [v 2]                             ; creates local binding v that shadows global one
    (println "f2: in let, v =" v)        ; local binding of 2
    (f1))                                ; of 1
  (println "f2: after let v =" v))       ; global binding of 1
 
(defn f3 []
  (println "f3: before binding v =" v)   ; global binding of 1
  (binding [v 3]                         ; same global binding with new, temporary value
    (println "f3: in binding, v =" v)    ; global binding of 3
    (f1))                                ; of 3
  (println "f3: after binding v =" v))   ; global binding of 1
 
(defn f4 []
 (def v 4)                               ; changes the value of the global binding
 (println "f4: after binding v =" v)     ; of 4
 (f1))                                   ; of 4
 
(f2)
(f3)
(f4)
(println "after calling f4, v =" v)      ; of 4
```

运行结果：

```python
f2: before let v = 1
f2: in let, v = 2
f1: v = 1
f2: after let v = 1
f3: before binding v = 1
f3: in binding, v = 3
f1: v = 3
f3: after binding v = 1
f4: after binding v = 4
f1: v = 4
after calling f4, v = 4
```

**参考推荐：**
[Clojure入门教程](http://xumingming.sinaapp.com/302/clojure-functional-programming-for-the-jvm-clojure-tutorial/)
[Clojure 基本语法](http://www.cnblogs.com/fxjwind/archive/2013/01/22/2871860.html)



