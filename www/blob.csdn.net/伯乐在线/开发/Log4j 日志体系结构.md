# Log4j 日志体系结构 - 文章 - 伯乐在线
本文作者： [伯乐在线](http://blog.jobbole.com) - [Float_Lu](http://www.jobbole.com/members/5334670325) 。未经作者许可，禁止转载！
欢迎加入伯乐在线 [专栏作者](http://blog.jobbole.com/99322)。
# 摘要
我们在写日志的时候首先要获取logger，在每一个使用log4j的项目都有很多个地方要获取logger，这些logger是真实的被实例化的Logger对象，他们有可能被分散在无数不同的类中，日志体系结构讲的是这些logger对象是如何组织的，他们之间又有什么样的关系。
# 体系结构
我们举个具体的实例来看看，假设我的项目包结构如下：
![](http://ww3.sinaimg.cn/mw690/7cc829d3gw1fa95kh8tbxj20bg05cglx.jpg)
项目结构
说明一下：com.flu.jdk包下面有两个类分别是LogTest1和LogTest2，然后在包com.flu包下面有一个LogTest3类，很显然，com.flu.jdk包是com.flu包的子包。假设我们在这三个类中分别通过LogManager.getLogger(xxx.class)获取三个logger实例，他们分别是logger1、logger2和logger3，我们将要讨论这三个logger的关系。
值得注意的是log4j的日志体系中，有一个特殊的日志对象叫做root（根），他是始终存在的，假设我们首先获取logger实例，log4j将构造下面这样一个图形（我不能把它叫做树）：
![](http://ww3.sinaimg.cn/mw690/7cc829d3gw1fa95ki5tt8j210o0t0mzg.jpg)
当只有logger1的时候
当我们获取logger2实例的时候，这个图将变成：
![](http://ww2.sinaimg.cn/mw690/7cc829d3gw1fa95kist6kj211q0swdiw.jpg)
当加入logger2日志实例时结构图
当我们获取logger3实例的时候，这个图又变了一个样，如下：
![](http://ww2.sinaimg.cn/mw690/7cc829d3gw1fa95kjlg0bj20ze0tkjv3.jpg)
当加入logger3日志实例之后
仅仅才三个日志实例，图就搞的略复杂，可想log4j应用中，将会有无数的日志实例按照这个规律组成纷繁复杂的有向图结构，虽然看起来杂乱，但是又规律。那么问题来了，这样的结构有什么用呢？下一节我们将会看到这种结构对于日志配置继承的影响。
# 配置继承
## log4j日志级别定义
在往下面看之前我们先来看看log4j对日志级别的定义：


```
public final static int OFF_INT = Integer.MAX_VALUE;
public final static int FATAL_INT = 50000;
public final static int ERROR_INT = 40000;
public final static int WARN_INT  = 30000;
public final static int INFO_INT  = 20000;
public final static int DEBUG_INT = 10000;
  //public final static int FINE_INT = DEBUG_INT;
public final static int ALL_INT = Integer.MIN_VALUE;
```
很显然，log4j的日志级别有下面的关系：


```
OFF > FATAL > ERROR > WARN > INFO > DEBUG > ALL
```
log4j在写日志的时候只有当当前写日志的级别大于等于当前日志实例的配置级别的时候，日志写操作才生效，比如当前日志实例的配置级别为INFO，那么log.info会写成功，而log.debug则不会写。
## 日志写源码剖析
我们来看看一句简单的log.info(“this is log message”)的背后，先来看看一段源代码：


```
public void info(Object message) {
  if(repository.isDisabled(Level.INFO_INT))
    return;
  if(Level.INFO.isGreaterOrEqual(this.getEffectiveLevel()))
    forcedLog(FQCN, Level.INFO, message, null);
}
public boolean isDisabled(int level) {
  return thresholdInt > level;
}
```
首先看看当前写的日志级别是否被禁止的，默认的情况下thresholdInt为ALL，因此INFO的级别显然比ALL大，因此下面会继续看看INFO的级别是否大于等于当前日志实例生效的级别，this.getEffectiveLevel()获取的实例是什么呢？我们继续看看代码：


```
public Level getEffectiveLevel() {
   for(Category c = this; c != null; c=c.parent) {
     if(c.level != null)
		return c.level;
   }
   return null; // If reached will cause an NullPointerException.
 }
```
当前日志生效的级别逻辑为首先看看当前日志实例是否有配置级别，如果没有，那么就继续找当前日志实例的parent节点，按照上一节中所表述的，如果当前日志的日志级别没有配置，当找到root的日志级别，并根据root的日志级别来断定是否继续进行日志写。这里提现了日志级别的继承关系，其实不仅仅是日志级别，日志其他相关的配置也会基于这种继承的特性，比如appender等。
# 项目应用
了解Log4j的日志体系结构以及日志写与日志级别关系之后，我们现在应该比较清楚项目中应该如何配置了。以Log4j.xml配置文件为例子，满足基本需求我们只需要配置root这个日志实例的日志级别即可，如下：


```
<root>
    <level value="INFO" />
    <appender-ref ref="CONSOLE" />
</root>
```
上面配置了root日志实例的日志级别为INFO，如果获取按照一定规范（当前类的权限定名作为日志实例名），那么我们可以保证所有的日志实例将继承root所配置的日志级别。
# 配置隔离
上面的配置略粗糙，假如我们想为不同的模块给予不同的配置怎么办呢？最常见的是业务日志与中间件日志，比如我们的业务业务包名为com.dianping.biz，而我们的rpc组件的包名字为com.dianping.pigeon，则我们可以使用下面方法给予不同的模块不同的配置：


```
<!--业务日志配置-->
<category name="com.dianping.biz">
    <level value="INFO" />
    <appender-ref ref="CONSOLE" />
</category>
<!--pigeon组件日志配置-->
<category name="com.dianping.pigeon">
    <level value="DEBUG" />
    <appender-ref ref="CONSOLE" />
</category>
```
通过上面的配置，我们可以指定com.dianping.biz包下面所有类获取的logger都继承name为com.dianping.biz的日志配置，而com.dianping.pigeon包下面的所有类获取的logger都继承name为com.dianping.pigeon的日志篇日志。不过通常设计良好的中间件都定制了日志配置以确保中间件日志与业务日志隔离。
> 
**打赏支持我写出更多好文章，谢谢！**
[打赏作者](#rewardbox)
#### 打赏支持我写出更多好文章，谢谢！
任选一种支付方式
![](http://jbcdn2.b0.upaiyun.com/2016/05/7cb05a1a5b9ef06cc9d40cc79a096f293.png)![](http://jbcdn2.b0.upaiyun.com/2016/05/12e72c4df391cc981614cc68aedd44085.jpg)
