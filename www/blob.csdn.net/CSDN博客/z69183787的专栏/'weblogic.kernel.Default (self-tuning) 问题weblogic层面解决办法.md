# 'weblogic.kernel.Default (self-tuning) 问题weblogic层面解决办法 - z69183787的专栏 - CSDN博客
2013年10月12日 09:53:35[OkidoGreen](https://me.csdn.net/z69183787)阅读数：11063
声明：出现这个问题有程序方面、网络方面、weblogic设置方面等等原因，此文章主要讲述由于weblogic设置而导致的解决办法。
因为：
1.程序问题，需要项目自己去解决，weblogic在做优化处理也于事无补。
2.网络中断或者认为关闭交互这种情况也不能用weblogic处理（这点我是这么认为的）
一、说明：
,"weblogic.kernel.Default"是从客户端提交请求后产生的线程所在的队列名。这个队列的线程数默认是15个。如果超过15个线程堵塞，则部署的应用将不能访问。同时后台报：
<2008-2-27 下午09时37分48秒 CST> <Error> <WebLogicServer> <BEA-000337> <ExecuteThread: '14' for queue: 'weblogic.kernel.Default' has been busy for "1,720" seconds working on the request "Http Request: /myapp/test/index.jsp", which is more than the configured time
 (StuckThreadMaxTime) of "600" seconds.> 
2，线程数(Tread Count)：指派到weblogic.kernel.Default队列的线程数。如果你不需要使用超过15个线程（默认），就不必更改这个属性值。
如果发送该请求较多，很有可能会导致weblogic的线程阻塞，严重会引起weblogic挂起现象。
可以通过以下几种方法解决： 
1）修改StuckThreadMaxTime参数，将默认的600s改成1200s，或者其它适合的值。
2）增大线程数，防止线程阻塞问题。
3）优化程序，减少处理时间。
二、修改办法
------------------------------------------↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓修改办法↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓---------------------------
1） 如何修改StuckThreadMaxTime参数值：[http://lujinan858.iteye.com/blog/986237](http://lujinan858.iteye.com/blog/986237)
启动weblogic服务，进入控制台：
your_domain->Environment->Servers->your_server->Configuration->Tuning->Stuck Thread Max Time
如下图：
![](https://img-my.csdn.net/uploads/201204/09/1333954291_6311.PNG)
2）怎样增大线程数
window环境下修改【bea】\user_projects\domains\my_domain\bin\setDomainEnv.cmd文件，查询最下面set JAVA_OPTIONS=%JAVA_OPTIONS%
改为：
set JAVA_OPTIONS=%JAVA_OPTIONS% -Dweblogic.threadpool.MinPoolSize=50
set JAVA_OPTIONS=%JAVA_OPTIONS% -Dweblogic.threadpool.MaxPoolSize=300
