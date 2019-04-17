# Java之URLEncoder和URLDecoder类使用小记 - 专注于数据挖掘算法研究和应用 - CSDN博客





2016年07月27日 10:27:31[fjssharpsword](https://me.csdn.net/fjssharpsword)阅读数：2066








                
网页中的表单使用POST方法提交时，数据内容是application/x-www-form-urlencoded类型。

从普通字符串到编码到application/x-www-form-urlencoded类型，遵循如下规则：

   1）字符"a"-"z"，"A"-"Z"，"0"-"9"，"."，"-"，"*"，和"_" 不编码，保持；

   2）将空格转换为加号 (+) ；

   3）将非文本内容转换成"%xy"的形式,xy是两位16进制的数值；

   4）表单中每个 name=value 对之间放置 & 符号；

Java之URLEncoder和URLDecoder类提供这一编码解码过程，基本代码如下：



```java
//将普通字符创转换成application/x-www-from-urlencoded字符串  
	        String urlString = URLEncoder.encode("name=中华&index=1 ", "UTF-8");  
	        System.out.println(urlString); 
			//将application/x-www-from-urlencoded字符串转换成普通字符串  
	        String keyWord = URLDecoder.decode(urlString, "UTF-8");  
	        System.out.println(keyWord);
```





