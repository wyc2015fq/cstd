# struts2整合json出现no result type defined for type 'json'的解决方法 - z69183787的专栏 - CSDN博客
2015年06月27日 10:05:28[OkidoGreen](https://me.csdn.net/z69183787)阅读数：708
struts2的json plugin可以实现struts2和json的完美结合，由于本篇主要是介绍整合过程中遇到的问题，所以编程实现的方法这里就不重复了，具体可以参看struts2的官方文档：[http://struts.apache.org/2.2.1.1/docs/json-plugin.html](http://struts.apache.org/2.2.1.1/docs/json-plugin.html)。
我在struts.xml中有如下action定义：
```
<action name="product_group" class="customers.products" method="getGroups">
<result type="json">
<param name="root">groupList</param>
</result>
</action>
```
在上面的定义中，action的result的type为json，json plugin就可将action中定义为groupList的field自动转换为json格式数据，并返回给前端UI。
但在deploy后，启动tomcat时却报了There is no result type defined for type 'json' mapped with name 'success'. Did you mean 'json'?的错误，因为struts2找不到json这个result type的定义。解决方法有下面两种：
1.将当前package的extends属性改为"json-default"，即让当前package从josn-default继承而不是struts-default继承；
2.但如果当前package确实无法继承"json-default"的话，还可以在当前package中定义result-type，将json给加进去，如下：
```
<result-types>
<result-type name="json" class="org.apache.struts2.json.JSONResult"/>
</result-types>
```
## 两种方法的原理：
json这个result type是在json-default  (struts2-json-plugin-2.1.8.1.jar\struts-plugin.xml)里面定义的，内容如下（省去了xml和doctype标签）：
```
![复制代码](http://common.cnblogs.com/images/copycode.gif)
<struts>
<package name="json-default" extends="struts-default">
<result-types>
<result-type name="json" class="org.apache.struts2.json.JSONResult"/>
</result-types>
<interceptors>
<interceptor name="json" class="org.apache.struts2.json.JSONInterceptor"/>
</interceptors>
</package>
</struts>
![复制代码](http://common.cnblogs.com/images/copycode.gif)
```
可见，name为"json"的result type是在json-default中定义的，所以，从json-default继承就可以使用json这个result。另外json-default还定义了一个name为"json"的interceptor。
另外，依json-default的定义来看，方法2中还应该再加一个json的interceptor定义才比较合适。
