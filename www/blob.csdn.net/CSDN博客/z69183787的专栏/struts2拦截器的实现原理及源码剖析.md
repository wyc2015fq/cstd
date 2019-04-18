# struts2拦截器的实现原理及源码剖析 - z69183787的专栏 - CSDN博客
2014年08月06日 07:17:34[OkidoGreen](https://me.csdn.net/z69183787)阅读数：699
个人分类：[Struts2](https://blog.csdn.net/z69183787/article/category/2175433)
**本文来自：曹胜欢博客专栏。转载请注明出处：****[http://blog.csdn.net/csh624366188](http://blog.csdn.net/csh624366188)**
拦截器（interceptor）是Struts2最强大的特性之一，也可以说是struts2的核心，拦截器可以让你在Action和result被执行之前或之后进行一些处理。同时，拦截器也可以让你将通用的代码模块化并作为可重用的类。Struts2中的很多特性都是由拦截器来完成的。拦截是AOP的一种实现策略。在Webwork的中文文档的解释为：拦截器是动态拦截Action调用的对象。它提供了一种机制可以使开发者可以定义在一个action执行的前后执行的代码，也可以在一个action执行前阻止其执行。同时也是提供了一种可以提取action中可重用的部分的方式。谈到拦截器，还有一个词大家应该知道——拦截器链（Interceptor Chain，在Struts 2中称为拦截器栈Interceptor Stack）。拦截器链就是将拦截器按一定的顺序联结成一条链。在访问被拦截的方法或字段时，拦截器链中的拦截器就会按其之前定义的顺序被调用。
**一．拦截器的实现原理：**
大部分时候，拦截器方法都是通过代理的方式来调用的。Struts 2的拦截器实现相对简单。当请求到达Struts 2的ServletDispatcher时，Struts 2会查找配置文件，并根据其配置实例化相对的拦截器对象，然后串成一个列表（list），最后一个一个地调用列表中的拦截器。事实上，我们之所以能够如此灵活地使用拦截器，**完全归功于“动态代理”的使用**。动态代理是代理对象根据客户的需求做出不同的处理。对于客户来说，只要知道一个代理对象就行了。那Struts2中，**拦截器是如何通过动态代理被调用的呢**？当Action请求到来的时候，会由系统的代理生成一个Action的代理对象，由这个代理对象调用Action的execute()或指定的方法，并在struts.xml中查找与该Action对应的拦截器。如果有对应的拦截器，就在Action的方法执行前（后）调用这些拦截器；如果没有对应的拦截器则执行Action的方法。其中系统对于拦截器的调用，是通过ActionInvocation来实现的。代码如下：
**[java]**[view
 plain](http://blog.csdn.net/csh624366188/article/details/7543342#)[copy](http://blog.csdn.net/csh624366188/article/details/7543342#)[print](http://blog.csdn.net/csh624366188/article/details/7543342#)[?](http://blog.csdn.net/csh624366188/article/details/7543342#)
- if (interceptors.hasNext()) {  
- Interceptor interceptor=(Interceptor)interceptors.next();  
- resultCode = interceptor.intercept(this);  
- } else {  
- if (proxy.getConfig().getMethodName() == null) {  
- resultCode = getAction().execute();  
- } else {  
- resultCode = invokeAction(getAction(), proxy.getConfig());  
- }  
- }  
可以发现Action并没有与拦截器发生直接关联，而完全是“代理”在组织Action与拦截器协同工作。如下图：
![](https://img-my.csdn.net/uploads/201205/07/1336384721_1343.jpg)
**二．拦截器执行分析**
     我们大家都知道，Interceptor的接口定义没有什么特别的地方，除了init和destory方法以外，intercept方法是实现整个拦截器机制的核心方法。而它所依赖的参数ActionInvocation则是著名的Action调度者。我们再来看看一个典型的Interceptor的抽象实现类：
**[java]**[view
 plain](http://blog.csdn.net/csh624366188/article/details/7543342#)[copy](http://blog.csdn.net/csh624366188/article/details/7543342#)[print](http://blog.csdn.net/csh624366188/article/details/7543342#)[?](http://blog.csdn.net/csh624366188/article/details/7543342#)
- publicabstractclass AroundInterceptor extends AbstractInterceptor {  
- 
- 
- /* (non-Javadoc)
- 
-  * @see com.opensymphony.xwork2.interceptor.AbstractInterceptor#intercept(com.opensymphony.xwork2.ActionInvocation)
- 
-  */
- 
- @Override
- 
- public String intercept(ActionInvocation invocation) throws Exception {  
- 
- String result = null;  
- 
- 
-         before(invocation);  
- 
- // 调用下一个拦截器，如果拦截器不存在，则执行Action
- 
-         result = invocation.invoke();  
- 
-         after(invocation, result);  
- 
- 
- return result;  
- 
- }  
- 
- 
- publicabstractvoid before(ActionInvocation invocation) throws Exception;  
- 
- 
- publicabstractvoid after(ActionInvocation invocation, String resultCode) throws Exception;  
- 
- 
- }  
     在这个实现类中，实际上已经实现了最简单的拦截器的雏形。这里需要指出的是一个很重要的方法invocation.invoke()。这是ActionInvocation中的方法，而ActionInvocation是Action调度者，所以这个方法具备以下2层含义：
**1. 如果拦截器堆栈中还有其他的Interceptor，那么invocation.invoke()将调用堆栈中下一个Interceptor的执行。**
**2. 如果拦截器堆栈中只有Action了，那么invocation.invoke()将调用Action执行。**
    所以，我们可以发现，invocation.invoke()这个方法其实是整个拦截器框架的实现核心。基于这样的实现机制，我们还可以得到下面2个非常重要的推论：
**1. 如果在拦截器中，我们不使用invocation.invoke()来完成堆栈中下一个元素的调用，而是直接返回一个字符串作为执行结果，那么整个执行将被中止。**
**2. 我们可以以invocation.invoke()为界，将拦截器中的代码分成2个部分，在invocation.invoke()之前的代码，将会在Action之前被依次执行，而在invocation.invoke()之后的代码，将会在Action之后被逆序执行。**
由此，我们就可以通过invocation.invoke()作为Action代码真正的拦截点，从而实现AOP。
**三.源码解析**
下面我们通过查看源码来看看Struts2是如何保证拦截器、Action与Result三者之间的执行顺序的。之前我曾经提到，ActionInvocation是Struts2中的调度器，所以事实上，这些代码的调度执行，是在ActionInvocation的实现类中完成的，这里，我抽取了DefaultActionInvocation中的invoke()方法，它将向我们展示一切。
**[java]**[view
 plain](http://blog.csdn.net/csh624366188/article/details/7543342#)[copy](http://blog.csdn.net/csh624366188/article/details/7543342#)[print](http://blog.csdn.net/csh624366188/article/details/7543342#)[?](http://blog.csdn.net/csh624366188/article/details/7543342#)
- /**
- 
-  * @throws ConfigurationException If no result can be found with the returned code
- 
-  */
- 
- public String invoke() throws Exception {  
- 
-     String profileKey = "invoke: ";  
- 
- try {  
- 
-      UtilTimerStack.push(profileKey);  
- 
- 
- 
- if (executed) {  
- 
- thrownew IllegalStateException("Action has already executed");  
- 
-      }  
- 
- // 依次调用拦截器堆栈中的拦截器代码执行
- 
- if (interceptors.hasNext()) {  
- 
- final InterceptorMapping interceptor = (InterceptorMapping) interceptors.next();  
- 
-      UtilTimerStack.profile("interceptor: "+interceptor.getName(),   
- 
- new UtilTimerStack.ProfilingBlock<String>() {  
- 
- public String doProfiling() throws Exception {  
- 
- // 将ActionInvocation作为参数，调用interceptor中的intercept方法执行
- 
-      resultCode = interceptor.getInterceptor().intercept(DefaultActionInvocation.this);  
- 
- returnnull;  
- 
- }  
- 
-      });  
- 
-      } else {  
- 
-      resultCode = invokeActionOnly();  
- 
-      }  
- 
- 
- // this is needed because the result will be executed, then control will return to the Interceptor, which will
- 
- // return above and flow through again
- 
- if (!executed) {  
- 
- // 执行PreResultListener
- 
- if (preResultListeners != null) {  
- 
- for (Iterator iterator = preResultListeners.iterator();  
- 
-      iterator.hasNext();) {  
- 
-      PreResultListener listener = (PreResultListener) iterator.next();  
- 
- 
- 
-      String _profileKey="preResultListener: ";  
- 
- try {  
- 
-      UtilTimerStack.push(_profileKey);  
- 
-      listener.beforeResult(this, resultCode);  
- 
-      }  
- 
- finally {  
- 
-      UtilTimerStack.pop(_profileKey);  
- 
-      }  
- 
-      }  
- 
-      }  
- 
- 
- // now execute the result, if we're supposed to
- 
- // action与interceptor执行完毕，执行Result
- 
- if (proxy.getExecuteResult()) {  
- 
-      executeResult();  
- 
-      }  
- 
- 
-      executed = true;  
- 
-      }  
- 
- 
- return resultCode;  
- 
-     }  
- 
- finally {  
- 
-      UtilTimerStack.pop(profileKey);  
- 
-     }  
- 
- }  
      从源码中，我们可以看到Action层的4个不同的层次，在这个方法中都有体现，他们分别是：拦截器（Interceptor）、Action、PreResultListener和Result。在这个方法中，保证了这些层次的有序调用和执行。由此我们也可以看出**Struts2在Action层次设计上的众多考虑，每个层次都具备了高度的扩展性和插入点，使得程序员可以在任何喜欢的层次加入自己的实现机制改变Action的行为。**
在这里，需要特别强调的，是其中拦截器部分的执行调用：
**[java]**[view
 plain](http://blog.csdn.net/csh624366188/article/details/7543342#)[copy](http://blog.csdn.net/csh624366188/article/details/7543342#)[print](http://blog.csdn.net/csh624366188/article/details/7543342#)[?](http://blog.csdn.net/csh624366188/article/details/7543342#)
- resultCode = interceptor.getInterceptor().intercept(DefaultActionInvocation.this);    
表面上，它只是执行了拦截器中的intercept方法，如果我们结合拦截器来看，就能看出点端倪来：
**[java]**[view
 plain](http://blog.csdn.net/csh624366188/article/details/7543342#)[copy](http://blog.csdn.net/csh624366188/article/details/7543342#)[print](http://blog.csdn.net/csh624366188/article/details/7543342#)[?](http://blog.csdn.net/csh624366188/article/details/7543342#)
- public String intercept(ActionInvocation invocation) throws Exception {     
- 
-     String result = null;     
- 
-          before(invocation);     
- 
- // 调用invocation的invoke()方法，在这里形成了递归调用  
- 
-       result = invocation.invoke();     
- 
-         after(invocation, result);     
- 
- return result;     
- 
- }    
  原来在intercept()方法又对ActionInvocation的invoke()方法进行递归调用，ActionInvocation循环嵌套在intercept()中，一直到语句result = invocation.invoke()执行结束。这样，Interceptor又会按照刚开始执行的逆向顺序依次执行结束。**一个有序链表，通过递归调用，变成了一个堆栈执行过程，将一段有序执行的代码变成了2段执行顺序完全相反的代码过程，从而巧妙地实现了AOP**。这也就成为了Struts2的Action层的AOP基础。
拦截器和过滤器之间有很多相同之处，但是两者之间存在根本的差别。其主要区别为以下几点：
1）拦截器是基于JAVA反射机制的，而过滤器是基于函数回调的。
2）过滤器依赖于Servlet容器，而拦截器不依赖于Servlet容器
3）拦截器只能对Action请求起作用，而过滤器可以对几乎所有的请求起作用。
4）拦截器可以访问Action上下文、值栈里的对象，而过滤器不能
5）在Action的生命周期中，拦截器可以多次被调用，而过滤器只能在容器初始化时被调用一次。
