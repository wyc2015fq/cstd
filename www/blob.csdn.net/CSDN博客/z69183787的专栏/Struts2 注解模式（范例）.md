# Struts2 注解模式（范例） - z69183787的专栏 - CSDN博客
2016年03月18日 21:18:16[OkidoGreen](https://me.csdn.net/z69183787)阅读数：968
相信大家一定看到了两个class中定义了一样的action，不过看类的元数据，是不同的命名空间。这里比较重要（对我来说）的是
```
![复制代码](http://common.cnblogs.com/images/copycode.gif)
@Action(value = "/login", results = {
        @Result(name = "sucess", location = "/index.jsp"),
        @Result(name = "chain", location = "chain", type = "chain"),// 同namespace下
                                                                    // chain
        @Result(name = "otherNsChain", type = "chain", params = { "namespace",
                "/user", "actionName", "chain" }),// 不同namespace下
        @Result(name = "redirect", type = "redirect", location = "redirect.action"),// 同namespace
        @Result(name = "otherNsRedirect", type = "redirect", location = "/user/redirect.action"),// 不同namespace
        @Result(name = "redirectAction", type = "redirectAction", params = {
                "actionName", "redirectAction" }),// 同namespace
        @Result(name = "otherNsRedirectAction", type = "redirectAction", params = {
                "namespace", "/user", "actionName", "redirectAction" }) // 不同namespace
})
![复制代码](http://common.cnblogs.com/images/copycode.gif)
```
这里面results的配置，当result的type是chain的时候，如果是同一个命名空间，那么就可以直接写一个 action的name，如果加上后缀名比如.action的话struts就会报错，找不到execute()方法，如果不是同一个命名空间下，则需要 添上params，params是一个String数组形式的容器，｛“key1”，“value1”，“key2”，“value2”｝以这种形式书 写，关与params的更多信息可以参考xwork-core包里的
com.opensymphony.xwork2.ActionChainResult.class
。redirect的一致，如果在不同的命名空间，直接写命名空间+action的名字。 redirectAction的和chain差不多，但也有些许的区别。redirectAction没有location的配置，action的 name需要在params里面配置，key为actionName，namespace也需要在params里面配置，key为namespace。更 多的请参考struts-core下的
org.apache.struts2.dispatcher.ServletActionRedirectResult.class
。下面附上一些常用的几个result type的类
```
![复制代码](http://common.cnblogs.com/images/copycode.gif)
 <result-types>
            <result-type name="chain" class="com.opensymphony.xwork2.ActionChainResult"/>
            <result-type name="dispatcher" class="org.apache.struts2.dispatcher.ServletDispatcherResult" default="true"/>
            <result-type name="freemarker" class="org.apache.struts2.views.freemarker.FreemarkerResult"/>
            <result-type name="httpheader" class="org.apache.struts2.dispatcher.HttpHeaderResult"/>
            <result-type name="redirect" class="org.apache.struts2.dispatcher.ServletRedirectResult"/>
            <result-type name="redirectAction" class="org.apache.struts2.dispatcher.ServletActionRedirectResult"/>
            <result-type name="stream" class="org.apache.struts2.dispatcher.StreamResult"/>
            <result-type name="velocity" class="org.apache.struts2.dispatcher.VelocityResult"/>
            <result-type name="xslt" class="org.apache.struts2.views.xslt.XSLTResult"/>
            <result-type name="plainText" class="org.apache.struts2.dispatcher.PlainTextResult" />
 </result-types>
![复制代码](http://common.cnblogs.com/images/copycode.gif)
```
- 基本上常用的以上都有了，包括上面提到的chain和redirectAction。 其实这些在struts-default.xml 也可以找到 ![大笑](http://www.linuxidc.com/upload/2013_06/130611102737801.gif)。这些类里面有他们可接受配置的demo。
- 写2个在搭建环境的时候遇到的异常。
- java.lang.NoSuchMethodException: com.struts2.action.IndexAction.execute() 可能是是配置有问题，比如我在上文说的在action的name中加上了后缀名或者也可能是在配置的时候action名前写了一个/
 。@Result(name="chain", location = "chain",type="chain"）这句代码如果在location的chain前加一个/，可能也会造成这个异常。
- - 还有一个异常是 提示没有create方法，这个可能是因为struts里面rest的插件和action冲突了。
- 注：以上两个异常不一定就是我说的问题引发的，真正的原因还是需要你自己去细细排查来确认的。
