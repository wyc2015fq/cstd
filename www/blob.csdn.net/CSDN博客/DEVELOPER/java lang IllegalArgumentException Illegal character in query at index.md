# java.lang.IllegalArgumentException: Illegal character in query at index - DEVELOPER - CSDN博客





2014年12月10日 14:33:30[学术袁](https://me.csdn.net/u012827205)阅读数：8939








                
今天进行app测试的时候，遇到了这个问题。ca，之前都没有注意，就是按照提供的接口堆起来，然后请求数据结果！今天测试的时候使用汉字+字母<English>进行拼接作为信息发送的时候，突然报错：
```java
java.lang.IllegalArgumentException: Illegal character in query at index
```


然后，我问了问技术总监，为什么出现了这个问题！我都是按照官方的拼接方式啊？！然后，他回了一句：需要URLEncode一下，在使用get方法向后台发送请求数据的时候都要进行encode；

比如：String url = "http://www.jxvdy.com/trend_add?token=xx&id=xx&content=inputContnt ";

我将整个url 都做了：url = URLEncoder.encode(url, "UTF-8"); 然后就进行测试，可是一个新的问题又出现了：

```java
Exception Target host must not be null, or set in parameters.
```
然后，我在网上查了查结果说，连接地址不完整，需要添加 " http:// "。出现了整个问题也就是在说，当我把整个url encode之后作为请求地址。但是在后台那边进行url解析的时候会得到一个经过encode的url，当然"  http://www.jxvdy.com/trend_add?  " 也会被encode，因此就出现了新的报错问题；

所以，我们只是需要把自己要发送的信息content进行encode， 然后把encode后的content拼接到 "  http://www.jxvdy.com/trend_add?token=xx&id=xx  "  就好！这样，在后台进行解析数据的时候，就能准确的分辨出那一段才是用户发送的Message，并返回正确的结果给请求用户。




