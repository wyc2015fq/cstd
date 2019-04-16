# ajax第一课：json - PeterBishop - CSDN博客





2019年01月09日 20:13:10[PeterBishop0](https://me.csdn.net/qq_40061421)阅读数：17
个人分类：[JavaWeb](https://blog.csdn.net/qq_40061421/article/category/8534452)









大纲:
- 简介
- 语法
- 使用



  接下来我们进入ajax篇，第一课先将json，讲啥看大纲。


- 简介:

JSON: **J**ava**S**cript **O**bject **N**otation(JavaScript 对象表示法)

JSON 是存储和交换文本信息的语法。类似 XML。

JSON 比 XML 更小、更快，更易解析。


- 语法:
	- ![](https://img-blog.csdnimg.cn/20190109195220220.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3FxXzQwMDYxNDIx,size_16,color_FFFFFF,t_70)
- ![](https://img-blog.csdnimg.cn/20190109195220249.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3FxXzQwMDYxNDIx,size_16,color_FFFFFF,t_70)



- 使用
	- json对象属性的访问和修改


修改: 对象.属性名 = 值

访问: 对象.属性名
- - json对象转字符串:


JSON.stringify(jsonObj)
- - json字符串转对象


JSON.parse(jsonStr)





