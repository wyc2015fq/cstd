# 从TabletoList到Xml、Json序列化 - 刘炫320的博客 - CSDN博客
2016年09月03日 18:33:49[刘炫320](https://me.csdn.net/qq_35082030)阅读数：311标签：[xml																[json																[sql																[c#																[orm](https://so.csdn.net/so/search/s.do?q=orm&t=blog)](https://so.csdn.net/so/search/s.do?q=c#&t=blog)](https://so.csdn.net/so/search/s.do?q=sql&t=blog)](https://so.csdn.net/so/search/s.do?q=json&t=blog)](https://so.csdn.net/so/search/s.do?q=xml&t=blog)
个人分类：[C#技术																[设计模式](https://blog.csdn.net/qq_35082030/article/category/6245500)](https://blog.csdn.net/qq_35082030/article/category/6240463)
在日常的使用中，经常需要持久化数据，那么数据来源纷繁多样，有可能来自SQL的，有可能来自文件的，有可能是来自XML或者JSON格式的（通常这类是从网上获得的），那么真的每次都要一点一点写么？
答案当然是否定的。
单单从SQL方面来讲，无论是熟悉的Hibernate还是流行的Mybatis，都是非常好的SQL_ORM框架。可是如果只是一个很小型的项目，需要这么大动干戈的使用这种吗？比如只是用来做一些小玩意。如果你不用这种，也不想自己写，那么你就需要两个小类就可以完成，一个是数据库帮助类，帮你完成数据库的连接和操作，另一个就是从内存数据库中映射到实体类的数据转换类。
那么提到映射这个概念，就需要了解它的原理。
首先你需要准备的一个是数据源，另一个是你的实体类，有的叫做model,有的叫做Entity，还有的叫做Domain，实际上都是一个只包含数据结构的实体类。在C#中，它使用的是Attribute和反射机制相互结合，就造就了可以自动映射的奇迹。具体步骤如下：
1、首先它先利用反射机制获取模型里的所有成员
2、然后遍历成员，如果成员是其属性，就再获取其attribute。
3、在Atrribute中获取到所定义的DataFieldAttribute。
4、然后利用属性名从表中取出相应的列，赋值给相应的属性。
有关这个工程，在我的Github上面https://github.com/liuxuan320/LightSQL提供了C#版本的，里面还附带了一个文件操作类。大家可以自行下载修正和补充。
其实这种形式在之前就已经出现，它叫做序列化操作，尤其是针对XML和JSON格式的序列化操作。也就是把XML或者JSON格式的数据自动的映射成实体类。
如果是JSON格式的，.Net自带了2个类供大 家使用，一个是JavaScriptSerializer类，但这需要引用web，并且必须要VS2013以上版本使用，也就是.NET4.5以上才行。另外一个是DataContractJsonSerializer，这个也是可以进行序列化的，而且可以处理其中的键类型不是字符串类型的，但是前者的兼容性更好。具体代码我也已经更新到https://github.com/liuxuan320/LightSQL上面了。
当然，如果你问，如果是多级的JSON怎么办？那你的成员也定义成List的就好了。
如果你又问，如果不规则怎么办?把泛型T直接变成obJect的即可。
忘记说了一点，你的模型的属性名要和JSON里键名一致才行。
当然这只是进行序列化和反序列化，如果你还需要更高的要求，比如，我要在原JSON格式上直接进行操作，那我想，你可能需要一个开源库Newtonsoft.Json。下载地址http://json.codeplex.com/.这个除了有原生的序列和反序列化JSON方法，还有直接创建JSON对象，当然它还有其他一些高级用法。不过缺点就是，你需要引入第三方库。
而对于XML来讲，它同样具有序列化和反序列化操作，同样的，我也已经写在工程里了，并且附了注释。当然，对XML的操作还有很多，但是我们这一篇只讨论关系映射、序列化及反序列化。
对于JSON和XML的操作当然有很多，如果要写的话，肯定要单独拿出来一篇来写，本篇文章主要介绍的就是Tabletolist及JSON、XML的序列化、反序列化操作。说到底，还是为了关系映射。
