# MongoDB Java Driver 使用 - z69183787的专栏 - CSDN博客
2016年02月19日 15:10:46[OkidoGreen](https://me.csdn.net/z69183787)阅读数：4133
**1、connect & authenticate &SSL**
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
```java
// insert a document
            Document doc = new Document("name", "MongoDB")
                    .append("type", "database")
                    .append("count", 1)
                    .append("info", new Document("x", 203).append("y", 102));
            mongoCollection.insertOne(doc);
```
**add mutiple document：**
```java
List<Document> documents = new ArrayList<Document>();
            for (int i = 0; i < 5; i++) {
                documents.add(new Document("i", i));
            }
            mongoCollection.insertMany(documents);
```
**count document in a collection：**
```java
for (Document cur : mongoCollection.find()) {
                System.out.println(cur.toJson());
            }
```
**query the collection：**
1、find first document
```java
Document myDoc = mongoCollection.find().first();
            System.out.println(myDoc.toJson());
```
2、find all documents in a collection：
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
```java
Document myDoc = mongoCollection.find(Filters.eq("i", 2)).first();
            System.out.println(myDoc.toJson());
```
**get a set of document with a query：**
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
```java
Document myDoc = mongoCollection.find(Filters.exists("i")).sort(Sorts.descending("i")).first();
            System.out.println(myDoc.toJson());
```
**projecting fields：**
```java
Document myDoc = mongoCollection.find().projection(Projections.excludeId()).first();
            System.out.println(myDoc.toJson());
```
**Aggregations：**
```java
mongoCollection.aggregate(Arrays.asList(
                            Aggregates.match(Filters.gt("i", 0)),
                            Aggregates.project(Document.parse("{times: {$multiply: ['$i', 10]}}")))
            ).forEach(printBlock);
            Document myDoc = mongoCollection.aggregate(Collections.singletonList(Aggregates.group(null, Accumulators.sum("total", "$i")))).first();
            System.out.println(myDoc.toJson());
```
**updating document：**
```java
mongoCollection.updateOne(Filters.eq("i", 1), Updates.set("i", 110));
```
```java
UpdateResult updateResult = mongoCollection.updateMany(Filters.gte("i", 1), Updates.inc("i", 100));
            System.out.println(updateResult.getModifiedCount());
```
**deleting document：**
```java
mongoCollection.deleteOne(Filters.eq("i", 210));
            DeleteResult deleteResult = mongoCollection.deleteMany(Filters.gte("i", 100));
            System.out.println(deleteResult.getDeletedCount());
```
**bulk operations ：**
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
```java
byte[] data = "Data to upload into GridFS".getBytes(StandardCharsets.UTF_8);
            GridFSUploadStream uploadStream = gridFSBucket.openUploadStream("sampleData", gridFSUploadOptions);
            uploadStream.write(data);
            uploadStream.close();
            System.out.println("The fileId of the uploaded file is: " + uploadStream.getFileId().toHexString());
```
**Finding files stored in GridFS**
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
```java
ObjectId fileId = new ObjectId("56cbfe4200b3c32c30a36066");
            FileOutputStream streamToDownloadTo = new FileOutputStream("C:\\Users\\Administrator\\Desktop\\amyAcker.jpg");
            gridFSBucket.downloadToStream(fileId, streamToDownloadTo);
            streamToDownloadTo.close();
            System.out.println(streamToDownloadTo.toString());
```
**DownloadToStreamByName**
```java
FileOutputStream streamToDownloadTo = new FileOutputStream("C:\\Users\\Administrator\\Desktop\\amyAcker.jpg");
            GridFSDownloadByNameOptions downloadOptions = new GridFSDownloadByNameOptions().revision(0);
            gridFSBucket.downloadToStreamByName("mongodb-tutorial", streamToDownloadTo, downloadOptions);
            streamToDownloadTo.close();<strong>
</strong>
```
**OpenDownloadStream**
```java
GridFSDownloadStream downloadStream = gridFSBucket.openDownloadStream(new ObjectId("56cc003a00b3c32a3474ba3f"));
            int fileLength = (int) downloadStream.getGridFSFile().getLength();
            byte[] bytesToWriteTo = new byte[fileLength];
            downloadStream.read(bytesToWriteTo);
            downloadStream.close();
            System.out.println(new String(bytesToWriteTo, StandardCharsets.UTF_8));
```
**OpenDownloadStreamByName**
```java
GridFSDownloadStream downloadStream = gridFSBucket.openDownloadStreamByName("sampleData");
            int fileLength = (int) downloadStream.getGridFSFile().getLength();
            byte[] bytesToWriteTo = new byte[fileLength];
            downloadStream.read(bytesToWriteTo);
            downloadStream.close();
            System.out.println(new String(bytesToWriteTo, StandardCharsets.UTF_8));
```
**Renaming files**
```java
<span style="font-size:12px;">gridFSBucket.rename(new ObjectId("56cbfe4200b3c32c30a36066"), "mongodbTutorial");</span>
```
**Deleting files**
```java
gridFSBucket.delete(new ObjectId("56cc003a00b3c32a3474ba3f"));
```
**JMX Monitoring**
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
```java
MongoClientOptions options = MongoClientOptions.builder()                            
                                               .addCommandListener(new TestCommandListener())  
                                               .build();
```
