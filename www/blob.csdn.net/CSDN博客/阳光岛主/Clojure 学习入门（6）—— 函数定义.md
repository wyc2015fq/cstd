
# Clojure 学习入门（6）—— 函数定义 - 阳光岛主 - CSDN博客

2013年12月22日 21:50:18[阳光岛主](https://me.csdn.net/sunboy_2050)阅读数：8221所属专栏：[Clojure 学习总结](https://blog.csdn.net/column/details/learn-clojure.html)



**一、创建函数：**
fn： fn是一个宏，用于定义一个简单的函数，如下：
```python
user=> (fn [] "hello")  
#<user$eval375$fn__376 user$eval375$fn__376@eabd2f>  
user=> ((fn [] "hello"))  
"hello"  
user=> ((fn [x] x) "hello") ; 带参数  
"hello"  
user=> ((fn [x] (str "hello " x)) "ithomer")
"hello ithomer"
```
简短的函数可以使用\#()，%表示唯一的参数；%1、%2 ..依次表示第1、2、..个参数；%&表示所有参数，如下：
```python
user=> (#(/ % 3) 4)   ;结果为 4/3
4/3  
user=> (#(/ %2 %1) 3 4)   ;结果为 4/3 
4/3  
user=> (#(apply / %&) 3 5 7)   ;结果为3/5/7  
3/35
```
下面是几个定义函数的例子：
```python
user=> ((fn [x] (+ 1 x)) 3)  ;一个参数完成加1的功能  
4  
user=> (#(+ 1 %) 3)  ;使用#符号完成加1的功能  
4  
user=> ((fn [x y] (* x y)) 3 4)  ;两个参数，实现乘积的功能  
12  
user=> (#(* %1 %2) 3 4)  ;使用#符号完成两个参数乘积的功能  
12
```
defn： defn 宏用来定义一个函数。它的参数包括一个函数名字，一个可选的注释字符串，参数列表，然后一个方法体。而函数的返回值则是方法体里面最后一个表达式的值。所有的函数都会返回一个值， 只是有的返回的值是nil。
```python
user=> (defn f1 [] "hello ithomer")				;定义无参函数  
#'user/f1
user=> (f1)
"hello ithomer"
user=> (defn f2 [x] (format "hello %s" x))		;定义一个参数函数 
#'user/f2
user=> (f2 "ithomer")
"hello ithomer"
user=> (defn f3 [x y] (+ x y))					;定义两个参数相加的函数  
#'user/f3  
user=> (f3 2 4)  
6  
user=> (defn f4 "f4 function comment" [] (println "f4 function here"))		;带注释的函数  
#'user/f4  
user=> (f4)  
f4 function here
nil 
user=> (doc f4)			;通过doc查看函数注释信息  
-------------------------
user/f4
([])
  f4 function comment
nil
user=> (defn f5 ([] (str "no parameter"))  
				([name] (str "my name is " name)))		;定义重载的函数（无参数、一个参数）
#'user/f5  
user=> (f5)  				; 无参数
"no parameter"  
user=> (f5 "clojure")  		; 一个参数
"my name is clojure"  
user=> (defn f1 [& a] (str a))		;定义变参函数  
#'user/f1  
user=> (f1 1 2 3)  
"(1 2 3)"  
user=> (defn m [& arg] (str arg ", size=" (count arg)))		;定义变参函数  
#'user/m  
user=> (m 1 2 3 4 5)  
"(1 2 3 4 5), size=5"  
user=> (m "a" 1 2.3 -1)  
"(\"a\" 1 2.3 -1), size=4"  
user=> (defn exp [a f1 b f2 c] (f2 (f1 a b) c))				;函数作为参数  
#'user/exp  
user=> (exp 5 - 2 + 3)  
6  
user=> (defn f [a] (fn [b] (- a b)))						;函数作为返回值  
#'user/f  
user=> ((f 7) 4)  
3
```

defn-： defn-与defn功能一致，都是用于定义函数的，defn-定义的函数作用域是私有的，而defn定义的函数是公有的，如下：
```python
user=> (ns test1)					;ns的意思是切换到指定的命名空间，如果不存在，则新建该命名空间  
nil  
test1=> (defn- foo [] "hello ithomer")			;定义私有函数foo，返回字符串world  
#'test1/foo  
test1=> (defn bar [] (str "hello " (foo)))		;定义公有函数bar，并调用私有函数foo  
#'test1/bar  
test1=> (foo)			;当前命名空间内调用foo函数  
"hello ithomer" 
test1=> (bar)			;当前命名空间内调用bar函数  
"hello hello ithomer" 
test1=> (ns test2)		;切换到test2命名空间中  
nil  
test2=> (test1/bar)		;调用test1命名空间的bar函数，返回成功  
"hello hello ithomer"
test2=> (test1/foo)		;调用test1命名空间的foo函数，出现异常，提示test1的foo函数不是公开的  
java.lang.IllegalStateException: var: #'test1/foo is not public (NO_SOURCE_FILE:79)
```

组合函数comp： 形如((comp f1 f2 .. fn) arg1 arg2 .. argn)
就是对参数从右到左组合执行所有函数，可以转变为：(f1 (f2 (.. (fn arg1 arg2 .. argn))))
举例如下：
```python
user=> (defn f [x y] (- (* x y)));使用defn定义函数方式  
#user/f  
user=> (f 2 4)  
-8  
user=> (def fc (comp - *));使用comp定义组合函数方式  
#user/fc  
user=> (fc 2 4)  
-8
```
偏函数partial： 形如((partial  f  arg1 arg2 .. argn)  arga argb .. argz)
就是执行：(f  arg1 arg2 .. argn  arga argb .. argz)
注意：偏函数的第一个参数是一个函数，后面至少有1个其他参数
partial函数称为“偏函数”或者“部分完整函数”，因为它是不完整的，定义也用def而不是defn。
```python
user=> (defn f [n] (* n 10));正常函数  
#'user/f  
user=> (f 2)  
20  
user=> (def fp (partial * 10));偏函数  
#'user/fp  
user=> (fp 2)  
20
```
constantly函数： constantly函数接受一个参数x，并返回一个变参函数，该变参函数无论参数是什么，都返回这个x值。
```python
user=> (def consf (constantly "a"))  
#'user/consf  
user=> (consf 1 2 3)  
"a"  
user=> (consf "a")  
"a"  
user=> (consf [1 2 3])  
"a"
```
**二、函数调用**
->：宏-> 我们也称为 “thread” 宏，它本质上是调用一系列的函数，前一个函数的返回值作为后一个函数的参数，返回最后一次函数调用的值，比如下面两行代码的作用是一样的：
```python
user=> (.toUpperCase "a b c d")
"A B C D"
user=> (.replace (.toUpperCase "a b c d") "A" "X")
"X B C D"
user=> (.split (.replace (.toUpperCase "a b c d") "A" "X") " ")				; 数组
#<String[] [Ljava.lang.String;@41ab11b0>
user=> (first (.split (.replace (.toUpperCase "a b c d") "A" "X") " "))  
"X" 
user=> (-> "a b c d" .toUpperCase)
"A B C D"
user=> (-> "a b c d" .toUpperCase (.replace "A" "X"))
"X B C D"
user=> (-> "a b c d" .toUpperCase (.replace "A" "X") (.split " "))			; 数组
#<String[] [Ljava.lang.String;@58e41bc3>
user=> (-> "a b c d" .toUpperCase (.replace "A" "X") (.split " ") first)
"X"
```

这样调用的好处是更少的()，也更接近于scala的习惯。
```python
user=> (-> (/ 144 12) (/ 2 3) str keyword list) 
(:2)
user=> (list (keyword (str (/ (/ 144 12) 2 3)))) 
(:2)
user=> (-> (/ 144 12))
12
user=> (-> (/ 144 12) (/ 2 3))
2
user=> (-> (/ 144 12) (/ 2 3) str)
"2"
user=> (-> (/ 144 12) (/ 2 3) str keyword)
:2
user=> (-> (/ 144 12) (/ 2 3) str keyword list)
(:2)
user=> (/ (/ 144 12) 2 3)
2
user=> (str (/ (/ 144 12) 2 3))
"2"
user=> (keyword (str (/ (/ 144 12) 2 3)))
:2
user=> (list (keyword (str (/ (/ 144 12) 2 3)))) 
(:2)
```

上面两句结果一样。
->>： 后面的函数迭代使用之前的函数结果作为最后一个参数，返回最后一次函数调用的值，试看下面两个语句：
```python
user=> (-> 10 (/ 3))  	; 10/3  10作为/函数第一个参数  
10/3
user=> (->> 10 (/ 3)) 	; 3/10  10作为/函数最后一个参数  
3/10
```

eval： eval解析表达式数据结构（不是字符串），并返回结果。
```python
user=> (eval (str "(println 1)"))			;str函数返回字符串
"(println 1)"
user=> (read-string "(println 1)")			;而read-string函数用于从字符串中读取对象
(println 1)
user=> (eval (read-string "(println 1)"))  
1
nil
```
apply函数： apply把给定的集合里面的所有元素一次性地给指定的函数作为参数调用，然后返回这个函数的返回值。可以把apply看作是SQL里面的聚合函数，如下：
```python
user=> (apply + [1 2 3 4])  
10
```
**三、函数检查**
fn?： fn?用于检查给定的参数是否为函数，是返回true，否则返回false，如：
```python
user=> (fn? #("test"))  
true
user=> (fn? 1)  
false
user=> (fn? nil)
false
user=> (fn? +)
true
user=> (fn? mod)
true
user=> (fn? rem)
true
user=> (fn? =)
true
user=> (fn? with-precision)
java.lang.Exception: Can't take value of a macro: #'clojure.core/with-precision (NO_SOURCE_FILE:32)
user=> (fn? min)
true
user=> (fn? >=)
true
```


**参考推荐：**
[clojure 学习](http://dead-knight.iteye.com/category/269733)


