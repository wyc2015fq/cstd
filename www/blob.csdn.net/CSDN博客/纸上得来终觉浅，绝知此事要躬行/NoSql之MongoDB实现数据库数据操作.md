# NoSql之MongoDB实现数据库数据操作 - 纸上得来终觉浅，绝知此事要躬行 - CSDN博客





2013年03月12日 23:24:54[boonya](https://me.csdn.net/boonya)阅读数：3916







**1、创建唯一数据库实例DB**

```java
package com.boonya.mongo;
import java.net.UnknownHostException;
import java.util.Arrays;
import com.mongodb.DB;
import com.mongodb.MongoClient;
import com.mongodb.ServerAddress;


public class DBConnection {
	
	private DBConnection(){}
	
	private static String  userName="boonya";
	private static char [] password="boonya".toCharArray();
	private static  DB db=null;
	
	/**
	 * mongoClient = new MongoClient();
         * // or
         * mongoClient = new MongoClient( "localhost" );
         * // or
         * mongoClient = new MongoClient( "localhost" , 27017 );
         * // or, to connect to a replica set, supply a seed list of members
	 * @return
	 */
	public synchronized static DB getInstance(){
        if(db==null){
    		MongoClient mongoClient=null;
    		try {
    			mongoClient = new MongoClient(Arrays.asList(new ServerAddress("localhost", 27017),
    					new ServerAddress("localhost", 27018),
    					new ServerAddress("localhost", 27019)));
    		} catch (UnknownHostException e) {
    			e.printStackTrace();
    		}
    		db = mongoClient.getDB("mydb");
    		boolean auth = db.authenticate(userName,password);
            if(!auth){                      //the authentication is invalid
            	return null;
            }
        }
		return db;
	}

}
```
**2、创建CRUD操作工具类**

```java
package com.boonya.mongo.optimize;
import java.net.UnknownHostException;
import com.boonya.mongo.utils.ConstantUtils;
import com.mongodb.DB;
import com.mongodb.DBCollection;
import com.mongodb.DBCursor;
import com.mongodb.DBObject;
import com.mongodb.Mongo;
import com.mongodb.MongoException;
import com.mongodb.MongoOptions;


public class MongDbCRUD {
	
	private static Mongo mongo = null;
	private static DB db;
	private static DBCollection table;
	
	private static MongDbCRUD instace;
	
	public static synchronized MongDbCRUD getInstance(){
		if(instace==null||mongo==null||db==null||table==null){
			instace = new MongDbCRUD();
		}
		return instace;
	}
	
	public MongDbCRUD(){
		String host = ConstantUtils.getValue("mongo.host").toString();
		int port = Integer.parseInt(ConstantUtils.getValue("mongo.port").toString());
		String datablease = ConstantUtils.getValue("mongo.datablease").toString();
		String tablelename = ConstantUtils.getValue("mongo.table.bound").toString();
		
		try {
			mongo = new Mongo(host, port);
			// 连接池
			MongoOptions opt = mongo.getMongoOptions();
			opt.connectionsPerHost =  10  ; //poolsize
			opt.threadsAllowedToBlockForConnectionMultiplier =  10 ;
			
			//获取temp DB；如果默认没有创建，mongodb会自动创建
			db = mongo.getDB(datablease);
			//获取users DBCollection；如果默认没有创建，mongodb会自动创建
			table = db.getCollection(tablelename);
			//DO SOMETHING
		} catch (UnknownHostException e) {
			e.printStackTrace();
		} catch (MongoException e) {
			e.printStackTrace();
		}
	}
	
	public MongDbCRUD(String host, int port, String datablease, String tablelename){
		try {
			mongo = new Mongo(host, port);
			// 连接池
			MongoOptions opt = mongo.getMongoOptions();
			opt.connectionsPerHost =  10  ; //poolsize
			opt.threadsAllowedToBlockForConnectionMultiplier =  10 ;
			
			//获取temp DB；如果默认没有创建，mongodb会自动创建
			db = mongo.getDB(datablease);
			//获取users DBCollection；如果默认没有创建，mongodb会自动创建
			table = db.getCollection(tablelename);
			//DO SOMETHING		
		} catch (UnknownHostException e) {
			e.printStackTrace();
		} catch (MongoException e) {
			e.printStackTrace();
		}
	}
	
	public static void free(){
		if(mongo!=null){
			mongo.close();
		}
		mongo = null;
		db = null;
		table = null;
		System.gc();
	}
	
	public static DBCollection getDBCollection(){
		return table;
	}
	
	public void add(DBObject row){
		table.save(row);
	}
	
	public void query(){
		DBCursor cur = table.find();
		while (cur.hasNext()) { 
			DBObject dbobject = cur.next();
			System.out.println(dbobject.get("key"));//get value
		} 
	}
	
}
```

**3、mongo.properties内容**



```
#mongodb setting
mongo.host=192.168.20.38
mongo.port=27017
mongo.database=map
mongo.table.bound=bound
mongo.username=username
mongo.pwd=pwd
```







