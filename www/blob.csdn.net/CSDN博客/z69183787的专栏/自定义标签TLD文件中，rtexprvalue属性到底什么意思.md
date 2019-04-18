# 自定义标签TLD文件中，rtexprvalue属性到底什么意思 - z69183787的专栏 - CSDN博客
2015年06月28日 18:25:38[OkidoGreen](https://me.csdn.net/z69183787)阅读数：1569
rtexprvalue的全称是 Run-time Expression Value， 它用于表示是否能够利用JSP表白式
上网看了一下，有所收获，总结来说最简单的就是在使用标签时能不能使用表达式来动态指定数据。true是可以，false是不可以。
做个简单例子：
定义TLD文件如下：
Java代码  ![收藏代码](http://cuisuqiang.iteye.com/images/icon_star.png)
- <tag>  
-     <name>selectDetail</name>  
-     <tag-class>com.nms.taglib.SelectDetail_Tag</tag-class>  
-     <body-content>JSP</body-content>  
-     <attribute>  
-         <name>id</name>  
-         <rtexprvalue>true</rtexprvalue>  
-     </attribute>  
-     <attribute>  
-         <name>selectVal</name>  
-         <rtexprvalue>true</rtexprvalue>  
-     </attribute>  
-     <attribute>  
-         <name>collection</name>  
-         <rtexprvalue>true</rtexprvalue>  
-     </attribute>  
- </tag>  
页面可以这样设置需要的值：
Java代码  ![收藏代码](http://cuisuqiang.iteye.com/images/icon_star.png)
- <%  
- User user = new User();  
- user.setId(1);  
- request.setAttribute("user",user);  
- %>         
- <html:selectDetail id="" collection="<%="SEX" %>" selectVal="${user.id}"></html:selectDetail>  
使用脚本和EL表达式来获取动态的值
如果你将rtexprvalue设置为flase，那么上面的使用会直接报错
必须这样使用：
Java代码  ![收藏代码](http://cuisuqiang.iteye.com/images/icon_star.png)
- <html:selectDetail id="" collection="SEX" selectVal="男"></html:selectDetail>  
我看了一下别人自定义标签，其实吧你全设置为true就可以了，方便使用也省事。
