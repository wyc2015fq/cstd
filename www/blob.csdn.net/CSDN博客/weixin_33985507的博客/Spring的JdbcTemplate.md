# Spring的JdbcTemplate - weixin_33985507的博客 - CSDN博客
2018年12月17日 21:03:00[weixin_33985507](https://me.csdn.net/weixin_33985507)阅读数：10
#### 一、JdbcTemplate
JdbcTemplate是spring框架中提供的一个对象，是对原始Jdbc API对象的简单封装。
spring框架提供的操作模板类。
|ORM持久化技术|模板类|
|----|----|
|JDBC|org.springframework.jdbc.core.JdbcTemplate|
|Hibernate3.0|org.springframework.orm.hibernate3.HibernateTemplate|
|IBatis(MyBatis)|org.springframework.orm.ibatis.SqlMapClientTemplate|
|JPA|org.springframework.orm.jpa.JpaTemplate|
#### 二、JdbcTemplate的增删改查操作
**1.数据库准备**
![7900193-cc25f92f2a60ac14.png](https://upload-images.jianshu.io/upload_images/7900193-cc25f92f2a60ac14.png)
数据库
**2.在spring配置文件中配置JdbcTemplate**
bean.xml
```
<?xml version="1.0" encoding="UTF-8"?>
<beans xmlns="http://www.springframework.org/schema/beans"
       xmlns:xsi="http://www.w3.org/2001/XMLSchema-instance"
       xsi:schemaLocation="http://www.springframework.org/schema/beans
                           http://www.springframework.org/schema/beans/spring-beans.xsd">
    <!-- 配置JdbcTemplate -->
    <bean id="jdbcTemplate" class="org.springframework.jdbc.core.JdbcTemplate">
        <property name="dataSource" ref="dataSource"></property>
    </bean>
    <!-- 配置数据源 -->
    <bean id="dataSource" class="org.springframework.jdbc.datasource.DriverManagerDataSource">
        <property name="driverClassName" value="com.mysql.jdbc.Driver"></property>
        <property name="url" value="jdbc:mysql://localhost:3306/goods"></property>
        <property name="username" value="root"></property>
        <property name="password" value="root"></property>
    </bean>
</beans>
```
**3.实体类stu.java**
```
package com.edu.domain;
public class Stu {
    private String sid;
    private String sname;
    private Integer age;
    private String gender;
    @Override
    public String toString() {
        return "Stu{" +
                "sid='" + sid + '\'' +
                ", sname='" + sname + '\'' +
                ", age=" + age +
                ", gender='" + gender + '\'' +
                '}';
    }
    public String getSid() {
        return sid;
    }
    public void setSid(String sid) {
        this.sid = sid;
    }
    public String getSname() {
        return sname;
    }
    public void setSname(String sname) {
        this.sname = sname;
    }
    public Integer getAge() {
        return age;
    }
    public void setAge(Integer age) {
        this.age = age;
    }
    public String getGender() {
        return gender;
    }
    public void setGender(String gender) {
        this.gender = gender;
    }
}
```
**4.CRUD基本操作**
```
package com.edu.springTemplate;
import com.edu.domain.Stu;
import org.springframework.context.ApplicationContext;
import org.springframework.context.support.ClassPathXmlApplicationContext;
import org.springframework.jdbc.core.BeanPropertyRowMapper;
import org.springframework.jdbc.core.JdbcTemplate;
import java.util.List;
public class JdbcTemplateDemo2 {
    public static void main(String[] args) {
        //1.获取容器
        ApplicationContext ac = new ClassPathXmlApplicationContext("bean.xml");
        //2.根据id获取bean对象
        JdbcTemplate jdbcTemplate = (JdbcTemplate) ac.getBean("jdbcTemplate");
        //3.执行操作
        //增删改
//        jdbcTemplate.update("insert into stu(sid,sname,age,gender) values(125,'bbb',2,'男')");
        //查
                                                //sql语句，结果集处理，参数
        List<Stu> list = jdbcTemplate.query("select * from stu where sid = ?",new BeanPropertyRowMapper<Stu>(Stu.class),1);
        for (Stu stus : list){
            System.out.println(stus);
        }
        //查询返回一行一列,查询聚合函数
        Integer integer = jdbcTemplate.queryForObject("select count(*) from stu where sid = ?",Integer.class,1);
        System.out.println(integer);
    }
}
```
![7900193-2f1ae717e9fb6f66.png](https://upload-images.jianshu.io/upload_images/7900193-2f1ae717e9fb6f66.png)
运行结果
#### 三、在dao中使用JdbcTemplate
**第一种**
1.Dao层接口
```
package com.edu.dao;
import com.edu.domain.Stu;
public interface IStuDao {
    /**
     * 根据id查询学生
     * @param sid
     * @return
     */
    Stu findStuById(Integer sid);
    /**
     * 更新学生信息
     * @param stu
     * @return
     */
    void updateStu(Stu stu);
}
```
2.Dao层实现类
```
package com.edu.dao.Impl;
import com.edu.dao.IStuDao;
import com.edu.domain.Stu;
import org.springframework.jdbc.core.BeanPropertyRowMapper;
import org.springframework.jdbc.core.JdbcTemplate;
import java.util.List;
public class StuImpl implements IStuDao {
    private JdbcTemplate jdbcTemplate;
    public void setJdbcTemplate(JdbcTemplate jdbcTemplate) {
        this.jdbcTemplate = jdbcTemplate;
    }
    @Override
    public Stu findStuById(Integer sid) {
        List<Stu> list = jdbcTemplate.query("select * from stu where sid = ?",new BeanPropertyRowMapper<Stu>(Stu.class),1);
        return list.isEmpty()?null:list.get(0);
    }
    @Override
    public void updateStu(Stu stu) {
        jdbcTemplate.update("update stu set age = ?,sname = ? where sid = 2",stu.getAge(),stu.getSname(),stu.getSid());
    }
}
```
3.配置dao
bean.xml
```
package com.edu.springTemplate;
import com.edu.dao.IStuDao;
import com.edu.domain.Stu;
import org.springframework.context.ApplicationContext;
import org.springframework.context.support.ClassPathXmlApplicationContext;
public class JdbcTemplateDemo3 {
    public static void main(String[] args) {
        ApplicationContext ac = new ClassPathXmlApplicationContext("bean.xml");
        IStuDao stuDao = (IStuDao) ac.getBean("stuDao");
        Stu stu = stuDao.findStuById(1);
        System.out.println(stu);
    }
}
```
![7900193-cfb5093482760253.png](https://upload-images.jianshu.io/upload_images/7900193-cfb5093482760253.png)
运行结果
**这种方法在dao很多的时候，每个dao都有重复代码**
重复代码：
```
private JdbcTemplate jdbcTemplate;
    public void setJdbcTemplate(JdbcTemplate jdbcTemplate) {
        this.jdbcTemplate = jdbcTemplate;
    }
```
**第二种**
JdbcDaoSupport是spring框架提供的一个类，该类中定义了一个JdbcTemplate对象，可以直接获取使用,也就是将第一种的重复代码抽取了出来。
只需要在dao的实现类里继承JdbcDaoSupport。
dao层实现类
```
package com.edu.dao.Impl;
import com.edu.dao.IStuDao;
import com.edu.domain.Stu;
import org.springframework.jdbc.core.BeanPropertyRowMapper;
import org.springframework.jdbc.core.support.JdbcDaoSupport;
import java.util.List;
public class StuImpl extends JdbcDaoSupport implements IStuDao {
    @Override
    public Stu findStuById(Integer sid) {
        List<Stu> list = getJdbcTemplate().query("select * from stu where sid = ?",new BeanPropertyRowMapper<Stu>(Stu.class),1);
        return list.isEmpty()?null:list.get(0);
    }
    @Override
    public void updateStu(Stu stu) {
        getJdbcTemplate().update("update stu set age = ?,sname = ? where sid = 2",stu.getAge(),stu.getSname(),stu.getSid());
    }
}
```
![7900193-aee11f831247aac1.png](https://upload-images.jianshu.io/upload_images/7900193-aee11f831247aac1.png)
运行结果
