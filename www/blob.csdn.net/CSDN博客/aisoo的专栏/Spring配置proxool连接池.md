# Spring配置proxool连接池 - aisoo的专栏 - CSDN博客
2009年11月09日 19:03:00[aisoo](https://me.csdn.net/aisoo)阅读数：973
1. proxool配置文件。proxool.xml
<?xml version="1.0" encoding="UTF-8"?>
<proxool-config>
<proxool>
    <alias>blddb</alias>
    <driver-url>jdbc:mysql://192.168.82.17:3306/bld</driver-url>
    <driver-class>com.mysql.jdbc.Driver</driver-class>
    <driver-properties>
      <property name="user" value="root"/>
      <property name="password" value="123"/>
    </driver-properties>
   <house-keeping-sleep-time>9000</house-keeping-sleep-time>
   <maximum-new-connections>20</maximum-new-connections>
   <prototype-count>5</prototype-count>
   <maximum-connection-count>100</maximum-connection-count>
   <minimum-connection-count>10</minimum-connection-count>
  </proxool>
</proxool-config>
2，web.xml设置
在xml里配置自启动的servlet，用来读取xml配置文件，
-------------------------
    <servlet>
        <servlet-name>contextConfigLocation</servlet-name>
        <servlet-class>org.springframework.web.context.ContextLoaderServlet</servlet-class>
        <load-on-startup>2</load-on-startup>
</servlet>
  <servlet>
    <servlet-name>ServletConfigurator</servlet-name>
    <servlet-class>org.logicalcobwebs.proxool.configuration.ServletConfigurator</servlet-class>
    <init-param>
      <param-name>xmlFile</param-name>
      <param-value>WEB-INF/proxool.xml</param-value>
    </init-param>
    <load-on-startup>1</load-on-startup>
  </servlet>
3.Spring 里配置 applicationContext.xml
<bean id="dataSource" class="org.springframework.jdbc.datasource.DriverManagerDataSource">
    <property name="driverClassName">
    <value>org.logicalcobwebs.proxool.ProxoolDriver</value>
    </property>
    <property name="url">
    <value>proxool.bldb</value>
    </property>
</bean>
所需jar包： spring.jar ， proxool-0.9.1.jar，  proxool-cglib.jar
写个工厂类测试：
import java.io.Serializable;
import java.sql.Connection;
import org.springframework.beans.factory.BeanFactory;
import org.springframework.context.support.ClassPathXmlApplicationContext;
import org.springframework.jdbc.datasource.DriverManagerDataSource;
public class ConnectionFactory implements Serializable {
    private static final long serialVersionUID = 1L;
    public static Connection getConnection() {   
        Connection conn=null; 
        try{
            BeanFactory factory = new ClassPathXmlApplicationContext("applicationContext.xml");
            DriverManagerDataSource dataSource = (DriverManagerDataSource)factory.getBean("dataSource");
            conn = dataSource.getConnection();
        } catch(Exception e) {
        }
        return conn;
    }
}
public class Test {
    public  static int isManager(String id) throws Exception {
        int flg = 0;
        Connection cn = ConnectionFactory.getConnection();
        cn.setAutoCommit(false);
        PreparedStatement pss = null;
        ResultSet rs = null;
        String sql = "SELECT ###  FROM $$$ WHERE ID= ? ";
        try{
            pss = cn.prepareStatement(sql);
            pss.setString(1, id);
            rs = pss.executeQuery();
            cn.commit();    
            if(rs.next()){
                flg = Integer.parseInt(rs.getString(1));
            }
        }catch(Exception e){
            cn.rollback();
            e.printStackTrace();
        }finally{
            rs.close();
            pss.close();
            cn.close();
        }
        return flg;
    }
     public static void main(String[] args) {
           isManager(001);
    }
}
