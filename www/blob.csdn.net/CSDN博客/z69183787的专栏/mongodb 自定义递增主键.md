# mongodb 自定义递增主键 - z69183787的专栏 - CSDN博客
2016年10月26日 18:32:22[OkidoGreen](https://me.csdn.net/z69183787)阅读数：2761
```java
private final static String COLUMN_SEQ = "seq";
    private final static String COLUMN_CATEGORY = "category";
    private Mongo mongo;
    private String dbName;
    private String collName;
    public void setMongo(Mongo mongo) {
        this.mongo = mongo;
    }
    public void setDbName(String dbName) {
        this.dbName = dbName;
    }
    public void setCollName(String collName) {
        this.collName = collName;
    }
    @Override
    public long nextSeq(String category) {
        if (category == null || "".equals(category)) {
            throw new IllegalArgumentException("category can't be null or empty");
        }
        DBObject update = new BasicDBObject("$inc", new BasicDBObject(COLUMN_SEQ, 1L));
        DBObject query = new BasicDBObject(COLUMN_CATEGORY, category);
        return (Long) mongo.getDB(dbName).getCollection(collName).findAndModify(query, null, null, false, update, true,
                true).get(COLUMN_SEQ);
    }
```
