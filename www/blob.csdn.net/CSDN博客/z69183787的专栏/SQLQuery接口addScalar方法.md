# SQLQuery接口addScalar方法 - z69183787的专栏 - CSDN博客
2014年01月27日 17:24:03[OkidoGreen](https://me.csdn.net/z69183787)阅读数：9682
在平时开发中Hibernate提供的hql基本能够满足我们的日常需求。但是在有些特殊的情况下，还是需要使用原生的sql,并且希望sql查询出来的结果能够绑定到pojo上。
Hibernate中由SQLQuery对象提供对原生sql语句的支持，例如如下语句在查询分析器中编写好后可直接使用
Sql代码  ![收藏代码](http://witcheryne.iteye.com/images/icon_star.png)
- -- 查询采集设备及正在执行的任务 --
- select d.guidID as deviceId, d.strName as deviceName,   
-     p.guidID as portId, p.strName as portName,   
-     t.guidTaskID as taskId, t.strName as taskName, t.enumRunStatus as runStatus,   
-     t.odtTaskStartDate as taskStartDate, t.odtTaskStopDate as taskStopDate  
- from ULCDevice as d   
- innerjoin ULCDevicePort as p on p.guidBelongDeviceID = d.guidID  
- leftouterjoin ULCTaskInfo as t on t.guidRecordPortID = p.guidID and t.enumRunStatus = 2  
- where d.enumType = 3  
- orderby d.guidID;  
HibernateCallback中，使用Session创建SQLQuery对象:
Java代码  ![收藏代码](http://witcheryne.iteye.com/images/icon_star.png)
- StringBuffer b = new StringBuffer();  
- 
- //... 省略SQL拼接代码
- String sql = b.toString();  
- 
- SQLQuery query = session.createSQLQuery(sql);  
**最后的重点是如何将查询结果映射到pojo:**
SQLQuery接口addScalar方法，可用于设置查询结果到Pojo的映射
`**SQLQuery [addScalar](http://docs.jboss.org/hibernate/core/3.5/api/org/hibernate/SQLQuery.html#addScalar(java.lang.String,%20org.hibernate.type.Type))**([String](http://java.sun.com/j2se/1.5.0/docs/api/java/lang/String.html?is-external=true) columnAlias, [Type](http://docs.jboss.org/hibernate/core/3.5/api/org/hibernate/type/Type.html) type)`
-- 参数中的Type为Hibernate中提供的数据库类型.
**如何使用见如下代码：**
Java代码  ![收藏代码](http://witcheryne.iteye.com/images/icon_star.png)
- public List<WorkStations> getWorkStations() {  
-     List<WorkStations> retList = this.getHibernateTemplate().executeFind(new HibernateCallback() {  
- 
- @Override
- public Object doInHibernate(Session session)  
- throws HibernateException, SQLException {  
- 
-             StringBuffer b = new StringBuffer()  
-                 .append("select d.guidID as deviceId, d.strName as deviceName, ")  
-                     .append("p.guidID as portId, p.strName as portName, ")  
-                     .append("t.guidTaskID as taskId, t.strName as taskName, t.enumRunStatus as runStatus, ")  
-                     .append("t.odtTaskStartDate as taskStartDate, t.odtTaskStopDate as taskStopDate ")  
-                 .append("from ULCDevice as d ")  
-                     .append("inner join ULCDevicePort as p on p.guidBelongDeviceID = d.guidID ")  
-                     .append("left outer join ULCTaskInfo as t on t.guidRecordPortID = p.guidID and t.enumRunStatus = 2 ")  
-                 .append("where d.enumType = 3 ")  
-                 .append("order by d.guidID");  
- 
-             String sql = b.toString();  
- 
-             SQLQuery query = session.createSQLQuery(sql);  
-             query.addScalar("deviceId", Hibernate.STRING);  
-             query.addScalar("deviceName", Hibernate.STRING);  
- 
-             query.addScalar("portId", Hibernate.STRING);  
-             query.addScalar("portName", Hibernate.STRING);  
- 
-             query.addScalar("taskId", Hibernate.STRING);  
-             query.addScalar("taskName", Hibernate.STRING);  
-             query.addScalar("runStatus", Hibernate.INTEGER);  
- 
-             query.addScalar("taskStartDate", Hibernate.TIMESTAMP);  
-             query.addScalar("taskStopDate", Hibernate.TIMESTAMP);  
- 
-             query.setResultTransformer(Transformers.aliasToBean(WorkStations.class));  
- 
-             List<WorkStations> list = query.list();  
-             System.out.println(list.size());  
- 
- return list;  
-         }  
- 
-     });  
- 
- return retList;  
- }  
**WorkStations.java**
Java代码  ![收藏代码](http://witcheryne.iteye.com/images/icon_star.png)
- publicclass WorkStations implements Serializable {  
- private String deviceId = null;  
- private String deviceName = null;  
- 
- private String portId = null;  
- private String portName = null;  
- 
- private String taskId = null;  
- private String taskName = null;  
- private Integer runStatus = null;  
- 
- private Date taskStartDate = null;  
- private Date taskStopDate = null;  
- 
- public WorkStations() {  
- 
-     }  
- 
- //... 省略 getter/sette r方法
- }  
