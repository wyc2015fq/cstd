# log4j1 如何平滑升级到log4j2呢 - z69183787的专栏 - CSDN博客
2017年07月10日 11:28:18[OkidoGreen](https://me.csdn.net/z69183787)阅读数：2102
如题，log4j1 如何平滑升级到log4j2呢？  原来使用Logger.getLogger方法获取到org.apache.log4j.Logger对象的 ，现在LogManager.getLogger 方法获取到org.apache.logging.log4j.Logger  
知道了
   有一个专门的jar包  log4j-1.2-api-2.0.jar ，导入就可以了，原来代码没有影响。
官网适配器：[http://logging.apache.org/log4j/2.x/log4j-1.2-api/](http://logging.apache.org/log4j/2.x/log4j-1.2-api/)
# Log4j 1.2 Bridge
The Log4j 1.2 Bridge allows applications coded to use Log4j 1.2 API to use Log4j 2 instead.
## Requirements
The Log4j 1.2 bridge is dependent on the Log4j 2 API and implementation. For more information, see [Runtime
 Dependencies](http://logging.apache.org/log4j/2.x/runtime-dependencies.html).
## Usage
To use the Log4j Legacy Bridge just remove all the Log4j 1.x jars from the application and replace them with the bridge jar. Once in place all logging that uses Log4j 1.x will be routed to Log4j 2. However, applications
 that attempt to modify legacy Log4j by adding Appenders, Filters, etc may experience problems if they try to verify the success of these actions as these methods are largely no-ops.
