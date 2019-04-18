# JMeter压力测试总结 - z69183787的专栏 - CSDN博客
2015年07月23日 11:46:25[OkidoGreen](https://me.csdn.net/z69183787)阅读数：4649
[Apache](http://blog.linuxeye.com/tag/apache)[JMeter](http://blog.linuxeye.com/tag/jmeter)是[Apache](http://blog.linuxeye.com/tag/apache)组织开发的基于Java的压力测试工具。用于对软件做压力测试，相比其他HTTP测试工具,JMeter最主要的特点在于扩展性强。JMeter能够自动扫描其lib/ext子目录下.jar文件中的[插件](http://baike.baidu.com/view/18979.htm)，并且将其装载到内存，让用户通过不同的菜单调用。并且能自动生成压力测试报告。
**1. 下载JMeter**
[http://jmeter.apache.org/download_jmeter.cgi](http://jmeter.apache.org/download_jmeter.cgi)
我用的是JMeter 2.9 r1437961 ,JDK 7u40
**2.启动JMeter**
运行bin/jmeter.bat
**3.添加线程组**
**测试计划->添加->Threads(users)->线程组**
![](https://img-blog.csdn.net/20130929182335453)
线程数:要模拟的并发用户量。
Ramp Up Period (in seconds)：在多长时间内均匀启动所有的线程。比如Number of Threads设为3000，Ramp Up Period设为300，则jmeter每隔0.1秒启动1个线程。
循环次数:单用户任务重复执行的次数。可以设为永远，这样jmeter就不会自动停止，需要强制终止。
**线程组->添加->sampler->Java请求**
![](https://img-blog.csdn.net/20130929182658671)
把我们基于Jmeter 的JAVA请求选中，ShootRequest,向鱼群发射炮弹的业务逻辑.
其中界面上的参数为程序中指定的，比如服务器IP端口之类的。
**4.实现Java Request**
建立JAVA工程，引入库ApacheJMeter_core.jar，ApacheJMeter_java.jar，实现个抽象类,AbstractJmeterClient，所有的请求必须在玩家登陆的前提下，所以登陆逻辑放到了公共的抽象类:
- package com.u9.jrobot;  
- publicabstractclass AbstractJmeterClient extends AbstractJavaSamplerClient {  
- privatestaticfinal Logger logger = LogManager  
-             .getLogger(AbstractJmeterClient.class);  
- private String tableIp = "172.16.1.37";  
- privateint port = 1470;  
- private Player player;  
- private String lobbyIp="172.16.1.34";  
- private SampleResult result;  
- protectedstaticint robotId = 0;  
- 
- // private String path;
- // private
- // 设置传入的参数，可以设置多个，已设置的参数会显示到Jmeter的参数列表中
- public Arguments getDefaultParameters() {  
-         Arguments args = new Arguments();  
-         args.addArgument("lobbyIp", lobbyIp);  
-         args.addArgument("tableIp", tableIp);  
-         args.addArgument("port", "" + port);  
-         args.addArgument("startId", "" + robotId);  
- return args;  
-     }  
- 
- // 初始化方法，实际运行时每个线程仅执行一次，在测试方法运行前执行
- publicvoid setupTest(JavaSamplerContext context) {  
- // 加载当前目录下的logback配置文件
-         result = new SampleResult();  
-         result.sampleStart(); // 事务的起点
-         tableIp = context.getParameter("tableIp");  
-         port = context.getIntParameter("port");  
-         lobbyIp = context.getParameter("lobbyIp");  
- 
- if (robotId == 0) {  
-             robotId = context.getIntParameter("startId");  
-         }  
-         String name = "robot" + robotId++;  
-         Map<String, Object> map = HttpManager.getInstance().login(lobbyIp,  
-                 name, "qqqqqq");  
- int retcode = (Integer) map.get("s");  
- if (retcode == 1) {  
- //玩家登陆游戏的逻辑
-         }  
- //  JobManager.getInstance().start();
-     }  
- 
- @Override
- // 测试执行的循环体，根据线程数和循环次数的不同可执行多次
- public SampleResult runTest(JavaSamplerContext arg) {  
- boolean success = true;  
- // result.sampleStart(); // 事务的起点
- try {  
-             result.setSuccessful(this.runTest(player));  
-         } catch (Exception e) {  
-             success = false;  
-         } finally {  
- // result.sampleEnd(); // 事务的终点
-             result.setSuccessful(success); // 设置本次事务成功或失败
-         }  
- return result;  
-     }  
- 
- publicabstractboolean runTest(Player player) throws Exception;  
- 
- // 结束方法，实际运行时每个线程仅执行一次，在测试方法运行结束后执行
- publicvoid teardownTest(JavaSamplerContext context) {  
-         result.sampleEnd(); // 事务的终点
-         robotId = context.getIntParameter("startId");  
-         GameClient gameClient = player.getGameClient();  
- try {  
-             PlayerManager.getInstance().remove(gameClient.getChannelId());  
-             gameClient.disconnect();  
-         } catch (ServiceException e) {  
-         }  
-     }  
- }  
实现具体的压力测试类:
- publicclass ShootRequest extends AbstractJmeterClient {  
- 
- privatestaticfinal Logger logger = LogManager  
-             .getLogger(ShootRequest.class);  
- privateint[] array = { 1, 5, 10, 20, 30, 40, 50, 100 };  
- privatestaticint token = 0;  
- 
- @Override
- publicboolean runTest(Player player) throws Exception {  
- 
- // KeepAlive_S2C_Msg ret=client.keepAlive(msg.build());
- <span style="white-space:pre">      </span>//具体射击逻辑实现
- returntrue;  
-     }  
- }  
用ant打包成jrobot.jar,不要设主类，jrobot.jar和lib文件放到\apache-jmeter-2.9\lib\ext\下，然后启动，选择相应的压力测试类，设置参数，启动JMeter
**5.添加Listener**
//TODO
ps:每个玩家都要单独启动个线程，如果3000玩家就要启动3000个线程组，感觉这设计有点郁闷，程序的很多消耗都在切换线程了。是不是我理解或者用错了？
版权声明：本文为博主原创文章，未经博主允许不得转载。
