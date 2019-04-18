# 原始 java 载入数据源 JNDI及直连 - z69183787的专栏 - CSDN博客
2013年11月13日 13:04:50[OkidoGreen](https://me.csdn.net/z69183787)阅读数：1608
Java：
```java
package com.wonders.ips.common;
import java.sql.*;
import java.util.*;
import javax.sql.*;
import javax.naming.*;
import com.wonders.ips.sysadmin.ConfigInfo;
import com.wonders.kwoa.common.ConnectPool;
/**
 * <p>Title: DB连接</p>
 * <p>Description: DB连接</p>
 * <p>Copyright: Copyright (c) 2007-1</p>
 * <p>Company: Wondersgroup</p>
 * @author Chen Xiaodong
 * @version 3.0
 */
public class DbConnection {
  Connection conn = null;
  Statement stmt = null;
  PreparedStatement pstmt = null;
  ResultSet rs = null;
  Context ctx = null;
  DataSource ds = null;
  ConfigInfo cfn = null;
  private String sql = ""; //查询字串
  public DbConnection() {
  }
  /** 取得表的最大记录号
   * @param tableName 表名称
   * @return 最大记录号
   */
  public int getMaxId(String tableName) {
    try {
      int MaxId;
      tableName = tableName.toLowerCase();
      sql = "select RC_maxid from tb_rowcount where RC_tablename = '"+ tableName +"'";
      if (stmt == null)
        stmt = conn.createStatement();
      rs = stmt.executeQuery(sql);
      if (rs.next()) {
        MaxId = rs.getInt(1)+1;
        sql = "update tb_rowcount set RC_maxid=" + MaxId + " where RC_tablename = '"+ tableName +"'";
        stmt.executeUpdate(sql);
      }
      else {
        sql = "insert into tb_rowcount values ('"+ tableName +"',2)";
        stmt.executeUpdate(sql);
        MaxId = 1;
      }
      
      return MaxId;
    } catch(Exception e) {
      System.out.println(e.getMessage());
      return 0;
    }
  }
  public Connection getCon() {
    try {
        cfn = new ConfigInfo();
        String strDBConType = cfn.getValue("dbcon_type");
        if ("jndi".equalsIgnoreCase(strDBConType)) {
        	String url = cfn.getValue("jndi");
        	ctx = new InitialContext();
        	ds = (DataSource) ctx.lookup(url);
        	conn = ds.getConnection();
        } 
        else if (strDBConType.equalsIgnoreCase("direct")) {        
          String strUrl = cfn.getValue("dburl");
          String strDBDriver = cfn.getValue("dbdriver");
          String strWebname = cfn.getValue("dbusername");
          String strWebpass = cfn.getValue("dbpassword");
          Class.forName(strDBDriver);
        
          conn = DriverManager.getConnection(strUrl, strWebname, strWebpass);
        }
        else if (strDBConType.equalsIgnoreCase("Connection Pool")) {   
          //String strIniConFac = cfn.getValue("initial_context_factory");
          //String strProUrl = cfn.getValue("provider_url");
          String strJNDI = cfn.getValue("jndi");
          
          /*Hashtable ht = new Hashtable();
          ht.put(Context.INITIAL_CONTEXT_FACTORY,strIniConFac);
          ht.put(Context.PROVIDER_URL,strProUrl);
          ctx = new InitialContext(ht);*/        	
         // ctx = new InitialContext();
        //  ds = (DataSource)ctx.lookup(strJNDI);
        //  conn = ds.getConnection();
          conn = ConnectPool.getInstance().getConnection();
        }
      } catch (Exception e) {
        e.printStackTrace();
      }    
      return conn;
    }
  
  
  public static void main(String[] args) {     
  }
}
```
配置文件：
```
<?xml version="1.0" standalone="yes"?>
<config>
	<server_path>G:\\work\\stfb\\WebRoot</server_path>
	<server_path_web>/stfb</server_path_web>
	<webapp_path_prefix>/stfb</webapp_path_prefix>
	<http_path>http://localhost:8010/</http_path><!-- 应用所在目录 不用 -->
	<os>windows</os>
	<test>aaaaa</test>
	<!--
	  <dburl>jdbc:jtds:sqlserver://10.1.41.101:1433/stww</dburl>
	  <dbdriver>net.sourceforge.jtds.jdbc.Driver</dbdriver>
	  <dbusername>sa</dbusername>
	  <dbpassword>sa</dbpassword>
	-->
	<!--
	  <dbcon_type>direct</dbcon_type>
	  <dburl>jdbc:oracle:thin:@10.1.41.158:1521:ORCL</dburl>
	  <dbdriver>oracle.jdbc.driver.OracleDriver</dbdriver>
	  <dbusername>test</dbusername>
	  <dbpassword>test</dbpassword>
	-->
  	<initial_context_factory></initial_context_factory>
  	<provider_url></provider_url>
	<dbcon_type>jndi</dbcon_type>
	<jndi>java:comp/env/stfb</jndi><!-- notice: not "sfpt" -->
</config>
```
