
# Clojure 学习入门（11）—— 宏 macro - 阳光岛主 - CSDN博客

2013年12月27日 23:59:51[阳光岛主](https://me.csdn.net/sunboy_2050)阅读数：10339所属专栏：[Clojure 学习总结](https://blog.csdn.net/column/details/learn-clojure.html)



clojure macro宏在运行之前机械展开，定义宏相当于给语言增加新特性，写宏的*原则*：
能写成函数就不要用宏（因为写宏没有写函数简单直观，容易写错，需要先在 REPL 中测试一番）
只有不得不用时才用宏（性能要求高时比函数调用快，或者需要“代码<->数据”相互转换）
精心设计的宏调用比函数调用更 DSL（如实现控制结构、传递 Java方法）

**宏与函数**

```python
;; 宏正确写法
(defmacro op [x f1 y f2 z]
  (list f2 z (list f1 x y)))      
(println (op 5 + 2 * 10))                   ;; 70
(println (macroexpand '(op 5 + 2 * 10)))                     ;; (* 10 (+ 5 2))
(println (macroexpand-1 '(op 5 + 2 * 10)))                   ;; (* 10 (+ 5 2))
(println (clojure.walk/macroexpand-all '(op 5 + 2 * 10)))    ;; (* 10 (+ 5 2))
;; 宏错误写法
(defmacro op2 [x f1 y f2 z]
  ( f2 z (f1 x y)))
(println (op2 5 + 2 * 10))                  ;; 2
(println (macroexpand '(op2 5 + 2 * 10)))                    ;; 2
(println (macroexpand-1 '(op2 5 + 2 * 10)))                  ;; 2
(println (clojure.walk/macroexpand-all '(op2 5 + 2 * 10)))   ;; 2
;; 不使用宏
(defn op3 [x f1 y f2 z]
  ( f2 z (f1 x y)))
(println (op3 5 + 2 * 10))                  ;; 70
(println (macroexpand '(op3 5 + 2 * 10)))                    ;; (op3 5 + 2 * 10)
(println (macroexpand-1 '(op3 5 + 2 * 10)))                  ;; (op3 5 + 2 * 10)
(println (clojure.walk/macroexpand-all '(op3 5 + 2 * 10)))   ;; (op3 5 + 2 * 10)
```
说明：
正确的宏写法，需要添加 list，宏用defmacro定义，不用宏写法的函数用defn定义
调试宏，用macroexpand展开

**宏符号**
|`
|原原本本地直译过去，不用`，let语句不被翻译，例如:|(let [datastr '{:a 1 :b 2}])
|
|~'
|后面的变量被直接翻译过去，例如：|(let [|~'conn|"meta"] (with-mongo|~'conn|))
|
|'~
|变量名本身而非值，例如：|(defn f1 [x] (println|'~x|":"|~x|))  (let [a 10] (f1  a)) ;; a:10
|
|~@
|表示多条语句
|
**示例1：**

```python
(defmacro debug [x] `(println "---" '~x ":" ~x))
(let [a 10] (debug a))             ;; --- a : 10
```
说明：
'~x  显示变量名，即a
~x 解析为变量值，即a的值 10

**示例2：**

```python
(defn make-connection [x] (println "in make-connection = " x) x)    ;; meta
(defn with-mongo [x] (println "in with-mongo = " x))   ;; meta
(defmacro with-dict
  "连接到 meta库的 dict表进行操作"
  [& body]
  `(let [~'dbname "meta"
         ~'tbname :dict
         ~'conn (make-connection ~'dbname)
         ]
     (with-mongo ~'conn)
     (println "~'conn = " ~'conn)   ;; meta
     (println "~'tbname = " ~'tbname)   ;; :dict
     (println "~@body = " ~@body)       ;; meta :dict db-test2' tbl-test2'
     ~@body))

(let [dbname 'db-test' 
      tbname 'tbl-test'
      dbname2 'db-test2' 
      tbname2 'tbl-test2'
      conn 'conn-sql'
      make-connection 'make-conn'
      body 'body1']
  (with-dict dbname tbname dbname2 tbname2))
```
运行结果：
in make-connection =  meta
in with-mongo =  meta
~'conn =  meta
~'tbname =  :dict
~@body =  meta :dict db-test2' tbl-test2'
说明：
make-connection 和 with-mongo 是定义的函数，后面传递的是参数，使用~'修饰直接翻译过去，即字符串传字符串，:dict 键值也传键值
with-dict 传递多个参数给body，其中 dbname tbname 在 with-dict 中被重新赋值，因此打印出的结果也为赋值后的最新结果




