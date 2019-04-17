# Java String关于replaceall函数转义字符的一个小贴士 - 专注于数据挖掘算法研究和应用 - CSDN博客





2017年03月11日 17:25:19[fjssharpsword](https://me.csdn.net/fjssharpsword)阅读数：2080








1、在使用replaceall函数对字符串进行特定字符替换时，发现带转义字符的\要特殊处理。

2、参考代码如下：


```java
String str="http:\u002F\u002Fi.ebayimg.com\u002Fimages\u002Fg\u002FNs4AAOSwEzxYUmT7\u002Fs-l1600.jpg";
		str=str.replaceAll("\\\\u002F", "/");
		System.out.println(str);
```




3、在java中要用正则来表示一个"\"，则需4个"\"即（"\\\\"），原因是：

1）"\"在java中是转义符，在正则表达式中也是转义符号

2）String.replaceAll(regex, replacement)实际用的是正则表达式的规则去匹配的；

3）先java转义，"\\\\"--解释成-->"\\"，replaceall方法中的正则再转义，"\\"---解释成-->"\"













