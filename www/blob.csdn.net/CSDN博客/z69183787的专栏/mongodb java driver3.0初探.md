# mongodb java driver3.0初探 - z69183787的专栏 - CSDN博客
2016年02月15日 16:22:08[OkidoGreen](https://me.csdn.net/z69183787)阅读数：1406
由于项目中关于地理位置的计算目前采用mysql以及自定义函数的方案，性能并不好，准备尝试一下mongodb，为了提前做一点知识储备，这几天学习了mongodb的java驱动包
关于mongodb，这里就不做详细介绍了，网上都可以找到。
下面详细描述一下使用mongodb的过程：
1、安装mongodb，我的系统win7，因此下载了windows版本，下载地址：[https://www.mongodb.org/downloads](https://www.mongodb.org/downloads)
这里需要注意的是，启动前需要手动建立保存数据的目录，mongodb不会自动帮你建立，我采用默认的目录名称C:\data\db，应该是可以修改的，然后启动安装目录bin目录下的mongod.exe
2、下载mongodb驱动包，下载地址：[点击打开链接](https://github.com/mongodb/mongo-java-driver/releases)  我是采用maven管理依赖包，因此很简单，在pom.xml中添加以下代码即可
**[html]**[view
 plain](http://blog.csdn.net/leiswpu/article/details/48465029#)[copy](http://blog.csdn.net/leiswpu/article/details/48465029#)
- <dependency>
- <groupId>org.mongodb</groupId>
- <artifactId>mongo-java-driver</artifactId>
- <version>3.0.3</version>
- </dependency>
最新的驱动版本是3.0，关于java操作mongodb网上找了许久，差不多都是<3.0版本的代码，老版本与新版本差别还是挺大的，许多老版本的方法已经Deprecated，因此只有从官方网站学习操作mongodb的方法
3、新建java工程。
直接上代码：
**[java]**[view
 plain](http://blog.csdn.net/leiswpu/article/details/48465029#)[copy](http://blog.csdn.net/leiswpu/article/details/48465029#)
-                 MongoClientOptions.Builder build = new MongoClientOptions.Builder();  
- //与数据最大连接数50
-         build.connectionsPerHost(50);  
- //如果当前所有的connection都在使用中，则每个connection上可以有50个线程排队等待
-         build.threadsAllowedToBlockForConnectionMultiplier(50);  
-         build.connectTimeout(1*60*1000);  
-         build.maxWaitTime(2*60*1000);  
-         MongoClientOptions options = build.build();  
-         MongoClient client = new MongoClient("127.0.0.1", options);  
- 
- //获取数据库test,不存在的话，会自动建立该数据库
-         MongoDatabase db = client.getDatabase("test");  
- //获取data集合，不存在的话，会自动建立该集合（相当于关系数据库中的数据表）
-                 MongoCollection<Document> users = db.getCollection("data");  
-         Document document = new Document();  
-         document.append("firstName", "lei");  
-         document.append("address", "sichuan chengdu");  
-         users.insertOne(document);  
- //MongoClient使用完后必须要close释放资源
-                 client.close();  
以上代码结合注释还是挺简单的，一看便知，运行后，可以通过mongodb的图形化工具查看插入的数据结果
关于图形化工具这里我推荐MongoVUE，这款工具需要付费，不过也有破解版，附件可以获得安装包，是破解了的，找了很多这类工具，还是感觉MongoVUE好用一些。
对上面代码稍微解释一下，`MongoClient`实际上是mongodb的连接池，在应用中我们只需要一个实例就可以了，关于这个连接池的设置，可以使用MongoClientOptions.Builder
具体需要设置的一些参数可以参考该类中的属性，其他查询，修改，删除操作可以查看MongoCollection类的方法
更多时候，我们可能需要用到自定义的MODEL对象，保存到数据库或者从数据库查询，这时可以通过MongoCollection的重载方法操作，如下：
**[java]**[view
 plain](http://blog.csdn.net/leiswpu/article/details/48465029#)[copy](http://blog.csdn.net/leiswpu/article/details/48465029#)
- MongoCollection<User> users = db.getCollection("data",User.class);  
- User u = new User();  
- u.setName("lhc");  
- u.setAge(100000);  
- users.insertOne(u);  
这种使用方式必须满足两个条件
- 一个User的编解码类实现Codec接口，并且在`MongoCollection`的 `CodecRegistry`注册
- 这个编解码对象可以编解码全部bson文档
`MongoCollection`默认配置了`Document`，`BasicDBObject`，`BsonDocument`的Codec，新的`CodecRegistry`实例可以通过三个级别配置：
- `在MongoClient`中通过`MongoClientOptions`
- `在MongoDatabase`中通过`withCodecRegistry`方法
- `在MongoCollection中通过withCodecRegistry`方法
现在，我们通过实现User类的Codec介绍具体的实现过程，请看下面代码：
**[java]**[view
 plain](http://blog.csdn.net/leiswpu/article/details/48465029#)[copy](http://blog.csdn.net/leiswpu/article/details/48465029#)
- package cn.dayuanzi.test;  
- 
- import org.bson.BsonReader;  
- import org.bson.BsonWriter;  
- import org.bson.codecs.Codec;  
- import org.bson.codecs.DecoderContext;  
- import org.bson.codecs.EncoderContext;  
- 
- 
- publicclass UserCodec implements Codec<User> {  
- 
- 
- public UserCodec(){  
- 
-     }  
- 
- /**
-      * @see org.bson.codecs.Decoder#decode(org.bson.BsonReader, org.bson.codecs.DecoderContext)
-      */
- @Override
- public User decode(BsonReader reader, DecoderContext decoderContext) {  
-         User user = new User();  
-         reader.readStartDocument();  
- //_id，_class是插入数据时自动生成的项
-                 reader.readObjectId("_id");  
-         reader.readString("_class");  
-         user.setName(reader.readString("name"));  
-         user.setAge(reader.readInt32("age"));  
- 
-         reader.readEndDocument();  
- return user;  
-     }  
- 
- /**
-      * @see org.bson.codecs.Encoder#encode(org.bson.BsonWriter, java.lang.Object, org.bson.codecs.EncoderContext)
-      */
- @Override
- publicvoid encode(BsonWriter writer, User value, EncoderContext encoderContext) {  
-         writer.writeStartDocument();  
- 
-         writer.writeString("name", value.getName());  
-         writer.writeInt32("age", value.getAge());  
-         writer.writeEndDocument();  
-     }  
- 
- /**
-      * @see org.bson.codecs.Encoder#getEncoderClass()
-      */
- @Override
- public Class<User> getEncoderClass() {  
- return User.class;  
-     }  
- }  
然后修改前面的代码：
**[java]**[view
 plain](http://blog.csdn.net/leiswpu/article/details/48465029#)[copy](http://blog.csdn.net/leiswpu/article/details/48465029#)
- MongoClientOptions.Builder build = new MongoClientOptions.Builder();  
- //与数据最大连接数50
- build.connectionsPerHost(50);  
- //如果当前所有的connection都在使用中，则每个connection上可以有50个线程排队等待
- build.threadsAllowedToBlockForConnectionMultiplier(50);  
- build.connectTimeout(1*60*1000);  
- build.maxWaitTime(2*60*1000);  
- MongoClientOptions options = build.build();  
- MongoClient client = new MongoClient("127.0.0.1", options);  
- 
- //获取数据库test,不存在的话，会自动建立该数据库
- MongoDatabase db = client.getDatabase("test");  
- CodecRegistry codecRegistry = CodecRegistries.fromRegistries(CodecRegistries.fromCodecs(new UserCodec()),MongoClient.getDefaultCodecRegistry());  
- //获取user集合，不存在的话，会自动建立该集合（相当于关系数据库中的数据表）
- MongoCollection<User> users = db.getCollection("user",User.class).withCodecRegistry(codecRegistry);  
- 
- User u = users.find().first();  
- System.out.println(u.getName()+">>>"+u.getAge());  
- //MongoClient使用完后必须要close释放资源
- client.close();  
再次执行程序，就会查询出user集合中第一条数据记录
在使用编解码功能时，我们可以发现并不是那么方便，有没有更好的实现方式还需要进一步的研究，以上就是使用mongodb最新版本的java驱动的过程
（最后发现MongoVUE的安装包十多M不能放在附件里，有需要的朋友只有搜索一下了，抱歉）
