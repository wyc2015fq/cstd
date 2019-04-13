
# Clojure 学习入门（8）—— 连接mongodb - 阳光岛主 - CSDN博客

2013年12月22日 23:00:18[阳光岛主](https://me.csdn.net/sunboy_2050)阅读数：8848个人分类：[Script																](https://blog.csdn.net/sunboy_2050/article/category/694817)
所属专栏：[Clojure 学习总结](https://blog.csdn.net/column/details/learn-clojure.html)



clojure 连接 mongodb示例
project.clj 文件添加monger依赖：[com.novemberain/monger "1.5.0"]

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
                 ])
```

示例代码：

```python
; clojure connect to mysql
; ithomer.net
; 2013.12.10
(ns myClojure.mongo3
  (:use [monger.core :only [connect! connect set-db! get-db]])
  (:require [monger.core :as mg]
            [monger.collection :as mc]))
 
;; 函数
(defn -main[& m] 
  
;;  (connect!) ;; 默认连接库 { :host "localhost" :port 27017 }
  (connect! { :host "172.27.22.21" :port 27017})    ; 指定 host 和 port
  
  (set-db! (mg/get-db "homer-db"))
  ;; 清空collection
  (mc/remove "workflow")
  
  ;; 单条插入
  (mc/insert "workflow" {:name "blog"  :url "http://blog.ithomer.net" :copyright  "ithomer" :dtime "2009-07-31"}) 
  (mc/insert "workflow" {:name "forum" :url "http://blog.ithomer.net" :copyright  "ithomer" :dtime "2013-12-01" })
  ;; 批量插入
  (mc/insert-batch "workflow" [{:name "csdn"   :url "http://blog.csdn.net/ithomer"     :copyright  "ithomer" :dtime "2010-01-01" }
                               {:name "csdn2"  :url "http://blog.csdn.net/sunboy_2050" :copyright  "ithomer" :dtime "2012-06-30" }])
  
  ;; 全量查询
   (doseq [item (map list (mc/find-maps "workflow"))] 
        (println item))
   
  ;; 条件查询
   (println)   ;; 空一行
   (doseq [item (mc/find-one "workflow" { :name "blog" })] 
         (println item))
   (println (mc/find-one "workflow" { :name "blog" }))
   
  ;; 更新 
   (mc/update "workflow" {:name "csdn"} {:copyright "yanggang"} :upsert true)
   
  ;; 删除
   (mc/remove "workflow" { :name "csdn2" })
  
  ;; 全量查询删除后的结果
   (println)
   (doseq [item (map list (mc/find-maps "workflow"))] 
        (println item))
   
   ;; 关闭连接
  (monger.core/disconnect!)
)
; 执行函数
(-main)
```
运行结果：

```python
({:_id #<ObjectId 52a6a9e1e4b0e53f523a9ddb>, :copyright ithomer, :name blog, :dtime 2009-07-31, :url http://blog.ithomer.net})
({:_id #<ObjectId 52a6a9e1e4b0e53f523a9ddc>, :copyright ithomer, :name forum, :dtime 2013-12-01, :url http://blog.ithomer.net})
({:_id #<ObjectId 52a6a9e1e4b0e53f523a9ddd>, :copyright ithomer, :name csdn, :dtime 2010-01-01, :url http://blog.csdn.net/ithomer})
({:_id #<ObjectId 52a6a9e1e4b0e53f523a9dde>, :copyright ithomer, :name csdn2, :dtime 2012-06-30, :url http://blog.csdn.net/sunboy_2050})
#<Entry _id=52a6a9e1e4b0e53f523a9ddb>
#<Entry copyright=ithomer>
#<Entry name=blog>
#<Entry dtime=2009-07-31>
#<Entry url=http://blog.ithomer.net>
#<BasicDBObject { "_id" : { "$oid" : "52a6a9e1e4b0e53f523a9ddb"} , "copyright" : "ithomer" , "name" : "blog" , "dtime" : "2009-07-31" , "url" : "http://blog.ithomer.net"}>
({:_id #<ObjectId 52a6a9e1e4b0e53f523a9ddb>, :copyright ithomer, :name blog, :dtime 2009-07-31, :url http://blog.ithomer.net})
({:_id #<ObjectId 52a6a9e1e4b0e53f523a9ddc>, :copyright ithomer, :name forum, :dtime 2013-12-01, :url http://blog.ithomer.net})
({:_id #<ObjectId 52a6a9e1e4b0e53f523a9ddd>, :copyright yanggang})
```
mongo数据库内容：
![](https://img-blog.csdn.net/20131210134509359)


**参考推荐：**
[Monger, a Clojure client for MongoDB](http://clojuremongodb.info/articles/getting_started.html)
[clojure简单操作mongo](http://f.dataguru.cn/thread-92356-1-1.html)



