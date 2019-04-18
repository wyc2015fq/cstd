# Jxls导出excel的若干方式总结（八）-- 导出一条查询记录到多个sheet - z69183787的专栏 - CSDN博客
2016年05月14日 17:28:51[OkidoGreen](https://me.csdn.net/z69183787)阅读数：2471
转载地址：http://blog.csdn.net/hu_shengyang/article/details/6736721 ，有详细的excel操作方法及博客；
注意：suplyArea后跟的是其对应数据库表的列名称。
![](http://hi.csdn.net/attachment/201108/31/0_13147836304U3U.gif)
Sheet1中的模板信息（模板）
![](http://hi.csdn.net/attachment/201108/31/0_1314783666lUhK.gif)
Sheet2中的模板信息（模板）
**[java]**[view
 plain](http://blog.csdn.net/hu_shengyang/article/details/6736721#)[copy](http://blog.csdn.net/hu_shengyang/article/details/6736721#)
- 代码：  
- List supplyAreaList = saBiz.getSupplyAreaById(supplyAreaId);  
-     SupplyArea sa = (SupplyArea) supplyAreaList.get(0);  
-     Long id = sa.getSupplyAreaId();  
-     String templateDir = "D:/excel/template/SupplyAreaRecordsInSheet.xls";  
-     String targetDir="D:/excel/export/testRecordsInSheet.xls";            
-     Map beans = new HashMap();  
-     Class.forName("oracle.jdbc.driver.OracleDriver").getInterfaces();  
-     Connection con = DriverManager.getConnection("jdbc:oracle:thin:@168.9.202.49:1521:GIS", "JMJL","JMJL" );  
-     Statement stmt = con.createStatement(ResultSet.TYPE_SCROLL_INSENSITIVE,ResultSet.CONCUR_UPDATABLE);  
-     String query = "select SUPPLYAREA_ID,EDITUSER,SUPPLYAREA_NAME,X,Y," +  
- "REMARKS,STARTDATE,ENDDATE,LIFECYCLE,DATAGUID " +  
- "from HSY_T_SUPPLYAREA where SUPPLYAREA_ID ="+id;  
-     ResultSet rs = stmt.executeQuery(query);  
-     RowSetDynaClass rsc = new RowSetDynaClass(rs,false);  
-     beans.put("suplyArea",rsc.getRows());  
-     XLSTransformer transformer = new XLSTransformer();  
-     transformer.transformXLS(templateDir, beans, targetDir);  
![](http://hi.csdn.net/attachment/201108/31/0_1314783748kr88.gif)
Sheet1中的导出信息（导出结果）
![](http://hi.csdn.net/attachment/201108/31/0_1314783777F270.gif)
Sheet2中的导出信息（导出结果）
