# MyBatis之TypeHandler - z69183787的专栏 - CSDN博客
2018年09月21日 11:09:55[OkidoGreen](https://me.csdn.net/z69183787)阅读数：93
[https://www.cnblogs.com/yulinfeng/p/5991170.html](https://www.cnblogs.com/yulinfeng/p/5991170.html)
在大学写web应用的时候经常会遇到这么个问题，当我要插入一条数据，某个数据是Date类型，数据库中却是VARCHAR类型，这个时候可能会傻乎乎的先把这个数据自己手动转换成String类型再插入到数据库中，其实大可不必。MyBatis为我们提供了更好的方法即是TypeHandler来应对Java和jdbc字段类型不匹配的情况。MyBatis中内置了不少的TypeHandler，如果我们想要自己自定义一个TypeHandler可以实现TypeHandler接口，也可以继承BaseTypeHandler类。下面我们实现一个将Java中的Date类型利用我们自定义的ExampleTypeHandler来转换为JDBC的VARCHAR类型。
　　我们对MyBatis的介绍先局限在使用，在会使用过后我们再究其原理、源码。
```java
package day_8_mybatis.util;
import java.sql.CallableStatement;
import java.sql.PreparedStatement;
import java.sql.ResultSet;
import java.sql.SQLException;
import java.util.Date;
import org.apache.ibatis.type.BaseTypeHandler;
import org.apache.ibatis.type.JdbcType;
/**
 * 注意在引入Date所在的包时，是java.util.Date，而不是java.sql.Date，这一点不要搞错。
 * @author turbo
 *
 * 2016年10月23日
 */
public class ExampleTypeHandler extends BaseTypeHandler<Date> {
    /* 根据列名，获取可以为空的结果
     * @see org.apache.ibatis.type.BaseTypeHandler#getNullableResult(java.sql.ResultSet, java.lang.String)
     */
    @Override
    public Date getNullableResult(ResultSet rs, String columnName)
            throws SQLException {
        String sqlTimetamp = rs.getString(columnName);
        if (null != sqlTimetamp){
            return new Date(Long.valueOf(sqlTimetamp));
        }
        return null;
    }
    /* 根据列索引，获取可以为空的结果
     * @see org.apache.ibatis.type.BaseTypeHandler#getNullableResult(java.sql.ResultSet, int)
     */
    @Override
    public Date getNullableResult(ResultSet rs, int columnIndex)
            throws SQLException {
        String sqlTimetamp = rs.getString(columnIndex);
        if (null != sqlTimetamp){
            return new Date(Long.valueOf(sqlTimetamp));
        }
        return null;
    }
    /*47      * @see org.apache.ibatis.type.BaseTypeHandler#getNullableResult(java.sql.CallableStatement, int)
     */
    @Override
    public Date getNullableResult(CallableStatement cs, int columnIndex)
            throws SQLException {
        String sqlTimetamp = cs.getString(columnIndex);
        if (null != sqlTimetamp){
            return new Date(Long.valueOf(sqlTimetamp));
        }
        return null;
    }
    /* 设置非空参数
     * @see org.apache.ibatis.type.BaseTypeHandler#setNonNullParameter(java.sql.PreparedStatement, int, java.lang.Object, org.apache.ibatis.type.JdbcType)
     */
    @Override
    public void setNonNullParameter(PreparedStatement ps, int i, Date parameter,
            JdbcType jdbcType) throws SQLException {
        ps.setString(i, String.valueOf(parameter.getTime()));
    }
    
}
```
　我们已经自定义了一个TypeHandler，接着我们要在mybatis-config.xml中注册。
```
<?xml version="1.0" encoding="UTF-8"?>
<!DOCTYPE configuration  
  PUBLIC "-//mybatis.org//DTD Config 3.0//EN"  
  "http://mybatis.org/dtd/mybatis-3-config.dtd">
  
<configuration>
<!-- 注意configuration中各个属性配置的顺序应为：properties,settings,typeAliases,typeHandlers,objectFactory,objectWrapperFactory,reflectorFactory,plugins,environments,databaseIdProvider,mappers)-->
    <properties>
        <property name="driver" value="com.mysql.jdbc.Driver"/>
        <property name="url" value="jdbc:mysql://localhost:3306/test"/>    
        <property name="username" value="root"/>
        <property name="password" value="0000"/>
    </properties>
    <typeHandlers>
        <typeHandler handler="day_8_mybatis.util.ExampleTypeHandler" javaType="java.util.Date" jdbcType="VARCHAR"/>
    </typeHandlers>
    
    <environments default="development">
        <environment id="development">
            <transactionManager type="JDBC" />
            <dataSource type="POOLED">
                <property name="driver" value="${driver}"/>
                <property name="url" value="${url}"/>
                <property name="username" value="${username}"/>
                <property name="password" value="${password}"/>
            </dataSource>            
        </environment>
    </environments> 
    <mappers>
        <mapper resource="day_8_mybatis/mapper/UserMapper.xml"/>
        <mapper resource="day_8_mybatis/mapper/NoteMapper.xml"/>
    </mappers>    
    
</configuration>
```
注意各个属性配置有顺序之分，不能随意穿插。
　　准备工作已经做完了，我们接着按部就班的实现POJO类 Note里面包含id和date字段。
```java
package day_8_mybatis.pojo;
import java.util.Date;
/**
 * @author turbo
 *
 * 2016年10月23日
 */
public class Note {
    private int id;
    private Date date;
    public int getId() {
        return id;
    }
    public void setId(int id) {
        this.id = id;
    }
    public Date getDate() {
        return date;
    }
    public void setDate(Date date) {
        this.date = date;
    }
}
```
接着是负责与数据库交互Dao层的NoteMapper接口，我们只举例查询和插入。
```java
package day_8_mybatis.mapper;
import day_8_mybatis.pojo.Note;
/**
 * @author turbo
 *
 * 2016年10月23日
 */
public interface NoteMapper {
    Note queryNote(int id);
    void insertNote(Note note);
}
```
我们再来看看在NoteMapper.xml中是如何利用我们刚才自定义的TypeHandler。
```
<?xml version="1.0" encoding="UTF-8"?>
<!DOCTYPE mapper  
  PUBLIC "-//mybatis.org//DTD Mapper 3.0//EN"  
  "http://mybatis.org/dtd/mybatis-3-mapper.dtd"> 
<mapper namespace="day_8_mybatis.mapper.NoteMapper">
    <resultMap type="day_8_mybatis.pojo.Note" id="note-base">
        <result property="id" column="id"/>
        <result property="date" column="date" typeHandler="day_8_mybatis.util.ExampleTypeHandler"/>
    </resultMap>
    
    <select id="queryNote" parameterType="int" resultMap="note-base">
        select * from note where id = #{id}
    </select>
    <insert id="insertNote" parameterType="day_8_mybatis.pojo.Note">
        insert into note (id, date) values(#{id}, #{date, typeHandler=day_8_mybatis.util.ExampleTypeHandler})　　<!--使用我们自定义的TypeHandler-->
    </insert>
</mapper>
```
最后我们在客户端测试一下。
```java
package day_8_mybatis;
import java.io.IOException;
import java.util.Date;
import org.apache.ibatis.session.SqlSession;
import day_8_mybatis.mapper.NoteMapper;
import day_8_mybatis.pojo.Note;
import day_8_mybatis.util.SessionFactory;
/**
 * 客户端
 * @author turbo
 *
 * 2016年9月11日
 */
public class Main {
    /**
     * @param args
     * @throws IOException 
     */
    public static void main(String[] args) throws Exception {
        String resource = "day_8_mybatis/mybatis-config.xml";        //获取mybatis配置文件路径
        SqlSession sqlSession = SessionFactory.getSqlSession(resource);    //通过SessionFactory工具类（此工具类为自己构造即util包中的SessionFactory）构造SqlSession
        
        NoteMapper noteMapper = sqlSession.getMapper(NoteMapper.class);
        
        Note note = new Note();
        note.setId(1);
        note.setDate(new Date());
        noteMapper.insertNote(note);    //插入
        sqlSession.commit();    //注意需要手动提交事务
        
        note = noteMapper.queryNote(2);    //查询
        System.out.println(note.getDate());
    }
}
```
注意在34行代码，需要手动提交事务，默认是关闭自动提交的，所以必须手动提交。开始没有提交事务，无论怎么都没办法插入到数据库，后来debug单步调试的时候发现了autoCommit=false，才想起来在以前大学的时候也遇到过这个这个问题所以一下就定位问题在哪儿了。
　　数据库中只有一个note表，字段为id类型为int，date字段为varchar。
![](https://images2015.cnblogs.com/blog/630246/201610/630246-20161023215332654-1207527077.png)
　　至此我们就完成了自定义的TypeHandler，其实MyBatis为我们提供的TypeHandler已经不少了，不过我们还是自己试验一把，先把MyBatis学会使用，再究其原理。
