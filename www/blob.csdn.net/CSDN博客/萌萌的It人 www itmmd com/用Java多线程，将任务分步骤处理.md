
# 用Java多线程，将任务分步骤处理 - 萌萌的It人 www.itmmd.com - CSDN博客


2015年04月11日 10:00:08[Jlins](https://me.csdn.net/dyllove98)阅读数：3818


场景简单，其实就是在做WEB请求的时候，处理的结果需要同时通知到一个第三方服务器，通知后再把结果返回给客户端。
现在这个通知只是一个简单通知，如果直接加在客户端请求里面，客户端会在请求第三方服务器时堵塞。另外为了客户端不能超时，所以通知失败后也不敢多次请求。
这里可以使用定制任务来解决这个问题，一个客户端请求后产生一个定制的任务，然后服务后台进行多线程的异步处理，这样就会大大减少客户端的请求时间，同样最大程度保证这个通知是及时成功给第三方的。
定制任务我们需要一张表，来存储任务。同时需要多线程来执行这些任务。至于谁来开启这些任务，你可以使用定时器：[http://javacui.com/framework/24.html](http://javacui.com/framework/24.html)，也可以使用后台线程。因为定时器已经有示例了，这里来说下后台线程。
因为是WEB服务，在服务启动的时候注册一个启动监听：
[?](http://javacui.com/Theory/321.html#)
|1
|2
|3
|4
|5
|6
|<!-- 初始化系统需要的配置项 开始 -->
|<|listener|>
|<|display-name|>webListener</|display-name|>
|<|listener-class|>cn.com.vogue.listener.InitListener</|listener-class|>
|</|listener|>
|<!-- 初始化系统需要的配置项 结束 -->
|

然后我们来实现这个监听处理，主要任务就是开启一个后台线程，实时查询是否有需要执行的任务，有的话就分发任务给子线程。
同时为了系统的可配性，这个主线程是否开启做一个配置项，加载的时候判断一下即可：
[?](http://javacui.com/Theory/321.html#)
|1
|isRunRulThread=0
|

[?](http://javacui.com/Theory/321.html#)
|1
|2
|3
|4
|5
|6
|7
|8
|9
|10
|11
|12
|13
|14
|15
|16
|17
|18
|19
|20
|21
|22
|import|javax.servlet.ServletContextEvent;
|import|javax.servlet.ServletContextListener;
|import|org.slf4j.Logger;
|import|org.slf4j.LoggerFactory;
|import|cn.com.vogue.task.UrlTask;
|import|cn.com.vogue.utils.ResourceUrl;
|import|cn.com.vogue.utils.SpringFactory;
|/**
|* 初始化
|*/
|public|class|InitListener|implements|ServletContextListener {
|private|static|Logger logger = LoggerFactory.getLogger(InitListener.|class|);
|public|void|contextDestroyed(ServletContextEvent sce) {
|}
|public|void|contextInitialized(ServletContextEvent sce) {
|if|(ResourceUrl.isRunRulThread ==|1|){
|logger.warn(|"==========================>>>>>>>URL请求定制任务开启"|);
|UrlTask urlTaskBase =|new|UrlTask();|// 开始任务
|SpringFactory.executorService.execute(urlTaskBase);
|}
|}
|}
|

这里我在SpringFactory里面定义了一个线程池，关于线程池：[http://javacui.com/Theory/151.html](http://javacui.com/Theory/151.html)。
为了不对对方服务器造成压力，任务子线程最多开10个，加上主线程，一共11个，所以线程池不用太大。
[?](http://javacui.com/Theory/321.html#)
|1
|2
|public|static|ExecutorService executorService= Executors.newFixedThreadPool(|11|);
|// 定长线程池
|

主线程任务：查询是否有需要执行的任务，任务分发。
子线程：处理任务。
[?](http://javacui.com/Theory/321.html#)
|1
|2
|3
|4
|5
|6
|7
|8
|9
|10
|11
|12
|13
|14
|15
|16
|17
|18
|19
|20
|21
|22
|23
|24
|25
|26
|27
|28
|29
|30
|31
|32
|33
|34
|35
|36
|37
|38
|39
|40
|41
|42
|43
|44
|45
|46
|47
|48
|49
|50
|51
|52
|53
|54
|55
|56
|57
|58
|59
|60
|61
|62
|63
|64
|65
|66
|67
|68
|69
|70
|71
|72
|import|java.util.List;
|import|org.slf4j.Logger;
|import|org.slf4j.LoggerFactory;
|import|cn.com.vogue.entity.UrlTaskBase;
|import|cn.com.vogue.service.systemManagement.UrlTaskBaseService;
|import|cn.com.vogue.utils.HttpUtils;
|import|cn.com.vogue.utils.SpringFactory;
|public|class|UrlTask|implements|Runnable{
|private|static|Logger logger = LoggerFactory.getLogger(UrlTask.|class|);
|public|static|Integer isRun =|new|Integer(|0|);
|// 该标量用于标记子线程在运行时，总线程不再派发新任务
|public|void|run() {
|while|(|true|){
|try|{
|UrlTaskBaseService taskSer = (UrlTaskBaseService)SpringFactory.getObject(|"urlTaskBaseService"|);
|List<UrlTaskBase> taskList = taskSer.findUrlTaskBaseUnSucc();
|if|(|null|!= taskList && taskList.size() >|0|){
|logger.warn(|"本次需要执行的任务个数："|+ taskList.size() +|" 个"|);
|for|(UrlTaskBase task : taskList){
|Init1ListenerRun run =|new|Init1ListenerRun(task, taskSer);
|SpringFactory.executorService.execute(run);
|try|{Thread.sleep(|1000|);}|catch|(Exception e) {}
|}
|}|else|{
|logger.warn(|"没有找到要执行的任务，休眠后继续执行"|);
|}
|}|catch|(Exception e) {
|logger.error(|"查询执行任务时异常"|, e);
|}
|while|(isRun >|0|){
|try|{Thread.sleep(|1|*|1000|);}|catch|(Exception e) {}
|}
|logger.error(|"60秒后进行下一次任务轮询"|);
|try|{Thread.sleep(|60|*|1000|);}|catch|(Exception e) {}|// 任务休眠60秒
|}
|}
|}
|class|Init1ListenerRun|implements|Runnable {
|private|static|Logger logger = LoggerFactory.getLogger(Init1ListenerRun.|class|);
|public|void|run() {
|synchronized|(UrlTask.isRun) { UrlTask.isRun ++; }
|try|{
|logger.warn(task.getId() +|" 任务路径："|+ task.getTaskUrl());
|if|(task.getTaskType() ==|0|){
|String result = HttpUtils.HttpGet(task.getTaskUrl());
|logger.warn(task.getId() +|" 返回结果："|+ result);
|if|(|null|!= result && !|""|.equals(result)){
|task.setBackStr(result);
|}
|task.setTryCount(task.getTryCount() +|1|);
|taskSer.save(task);
|}|else|{
|String result = HttpUtils.HttpPost(task.getTaskUrl(), task.getTaskBody());
|logger.warn(task.getId() +|" 返回结果："|+ result);
|if|(|null|!= result && !|""|.equals(result)){
|task.setBackStr(result);
|}
|task.setTryCount(task.getTryCount() +|1|);
|taskSer.save(task);
|}
|}|catch|(Exception e) {
|logger.error(|"执行任务异常"|, e);
|}
|synchronized|(UrlTask.isRun) { UrlTask.isRun --; }
|}
|private|UrlTaskBase task;
|private|UrlTaskBaseService taskSer;
|public|Init1ListenerRun(UrlTaskBase task, UrlTaskBaseService taskSer){
|this|.task = task;
|this|.taskSer = taskSer;
|}
|}
|

这里用一个标量来标记有子线程在执行，主线程则休眠。
用对象来传递需要执行的任务，实体代码如下：
[?](http://javacui.com/Theory/321.html#)
|1
|2
|3
|4
|5
|6
|7
|8
|9
|10
|11
|12
|13
|14
|15
|16
|public|class|UrlTaskBase|extends|IdEntity{
|// 请求的地址，如果是Get，则带参数
|private|String taskUrl;
|// 请求类型 0 GET 1 POST
|private|int|taskType;
|// POST 请求参数
|private|String taskBody;
|// 成功的标记
|private|String okMark;
|// 实际返回结果，和 okMark 一致为成功
|private|String backStr;
|// 尝试次数
|private|int|tryCount;
|// 创建时间
|private|String createTime;
|}
|

任务分为GET请求和POST请求两种。
更多信息可参考：

## [IE6 IE7 textarea height 100% 无效的问题](http://www.itmmd.com/201504/691.html)
## [jquery.html()方法再加载带有js的htm内容的时候是可以保证js的执行顺序的](http://www.itmmd.com/201504/689.html)
## [it新员工如何快速学习](http://www.itmmd.com/201503/675.html)



