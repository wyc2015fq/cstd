# 关于struts2标签中 #parameters.XXX获取参数的问题 - z69183787的专栏 - CSDN博客
2013年07月23日 19:31:03[OkidoGreen](https://me.csdn.net/z69183787)阅读数：6092

**```java``````java小技巧：#parameters.country[``````java1``````java]可以这样取值！原来Struts2是将URL传的参数封装 成一个数组的，```**
**```java``````java也就是说你可以在一个URL传两个同名的参数（即?name=a&name=b）；```**
今天想用  if test 标签 来判断 url参数的值
<s:property value="#parameters.zs"/>
<s:if test='#parameters.zs=="abc"'>
1</s:if>
<s:else>
2</s:else>
但无论如何也不相等
修改为
<s:property value="#parameters.zs"/>
<s:if test='#parameters.zs[0]=="abc"'>
1</s:if>
<s:else>
2</s:else>
后成功，没想到 parameters.xxx  的返回值居然是个数组。
在if test 标签中 必须使用 [0]
