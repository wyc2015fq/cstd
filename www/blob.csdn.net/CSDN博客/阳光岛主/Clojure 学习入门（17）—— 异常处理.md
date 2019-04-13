
# Clojure 学习入门（17）—— 异常处理 - 阳光岛主 - CSDN博客

2014年03月10日 22:44:32[阳光岛主](https://me.csdn.net/sunboy_2050)阅读数：9931个人分类：[Script																](https://blog.csdn.net/sunboy_2050/article/category/694817)
所属专栏：[Clojure 学习总结](https://blog.csdn.net/column/details/learn-clojure.html)



**异常处理**
Clojure代码里面抛出来的异常都是运行时异常。当然从Clojure代码里面调用的java代码还是可能抛出那种需要检查的异常的。
try - catch - finally 以及throw 提供了和java里面类似的功能。
try、catch、throw、finally：
user=> (try(throw(Exception."error")) (finally(println"final")))
final
Exception error  user/eval310 (NO_SOURCE_FILE:1)

user=> (try(/30) (catchException e (println e)))
\#<ArithmeticException java.lang.ArithmeticException: Divide by zero>
nil

**assert：**
它测试一个表达式， 如果这个表达式的值为false的话，它会抛出异常。user=> (asserttrue)
nil
user=> (assertfalse)
AssertionError Assert failed:falseuser/eval317 (NO_SOURCE_FILE:1)
user=> (assertnil)
AssertionError Assert failed: nil  user/eval319 (NO_SOURCE_FILE:1)
user=> (assert0)
nil
user=> (assert[123])
nil
user=> (assert"foo")
nil

**clojure 完整示例：**

```python
(ns helloclojure.myexception)
(defn exception_test1 []
  (try (throw (Exception. "--error--"))
    (finally (println "final"))
    ))
(defn exception_test2 []
  (try (/ 3 0) 
    (catch Exception e (println "error: " e))
    ))
(defn exception_test3 [cls]
  (try (Class/forName cls) true
    (catch ClassNotFoundException e false))
  )
;(exception_test1)
(exception_test2)   ; error:  #<ArithmeticException java.lang.ArithmeticException: Divide by zero>
;(exception_test3 helloclojure.myexception)

(assert true)        
;(assert false)
;(try (assert false)
;  (catch Exception e (println "error" e))
;  (finally (println 'finally)))
;(assert nil)
(assert [1 2 3])
(assert "foo")
(assert 0)
(print 'end)
```



