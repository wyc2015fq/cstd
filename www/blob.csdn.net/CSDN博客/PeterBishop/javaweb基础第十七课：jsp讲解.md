# javaweb基础第十七课：jsp讲解 - PeterBishop - CSDN博客





2019年01月04日 18:29:55[PeterBishop0](https://me.csdn.net/qq_40061421)阅读数：30








大纲:
- 运行原理
- 4大域对象
- jsp指令
	- page
- include
- taglib

- jsp动作标签
	- jsp:forword
- jsp:include




这节课我们来讲讲jsp的一些东西，讲什么大纲都已经写好了


- 运行原理:

tomcat会将.jsp文件编译成一个Servlet类，所以jsp的本质其实就是一个Servlet


- 4大域对象
|域对象|对应java对象|作用范围|
|----|----|----|
|page|this|本页面|
|request|HttpServletRequest|一次请求完成|
|session|HttpSession|一次会话完成(以后讲)|
|application|ServletContext|全局|


- jsp指令

![](https://img-blog.csdnimg.cn/20190104182754726.png)
- - page指令


![](https://img-blog.csdnimg.cn/20190104182754797.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3FxXzQwMDYxNDIx,size_16,color_FFFFFF,t_70)

![](https://img-blog.csdnimg.cn/20190104182754873.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3FxXzQwMDYxNDIx,size_16,color_FFFFFF,t_70)

基本上不会去手写，需要的话ide会自动导
- - include指令


![](https://img-blog.csdnimg.cn/20190104182754948.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3FxXzQwMDYxNDIx,size_16,color_FFFFFF,t_70)

用来引入静态html页面(也就是.html文件)
- - taglib指令


讲jstl的时候再说


- jsp动作标签
	- jsp:forword


![](https://img-blog.csdnimg.cn/20190104182754954.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3FxXzQwMDYxNDIx,size_16,color_FFFFFF,t_70)

本质就是向浏览器发起一个请求，url就是page的属性值
- - jsp:include


![](https://img-blog.csdnimg.cn/20190104182754995.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3FxXzQwMDYxNDIx,size_16,color_FFFFFF,t_70)

动态引入，说白了就是引.jsp文件的



总结: 引入.html文件用include指令，引入.jsp文件用jsp:include动作



