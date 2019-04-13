
# java中mongoDB连接遇到的问题 - lin_tuer的博客 - CSDN博客


2017年11月08日 16:24:57[lin_tuer](https://me.csdn.net/lin_tuer)阅读数：2306标签：[mongodb																](https://so.csdn.net/so/search/s.do?q=mongodb&t=blog)[java																](https://so.csdn.net/so/search/s.do?q=java&t=blog)[
							](https://so.csdn.net/so/search/s.do?q=mongodb&t=blog)个人分类：[JAVA																](https://blog.csdn.net/lin_tuer/article/category/6407722)



# 用单例模式建立mongoDB的连接，需要每次操作完关闭连接吗？
关于建立单例模式的MongoDB连接，参考这里[MongoClient连接池用法](http://blog.csdn.net/gaopeng0071/article/details/51281103)
建立了之后，每次操作调用同一个实例，MongoClient又是线程安全的，貌似就没有什么问题了。但是 突然想到，这些连接获取后，操作完不主动释放，会不会一直被占有？
– 于是开始找各种资料，这篇文章给出了很形象的说明，[MongoDB的连接池和管理](MongoDB%E7%9A%84%E8%BF%9E%E6%8E%A5%E6%B1%A0%E5%92%8C%E8%BF%9E%E6%8E%A5%E7%AE%A1%E7%90%86)
– 原来，答案是没有必要！
在关系型数据库中，我们总是需要关闭使用的数据库连接，不然大量的创建连接会导致资源的浪费甚至于数据库宕机。
通常我们习惯于new 一个connection并且通常在finally语句中调用connection的close()方法将其关闭。正巧，mongoDB中当我们new一个Mongo的时候，会发现它也有一个close()方法。所以会出现这样的情况：我们在需要DB操作的方法中new一个mongo实例，然后调用mongo.getDB()方法拿到对应的连接，操作完数据之后再调用mongo.close()方法来关闭连接。 看起来貌似是没有什么问题，但是如果你再研究一下mongo的API，你会发现这样耳朵操作就相当于园丁在浇花的时候去打了一桶水，然后舀了一勺水浇一朵花，然后他把一桶水全倒了回去，重新打一桶水，再舀了一勺水浇另外一朵花。。。
说到这里大家应该都已经明白了，*其实当你new Mongo()的时候，就创建了一个连接池，getDB()只是从这个连接池中拿一个可用的连接。而连接池是不需要我们及时关闭的，我们可以在程序的生命周期中维护一个这样的单例，至于从连接池中拿出的连接，我们需要关闭吗？答案是NO。*你会发现DB根本没有close()之类的方法。在mongoDB中，一个连接池会维持一定数目的连接，当你需要的时候调用getDB()去连接池中拿到连接，而mongo会在这个DB执行完数据操作时候自动收回连接到连接池中待用。所以在mongoDB中大家不必担心连接没有关闭的问题，在你需要在所有操作结束或者整个程序shutdown的时候调用mongo的close()方法即可。
# MongoDB带安全验证的连接如何设置MongoClientOptions
管理如何创建MongoClient 这篇文章给了详细的介绍[MongoDB 连接 for java](http://blog.csdn.net/u013753516/article/details/44747615)
由于我的需要安全验证，我就选择了这种
`MongoClientOptions clientOptions =   
    new MongoClientOptions``.Builder``()``.connectionsPerHost``(``50``)``.maxWaitTime``(``2000``)``.build``()``;``List<MongoCredential> lstCredentials =   
        Arrays``.asList``(MongoCredential``.createMongoCRCredential``(``"admin"``,``"myinfo"``,``"123456"``.toCharArray``()))``;``client = new MongoClient(new ServerAddress(``"127.0.0.1"``),lstCredentials, clientOptions)``;``client``.close``()``;`可是 运行项目，却报出了认证错误，查了之后发现。。方法不对，。
`Arrays``.asList``(MongoCredential``.createMongoCRCredential``(``"admin"``,``"myinfo"``,``"123456"``.toCharArray``()))``;`改为
`Arrays``.asList``(MongoCredential``.createCredential``(``"admin"``,``"myinfo"``,``"123456"``.toCharArray``()))``;`好了就。。。

