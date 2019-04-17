# activeMQ数据消费了，但是在中间站却没有删除数据的原因 - zj360202的专栏 - CSDN博客





2014年10月16日 16:03:00[zj360202](https://me.csdn.net/zj360202)阅读数：2465








```java
connection.createSession(Boolean.FALSE, Session.AUTO_ACKNOWLEDGE); 第一个参数当改成TRUE后，就会出现消费了数据在MQ中间站却没有删除记录的情况
```




