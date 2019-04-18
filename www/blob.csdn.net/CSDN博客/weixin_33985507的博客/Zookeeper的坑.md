# Zookeeper的坑 - weixin_33985507的博客 - CSDN博客
2016年11月24日 17:08:00[weixin_33985507](https://me.csdn.net/weixin_33985507)阅读数：3
### Window启动Zookeeper报错java.lang.NumberFormatException
用`zkServer start`命令报如题的错误，改为直接用`zkServer`启动则ok
还有文件路径的设置也要特别小心了
```
dataDir=D:/zookeeper-3.3.6/data这样是可以的
dataDir=D:\\ookeeper-3.3.6\\data这样是可以的
dataDir=D:\ookeeper-3.3.6\data这样是不行的
```
