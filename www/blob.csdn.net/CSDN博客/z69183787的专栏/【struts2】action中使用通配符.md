# 【struts2】action中使用通配符 - z69183787的专栏 - CSDN博客
2016年01月20日 16:52:32[OkidoGreen](https://me.csdn.net/z69183787)阅读数：813
个人分类：[Struts2](https://blog.csdn.net/z69183787/article/category/2175433)

在以前的学习中，<action>元素的配置，都是用明确的配置，其name、class等属性都是一个明确的值。其实Struts2还支持class属性和method属性使用来自name属性的通配符。
1）接下来看看使用通配符的示例，如果我们使用以下<action>配置：
```
<action name="*_*" class="cn.javass.action.action.{1}Action" method="{2}">  
     <result name="toWelcome">/s2impl/welcome.jsp</result>  
</action>
```
在上面的配置中：
       name属性的值中“*”代表长度不为0的任意字符串，因此，它可以响应的action只需要名称中间有一个下划线即可。比如页面可访问的action名称为：HelloWorld_create.action、HelloWorld _update.action等等。
       在name属性定义了通配符之后，class属性使用第一个通配符（使用{1}作为占位），method属性使用第二个通配符。
       如果使用HelloWorld_create.action作为访问的action名称的话，struts.xml中action名称为HelloWorld_create，第一个通配符匹配HelloWorld，第二个通配符匹配create。因此，由cn.javass.action.action.HelloWorldAction的create方法来响应。
2）那么，对于<result>元素，能不能也使用<action>元素的name属性定义的通配符呢？答案是可以的，假如有如下的配置：
```
<action name="*_*_*_*" class="cn.javass.action.action.{1}Action" method="{2}">  
     <result name="{3}">/${folder}/{4}.jsp</result>  
</action>
```
　3）在使用通配符的时候，也有可能不止一个使用通配符的<action>元素可能匹配这次URL的访问，看以下的配置文件：
```
<action name="HelloWorld_create" class="cn.javass.action.action.HelloWorldAction" method="create2">  
      <result name="toWelcome">/s2impl/welcome.jsp</result>  
</action>  
<action name="*_*" class="cn.javass.action.action.{1}Action" method="{2}">  
      <result name="toWelcome">/s2impl/welcome.jsp</result>  
</action>
```
　　这时候，如果访问“/helloworld/HelloWorld_create.action”，Struts2首先会查找是否有精确匹配的<action>元素，这时候无论以上<action>元素以什么顺序出现，Struts2肯定会先找到并使用精确匹配的<action>元素。但是，如果没有精确匹配的<action>元素，则Struts2会找到第一个匹配的使用通配符的<action>元素来使用。
　　通配符对于那些简单的CRUD的工程或软件原型来说，只要Action的包名、Action的类名、对应的方法名写的有规律的应用，能大大简化配置的工作。
