# Java中关于HashMap的元素遍历的顺序问题 - 孤傲苍狼 - 博客园



# [孤傲苍狼](https://www.cnblogs.com/xdp-gacl/)

只为成功找方法，不为失败找借口！




## [Java中关于HashMap的元素遍历的顺序问题](https://www.cnblogs.com/xdp-gacl/p/3558625.html)



　　今天在使用如下的方式遍历HashMap里面的元素时

```
1 for (Entry<String, String> entry : hashMap.entrySet()) {
2     MessageFormat.format("{0}={1}",entry.getKey(),entry.getValue());
3 }
```

　　发现得到的元素不是按照之前加入HashMap的顺序输出的，这个问题我之前倒是没有注意过，后来上网查了一下原因，发现是：**HashMap散列图、Hashtable散列表是按“有利于随机查找的散列(hash)的顺序”。并非按输入顺序。遍历时只能全部输出，而没有顺序**。甚至可以rehash()重新散列，来获得更利于随机存取的内部顺序。
　　总之，遍历HashMap或Hashtable时不要求顺序输出，即与顺序无关。

```
1 Map<String, String> paramMap = new HashMap<String, String>();
```

　　可以用**java.util.LinkedHashMap**就是按加入时的顺序遍历了。

```
1 Map<String, String> paramMap = new LinkedHashMap <String, String>();
```

　　类似的还有 **java.util.LinkedHashSet**









