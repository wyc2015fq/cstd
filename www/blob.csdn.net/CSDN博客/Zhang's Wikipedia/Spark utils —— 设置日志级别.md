
# Spark utils —— 设置日志级别 - Zhang's Wikipedia - CSDN博客


2019年01月22日 22:14:45[Inside_Zhang](https://me.csdn.net/lanchunhui)阅读数：115



## 1. 通过 SparkContext 指定日志级别
`val sc: SparkContext = new SparkContext(sparkConf)
sc.setLogLevel("WARN")
//sc.setLogLevel("DEBUG")
//sc.setLogLevel("ERROR")
//sc.setLogLevel("INFO")`

