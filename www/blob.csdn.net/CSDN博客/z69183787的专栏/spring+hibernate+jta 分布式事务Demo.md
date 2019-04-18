# spring+hibernate+jta 分布式事务Demo - z69183787的专栏 - CSDN博客
2014年06月12日 21:35:40[OkidoGreen](https://me.csdn.net/z69183787)阅读数：4869
转载“：[http://momoko8443.iteye.com/blog/190994](http://momoko8443.iteye.com/blog/190994)
0、设计分布式数据库，修改第一部分的例子。先将db1中的user1表中的记录清除，把user2表drop掉，然后create一个新的database db2来模拟分布式数据库，在db2中新建一张表user2，结构和user1一致。接下来的操作就是同时对2个数据库的2张表进行操作了。![clip_image001](http://tkfiles.storage.msn.com/y1pR6nssK-Ax5zDI2yLYF1qrKWwK6ZQqLYI90-FejR7MrouKNSg0qgWMmS1AL3E_tjyZADZqfm0-i0?PARTNER=WRITER)
![clip_image003](http://tkfiles.storage.msn.com/y1pR6nssK-Ax5znXZwHV6tM90AyLmuAmnqHzIaz2o52f8hXPPrQumBtDm4Afpx8G_fMevflbka6j3A?PARTNER=WRITER)![clip_image005](http://tkfiles.storage.msn.com/y1pR6nssK-Ax5y4virSUk4qOGPyCKJRheMQVLYSf0SjXFZgu0GQWP5xAqT-9UXb2idU7TRdAk0pQ9I?PARTNER=WRITER)
1、DAO接口、DAOImpl实现，Service接口、ServiceImpl实现都不用去动它，为使第一次测试时事务顺利提交，我们把第一部分中对于serviceImpl中insert方法中故意制造的困难去除。![clip_image006](http://tkfiles.storage.msn.com/y1pR6nssK-Ax5wum5rIPsNHKX1EnC_WRCFnq7vWDxX4fdq-DpTLZ1DpMUKaOFAK0GxGPzLE9eMUkxM?PARTNER=WRITER)
——————————————————————————————
文件:**class** User1AndUser2ServiceImpl.java
代码:
**public void** insertUser1AndUser2(User1 user) {
**try**{
System.*out*.println("insertUser1AndUser2 方法开始执行");
User1 insertedUser1=user1DAOImpl.insertUser1(user);
User2 user2=**new** User2();
//String tmp="制造困难制造困难制造困难制造困难制造困难制造困难制造困难制造困难制造困难制造困难制造困难制造困难";
//user2.setName(insertedUser1.getName()+tmp);
user2.setName(insertedUser1.getName());
user2DAOImpl.insertUser2(user2);
}
**catch**(DataAccessException e){
System.*err*.println("service 抛出异常");
**throw** e;
}
}
——————————————————————————————————————————
2、spring是不能直接管理分布式事务的，需要管理的话就得借助jta，但是jta又不能运行在tomcat这个容器中，于是我们就要用到AtomikosTransactionsEssentials这个第三方jar包来提供jta支持。AtomikosTransactionsEssentials可以从[http://www.atomikos.com](http://www.atomikos.com/)[下载，当然我这里也提供这些类包的ftp](http://www.atomikos.com/home.html)下载（ftp下载，勾选连接到服务器，用户名: momo 密码 momoko8443）。解压zip文件后，我们从dist文件夹中拷贝几个jar包复制到工程中的lib下。如果你不使用spring管理hibernate，则把hibernate的包也拷上
![clip_image008](http://tkfiles.storage.msn.com/y1pR6nssK-Ax5xgqDWF4ZUje3nnL91W8NBP5aoKh6MOdTWTshyVbIBb_3HFBxX_xycwiGGRCfR5X7U?PARTNER=WRITER)![clip_image009](http://tkfiles.storage.msn.com/y1pR6nssK-Ax5yZa2HvD0KaRHcRsxVqVhxdZDF4aXW_oDkmGGtLgmoyWKkDOO1iPMas99r2QRbhQl8?PARTNER=WRITER)
3、修改applicationContext.xml，由于database的增加，所以第一部分代码中的一个dataSource+一个sessionFactory的配置肯定不能满足多数据库的需求。这里我们要配置2个dataSource和2个sessionFactory。分别来管理起db1和db2。
由于要使用jta管理事务，dataSource就必须使用实现了javax.sql.XADataSource接口的连接池来连接，AtomikosTransactionsEssentials为我们提供了一个名为SimpleDataSourceBean的连接池（名字有点怪），废话不多说了，直接看修改的dataSource和sessionFactory，注意这里的数据库必须是mysql5.0以上，且驱动也要5.0以上否则不提供XADataSource支持。另外还有个叫Xapool的连接池也是支持xadatasource的，你可以试试看。![clip_image010](http://tkfiles.storage.msn.com/y1pR6nssK-Ax5z3RcSr3eUHpfmu718pKDMDR2iiFGsvz1Pjhvh8V9Nd3S8kdw-n8FQeYF24hsxF5LY?PARTNER=WRITER)
——————————————————————————————————————————
文件:applicationContext.xml
配置:
<bean id="dataSource1" class="com.atomikos.jdbc.DataSourceBean">
<property name="uniqueResourceName"><value>db1</value></property>
<property name="xaDataSourceClassName"><value>com.mysql.jdbc.jdbc2.optional.MysqlXADataSource</value></property>
<property name="xaDataSourceProperties">
<value>URL=jdbc:mysql://localhost:3306/db1;user=root;password=root</value>
</property>
<property name="exclusiveConnectionMode"><value>true</value></property>
<property name="connectionPoolSize"><value>3</value></property>
<property name="validatingQuery"><value>SELECT 1</value></property>
</bean>
<bean id="sessionFactory1"
class="org.springframework.orm.hibernate3.LocalSessionFactoryBean">
<property name="dataSource">
<ref bean="dataSource1" />
</property>
<property name="hibernateProperties">
<props>
<prop key="hibernate.dialect">
org.hibernate.dialect.MySQLDialect
</prop>
</props>
</property>
<property name="mappingResources">
<list>
<value>com/zhiku/vo/User1.hbm.xml</value>
</list>
</property>
</bean>
————————————————————————————————————————
第二对连接db2的配置也是一样的,就是把出现1的地方都替换成2
————————————————————————————
文件:applicationContext.xml
配置:
<bean id="dataSource2" class="com.atomikos.jdbc.DataSourceBean">
<property name="uniqueResourceName"><value>db2</value></property>
<property name="xaDataSourceClassName"><value>com.mysql.jdbc.jdbc2.optional.MysqlXADataSource</value></property>
<property name="xaDataSourceProperties">
<value>URL=jdbc:mysql://localhost:3306/db2;user=root;password=root</value>
</property>
<property name="exclusiveConnectionMode"><value>true</value></property>
<property name="connectionPoolSize"><value>3</value></property>
<property name="validatingQuery"><value>SELECT 1</value></property>
</bean>
<bean id="sessionFactory2"
class="org.springframework.orm.hibernate3.LocalSessionFactoryBean">
<property name="dataSource">
<ref bean="dataSource2" />
</property>
<property name="hibernateProperties">
<props>
<prop key="hibernate.dialect">
org.hibernate.dialect.MySQLDialect
</prop>
</props>
</property>
<property name="mappingResources">
<list>
<value>com/zhiku/vo/User2.hbm.xml</value>
</list>
</property>
</bean>
————————————————————————————————————————————
4、继续修改applicationContext.xml，现在是关键所在，开始添加jta事务管理了。注意看代码了，首先定一个atomikos提供的transactionManager![clip_image011](http://tkfiles.storage.msn.com/y1pR6nssK-Ax5xe0ZpMAacTZ8AgcOHUEn5UxyfvLAfzYlF8RKtprC20kP2xsjW-UrMu57tyzv6a-Lw?PARTNER=WRITER)
——————————————————————————————————
文件:applicationContext.xml
配置:
<bean id="atomikosTransactionManager" class="com.atomikos.icatch.jta.UserTransactionManager" init-method="init" destroy-method="close">
<property name="forceShutdown"><value>true</value></property>
</bean>
————————————————————————————
再定一个atomikos对UserTransaction接口的实现![clip_image012](http://tkfiles.storage.msn.com/y1pR6nssK-Ax5xMrpOvfmM0phN-W1YFKWHUJjEvZD_4o-s8Z13I59MvSKnmuosVDqHshVLCEgyfA6o?PARTNER=WRITER)
——————————————————————————
文件:applicationContext.xml
配置:
<bean id="atomikosUserTransaction" class="com.atomikos.icatch.jta.UserTransactionImp">
<property name="transactionTimeout"><value>300</value></property>
</bean>
————————————————————————
接着把第一部分中用到的hibernateTransactionManager进行大换血，由于这里不是由spring通过hibernate来管理事务了，而是由jta管理因此class要换掉。transactionManager和userTransaction接口的具体实现就注入之前声明的2个bean，
看到这里是不是有点晕？![clip_image013](http://tkfiles.storage.msn.com/y1pR6nssK-Ax5zA8UTmAbImh9fc5mUoObaqcderOnpS7SABzf-kOvVOsdvUyQL08cDWrHHVejc0pvA?PARTNER=WRITER)，脑子里![clip_image014](http://tkfiles.storage.msn.com/y1pR6nssK-Ax5yNDe-QRD0cFwCpzmGWNJQokAP0mCmg1PxsyeekBpcVFq_P31jf5sGP9b3rnM9ZhYY?PARTNER=WRITER)。正常正常，先死记硬背
————————————————————————————
文件:applicationContext.xml
配置:
<bean id="hibernateTransactionManager" class="org.springframework.transaction.jta.JtaTransactionManager">
<property name="transactionManager">
<ref local="atomikosTransactionManager"/>
</property>
<property name="userTransaction">
<ref local="atomikosUserTransaction"/>
</property>
</bean>
——————————————————————————————
user1DAOImpl和user2DAOImpl的sessionFactory属性稍作修改注入新的sessionFactory。user2DAOImpl类似，这里就不给出代码了。![clip_image015](http://tkfiles.storage.msn.com/y1pR6nssK-Ax5wJKAL7l_QkIf_49BoVf67TjESFlJ29bvDwdlLlo6LDyphC_AmZdeGvHYv7NftEh0w?PARTNER=WRITER)
——————————————————————————————
文件:applicationContext.xml
配置:
<bean id="user1DAOImpl" class="com.zhiku.dao.impl.User1DAOImpl">
<property name="sessionFactory">
<ref local="sessionFactory1"/>
</property>
</bean>
————————————————————————————
5、映射文件可以重新生成一下，也可以把User2.hbm.xml中的catalog="db1"改成db2就可以不必重新生成而套用原来的
<class name="com.zhiku.vo.User2" table="user2" catalog="db2">
其他的都不必修改
接下来，run application。看下控制台，很好，正如我们期望的一样，第一次测试时，事务正常提交了。![clip_image016](http://tkfiles.storage.msn.com/y1pR6nssK-Ax5wlCWl6Zj_iCd-9wQLqAB9woo8TJiLNrQ4bsAzgn3_9uQcvknS8AfsM1YvjkP9g-C4?PARTNER=WRITER)
![clip_image018](http://tkfiles.storage.msn.com/y1pR6nssK-Ax5w0JU1vFdN3Px0DduNr-8YO2_XXS5z598s0xSnzGdr6Q_v-W0MXK6lLIyxKD_JN494?PARTNER=WRITER)
看下数据库，果然，2个db中user表都被**“插入”**了![clip_image019](http://tkfiles.storage.msn.com/y1pR6nssK-Ax5zFlLXZeNMqigRTcp2W-DdRIgwdKenQ9QmWQkiFLf1Lk4Buwzc2gf_lvOnzS157Dhs?PARTNER=WRITER)一条记录。如下图所示：
![clip_image020](http://tkfiles.storage.msn.com/y1pR6nssK-Ax5w8EjJPHaLVtD0x_b2z675d6pGToPkEddYFvRXbZZa8K34-8Pq_G924wq0krmW_0Lw?PARTNER=WRITER)
![clip_image021](http://tkfiles.storage.msn.com/y1pR6nssK-Ax5y-qOnGlSCla1QcWHfo4C9CWGiwYTMKDO2MqxKB61fVNHkzTv3U1ZAooEE3WVzWIew?PARTNER=WRITER)，不要意思上错图了。如下图所示：
![clip_image023](http://tkfiles.storage.msn.com/y1pR6nssK-Ax5zwklwm2aHZ0TUCr1dN9G1bZf3w5SiHIPxRzkzdSuJTvCDOJN4yH970BSMx4F5qnKE?PARTNER=WRITER)
![clip_image025](http://tkfiles.storage.msn.com/y1pR6nssK-Ax5yGH9HhPKQuojGV0ztq5SX4PvMLlaSvXVlpkwfjAH32Ef8VAcyF7BEG02Ve00khF04?PARTNER=WRITER)
6、按照管理，接下来就是应该测roll back了，好，给它制造困难，把serviceImpl中的“困难”加上。![clip_image026](http://tkfiles.storage.msn.com/y1pR6nssK-Ax5yhD-ANs8e3H-OTFhN-wYauxyZ18iMTQ2tKiIh_Bkp7e1NQvNat5TMc5_BoRTCwnEc?PARTNER=WRITER) Run application
![clip_image028](http://tkfiles.storage.msn.com/y1pR6nssK-Ax5xxxs7yQMqISXYjUuzOm_eoKVjlgMTH51_U0xfNAyVUzeQxMzSimWBZ0qwAyiX5rdA?PARTNER=WRITER)
看一下控制台输出，果然抛出了异常![clip_image029](http://tkfiles.storage.msn.com/y1pR6nssK-Ax5w0I1TVHzvKntIelkVxCVRDgibgf-BV6i_jbKE1iyryhAcYI51-QpeQYxBURxBKF5c?PARTNER=WRITER)
![clip_image031](http://tkfiles.storage.msn.com/y1pR6nssK-Ax5xeAoOEroEjSfC8uTZvysTikW9c1-KYZpY17BhTQRxxwufWRnSrV-sBfUY3zVNh3MU?PARTNER=WRITER)
再观察下数据库中的记录。OK！roll back成功！![clip_image032](http://tkfiles.storage.msn.com/y1pR6nssK-Ax5w6U0sZzmB9xv4xlmIIzJuYulbZjevptG8PiqtiiwUTnr5aPOMUXgXMkJvWP8EQLCY?PARTNER=WRITER)
![clip_image034](http://tkfiles.storage.msn.com/y1pR6nssK-Ax5z_4G3iF_F8rynyHO2QPmIn1h08s1qSjYCOD16wOKdY37B6kOnWgkqrzjbhS1k00oM?PARTNER=WRITER)
![clip_image036](http://tkfiles.storage.msn.com/y1pR6nssK-Ax5yNaKGTZKU_VPsrsts2Q8IozxXnZr1_M1hnN9IjXR3_8cAcQsMNPypGg36SGYh5-iU?PARTNER=WRITER)
7、至此分布式事务管理demo演示完毕，2个demo都是演示性质的，知侧重于配置和实现，至于其中的原理，由于我能力有限不能表述的很清楚，你可以查看spring的api文档进行探索。![clip_image037](http://tkfiles.storage.msn.com/y1pR6nssK-Ax5zlb0TAwT1ikZ-7BkReChF_aYtgwxMjJjSh03-qMyd-AR78MMRyOXpatgDZePcAJkQ?PARTNER=WRITER)，谢谢观赏。
ps.这里同样给出第二部分工程的代码下载（ftp下载，勾选连接到服务器，用户名: momo 密码 momoko8443）
