# Java String关于indexof方法区分大小写的一个小贴士 - 专注于数据挖掘算法研究和应用 - CSDN博客





2017年03月11日 17:59:27[fjssharpsword](https://me.csdn.net/fjssharpsword)阅读数：7120








indexof方法区分大小写，如果要处理的字符串对大小写不敏感，可以统一将字符串大写或小写后再indexof，如：

1）全部转换为大写：str.toUpperCase().IndexOf(s.toUpperCase())

2）全部转换为小写：str.toLowerCase().IndexOf(s.toLowerCase())

不过有时候字符串本身大小写也是敏感，不能统一转换，那就只能大小写都判断，如：



```java
if(src.contains("jpg"))//indexof区分大小写
	            		src=src.substring(src.indexOf("http"), src.indexOf("jpg")+3);
	            	else if (src.contains("JPG"))
	            		src=src.substring(src.indexOf("http"), src.indexOf("JPG")+3);
```







