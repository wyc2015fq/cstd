# oracle的CLOB和BLOB (jdbc实现对其操作示例代码) - z69183787的专栏 - CSDN博客
2013年11月22日 13:28:49[OkidoGreen](https://me.csdn.net/z69183787)阅读数：1645
鸣谢：[http://feicer.iteye.com/blog/567587](http://feicer.iteye.com/blog/567587)
一、区别和定义 
    LONG: 可变长的字符串数据，最长2G，LONG具有VARCHAR2列的特性，可以存储长文本一个表中最多一个LONG列 
　　LONG RAW: 可变长二进制数据，最长2G 
　　CLOB:  字符大对象Clob 用来存储单字节的字符数据 
　　NCLOB: 用来存储多字节的字符数据 
　　BLOB: 用于存储二进制数据 
　　BFILE: 存储在文件中的二进制数据，这个文件中的数据只能被只读访。但该文件不包含在数据库内。 
        bfile字段实际的文件存储在文件系统中,字段中存储的是文件定位指针.bfile对oracle来说是只读的,也不参与事务性控制和数据恢复. 
　　CLOB，NCLOB，BLOB都是内部的LOB(Large Object)类型，最长4G，没有LONG只能有一列的限制 
　　要保存图片、文本文件、Word文件各自最好用哪种数据类型? 
　　--BLOB最好，LONG　RAW也不错，但Long是oracle将要废弃的类型，因此建议用BLOB。 
二、操作 
1、 get 
CLOB 
java 代码
Java代码 ![复制代码](http://feicer.iteye.com/images/icon_copy.gif)![收藏代码](http://feicer.iteye.com/images/icon_star.png)![](http://feicer.iteye.com/images/spinner.gif)
- //获得数据库连接     
-     Connection con = ConnectionFactory.getConnection();       
-     con.setAutoCommit(false);    
   
-     Statement st = con.createStatement();       
- //不需要“for update”     
-     ResultSet rs = st.executeQuery("select CLOBATTR from TESTCLOB where ID=1");    
   
- if (rs.next())    
   
-     {       
-         java.sql.Clob clob = rs.getClob("CLOBATTR");    
   
-         Reader inStream = clob.getCharacterStream();       
- char[] c = newchar[(int) clob.length()];    
   
-         inStream.read(c);       
- //data是读出并需要返回的数据，类型是String     
-         data = new String(c);    
   
-         inStream.close();       
-     }       
-     inStream.close();       
-     con.commit();       
-     con.close();      

BLOB 
java 代码
Java代码 ![复制代码](http://feicer.iteye.com/images/icon_copy.gif)![收藏代码](http://feicer.iteye.com/images/icon_star.png)![](http://feicer.iteye.com/images/spinner.gif)
- //获得数据库连接     
-     Connection con = ConnectionFactory.getConnection();       
-     con.setAutoCommit(false);    
   
-     Statement st = con.createStatement();       
- //不需要“for update”     
-     ResultSet rs = st.executeQuery("select BLOBATTR from TESTBLOB where ID=1");    
   
- if (rs.next())    
   
-     {       
-         java.sql.Blob blob = rs.getBlob("BLOBATTR");    
   
-         InputStream inStream = blob.getBinaryStream();       
- //data是读出并需要返回的数据，类型是byte[]     
-         data = newbyte[input.available()];    
   
-         inStream.read(data);       
-         inStream.close();       
-     }       
-     inStream.close();       
-     con.commit();       
-     con.close();    

2、 put 
CLOB 
java 代码
Java代码 ![复制代码](http://feicer.iteye.com/images/icon_copy.gif)![收藏代码](http://feicer.iteye.com/images/icon_star.png)![](http://feicer.iteye.com/images/spinner.gif)
- //获得数据库连接     
-     Connection con = ConnectionFactory.getConnection();       
-     con.setAutoCommit(false);    
   
-     Statement st = con.createStatement();       
- //插入一个空对象empty_clob()     
-     st.executeUpdate("insert into TESTCLOB (ID, NAME, CLOBATTR) values (1, "thename", empty_clob())");    
   
- //锁定数据行进行更新，注意“for update”语句     
-     ResultSet rs = st.executeQuery("select CLOBATTR from TESTCLOB where ID=1 for update");    
   
- if (rs.next())    
   
-     {       
- //得到java.sql.Clob对象后强制转换为oracle.sql.CLOB    
-         oracle.sql.CLOB clob = (oracle.sql.CLOB) rs.getClob("CLOBATTR");    
   
-         Writer outStream = clob.getCharacterOutputStream();       
- //data是传入的字符串，定义：String data     
- char[] c = data.toCharArray();    
   
-         outStream.write(c, 0, c.length);    
   
-     }       
-     outStream.flush();       
-     outStream.close();       
-     con.commit();       
-     con.close();   
BLOB 
java 代码
Java代码 ![复制代码](http://feicer.iteye.com/images/icon_copy.gif)![收藏代码](http://feicer.iteye.com/images/icon_star.png)![](http://feicer.iteye.com/images/spinner.gif)
- //获得数据库连接     
-     Connection con = ConnectionFactory.getConnection();       
-     con.setAutoCommit(false);    
   
-     Statement st = con.createStatement();       
- //插入一个空对象empty_blob()     
-     st.executeUpdate("insert into TESTBLOB (ID, NAME, BLOBATTR) values (1, "thename", empty_blob())");    
   
- //锁定数据行进行更新，注意“for update”语句     
-     ResultSet rs = st.executeQuery("select BLOBATTR from TESTBLOB where ID=1 for update");    
   
- if (rs.next())    
   
-     {       
- //得到java.sql.Blob对象后强制转换为oracle.sql.BLOB    
-         oracle.sql.BLOB blob = (oracle.sql.BLOB) rs.getBlob("BLOBATTR");    
   
-         OutputStream outStream = blob.getBinaryOutputStream();       
- //data是传入的byte数组，定义：byte[] data    
-         outStream.write(data, 0, data.length);    
   
-     }       
-     outStream.flush();       
-     outStream.close();       
-     con.commit();       
-     con.close();       
- 
1.Hibernate映射类型：
在对象-关系映射文件中，Hibernate映射类型作为Java类型和SQL类型的桥梁。在映射文件中建议使用Hibernate映射类型。
1.1.java基本映射类型的Hibernate映射类型：
Java类型、Hibernate映射类型以及SQL类型之间的对应关系：
|**Java类型**|**Hibernate映射类型**|**标准SQL类型**|
|----|----|----|
|int或java.lang.Integer|integer或int|INTEGER|
|long或java.lang.Long|long|BIGINT|
|short或java.lang.Short|short|SMALLINT|
|byte或java.lang.Byte|byte|TINYINT|
|float或java.lang.Float|float|FLOAT|
|double或java.lang.Double|double|DOUBLE|
|java.math.BigDecimal|big_decimal|NUMERIC|
|char或java.lang.Character|character|CHAR(1)|
|java.lang.String|string|VARCHAR|
|boolean或java.lang.Boolean|boolean|BIT|
|boolean或java.lang.Boolean|yes_no|CHAR(1)('Y'或' N')|
|boolean或java.lang.Boolean|true_false|CHAR(1)('T'或' F')|
1.2.java时间和日期类型的Hibernate映射类型：
|**Java类型**|**Hibernate映射类型**|**标准SQL类型**|**说明**|
|----|----|----|----|
|java.util.Date或java.sql.Date|date|DATE|日期|
|java.util.Date或java.sql.Time|time|TIME|时间|
|java.util.Date或java.sql.Timestamp|timestamp|TIMESTAMP|时间戳|
|java.util.Calendar|calendar|TIMESTAMP||
|java.util.Calendar|calendar_date|DATE||
1.3.java大对象类型的Hibernate映射类型：
|**Java类型**|**Hibernate映射类型**|**标准SQL类型**|**MySQL类型**|**Oracle类型**|
|----|----|----|----|----|
|byte[]|binary|VARBINARY(或者BLOB)|BLOB|BLOB|
|java.lang.String|text|CLOB|TEXT|CLOB|
|实现java.io.Serializable接口的任意一个java类|serializable|VARBINARY(或者BLOB)|BLOB|BLOB|
|java.sql.Clob|clob|CLOB|TEXT|CLOB|
|java.sql.Blob|blob|BLOB|BLOB|BLOB|
2.映射对象标识符(OID)：
Hibernate中使用对象标识符(OID)来惟一标识一个对象。为了应付多变的业务要求，Hibernate制定了多种主键生成器。在hibernate的映射文件中，可以指定主键的生成器，如：
<idname="id"column="id"type="int">
<generatorclass="native"/>
</id>
Hibernate提供的几种内置标识符生成器：
|标识符生成器|描述|
|----|----|
|**increment：**|由**hibernate**自动以递增的方式生成标识符，每次增量为1．适用于代理**主键**．|
|**identity：**|由底层数据库生成标识符．数据库必须支持自动增长字段类型，不便于不同数据库之间的移植．适用于代理**主键**．|
|**sequence：**|**Hibernate**根据底层数据库序列来生成标识符．前提是条件是底层数据库支持序列(如Oracle)．适用于代理**主键**．|
|**hilo：**|**Hibernate**根据high/how算法来生成标识符．适用于代理**主键**．|
|**seqhilo：**|使用一个高/低位算法来高效的生成`long`, `short` 或者 `int`类型的标识符，给定一个数据库序列（sequence)的名字．适用于代理**主键．**|
|**native：**|**根据底层数据库对自动生成标识符的支持能力，来选择identity，sequence或hilo．适用于代理**主键**．**|
|**uuid.hex：**|**Hibernate**采用128位的UUID（Universal Unique Identification）算法来生成标识符．UUID算法能够在网络环境中生成唯一的字符串标识符．但字符串型的**主键**比整型的**主键**占用更多的数据库空间．适用于代理**主键**．|
|**uuid.string：**|**使用**UUID算法来生成标识符．UUID被编码为一个16个字符长的任意ASCII组成的字符串。适用于代理**主键**．|
|**assigned：**|由Java应用程序负责生成标识符，需在保存数据前完成．适用于自然**主键**．|
