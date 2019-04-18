# MongoDB 3.0 for java 学习记录（一、java连接MongoDB） - z69183787的专栏 - CSDN博客
2016年02月15日 16:32:44[OkidoGreen](https://me.csdn.net/z69183787)阅读数：1155
1、当MongoDB没有添加使用auth认证方式的时候：
> 
  MongoClientURI uri = new MongoClientURI("mongodb://192.168.100.140:27017/local",
                MongoClientOptions.builder().cursorFinalizerEnabled(false));
  MongoClient client = new MongoClient(uri);
  MongoDatabase db=client.getDatabase("local");
  MongoCollection<Document> collection = db.getCollection("system.users");
  List<Document> foundDocument = collection.find().into(new ArrayList<Document>());
  System.out.println(foundDocument);
2、当MongoDB启动的时候添加--auth参数启动MongoDB（即安全认证模式下）
> 
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
mongoDB 3.0 安全权限访问控制：http://blog.csdn.net/gsying1474/article/details/47813059
