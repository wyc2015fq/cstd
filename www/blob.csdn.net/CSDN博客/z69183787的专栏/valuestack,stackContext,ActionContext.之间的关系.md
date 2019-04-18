# valuestack,stackContext,ActionContext.之间的关系 - z69183787的专栏 - CSDN博客
2015年07月16日 16:00:49[OkidoGreen](https://me.csdn.net/z69183787)阅读数：539
个人分类：[Struts2																[Ognl表达式](https://blog.csdn.net/z69183787/article/category/2202979)](https://blog.csdn.net/z69183787/article/category/2175433)
三者之间的关系如下图所示：
![relation](https://p-blog.csdn.net/images/p_blog_csdn_net/quechao123/EntryImages/20090804/QQ%E6%88%AA%E5%9B%BE%E6%9C%AA%E5%91%BD%E5%90%8D.png)
ActionContext 
一次Action调用都会创建一个ActionContext 
调用：ActionContext context = ActionContext.getContext() 
ValueStack 
由OGNL框架实现 
可以把它简单的看作一个List 
Stack Object：放入stack中的对象，一般是action。 
Stack Context（map）：stack上下文，它包含一些列对象，包括request/session/attr/application map等。 
EL：存取对象的任意属性，调用对象的方法，遍历整个对象结构图。 
ActionContext是Action上下文
可以得到request session application
ValueStack是值栈 存放表单中的值
Stack Context 栈上下文 也是用来存值的
个人感觉吧，action context 是在action中通过actionSupport类来获取到，主要作用是获取request之类的对象
然后valuestack和stack context都是为了使用OGNL，其中value stack 是stack context的根对象，所以我们在JSP页面中访问value stack的内容时，是不用加#，而如果是访问stack context的其他对象则要加上#。

[/color]学习[Struts](http://www.chinawin.net/tag/struts/)2，一直不明白表单中的值是怎么传给Action的，上网查了些资料，基本了解了！下面基本是从几个人的BOLG转载过来，以后记不清了再来看~
 [color=red]先看看我做的实验jsp页面
<s:form action="hello/converter.action" method="post">
<s:textfield name="point" label="点"></s:textfield>
<s:textfield name="point2" label="2"></s:textfield>
<s:textfield name="point3" label="3"></s:textfield>
<s:textfield name="age" label="年龄"></s:textfield>
<s:textfield name="date" label="日期"></s:textfield>
<s:submit name="提交"></s:submit>
</s:form>
结果图（是通过<s:debug></s:debug>得到的）
value stack：
![Struts2中OGNL，valueStack，stackContext的学习（续）](http://img.chinawin.net/it/d33/120545/1.jpg)
Stack context：
![Struts2中OGNL，valueStack，stackContext的学习（续）](http://img.chinawin.net/it/d33/120545/2.jpg)
![Struts2中OGNL，valueStack，stackContext的学习（续）](http://img.chinawin.net/it/d33/120545/3.jpg)
![Struts2中OGNL，valueStack，stackContext的学习（续）](http://img.chinawin.net/it/d33/120545/4.jpg)
通过图中我们可以看到
valuestack中包括我传递的值（point，point2，point3，age，date）
stack context中包括了 request application OgnlValueStack(root) session parameters 等属性值栈(ValueStack)　　　　
Struts2将OGNL上下文设置为Struts2中的ActionContext(内部使用的仍然是OgnlContext)，并将值栈设为OGNL的根对象。　　　　
我们知道，OGNL上下文中的根对象可以直接访问，不需要使用任何特殊的“标记”，而引用上下文中的其他对象则需要使用“#”来标记。由于值栈是上下文中的根对象，因此可以直接访问。那么对于值栈中的对象该如何访问呢？Struts2提供了一个特殊的OGNLPropertyAccessor，它可以自动查找栈内的所有对象(从栈顶到栈底)，直接找到一个具有你所查找的属性的对象。也就是说，对于值栈中的任何对象都可以直接访问，而不需要使用“#”。　　　　
假设值栈中有两个对象：student和employee，两个对象都有name属性，student有学号属性number，而employee有薪水属性salary。employee先入栈，student后入栈，位于栈顶，那么对于表达式name,访问的就是student的name属性，因为student对象位于栈顶；表达式salary，访问的就是employee的salary属性。正如你所见，访问值栈中的对象属性或方法，无须指明对象，也不用“#”，就好像值栈中的对象都是OGNL上下文中的根对象一样。这就是Struts2在OGNL基础上做出的改进。值栈中的Action实例　　
Struts2框架总是把Action实例放在栈顶。因为Action在值栈中，而值栈又是OGNL中的根，所以引用Action的属性可以省略“#”标记，这也是为什么我们在结果页面中可以直接访问Action的属性的原因。Struts2中的命名对象　　　　
Struts2还提供了一些命名对象，这些对象没有保存在值栈中，而是保存在ActionContext中，因此访问这些对象需要使用“#”标记。这些命名对象都是Map类型。　　　parameters　　　　
用于访问请求参数。如：#parameters['id']或#parameters.id，相当于调用了HttpServletRequest对象的getParameter()方法。　　　　
注意，parameters本质上是一个使用HttpServletRequest对象中的请求参数构造的Map对象，一量对象被创建(在调用Action实例之前就已经创建好了)，它和HttpServletRequest对象就没有了任何关系。　


**先分清楚下ActionContext 、ValueStack 、Stack Context三者**
ActionContext 
一次Action调用都会创建一个ActionContext 
调用：ActionContext context = ActionContext.getContext() 
ValueStack 
由OGNL框架实现 
可以把它简单的看作一个栈（List） 。
Stack Object：放入stack中的对象，一般是action。 
Stack Context（map）：stack上下文，它包含一系列对象，包括request/session/attr/application map等。 
EL：存取对象的任意属性，调用对象的方法，遍历整个对象结…
ActionContext是Action上下文，可以得到request session application 
ValueStack是值栈 存放表单中的值 
Stack Context 栈上下文 也是用来存值的
**struts2对OGNL上下文的概念又做了进一步扩充，在struts2中，OGNL上下文通常如下所示：**
                    |--request   
                        |   
                        |--application   
                        |   
 context map---|--OgnlValueStack(root) [ user, action, OgnlUtil, ... ]   
                        |   
                        |--session   
                        |   
                        |--attr   
                        |   
                        |--parameters
    在Struts2中，采用标准命名的上下文(Context)来处理OGNL表达式。处理OGNL的顶级对象是一个Map（也叫context map），而OGNL在这个context中就是一个顶级对象(root)。在用法上，顶级对象的属性访问，是不需要任何标记前缀的。而其它非顶级的对象访问，需要使用#标记。
    Struts2框架把OGNL Context设置为我们的ActionContext。并且ValueStack作为OGNL的根对象。除value stack之外，Struts2框架还把代表application、session、request这些对象的Map对象也放到ActionContext中去。（这也就是Struts2建议在Action类中不要直接访问Servlet API的原因，他可以**通过ActionContext对象来部分代替这些（Servlet API）功能** ，以方便对Action类进行测试！）
    Action的实例，总是放到value stack中。因为Action放在stack中，而stack是root(根对象)，所以对Action中的属性的访问就可以省略#标记。但是，要访问ActionContext中其它对象的属性，就必须要带上#标记，以便让OGNL知道，不是从根对象，而是从其它对象中去寻找。
    那么访问Action中的属性的代码就可以这样写
<s:property value="postalCode"/>
    其它ActionContext中的非根对象属性的访问要像下面这样写：
<s:property value="#session.mySessionPropKey"/> or
<s:property value="#session['mySessionPropKey']"/> or
<s:property value="#request['myRequestPropKey']"/>
    对Collection的处理，内容就很简单。
<s:select label="label" name="name" list="{'name1','name2','name3'}" value="%{'name2'}" />
    这是处理List。这个代码在页面上建立一个下拉选项，内容是list中的内容，默认值是name2.
处理map
 <s:select label="label" name="name" list="#{'foo':'foovalue', 'bar':'barvalue'}" />
     需要注意的是，判断一个值是否在collection中。我们要使用in或者not in来处理。
<s:if test="'foo' in {'foo','bar'}">
   muhahaha
</s:if>
<s:else>
   boo
</s:else>
 另外，可以使用通配符来选择collection对象的子集。
 ?——所有匹配选择逻辑的元素
 ^——只提取符合选择逻辑的第一个元素
 $——只提取符合选择逻辑的最后一个元素
person.relatives.{? #this.gender == 'male'}
````````````````````````````````````````````````````````````````````````````````` 
** 以下为补充摘录的一些问题：**
**提问： 在Struts2中，如何使用自身的Tag读取Action中的变量？**
Struts2自身的Tag会根据value中的OGNL表达式，在ValueStack中寻找相应的对象。因为action在ValueStack的顶部，所以默认情况下，Struts2的Tag中的OGNL表达式将查找action中的变量。请注意，value中的内容直接是OGNL表达式，无需任何el的标签包装。 
例如：<s:property value="user.name" /> 
**提问： 在Struts2中，如何使用自身的Tag读取HttpServletRequest，HttpSession中的变量？**
在上面的知识中，我们知道，Struts2中OGNL的上下文环境中，包含request，session，application等servlet对象的Map封装。既然这些对象都在OGNL的上下文中，那么根据OGNL的基本知识，我们可以通过在表达式前面加上#符号来对这些变量的值进行访问。 
例如：<s:property value="%{#application.myApplicationAttribute}" /> 
<s:property value="%{#session.mySessionAttribute}" /> 
<s:property value="%{#request.myRequestAttribute}" /> 
<s:property value="%{#parameters.myParameter}" />
**提问： 在Struts2中，如何使用JSTL来读取Action中的变量？**
这是一个历史悠久的问题。因为事实上，很多朋友（包括我在内）是不使用Struts2自身的标签库，而是使用JSTL的，可能因为JSTL标签库比较少，简单易用的原因吧。 
我们知道，JSTL默认是从page，request，session，application这四个Scope逐次查找相应的EL表达式所对应的对象的值。那么如果要使用JSTL来读取Action中的变量，就需要把Action中的变量，放到request域中才行。所以，早在Webwork2.1.X的年代，我们会编写一个拦截器来做这个事情的。大致的原理是：在Action执行完返回之前，依次读取Action中的所有的变量，并依次调用request.setAttribute()来进行设置。具体的整合方式，请参考以下这篇文档：[http://wiki.opensymphony.com/display/WW/Using+WebWork+and+XWork+with+JSP+2.0+and+JSTL+1.1](http://wiki.opensymphony.com/display/WW/Using+WebWork+and+XWork+with+JSP+2.0+and+JSTL+1.1)
不过随着时代的发展，上面的这种方式，已经不再被推荐使用了。（虽然如此，我们依然可以学习它的一个解决问题的思路）目前来说，自从Webwork2.2以后，包括Struts2，都使用另外一种整合方式：对HttpServletRequest进行装饰。让我们来看一下源码：
Java代码 ![复制代码](http://blog.csdn.net/images/icon_copy.gif)
- publicclass StrutsRequestWrapper extends HttpServletRequestWrapper {
   
- 
- /** 
-      * The constructor 
-      * @param req The request 
-      */
- public StrutsRequestWrapper(HttpServletRequest req) {
   
- super(req);
   
-     }   
- 
- /** 
-      * Gets the object, looking in the value stack if not found 
-      * 
-      * @param s The attribute key 
-      */
- public Object getAttribute(String s) {
   
- if (s != null && s.startsWith("javax.servlet")) {
   
- // don't bother with the standard javax.servlet attributes, we can short-circuit this 
- // see WW-953 and the forums post linked in that issue for more info 
- returnsuper.getAttribute(s);
   
-         }   
- 
-         ActionContext ctx = ActionContext.getContext();   
-         Object attribute = super.getAttribute(s);
   
- 
- boolean alreadyIn = false;
   
-         Boolean b = (Boolean) ctx.get("__requestWrapper.getAttribute");
   
- if (b != null) {
   
-             alreadyIn = b.booleanValue();   
-         }   
- 
- // note: we don't let # come through or else a request for 
- // #attr.foo or #request.foo could cause an endless loop 
- if (!alreadyIn && attribute == null && s.indexOf("#") == -1) {
   
- try {
   
- // If not found, then try the ValueStack 
-                 ctx.put("__requestWrapper.getAttribute", Boolean.TRUE);
   
-                 ValueStack stack = ctx.getValueStack();   
- if (stack != null) {
   
-                     attribute = stack.findValue(s);   
-                 }   
-             } finally {
   
-                 ctx.put("__requestWrapper.getAttribute", Boolean.FALSE);
   
-             }   
-         }   
- return attribute;
   
-     }   
- }  
**[java]**[view
 plain](http://blog.csdn.net/siobhan/article/details/4378434#)[copy](http://blog.csdn.net/siobhan/article/details/4378434#)
- publicclass StrutsRequestWrapper extends HttpServletRequestWrapper {  
- 
- /**
-      * The constructor
-      * @param req The request
-      */
- public StrutsRequestWrapper(HttpServletRequest req) {  
- super(req);  
-     }  
- 
- /**
-      * Gets the object, looking in the value stack if not found
-      *
-      * @param s The attribute key
-      */
- public Object getAttribute(String s) {  
- if (s != null && s.startsWith("javax.servlet")) {  
- // don't bother with the standard javax.servlet attributes, we can short-circuit this
- // see WW-953 and the forums post linked in that issue for more info
- returnsuper.getAttribute(s);  
-         }  
- 
-         ActionContext ctx = ActionContext.getContext();  
-         Object attribute = super.getAttribute(s);  
- 
- boolean alreadyIn = false;  
-         Boolean b = (Boolean) ctx.get("__requestWrapper.getAttribute");  
- if (b != null) {  
-             alreadyIn = b.booleanValue();  
-         }  
- 
- // note: we don't let # come through or else a request for
- // #attr.foo or #request.foo could cause an endless loop
- if (!alreadyIn && attribute == null && s.indexOf("#") == -1) {  
- try {  
- // If not found, then try the ValueStack
-                 ctx.put("__requestWrapper.getAttribute", Boolean.TRUE);  
-                 ValueStack stack = ctx.getValueStack();  
- if (stack != null) {  
-                     attribute = stack.findValue(s);  
-                 }  
-             } finally {  
-                 ctx.put("__requestWrapper.getAttribute", Boolean.FALSE);  
-             }  
-         }  
- return attribute;  
-     }  
- }  
 看到了嘛？这个类会**在**Struts2初始化的时候，替换HttpServletRequest，运行于整个Struts2的运行过程中，当我们试图调用request.getAttribute()的时候，就会执行上面的这个方法。（这是一个典型的装饰器模式）在执行上面的方法时，会首先调用HttpServletRequest中原本的request.getAttribute()，如果没有找到，它会继续到ValueStack中去查找，而action在ValueStack中，所以action中的变量通过OGNL表达式，就能找到对应的值了。
在这里，在el表达式广泛使用的今天，JSTL1.1以后，也支持直接使用el表达式。注意与直接使用struts2的tag的区别，这里需要使用el的表示符号：${} 
例如：${user.name}, <c:out value="${department.name}" /> 
**提问： 在Struts2中，如何使用Freemarker等模板来读取Action中的变量以及HttpServletRequest和HttpSession中的变量？**
Freemarker等模板在Struts2中有对应的Result，而在这些Result中，Freemarker等模板会根据ValueStack和ActionContext中的内容，构造这些模板可识别的Model，从而使得模板可以以他们各自的语法对ValueStack和ActionContext中的内容进行读取。 
有关Freemarker对于变量的读取，可以参考Struts2的官方文档，非常详细：[http://struts.apache.org/2.0.14/docs/freemarker.html](http://struts.apache.org/2.0.14/docs/freemarker.html)
**设值计算**
Struts2中使用OGNL进行设值计算，就是指View层传递数据到Control层，并且能够设置到相应的Java对象中。这个过程从逻辑上说需要分成两步来完成： 
1. 对于每个请求，都建立一个与相应Action对应的ActionContext作为OGNL的上下文环境和ValueStack，并且把Action压入ValueStack 
2. 在请求进入Action代码前，通过某种通用的机制，搜集页面上传递过来的参数，并调用OGNL相关的代码，对Action进行设值。 
上面的第一个步骤，在处理URL请求时完成，而第二个步骤由struts2内置的拦截器完成。
