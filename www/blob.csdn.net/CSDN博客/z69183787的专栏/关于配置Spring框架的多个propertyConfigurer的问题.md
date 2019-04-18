# 关于配置Spring框架的多个propertyConfigurer的问题 - z69183787的专栏 - CSDN博客
2018年05月09日 11:38:01[OkidoGreen](https://me.csdn.net/z69183787)阅读数：583
[https://blog.csdn.net/aa427/article/details/38375259](https://blog.csdn.net/aa427/article/details/38375259)
近来对web架构中的缓存比较感兴趣 ，所以分别试着在windows环境下配起了redis和memcache，刚开始配了Memcache，跑起项目不存在问题（项目框架是SpringMvc+MyBatis），我的Spring的配置文件目录如下
![](https://img-blog.csdn.net/20140804211159326?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvQUE0Mjc=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)
从web项目中启动加载applicationContext.xml,这本来是一个比较简单的配置（在配置memcache和redis之前是不存在问题的），后来为了配置memcache，所以在web.xml中我又启动了,web.xml的配置又加了一个xml，如下图![](https://img-blog.csdn.net/20140804211737113?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvQUE0Mjc=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)
在Spring-memcache.xml中又配置了propertyConfigure
![](https://img-blog.csdn.net/20140804211648687?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvQUE0Mjc=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)
启动项目，目前没有问题，但是接下来我配置redis的时候也差不多按照这个思路配置，所以在web.xml中又加上了spring-redis.xml  
![](https://img-blog.csdn.net/20140804212117198?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvQUE0Mjc=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)
而在spring-redis.xml中又配置了一个propertyConfigure
![](https://img-blog.csdn.net/20140804212502915?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvQUE0Mjc=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)
启动项目，本来觉得这样的配置应该不会出现问题，没想到报错了，竟然出问题了
![](https://img-blog.csdn.net/20140804212741661?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvQUE0Mjc=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)
说是不能替换，左想右想，这不对啊   明显的properties文件里面的东西属性是一样的  这不可能出错啊。终于找到资源了
http://www.iteye.com/topic/1131688
http://blog.sina.com.cn/s/blog_4550f3ca0100ubmt.html
查阅资料  原来是先加载了spring-memcache.xml  然后先把
![](https://img-blog.csdn.net/20140804214747761?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvQUE0Mjc=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)
但是spring是要把3个xml都加载完的 所以上面的memcache.propertis里面的配置在还没有读取出来的时候，spring容器开始要替换里面的表达式，所以出现这么个问题（简单来说就是最好把properties文件全部在一个propertiesConfigure加载）
现在来说解决方案
1）在applicationcontext.xml中进行配置文件的集合，
![](https://img-blog.csdn.net/20140804215101562?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvQUE0Mjc=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)
在各个分配置中，不再配置propertyConfigure了，这样就解决了这么个问题
2）如果觉得还是想非要分别配置更好理解的话
那么在各个propertyConfigure加上“orderr”和“ignoreUnresolvablePlaceholders”，order表示顺序，值越小优先级越高即越早执行；
默认情况下ignore-unresolvable；即如果没找到的情况是否抛出异常。默认false：即抛出异常；
![](https://img-blog.csdn.net/20140804220407771?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvQUE0Mjc=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)
![](https://img-blog.csdn.net/20140804220241875?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvQUE0Mjc=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)
这么配置的意思是先加载memcache.properties文件，在替换表达式的时候不会报错，等待以后的表达式替换（因为配置了ignoreUnresolvablePlaceh=true），则等待加载了redis.propertie文件的时候把其余的表达式给替换掉
（这样配置的要求在于order最大的的ignoreUnresolvablePlace为false，其余的都必须为true，因为先加载的在替换的时候不能报错，等待后面把表达式替换掉）
第二个问题：
![](https://img-blog.csdn.net/20140804221018053?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvQUE0Mjc=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)
这解决了上次配置框架的时候出现的问题，终于无意中找到了答案，在此记录下
