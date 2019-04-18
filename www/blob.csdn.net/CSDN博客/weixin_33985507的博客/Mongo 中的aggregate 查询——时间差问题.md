# Mongo 中的aggregate 查询——时间差问题 - weixin_33985507的博客 - CSDN博客
2018年11月30日 18:09:00[weixin_33985507](https://me.csdn.net/weixin_33985507)阅读数：21
时间很宝贵。
一个需求是存储一个时间戳，前端展示距离当前时间的天数。需要在后端完成，前端只负责展示。
同上一篇文章，[Mongo 中的aggregate 查询——时间问题](https://www.jianshu.com/p/ff2a4ca85ac3)，在完成版本升级后，可用以下语句：
```
Aggregation.project("id", "isbn", "title", "outDate", "inDate", "status")
                            .andExpression("{$floor:{$divide:{{$subtract:{'$inDate',new java.util.Date()}},86400000}}}").as("time")
```
写到这忽然觉得 andExpression的句法和Excel中的函数有点像，函数名接括号里的参数。
其中$subtract{时间一，时间二}是求两个时间戳之间的差，new java.util.Date() 是执行到这里时取系统当前时间。这句尝试了特别多次，才最终发现这样时对的。在mongo中查询时new Date()可以直接起作用，在java里一定得带上包名。
其次，$divide是计算除法，将上一步中求差的结果，和86400000做除法，得到天数。86400000=24乘以60乘以60乘以1000（此处发现星号不显示。。）。
最后，$floor将除法的结果向下取整了。这个可以按需求四舍五入、向上取整都可以。
参考[Mongodb官方文档](https://docs.mongodb.com/manual/meta/aggregation-quick-reference/)。
