# Struts2的声明式异常处理 - z69183787的专栏 - CSDN博客
2014年11月24日 15:14:59[OkidoGreen](https://me.csdn.net/z69183787)阅读数：637
个人分类：[Struts2](https://blog.csdn.net/z69183787/article/category/2175433)
在struts2应用程序中你还在使用try catch语句来捕获异常么？如果是这样的，那你OUT啦！struts2支持声明式异常处理，可以再Action中直接抛出异常而交给struts2来处理，当然需要我们在xml文件中配置，由于抛出同样的异常的处理方法通常都一样，所以如果能在xml中配置全局异常，将会使得开发便捷性大大提高。
以前的异常捕获可能是这样的：
```java
/**
```
```java
```
```java
* 执行更新
```
```java
```
```java
*
```
```java
```
```java
* @return
```
```java
```
```java
*/
```
```java
public
```
```java
String update() {
```
```java
```
```java
Article article =
```
```java
new
```
```java
Article();
```
```java
```
```java
article.setContent(content);
```
```java
```
```java
article.setTitle(title);
```
```java
```
```java
article.setId(id);
```
```java
```
```java
try
```
```java
{
```
```java
```
```java
articleService.update(article);
```
```java
```
```java
return
```
```java
SUCCESS;
```
```java
```
```java
}
```
```java
catch
```
```java
(SQLException e) {
```
```java
```
```java
e.printStackTrace();
```
```java
```
```java
return
```
```java
ERROR;
```
```java
```
```java
}
```
```java
catch
```
```java
(InvalidInputException e) {
```
```java
```
```java
e.printStackTrace();
```
```java
```
```java
System.out.println(
```
```java
"输入非法"
```
```java
);
```
```java
```
```java
return
```
```java
ERROR;
```
```java
```
```java
}
```
```java
}
```
这种方式是完全的手动处理异常，一来不够简洁明快，而且还不容易维护，毕竟如果修改了这些代码都需要再次编译。
采用struts2的声明式异常处理就会简单很多了。
首先，上面的代码的try catch 就可以全都不要了，但是，当然，得新加throw语句抛出异常：
```java
/**
```
```java
```
```java
* 执行更新
```
```java
```
```java
*
```
```java
```
```java
* @return
```
```java
```
```java
* @throws InvalidInputException
```
```java
```
```java
* @throws SQLException
```
```java
```
```java
*/
```
```java
public
```
```java
String update()
```
```java
throws
```
```java
SQLException, InvalidInputException {
```
```java
```
```java
Article article =
```
```java
new
```
```java
Article();
```
```java
```
```java
article.setContent(content);
```
```java
```
```java
article.setTitle(title);
```
```java
```
```java
article.setId(id);
```
```java
```
```java
articleService.update(article);
```
```java
```
```java
return
```
```java
SUCCESS;
```
```java
}
```
代码清晰了很多，不是么？
捕获异常的任务则交给xml配置文件了，配置文件还是比较容易理解的：
`<``package``name="wow" extends="struts-default">`
`    ``<``global-results``>`
`        ``<``result``name="sql">/internal_Error.jsp</``result``>`
`        ``<``result``name="invalidinput">/invalid_Input.jsp</``result``>`
`        ``<``result``name="naming">/internal_Error.jsp</``result``>`
`    ``</``global-results``>`
`    ``<``global-exception-mappings``>`
`        ``<``exception-mapping``result="sql" exception="java.sql.SQLException"></``exception-mapping``>`
`        ``<``exception-mapping``result="invalidinput"`
`            ``exception="cn.codeplus.exception.InvalidInputException"></``exception-mapping``>`
`        ``<``exception-mapping``result="naming"`
`            ``exception="javax.naming.NamingException"></``exception-mapping``>`
`    ``</``global-exception-mappings``>`
`    ``<``action``name="*_*" class="cn.codeplus.action.{2}Action" method="{1}">`
`        ``<``result``name="success">/{1}_{2}_success.jsp</``result``>`
`        ``<``result``name="error">/{1}_{2}_error.jsp</``result``>`
`        ``<!--<exception-mapping result="sql" exception="java.sql.SQLException"></exception-mapping>-->`
`    ``</``action``>`
`</``package``>`
用于异常处理的<exception-mapping>标签可以配置在Action中，也可以配置在<global-exception-mappings>，顾名思义<global-exception-mappings>就是全局异常，当然执行Action的时候发生异常时，如果在Action中没有捕获异常而是抛出异常的话，struts2会首先在正在执行的Action中查找<exception-mapping>，寻找对应的Exception进行处理，如果找不到，才会去<global-exception-mappings>去寻找对应的Exception处理，如果还是找不到的话，就只好抛出异常了。
下面说说异常处理：
`<``exception-mapping``result="sql" exception="java.sql.SQLException"></``exception-mapping``>`
上面代码说明，当捕获java.sql.SQLException时候，去寻找对应的result为sql的视图返回，即<global-result>中name为sql的result去返回internal_Error.jsp。当然如果<exception-mapping>配置在action中的话，则会首先去action的result搜寻返回视图，失败了才会去搜寻<global-result>。
在我们编写上面的xml配置的时候可能会遇到如下错误：
![](http://pic002.cnblogs.com/images/2011/315806/2011071609295541.png)
这个是因为，我们xml配置文件各个标签<action>、<global-result>、<global-exception-mapping>的顺序不对，调整一下标签的顺序，符合黄色的提示语即可。
最后，我们说说视图层怎样获取异常信息，invalid_Input.jsp文件是这样的：
`...`
`<%@taglib prefix="s" uri="/struts-tags"%>`
`...`
`<``body``>`
`    ``<``jsp:include``page="nav.jsp"></``jsp:include``>`
`    ``<``div``>`
`        ``抱歉，服务器内部错误。`
`    ``</``div``>`
`    ``<``div``>`
`        ``<``s:property``value="exception.message"/>`
`    ``</``div``>`
`    ``<``s:debug``></``s:debug``>`
`</``body``>`
`...`
`<``s:property``value="exception.message"/>表示从valuestack中获取错误信息，显示在前台页面上。当然，我们也可以选择更人性化得处理方案，比如说，放个失望的表情，写上“抱歉，服务器内部错误，您可以发邮件给我们提示此错误，xxxx@xxxx.com”等等；`
`经测试，当发生SQLException的时候，页面信息如下：`
![](http://pic002.cnblogs.com/images/2011/315806/2011071609412086.png)
好了，很高兴来到园子，嘿嘿，希望这篇文章对大家有用。
