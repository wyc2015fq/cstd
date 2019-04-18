# pageContext.pushBody() getOutputStream方法冲突 - z69183787的专栏 - CSDN博客
2013年10月24日 11:41:45[OkidoGreen](https://me.csdn.net/z69183787)阅读数：5784
out.clear(); 
out = pageContext.pushBody(); 
复制代码out = pageContext.pushBody(); 是什么意思？为什么不加入了就报异常: java.lang.IllegalStateException: getOutputStream() has already been called for this response 
复制代码 
首先你要知道pushBody()的作用是保存当前的out对象，并更新PageContext中Page范围内Out对象。至于为什么要加上这句话，是因为JSP容器在处理完成请求后会调用releasePageConter方法释放所有的PageContestObject，并且同时调用getWriter方法。由于getWriter方法与在JSP页面中使用流相关的getOutputStream方法冲突，所以会造成这种异常，解决方法就是楼上给的一样，只需要在JSP页面的最后加上这两条语句。out.clear(); 
out = pageContext.pushBody(); 
