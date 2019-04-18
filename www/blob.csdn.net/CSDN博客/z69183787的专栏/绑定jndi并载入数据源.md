# 绑定jndi并载入数据源 - z69183787的专栏 - CSDN博客
2013年02月05日 14:45:36[OkidoGreen](https://me.csdn.net/z69183787)阅读数：1601
```java
public Connection getConnection()
        throws SQLException
    {
        Connection conn;
        Context context = getContext();
        DataSource dataSource = (DataSource)context.lookup(SystemProperty.getProperty("datasource"));
        conn = dataSource.getConnection();
        return conn;
        Exception ex;
        ex;
        Debug.error(ex);
        throw new ConfigException("Get Connection Error:" + ex.getMessage());
    }
    public Context getContext()
        throws NamingException
    {
        if(context != null)
        {
            return context;
        } else
        {
            Hashtable props = new Hashtable();
            props.put("java.naming.factory.initial", SystemProperty.getProperty("java.naming.factory.initial"));
            props.put("java.naming.provider.url", SystemProperty.getProperty("java.naming.provider.url"));
            props.put("java.naming.security.principal", SystemProperty.getProperty("java.naming.security.principal"));
            props.put("java.naming.security.credentials", SystemProperty.getProperty("java.naming.security.credentials"));
            context = new InitialContext(props);
            return context;
        }
    }
```
设置 InitialContext 的 JNDI 环境属性 使用上下文查找已命名对象 使用已命名对象获取对象引用 关闭上下文。
附上配置文件：
```
#系统配置文件
system.config=com.corp.frame.core.base.defaultdriver.DefaultConfigDriver
#system.config=com.corp.frame.core.base.defaultdriver.DefaultConfigDriver_SampleContext
system.cache=true
system.debug=true
# jeus
java.naming.factory.initial=jeus.jndi.JEUSContextFactory
java.naming.provider.url=localhost:9736
java.naming.security.principal=administrator
java.naming.security.credentials=11111111
# tomcat
#java.naming.factory.initial=org.apache.naming.java.javaURLContextFactory
#java.naming.provider.url=jnp://localhost:8088
#java.naming.security.principal=admin
#java.naming.security.credentials=
#Weblogic
#java.naming.factory.initial=weblogic.jndi.WLInitialContextFactory   
#java.naming.provider.url=t3://localhost:7001
#java.naming.security.principal=weblogic
#java.naming.security.credentials=weblogic
database.cutpageByProcude=false
#for tomacat
#datasource=java:comp/env/jdbc/stpt
#for jeus
datasource=jdbc/stpt
workflow=G:/work/stoa/dist/stoa/WEB-INF/workflow-config.xml
uploadFileSavePath=D:/upload/stoa/
uploadFilePath=D:/upload/stoa/
countOfPage=10
permission.command.data=false
permission.command.data.validate.class=com.corp.frame.core.permission.data.DataPermissionValidateByIn
permission.command.menu=false
permission.command.submit=true
permission.command.link=true
permission.command.transaction=true
```
