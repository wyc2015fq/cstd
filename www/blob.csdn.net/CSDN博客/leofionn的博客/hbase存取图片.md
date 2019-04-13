
# hbase存取图片 - leofionn的博客 - CSDN博客


2018年07月02日 23:37:31[leofionn](https://me.csdn.net/qq_36142114)阅读数：161


import java.io.IOException;
import org.apache.hadoop.conf.Configuration;
import org.apache.hadoop.hbase.HBaseConfiguration;
import org.apache.hadoop.hbase.HColumnDescriptor;
import org.apache.hadoop.hbase.HTableDescriptor;
import org.apache.hadoop.hbase.MasterNotRunningException;
import org.apache.hadoop.hbase.TableName;
import org.apache.hadoop.hbase.ZooKeeperConnectionException;
import org.apache.hadoop.hbase.client.Get;
import org.apache.hadoop.hbase.client.HBaseAdmin;
import org.apache.hadoop.hbase.client.HTable;
import org.apache.hadoop.hbase.client.HTablePool;
import org.apache.hadoop.hbase.client.Put;
import org.apache.hadoop.hbase.client.Result;
import org.apache.hadoop.hbase.util.Bytes;
import java.awt.image.BufferedImage;
import java.io.ByteArrayInputStream;
import java.io.ByteArrayOutputStream;
import java.io.File;
import java.io.IOException;
import javax.imageio.ImageIO;
import sun.misc.BASE64Decoder;
import sun.misc.BASE64Encoder;

public class OperationTable {
static BASE64Encoder encoder = new sun.misc.BASE64Encoder();
static BASE64Decoder decoder = new sun.misc.BASE64Decoder();
private static Configuration conf = null;
static {
conf = HBaseConfiguration.create();
conf.set("hbase.zookeeper.quorum", "master");// 使用eclipse时必须添加这个，否则无法定位master需要配置hosts
conf.set("hbase.zookeeper.property.clientPort", "2181");
}

public static void main(String[] args) throws IOException {
//String a=getImageBinary();
//                String[] cols = { "title1"};
//                String[] colsValue = {a};
//                base64StringToImage(a);
//
// 创建表
//createTable("blog");
// 添加值
//addData("qqq", "blog1", cols, colsValue);
String a=getData("blog1", "qqq","article","title1");
base64StringToImage(a,"ww");
}
//在Hasee里建立表
private static void createTable(String TableName) throws MasterNotRunningException,
ZooKeeperConnectionException, IOException {

HBaseAdmin admin = new HBaseAdmin(conf);// 新建一个数据库管理员//新api
if (admin.tableExists(TableName.valueOf("LogTable"))) {
System.out.println("table is not exist!");
System.exit(0);
} else {

HTableDescriptor desc = new HTableDescriptor(
TableName.valueOf(TableName));
desc.addFamily(new HColumnDescriptor("article"));

admin.createTable(desc);
admin.close();
System.out.println("create table Success!");
}
}

//插入多条数据
private static void addData(String rowKey, String tableName,String[] column1, String[] value1) throws IOException {
Put put = new Put(Bytes.toBytes(rowKey));// 设置rowkey
HTable table = new HTable(conf, Bytes.toBytes(tableName));// HTabel负责跟记录相关的操作如增删改查等//
// 获取表
HColumnDescriptor[] columnFamilies = table.getTableDescriptor() // 获取所有的列族
.getColumnFamilies();

for (int i = 0; i < columnFamilies.length; i++) {
String familyName = columnFamilies[i].getNameAsString(); // 获取列族名
if (familyName.equals("article")) { // article列族put数据
for (int j = 0; j < column1.length; j++) {
put.add(Bytes.toBytes(familyName),
Bytes.toBytes(column1[j]), Bytes.toBytes(value1[j]));
}
}

}
table.put(put);
System.out.println("add data Success!");
}
//将一张照片转化成二进制返回
static String getImageBinary() {
File f = new File("E:/1.jpg");
BufferedImage bi;
try {
bi = ImageIO.read(f);
ByteArrayOutputStream baos = new ByteArrayOutputStream();
ImageIO.write(bi, "jpg", baos);  //经测试转换的图片是格式这里就什么格式，否则会失真
byte[] bytes = baos.toByteArray();
return encoder.encodeBuffer(bytes).trim();
} catch (IOException e) {
e.printStackTrace();
}
return null;
}
//将二进制转换成照片存进电脑
static void base64StringToImage(String base64String,String name) {
try {
byte[] bytes1 = decoder.decodeBuffer(base64String);
ByteArrayInputStream bais = new ByteArrayInputStream(bytes1);
BufferedImage bi1 = ImageIO.read(bais);
File w2 = new File("e://"+name+".jpg");// 可以是jpg,png,gif格式
ImageIO.write(bi1, "jpg", w2);// 不管输出什么格式图片，此处不需改动
} catch (IOException e) {
e.printStackTrace();
}
}
//根据表名和行健查找图片的二进制
private static String getData( String tableName,String rowKey,String family,String qualifier) throws IOException {
HTable table = new HTable(conf, Bytes.toBytes(tableName));

// Instantiating Get class
Get g = new Get(Bytes.toBytes(rowKey));

// Reading the data
Result result = table.get(g);
String a = new String(result.getValue(family.getBytes(), qualifier.getBytes()));

System.out.println("get data Success!");
return a;
}
}

