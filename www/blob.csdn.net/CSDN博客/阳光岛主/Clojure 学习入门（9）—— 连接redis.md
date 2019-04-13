
# Clojure 学习入门（9）—— 连接redis - 阳光岛主 - CSDN博客

2013年12月22日 23:34:09[阳光岛主](https://me.csdn.net/sunboy_2050)阅读数：9175所属专栏：[Clojure 学习总结](https://blog.csdn.net/column/details/learn-clojure.html)



**clojure 连接 redis 示例**
project.cli 添加redis依赖：[clj-redis "0.0.12"]

```python
(defproject myClojure "0.1.0-SNAPSHOT"
  :description "FIXME: write description"
  :url "http://example.com/FIXME"
  :license {:name "Eclipse Public License"
            :url "http://www.eclipse.org/legal/epl-v10.html"}
  :dependencies [
                 [org.clojure/clojure "1.5.1"]
                 [org.clojure/java.jdbc "0.0.6"]
                 [org.clojure/clojure-contrib "1.2.0"]
                 [mysql/mysql-connector-java "5.1.26"]
                 
                 [com.novemberain/monger "1.5.0"]
                 
                 [com.taoensso/carmine "1.2.1"]
                 [clj-redis "0.0.12"]
                 ])
```

**示例代码：**

```python
(ns myClojure.redis2
  (require [clj-redis.client :as redis]))
;; 连接redis
(def db (redis/init))
(println (redis/ping db))
(println (redis/set db "foo" "BAR"))
(println (redis/get db "foo"))
(println)  ;; 空一行
(println (redis/set db "blog" "http://blog.ithomer.net"))
(println (redis/get db "blog"))
```

**运行结果：**
PONG
OK
BAR
OK
http://blog.ithomer.net

**redis 数据内容：**
![](https://img-blog.csdn.net/20131210141440109)


**Carmine 示例**
Carmine, a Clojure Redis client
project.clj 添加依赖：[com.taoensso/carmine "2.4.0"]

**示例代码：**

```python
(ns myClojure.redis
  (:require [taoensso.carmine :as car]
            [taoensso.carmine.message-queue :as carmine-mq])
  (:gen-class :main true))
(def pool         (car/make-conn-pool)) ; See docstring for additional options
(def spec-server1 (car/make-conn-spec)) ; ''
(defmacro wcar [& body] `(car/with-conn pool spec-server1 ~@body))
(defn -main [& args]
  (println "rd01 run!")
  ;; 
   (wcar (car/ping)
      (car/set "foo" "hello ithomer")
      (car/get "foo")) 
  )
(-main)
```

**运行结果：**
rd01 run!["PONG" "OK" "hello ithomer"]


**参考推荐：**
[Carmine, a Clojure Redis client](https://github.com/ptaoussanis/carmine)（github）
[clj-redis](https://github.com/mmcgrana/clj-redis)（github）
[clojure操作redis](http://f.dataguru.cn/thread-48035-1-1.html)



