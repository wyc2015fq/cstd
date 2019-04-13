
# Memcached常用命令及使用说明 -  芝麻软件工作室的专栏 - CSDN博客


2016年07月10日 11:55:04[seven-soft](https://me.csdn.net/softn)阅读数：1420个人分类：[memcache																](https://blog.csdn.net/softn/article/category/6304643)



## 一、存储命令
存储命令的格式：
|1
|2
|<command
 name> <key> <flags> <exptime> <bytes>
|<data
 block>
|

参数说明如下：
|<command name>|set/add/replace|
|<key>|查找关键字|
|<flags>|客户机使用它存储关于键值对的额外信息|
|<exptime>|该数据的存活时间，0表示永远|
|<bytes>|存储字节数|
|<data block>|存储的数据块（可直接理解为key-value结构中的value）|
### 1、添加
(1)、无论如何都存储的set
![set](http://images.cnblogs.com/cnblogs_com/jeffwongishandsome/201111/201111061815076698.jpg)
这个set的命令在memcached中的使用频率极高。set命令不但可以简单添加，如果set的key已经存在，该命令可以更新该key所对应的原来的数据，也就是实现更新的作用。
可以通过“get 键名”的方式查看添加进去的记录：
![set_get](http://images.cnblogs.com/cnblogs_com/jeffwongishandsome/201111/201111061815075236.jpg)
如你所知，我们也可以通过delete命令删除掉，然后重新添加。
![delete](http://images.cnblogs.com/cnblogs_com/jeffwongishandsome/201111/201111061815084647.jpg)
(2)、只有数据**不存在**时进行添加的add
![add](http://images.cnblogs.com/cnblogs_com/jeffwongishandsome/201111/201111061815089597.jpg)
(3)、只有数据**存在**时进行替换的replace
![replace](http://images.cnblogs.com/cnblogs_com/jeffwongishandsome/201111/2011110618150988.jpg)

### 2、删除
![delete](http://images.cnblogs.com/cnblogs_com/jeffwongishandsome/201111/201111061815098942.jpg)
可以看到，删除已存在的键值和不存在的记录可以返回不同的结果。

## 二、读取命令
### 1、get
get命令的key可以表示一个或者多个键，键之间以空格隔开
![get](http://images.cnblogs.com/cnblogs_com/jeffwongishandsome/201111/201111061815106924.jpg)
### 2、gets
![gets](http://images.cnblogs.com/cnblogs_com/jeffwongishandsome/201111/201111061815109366.jpg)
可以看到，gets命令比普通的get命令多返回了一个数字（上图中为13）。这个数字可以检查数据是否发生改变。当key对应的数据改变时，这个多返回的数字也会改变。
### 3、cas
cas即checked and set的意思，只有当最后一个参数和gets所获取的参数匹配时才能存储，否则返回“EXISTS”。
![cas](http://images.cnblogs.com/cnblogs_com/jeffwongishandsome/201111/201111061815118220.jpg)

## 三、状态命令
### 1、stats
![stats](http://images.cnblogs.com/cnblogs_com/jeffwongishandsome/201111/201111061815112614.jpg)

### 2、stats items
![statsitems](http://images.cnblogs.com/cnblogs_com/jeffwongishandsome/201111/201111061815123661.jpg)
执行stats items，可以看到STAT items行，如果memcached存储内容很多，那么这里也会列出很多的STAT items行。

### 3、stats cachedump slab_id limit_num
我们执行stats cachedump 1 0 命令效果如下：
![statscachedump](http://images.cnblogs.com/cnblogs_com/jeffwongishandsome/201111/201111061815122200.jpg)
这里slab_id为1，是由2中的stats items返回的结果（STAT items后面的数字）决定的；limit_num看起来好像是返回多少条记录，猜的一点不错， 不过0表示显示出所有记录，而n(n>0)就表示显示n条记录，如果n超过该slab下的所有记录，则结果和0返回的结果一致。
![statscachedump1](http://images.cnblogs.com/cnblogs_com/jeffwongishandsome/201111/201111061815136594.jpg)
通过stats items、stats cachedump slab_id limit_num配合get命令可以遍历memcached的记录。

### 4、其他stats命令
如stats slabs,stats sizes,stats reset等等使用也比较常见。
![statsother](http://images.cnblogs.com/cnblogs_com/jeffwongishandsome/201111/20111106181513988.jpg)

## 四、其他常见命令
### 1、append
![append](http://images.cnblogs.com/cnblogs_com/jeffwongishandsome/201111/201111061815148970.jpg)
在现有的缓存数据后添加缓存数据，如现有缓存的key不存在服务器响应为NOT_STORED。

### 2、prepend
和append非常类似，但它的作用是在现有的缓存数据前添加缓存数据。
![prepend](http://images.cnblogs.com/cnblogs_com/jeffwongishandsome/201111/20111106181514332.jpg)

### 3、flush_all
![flush_all](http://images.cnblogs.com/cnblogs_com/jeffwongishandsome/201111/201111061809104896.jpg)
该命令有一个可选的数字参数。它总是执行成功，服务器会发送 “OK\r\n” 回应。它的效果是使已经存在的项目立即失效（缺省），或在指定的时间后。此后执行取回命令，将不会有任何内容返回（除非重新存储同样的键名）。 flush_all 实际上没有立即释放项目所占用的内存，而是在随后陆续有新的项目被储存时执行（这是由memcached的懒惰检测和删除机制决定的）。
flush_all 效果是它导致所有更新时间早于 flush_all 所设定时间的项目，在被执行取回命令时命令被忽略。
### 4、其他命令
memcached还有很多命令，比如对于存储为数字型的可以通过incr/decr命令进行增减操作等等，这里只列出开发和运维中经常使用的命令，其他的不再一一举例说明。

### 补充一则：简单认识.net framework中的几种缓存
web站点中缓存的重要性毋庸置疑。我想很多asp.net开发人员在开发web应用系统的时候优先考虑使用的缓存并不是第三方缓存解决方案（比如分布式缓存memcached、redis等等），而应该是.net framework已经提供的多种缓存解决方案。下面结合自己的开发经验谈谈对.net framework中缓存的认识。
1、System.Web.Caching.Cache
估计大部分做过asp.net开发的人都用过这个命名空间下的缓存，我们可以直接使用HttpContext.Current.Cache实例而不用实例化。当然这个命名空间下的Cache类是允许您实例化的，需要定制自己的缓存系统的当然可以完全自己控制如何初始化这个类。我在园子里看到过有很多文章介绍Cache的CRUD辅助类库大多数都是针对System.Web.Caching.Cache。
需要说明的是，我们还可以通过该命名空间下的HttpRuntime.Cache实现web、控制台、winform等不同表现形式下的缓存，而且完全无需自己实例化。HttpRuntime.Cache是之前个人开发中使用比较多的一个类，现在比较偏爱.net framework4.0中的增强型的缓存类MemoryCache。
2、Output Cache
众所周知，输出缓存主要分页面输出缓存和页面部分缓存。说白了，就是缓存整个页面的html或者部分html，本来没什么值得讨论的，但是最近看到dudu的[这篇](http://www.cnblogs.com/dudu/archive/2011/11/03/outputcache_Bug_vary.html)博客才恍然发现，想不到使用它还真是大有讲究，我以前怎么就没有发现这个问题呢？看来发现问题和解决问题的能力同样重要，有时候前者甚至更重要啊。
3、System.Runtime.Caching
现在个人开发中使用最多的类MemoryCache出自这个命名空间，使用前需要引用using System.Runtime.Caching。MemoryCache继承自ObjectCache, IEnumerable, IDisposable，其中ObjectCache是个抽象类。用过MemoryCache的人都知道，这个MemoryCache有一个属性叫Default，通常可以像下面这样使用：
privatestaticObjectCache memCache = MemoryCache.Default;当然我们也完全可以通过public MemoryCache(string name, NameValueCollection config = null)构造函数初始化缓存对象。
接着我们可以在web.config文件中配置每个MemoryCache实例运行的内存使用配额方案和配额检查周期，下面示例参考[MSDN](http://msdn.microsoft.com/zh-cn/library/dd941872.aspx)：
<system.runtime.caching><memoryCache><namedCaches><addname="Default"cacheMemoryLimitMegabytes="10"pollingInterval="00:02:00"/></namedCaches></memoryCache></system.runtime.caching>这些配置意义在于可以明确指定每个MemoryCache实例运行的内存使用配额方案和配额检查周期。比如我们可以通过配置来按需更改MemoryCache.Default实例的内存配额（不知道缓存可用最大内存是多少，可能还是传说中的800M左右）。缓存过期策略与其它的缓存框架大同小异，与System.Web.Caching.Cache的不同只是名称不叫CacheDependency，而叫[ChangeMonitor](http://msdn.microsoft.com/zh-cn/library/system.runtime.caching.changemonitor.aspx)，并且提供了基于文件和目录的缓存依赖策略。关于缓存过期策略也比较有探讨的必要，不过个人开发中比较偏重于数据缓存和替换，目前还没有接触和使用过比较完美的过期策略解决方案。

