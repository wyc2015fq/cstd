# web.xml的一些配置信息详解 resorce-ref - z69183787的专栏 - CSDN博客
2014年03月16日 17:20:14[OkidoGreen](https://me.csdn.net/z69183787)阅读数：3824
1、web.xml中的<resource-ref>
Q：在使用数据库连接池的时候，书上说需要配置这个属性，但是试了试，不配也没关系，照样用。那我请教的是，这个属性的作用是什么呢？
A：
你是在tomcat中配置连接池吗？
在tomcat中配置好了，web.xml是不用再配的。
直接用就可以。
程序中取：
Context intitCtx = new InitialContext();
Context envCtx = (Context)intitCtx.lookup("java:comp/env");//从JNDI取出java环境上下文对象
DataSource ds = (DataSource)envCtx.lookup("jdbc/pro");//取数据源
Connection conn = ds.getConnection();//从连接池获取连接对象 
---项目中使用的（报表平台）DataSourceHelper
    public static Connection getConn() {
        Connection conn = null;
        DataSource ds = null;
        try {
            InitialContext ctx = new InitialContext();
            Context ctx2 = (Context) ctx.lookup("java:comp/env");
            ds = (DataSource) ctx2.lookup("jdbc/report");
            conn = ds.getConnection();
        } catch (Exception ex) {
            log.warn(ex);
            ex.printStackTrace();
        }
        return conn;
    }
    public static Connection getConn(String jndiName) {
        Connection conn = null;
        DataSource ds = null;
        if (StringUtils.isEmpty(jndiName))
            jndiName = "jdbc/report";
        try {
            InitialContext ctx = new InitialContext();
            Context ctx2 = (Context) ctx.lookup("java:comp/env");
            ds = (DataSource) ctx2.lookup(jndiName);
            conn = ds.getConnection();
        } catch (Exception ex) {
            log.warn(ex);
            ex.printStackTrace();
        }
        return conn;
    }
web.xml中的配置：
** <resource-ref>     <description>DB Connection</description>     <res-ref-name>jdbc/report</res-ref-name>     <res-type>javax.sql.DataSource</res-type>     <res-auth>Container</res-auth> </resource-ref>**
**resource-ref子元素的描述如下：● res-ref-name是资源工厂引用名的名称。该名称是一个与java:comp/env上下文相对应的JNDI名称，并且在整个Web应用中必须是惟一的。● res-auth表明：servlet代码通过编程注册到资源管理器，或者是容器将代表servlet注册到资源管理器。该元素的值必须为Application或Container。● res-sharing-scope表明：是否可以共享通过给定资源管理器连接工厂引用获得的连接。该元素的值必须为Shareable(默认值)或Unshareable。**
tomcat中配置：
server.xml
 <Context docBase="new_rpt" path="/new_rpt" workDir="work" reloadable="true"> 
 <Resource                                                                  
         name="jdbc/report"                                                 
         type="javax.sql.DataSource"                                        
         maxActive="10"                                                     
         maxIdle="8"                                                       
         username="xxx"                                                   
         maxWait="5000"                                                     
         driverClassName="oracle.jdbc.OracleDriver"                         
         password="xxx"                                                   
         url="jdbc:oracle:thin:@127.0.1.1:1521:report"/>                  
         </Context>    
Tomcat先找到web.xml下的<resource-ref>，然后再找server.xml下面的<Resource>。如果没有找到<Resource name=”JDBC/TestDB”>，或者名字错了，则会报“Cannot create JDBC driver of class '' for connect URL 'null'”错误。 
意思也就是web.xml中<resource-ref>下<res-ref-name></res-ref-name>中的名称要与Resource 标签中的一致， 否则会报这个错误 
