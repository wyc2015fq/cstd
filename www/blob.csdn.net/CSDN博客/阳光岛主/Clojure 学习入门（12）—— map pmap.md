
# Clojure 学习入门（12）—— map-pmap - 阳光岛主 - CSDN博客

2013年12月29日 23:23:30[阳光岛主](https://me.csdn.net/sunboy_2050)阅读数：9756所属专栏：[Clojure 学习总结](https://blog.csdn.net/column/details/learn-clojure.html)



clojure map数据结构的常用方法，小结一下

**map 各种排序**

```python
(println (sorted-map-by > 1 "a", 2 "b", 3 "c"))     ;; {3 c, 2 b, 1 a}
;; 相等 value 取一个
(let [results {:A 1 :B 2 :C 2 :D 5 :E 1 :F 1}]
  (println (into (sorted-map-by (fn [key1 key2]
                                  (compare (get results key2)
                                           (get results key1))))
                 results)))              ;;    {:D 5, :C 2, :A 1}
;; 全不等 value 
(let [results {:A 1 :B 2 :F 4 :E 3 :D 6 :C 5}]
  (println (into (sorted-map-by (fn [key1 key2]
                                  (compare (get results key2)
                                           (get results key1))))
                 results)))              ;;    {:D 6, :C 5, :F 4, :E 3, :B 2, :A 1}
;; value 升序
(let [results {:A 1 :B 2 :F 4 :E 3 :D 6 :C 5}]
  (println (into (sorted-map-by (fn [key1 key2]
                                  (compare [(get results key1) key1]
                                           [(get results key2) key2])))
                 results)))    ;;   {:A 1, :B 2, :E 3, :F 4, :C 5, :D 6}
;; value 降序
(let [results {:A 1 :B 2 :F 4 :E 3 :D 6 :C 5}]
  (println (into (sorted-map-by (fn [key1 key2]
                                  (compare [(get results key2) key2]
                                           [(get results key1) key1])))
                 results)))    ;;   {:D 6, :C 5, :F 4, :E 3, :B 2, :A 1}
;; value 升序
(let [results {:A 1 :B 2 :F 4 :E 3 :D 6 :C 5}]
  (println (into (sorted-map-by (fn [key1 key2]
                                  (compare [(get results :key1) key1]
                                           [(get results :key2) key2])))
                 results)))    ;;   {:A 1, :B 2, :C 5, :D 6, :E 3, :F 4}
;; value 降序
(let [results {:A 1 :B 2 :F 4 :E 3 :D 6 :C 5}]
  (println (into (sorted-map-by (fn [key1 key2]
                                  (compare [(get results :key2) key2]
                                           [(get results :key1) key1])))
                 results)))    ;;   {:F 4, :E 3, :D 6, :C 5, :B 2, :A 1}

;; 默认升序排列
(println (into (sorted-map) {:2013-12-23 0, :2013-12-22 0, :2013-12-21 0, :2013-12-20 3600, :2013-12-24 0, :2013-12-18 0, :2013-12-19 3600}))
;; {:2013-12-18 0, :2013-12-19 3600, :2013-12-20 3600, :2013-12-21 0, :2013-12-22 0, :2013-12-23 0, :2013-12-24 0}
;; 取排序后最末一个
(println (last (into (sorted-map) {:2013-12-23 0, :2013-12-22 0, :2013-12-21 0, :2013-12-20 3600, :2013-12-24 0, :2013-12-18 0, :2013-12-19 3600})))
;; [:2013-12-24 0]

;; 自定义key降序排序
(let [results {:2013-12-23 0, :2013-12-22 0, :2013-12-21 0, :2013-12-20 3600, :2013-12-24 0, :2013-12-18 0, :2013-12-19 3600}]
  (println (into (sorted-map-by (fn [key1 key2]
                                  (compare [(get results :key2) key2]
                                           [(get results :key1) key1])))
                 results)))    
;; {:2013-12-24 0, :2013-12-23 0, :2013-12-22 0, :2013-12-21 0, :2013-12-20 3600, :2013-12-19 3600, :2013-12-18 0}
```

**先排序后取值**

```python
;; 先对日期排序，然后取前num天
(defn take-sort-data [data num]
    (let [
          result-sorted  (into (sorted-map-by (fn [key1 key2]
                                                (compare [(get data :key2) key2]
                                                         [(get data :key1) key1]))) data)    ;; 对日期排序
          result (select-keys result-sorted (take num (keys result-sorted)))]
      (println "data = " data)
      (println "result-sorted = " result-sorted)
      (println "result = " result)
      (println "json/write-str result = " (json/write-str result)))
    )
(let [data {:2013-12-23 0, :2013-12-22 0, :2013-12-21 0, :2013-12-20 3600, :2013-12-24 0, :2013-12-18 0, :2013-12-19 3600}]
  (take-sort-data  data 6)
  (take-sort-data  data 4)
  )
;; (take-sort-data  data 6) 's result
;data =  {:2013-12-24 0, :2013-12-23 0, :2013-12-20 3600, :2013-12-21 0, :2013-12-22 0, :2013-12-18 0, :2013-12-19 3600}
;result-sorted =  {:2013-12-24 0, :2013-12-23 0, :2013-12-22 0, :2013-12-21 0, :2013-12-20 3600, :2013-12-19 3600, :2013-12-18 0}
;result =  {:2013-12-19 3600, :2013-12-20 3600, :2013-12-21 0, :2013-12-22 0, :2013-12-23 0, :2013-12-24 0}
;json/write-str result =  {"2013-12-19":3600,"2013-12-20":3600,"2013-12-21":0,"2013-12-22":0,"2013-12-23":0,"2013-12-24":0}
;
;; (take-sort-data  data 4) 's result
;data =  {:2013-12-24 0, :2013-12-23 0, :2013-12-20 3600, :2013-12-21 0, :2013-12-22 0, :2013-12-18 0, :2013-12-19 3600}
;result-sorted =  {:2013-12-24 0, :2013-12-23 0, :2013-12-22 0, :2013-12-21 0, :2013-12-20 3600, :2013-12-19 3600, :2013-12-18 0}
;result =  {:2013-12-21 0, :2013-12-22 0, :2013-12-23 0, :2013-12-24 0}
;json/write-str result =  {"2013-12-21":0,"2013-12-22":0,"2013-12-23":0,"2013-12-24":0}
```

**参考推荐：**
[sorted-map](http://clojuredocs.org/clojure_core/clojure.core/sorted-map)
[sorted-map-by](http://clojuredocs.org/clojure_core/clojure.core/sorted-map-by)
[compare](http://clojuredocs.org/clojure_core/clojure.core/compare)



