# String cannot be resolved to a type - xqhrs232的专栏 - CSDN博客
2018年10月19日 15:06:29[xqhrs232](https://me.csdn.net/xqhrs232)阅读数：119
原文地址::[https://blog.csdn.net/xueer88888888888888/article/details/78991459](https://blog.csdn.net/xueer88888888888888/article/details/78991459)
报错内容: 
Multiple annotations found at this line: 
- String cannot be resolved to a type 
- The method getContextPath() from the type HttpServletRequest refers to the missing type String
解决办法: 
首先 右击该项目 - Build Path - Configure Build Path ， 在 Libraries 选项下，会发现有个出错的jre Libraries，将其Remove掉。 
然后 右击该项目 - Build Path - Add Library， 选择JRE System Library，选择Workspace defaulr JRE(jdk1.6)(此前JDK已成功安装并配置好)， 然后 Finish 。
