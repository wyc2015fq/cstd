# spring jpa 行级锁的实现 - z69183787的专栏 - CSDN博客
2018年02月21日 20:00:59[OkidoGreen](https://me.csdn.net/z69183787)阅读数：465
[http://blog.csdn.net/fengyuxue11011/article/details/47039765](http://blog.csdn.net/fengyuxue11011/article/details/47039765)
最近要做一个新项目，需要借助mysql的行级锁机制，又由于是第一次使用jpa去实现行级锁，所以遇到了一丢丢问题，昨天晚上用了1个多小时解决了。。分享下。。
--------------------------------------------------------------------------------------------------------------------------------------------------
1.这是spring配置文件的内容，相信大多数人也都能从网上search到：
[java][view
 plain](http://blog.csdn.net/fengyuxue11011/article/details/47039765#)[copy](http://blog.csdn.net/fengyuxue11011/article/details/47039765#)
- <bean id="hibernateJpaVendorAdapter"
- class="org.springframework.orm.jpa.vendor.HibernateJpaVendorAdapter" />  
-     <bean id="entityManagerFactory"
- class="org.springframework.orm.jpa.LocalContainerEntityManagerFactoryBean">  
- <!-- 指定下数据源 -->  
-         <property name="dataSource" ref="<strong>dataSource</strong>" />   
-         <property name="jpaVendorAdapter" ref="hibernateJpaVendorAdapter" />  
-         <!-- 指定Entity实体类包路径 -->  
- <property name="packagesToScan">  
-             <array>  
-                 <value>com.xxx.xx.xxx.core.entity</value>  
-             </array>  
-         </property>  
-         <property name="jpaProperties">  
-             <props>  
-                 <prop key="hibernate.dialect">org.hibernate.dialect.MySQLDialect</prop>  
-                 <prop key="hibernate.ejb.naming_strategy">org.hibernate.cfg.ImprovedNamingStrategy</prop>  
-                 <prop key="hibernate.cache.provider_class">org.hibernate.cache.NoCacheProvider</prop>  
-                 <prop key="hibernate.show_sql">true</prop> <!-- 指是否显示SQL，可以根据需要 -->  
-                 <prop key="hibernate.format_sql">false</prop>  
-             </props>  
-         </property>  
-     </bean>  
- <!-- 指定下Dao层的包路径-->  
-     <jpa:repositories base-package="com.xxx.xx.xxx.core.dao"
-                       entity-manager-factory-ref="entityManagerFactory"
-                       transaction-manager-ref="transactionManagerjpa" />  
- 
-     <bean id="transactionManagerjpa"class="org.springframework.orm.jpa.JpaTransactionManager">  
-         <property name="entityManagerFactory" ref="entityManagerFactory" />  
-     </bean>  
-     <tx:annotation-driven transaction-manager="transactionManagerjpa"  />  
2.简单的贴一个Entity对象和一个Dao
[java][view
 plain](http://blog.csdn.net/fengyuxue11011/article/details/47039765#)[copy](http://blog.csdn.net/fengyuxue11011/article/details/47039765#)
- import javax.persistence.Column;  
- import javax.persistence.Entity;  
- import javax.persistence.GeneratedValue;  
- import javax.persistence.GenerationType;  
- import javax.persistence.Id;  
- import javax.persistence.Table;  
- import java.io.Serializable;  
- import java.util.Date;  
- 
- @Entity
- @Table(name = "job_info")  
- publicclass JobInfo implements Serializable {  
- 
- @Id
- @GeneratedValue(strategy = GenerationType.IDENTITY)  
- private Long id;  
- @Column(name = "job_name")  
- private String jobName;  
- @Column(name = "job_desc")  
- private String jobDesc;  
- 
- .... get/set方法不再赘述  
- }  
[java][view
 plain](http://blog.csdn.net/fengyuxue11011/article/details/47039765#)[copy](http://blog.csdn.net/fengyuxue11011/article/details/47039765#)
- import javax.persistence.LockModeType;  
- import org.springframework.data.jpa.repository.JpaRepository;  
- import org.springframework.data.jpa.repository.Lock;  
- import org.springframework.data.jpa.repository.Query;  
- import org.springframework.data.repository.query.Param;  
- import org.springframework.stereotype.Repository;  
- import com.xxx.xx.core.entity.JobInfo;  
- @Repository
- publicinterface JobInfoDao extends JpaRepository<JobInfo, Long> {  
- @Query(value = "select j from JobInfo j where j.jobName = :jobname ")  
- public JobInfo getJobForUpdate(@Param("jobname") String jobname);  
- @Lock(value = LockModeType.PESSIMISTIC_WRITE)  
- @Query(value = "select j from JobInfo j where j.id = :id ")  
- publicvoid getJobByIdForUpdate(@Param("id") Long id);  
- @Lock(value = LockModeType.PESSIMISTIC_WRITE) // TODO 千万不要用这个哦！
- @Query(value = "select j from JobInfo j where j.jobName = :jobname ")  
- publicvoid getJobByNameForUpdate(@Param("jobname") String jobname);  
- }  
3.service层，此为测试代码
[java][view
 plain](http://blog.csdn.net/fengyuxue11011/article/details/47039765#)[copy](http://blog.csdn.net/fengyuxue11011/article/details/47039765#)
- import org.springframework.transaction.annotation.Transactional;  
- 
- publicclass JobService implements IJobService {  
- @Autowired
- private JobInfoDao jobInfoDao;  
- @Transactional// 这个是需要标注的，因为Dao层有for update 的机制，那么这边就要开启事务了，否则会报错的。。。
- public JobInfo getJobForUpdate(Long id) {  
-         jobInfoDao.getJobByIdForUpdate(id);  
- try {  
-             Thread.sleep(100000);  
-         } catch (InterruptedException e) {  
-         }  
- returnnull;  
-     }  
- }  
4.完成。
当调用JobService中的 getJobByIdForUpdate时，就可以达到行级锁的目的了！ 
----------------------------------------------------------------------------------------------------------------------------
如果你只是需要通过jpa实现行级锁，那么好，以上的东东，已经够了。但是呢，我实际开发中，并不是那么顺的，现在我来说下我遇到了什么鬼。。。
1.行级锁，大家一定都听到过，也肯定比较喜欢。对于mysql，InnoDB预设的是Row-level Lock，但是，需要明确的指定主键，才会执行行级锁，否则执行的为表锁。
比如：
select * from job_info where id = 1 for update;  
那么上面这句，为行级锁。
而 select * from job_info where job_name = 'test' for update;
这句，就变成了表锁了。。。。（我当时泪也流干了，各种查DB引擎，命令行测试，多亏了(http://blog.sina.com.cn/s/blog_88d2d8f501011sgl.html  MySQL中select * for update锁表的问题) 这篇文章）
那么好，现在关于如何才能让mysql执行行级锁的问题解决了。。。
2.jpa如何搞 select for update。
也是醉了，在Dao层的方法上，要配置Lock的注解。并且要加上LockModeType.PESSIMISTIC_WRITE ，这个就相当于for update了。大家也可以在程序运行时，打印出的sql中看到。 这个东东，得益于 (http://suene.iteye.com/blog/1756295  Spring Data JPA,基础学习笔记.)
 该文章。
至此呢，终于解决掉了行级锁和jpa注解实现  for update 的问题。。。
注：这里写注呢，是因为前面的demo代码里，也有坑的。。大家应该也能注意到了，在Dao层中，getJobByNameForUpdate 这个方法千万不要用哦！它可是会导致锁表的哦！ 
