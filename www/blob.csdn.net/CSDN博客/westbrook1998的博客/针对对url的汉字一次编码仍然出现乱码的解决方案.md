# 针对对url的汉字一次编码仍然出现乱码的解决方案 - westbrook1998的博客 - CSDN博客





2018年02月19日 19:15:49[westbrook1998](https://me.csdn.net/westbrook1998)阅读数：63标签：[编码																[url传参																[java](https://so.csdn.net/so/search/s.do?q=java&t=blog)
个人分类：[java](https://blog.csdn.net/westbrook1998/article/category/7459913)





## 解决方案就是编码两次。。。

```java
var newURl=encodeURIComponent(encodeURIComponent（url）)
```

这样java后台通过两次解码就能恢复中文了](https://so.csdn.net/so/search/s.do?q=url传参&t=blog)](https://so.csdn.net/so/search/s.do?q=编码&t=blog)




