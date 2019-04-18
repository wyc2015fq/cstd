# hibernate反向生成数据库表 - z69183787的专栏 - CSDN博客
2015年01月07日 12:03:06[OkidoGreen](https://me.csdn.net/z69183787)阅读数：3337
**hibernate反向生成数据库表**
第一步:添加Hibernate jar包,hibernate.cfg.xml写好数据源.
hibernate.cfg.xml:
<!DOCTYPEhibernate-configuration PUBLIC
"-//Hibernate/Hibernate Configuration DTD 3.0//EN"
"http://hibernate.sourceforge.net/hibernate-configuration-3.0.dtd">
< hibernate-configuration>
<session-factory>
<propertyname="hibernate.connection.url">jdbc:mysql://localhost/test</property>
<propertyname="hibernate.connection.driver_class">com.mysql.jdbc.Driver</property>
<property name="hibernate.connection.username">root</property>
<propertyname="hibernate.connection.password">root</property>
<propertyname="hibernate.dialect">org.hibernate.dialect.MySQLDialect</property>
<property name="hibernate.show_sql">true</property>
<mapping resource="com/bc/bean/HomeBusiness.hbm.xml"/>
</session-factory>
< /hibernate-configuration>
第二步:编写实体类;
packagecom.bc.bean;
import java.util.Date;
public class Business {
private Integer uId;
private String uName;
private Date hirDate;
private int zStoreNum;
private int zBranchNum;
private int zTradNum;
private int sStoreNum;
private int sBranchNum;
private int sTradNum;
private int bStoreNum;
private int bBranchNum;
private int bTradNum;
private int totalNum;
private String home;
public Business() {
super();
}
public Integer getuId() {
return uId;
}
public void setuId(Integer uId) {
this.uId = uId;
}
public String getuName() {
return uName;
}
public void setuName(String uName) {
this.uName = uName;
}
public Date getHirDate() {
return hirDate;
}
public void setHirDate(Date hirDate) {
this.hirDate = hirDate;
}
public int getzStoreNum() {
return zStoreNum;
}
public void setzStoreNum(int zStoreNum) {
this.zStoreNum = zStoreNum;
}
public int getzBranchNum() {
return zBranchNum;
}
public void setzBranchNum(int zBranchNum) {
this.zBranchNum = zBranchNum;
}
public int getzTradNum() {
return zTradNum;
}
public void setzTradNum(int zTradNum) {
this.zTradNum = zTradNum;
}
public int getsStoreNum() {
return sStoreNum;
}
public void setsStoreNum(int sStoreNum) {
this.sStoreNum = sStoreNum;
}
public int getsBranchNum() {
return sBranchNum;
}
public void setsBranchNum(int sBranchNum) {
this.sBranchNum = sBranchNum;
}
public int getsTradNum() {
return sTradNum;
}
public void setsTradNum(int sTradNum) {
this.sTradNum = sTradNum;
}
public int getbStoreNum() {
return bStoreNum;
}
public void setbStoreNum(int bStoreNum) {
this.bStoreNum = bStoreNum;
}
public int getbBranchNum() {
return bBranchNum;
}
public void setbBranchNum(int bBranchNum) {
this.bBranchNum = bBranchNum;
}
public int getbTradNum() {
return bTradNum;
}
public void setbTradNum(int bTradNum) {
this.bTradNum = bTradNum;
}
public int getTotalNum() {
return totalNum;
}
public void setTotalNum(int totalNum) {
this.totalNum = totalNum;
}
public String getHome() {
return home;
}
public void setHome(String home) {
this.home = home;
}
}
第三步:编写实体类映射文件;
HomeBusiness.hbm.xml
<?xmlversion="1.0"?>
< !DOCTYPE hibernate-mapping PUBLIC 
"-//Hibernate/Hibernate Mapping DTD 3.0//EN"
"http://hibernate.sourceforge.net/hibernate-mapping-3.0.dtd">
< hibernate-mapping package="com.bc.bean">
<class name="HomeBusiness" table="t_HomeBusiness">
<id name="id" type="java.lang.Integer">
<column name="id" precision="6" scale="0"/>
<generator class="native">
</generator>
</id>
<property name="name" column="name"/>
<property name="one" column="one"/>
<property name="two" column="two"/>
<property name="three" column="three"/>
<property name="four" column="four"/>
<property name="five" column="five"/>
<property name="totalNum" column="totalNum"/>
<property name="home" column="home"/>
<property name="team" column="team"/>
</class>
< /hibernate-mapping>
第四步:Main执行创建
ExportDB.java
packagecom.test.bean.test;
import org.hibernate.cfg.Configuration;
import org.hibernate.tool.hbm2ddl.SchemaExport;
public class ExportDB {
public static void main(String[] args) {
//读取hibernate.cfg.xml文件
Configuration cfg = new Configuration().configure();
SchemaExport export = new SchemaExport(cfg);
export.create(true, true);
}
}
按照以上步骤,即可反向生成!
**[使用Hibernate生成或还原数据库表](http://www.cnblogs.com/modou/articles/1274211.html)**
利用MyEclipse可以很容易的根据数据库表生成Hibernate mapping file和实体类。
同时反过来，利用生成的Hibernate mapping file和实体类也可以生成数据库表，这样就可以免去部署项目时建立数据库表的过程，也可以利用这种方法从一种数据库导入到另一种数据库。
在使用前一定要确保Hibernate在应用中可以正常访问数据库。
自动生成只适用于数据库的表，数据库没有的话，需要自已手工建立。
方法有两种：
1.hibernate.cfg.xml配置文件中添加属性：<propertyname="hbm2ddl.auto">create</property>
这种方式可以在启动web服务后首次调用Hibernate时自动建立数据库表，建立表后再把这个属性去掉，防止以后自动重新建立表，丢失数据。
2.以代码的形式实现：
Configuration cfg = newConfiguration().configure();
SchemaExport schemaExport= new SchemaExport(cfg);
schemaExport.create(false, 
true);
我在测试不同数据库之间导入数据时出现错误，从SQLServer建立的映射无法还原到MySQL数据库，对比了一下，发现SQLServer在Hibernate mapping file里多了一个schema="dbo",把每个mapping file里的schema="dbo"去掉，就可以成功还原了。
使用Hibernate在多种数据库之间切换时，也会出现这种问题，解决办法就是在mapping file里不要指定schema，如果像SQLServer需要指定schema的话，可以在Hibernate配置文件hibernate.cfg.xml里指定默认schema：<propertyname="default_schema">dbo</property>
后来的一次应用，从SQLServer导入Oracle时出现两个错误：
1.ORA-01658: 无法为表空间RUIRI中的段创建 INITIAL 区
这个错误是因为表空间不足，修改表空间的大小即可。
2.ORA-00904: : 无效的标识符
这个错误是因为有些Oracle关键字被用于字段名，导致错误，比如Comment,Share,Remark,Level。解决办法比较笨，字段名中加入双引号，手工建表。
**Hibernate自动根据映射创建数据库表**
方案一
　　在hibernate.cfg.xml中添加：
　　1 <properties>
　　2<property name="hibernate.hbm2ddl.auto" value="create"/>
　　3</properties>
　　value的值可选项如下：
　　1 *validate 加载hibernate时，验证创建数据库表结构
　　2 * create 每次加载hibernate，重新创建数据库表结构。
　　3 * create-drop 加载hibernate时创建，退出是删除表结构
　　4 * update 加载hibernate自动更新数据库结构
　　方案二
　　用自动生成代码后，在/src下面有一个jdbc.properties文件，这里加一句hibernate.hbm2ddl.auto=update。值的可选项同上。
