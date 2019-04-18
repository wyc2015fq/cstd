# J2EE操作Oracle的clob类型字段 - ljx0305的专栏 - CSDN博客
2010年02月26日 16:51:00[ljx0305](https://me.csdn.net/ljx0305)阅读数：746标签：[oracle																[exception																[null																[sql																[string																[insert](https://so.csdn.net/so/search/s.do?q=insert&t=blog)](https://so.csdn.net/so/search/s.do?q=string&t=blog)](https://so.csdn.net/so/search/s.do?q=sql&t=blog)](https://so.csdn.net/so/search/s.do?q=null&t=blog)](https://so.csdn.net/so/search/s.do?q=exception&t=blog)](https://so.csdn.net/so/search/s.do?q=oracle&t=blog)
个人分类：[Oracle体系结构](https://blog.csdn.net/ljx0305/article/category/428729)
2008-09-02
### [J2EE操作Oracle的clob类型字段](blog/235911)
关键字: java 
Oracle中，Varchar2支持的最大字节数为4KB，所以对于某些长字符串的处理，我们需要用CLOB类型的字段，CLOB字段最大支持4GB。 
还有其他几种类型： 
blob:二进制,如果exe,zip 
clob:单字节码,比如一般的文本文件. 
nlob:多字节码,如UTF格式的文件. 
以下就是对CLOG字段的操作方法，在我们的项目中帮助文档部分用到。 
1、首先是写入 
```java
/* 以下表PF_HELP_CONTENT中的HCONTENT字段时CLOB类型的 */
// 通过序列器生成帮助ID 
Map map = Query.getMap("Select TO_CHAR(SEQ_HID.nextval) HID FROM DUAL "); 
hid = String.valueOf(map.get("HID")); 
//插入一条数据，注意CLOB字段，需要先插入一个空的clob类型 empty_clob()，然后再单独更新clob字段 
sql = "Insert INTO PF_HELP_CONTENT(HID,HCONTENT) VALUES (?,empty_clob())  "; 
try
{           
     //执行插入 
     rtn = DbUtils.executeUpdate(sql,hid);     
     /* 插入成功后，修改HCONTENT字段内容 */
     //取得数据库连接                          
     Connection conn = DbUtils.getConnection(); 
     //手动提交 
     conn.setAutoCommit(false); 
     //定义ResultSet 和 Clob 变量 
     ResultSet rs = null; 
     oracle.sql.CLOB clob = null; 
     //更新SQL 
     String sqlclob = "Select HCONTENT FROM PF_HELP_CONTENT Where HID=? FOR Update "; 
     java.sql.PreparedStatement pstmt = conn.prepareStatement(sqlclob); 
     //hid是varchar2类型的，所以用setString 
     pstmt.setString(1,hid); 
     //执行update语句 
     rs= pstmt.executeQuery(); 
     if(rs.next()) 
     { 
        //取得刚才的HCONTENT的内容，也就是刚才添加的empty_clob() 
        clob = (oracle.sql.CLOB)rs.getClob(1); 
     } 
     //需要用clob.getCharacterOutputStream()流方式输出 
     Writer write = clob.getCharacterOutputStream(); 
     //写入具体内容，helpform.getHContent() 存的是帮助的内容 
     write.write(helpform.getHContent()); 
     write.flush(); 
     write.close(); 
     rs.close(); 
     //提交 
     conn.commit(); 
     conn.close(); 
} 
catch(Exception ex) 
{ 
    //......... 
}
```
2、修改CLOB字段内容  
```java
/* 修改跟插入时基本一致，也是用for update来实现 */
//如果修改前的字段内容长度大于当前修改的长度时，末尾的部分内容仍然会存在 
//所以在修改内容前，需要将PF_HELP_CONTENT内容置空 
sql = " Update PF_HELP_CONTENT SET HCONTENT=empty_clob() Where HID=? "; 
try
{       
 rtn = DbUtils.executeUpdate(sql,hid); 
 //以下操作跟添加时一样                                
 Connection conn = DbUtils.getConnection(); 
 conn.setAutoCommit(false); 
 ResultSet rs = null; 
 oracle.sql.CLOB clob = null; 
 String sqlclob = "Select HCONTENT FROM PF_HELP_CONTENT Where HID=? FOR Update "; 
 java.sql.PreparedStatement pstmt = conn.prepareStatement(sqlclob); 
 pstmt.setString(1,hid); 
 rs= pstmt.executeQuery(); 
 if(rs.next()) 
 { 
    clob = (oracle.sql.CLOB)rs.getClob(1); 
 } 
 Writer write = clob.getCharacterOutputStream(); 
 write.write(helpform.getHContent()); 
 write.flush(); 
 write.close(); 
 rs.close(); 
 conn.commit(); 
 conn.close();                                 
} 
catch(Exception ex) 
{ 
  //... 
}
```
3、取出CLOB字段的文本内容 
```java
/* 前面部分都一致 */
Connection conn = DbUtils.getConnection(); 
conn.setAutoCommit(false); 
ResultSet rs = null; 
oracle.sql.CLOB clob = null; 
String sqlclob = "Select HCONTENT FROM PF_HELP_CONTENT Where HID=? "; 
java.sql.PreparedStatement pstmt = conn.prepareStatement(sqlclob); 
pstmt.setString(1,hid); 
rs= pstmt.executeQuery(); 
if(rs.next()) 
{ 
    //rs.getClob(1)中参数1指的是HCONTENT字段索引，第一个字段从1开始而不是从0。 
    //也可以用字段名来取rs.getClob("HCONTENT") 
    clob = (oracle.sql.CLOB)rs.getClob(1); 
} 
if(clob==null || clob.length()==0) 
{ 
    hcontent = ""; 
}else
{ 
    //取CLOB字段内容为字符串 
    hcontent=clob.getSubString((long)1,(int)clob.length()); 
} 
rs.close(); 
conn.close(); 
request.setAttribute("HCONTENT",hcontent);
```
