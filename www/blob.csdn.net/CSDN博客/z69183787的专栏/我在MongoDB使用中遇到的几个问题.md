# 我在MongoDB使用中遇到的几个问题 - z69183787的专栏 - CSDN博客
2016年10月26日 18:20:04[OkidoGreen](https://me.csdn.net/z69183787)阅读数：3673
#### 1.MongoDB主键_id
在我一开始接触MongoDB的时候我觉得无论是什么数据库都是有主键的，而且可以自定义，直到遇到了MongoDB。没有人会喜欢被强制使用“_id”这样的主键名称，MongoBlog上面是这样说的：
> 
**blog.mongodb.org:**Every document stored in MongoDB must have an "_id" key, and you can have other keys as many as your wish.
MongoDB默认会为每个document生成一个 _id 属性，作为默认主键，且默认值为ObjectId,可以更改 _id 的值(可为空字符串)，但每个document必须拥有 _id 属性。如果你不想使用默认的主键你需要插入自己的主键名和值（自行管理、维护），例如：userId: 20140600001。
为什么是这样的？粗略说来大概是这么一句话：MongoDB是以空间换取时间和效率，本身不提供关系数据库中常见的主键生成策略、事务。因此使用者或者说服务器端需要有自己的处理逻辑。
#### 2.时间
MongoDB中的时间类型默认是MongoDate，MongoDate默认是按照UTC（世界标准时间）来存储。例如下面的两种使用方式:
```cpp
```cpp
db.col.insert({"date": new Date(),num: 1})
db.col.insert({"date": new Date().toLocaleString(),num: 2})
db.col.find()
{
    "_id" : ObjectId("539944b14a696442d95eaf08"),
    "date" : ISODate("2014-06-12T06:12:01.500Z"),
    "num" : 1
}
{
    "_id" : ObjectId("539944b14a696442d95eaf09"),
    "date" : "Thu Jun 12 14:12:01 2014",
    "num" : 2
}
```
```
> 
**注意：**第一条数据存储的是一个Date类型，第二条存储存储的是String类型。两条数据的时间相差大约8个小时（忽略操作时间），第一条数据MongoDB是按照UTC时间来进行存储。
另外还有朋友说构造日期时要指定完整的时间，否则也将会造成时间不准确。对于这个，我做了尝试并没有出现这种情况，是否是版本问题不得而知……
一般，我们在使用MongoDB存储时间的时候会把时间存储成字符串（或者直接存储为毫秒数），将MongoDB内置处理的这部分内容完全交由服务器来做，这样也能保证所有的时间格式统一、逻辑统一。
#### 3.MongoDB中的一对多、多对多关系（MongoDB中数据关系的处理）
MongoDB的基本单元是Document（文档），通过文档的嵌套（关联）来组织、描述数据之间的关系。
例如我们要表示一对多关系，在关系型数据库中我们通常会设计两张表A（一）、B（多），然后在B表中存入A的主键，以此做关联关系。然后查询的时候需要从两张表分别取数据。MongoDB中的Document是通过嵌套来描述数据之间的关系，例如：
```
{
    _id:ObjectId(akdjfiou23o4iu23oi5jktlksdjfa)
    teacherName: "foo",
    students: [
        {
            stuName: "foo",
            totalScore：100，
            otherInfo :[]
            ...
        },{
            stuName: "bar",
            totalScore：90，
            otherInfo :[]
            ...
        }
    ]
}
```
一次查询便可得到所有老师和同学的对应关系。当然我们还可以进一步的进行子查询，例如我们可以选择totalScore在100以上的学生，包括再对students进行分页等等。
> 
**注意：**不是所有的一对多关系都适合这种组织形式
如何处理好多对多的关系可谓是NoSQL的精髓所在。理论上，可以在一个集合中完成存储，不过实际上这样的情况非常罕见。这是由于查询的多样性所导致的，若是只有一种类型的查询，则这种多对多的关系放在一个良好设计的集合中，虽然会有大量的冗余，但是效率一定是最高的。如何设计这种数据库的关键就是看你有多少种查询，每一种的频率是多少，使用的其他要求是什么样的。对于不同的查询，同样的数据库设计的性能也是大不一样。还有一点，一般不要拆成三个集合，这是传统的关系型数据库的思维方式。传统关系型数据库是为了防止冗余，而MongoDB优良的存取速度以及分片结构恰恰可以应对数据冗余。NoSQL（Not
 Only SQL）数据库一定要换个角度思考问题。常见的情况就是拆成两个集合，然后有一部分冗余，对最常用的查询做一个索引。这个这个还得多多实践。
#### 4.内嵌文档查询
在MongoDB中文档的查询是与顺序有关的。例如：
```
{
    "address" : {
        "province" : "河北省",
        "city" : "石家庄",
        ...
    },
    "number" : 2640613
}
```
要搜索province为“河北省”、city为“石家庄”可以这样:
```
db.col.find(
    {
        "address":{
            "city" : "石家庄",
            "province" : "河北省"
        }
    }
)
```
然而这样什么都不会查询到。事实上，这样的查询MongoDB会当做全量匹配查询，即document中所有属性与查询条件全部一致时才会被返回。当然这里的“全部一致”也包括属性的顺序。那么，上面的查询如果想搜索到之前的应该先补充number属性，然后更改address属性下的顺序。
在实际应用中我们当然不会这么来查询文档，尤其是需要查询内嵌文档的时候。MongoDB中提供"."（点）表示法来查询内嵌文档。因此，上面的查询可以这样写：
```
db.col.find(
    {
        "address.privince":"河北省"
    }
)
```
然而此时我们却引入了另一个问题，例如将URL作为键保存的时候。一种解决办法是在插入前执行一次替换，转换为URL中的非法字符。
当文档结构变得更加复杂以后，内嵌文档的匹配需要些许技巧。例如，假设有博客文章若干，要找到由Joe发表的5分以上的评论。博客文章的结构如下例所示：
```
{
    "content" : "...",
    "comments" : [
        {
            "author" : "joe",
            "score" : 3,
            "comment" : "nice post"
        },
        {
           "author" : "mary",
           "score" : 6,
           "comment" : "terrible post"
        }
    ]
}
```
不能直接用db.blog.find({"comments":{"author":"joe","score":{"$gte":5}}})来查寻。内嵌文档匹配要求整个文档完全匹配，而这不会匹配"comment"键。使用db.blog.find({"comments.author" : "joe", "comments.score" : {"$gte" : 5}})同样也不会达到目的。因为符合author条件的评论和符合score条件的评论可能不是同一条评论。也就是说，会返回刚才显示的那个文档。因为"author"
 : "joe"在第一条评论中匹配了，"score" : 6在第二条评论中匹配了。
要正确地指定一组条件，而不用指定每个键，要使用"$elemMatch"。这种模糊的命名条件句能用来部分指定匹配数组中的单个内嵌文档的限定条件。所以正确的写法应该是这样的：
```bash
db.blog.find(
    {
        "comments" : {
            "$elemMatch" : {
                "author" : "joe",
                "score":{
                    "$gte":5
                }
            }
        }
    }
)
```
"$elemMatch"将限定条件进行分组，仅当需要对一个内嵌文档的多个键操作时才会用到。
#### MongoDB安全
MongoDB在默认设置下并没有身份验证。MongoDB会认为自身处在一个拥有防火墙的信任网络。但是这不代表它不支持身份验证，如果需要可以轻松的开启（启动参数auth）。
还有一点就是，与MongoDB的连接默认情况下都是非加密的，这就意味你的数据可能被第三方记录和使用。如果你的MongoDB是在自己的非广域网下使用，那么这种情况是不可能发生的。
然而如果你是通过公网访问MongoDB的话，那么你肯定会希望你的通信是经过加密的。由于各种原因，公版的MongoDB是不支持SSL的,如果需要使用带有SSL的MongoDB需要自行编译，私人定制一个自己的MongoDB。企业版本肯定是含有这个功能的，不过最少也要掏出2500刀吧。
如何编译带有SSL支持的MongoDB可以参考这篇文章：
[Compiling MongoDB with SSL Support on Ubuntu](http://gravitronic.com/compiling-mongodb-with-ssl-support-on-ubuntu-12-04-lts/)
#### 参考资料
- 《MongoDB: The Definitive Guide》
- [](http://baike.baidu.com/view/2677528.htm?fr=aladdin)[http://baike.baidu.com/view/2677528.htm?fr=aladdin](http://baike.baidu.com/view/2677528.htm?fr=aladdin)
- [blog.mongodb.org](http://blog.mongodb.org)
- [](http://my.oschina.net/zhengyun/blog/108421)[http://my.oschina.net/zhengyun/blog/108421](http://my.oschina.net/zhengyun/blog/108421)
- [](http://www.csdn.net/article/2012-11-15/2811920-mongodb-quan-gong-lue)[http://www.csdn.net/article/2012-11-15/2811920-mongodb-quan-gong-lue](http://www.csdn.net/article/2012-11-15/2811920-mongodb-quan-gong-lue)
文／Terry_Wang（简书作者）
原文链接：http://www.jianshu.com/p/e59cd2dc5274
著作权归作者所有，转载请联系作者获得授权，并标注“简书作者”。
