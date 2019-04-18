# Java连接SQL Server和MongoDb - Orisun - 博客园







# [Java连接SQL Server和MongoDb](https://www.cnblogs.com/zhangchaoyang/articles/1955487.html)





java操作SQL Server

```
1 package Db;
 2 
 3  import java.sql.*;
 4 
 5  publicclass javaConSQL {
 6 publicstaticvoid main(String[] args) {
 7         String JDriver ="com.microsoft.sqlserver.jdbc.SQLServerDriver"; // SQL数据库引擎
 8          String connectDB ="jdbc:sqlserver://192.168.0.117:1433;DatabaseName=租房查询系统"; // 数据源
 9  
10 try {
11             Class.forName(JDriver);// 加载数据库引擎，返回给定字符串名的类
12         } catch (ClassNotFoundException e) {
13 // e.printStackTrace();
14             System.out.println("加载数据库引擎失败");
15             System.exit(0);
16         }
17         System.out.println("数据库驱动成功");
18 
19 try {
20             String user ="zcy";
21             String password ="zcy";
22             Connection con = DriverManager.getConnection(connectDB, user,
23                     password);// 连接数据库对象
24             System.out.println("连接数据库成功");
25             Statement stmt = con.createStatement();// 创建SQL命令对象
26 
27 // 创建表
28             System.out.println("开始创建表");
29             String query ="create table TABLE1(ID NCHAR(2),NAME NCHAR(10))";// 创建表SQL语句
30             stmt.executeUpdate(query);// 执行SQL命令对象
31             System.out.println("表创建成功");
32 
33 // 输入数据
34             System.out.println("开始插入数据");
35             String a1 ="INSERT INTO TABLE1 VALUES('1','旭哥')";// 插入数据SQL语句
36             String a2 ="INSERT INTO TABLE1 VALUES('2','伟哥')";
37             String a3 ="INSERT INTO TABLE1 VALUES('3','张哥')";
38             stmt.executeUpdate(a1);// 执行SQL命令对象
39             stmt.executeUpdate(a2);
40             stmt.executeUpdate(a3);
41             System.out.println("插入数据成功");
42 
43 // 读取数据
44             System.out.println("开始读取数据");
45             ResultSet rs = stmt.executeQuery("SELECT * FROM TABLE1");// 返回SQL语句查询结果集(集合)
46 // 循环输出每一条记录
47 while (rs.next()) {
48 // 输出每个字段
49                 System.out.println(rs.getString("ID") +"\t"
50 + rs.getString("NAME"));
51             }
52             System.out.println("读取完毕");
53 
54 // 关闭连接
55             stmt.close();// 关闭命令对象连接
56             con.close();// 关闭数据库连接
57         } catch (SQLException e) {
58             e.printStackTrace();
59 // System.out.println("数据库连接错误");
60             System.exit(0);
61         }
62     }
63 }
```

java操作MongoDb

```
1 package zcy;
 2 
 3 import java.io.*;
 4 import java.net.*;
 5 import java.util.*;
 6 import com.mongodb.*;
 7 import com.mongodb.gridfs.*;
 8 
 9 publicclass AddData {
10 publicstaticvoid main(String[] args) throws UnknownHostException,MongoException{
11 //建立数据库连接
12         Mongo m=new Mongo("127.0.0.1");
13 //指定使用的数据库
14         DB db=m.getDB("Rental");
15 //指定使用的数据集合
16         DBCollection coll=db.getCollection("HouseInfo");
17 //添加房屋基本信息
18         BasicDBObject doc=new BasicDBObject();
19         doc.put("title","二七 东立国际 一楼 精装小户 家电齐全 24栋—");        //消息标题
20         doc.put("date",new Date());                                    //发布日期
21         doc.put("seller", 0);                                        //seller为0表示是业主，为1表示是经纪人
22         doc.put("contact","13478523694");                            //联系方式
23         doc.put("regional","江岸二七路");                                //区域
24         doc.put("district","东立国际二期");                            //小区
25         doc.put("address","江岸区工农兵路与二七路交汇处");                //详细地址
26         doc.put("rent",1200);                                        //租金
27         doc.put("class","1室1厅1卫");                                    //整套户型
28         doc.put("basicinfo","普通住宅，精装修，朝向南北(第1层，共19层)");    //基本情况
29         doc.put("config","床/热水器/洗衣机/空调/冰箱/电视机/宽带");        //配置
30         doc.put("area",42);                                            //总面积
31         doc.put("credit",3);                                        //信用等级    
32     
33 //添加照片信息
34         ArrayList<GridFSFile> list=new ArrayList<GridFSFile>();
35         GridFS f=new GridFS(db);
36         File image=new File("/home/orisun/Rental/rental1.jpg");
37         GridFSFile infile;
38 try{
39             infile=f.createFile(image);
40             infile.put("title","卧室");
41             infile.put("describe","这是卧室，照片上显示的实际中都有");
42             infile.save();
43             list.add(infile);
44         }catch(IOException e){
45             e.printStackTrace();
46         }
47         image=new File("/home/orisun/Rental/rental2.jpg");
48 try{
49             infile=f.createFile(image);
50             infile.put("title","大厅");
51             infile.put("describe","这是大厅，照片上显示的实际中都有");
52             infile.save();
53             list.add(infile);
54         }catch(IOException e){
55             e.printStackTrace();
56         }
57         image=new File("/home/orisun/Rental/rental3.jpg");
58 try{
59             infile=f.createFile(image);
60             infile.put("title","卫生间");
61             infile.put("describe","这是卫生间，照片上显示的实际中都有");
62             infile.save();
63             list.add(infile);
64         }catch(IOException e){
65             e.printStackTrace();
66         }
67         
68         doc.put("photo", list);
69         coll.insert(doc);
70     }
71 }
```












