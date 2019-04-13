
# 报错：JPA-style positional param was not an integral ordinal; - lin_tuer的博客 - CSDN博客


2017年11月03日 15:26:09[lin_tuer](https://me.csdn.net/lin_tuer)阅读数：1995


使用 spring boot 的时候，数据库那里报了这个错误，查了很久都说是空格 格式之类的问题
我的源码是这样的


```java
@Query(value = "select * from organizer_msg_t where from_id=?1 and to_id=?2;",nativeQuery = true)
    public List<OrganizersMessage> findMsgByFromToId(Long from_id, Long to_id);
```

看了许久 也不知道哪里有问题，，结果，，，

在  sql 语句最后加了一个空格，，就好了，，真是，，，

```java
@Query(value = "select * from organizer_msg_t where from_id=?1 and to_id=?2 ;",nativeQuery = true)
    public List<OrganizersMessage> findMsgByFromToId(Long from_id, Long to_id);
```

就在 ?2 后面，，服气


