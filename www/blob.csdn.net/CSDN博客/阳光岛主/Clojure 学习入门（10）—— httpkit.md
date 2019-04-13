
# Clojure 学习入门（10）—— httpkit - 阳光岛主 - CSDN博客

2013年12月23日 00:02:05[阳光岛主](https://me.csdn.net/sunboy_2050)阅读数：11901所属专栏：[Clojure 学习总结](https://blog.csdn.net/column/details/learn-clojure.html)



**Http-kit**是主要由Java 和 Clojure开发，为Clojure定制的零依赖的HTTP lib，仅用3000多行代码，实现了高性能 Server，Client，Timer。代码量少，实现简单，对Hacker友好。
在普通的PC上进行性能测试时，http-kit server每秒能处理数万个请求，并能轻松保持数十万并发，每个连接消耗几k内存，并发数仅与内存大小有关，高并发不影响 latency。
http-kit 下载：[http-kit](https://clojars.org/http-kit)
http-kit 官网：[http-kit.org](http://http-kit.org/)

1）HTTP Server：
高性能， 在普通机器上，每秒处理数万个请求
高并发支持。采用异步IO + 线程池的方式，在普通机器上，数十万并发
支持HTTP长连和WebSocket
2）HTTP Client：
keep-alive：极端情况下，keep-alive可提高一倍性能
异步IO + 线程池
API 友好
2.0.0 版本修改纪录：
增加了高性能Timer
重新设计了client的API，支持同步／异步调用，两者API几乎一样，方便切换
Client实现了TCP链路复用（keep-alive)，对服务端应用来说，可提高不少性能
重新设计，统一了WebSocket和HTTP长连/Streaming 的API，可使对不支持WebSocket的客户端，轻松降级到HTTP长连。
WebSocket支持收发二进制帧
http-kit 力争做到API友好，性能强悍，实现简单，文档完善。

**示例1：**

```python
(ns myClojure.syntax.httpkit2
  (:use org.httpkit.server))
(defn app [req]
  {:status  200
   :headers {"Content-Type" "text/html"}
   :body    "hello ithomer.net"})
(run-server app {:ip "172.27.22.21" :port 8888 :thread 10})
(println "look at: http://172.27.22.21:8888")
```
控制台输出：
look at: http://172.27.22.21:8888\# 指定了IP地址后，访问 http://localhost:8888 将访问不到
访问地址：http://172.27.22.21:8888
![](https://img-blog.csdn.net/20131210210557953)

**示例 2：**
代码：

```python
; clojure connect to mysql
; ithomer.net
; 2013.12.10
(ns myClojure.syntax.httpkit3
  (:use [compojure.route :only [files not-found]]
        [compojure.handler :only [site]]
        [compojure.core :only [defroutes GET POST DELETE ANY context]]
        [org.httpkit.server])
  (:require [compojure.route :as route]))
(defn show-landing-page [req] ;; ordinary clojure function, accepts a request map, returns a response map
  ;; return landing page's html string. possible template library:
  ;; mustache (https://github.com/shenfeng/mustache.clj, https://github.com/fhd/clostache...)
  ;; enlive (https://github.com/cgrand/enlive)
  ;; hiccup(https://github.com/weavejester/hiccup)
    (println "<h1>Hello World</h1>")
  )
 
(defn update-userinfo [req]          ;; ordinary clojure function
  (let [user-id (-> req :params :id)    ; param from uri
        password (-> req :params :password)] ; form param
       (println user-id password)
    ))
(defn get-user-by-id [req]          ;; ordinary clojure function
  (let [user-id (-> req :params :id)    ; param from uri
        password (-> req :params :password)] ; form param
       (println user-id password)
    )
  )
(defroutes all-routes
 (GET "/" [] show-landing-page)
  (context "/user/:id" []
           (GET / [] get-user-by-id)
           (POST / [] update-userinfo))
  (route/not-found "<p>Page not found.</p>"))    ;; all other, return 404
(run-server (site #'all-routes) {:port 8888})
```
运行后，访问网址：[http://172.27.22.21:8888](http://172.27.22.21:8888/)
![](https://img-blog.csdn.net/20131210211043921)
控制台打印输出：
<h1>Hello World</h1>


**参考推荐：**
[http-kit server](http://http-kit.org/server.html)（官方）
[compojure](https://github.com/weavejester/compojure)（github）
[http-kit做高性能 HTTP 反向代理](http://www.oschina.net/code/snippet_583336_17931)



