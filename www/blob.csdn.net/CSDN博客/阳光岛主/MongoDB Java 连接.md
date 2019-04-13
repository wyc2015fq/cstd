
# MongoDB Java 连接 - 阳光岛主 - CSDN博客

2014年01月15日 22:41:54[阳光岛主](https://me.csdn.net/sunboy_2050)阅读数：13259


Java 连接MongoDB

```python
package mymaven;
import java.net.UnknownHostException;
import java.util.Set;
import com.mongodb.BasicDBObject;
import com.mongodb.DB;
import com.mongodb.DBCollection;
import com.mongodb.DBCursor;
import com.mongodb.DBObject;
import com.mongodb.Mongo;
import com.mongodb.MongoException;
public class Test {
	/**
	 * @author ithomer.net
	 * @datetime 2013-11-11
	 */
	public static void main(String[] args) throws UnknownHostException, MongoException {
		Mongo mongo = new Mongo("172.27.9.104", 27017);		// 连接数据库
		DB db = mongo.getDB("mytestdb");					// 数据库
		Set<String> cols = db.getCollectionNames();			// 获取数据库中所有的集合（类似于获取关系数据库中的表）
		
		// 打印出数据库中的集合，这里应当为null
		for (String s : cols) {
			System.out.println(s);
		}
		
		DBCollection collection = db.getCollection("mytestcoll");		// 创建一个集合
		collection.drop();												// 删除集合，插入数据时自动重建
		BasicDBObject obj = new BasicDBObject();						// 初始化一个基本DB对象，插入数据库的就是DB对象
		
		obj.put("from", "blog.ithomer.net");		// 放入几个键值对
		obj.put("to", "forum.ithomer.net");
		obj.put("subject", "ithomer.net");
		collection.insert(obj);						// 插入对象
		
		DBObject dbobj = collection.findOne();		// 查看一条记录，findOne()=find().limit(1);
		System.out.println(dbobj);					// 打印出刚才插入的数据
		
		// 插入10条{ranking:i}的数据
		for (int i = 0; i < 10; i++) {
			collection.insert(new BasicDBObject().append("ranking", i));
		}
		System.out.println("count: " + collection.getCount());		// 打印集合中的数据总数
		
		DBCursor cursor = collection.find();		// 然后我们使用这个游标来遍历集合
		while (cursor.hasNext()) {
			System.out.println(cursor.next());
		}
		// 简单的条件查询，查询ranking为1的记录
		BasicDBObject query = new BasicDBObject();
		query.put("ranking", 1);
		cursor = collection.find(query);
		System.out.println("collection find({\"ranking\":1}):");
		while (cursor.hasNext()) {
			System.out.println(cursor.next());
		}
		
		// 复杂的条件查询，查询ranking大于等于5小于9的记录
		query = new BasicDBObject();
		query.put("ranking", new BasicDBObject("$gte", 5).append("$lt", 9));
		cursor = collection.find(query);
		System.out.println("collection find({\"ranking\":[5-9)}):");
		while (cursor.hasNext()) {
			System.out.println(cursor.next());
		}
		
//		mongo.dropDatabase("mytestdb");			// 最后删除我们的测试数据库
	}
}
```

运行结果：

```python
mytestcoll
system.indexes
{ "_id" : { "$oid" : "52c62ed8e4b0f4de3dd10107"} , "from" : "blog.ithomer.net" , "to" : "forum.ithomer.net" , "subject" : "ithomer.net"}
count: 11
{ "_id" : { "$oid" : "52c62ed8e4b0f4de3dd10107"} , "from" : "blog.ithomer.net" , "to" : "forum.ithomer.net" , "subject" : "ithomer.net"}
{ "_id" : { "$oid" : "52c62ed8e4b0f4de3dd10108"} , "ranking" : 0}
{ "_id" : { "$oid" : "52c62ed8e4b0f4de3dd10109"} , "ranking" : 1}
{ "_id" : { "$oid" : "52c62ed8e4b0f4de3dd1010a"} , "ranking" : 2}
{ "_id" : { "$oid" : "52c62ed8e4b0f4de3dd1010b"} , "ranking" : 3}
{ "_id" : { "$oid" : "52c62ed8e4b0f4de3dd1010c"} , "ranking" : 4}
{ "_id" : { "$oid" : "52c62ed8e4b0f4de3dd1010d"} , "ranking" : 5}
{ "_id" : { "$oid" : "52c62ed8e4b0f4de3dd1010e"} , "ranking" : 6}
{ "_id" : { "$oid" : "52c62ed8e4b0f4de3dd1010f"} , "ranking" : 7}
{ "_id" : { "$oid" : "52c62ed8e4b0f4de3dd10110"} , "ranking" : 8}
{ "_id" : { "$oid" : "52c62ed8e4b0f4de3dd10111"} , "ranking" : 9}
collection find({"ranking":1}):
{ "_id" : { "$oid" : "52c62ed8e4b0f4de3dd10109"} , "ranking" : 1}
collection find({"ranking":[5-9)}):
{ "_id" : { "$oid" : "52c62ed8e4b0f4de3dd1010d"} , "ranking" : 5}
{ "_id" : { "$oid" : "52c62ed8e4b0f4de3dd1010e"} , "ranking" : 6}
{ "_id" : { "$oid" : "52c62ed8e4b0f4de3dd1010f"} , "ranking" : 7}
{ "_id" : { "$oid" : "52c62ed8e4b0f4de3dd10110"} , "ranking" : 8}
```
[源码下载](http://download.csdn.net/detail/sunboy_2050/6803207)（maven）


**mongodb数组示例：**
```python
@SuppressWarnings("unchecked")
	public static void loadMediaTags(List<MediaEntity> mediaEntityList) {
		mediaEntityList.clear();
		
		try {
			Mongo mongo = new Mongo(CosineCluster.gMongo_HOST, CosineCluster.gMongo_PORT);
			DB db = mongo.getDB(CosineCluster.gMongo_DB);
			DBCollection collection = db.getCollection(CosineCluster.gMongo_Coll_Media);
			
			DBCursor cursor = collection.find();
			int index = 0;
			long startTime = System.currentTimeMillis();
			while(cursor.hasNext()){
				BasicDBObject obj = (BasicDBObject) cursor.next();
				
				long id = obj.getLong("_id");
				ArrayList<String> tagList = (ArrayList<String>) obj.get("tag");					// tag
				ArrayList<String> keywordList = (ArrayList<String>)obj.get("keyword");			// keyword
				ArrayList<Integer> copyrightList = (ArrayList<Integer>)obj.get("copyright");	// copyright
				
		
				MediaEntity mediaEntity = new MediaEntity();
				
				mediaEntity.setId(id);
				
				// tag
				for(int j=0; j<tagList.size(); j++) {
					mediaEntity.addTag(tagList.get(j));
					int tagId = getTagInt(tagList.get(j));
					mediaEntity.addTag(tagId);
				}
				
				// actors
				ArrayList<DBObject> actorsObjList = (ArrayList<DBObject>)obj.get("actors");		// actors
				for(int j=0; j<actorsObjList.size(); j++) {
					 mediaEntity.addActor((String) actorsObjList.get(j).get("name"));	
					 int actorId = getActorInt((String)actorsObjList.get(j).get("name"));
					 mediaEntity.addActor(actorId);
				}
				// director
				ArrayList<DBObject> directorObjList = (ArrayList<DBObject>)obj.get("director");	// director
				for(int j=0; j<directorObjList.size(); j++) {
					 mediaEntity.addDirector((String) directorObjList.get(j).get("name"));	
					 int directorId = getDirectorInt((String) directorObjList.get(j).get("name"));
					 mediaEntity.addDirector(directorId);
				}
				
				// keyword
				for(int j=0; j<keywordList.size(); j++) {
					mediaEntity.addKeyword(keywordList.get(j));
					int keywordId = getKeywordInt(keywordList.get(j));
					mediaEntity.addKeyword(keywordId);
				}
				
				// copyright
				for(int j=0; j<copyrightList.size(); j++) {
					mediaEntity.addCopyright(copyrightList.get(j));
				}
				
				mediaEntityList.add(mediaEntity);
				
				index++;
				if(index > 100) {
					break;
				}
				System.out.println(index + " --- mediaEntity : " + mediaEntity.toString());
			}
			long costTime = System.currentTimeMillis() - startTime;
			System.out.println("load data costTime = " + index + "; costTime = " + costTime/1000f);
			
		} catch (Exception e) {
			e.printStackTrace();
		}
	}
	
	public static int getTagInt(String tag) {
		int tagIntId = -1;
		
		try {
			MongoClient mongo = new MongoClient(CosineCluster.gMongo_HOST, CosineCluster.gMongo_PORT);
			DB db = mongo.getDB(CosineCluster.gMongo_DB);
			DBCollection tagmapCollection = db.getCollection("recommend_tag_map");
			DBCursor cursor = tagmapCollection.find(new BasicDBObject("name", tag));	
			if(cursor == null || cursor.toArray().size() <= 0) {		// 处理小于2或n的映射关键字，下同
				return tagIntId;
			}
			DBObject obj = cursor.toArray().get(0);
			
			
			String name = tag;
			tagIntId = (Integer) obj.get("id");
			int num = (Integer) obj.get("num");
			
			mongo.close();
		} catch (UnknownHostException e) {
			e.printStackTrace();
		}
		
		return tagIntId;
	}
	public static int getActorInt(String actor) {
		int actorIntId = -1;
		try {
			MongoClient mongo = new MongoClient(CosineCluster.gMongo_HOST, CosineCluster.gMongo_PORT);
			DB db = mongo.getDB(CosineCluster.gMongo_DB);
			DBCollection tagmapCollection = db.getCollection("recommend_actor_map");
			
			DBCursor cursor = tagmapCollection.find(new BasicDBObject("name", actor));
			if(cursor == null || cursor.toArray().size() <= 0) {
				return actorIntId;
			}
			DBObject obj = cursor.toArray().get(0);
			
			String name = actor;
			actorIntId = (Integer) obj.get("id");
			int num = (Integer) obj.get("num");
			
			mongo.close();
		} catch (UnknownHostException e) {
			e.printStackTrace();
		}
		
		return actorIntId;
	}
```

**参考推荐：**
[Mongodb快速入门之使用Java操作Mongodb](http://tech.it168.com/a2011/0617/1206/000001206231_all.shtml)



