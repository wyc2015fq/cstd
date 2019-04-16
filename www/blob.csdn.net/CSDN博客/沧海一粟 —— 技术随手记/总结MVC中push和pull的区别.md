# 总结MVC中push和pull的区别 - 沧海一粟 —— 技术随手记 - CSDN博客





2012年11月22日 15:22:30[慢游](https://me.csdn.net/eengel)阅读数：3096








讨论MVC框架中的push和pull机制，主要是理一下MVC中View和Controller之间的关系。

到底push和pull有什么区别？由于不同MVC框架的具体实现不同，这个问题还没有统一的答案。这儿总结了几篇文章[1-3]及其评论中的主要观点，做成表格列举如下。


|**Push**|**Pull**|
|----|----|
|Controller解释用户行为，产生相应的data并推送到View上|假设用户需要某种输出，因此View去访问Controller，拿到所需data后呈现给用户|
|适用于执行（用户）行为和任务e.g. 添加用户评价|适用于响应显示data的需求e.g. 显示客户信息及购物清单|
|通常涉及一个Controller及对应的View|通常涉及一个View和多个Controller|
|推送专门为某个View的datae.g. JSP中用tag取得data|推送各个View都可以用的datae.g. JSP中从Context中取得data|
|更好地分离View|分离View做的不太好|





参考文献：

[1] Pull vs. Push MVC Architecture, [http://www.guyrutenberg.com/2008/04/26/pull-vs-push-mvc-architecture/](http://www.guyrutenberg.com/2008/04/26/pull-vs-push-mvc-architecture/)(中文版[http://daur.iteye.com/blog/1384892](http://daur.iteye.com/blog/1384892)).

[2] Clarification on MVC Pull and MVC Push,[http://www.theserverside.com/discussions/thread.tss?thread_id=22143](http://www.theserverside.com/discussions/thread.tss?thread_id=22143)

[3] Web application framework, [http://en.wikipedia.org/wiki/Web_application_framework#Push-based_vs._pull-based](http://en.wikipedia.org/wiki/Web_application_framework#Push-based_vs._pull-based)




