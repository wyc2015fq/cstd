# NoSql之MongoDB的常用类管理 - 纸上得来终觉浅，绝知此事要躬行 - CSDN博客





2013年03月12日 23:39:30[boonya](https://me.csdn.net/boonya)阅读数：3268








**1、DBCollection的管理**


```java
package com.boonya.mongo;
import java.util.Set;
import com.mongodb.DBCollection;
public class CollectionManager {
 
 public static DBCollection  getDbCollection(String collectionName){
  return DBConnection.getInstance().getCollection(collectionName.equals("")?"myCollection":collectionName);
 }
 
 public String getCollectionNames(){
  String strNames="";
  Set<String> colls =DBConnection.getInstance().getCollectionNames();
  for (String cname : colls) {
   strNames+=cname+",";
  }
  return strNames;
 }

}
```

**2、并发操作管理**


```java
package com.boonya.mongo;
import com.mongodb.BasicDBObject;
import com.mongodb.DB;
import com.mongodb.DBCollection;
import com.mongodb.DBObject;
import com.mongodb.WriteConcern;

public class ConcurrencyManager {
 
 private static DBCollection coll= CollectionManager.getDbCollection("myCollection");
 
 /**
  * If you want to ensure complete consistency in a “session” (maybe an http request), 
  * you would want the driver to use the same socket, which you can achieve by using a
  * “consistent request”. Call requestStart() before your operations and requestDone()
  * to release the connection back to the pool:
  * DB and DBCollection are completely thread safe. In fact, they are cached so you get
  * the same instance no matter what
  */
 public void insertData(){
  DB db=DBConnection.getInstance();
  db.requestStart();
  try {
   db.requestEnsureConnection();
   //do something
  } finally {
   db.requestDone();
  }

 }
 
 /**
  * Since by default a connection is given back to the pool after each request, you may 
  * wonder how calling getLastError() works after a write. You should actually use a 
  * write concern like WriteConcern.SAFE instead of calling getLastError() manually. 
  * The driver will then call getLastError() before putting the connection back in the pool.
  * 
  * WriteConcern.NONE            No exceptions thrown. 
     * WriteConcern.NORMAL          Exceptions are only thrown when the primary node is unreachable for
     *                              a read, or the full replica set is unreachable. 
     * WriteConcern.SAFE            Same as the above, but exceptions thrown when there is a server error
     *                              on writes or reads. Calls getLastError(). 
     * WriteConcern.REPLICAS_SAFE   Tries to write to two separate nodes. Same as the above, but
     *                              will throw an exception if two writes are not possible. 
     * WriteConcern.FSYNC_SAFE      Same  as WriteConcern.SAFE, but also waits for write to be written to disk. 
     *
  */
 public void writeConcern(){
  coll.insert(new BasicDBObject("name","boonya").append("age", 21), WriteConcern.SAFE);

  // is equivalent to

  DB db=DBConnection.getInstance();
  DBCollection coll=CollectionManager.getDbCollection("myCollection");
  db.requestStart();
  try {
     coll.insert(new BasicDBObject("name","boonya").append("age", 21));
     DBObject err = db.getLastError();
     System.out.println(err);
  } finally {
     db.requestDone();
  }

 }

}
```

**3、数据库管理**


```java
package com.boonya.mongo;
import java.net.UnknownHostException;
import com.mongodb.MongoClient;
public class DatabaseManager {
 
 public String getDatabaseNames() throws UnknownHostException{
  String strNames="";
  MongoClient mongoClient;
  mongoClient = new MongoClient();
  for (String name : mongoClient.getDatabaseNames()) {
     strNames+=name+",";
  }
  return strNames;
 }
 
 public void  dropDatabase(String databaseName) throws UnknownHostException{
      MongoClient mongoClient = new MongoClient();
      mongoClient.dropDatabase(databaseName);
 }

}
```

**4、文档管理**


```java
package com.boonya.mongo;
import java.util.ArrayList;
import java.util.List;
import com.mongodb.BasicDBObject;
import com.mongodb.DBCollection;
import com.mongodb.DBCursor;
import com.mongodb.DBObject;
public class DocumentManager {
 
 private static DBCollection coll= CollectionManager.getDbCollection("myCollection");
 
 public void insertADocument(){
  BasicDBObject doc = new BasicDBObject("name", "MongoDB").append("type", "database")
   .append("count", 1)
   .append("info", new BasicDBObject("x", 203).append("y", 102));
         coll.insert(doc);
 }
 
 /**
  * PRINT BELOW:
  * { "_id" : "49902cde5162504500b45c2c" ,
     * "name" : "MongoDB" ,
     * "type" : "database" ,
     *  "count" : 1 ,
     *  "info" : { "x" : 203 , "y" : 102}}
  * @return
  */
 public DBObject findTheFirstDocument(){
  System.out.println("collection count:"+coll.getCount());
  DBObject myDoc = coll.findOne();
  System.out.println(myDoc);
        return myDoc;
 }
 
    public List<DBObject> getADocumentsByCursor(){
     List<DBObject> docs=new ArrayList<DBObject>();
     DBCursor cursor =coll.find();
     try {
        while(cursor.hasNext()) {
         DBObject obj=cursor.next();
            System.out.println(obj);
            docs.add(obj);
        }
        return docs;
     } finally {
        cursor.close();
     }
 }
    
    /**
     * PRINT LIKE THIS:
     * { "_id" : "49903677516250c1008d624e" , "i" : 71 }
     */
    public void queryADocument(){
     BasicDBObject query = new BasicDBObject("i", 71);
     DBCursor cursor =coll.find();
     cursor = coll.find(query);
     try {
        while(cursor.hasNext()) {
            System.out.println(cursor.next());
        }
     } finally {
        cursor.close();
     }
    }
    
    public void  editQueryRegular(){
     //db.things.find({j: {$ne: 3}, k: {$gt: 10} });
     BasicDBObject query = new BasicDBObject("j", new BasicDBObject("$ne", 3)
            .append("k", new BasicDBObject("$gt", 10)));
     DBCursor cursor = coll.find(query);
        try {
         while(cursor.hasNext()) {
           System.out.println(cursor.next());
            }
        } finally {
         cursor.close();
        }
    }
    
    public void getAsetDocument(){
     BasicDBObject query = new BasicDBObject("i", new BasicDBObject("$gt", 50));  // e.g. find all where i > 50
//     query = new BasicDBObject("i", new BasicDBObject("$gt", 20).
//                append("$lte", 30));  // i.e.   20 < i <= 30
     DBCursor  cursor = coll.find(query);
     try {
        while(cursor.hasNext()) {
            System.out.println(cursor.next());
        }
     } finally {
        cursor.close();
     }
    }
 

}
```

**5、索引管理**


```java
package com.boonya.mongo;
import java.util.List;
import com.mongodb.BasicDBObject;
import com.mongodb.DBCollection;
import com.mongodb.DBObject;

public class IndexManager {

  private static DBCollection coll= CollectionManager.getDbCollection("myCollection");

  public void createIndex(){
    // create index on "i", ascending
    coll.createIndex(new BasicDBObject("i", 1));
  }

  public List<DBObject> getIndexList(){
    List<DBObject> list = coll.getIndexInfo();
    for (DBObject o : list) {
       System.out.println(o);
       list.add(o);
    }
    return list;
  }

}
```

**6、预读取机制**


```java
package com.boonya.mongo;
import com.mongodb.BasicDBObject;
import com.mongodb.DBCollection;
import com.mongodb.DBCursor;
import com.mongodb.DBObject;
import com.mongodb.ReadPreference;
public class ReadPreferences {

  private static DBCollection coll= CollectionManager.getDbCollection("myCollection");

 /**
  * ReadPreference.primary();
  * ReadPreference.primaryPreferred();
  * ReadPreference.secondary();
  * ReadPreference.secondaryPreferred();
  * ReadPreference.nearest();
  */
  public void read(){
    DBObject query = new BasicDBObject("name", "simple doc");

    //Read from primary if available, otherwise a secondary.
    ReadPreference preference = ReadPreference.primaryPreferred();
    DBCursor cur = new DBCursor(coll, query, (DBObject) preference);

    //Read from any member node from the set of nodes which respond the fastest. 
    //The responsiveness of a node is measured with pings. Any node whose ping 
    //time is within 15 milliseconds of the node with the lowest ping time is considered near.
    DBObject result =coll.findOne(query, (DBObject) ReadPreference.nearest());


    // initialize a properly tagged read preference
    ReadPreference tagged_pref =ReadPreference.secondaryPreferred(new BasicDBObject("datacenter", "Los Angeles"));

    // include the tagged read preference in this request}}
    DBObject result1=coll.findOne(new BasicDBObject("name", "simple doc"), (DBObject) tagged_pref);

    // read from either LA or US_West
    DBObject tagSetOne = new BasicDBObject("datacenter", "Los Angeles");
    DBObject tagSetTwo = new BasicDBObject("region", "US_West");
    ReadPreference pref =ReadPreference.primaryPreferred(tagSetOne, tagSetTwo);

 }

}
```






