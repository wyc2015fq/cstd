
# 从客服端发送XML文档，服务端接收并存储到数据库对应的表单中练习题 - 李昆鹏的博客 - CSDN博客


2018年07月04日 19:03:55[李昆鹏](https://me.csdn.net/weixin_41547486)阅读数：186


---------------------------------------从客服端发送XML文档，服务端接收并存储到数据库对应的表单中练习题­----------------------------------------
|*<?xml version="1.0"?>
|*
|*<fileset fileNum="12122"  dirNum="78812" extendNameTypeNum="772">
|*    <fileGroup extendType="bmp"  fileNum="233">
|*           <file length="212" lastModified="2008-12-12  21:12:32" readonly="true"  parent="c:/a/d/b/d">mypic</file>
|*           <file length="2123"  lastModified="2008-12-12 21:12:32" readonly="false"  parent="c:/a/d/b/c">yourpic</file>
|*           ...
|*    </fileGroup>
|*    ...
|*</fileset>
|
**题目：通过客服端讲文件夹中所有文件的信息以xml文档传入服务端，再从服务端解析到数据库各个表单中**
**花了一天完成的结果大家不要笑我，看是简单其实很费时间。**
# 客服端
**客服端需要使用文件的操作和IO****流、TCP网络编程、多线程的技术。**
![](https://img-blog.csdn.net/20180704190334718?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3dlaXhpbl80MTU0NzQ4Ng==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)
## 第一步创建Customer类
Customer类是将文件夹中所有的文件信息写入到项目的src中，存储在xml文档中。代码如下：
|package cn.tx.customer.file;
|import java.io.File;
|import java.io.FileNotFoundException;
|import java.io.FileOutputStream;
|import java.io.IOException;
|import  java.io.UnsupportedEncodingException;
|import java.text.SimpleDateFormat;
|import java.util.ArrayList;
|import java.util.Date;
|import java.util.List;
|import java.util.TreeSet;
|import org.dom4j.Document;
|import org.dom4j.DocumentHelper;
|import org.dom4j.Element;
|import org.dom4j.io.OutputFormat;
|import org.dom4j.io.XMLWriter;
|/**
|* 客服端中将文件中所有文件的信息写入xml文档中的类
|*  @author 李昆鹏
|*
|*/
|public class Customer {
|/**
|* 给出一个文件对象将文件对象中的所有子文件信息写入xml中
|* @param file
|*/
|public  static File writerXML(File file) {
|File  returnFile = new File("src/XMLFile.xml");
|//创建xml文档对象
|Document  doc = DocumentHelper.createDocument();
|Element  root = doc.addElement("feliSet");
|//获得文件内的总文件数量并且添加给feliset元素的属性
|root.addAttribute("fileNum",getFileCount(file)+"");
|//获得文件内的总文件夹数量并且添加给feliset元素的属性
|root.addAttribute("dirNum",  String.valueOf(getDirCount(file)));
|//添加文件后缀名的数量属性
|root.addAttribute("extendNameTypeNum",  getPostfixCount(file)+"");
|//获得所有后缀名的集合
|TreeSet<String>  postfixSet = getPostfixSet(file);
|for  (String postfix : postfixSet) {
|Element  fileGroup = root.addElement("fileGroup")
|.addAttribute("extendType",  postfix)
|.addAttribute("fileNum",  Integer.toString(getPostfixFileCount(file, postfix)));
|//获得指定后缀名的文件集合
|List<File>  fileList =  getPostfixFile(file,  postfix);
|for  (File f : fileList) {
|fileGroup.addElement("file")
|.addAttribute("length",  Long.toString(f.length()))
|.addAttribute("lastModified",  getLastModified(f))
|.addAttribute("redonly",  isReadonly(f))
|.addAttribute("parent",  getParent(f))
|.setText(getFileName(f));
|}
|}
|FileOutputStream  out = null;
|XMLWriter  xw = null;
|try  {
|//创建字节输出流
|out  = new FileOutputStream(returnFile);
|//创建写入xml文件对象
|xw  = new XMLWriter(out,OutputFormat.createPrettyPrint());
|xw.write(doc);
|System.out.println("xml文件创建成功！");
|}  catch (FileNotFoundException e) {
|e.printStackTrace();
|}  catch (UnsupportedEncodingException e) {
|e.printStackTrace();
|}  catch (IOException e) {
|e.printStackTrace();
|}finally  {
|try  {
|if(xw  != null) {
|xw.close();
|}
|if(out  != null) {
|out.close();
|}
|}  catch (IOException e) {
|e.printStackTrace();
|}
|}
|return  returnFile;
|}
|/**
|* 返回文件是否是仅读的
|* @param file
|* @return
|*/
|public  static String isReadonly(File file) {
|boolean  isReadonly = file.canRead()&&!file.canWrite();
|return  Boolean.toString(isReadonly);
|}
|/**
|* 返回一个有多少文件
|* @param file 要查询的文件对象
|* @return 返回file中所有的文件数量
|*/
|public  static int getFileCount(File file) {
|//记录file中所有文件的变量
|int  fileCount = 0;
|if(file.isFile()  || !file.exists()) {
|return  0;
|}
|//获得file中所有的文件
|File[]  files = file.listFiles();
|//遍历file中所有的文件
|for  (File f : files) {
|if(f.isDirectory())  {
|fileCount  +=getFileCount(f);
|}  else {
|fileCount++;
|}
|}
|return  fileCount;
|}
|/**
|*
|* @param file 查询指定文件对象下移动有多少目录
|* @return 返回file下所有目录的数量
|*/
|public  static int getDirCount(File file) {
|//记录file中所有文件目录的变量
|int  dirCount = 0;
|//记录file中所有文件目录的变量
|if(file.isFile()  || !file.exists()) {
|return  0;
|}
|//获得file中所有的文件
|File[]  files = file.listFiles();
|//遍历file中所有的文件
|for  (File f : files) {
|if(f.isDirectory())  {
|dirCount++;
|dirCount+=getDirCount(f);
|}
|}
|return  dirCount;
|}
|/**
|* 获得文件内所有文件的后缀名
|* @param file
|* @return 返回文件中所有文件后缀名的集合
|*/
|public  static TreeSet<String> getPostfixSet(File file) {
|//建立存储后缀名的集合
|TreeSet<String>  setPostfix = new TreeSet<String>();
|if(file.isFile())  {
|//获得文件名包含后缀名
|String  fileName = file.getName();
|//获得文件后缀名的第一个索引
|int  index = fileName.lastIndexOf(".")+1;
|//截取文件名获得后缀名
|String  postfixName = fileName.substring(index);
|setPostfix.add(postfixName);
|return  setPostfix;
|}
|if(!file.exists())  {
|return  setPostfix;
|}
|File[]  files = file.listFiles();
|for  (File f : files) {
|if(f.isDirectory())  {
|setPostfix.addAll(getPostfixSet(f));
|}else  {
|//获得文件名包含后缀名
|String  fileName = f.getName();
|//获得文件后缀名的第一个索引
|int  index = fileName.lastIndexOf(".")+1;
|//截取文件名获得后缀名
|String  postfixName = fileName.substring(index);
|setPostfix.add(postfixName);
|}
|}
|return  setPostfix;
|}
|/**
|* 通过所有后缀名的集合获取到后缀名的数量
|* @param file
|* @return
|*/
|public  static int getPostfixCount(File file) {
|return  getPostfixSet(file).size();
|}
|/**
|* 根据后缀名获得所有同后缀名的文件对象集合
|* @param file
|* @param postfixName
|* @return
|*/
|public  static List<File> getPostfixFile(File file, String postfixName) {
|//创建存储文件的集合
|List<File>  fileList = new ArrayList<File>();
|if(!file.exists())  {
|return  fileList;
|}
|if(file.isFile())  {
|if(postfixName.equals(getPostfixName(file)))  {
|fileList.add(file);
|}
|return  fileList;
|}
|File[]  files = file.listFiles();
|for  (File f : files) {
|if(f.isDirectory())  {
|fileList.addAll(getPostfixFile(f,  postfixName));
|}else  {
|if(postfixName.equals(getPostfixName(f)))  {
|fileList.add(f);
|}
|}
|}
|return  fileList;
|}
|/**
|* 通过文件对象返回文件的后缀名
|* @param file 通过文件对象返回文件的后缀名
|* @return
|*/
|public  static String getPostfixName(File file) {
|String  postfixName = null;
|if(file.isFile()  && file.exists()) {
|String  str = file.getName();
|postfixName  = str.substring(str.lastIndexOf(".")+1);
|}  else {
|System.out.println("请检查你的文件是否有误");
|}
|return  postfixName;
|}
|/**
|* 获得file文件内指定后缀名的所有文件对象的数量
|* @param file
|* @param postfixName
|* @return
|*/
|public  static int getPostfixFileCount(File file, String postfixName) {
|return  getPostfixFile(file,postfixName).size();
|}
|/**
|* 获得文件的上级文件路径
|* @param file
|* @return
|*/
|public  static String getParent(File file) {
|String  str =   file.getAbsolutePath().substring(0,file.getAbsolutePath().lastIndexOf("\\")+1);
|return  str.replace("\\", "/");
|}
|/**
|* 获得文件的最后修改时间，并按指定的格式返回字符串
|* @param file
|* @return
|*/
|public  static String getLastModified(File file) {
|long  time = file.lastModified();
|Date  date = new Date(time);
|SimpleDateFormat  sdf = new SimpleDateFormat("yyyy-MM-dd HH:mm:ss");
|return  sdf.format(date);
|}
|/**
|* 返回去掉后缀名的文件名
|* @param file
|* @return
|*/
|public  static String getFileName(File file) {
|String  name = file.getName();
|int  index = name.lastIndexOf(".");
|if(file.isFile()  && index > 0) {
|return  name.substring(0,name.lastIndexOf("."));
|}  else {
|return  "";
|}
|}
|}
|

## CurtomerThread类
Curtomerthread类就是把已经写好的xml文档发送到服务器端的网络编程功能。代码如下：
|package cn.tx.customer.file;
|import java.io.BufferedInputStream;
|import java.io.BufferedOutputStream;
|import java.io.File;
|import java.io.FileInputStream;
|import java.io.IOException;
|import java.io.OutputStream;
|import java.net.Socket;
|import java.net.UnknownHostException;
|/**
|* 客服端网络传输xml文件类
|*  @author 李昆鹏
|*
|*/
|public class CurtomerThread {
|//ip地址
|private  String ip;
|//端口
|private  int poot;
|//文件对象名
|private  String file;
|public  CurtomerThread(String ip, int poot, String file) {
|super();
|this.ip  = ip;
|this.poot  = poot;
|this.file  = file;
|//生成xml文件
|//启动文件传输线程
|Sender  s = new Sender();
|Thread  t = new Thread(s);
|//启动线程
|t.start();
|}
|class  Sender implements Runnable{
|@Override
|public  void run() {
|Socket  s = null;
|BufferedInputStream  br = null;
|BufferedOutputStream  bw = null;
|try  {
|s  = new Socket(ip,poot);
|//加载xml文件到输入流
|br  = new BufferedInputStream(new FileInputStream(Customer.writerXML(new  File(file))));
|//从网络接口Socket中获取输出流通道
|OutputStream  os = s.getOutputStream();
|//创建高效缓冲输出流
|bw  = new BufferedOutputStream(os);
|//定义一个字节数组用来存储文档中读取的字节
|byte[]  bytes = new byte[1024];
|//定义一个变量用来接收每次读取到字节的数量
|int  len = 0;
|//循环读取文档，并且发送文档
|while((len=br.read(bytes))  != -1) {
|//写入到输出流
|bw.write(bytes,  0, len);
|//清空缓冲区
|bw.flush();
|}
|}  catch (UnknownHostException e) {
|e.printStackTrace();
|}  catch (IOException e) {
|e.printStackTrace();
|}  finally {
|try  {
|if(bw  != null) {
|bw.close();
|}
|if(br  != null) {
|br.close();
|}
|if(s  != null) {
|s.close();
|}
|}  catch (IOException e) {
|e.printStackTrace();
|}
|}
|}
|}
|}
|

## CustomerWindow类
customerWindow类是客服端使用的主窗口，将xml文档发送到服务端，完成这个类客服端就完工了，接下来是把数据库的表建好。
代码如下：
|package cn.tx.customer.file;
|import java.io.File;
|import java.util.Scanner;
|/**
|* 客户端主窗口
|*  @author 李昆鹏
|*
|*/
|public class CurtomerWindow {
|/**
|* 主窗口
|* @param args
|*/
|public  static void main(String[] args) {
|start();
|System.out.println("文件已传输完毕");
|}
|/**
|* 将文件中的信息已xml文档发送给服务端
|*/
|public  static void start() {
|String  file= null;
|Scanner  s = null;
|while(true)  {
|System.out.print("请按指定格式输入文件或者文件夹的路径(比如：E:/eclipse):");
|s  = new Scanner(System.in);
|file  = s.nextLine();
|if(!(new  File(file).exists())) {
|System.out.println("你的输入的文件不存在，请重新输入！");
|continue;
|}
|break;
|}
|System.out.print("请输入地址ip:");
|String  ip = s.nextLine();
|System.out.println(ip);
|System.out.println();
|System.out.println("请输入端口：");
|int  poot = s.nextInt();
|s.nextLine();
|System.out.println(poot);
|new  CurtomerThread(ip, poot, file);
|//new  CurtomerThread("192.168.1.103", 10005, "E:/李昆鹏/华南理工计算机");
|}
|}
|

# 数据库建表
我们需要三张表，分别存储总文件表，以后缀名分组的组文件表和文件表，建表的方法我就不多说了
![](https://img-blog.csdn.net/20180704190155599?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3dlaXhpbl80MTU0NzQ4Ng==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)

# 服务端
类有点多大家不要急，最主要的知识点还是JDBC、多线程、DOM4J、IO流
![](https://img-blog.csdn.net/20180704190206877?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3dlaXhpbl80MTU0NzQ4Ng==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)

## Server类服务端类
Server类用来接收客服端发送过来的XML文档，并且直接将xml文档中的信息写入数据库中
|package cn.tx.server;
|import java.io.BufferedInputStream;
|import java.io.BufferedOutputStream;
|import java.io.FileOutputStream;
|import java.io.IOException;
|import java.net.InetAddress;
|import java.net.ServerSocket;
|import java.net.Socket;
|import cn.tx.file.DBUtil;
|//接收xml文档并且存储到硬盘上
|public class Server {
|public  static final String XML_FILE_PATH = "src/XMLFile.xml";
|public  static void main(String[] args) {
|new  Server(10005);
|}
|//端口
|private  int port;
|public  Server(int port){
|super();
|this.port  = port;
|Receive  r =new Receive();
|Thread  t = new Thread(r);
|t.start();
|}
|class  Receive implements Runnable{
|@Override
|public  void run() {
|//创建服务端的Socket
|ServerSocket  ss = null;
|//接口
|Socket  s =null;
|//高效缓冲字节输入流
|BufferedInputStream  bi = null;
|//高效缓冲字节输出流
|BufferedOutputStream  bo = null;
|try  {
|ss  = new ServerSocket(port);
|System.out.println(port+"服务器已经启动");
|//通过服务器接口获取到Socket
|s  = ss.accept();
|//获得客服端的IP
|InetAddress  ia = s.getInetAddress();
|//获得高效缓冲字节输入流
|bi  = new BufferedInputStream(s.getInputStream());
|bo  = new BufferedOutputStream(new FileOutputStream(XML_FILE_PATH));
|//接收读取数据的字节数组
|byte[]  bytes = new byte[1024];
|//接收每次读取数据的长度变量
|int  len = 0;
|//循环读取数据
|while((len  = bi.read(bytes)) != -1){
|//写入数据
|bo.write(bytes,  0, len);
|//释放高效缓冲区
|bo.flush();
|}
|System.out.println("xml文件写入成功");
|//将xml文件写入数据库中
|DBUtil.writeXMLFile();
|}  catch (IOException e) {
|e.printStackTrace();
|}  finally{
|try  {
|if(bo  != null){
|bo.close();
|}
|if(bi  != null){
|bi.close();
|}
|if(s  != null){
|s.close();
|}
|if(ss  != null){
|ss.close();
|}
|}  catch (IOException e) {
|e.printStackTrace();
|}
|}
|}
|}
|}
|

## DBUtil类
DBUtil类主要是通过java对数据库的一些操作，将xml文档中的信息写入相对应的表单中
|package cn.tx.file;
|import java.io.IOException;
|import java.io.InputStream;
|import java.sql.Connection;
|import java.sql.Date;
|import java.sql.DriverManager;
|import java.sql.PreparedStatement;
|import java.sql.ResultSet;
|import java.sql.SQLException;
|import java.util.List;
|import java.util.Properties;
|import javax.sql.DataSource;
|import  org.apache.commons.dbcp.BasicDataSourceFactory;
|public class DBUtil {
|/**
|* 获取Connection对表
|*
|* @return
|*/
|public  static Connection getConnection() {
|InputStream  in =  DBUtil.class.getClassLoader().getResourceAsStream("dbcp.properties");
|System.out.println(in);
|Properties  p = new Properties();
|Connection  conn = null;
|try  {
|//  把参数加载到Properties集合中
|p.load(in);
|DataSource  ds = BasicDataSourceFactory.createDataSource(p);
|conn  = ds.getConnection();
|//  获取驱动
|/*
|* String driverClass =  p.getProperty("driverClass"); //获取资源定位器
|* String url =  p.getProperty("url"); //获取账号和密码 String  username =
|* p.getProperty("username");  String password =
|* p.getProperty("password"); //加载驱动 Class.forName(driverClass);
|* conn =DriverManager.getConnection(url,  username, password);
|*/
|}  catch (IOException e) {
|e.printStackTrace();
|}  catch (ClassNotFoundException e) {
|e.printStackTrace();
|}  catch (SQLException e) {
|e.printStackTrace();
|}  catch (Exception e) {
|e.printStackTrace();
|}
|return  conn;
|}
|/**
|* 获取preparedStatement对象
|*
|* @param sql
|* @return
|*/
|public  static PreparedStatement getPrepare(String sql) {
|PreparedStatement  ps = null;
|//  获得Connection连接
|Connection  conn = getConnection();
|try  {
|ps  = conn.prepareStatement(sql);
|}  catch (SQLException e) {
|e.printStackTrace();
|}
|return  ps;
|}
|/**
|* 释放资源
|*
|* @param conn
|*/
|public  static void release(Connection conn) {
|try  {
|if  (conn != null) {
|conn.close();
|}
|}  catch (SQLException e) {
|e.printStackTrace();
|}
|}
|/**
|* 释放资源
|*
|* @param conn
|*/
|public  static void release(PreparedStatement ps) {
|try  {
|Connection  conn = ps.getConnection();
|if  (ps != null) {
|ps.close();
|}
|if  (conn != null) {
|conn.close();
|}
|}  catch (SQLException e) {
|e.printStackTrace();
|}
|}
|/**
|* 释放资源
|*
|* @param conn
|*/
|public  static void release(Connection conn, PreparedStatement ps, ResultSet rs) {
|try  {
|if  (rs != null) {
|rs.close();
|}
|if  (ps != null) {
|ps.close();
|}
|if  (conn != null) {
|conn.close();
|}
|}  catch (SQLException e) {
|e.printStackTrace();
|}
|}
|/**
|* 向数据库存储全部文件信息
|*/
|public  static void writeXMLFile() {
|writeFileSet();
|writeFileGroup();
|writeFile();
|}
|/**
|* 向数据库存储总文件信息
|*/
|public  static void writeFileSet() {
|PreparedStatement  ps = null;
|try  {
|//  获取总文件的对象
|FileSet  fs = FileUtil.getFileSet();
|String  sql = "insert into fileset values(?,?,?,?)";
|ps  = getPrepare(sql);
|ps.setInt(1,  1);
|ps.setInt(2,  fs.getFileNum());
|ps.setInt(3,  fs.getDirNum());
|ps.setInt(4,  fs.getExtendNameTypeNum());
|ps.executeUpdate();
|System.out.println("FileSet加入数据成功！");
|}  catch (SQLException e) {
|e.printStackTrace();
|}  finally {
|release(ps);
|}
|}
|/**
|* 向数据库写入文件组信息
|*/
|public  static void writeFileGroup() {
|FileSet  fileSet = FileUtil.getFileSet();
|PreparedStatement  ps = null;
|try  {
|String  sql = "insert into filegroup values(?,?,?,?)";
|ps  = getPrepare(sql);
|//  获取文件组对象集合
|List<FileGroup>  listFileGroup = fileSet.getListFileGroup();
|for  (int i = 1; i <= listFileGroup.size(); i++) {
|FileGroup  fg = listFileGroup.get(i - 1);
|ps.setInt(1,  i);
|ps.setInt(2,  1);
|ps.setString(3,  fg.getExtendType());
|ps.setInt(4,  fg.getFileNum());
|ps.addBatch();
|}
|ps.executeBatch();
|System.out.println("一组文件写入成功");
|}  catch (SQLException e) {
|e.printStackTrace();
|}  finally {
|release(ps);
|}
|}
|/**
|* 向数据库写入文件信息
|*/
|public  static void writeFile() {
|int  count = 0;
|FileSet  fileSet = FileUtil.getFileSet();
|PreparedStatement  ps = null;
|try  {
|String  sql = "insert into afile values(?,?,?,?,?,?,?)";
|ps = getPrepare(sql);
|//  获取文件组对象集合
|List<FileGroup>  listFileGroup = fileSet.getListFileGroup();
|for  (int i = 1; i <= listFileGroup.size(); i++) {
|FileGroup  fg = listFileGroup.get(i - 1);
|List<File>  listFile = fg.getListFile();
|for  (int j = 1; j <= listFile.size(); j++) {
|count++;
|File  f = listFile.get(j - 1);
|ps.setInt(1,  count);
|ps.setInt(2,  i);
|ps.setInt(3,  f.getLength());
|ps.setDate(4,  new Date(f.getLastModified().getTime()));
|ps.setString(5,  f.isReadonly() + "");
|ps.setString(6,  f.getParent());
|ps.setString(7,  f.getName());
|ps.addBatch();
|}
|}
|ps.executeBatch();
|System.out.println("全部写完");
|}  catch (SQLException e) {
|e.printStackTrace();
|}  finally {
|release(ps);
|}
|}
|}
|

## FileUtil类
FileUtil类是用来将xml文档中的信息传入相对应的java对象中
|package cn.tx.file;
|import java.io.FileInputStream;
|import java.io.FileNotFoundException;
|import java.text.ParseException;
|import java.text.SimpleDateFormat;
|import java.util.ArrayList;
|import java.util.Date;
|import java.util.List;
|import org.dom4j.Document;
|import org.dom4j.DocumentException;
|import org.dom4j.Element;
|import org.dom4j.io.SAXReader;
|import cn.tx.server.Server;
|public class FileUtil {
|/**
|*
|* @return
|*/
|/**
|* 获得xml文档中所有文件的元素并写入File类集合中
|*
|* @param fileGroupList
|*/
|public  static FileSet getFileSet() {
|Document  doc = getDocment();
|Element  root = doc.getRootElement();
|//获取根元素中的属性
|String  fileNum = root.attributeValue("fileNum");
|String  dirNum = root.attributeValue("dirNum");
|String  extendNameTypeNum = root.attributeValue("extendNameTypeNum");
|//创建一个所有文件集合对象
|FileSet  fileSet = new FileSet(Integer.valueOf(fileNum), Integer.valueOf(dirNum),  Integer.valueOf(extendNameTypeNum), new ArrayList<FileGroup>());
|//获取根元素下的所有子元素
|List<Element>  fileGroupList  = root.elements();
|for  (Element e : fileGroupList) {
|fileSet.getListFileGroup().add(getFileGroup(e));
|}
|return  fileSet;
|}
|/**
|* 通过文件组元素，获取到文件组的对象
|*
|* @param fileList
|*/
|public  static FileGroup getFileGroup(Element fileGroup) {
|String  extendType = fileGroup.attributeValue("extendType");
|String  fileNum = fileGroup.attributeValue("fileNum");
|FileGroup  fg = new FileGroup(extendType, Integer.valueOf(fileNum), new  ArrayList<File>());
|//  获取文件组元素下的文件元素集合
|List<Element>  files = fileGroup.elements();
|for  (Element e : files) {
|//  将文件对象添加到组对象的文件集合属性中
|fg.getListFile().add(getFile(e));
|}
|return  fg;
|}
|/**
|* 根据文件元素返回文件对象
|*
|* @param file
|* @return File
|*/
|public  static File getFile(Element file) {
|File  f = null;
|try  {
|//  获得文件的大小
|String  length = file.attributeValue("length");
|//  获得文件的最后修改时间
|String  lastModified = file.attributeValue("lastModified");
|//  获得文件是否是可读的
|String  readonly = file.attributeValue("redonly");
|//  获得文件的上级路径
|String  parent = file.attributeValue("parent");
|//  获得文件的名字
|String  name = file.getText();
|//  把最后修改文件的时间由字符串类型转为毫秒数
|SimpleDateFormat  sdf = new SimpleDateFormat("yyyy-MM-dd HH:mm:ss");
|Date  date = sdf.parse(lastModified);
|f  = new File(Integer.valueOf(length), date, Boolean.valueOf(readonly), parent,  name);
|}  catch (ParseException e) {
|e.printStackTrace();
|}
|return  f;
|}
|/**
|* 获得xml文档对象
|*/
|public  static Document getDocment() {
|SAXReader  sr = null;
|FileInputStream  in = null;
|Document  doc = null;
|try  {
|//  创建字节输入流
|in  = new FileInputStream(Server.XML_FILE_PATH);
|sr  = new SAXReader();
|//  通过SAXReader获取xml文档对象
|doc  = sr.read(in);
|}  catch (FileNotFoundException e) {
|e.printStackTrace();
|}  catch (DocumentException e) {
|e.printStackTrace();
|}
|return  doc;
|}
|}
|

## FileSet、FileGroup和File类
FileSet、FileGroup和File类主要是用来存储xml文档中相对应的信息
FileSet
|package cn.tx.file;
|import java.util.List;
|/**
|* 存储xml文档中所有的文件信息
|*  @author 69301
|*
|*/
|public class FileSet {
|private  Integer fileNum;
|private  Integer dirNum;
|private  Integer extendNameTypeNum;
|private  List<FileGroup> listFileGroup;
|public  FileSet(Integer fileNum, Integer dirNum, Integer extendNameTypeNum,  List<FileGroup> listFileGroup) {
|super();
|this.fileNum  = fileNum;
|this.dirNum  = dirNum;
|this.extendNameTypeNum  = extendNameTypeNum;
|this.listFileGroup  = listFileGroup;
|}
|@Override
|public  String toString() {
|return  "FileSet [fileNum=" + fileNum + ", dirNum=" + dirNum +  ", extendNameTypeNum=" + extendNameTypeNum
|+  ", listFileGroup=" + listFileGroup + "]\n";
|}
|public  List<FileGroup> getListFileGroup() {
|return  listFileGroup;
|}
|public  void setListFileGroup(List<FileGroup> listFileGroup) {
|this.listFileGroup  = listFileGroup;
|}
|public  Integer getFileNum() {
|return  fileNum;
|}
|public  void setFileNum(Integer fileNum) {
|this.fileNum  = fileNum;
|}
|public  Integer getDirNum() {
|return  dirNum;
|}
|public  void setDirNum(Integer dirNum) {
|this.dirNum  = dirNum;
|}
|public  Integer getExtendNameTypeNum() {
|return  extendNameTypeNum;
|}
|public  void setExtendNameTypeNum(Integer extendNameTypeNum) {
|this.extendNameTypeNum  = extendNameTypeNum;
|}
|}
|
FileGroup
|package cn.tx.file;
|import java.util.List;
|/**
|* 存储xml文档中每一种后缀名的文件
|*  @author 69301
|*
|*/
|public class FileGroup {
|private  String extendType;
|private  Integer fileNum;
|private  List<File> listFile;
|public  String getExtendType() {
|return  extendType;
|}
|public  FileGroup(String extendType, Integer fileNum, List<File> listFile) {
|super();
|this.extendType  = extendType;
|this.fileNum  = fileNum;
|this.listFile  = listFile;
|}
|@Override
|public  String toString() {
|return  "FileGroup [extendType=" + extendType + ", fileNum=" +  fileNum + ", listFile=" + listFile + "]\n";
|}
|public  void setExtendType(String extendType) {
|this.extendType  = extendType;
|}
|public  Integer getFileNum() {
|return  fileNum;
|}
|public  void setFileNum(Integer fileNum) {
|this.fileNum  = fileNum;
|}
|public  List<File> getListFile() {
|return  listFile;
|}
|public  void setListFile(List<File> listFile) {
|this.listFile  = listFile;
|}

|}
|
File
|package cn.tx.file;
|import java.util.Date;
|/**
|* 存储xml文档中单个文件的信息
|*  @author 69301
|*
|*/
|public class File {

|private  Integer length;
|private  Date lastModified;
|private  boolean isReadonly;
|private  String parent;
|private  String name;
|@Override
|public  String toString() {
|return  "File [length=" + length + ", lastModified=" +  lastModified + ", isReadonly=" + isReadonly + ", parent="
|+  parent + ", name=" + name + "]\n";
|}
|public  Integer getLength() {
|return  length;
|}
|public  void setLength(Integer length) {
|this.length  = length;
|}
|public  Date getLastModified() {
|return  lastModified;
|}
|public  void setLastModified(Date lastModified) {
|this.lastModified  = lastModified;
|}
|public  boolean isReadonly() {
|return  isReadonly;
|}
|public  void setReadonly(boolean isReadonly) {
|this.isReadonly  = isReadonly;
|}
|public  String getParent() {
|return  parent;
|}
|public  void setParent(String parent) {
|this.parent  = parent;
|}
|public  String getName() {
|return  name;
|}
|public  void setName(String name) {
|this.name  = name;
|}
|public  File(Integer length, Date lastModified, boolean isReadonly, String parent,  String name) {
|super();
|this.length  = length;
|this.lastModified  = lastModified;
|this.isReadonly  = isReadonly;
|this.parent  = parent;
|this.name  = name;
|}

|}
|

# 最后来点效果图
![](https://img-blog.csdn.net/20180704190253980?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3dlaXhpbl80MTU0NzQ4Ng==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)
![](https://img-blog.csdn.net/20180704190259603?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3dlaXhpbl80MTU0NzQ4Ng==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)
![](https://img-blog.csdn.net/20180704190304892?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3dlaXhpbl80MTU0NzQ4Ng==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)
![](https://img-blog.csdn.net/20180704190310796?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3dlaXhpbl80MTU0NzQ4Ng==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)


