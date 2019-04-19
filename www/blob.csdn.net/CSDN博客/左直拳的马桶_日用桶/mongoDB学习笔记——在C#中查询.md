# mongoDB学习笔记——在C#中查询 - 左直拳的马桶_日用桶 - CSDN博客
2014年02月10日 11:44:01[左直拳](https://me.csdn.net/leftfist)阅读数：8861
**1、下载安装**
 想要在C#中使用MongoDB,首先得要有个MongoDB支持的C#版的驱动。C#版的驱动貌似有很多种，如官方提供的[samus](https://github.com/samus/mongodb-csharp)。 实现思路大都类似。这里我们用官方提供的mongo-csharp-driver ：
下载地址：http://github.com/mongodb/mongo-csharp-driver/downloads
解压之后得到两个dll：
 MongoDB.Driver.dll：顾名思义，驱动程序
 MongoDB.Bson.dll：序列化、Json相关
 然后在我们的程序中引用这两个dll。
**2、连接数据库**
```
using MongoDB.Bson;
using MongoDB.Driver;
```
```
//数据库连接字符串
        const string strconn = "mongodb://127.0.0.1:27017";
        //数据库名称
        const string dbName = "test";
        MongoServer server;
        MongoDatabase db;
        void Init()
        {
            //创建数据库链接
            server = MongoDB.Driver.MongoServer.Create(strconn);
            //获得数据库
            db = server.GetDatabase(dbName);
        }
```
**3、查询数据**
1）查询规则集合
```
class Users
        {
            public ObjectId _id;//BsonType.ObjectId 这个对应了 MongoDB.Bson.ObjectId 
            public string Name { get; set; }
            public string Sex { set; get; }
        }
        const string ENTER = "\r\n";
        private void btnQuery_Click(object sender, EventArgs e)
        {
            //获取Users集合
            MongoCollection col = db.GetCollection("Users");
            //查询全部集合里的数据
            var result1 = col.FindAllAs<Users>().ToList();
            StringBuilder sb = new StringBuilder("");
            foreach (Users user in (List<Users>)result1)
            {
                sb.Append(user.Name + ENTER);
            }
            MessageBox.Show(sb.ToString());
        }
```
这种方式只适合每个文档(document)结构都一样的集合：
![](https://img-blog.csdn.net/20140210110739500?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvbGVmdGZpc3Q=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)
但是NoSql诟病关系型数据库的论点之一，就是这种所谓每条记录的结构都要保持一致的情况，因为有些字段不是所有记录的必须。那么，在mongoDB这里，对于集合中文档结构不一致的情况下，上述代码获取不到数据，似乎应该采用如下方式：
2）查询不规则集合
集合情况：
![](https://img-blog.csdn.net/20140210113751453?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvbGVmdGZpc3Q=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)
在集合thins里，基本上每个文档结构都不一样。
查找代码如下：
```
private void btnQuery2_Click(object sender, EventArgs e)
        {
            //获取thins集合
            MongoCollection col = db.GetCollection("thins");
            //查询全部集合里的数据
            var result1 = col.FindAllAs<BsonDocument>().ToList();
            StringBuilder sb = new StringBuilder("");
            foreach (BsonDocument bd in (List<BsonDocument>)result1)
            {
                string name = bd.Contains("name") ? bd.GetValue("name").ToString() : "不存在";
                sb.Append(name + ENTER);
            }
            MessageBox.Show(sb.ToString());
        }
```
