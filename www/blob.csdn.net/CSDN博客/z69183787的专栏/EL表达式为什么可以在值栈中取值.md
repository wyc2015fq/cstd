# EL表达式为什么可以在值栈中取值 - z69183787的专栏 - CSDN博客
2014年04月18日 10:21:01[OkidoGreen](https://me.csdn.net/z69183787)阅读数：5906
我在后台将一个对象的list集合放到了值栈的Map栈中
Java code
[?](http://bbs.csdn.net/topics/390687437#clipboardWindow)
```
```java
public
```
```java
String getLastVersionPD(){
```
```java
ActionContext.getContext().put(
```
```java
"pdList"
```
```java
,
```
```java
this
```
```java
.pdManager.getLastVersonPD());
```
```java
return
```
```java
"listAction"
```
```java
;
```
```java
}
```
```
然后在前台用OGNL表达式迭代
XML/HTML code
[?](http://bbs.csdn.net/topics/390687437#clipboardWindow)
```
`<``s:iterator``value``=``"#pdList"``>`
`  ``<``s:property``value``=``"name"``/>`
`  ``<``s:property``value``=``"version"``/>`
`  ``<``s:a``action``=``"pdManagerAction_delete?key=%{key}"``>删除</``s:a``>`
`  ``<``a``href``=``"javascript: showProcessImage('${requestScope.deploymentId}')"``>查看流程图</``a``>`
`</``s:iterator``>`
```
name version key deploymentId都是一个对象的属性，迭代时是放在值栈的对象栈的顶部的，前边的name version key使用OGNL表达式能在值栈的对象栈取到值，为什么使用EL表达式也可以在request作用域取得deploymentId属性呢
楼主这个问题,为什么用el取request范围的东西会同样取到值栈的内容呢.这个问题就要结合struts2的源码来说了,我们知道request在javaee里面是HttpServletRequest它是一个接口里面的一切包装都是通过javax.servlet.http.HttpServletRequestWrapper类来实现的.struts2有自己的javax.servlet.http.HttpServletRequestWrapper实现,也就是在struts2-core包下的org.apache.struts2.dispatcher.StrutsRequestWrapper重新对getAttribute方法进行了重写.
我们知道el表达式取request范围的值就是调用request.getAttribute(String s);
一下这段是struts2的org.apache.struts2.dispatcher.StrutsRequestWrapper对getAttribute方法重写的源码部分
Java code
[?](http://bbs.csdn.net/topics/390687437#clipboardWindow)
```
```java
public
```
```java
Object getAttribute(String s) {
```
```java
```
```java
if
```
```java
(s !=
```
```java
null
```
```java
&& s.startsWith(
```
```java
"javax.servlet"
```
```java
)) {
```
```java
```
```java
//如果是${requestScope.javax.servlet.属性名的}就直接去request里面取值
```
```java
```
```java
return
```
```java
super
```
```java
.getAttribute(s);
```
```java
```
```java
}
```
```java
//关键在下面,先从request里面取值取不到时就从值栈里面取值,所以就可以解释楼主这个为啥也能取到值了
```
```java
```
```java
ActionContext ctx = ActionContext.getContext();
```
```java
```
```java
Object attribute =
```
```java
super
```
```java
.getAttribute(s);
```
```java
```
```java
if
```
```java
(ctx !=
```
```java
null
```
```java
) {
```
```java
```
```java
if
```
```java
(attribute ==
```
```java
null
```
```java
) {
```
```java
```
```java
boolean
```
```java
alreadyIn =
```
```java
false
```
```java
;
```
```java
```
```java
Boolean b = (Boolean) ctx.get(
```
```java
"__requestWrapper.getAttribute"
```
```java
);
```
```java
```
```java
if
```
```java
(b !=
```
```java
null
```
```java
) {
```
```java
```
```java
alreadyIn = b.booleanValue();
```
```java
```
```java
}
```
```java
```
```java
if
```
```java
(!alreadyIn && s.indexOf(
```
```java
"#"
```
```java
) == -
```
```java
1
```
```java
) {
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
// If not found, then try the ValueStack
```
```java
```
```java
ctx.put(
```
```java
"__requestWrapper.getAttribute"
```
```java
, Boolean.TRUE);
```
```java
```
```java
ValueStack stack = ctx.getValueStack();
```
```java
```
```java
if
```
```java
(stack !=
```
```java
null
```
```java
) {
```
```java
```
```java
attribute = stack.findValue(s);
```
```java
```
```java
}
```
```java
```
```java
}
```
```java
finally
```
```java
{
```
```java
```
```java
ctx.put(
```
```java
"__requestWrapper.getAttribute"
```
```java
, Boolean.FALSE);
```
```java
```
```java
}
```
```java
```
```java
}
```
```java
```
```java
}
```
```java
```
```java
}
```
```java
```
```java
return
```
```java
attribute;
```
```java
```
```java
}
```
```
