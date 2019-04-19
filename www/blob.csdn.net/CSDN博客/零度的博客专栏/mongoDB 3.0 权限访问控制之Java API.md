# mongoDB 3.0 权限访问控制之Java API - 零度的博客专栏 - CSDN博客
2016年08月02日 10:33:19[零度anngle](https://me.csdn.net/zmx729618)阅读数：2666
1、当MongoDB没有添加-auth认证参数方式启动时：
```
MongoClientURI uri = new MongoClientURI("mongodb://192.168.100.140:27017/local",
                                           MongoClientOptions.builder().cursorFinalizerEnabled(false));
  MongoClient client = new MongoClient(uri);
  MongoDatabase db=client.getDatabase("local");
  MongoCollection<Document> collection = db.getCollection("system.users");
  List<Document> foundDocument = collection.find().into(new ArrayList<Document>());
  System.out.println(foundDocument);
```
2、当MongoDB添加--auth认证参数方式启动时：
```
MongoClient client = null;
  ServerAddress serverAddress = new ServerAddress("192.168.100.140",27017);
  List<ServerAddress> seeds = new ArrayList<ServerAddress>();
  seeds.add(serverAddress);
  MongoCredential credentials = MongoCredential.createScramSha1Credential("admin", "admin", "admin".toCharArray());
  List<MongoCredential> credentialsList = new ArrayList<MongoCredential>();
  credentialsList.add(credentials);
  client = new MongoClient(seeds, credentialsList);
  MongoDatabase db = client.getDatabase("admin");
  // MongoIterable<Document> collections=db.listCollections();
  MongoCollection<Document> collection = db.getCollection("system.users");
  List<Document> foundDocument = collection.find().into(
    new ArrayList<Document>());
  System.out.println(foundDocument);
```
