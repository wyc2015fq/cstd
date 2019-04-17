# TDB Can't open database at location /path/to/db as it is already locked by the process with PID - 博客堂 - CSDN博客





2019年03月27日 14:56:56[最小森林](https://me.csdn.net/u012052268)阅读数：49标签：[fuseki																[TDB																[PID																[locked																[database](https://so.csdn.net/so/search/s.do?q=database&t=blog)
个人分类：[知识图谱](https://blog.csdn.net/u012052268/article/category/8694645)





# 1现象

当运行apache-jena-fuseki-3.7.0的时候，命令窗报错：Can’t open database at location /path/to/db as it is already locked by the process with PID 1234 when trying to open a TDB database。

说我们的TDB数据库被 一个进程给占用了。我赶紧搜索怎么解决。

# 2解决办法：

## 2.1官网描述

搜到了 两个网址：[https://issues.apache.org/jira/browse/JENA-1136](https://issues.apache.org/jira/browse/JENA-1136)  和官网：[https://jena.apache.org/documentation/tdb/faqs.html#lock-exception](https://jena.apache.org/documentation/tdb/faqs.html#lock-exception)

很好，正好官网有类似的解决问题，我们来看一下官网解释：

> 
此异常是TDB自动多JVM使用防范的结果，如前所述我可以在多个应用程序之间共享TDB数据集吗？问题TDB数据库只能由单个JVM安全使用，否则可能会发生数据损坏。从1.1.0开始，TDB会尽可能自动执行此限制，如果您尝试访问从另一个JVM访问的数据库，您将收到此异常。


推荐解决办法：

> 
在极少数情况下，您可能会发现该过程完全不相关（这可能由于过时的锁定文件而发生，因为它们并不总是自动清除），在这种情况下，您可以尝试从数据库目录中手动删除tdb.lock文件。如果您确定其他进程未访问TDB数据库，请执行此操作，否则可能会发生数据损坏。


我按照这个办法 ，去jena的tdb数据文件夹中把tdb.lock文件 删除了，  但是再次运行 还是没用。 还是报错！

## 2.2 去找pid

既然直接删除没用， 那么就去找这个PID。

### 2.2.1 windows中找 进程id（PID）的方法为：

```
1、右键点击任务栏，打开任管管理器;

2、我们点击“进程”标签切换到进程选项卡下;

3、点击任务管理器菜单上的“查看按钮”，并选点击“选择列”;

4、在选择进程页列中，我们勾选“PID(进程标识符)”选项，并点击确定即可;

5、这时候我们看到PID已经显示在进程页列表里了。
```

### 2.2.2 我们发现

我们发现原来是  pid6844  原来是 notepad++ 把apache-jena-fuseki-3.7.0的 配置文件打开了，这样就锁定了。  所以我们把 NOTPAD++软件关闭； 或者把这个进程给杀死即可。](https://so.csdn.net/so/search/s.do?q=locked&t=blog)](https://so.csdn.net/so/search/s.do?q=PID&t=blog)](https://so.csdn.net/so/search/s.do?q=TDB&t=blog)](https://so.csdn.net/so/search/s.do?q=fuseki&t=blog)




