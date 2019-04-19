# 单机运行spark-shell出现ERROR Remoting: Remoting error: [Startup failed] - BlueSky - CSDN博客
2015年11月16日 22:43:39[ASIA_kobe](https://me.csdn.net/ASIA_kobe)阅读数：602标签：[spark](https://so.csdn.net/so/search/s.do?q=spark&t=blog)
个人分类：[Spark Learning](https://blog.csdn.net/ASIA_kobe/article/category/5912555)
从现象来看应该akka不能绑定到ip或者端口，于是google找到答案，[http://mail-archives.apache.org/mod_mbox/spark-user/201402.mbox/%3C9A13072E9AA64A9B846FACA846FCA7C8@gmail.com%3E](http://mail-archives.apache.org/mod_mbox/spark-user/201402.mbox/%3C9A13072E9AA64A9B846FACA846FCA7C8@gmail.com%3E)
只要将conf/spark-env.sh添加两个环境变量：
```bash
export SPARK_MASTER_IP=localhost
export SPARK_LOCAL_IP=localhost
```
