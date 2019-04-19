# MongoDB 3.0之Java API使用 - 零度的博客专栏 - CSDN博客
2016年08月02日 16:24:41[零度anngle](https://me.csdn.net/zmx729618)阅读数：12595
```
import java.util.*;
import org.bson.Document;  
import org.bson.conversions.Bson;  
  
import com.mongodb.BasicDBObject;  
import com.mongodb.Block;  
import com.mongodb.MongoClient;  
import com.mongodb.client.*;  
import com.mongodb.client.model.BulkWriteOptions;  
import com.mongodb.client.model.DeleteOneModel;  
import com.mongodb.client.model.InsertOneModel;  
import com.mongodb.client.model.ReplaceOneModel;  
import com.mongodb.client.model.UpdateOneModel;  
import com.mongodb.client.result.DeleteResult;  
import com.mongodb.client.result.UpdateResult;  
  
import static com.mongodb.client.model.Filters.*;  
  
/** 
 * 程序入口 
 *  
 * @author John 
 * 
 */  
public class testmain {  
    public static void main(String[] args) {  
        testmain tm = new testmain();  
        tm.test();  
    }  
  
    /** 
     * test 
     */  
    private void test() {  
        // 获取链接  
        MongoClient mongoClient = new MongoClient("localhost", 27017);  
        // 获取数据库  
        MongoDatabase database = mongoClient.getDatabase("mydb");  
        // 进入某个文档集  
        MongoCollection<Document> collection = database.getCollection("test");  
  
        /********************** 数据插入 ****************************/  
        // 创建新文档  
        // Document doc = new Document("name", "MongoDB")  
        // .append("type", "database").append("count", 1)  
        // .append("info", new Document("x", 203).append("y", 102));  
        // 将文档插入文档集合  
        // collection.insertOne(doc);  
  
        // 创建一个包含多个文档的列表  
        // List<Document> documents = new ArrayList<Document>();  
        // for (int i = 0; i < 100; i++) {  
        // documents.add(new Document("i", i));  
        // }  
        // 向文档中插入列表  
        // collection.insertMany(documents);  
  
        /***************** 数据读取 ****************************************/  
        // // 显示集合中的文档的数量  
        // System.out.println(collection.count());  
        // 查询集合中的第一个文档  
        // Document myDoc = collection.find().first();  
        // System.out.println(myDoc.toJson());  
  
        //获取集合中的全部文档  
        // MongoCursor<Document> cursor = collection.find().iterator();  
        // try {  
        // while (cursor.hasNext()) {  
        // System.out.println(cursor.next().toJson());  
        // }  
        // } finally {  
        // cursor.close();  
        // }  
  
        //获取全部文档的另一种方法  
        // for (Document cur : collection.find()) {  
        // System.out.println(cur.toJson());  
        // }  
  
        // // 根据条件获取某分文档 eq:==  
        // Document myDoc = collection.find(eq("i", 71)).first();  
        // System.out.println(myDoc.toJson());  
  
        // 通过查询语句一次性获取多个数据  
        // Block<Document> printBlock = new Block<Document>() {  
        // @Override  
        // public void apply(final Document document) {  
        // System.out.println(document.toJson());  
        // }  
        // };  
        // 获得所有大于50的  
        // collection.find(gt("i", 50)).forEach(printBlock);  
        // 大于50 小于 100  
        // collection.find(and(gt("i", 50), lte("i", 100))).forEach(printBlock);  
  
        // 对输出文档进行排序,-1为递减，1为递增  
        // 官方文档的例子有误：http://mongodb.github.io/mongo-java-driver/3.0/driver/getting-started/quick-tour/#sorting-documents  
        // Document myDoc = collection.find(exists("i"))  
        // .sort(new BasicDBObject("i", -1)).first();  
        // System.out.println(myDoc.toJson());  
  
        // 选择性输出结果中的元素，0为不显示，1为显示  
        // 官方文档中的例子又不能用：http://mongodb.github.io/mongo-java-driver/3.0/driver/getting-started/quick-tour/#projecting-fields  
        // BasicDBObject exclude = new BasicDBObject();  
        // exclude.append("_id", 0);  
        // exclude.append("count", 0);  
        // exclude.append("name", 1);  
        // exclude.append("info", 1);  
        // Document myDoc = collection.find().projection(exclude).first();  
        // System.out.println(myDoc.toJson());  
  
        /************************* 修改数据库中数据 *************************************/  
  
        // 修改时的参数：  
        // $inc 对指定的元素加  
        // $mul 乘  
        // $rename 修改元素名称  
        // $setOnInsert 如果以前没有这个元素则增加这个元素，否则不作任何更改  
        // $set 修改制定元素的值  
        // $unset 移除特定的元素  
        // $min 如果原始数据更大则不修改，否则修改为指定的值  
        // $max 与$min相反  
        // $currentDate 修改为目前的时间  
  
        // //修改第一个符合条件的数据  
        // $set 为修改  
        // collection.updateOne(eq("i", 10), new Document("$set", new  
        // Document("i", 110)));  
        // // 获取全部文档,可以看到以前10的地方变成了110  
        // for (Document cur : collection.find()) {  
        // System.out.println(cur.toJson());  
        // }  
  
        // 批量修改数据并且返回修改的结果，讲所有小于100的结果都加100  
        // UpdateResult updateResult = collection.updateMany(lt("i", 100),  
        // new Document("$inc", new Document("i", 100)));  
        // // 显示发生变化的行数  
        // System.out.println(updateResult.getModifiedCount());  
        // // 获取全部文档,可以看到除了刚才修改的110其他的全为了100  
        // for (Document cur : collection.find()) {  
        // System.out.println(cur.toJson());  
        // }  
  
        /************************** 删除数据 *****************************/  
        // 删除第一个符合条件的数据  
        // collection.deleteOne(eq("i", 110));  
        // // 获取全部文档,可以看到没有110这个数了  
        // for (Document cur : collection.find()) {  
        // System.out.println(cur.toJson());  
        // }  
  
        // 删除所有符合条件的数据，并且返回结果  
        // DeleteResult deleteResult = collection.deleteMany(gte("i", 100));  
        // // 输出删除的行数  
        // System.out.println(deleteResult.getDeletedCount());  
        // // 获取全部文档,所有i>=100的数据都没了  
        // for (Document cur : collection.find()) {  
        // System.out.println(cur.toJson());  
        // }  
        /*************************** 程序块，一次执行多条语句 ********************************/  
        // 按照语句先后顺序执行  
        // collection.bulkWrite(Arrays.asList(new InsertOneModel<>(new Document(  
        // "_id", 4)), new InsertOneModel<>(new Document("_id", 5)),  
        // new InsertOneModel<>(new Document("_id", 6)),  
        // new UpdateOneModel<>(new Document("_id", 1), new Document(  
        // "$set", new Document("x", 2))), new DeleteOneModel<>(  
        // new Document("_id", 2)),  
        // new ReplaceOneModel<>(new Document("_id", 3), new Document(  
        // "_id", 3).append("x", 4))));  
        // // 获取全部文档  
        // for (Document cur : collection.find()) {  
        // System.out.println(cur.toJson());  
        // }  
  
        // 不按照语句先后顺序执行  
        // collection.bulkWrite(Arrays.asList(new InsertOneModel<>(new Document(  
        // "_id", 4)), new InsertOneModel<>(new Document("_id", 5)),  
        // new InsertOneModel<>(new Document("_id", 6)),  
        // new UpdateOneModel<>(new Document("_id", 1), new Document(  
        // "$set", new Document("x", 2))), new DeleteOneModel<>(  
        // new Document("_id", 2)),  
        // new ReplaceOneModel<>(new Document("_id", 3), new Document(  
        // "_id", 3).append("x", 4))), new BulkWriteOptions()  
        // .ordered(false));  
        // 获取全部文档  
        // for (Document cur : collection.find()) {  
        // System.out.println(cur.toJson());  
        // }  
          
          
        // 关闭数据库连接  
        mongoClient.close();  
  
    }  
  
}
```
**1、connect & authenticate &SSL**
**[java]**[view plain](http://blog.csdn.net/z69183787/article/details/50698172#)[copy](http://blog.csdn.net/z69183787/article/details/50698172#)[print](http://blog.csdn.net/z69183787/article/details/50698172#)[?](http://blog.csdn.net/z69183787/article/details/50698172#)![在CODE上查看代码片](https://code.csdn.net/assets/CODE_ico.png)[](https://code.csdn.net/snippets/1582477/fork)
- MongoClientOptions.Builder build = new MongoClientOptions.Builder();  
- //与数据最大连接数50
-         build.connectionsPerHost(50);  
- //如果当前所有的connection都在使用中，则每个connection上可以有50个线程排队等待
-         build.threadsAllowedToBlockForConnectionMultiplier(50);  
-         build.connectTimeout(1*60*1000);  
-         build.maxWaitTime(2*60*1000);  
-         MongoClientOptions options = build.build();  
-         MongoClient mongoClient = null;  
- 
- //        //SSL
- //        new MongoClientURI("mongodb://localhost/?ssl=true")
- //        MongoClientOptions.builder().sslEnabled(true).build()
- try {  
- //            ServerAddress sa = new ServerAddress("localhost", 27017);
- //            List<MongoCredential> mongoCredentialList = new ArrayList<MongoCredential>();
- //            mongoCredentialList.add(MongoCredential.createCredential("zhou", "zhou", "zhou".toCharArray()));
- //            mongoClient = new MongoClient(sa, mongoCredentialList,options);
- 
- 
- 
-             MongoClientURI uri = new MongoClientURI("mongodb://zhou:zhou@127.0.0.1:27017/?authSource=zhou",build);  
-             mongoClient = new MongoClient(uri);  
![](http://static.blog.csdn.net/images/save_snippets.png)
```java
MongoClientOptions.Builder build = new MongoClientOptions.Builder();
        //与数据最大连接数50
        build.connectionsPerHost(50);
        //如果当前所有的connection都在使用中，则每个connection上可以有50个线程排队等待
        build.threadsAllowedToBlockForConnectionMultiplier(50);
        build.connectTimeout(1*60*1000);
        build.maxWaitTime(2*60*1000);
        MongoClientOptions options = build.build();
        MongoClient mongoClient = null;
//        //SSL
//        new MongoClientURI("mongodb://localhost/?ssl=true")
//        MongoClientOptions.builder().sslEnabled(true).build()
        try {
//            ServerAddress sa = new ServerAddress("localhost", 27017);
//            List<MongoCredential> mongoCredentialList = new ArrayList<MongoCredential>();
//            mongoCredentialList.add(MongoCredential.createCredential("zhou", "zhou", "zhou".toCharArray()));
//            mongoClient = new MongoClient(sa, mongoCredentialList,options);
            MongoClientURI uri = new MongoClientURI("mongodb://zhou:zhou@127.0.0.1:27017/?authSource=zhou",build);
            mongoClient = new MongoClient(uri);
```
**2、CRUD**
**[java]**[view plain](http://blog.csdn.net/z69183787/article/details/50698172#)[copy](http://blog.csdn.net/z69183787/article/details/50698172#)[print](http://blog.csdn.net/z69183787/article/details/50698172#)[?](http://blog.csdn.net/z69183787/article/details/50698172#)![在CODE上查看代码片](https://code.csdn.net/assets/CODE_ico.png)[](https://code.csdn.net/snippets/1582477/fork)
- MongoDatabase mongoDatabase = mongoClient.getDatabase("zhou");  
-             MongoCollection<Document> mongoCollection = mongoDatabase.getCollection("user");  
- // insert a document
-             Document document = new Document("x", 1);  
-             mongoCollection.insertOne(document);  
-             document.append("x", 2).append("y", 3);  
- 
- // replace a document
-             mongoCollection.replaceOne(Filters.eq("_id", document.get("_id")), document);  
- 
- // find documents
-             List<Document> foundDocument = mongoCollection.find().into(new ArrayList<Document>());  
![](http://static.blog.csdn.net/images/save_snippets.png)
```java
MongoDatabase mongoDatabase = mongoClient.getDatabase("zhou");
            MongoCollection<Document> mongoCollection = mongoDatabase.getCollection("user");
            // insert a document
            Document document = new Document("x", 1);
            mongoCollection.insertOne(document);
            document.append("x", 2).append("y", 3);
            // replace a document
            mongoCollection.replaceOne(Filters.eq("_id", document.get("_id")), document);
            // find documents
            List<Document> foundDocument = mongoCollection.find().into(new ArrayList<Document>());
```
**insert adocument：**
**[java]**[view plain](http://blog.csdn.net/z69183787/article/details/50698172#)[copy](http://blog.csdn.net/z69183787/article/details/50698172#)[print](http://blog.csdn.net/z69183787/article/details/50698172#)[?](http://blog.csdn.net/z69183787/article/details/50698172#)![在CODE上查看代码片](https://code.csdn.net/assets/CODE_ico.png)[](https://code.csdn.net/snippets/1582477/fork)
- // insert a document
-             Document doc = new Document("name", "MongoDB")  
-                     .append("type", "database")  
-                     .append("count", 1)  
-                     .append("info", new Document("x", 203).append("y", 102));  
-             mongoCollection.insertOne(doc);  
![](http://static.blog.csdn.net/images/save_snippets.png)
```java
// insert a document
            Document doc = new Document("name", "MongoDB")
                    .append("type", "database")
                    .append("count", 1)
                    .append("info", new Document("x", 203).append("y", 102));
            mongoCollection.insertOne(doc);
```
**add mutiple document：**
**[java]**[view plain](http://blog.csdn.net/z69183787/article/details/50698172#)[copy](http://blog.csdn.net/z69183787/article/details/50698172#)[print](http://blog.csdn.net/z69183787/article/details/50698172#)[?](http://blog.csdn.net/z69183787/article/details/50698172#)![在CODE上查看代码片](https://code.csdn.net/assets/CODE_ico.png)[](https://code.csdn.net/snippets/1582477/fork)
- List<Document> documents = new ArrayList<Document>();  
- for (int i = 0; i < 5; i++) {  
-                 documents.add(new Document("i", i));  
-             }  
-             mongoCollection.insertMany(documents);  
![](http://static.blog.csdn.net/images/save_snippets.png)
```java
List<Document> documents = new ArrayList<Document>();
            for (int i = 0; i < 5; i++) {
                documents.add(new Document("i", i));
            }
            mongoCollection.insertMany(documents);
```
**count document in a collection：**
**[java]**[view plain](http://blog.csdn.net/z69183787/article/details/50698172#)[copy](http://blog.csdn.net/z69183787/article/details/50698172#)[print](http://blog.csdn.net/z69183787/article/details/50698172#)[?](http://blog.csdn.net/z69183787/article/details/50698172#)![在CODE上查看代码片](https://code.csdn.net/assets/CODE_ico.png)[](https://code.csdn.net/snippets/1582477/fork)
- for (Document cur : mongoCollection.find()) {  
-                 System.out.println(cur.toJson());  
-             }  
![](http://static.blog.csdn.net/images/save_snippets.png)
```java
for (Document cur : mongoCollection.find()) {
                System.out.println(cur.toJson());
            }
```
**query the collection：**
1、find first document
**[java]**[view plain](http://blog.csdn.net/z69183787/article/details/50698172#)[copy](http://blog.csdn.net/z69183787/article/details/50698172#)[print](http://blog.csdn.net/z69183787/article/details/50698172#)[?](http://blog.csdn.net/z69183787/article/details/50698172#)![在CODE上查看代码片](https://code.csdn.net/assets/CODE_ico.png)[](https://code.csdn.net/snippets/1582477/fork)
- Document myDoc = mongoCollection.find().first();  
-             System.out.println(myDoc.toJson());  
![](http://static.blog.csdn.net/images/save_snippets_01.png)
```java
Document myDoc = mongoCollection.find().first();
            System.out.println(myDoc.toJson());
```
2、find all documents in a collection： 
**[java]**[view plain](http://blog.csdn.net/z69183787/article/details/50698172#)[copy](http://blog.csdn.net/z69183787/article/details/50698172#)[print](http://blog.csdn.net/z69183787/article/details/50698172#)[?](http://blog.csdn.net/z69183787/article/details/50698172#)![在CODE上查看代码片](https://code.csdn.net/assets/CODE_ico.png)[](https://code.csdn.net/snippets/1582477/fork)
- MongoCursor<Document> cursor = mongoCollection.find().iterator();  
- try {  
- while (cursor.hasNext()) {  
-                     System.out.println(cursor.next().toJson());  
-                 }  
-             } finally {  
-                 cursor.close();  
-             }  
![](http://static.blog.csdn.net/images/save_snippets.png)
```java
MongoCursor<Document> cursor = mongoCollection.find().iterator();
            try {
                while (cursor.hasNext()) {
                    System.out.println(cursor.next().toJson());
                }
            } finally {
                cursor.close();
            }
```
**get a single document with a query filter：**
**[java]**[view plain](http://blog.csdn.net/z69183787/article/details/50698172#)[copy](http://blog.csdn.net/z69183787/article/details/50698172#)[print](http://blog.csdn.net/z69183787/article/details/50698172#)[?](http://blog.csdn.net/z69183787/article/details/50698172#)![在CODE上查看代码片](https://code.csdn.net/assets/CODE_ico.png)[](https://code.csdn.net/snippets/1582477/fork)
- Document myDoc = mongoCollection.find(Filters.eq("i", 2)).first();  
-             System.out.println(myDoc.toJson());  
![](http://static.blog.csdn.net/images/save_snippets_01.png)
```java
Document myDoc = mongoCollection.find(Filters.eq("i", 2)).first();
            System.out.println(myDoc.toJson());
```
**get a set of document with a query：**
**[java]**[view plain](http://blog.csdn.net/z69183787/article/details/50698172#)[copy](http://blog.csdn.net/z69183787/article/details/50698172#)[print](http://blog.csdn.net/z69183787/article/details/50698172#)[?](http://blog.csdn.net/z69183787/article/details/50698172#)![在CODE上查看代码片](https://code.csdn.net/assets/CODE_ico.png)[](https://code.csdn.net/snippets/1582477/fork)
- Block<Document> printBlock = new Block<Document>() {  
- @Override
- publicvoid apply(final Document document) {  
-                     System.out.println(document.toJson());  
-                 }  
-             };  
-             mongoCollection.find(Filters.gt("i", 1)).forEach(printBlock);  
-             mongoCollection.find(Filters.and(Filters.gt("i", 2), Filters.lte("i", 4))).forEach(printBlock);  
![](http://static.blog.csdn.net/images/save_snippets.png)
```java
Block<Document> printBlock = new Block<Document>() {
                @Override
                public void apply(final Document document) {
                    System.out.println(document.toJson());
                }
            };
            mongoCollection.find(Filters.gt("i", 1)).forEach(printBlock);
            mongoCollection.find(Filters.and(Filters.gt("i", 2), Filters.lte("i", 4))).forEach(printBlock);
```
**sort documents：**
**[java]**[view plain](http://blog.csdn.net/z69183787/article/details/50698172#)[copy](http://blog.csdn.net/z69183787/article/details/50698172#)[print](http://blog.csdn.net/z69183787/article/details/50698172#)[?](http://blog.csdn.net/z69183787/article/details/50698172#)![在CODE上查看代码片](https://code.csdn.net/assets/CODE_ico.png)[](https://code.csdn.net/snippets/1582477/fork)
- Document myDoc = mongoCollection.find(Filters.exists("i")).sort(Sorts.descending("i")).first();  
-             System.out.println(myDoc.toJson());  
![](http://static.blog.csdn.net/images/save_snippets.png)
```java
Document myDoc = mongoCollection.find(Filters.exists("i")).sort(Sorts.descending("i")).first();
            System.out.println(myDoc.toJson());
```
**projecting fields：**
**[java]**[view plain](http://blog.csdn.net/z69183787/article/details/50698172#)[copy](http://blog.csdn.net/z69183787/article/details/50698172#)[print](http://blog.csdn.net/z69183787/article/details/50698172#)[?](http://blog.csdn.net/z69183787/article/details/50698172#)![在CODE上查看代码片](https://code.csdn.net/assets/CODE_ico.png)[](https://code.csdn.net/snippets/1582477/fork)
- Document myDoc = mongoCollection.find().projection(Projections.excludeId()).first();  
-             System.out.println(myDoc.toJson());  
![](http://static.blog.csdn.net/images/save_snippets_01.png)
```java
Document myDoc = mongoCollection.find().projection(Projections.excludeId()).first();
            System.out.println(myDoc.toJson());
```
**Aggregations：**
**[java]**[view plain](http://blog.csdn.net/z69183787/article/details/50698172#)[copy](http://blog.csdn.net/z69183787/article/details/50698172#)[print](http://blog.csdn.net/z69183787/article/details/50698172#)[?](http://blog.csdn.net/z69183787/article/details/50698172#)![在CODE上查看代码片](https://code.csdn.net/assets/CODE_ico.png)[](https://code.csdn.net/snippets/1582477/fork)
- mongoCollection.aggregate(Arrays.asList(  
-                             Aggregates.match(Filters.gt("i", 0)),  
-                             Aggregates.project(Document.parse("{times: {$multiply: ['$i', 10]}}")))  
-             ).forEach(printBlock);  
-             Document myDoc = mongoCollection.aggregate(Collections.singletonList(Aggregates.group(null, Accumulators.sum("total", "$i")))).first();  
-             System.out.println(myDoc.toJson());  
![](http://static.blog.csdn.net/images/save_snippets.png)
```java
mongoCollection.aggregate(Arrays.asList(
                            Aggregates.match(Filters.gt("i", 0)),
                            Aggregates.project(Document.parse("{times: {$multiply: ['$i', 10]}}")))
            ).forEach(printBlock);
            Document myDoc = mongoCollection.aggregate(Collections.singletonList(Aggregates.group(null, Accumulators.sum("total", "$i")))).first();
            System.out.println(myDoc.toJson());
```
**updating document：**
**[java]**[view plain](http://blog.csdn.net/z69183787/article/details/50698172#)[copy](http://blog.csdn.net/z69183787/article/details/50698172#)[print](http://blog.csdn.net/z69183787/article/details/50698172#)[?](http://blog.csdn.net/z69183787/article/details/50698172#)![在CODE上查看代码片](https://code.csdn.net/assets/CODE_ico.png)[](https://code.csdn.net/snippets/1582477/fork)
- mongoCollection.updateOne(Filters.eq("i", 1), Updates.set("i", 110));  
![](http://static.blog.csdn.net/images/save_snippets_01.png)
```java
mongoCollection.updateOne(Filters.eq("i", 1), Updates.set("i", 110));
```
**[java]**[view plain](http://blog.csdn.net/z69183787/article/details/50698172#)[copy](http://blog.csdn.net/z69183787/article/details/50698172#)[print](http://blog.csdn.net/z69183787/article/details/50698172#)[?](http://blog.csdn.net/z69183787/article/details/50698172#)![在CODE上查看代码片](https://code.csdn.net/assets/CODE_ico.png)[](https://code.csdn.net/snippets/1582477/fork)
- UpdateResult updateResult = mongoCollection.updateMany(Filters.gte("i", 1), Updates.inc("i", 100));  
-             System.out.println(updateResult.getModifiedCount());  
![](http://static.blog.csdn.net/images/save_snippets.png)
```java
UpdateResult updateResult = mongoCollection.updateMany(Filters.gte("i", 1), Updates.inc("i", 100));
            System.out.println(updateResult.getModifiedCount());
```
**deleting document：**
**[java]**[view plain](http://blog.csdn.net/z69183787/article/details/50698172#)[copy](http://blog.csdn.net/z69183787/article/details/50698172#)[print](http://blog.csdn.net/z69183787/article/details/50698172#)[?](http://blog.csdn.net/z69183787/article/details/50698172#)![在CODE上查看代码片](https://code.csdn.net/assets/CODE_ico.png)[](https://code.csdn.net/snippets/1582477/fork)
- mongoCollection.deleteOne(Filters.eq("i", 210));  
-             DeleteResult deleteResult = mongoCollection.deleteMany(Filters.gte("i", 100));  
-             System.out.println(deleteResult.getDeletedCount());  
![](http://static.blog.csdn.net/images/save_snippets.png)
```java
mongoCollection.deleteOne(Filters.eq("i", 210));
            DeleteResult deleteResult = mongoCollection.deleteMany(Filters.gte("i", 100));
            System.out.println(deleteResult.getDeletedCount());
```
**bulk operations ：**
**[java]**[view plain](http://blog.csdn.net/z69183787/article/details/50698172#)[copy](http://blog.csdn.net/z69183787/article/details/50698172#)[print](http://blog.csdn.net/z69183787/article/details/50698172#)[?](http://blog.csdn.net/z69183787/article/details/50698172#)![在CODE上查看代码片](https://code.csdn.net/assets/CODE_ico.png)[](https://code.csdn.net/snippets/1582477/fork)
- // 1. Ordered bulk operation - order is guarenteed
-             mongoCollection.bulkWrite(  
-                     Arrays.asList(new InsertOneModel<Document>(new Document("_id", 4)),  
- new InsertOneModel<Document>(new Document("_id", 5)),  
- new InsertOneModel<Document>(new Document("_id", 6)),  
- new UpdateOneModel<Document>(new Document("_id", 1),  
- new Document("$set", new Document("x", 2))),  
- new DeleteOneModel<Document>(new Document("_id", 2)),  
- new ReplaceOneModel<Document>(new Document("_id", 3),  
- new Document("_id", 3).append("x", 4))));  
- 
- 
- // 2. Unordered bulk operation - no guarantee of order of operation
-             mongoCollection.bulkWrite(  
-                     Arrays.asList(new InsertOneModel<Document>(new Document("_id", 4)),  
- new InsertOneModel<Document>(new Document("_id", 5)),  
- new InsertOneModel<Document>(new Document("_id", 6)),  
- new UpdateOneModel<Document>(new Document("_id", 1),  
- new Document("$set", new Document("x", 2))),  
- new DeleteOneModel<Document>(new Document("_id", 2)),  
- new ReplaceOneModel<Document>(new Document("_id", 3),  
- new Document("_id", 3).append("x", 4))),  
- new BulkWriteOptions().ordered(false));  
![](http://static.blog.csdn.net/images/save_snippets.png)
```java
// 1. Ordered bulk operation - order is guarenteed
            mongoCollection.bulkWrite(
                    Arrays.asList(new InsertOneModel<Document>(new Document("_id", 4)),
                            new InsertOneModel<Document>(new Document("_id", 5)),
                            new InsertOneModel<Document>(new Document("_id", 6)),
                            new UpdateOneModel<Document>(new Document("_id", 1),
                                    new Document("$set", new Document("x", 2))),
                            new DeleteOneModel<Document>(new Document("_id", 2)),
                            new ReplaceOneModel<Document>(new Document("_id", 3),
                                    new Document("_id", 3).append("x", 4))));
            // 2. Unordered bulk operation - no guarantee of order of operation
            mongoCollection.bulkWrite(
                    Arrays.asList(new InsertOneModel<Document>(new Document("_id", 4)),
                            new InsertOneModel<Document>(new Document("_id", 5)),
                            new InsertOneModel<Document>(new Document("_id", 6)),
                            new UpdateOneModel<Document>(new Document("_id", 1),
                                    new Document("$set", new Document("x", 2))),
                            new DeleteOneModel<Document>(new Document("_id", 2)),
                            new ReplaceOneModel<Document>(new Document("_id", 3),
                                    new Document("_id", 3).append("x", 4))),
                    new BulkWriteOptions().ordered(false));
```
**Uploading to GridFS**
**uploadFromStream**
**[java]**[view plain](http://blog.csdn.net/z69183787/article/details/50698172#)[copy](http://blog.csdn.net/z69183787/article/details/50698172#)[print](http://blog.csdn.net/z69183787/article/details/50698172#)[?](http://blog.csdn.net/z69183787/article/details/50698172#)![在CODE上查看代码片](https://code.csdn.net/assets/CODE_ico.png)[](https://code.csdn.net/snippets/1582477/fork)
- GridFSBucket gridFSBucket = GridFSBuckets.create(mongoDatabase, "files");  
- // Get the input stream
-             InputStream streamToUploadFrom = new FileInputStream(new File("C:\\Users\\Administrator\\Desktop\\aa.jpg"));  
- 
- // Create some custom options
-             GridFSUploadOptions gridFSUploadOptions = new GridFSUploadOptions()  
-                     .chunkSizeBytes(1024)  
-                     .metadata(new Document("type", "presentation"));  
- 
-             ObjectId fileId = gridFSBucket.uploadFromStream("mongodb-tutorial", streamToUploadFrom, gridFSUploadOptions);  
-             System.out.println(fileId);  
![](http://static.blog.csdn.net/images/save_snippets.png)
```java
GridFSBucket gridFSBucket = GridFSBuckets.create(mongoDatabase, "files");
            // Get the input stream
            InputStream streamToUploadFrom = new FileInputStream(new File("C:\\Users\\Administrator\\Desktop\\aa.jpg"));
// Create some custom options
            GridFSUploadOptions gridFSUploadOptions = new GridFSUploadOptions()
                    .chunkSizeBytes(1024)
                    .metadata(new Document("type", "presentation"));
            ObjectId fileId = gridFSBucket.uploadFromStream("mongodb-tutorial", streamToUploadFrom, gridFSUploadOptions);
            System.out.println(fileId);
```
**openUploadStream**

**[java]**[view plain](http://blog.csdn.net/z69183787/article/details/50698172#)[copy](http://blog.csdn.net/z69183787/article/details/50698172#)[print](http://blog.csdn.net/z69183787/article/details/50698172#)[?](http://blog.csdn.net/z69183787/article/details/50698172#)![在CODE上查看代码片](https://code.csdn.net/assets/CODE_ico.png)[](https://code.csdn.net/snippets/1582477/fork)
- byte[] data = "Data to upload into GridFS".getBytes(StandardCharsets.UTF_8);  
-            GridFSUploadStream uploadStream = gridFSBucket.openUploadStream("sampleData", gridFSUploadOptions);  
-            uploadStream.write(data);  
-            uploadStream.close();  
-            System.out.println("The fileId of the uploaded file is: " + uploadStream.getFileId().toHexString());  
![](http://static.blog.csdn.net/images/save_snippets.png)
```java
byte[] data = "Data to upload into GridFS".getBytes(StandardCharsets.UTF_8);
            GridFSUploadStream uploadStream = gridFSBucket.openUploadStream("sampleData", gridFSUploadOptions);
            uploadStream.write(data);
            uploadStream.close();
            System.out.println("The fileId of the uploaded file is: " + uploadStream.getFileId().toHexString());
```
**Finding files stored in GridFS**
**[java]**[view plain](http://blog.csdn.net/z69183787/article/details/50698172#)[copy](http://blog.csdn.net/z69183787/article/details/50698172#)[print](http://blog.csdn.net/z69183787/article/details/50698172#)[?](http://blog.csdn.net/z69183787/article/details/50698172#)![在CODE上查看代码片](https://code.csdn.net/assets/CODE_ico.png)[](https://code.csdn.net/snippets/1582477/fork)
- gridFSBucket.find(Filters.eq("metadata.type", "presentation")).forEach(  
- new Block<GridFSFile>() {  
- @Override
- publicvoid apply(final GridFSFile gridFSFile) {  
-                             System.out.println(gridFSFile.getFilename());  
-                         }  
-                     });  
![](http://static.blog.csdn.net/images/save_snippets.png)
```java
gridFSBucket.find(Filters.eq("metadata.type", "presentation")).forEach(
                    new Block<GridFSFile>() {
                        @Override
                        public void apply(final GridFSFile gridFSFile) {
                            System.out.println(gridFSFile.getFilename());
                        }
                    });
```
**Downloading from GridFS**
**DownloadFromStream**
**[java]**[view plain](http://blog.csdn.net/z69183787/article/details/50698172#)[copy](http://blog.csdn.net/z69183787/article/details/50698172#)[print](http://blog.csdn.net/z69183787/article/details/50698172#)[?](http://blog.csdn.net/z69183787/article/details/50698172#)![在CODE上查看代码片](https://code.csdn.net/assets/CODE_ico.png)[](https://code.csdn.net/snippets/1582477/fork)
- ObjectId fileId = new ObjectId("56cbfe4200b3c32c30a36066");  
-             FileOutputStream streamToDownloadTo = new FileOutputStream("C:\\Users\\Administrator\\Desktop\\amyAcker.jpg");  
-             gridFSBucket.downloadToStream(fileId, streamToDownloadTo);  
-             streamToDownloadTo.close();  
-             System.out.println(streamToDownloadTo.toString());  
![](http://static.blog.csdn.net/images/save_snippets.png)
```java
ObjectId fileId = new ObjectId("56cbfe4200b3c32c30a36066");
            FileOutputStream streamToDownloadTo = new FileOutputStream("C:\\Users\\Administrator\\Desktop\\amyAcker.jpg");
            gridFSBucket.downloadToStream(fileId, streamToDownloadTo);
            streamToDownloadTo.close();
            System.out.println(streamToDownloadTo.toString());
```
**DownloadToStreamByName**
**[java]**[view plain](http://blog.csdn.net/z69183787/article/details/50698172#)[copy](http://blog.csdn.net/z69183787/article/details/50698172#)[print](http://blog.csdn.net/z69183787/article/details/50698172#)[?](http://blog.csdn.net/z69183787/article/details/50698172#)![在CODE上查看代码片](https://code.csdn.net/assets/CODE_ico.png)[](https://code.csdn.net/snippets/1582477/fork)
-             FileOutputStream streamToDownloadTo = new FileOutputStream("C:\\Users\\Administrator\\Desktop\\amyAcker.jpg");  
-             GridFSDownloadByNameOptions downloadOptions = new GridFSDownloadByNameOptions().revision(0);  
-             gridFSBucket.downloadToStreamByName("mongodb-tutorial", streamToDownloadTo, downloadOptions);  
-             streamToDownloadTo.close();<strong>  
- </strong>  
![](http://static.blog.csdn.net/images/save_snippets.png)
```java
FileOutputStream streamToDownloadTo = new FileOutputStream("C:\\Users\\Administrator\\Desktop\\amyAcker.jpg");
            GridFSDownloadByNameOptions downloadOptions = new GridFSDownloadByNameOptions().revision(0);
            gridFSBucket.downloadToStreamByName("mongodb-tutorial", streamToDownloadTo, downloadOptions);
            streamToDownloadTo.close();<strong>
</strong>
```
**OpenDownloadStream**
**[java]**[view plain](http://blog.csdn.net/z69183787/article/details/50698172#)[copy](http://blog.csdn.net/z69183787/article/details/50698172#)[print](http://blog.csdn.net/z69183787/article/details/50698172#)[?](http://blog.csdn.net/z69183787/article/details/50698172#)![在CODE上查看代码片](https://code.csdn.net/assets/CODE_ico.png)[](https://code.csdn.net/snippets/1582477/fork)
- GridFSDownloadStream downloadStream = gridFSBucket.openDownloadStream(new ObjectId("56cc003a00b3c32a3474ba3f"));  
- int fileLength = (int) downloadStream.getGridFSFile().getLength();  
- byte[] bytesToWriteTo = newbyte[fileLength];  
-             downloadStream.read(bytesToWriteTo);  
-             downloadStream.close();  
- 
-             System.out.println(new String(bytesToWriteTo, StandardCharsets.UTF_8));  
![](http://static.blog.csdn.net/images/save_snippets.png)
```java
GridFSDownloadStream downloadStream = gridFSBucket.openDownloadStream(new ObjectId("56cc003a00b3c32a3474ba3f"));
            int fileLength = (int) downloadStream.getGridFSFile().getLength();
            byte[] bytesToWriteTo = new byte[fileLength];
            downloadStream.read(bytesToWriteTo);
            downloadStream.close();
            System.out.println(new String(bytesToWriteTo, StandardCharsets.UTF_8));
```
**OpenDownloadStreamByName**
**[java]**[view plain](http://blog.csdn.net/z69183787/article/details/50698172#)[copy](http://blog.csdn.net/z69183787/article/details/50698172#)[print](http://blog.csdn.net/z69183787/article/details/50698172#)[?](http://blog.csdn.net/z69183787/article/details/50698172#)![在CODE上查看代码片](https://code.csdn.net/assets/CODE_ico.png)[](https://code.csdn.net/snippets/1582477/fork)
- GridFSDownloadStream downloadStream = gridFSBucket.openDownloadStreamByName("sampleData");  
- int fileLength = (int) downloadStream.getGridFSFile().getLength();  
- byte[] bytesToWriteTo = newbyte[fileLength];  
-             downloadStream.read(bytesToWriteTo);  
-             downloadStream.close();  
- 
-             System.out.println(new String(bytesToWriteTo, StandardCharsets.UTF_8));  
![](http://static.blog.csdn.net/images/save_snippets.png)
```java
GridFSDownloadStream downloadStream = gridFSBucket.openDownloadStreamByName("sampleData");
            int fileLength = (int) downloadStream.getGridFSFile().getLength();
            byte[] bytesToWriteTo = new byte[fileLength];
            downloadStream.read(bytesToWriteTo);
            downloadStream.close();
            System.out.println(new String(bytesToWriteTo, StandardCharsets.UTF_8));
```
**Renaming files**
**[java]**[view plain](http://blog.csdn.net/z69183787/article/details/50698172#)[copy](http://blog.csdn.net/z69183787/article/details/50698172#)[print](http://blog.csdn.net/z69183787/article/details/50698172#)[?](http://blog.csdn.net/z69183787/article/details/50698172#)![在CODE上查看代码片](https://code.csdn.net/assets/CODE_ico.png)[](https://code.csdn.net/snippets/1582477/fork)
- <span style="font-size:12px;">gridFSBucket.rename(new ObjectId("56cbfe4200b3c32c30a36066"), "mongodbTutorial");</span>  
![](http://static.blog.csdn.net/images/save_snippets.png)
```java
<span style="font-size:12px;">gridFSBucket.rename(new ObjectId("56cbfe4200b3c32c30a36066"), "mongodbTutorial");</span>
```
**Deleting files**
**[java]**[view plain](http://blog.csdn.net/z69183787/article/details/50698172#)[copy](http://blog.csdn.net/z69183787/article/details/50698172#)[print](http://blog.csdn.net/z69183787/article/details/50698172#)[?](http://blog.csdn.net/z69183787/article/details/50698172#)![在CODE上查看代码片](https://code.csdn.net/assets/CODE_ico.png)[](https://code.csdn.net/snippets/1582477/fork)
- gridFSBucket.delete(new ObjectId("56cc003a00b3c32a3474ba3f"));  
![](http://static.blog.csdn.net/images/save_snippets_01.png)
```java
gridFSBucket.delete(new ObjectId("56cc003a00b3c32a3474ba3f"));
```
**JMX Monitoring**
**[java]**[view plain](http://blog.csdn.net/z69183787/article/details/50698172#)[copy](http://blog.csdn.net/z69183787/article/details/50698172#)[print](http://blog.csdn.net/z69183787/article/details/50698172#)[?](http://blog.csdn.net/z69183787/article/details/50698172#)![在CODE上查看代码片](https://code.csdn.net/assets/CODE_ico.png)[](https://code.csdn.net/snippets/1582477/fork)
- <span style="font-size:10px;">publicclass TestCommandListener implements CommandListener {                          
- @Override
- publicvoid commandStarted(final CommandStartedEvent event) {                                                      
-         System.out.println(String.format("Sent command '%s:%s' with id %s to database '%s' "
-                                          + "on connection '%s' to server '%s'",                                        
-                                          event.getCommandName(),                                                       
-                                          event.getCommand().get(event.getCommandName()),                               
-                                          event.getRequestId(),                                                         
-                                          event.getDatabaseName(),                                                      
-                                          event.getConnectionDescription()                                              
-                                               .getConnectionId(),                                                      
-                                          event.getConnectionDescription().getServerAddress()));                        
-     }                                                                                                                  
- 
- @Override
- publicvoid commandSucceeded(final CommandSucceededEvent event) {                                                  
-         System.out.println(String.format("Successfully executed command '%s' with id %s "
-                                          + "on connection '%s' to server '%s'",                                        
-                                          event.getCommandName(),                                                       
-                                          event.getRequestId(),                                                         
-                                          event.getConnectionDescription()                                              
-                                               .getConnectionId(),                                                      
-                                          event.getConnectionDescription().getServerAddress()));                        
-     }                                                                                                                  
- 
- @Override
- publicvoid commandFailed(final CommandFailedEvent event) {                                                        
-         System.out.println(String.format("Failed execution of command '%s' with id %s "
-                                          + "on connection '%s' to server '%s' with exception '%s'",                    
-                                          event.getCommandName(),                                                       
-                                          event.getRequestId(),                                                         
-                                          event.getConnectionDescription()                                              
-                                               .getConnectionId(),                                                      
-                                          event.getConnectionDescription().getServerAddress(),                          
-                                          event.getThrowable()));                                                       
-     }                                                                                                                  
- }  </span>  
![](http://static.blog.csdn.net/images/save_snippets.png)
```java
<span style="font-size:10px;">public class TestCommandListener implements CommandListener {                        
    @Override                                                                                                        
    public void commandStarted(final CommandStartedEvent event) {                                                    
        System.out.println(String.format("Sent command '%s:%s' with id %s to database '%s' "                         
                                         + "on connection '%s' to server '%s'",                                      
                                         event.getCommandName(),                                                     
                                         event.getCommand().get(event.getCommandName()),                             
                                         event.getRequestId(),                                                       
                                         event.getDatabaseName(),                                                    
                                         event.getConnectionDescription()                                            
                                              .getConnectionId(),                                                    
                                         event.getConnectionDescription().getServerAddress()));                      
    }                                                                                                                
                                                                                                                     
    @Override                                                                                                        
    public void commandSucceeded(final CommandSucceededEvent event) {                                                
        System.out.println(String.format("Successfully executed command '%s' with id %s "                            
                                         + "on connection '%s' to server '%s'",                                      
                                         event.getCommandName(),                                                     
                                         event.getRequestId(),                                                       
                                         event.getConnectionDescription()                                            
                                              .getConnectionId(),                                                    
                                         event.getConnectionDescription().getServerAddress()));                      
    }                                                                                                                
                                                                                                                     
    @Override                                                                                                        
    public void commandFailed(final CommandFailedEvent event) {                                                      
        System.out.println(String.format("Failed execution of command '%s' with id %s "                              
                                         + "on connection '%s' to server '%s' with exception '%s'",                  
                                         event.getCommandName(),                                                     
                                         event.getRequestId(),                                                       
                                         event.getConnectionDescription()                                            
                                              .getConnectionId(),                                                    
                                         event.getConnectionDescription().getServerAddress(),                        
                                         event.getThrowable()));                                                     
    }                                                                                                                
}  </span>
```
**[java]**[view plain](http://blog.csdn.net/z69183787/article/details/50698172#)[copy](http://blog.csdn.net/z69183787/article/details/50698172#)[print](http://blog.csdn.net/z69183787/article/details/50698172#)[?](http://blog.csdn.net/z69183787/article/details/50698172#)![在CODE上查看代码片](https://code.csdn.net/assets/CODE_ico.png)[](https://code.csdn.net/snippets/1582477/fork)
- MongoClientOptions options = MongoClientOptions.builder()                              
-                                                .addCommandListener(new TestCommandListener())    
-                                                .build();      
![](http://static.blog.csdn.net/images/save_snippets.png)
```java
MongoClientOptions options = MongoClientOptions.builder()                            
                                               .addCommandListener(new TestCommandListener())  
                                               .build();
```

