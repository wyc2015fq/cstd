# 面试|大数据中无处不在的ServiceLoader - Spark高级玩法 - CSDN博客
2018年12月27日 00:01:25[Spark高级玩法](https://me.csdn.net/rlnLo2pNEfx9c)阅读数：47
ServiceLoader是SPI的是一种实现，所谓SPI，即Service Provider Interface，用于一些服务提供给第三方实现或者扩展，可以增强框架的扩展或者替换一些组件。 
要配置在相关项目的固定目录下：
resources/META-INF/services/接口全称。
这个在大数据的应用中颇为广泛，比如Spark2.3.1 的集群管理器插入：
private def getClusterManager(url: String): Option[ExternalClusterManager] = {
    val loader = Utils.getContextOrSparkClassLoader
    val serviceLoaders =
      ServiceLoader.load(classOf[ExternalClusterManager], loader).asScala.filter(_.canCreate(url))
if (serviceLoaders.size > 1) {
thrownew SparkException(
        s"Multiple external cluster managers registered for the url $url: $serviceLoaders")
    }
    serviceLoaders.headOption
  }
配置是在
![640?wx_fmt=png](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_png/adI0ApTVBFVAmLyicfL1We292GzoicEXjpiacxvqSIXG3nrjaBjPBb5oJWZhblCwNET8A5r2uYdvbZ0MaQOaHShNQ/640?wx_fmt=png)
spark sql数据源的接入，新增数据源插入的时候可以采用这种方式，要实现的接口是DataSourceRegister。
**简单测试**
首先实现一个接口
package bigdata.spark.services;
publicinterfaceDoSomething{
public String shortName(); //可以制定实现类名加载
publicvoiddoSomeThing();
}
然后将接口配置在resources/META-INF/services/
![640?wx_fmt=png](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_png/adI0ApTVBFVAmLyicfL1We292GzoicEXjp0z2r9YV0ic1aM6ljfPYYwcMQespMhUKkvMOQ5ibWLicWLt3bmwNdwKecA/640?wx_fmt=png)
实现该接口
package bigdata.spark.services;
publicclassSayHelloimplementsDoSomething{
@Override
public String shortName(){
return"SayHello";
    }
@Override
publicvoiddoSomeThing(){
        System.out.println("hello !!!");
    }
}
测试
package bigdata.spark.services;
import java.util.ServiceLoader;
publicclasstest{
static ServiceLoader<DoSomething> loader = ServiceLoader.load(DoSomething.class);
publicstaticvoidmain(String[] args){
for(DoSomething sayhello : loader){
         //要加载的类名称我们可以制定
if(sayhello.shortName().equalsIgnoreCase("SayHello")){
                sayhello.doSomeThing();
            }
        }
    }
}
输出结果如下：
![640?wx_fmt=png](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_png/adI0ApTVBFVAmLyicfL1We292GzoicEXjpQfia0IZFIRTtTibUy3AtzQoapL43deaSQw0XkEXhRIpyic8KpAWWF3ydA/640?wx_fmt=png)
这个主要是为讲自定义数据源作准备。
欢迎订阅浪尖知识星球，跟450人一起学习。
推荐阅读：
[面试|海量文本去重～minhash](http://mp.weixin.qq.com/s?__biz=MzA3MDY0NTMxOQ==&mid=2247486153&idx=1&sn=b0fe21f7b07bbeaa50c14a848b71a25f&chksm=9f38e9e1a84f60f720b7f03cdae6b693092cd4aeaca33d4721fff4502d0c0eeb855f2b873649&scene=21#wechat_redirect)
[kafka消费者分组消费的再平衡策略](http://mp.weixin.qq.com/s?__biz=MzA3MDY0NTMxOQ==&mid=2247486159&idx=1&sn=ddd255b089f3052dec3aacfbcd5dd8ea&chksm=9f38e9e7a84f60f18a84d77f7788074aa36ba130dc292b62023e139b0c136ff1018c7bc68dd8&scene=21#wechat_redirect)
[面试中常问的List去重问题，你都答对了吗？](http://mp.weixin.qq.com/s?__biz=MzA3MDY0NTMxOQ==&mid=2247486091&idx=1&sn=fd604bc06167adb8c74080d22266f5b2&chksm=9f38e9a3a84f60b5aaf3110171c043696e7d4a28eada6ba8f2875010ecef159b54626f396198&scene=21#wechat_redirect)
![640?wx_fmt=jpeg](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_jpg/adI0ApTVBFUibDu2gaX89M600Ip3MDrVY0ibqsWg1gHuTdGCX1QfbyyCPlf31gLQna9OvNyRsBaByXsZrZeqibtVQ/640?wx_fmt=jpeg)
