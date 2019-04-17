# 读《云GIS技术与实践》之概念篇 - 长歌行 - CSDN博客





置顶2019年01月06日 21:35:53[iuhsihsow](https://me.csdn.net/iuhsihsow)阅读数：119
所属专栏：[时空云计算](https://blog.csdn.net/column/details/32384.html)









![在这里插入图片描述](https://img-blog.csdnimg.cn/20190106212601633.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L2l1aHNpaHNvdw==,size_16,color_FFFFFF,t_70)

之前在Amazon上买的，大概27块钱包邮。

地理信息从之前的纸质地图和书籍文献过渡到了电子信息，使用arcgis或者supermap系列软件可以很容易的查看地理数据并进行地理分析。但是真正让地理信息走入寻常百姓的视野中的，应该还是在google earth的推出之后。现在随着互联网和云的普及与推广，GIS必然会以更平易近人的方式让我们接触到。云GIS应该是一种趋势。

![在这里插入图片描述](https://img-blog.csdnimg.cn/20190106212706621.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L2l1aHNpaHNvdw==,size_16,color_FFFFFF,t_70)

### GIS的新特性：
- 
数据密集
- 数据规模呈指数级增长，对数据的归档和共享方式有了新要求。

- 
计算密集
- 由于数据量的增大和分析要求的提高，算法和计算模型都更加复杂，特别是深度学习的引入。单机或者简单的集群不能满足要求了。

- 
并发密集

```
- 面对公众的在线GIS服务越来越重要
```

- 
时空密集
- 随着网络，存储和传感器的发展，时间维度的信息也可以被更多的途径获取和存储传输下来。时空密集是地理信息科学面临的最根本的挑战。


### 云计算的特性：
- 按需自服务
- 跨网络访问
- 资源池化
- 动态可伸缩
- 按使用付费

而云体系可以从一定程度上克服上面的问题。为GIS的高存储、高计算、高可靠提供了一种新的可靠手段。同时服务式GIS本身也具有资源按需使用的需求和可行性。

### 云GIS的服务模式：
- 基础设施即服务 Iaas
- 平台即服务 PaaS
- 软件即服务 SaaS
- 数据即服务 DaaS
![在这里插入图片描述](https://img-blog.csdnimg.cn/20190106213113172.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L2l1aHNpaHNvdw==,size_16,color_FFFFFF,t_70)
### 核心价值

云GIS的核心价值是利用云计算的集约化计算提供服务，节约资源。对于GIS的支持包括：
- 云GIS可以用于地理时空数据的获取。
- 云GIS支持地理建模。
- 云GIS支持知识与决策。

不过我认为，目前来说，云GIS的核心价值不完全是省钱。因为有些事情，重点不在资金方面，而是在于如何能够短时间完成困难的任务。很多项目初期，主要是要解决有无的问题。而云GIS此时就可以给大家提供这么一个快速上手平台，消除信息的不对称，克服客户在存储或者计算上面的通用难题，而让他们能直接解决业务上的问题。此时云GIS平台就像一些编程框架或者office套件一样。

其实在技术方面，我这篇博客不想写太多内容，技术的发展太快了，而且内容太多，不是一篇博客就能覆盖住的。只是简单提一下。

### 云GIS核心技术栈

![在这里插入图片描述](https://img-blog.csdnimg.cn/20190106213238287.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L2l1aHNpaHNvdw==,size_16,color_FFFFFF,t_70)

### 虚拟化与云

虚拟化允许一个平台同时运行多个操作系统，将计算机的各种实体对象，如CPU、内存、网络以及存储等各种资源进行抽象。

现在很多号称为云GIS的平台其实只做到了一台或者多台机器的虚拟化，云是建立在虚拟化上面的，但是虚拟化不是云的全部。这部分知乎有很多深入探讨，可以移步过去看看。关键词：虚拟化，云，集群，分布式。

### 私有云，公有云

（阿里云这边称为公共云，专有云。）

私有云一般是大型政企或者有特别的保密需求才会考虑。小型团体公司个人重要还是在公有云上开发。主流公有云平台基本都提供给了弹性计算、数据存储、云安全与管理、网络、大规模计算等功能。如Amazon，Azue，阿里云，京东云，腾讯云等。主流的公有云GIS平台有ArcGIS Online，SuperMap Online，地图慧，MapBox，OpenStreetMap，GeoHey等。

### 小看法

不过目前很多公司，单位还是属于重资产过于服务的思维模式，云GIS的发展还是需要有一个刚需的突破点。或许从存储，或许从服务，或许从AI。。等等。




