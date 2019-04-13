
# Mybatis 常用及复杂查询,mybatis-generator自动生成mapper接口 - wangzhiguo9261的博客 - CSDN博客


2018年06月27日 14:50:32[码里偷闲王果果](https://me.csdn.net/wangzhiguo9261)阅读数：93


#### mybatis-generator   自动生成Mapper和Mapper.xml
没有mybatis-generator的朋友 请自行百度下载
重要的事情说三遍
每一次用mybatis-generator之前都要先检查里面的src目录有没有之前生成过的文件,假如有就全部都删掉,不删掉又生成，会变成大坑,小心!!
每一次用mybatis-generator之前都要先检查里面的src目录有没有之前生成过的文件,假如有就全部都删掉,不删掉又生成，会变成大坑,小心!!
每一次用mybatis-generator之前都要先检查里面的src目录有没有之前生成过的文件,假如有就全部都删掉,不删掉又生成，会变成大坑,小心!!
这篇博客的demo是基于我上一篇博客(Mybatis持久层框架环境搭建,加简单查询,插入数据)
因为我要用自动生成的Mapper接口 和Mapper.xml所以就把上篇博客用的demo的dao包和entity包下的东西都删掉了
接下来演示自动生成那些写起来很麻烦的Mapper接口和Mapper.xml

![](https://img-blog.csdn.net/20180202115045053?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvamllZXJsaWFuc2hhbg==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)
1.配置generatorConfig.xml为:
<?xml version="1.0" encoding="UTF-8"?>
<!DOCTYPE generatorConfiguration
PUBLIC "-//mybatis.org//DTD MyBatis Generator Configuration 1.0//EN"
"http://mybatis.org/dtd/mybatis-generator-config_1_0.dtd">
<generatorConfiguration>
<!-- classPath:数据库的JDBC驱动-->
<classPathEntry
location="mysql-connector-java-5.1.25-bin.jar"/>
<context id="MysqlTables"  targetRuntime="MyBatis3">
<!-- 注意这里面的顺序确定的，不能随便更改-->
<!--可选的(0 or 1) -->
<!--注释生成器 -->
<commentGenerator>
<!-- 是否去除自动生成的注释 true 是  false:否 -->
<property name="suppressAllComments" value="true"/>
</commentGenerator>
<!--数据库连接的信息: 驱动类 、连接地址、用户名、密码-->
<jdbcConnection driverClass="com.mysql.jdbc.Driver"
connectionURL="jdbc:mysql://localhost:3306/mysql_demo"
userId="root" password="root">
</jdbcConnection>
<!-- 默认false 把JDBC DECTMAL 和 NUMERIC 类型解析为  Integer true 把JDBC DECIMAL 和 NUMERIC
类型解析为java.math.BigDecimal-->
<!-- 类型转换器或者加类型解析器-->
<javaTypeResolver>
<property name="forceBigDecimals" value="false"/>
</javaTypeResolver>
<!-- Java模型生成器-->
<!-- targetProject:自动生成代码的位置-->
<javaModelGenerator targetPackage="entity"
targetProject="src"
>
<!-- TODO enableSubPackages:是否让schema作为包的后缀-->
<property name="enableSubPackages" value="true"/>
<!-- 从数据库返回的值被清理前后的空格-->
<property name="trimStrings" value="true"/>
</javaModelGenerator>
<!--map xml生成器 -->
<sqlMapGenerator
targetPackage="dao"
targetProject="src">
<property name="enableSubPackages" value="true"/>
</sqlMapGenerator>
<!--可选的 （0or1） -->
<!-- mapper  或者 就是dao接口生成器-->
<javaClientGenerator
type="XMLMAPPER"
targetPackage="dao"
targetProject="src">
<property name="enableSubPackages" value="true"/>
</javaClientGenerator>
<!--必须的(1..N）-->
<!-- pojo实体生成器-->
<!-- tablename:用于自动生成代码的数据库表; domainObjectName:对应数据库表的javaBean类名-->
<!--schema即为数据库名  可不写 -->
<table
tableName="student"
domainObjectName="Student"
enableInsert="true"
enableCountByExample="false"
enableUpdateByExample="false"
enableDeleteByExample="false"
enableSelectByExample="false"
selectByExampleQueryId="false">
<!--忽略字段  可选的（0 or 1 ） -->
<!--//无论字段是什么类型  生成的类属性都是varchar  可选的（0 or 1） 测试无效 -->
<!-- -->
</table>
<table tableName="teacher"
domainObjectName="Teacher"
enableInsert="true"
enableCountByExample="false"
enableUpdateByExample="false"
enableDeleteByExample="false"
enableSelectByExample="false"
selectByExampleQueryId="false">
<!--忽略字段  可选的（0 or 1 ） -->
<!--//无论字段是什么类型  生成的类属性都是varchar  可选的（0 or 1） 测试无效 -->
<!-- -->
</table>
</context>
</generatorConfiguration>

table标签以上的配置,配置过一次就行了,table标签根据不同项目自己修改
我的demo有student表和teacher表


2.在cmd窗口填写命令行  调用jar包
win键+R  填入cmd就会弹出cmd窗口了
根据自己下载的mybatis-generator文件夹位置,按下图输入
![](https://img-blog.csdn.net/20180202115214561?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvamllZXJsaWFuc2hhbg==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)
再按回车

![](https://img-blog.csdn.net/20180202115321503?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvamllZXJsaWFuc2hhbg==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)
此时查看mybatis-generator文件夹下的src目录里多了dao文件夹和entity文件夹
entity文件夹里自动生成了Student实体类和Teacher实体类
dao文件夹里自动生成了 Mapper接口  和Mapperxml    这样的逆向生成代码才是成功的
这样就完成mybatis-generator自动生成mapper了  比起自己写,简直是省了一大堆时间

假如这里说有警告,生成的也是不对的 (这里是个大坑  逆向生成时存在了多个数据库,所以那里才说有一个警告 要没有警告所逆向生成的Mapper才是成功的 )
例如:
![](https://img-blog.csdn.net/20180202115705283?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvamllZXJsaWFuc2hhbg==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)
虽然生成成功了,但是有个警告,这样也是不行的

#### 一对一查询关系 （一个学生对应一个老师）
当我们实体类的属性不够用时,我们要添加对象的属性,我们就要自行修改Mapper接口了
在Student实体里添加 teacher属性,为符合javabean规范 也加了get和set方法
1.在Student类里加了：
private Teacher teacher;

public Teacher getTeacher() {
return teacher;
}

public void setTeacher(Teacher teacher) {
this.teacher = teacher;
}


2.修改StudentMapper.xml  在resultMap里添加association(关联),再添加teacherResultMap
StudentMapper.xml里的resultMap标签添加association联系,然后在下面再加一个关联的resultMap
<resultMap id="BaseResultMap" type="entity.Student">
<id column="Sid" jdbcType="VARCHAR" property="sid" />
<result column="Sname" jdbcType="VARCHAR" property="sname" />
<result column="Sage" jdbcType="TIMESTAMP" property="sage" />
<result column="Ssex" jdbcType="VARCHAR" property="ssex" />
<result column="Tid" jdbcType="INTEGER" property="tid" />
<result column="SageNum" jdbcType="INTEGER" property="sagenum" />
<association property="teacher" resultMap="teacherResultMap"/>
</resultMap>

<resultMap id="teacherResultMap" type="entity.Teacher">
<id column="Tid" jdbcType="INTEGER" property="tid" />
<result column="Tname" jdbcType="VARCHAR" property="tname" />
<result column="username" jdbcType="VARCHAR" property="username" />
<result column="password" jdbcType="VARCHAR" property="password" />
</resultMap>

关联查询就要用到多表查询了,要写新的sql语句了：
<select id="selectByPrimaryKeyWithTeacher" parameterType="java.lang.String" resultMap="BaseResultMap">
select
<include refid="Base_Column_List" />,
Tid,Tname,username,password
from student s
LEFT JOIN teacher t ON t.Tid=s.Sid
where Sid = \#{sid,jdbcType=VARCHAR}
</select>

StudenMapper.xml:

<?xml version="1.0" encoding="UTF-8" ?>
<!DOCTYPE mapper PUBLIC "-//mybatis.org//DTD Mapper 3.0//EN" "http://mybatis.org/dtd/mybatis-3-mapper.dtd" >
<mapper namespace="dao.StudentMapper" >
<resultMap id="BaseResultMap" type="entity.Student" >
<id column="Sid" property="sid" jdbcType="VARCHAR" />
<result column="Sname" property="sname" jdbcType="VARCHAR" />
<result column="Sage" property="sage" jdbcType="TIMESTAMP" />
<result column="Ssex" property="ssex" jdbcType="VARCHAR" />
<result column="Tid" property="tid" jdbcType="INTEGER" />
<result column="SageNum" property="sagenum" jdbcType="INTEGER" />
<association property="teacher" resultMap="teacherResultMap"/>
</resultMap>

<resultMap id="teacherResultMap" type="entity.Teacher">
<id column="Tid" jdbcType="INTEGER" property="tid" />
<result column="Tname" jdbcType="VARCHAR" property="tname" />
<result column="username" jdbcType="VARCHAR" property="username" />
<result column="password" jdbcType="VARCHAR" property="password" />
</resultMap>

<sql id="Base_Column_List" >
Sid, Sname, Sage, Ssex, s.Tid, SageNum
</sql>

<select id="selectByPrimaryKeyWithTeacher" parameterType="java.lang.String" resultMap="BaseResultMap">
select
<include refid="Base_Column_List" />,
t.Tid,Tname,username,password
from student s
LEFT JOIN teacher t ON t.Tid=s.Sid
where Sid = \#{sid,jdbcType=VARCHAR}
</select>

<select id="selectByPrimaryKey" resultMap="BaseResultMap" parameterType="java.lang.String" >
select
<include refid="Base_Column_List" />
from student
where Sid = \#{sid,jdbcType=VARCHAR}
</select>
<delete id="deleteByPrimaryKey" parameterType="java.lang.String" >
delete from student
where Sid = \#{sid,jdbcType=VARCHAR}
</delete>
<insert id="insert" parameterType="entity.Student" >
insert into student (Sid, Sname, Sage,
Ssex, Tid, SageNum)
values (\#{sid,jdbcType=VARCHAR}, \#{sname,jdbcType=VARCHAR}, \#{sage,jdbcType=TIMESTAMP},
\#{ssex,jdbcType=VARCHAR}, \#{tid,jdbcType=INTEGER}, \#{sagenum,jdbcType=INTEGER})
</insert>
<insert id="insertSelective" parameterType="entity.Student" >
insert into student
<trim prefix="(" suffix=")" suffixOverrides="," >
<if test="sid != null" >
Sid,
</if>
<if test="sname != null" >
Sname,
</if>
<if test="sage != null" >
Sage,
</if>
<if test="ssex != null" >
Ssex,
</if>
<if test="tid != null" >
Tid,
</if>
<if test="sagenum != null" >
SageNum,
</if>
</trim>
<trim prefix="values (" suffix=")" suffixOverrides="," >
<if test="sid != null" >
\#{sid,jdbcType=VARCHAR},
</if>
<if test="sname != null" >
\#{sname,jdbcType=VARCHAR},
</if>
<if test="sage != null" >
\#{sage,jdbcType=TIMESTAMP},
</if>
<if test="ssex != null" >
\#{ssex,jdbcType=VARCHAR},
</if>
<if test="tid != null" >
\#{tid,jdbcType=INTEGER},
</if>
<if test="sagenum != null" >
\#{sagenum,jdbcType=INTEGER},
</if>
</trim>
</insert>
<update id="updateByPrimaryKeySelective" parameterType="entity.Student" >
update student
<set >
<if test="sname != null" >
Sname = \#{sname,jdbcType=VARCHAR},
</if>
<if test="sage != null" >
Sage = \#{sage,jdbcType=TIMESTAMP},
</if>
<if test="ssex != null" >
Ssex = \#{ssex,jdbcType=VARCHAR},
</if>
<if test="tid != null" >
Tid = \#{tid,jdbcType=INTEGER},
</if>
<if test="sagenum != null" >
SageNum = \#{sagenum,jdbcType=INTEGER},
</if>
</set>
where Sid = \#{sid,jdbcType=VARCHAR}
</update>
<update id="updateByPrimaryKey" parameterType="entity.Student" >
update student
set Sname = \#{sname,jdbcType=VARCHAR},
Sage = \#{sage,jdbcType=TIMESTAMP},
Ssex = \#{ssex,jdbcType=VARCHAR},
Tid = \#{tid,jdbcType=INTEGER},
SageNum = \#{sagenum,jdbcType=INTEGER}
where Sid = \#{sid,jdbcType=VARCHAR}
</update>
</mapper>


3.因为StudentMapper.xml里面多了一个查询方法  则要在StudentMapper里面写多个方法
StudentMapper接口:
public interface StudentMapper {
int deleteByPrimaryKey(String sid);

int insert(Student record);

int insertSelective(Student record);

Student selectByPrimaryKey(String sid);

Student selectByPrimaryKeyWithTeacher(String sid);

int updateByPrimaryKeySelective(Student record);

int updateByPrimaryKey(Student record);
}


4.修改主函数:
public class Main {
public static void main(String[] args) throws Exception {
//加载配置文件  通过io流加载xml文件
InputStream in= Main.class.getResourceAsStream("resource/mybatis-cfg.xml");
SqlSessionFactory factory=new SqlSessionFactoryBuilder().build(in);
SqlSession session= factory.openSession();
StudentMapper mapper=session.getMapper(StudentMapper.class);
Student student=new Student();
student=mapper.selectByPrimaryKeyWithTeacher("1");
session.commit();
//关闭会话   避免占用资源
session.close();
}
}



运行程序:
![](https://img-blog.csdn.net/20180202120207084?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvamllZXJsaWFuc2hhbg==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)

#### 一对多的查询关系（一个老师对应了多个学生）:
1.修改TeacherMapper.xml里的查询方法
TeacherMapper.xml:

<?xml version="1.0" encoding="UTF-8" ?>
<!DOCTYPE mapper PUBLIC "-//mybatis.org//DTD Mapper 3.0//EN" "http://mybatis.org/dtd/mybatis-3-mapper.dtd" >
<mapper namespace="dao.TeacherMapper" >

<!--一对一关联用association  一对多关联用collection-->
<resultMap id="BaseResultMap" type="entity.Teacher" >
<id column="Tid" property="tid" jdbcType="INTEGER" />
<result column="Tname" property="tname" jdbcType="VARCHAR" />
<result column="username" property="username" jdbcType="VARCHAR" />
<result column="password" property="password" jdbcType="VARCHAR" />
<collection property="stuList" resultMap="stuResultMap" />
</resultMap>

<resultMap id="stuResultMap" type="entity.Student" >
<id column="Sid" property="sid" jdbcType="VARCHAR" />
<result column="Sname" property="sname" jdbcType="VARCHAR" />
<result column="Sage" property="sage" jdbcType="TIMESTAMP" />
<result column="Ssex" property="ssex" jdbcType="VARCHAR" />
<result column="Tid" property="tid" jdbcType="INTEGER" />
<result column="SageNum" property="sagenum" jdbcType="INTEGER" />
</resultMap>

<sql id="Base_Column_List" >
t.Tid, Tname, username, password
</sql>
<select id="selectByPrimaryKeyWithStudent" parameterType="java.lang.Integer" resultMap="BaseResultMap">
select
<include refid="Base_Column_List" />,
Sid, Sname, Sage, Ssex, s.Tid, SageNum
from teacher t
LEFT JOIN student s ON s.Tid=t.Tid
where t.Tid = \#{tid,jdbcType=INTEGER}
</select>

<select id="selectByPrimaryKey" resultMap="BaseResultMap" parameterType="java.lang.Integer" >
select
<include refid="Base_Column_List" />
from teacher
where Tid = \#{tid,jdbcType=INTEGER}
</select>
<delete id="deleteByPrimaryKey" parameterType="java.lang.Integer" >
delete from teacher
where Tid = \#{tid,jdbcType=INTEGER}
</delete>
<insert id="insert" parameterType="entity.Teacher" >
insert into teacher (Tid, Tname, username,
password)
values (\#{tid,jdbcType=INTEGER}, \#{tname,jdbcType=VARCHAR}, \#{username,jdbcType=VARCHAR},
\#{password,jdbcType=VARCHAR})
</insert>
<insert id="insertSelective" parameterType="entity.Teacher" >
insert into teacher
<trim prefix="(" suffix=")" suffixOverrides="," >
<if test="tid != null" >
Tid,
</if>
<if test="tname != null" >
Tname,
</if>
<if test="username != null" >
username,
</if>
<if test="password != null" >
password,
</if>
</trim>
<trim prefix="values (" suffix=")" suffixOverrides="," >
<if test="tid != null" >
\#{tid,jdbcType=INTEGER},
</if>
<if test="tname != null" >
\#{tname,jdbcType=VARCHAR},
</if>
<if test="username != null" >
\#{username,jdbcType=VARCHAR},
</if>
<if test="password != null" >
\#{password,jdbcType=VARCHAR},
</if>
</trim>
</insert>
<update id="updateByPrimaryKeySelective" parameterType="entity.Teacher" >
update teacher
<set >
<if test="tname != null" >
Tname = \#{tname,jdbcType=VARCHAR},
</if>
<if test="username != null" >
username = \#{username,jdbcType=VARCHAR},
</if>
<if test="password != null" >
password = \#{password,jdbcType=VARCHAR},
</if>
</set>
where Tid = \#{tid,jdbcType=INTEGER}
</update>
<update id="updateByPrimaryKey" parameterType="entity.Teacher" >
update teacher
set Tname = \#{tname,jdbcType=VARCHAR},
username = \#{username,jdbcType=VARCHAR},
password = \#{password,jdbcType=VARCHAR}
where Tid = \#{tid,jdbcType=INTEGER}
</update>
</mapper>


2.在TeacherMapper里面添加方法:
TeacherMapper:
public interface TeacherMapper {
int deleteByPrimaryKey(Integer tid);

int insert(Teacher record);

int insertSelective(Teacher record);

Teacher selectByPrimaryKey(Integer tid);

Teacher selectByPrimaryKeyWithStudent(Integer tid);

int updateByPrimaryKeySelective(Teacher record);

int updateByPrimaryKey(Teacher record);
}


3.修改主函数:
public class Main {

public static void main(String[] args) throws Exception {
//加载配置文件  通过io流加载xml文件
InputStream in= Main.class.getResourceAsStream("resource/mybatis-cfg.xml");
SqlSessionFactory factory=new SqlSessionFactoryBuilder().build(in);
SqlSession session= factory.openSession();
TeacherMapper mapper=session.getMapper(TeacherMapper.class);
Teacher teacher=new Teacher();
teacher=mapper.selectByPrimaryKeyWithStudent(1);
//mybatis自动把查询出的数据,自动映射,封装成对象  大大节省了时间
//现在看来   写StudentMapper  和StudentMapper.xml都很费时间  在后面,我会在博客里说如何自动生成Mapper和Mapper.xml
session.commit();
//关闭会话   避免占用资源
session.close();
}
}

执行程序:
![](https://img-blog.csdn.net/20180202120438345?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvamllZXJsaWFuc2hhbg==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)
查询出了一个老师对象和学生列表(里面有四个学生对象)

#### 倘若前端传来一个表单  要我填入一个老师的信息和一个学生的信息
要往数据库"同时"插入学生和老师的信息
问题来了,一个老师的Tid对应一个学生的Tid,要怎样才能在插入数据库之前,学生对象拿到老师的Tid,拿到之后一个学生就能绑定一个老师了

1.修改主函数:
public class Main {

public static void main(String[] args) throws Exception {
//加载配置文件  通过io流加载xml文件
InputStream in= Main.class.getResourceAsStream("resource/mybatis-cfg.xml");
SqlSessionFactory factory=new SqlSessionFactoryBuilder().build(in);
SqlSession session= factory.openSession();
TeacherMapper mapper=session.getMapper(TeacherMapper.class);
StudentMapper studentMapper=session.getMapper(StudentMapper.class);
Teacher teacher=new Teacher();
teacher.setPassword("123456");
teacher.setUsername("gaga");
//用TeacherMapper,先插入教师的信息,mybatis再把插入的Tid返回来
mapper.insertSelective(teacher);
Student stu=new Student();
stu.setSagenum(12);
stu.setSname("gugu");
stu.setSid("20");
stu.setTid(teacher.getTid());
studentMapper.insertSelective(stu);
//mybatis自动把查询出的数据,自动映射,封装成对象  大大节省了时间
//现在看来   写StudentMapper  和StudentMapper.xml都很费时间  在后面,我会在博客里说如何自动生成Mapper和Mapper.xml
session.commit();
//关闭会话   避免占用资源
session.close();
}
}



2.修改TeacherMapper.xml里的插入语句:
TeacherMapper.xml:
<?xml version="1.0" encoding="UTF-8" ?>
<!DOCTYPE mapper PUBLIC "-//mybatis.org//DTD Mapper 3.0//EN" "http://mybatis.org/dtd/mybatis-3-mapper.dtd" >
<mapper namespace="dao.TeacherMapper" >

<!--一对一关联用association  一对多关联用collection-->
<resultMap id="BaseResultMap" type="entity.Teacher" >
<id column="Tid" property="tid" jdbcType="INTEGER" />
<result column="Tname" property="tname" jdbcType="VARCHAR" />
<result column="username" property="username" jdbcType="VARCHAR" />
<result column="password" property="password" jdbcType="VARCHAR" />
<collection property="stuList" resultMap="stuResultMap" />
</resultMap>

<resultMap id="stuResultMap" type="entity.Student" >
<id column="Sid" property="sid" jdbcType="VARCHAR" />
<result column="Sname" property="sname" jdbcType="VARCHAR" />
<result column="Sage" property="sage" jdbcType="TIMESTAMP" />
<result column="Ssex" property="ssex" jdbcType="VARCHAR" />
<result column="Tid" property="tid" jdbcType="INTEGER" />
<result column="SageNum" property="sagenum" jdbcType="INTEGER" />
</resultMap>

<sql id="Base_Column_List" >
t.Tid, Tname, username, password
</sql>

<select id="selectByPrimaryKeyWithStudent" parameterType="java.lang.Integer" resultMap="BaseResultMap">
select
<include refid="Base_Column_List" />,
Sid, Sname, Sage, Ssex, s.Tid, SageNum
from teacher t
LEFT JOIN student s ON s.Tid=t.Tid
where t.Tid = \#{tid,jdbcType=INTEGER}
</select>
<select id="selectByPrimaryKey" resultMap="BaseResultMap" parameterType="java.lang.Integer" >
select
<include refid="Base_Column_List" />
from teacher
where Tid = \#{tid,jdbcType=INTEGER}
</select>
<delete id="deleteByPrimaryKey" parameterType="java.lang.Integer" >
delete from teacher
where Tid = \#{tid,jdbcType=INTEGER}
</delete>
<insert id="insert" parameterType="entity.Teacher" >
insert into teacher (Tid, Tname, username,
password)
values (\#{tid,jdbcType=INTEGER}, \#{tname,jdbcType=VARCHAR}, \#{username,jdbcType=VARCHAR},
\#{password,jdbcType=VARCHAR})
</insert>

<!--useGeneratedKeys  是否返回主键   keyProperty  返回的主键设置到哪一个字段  useGeneratedKeys="true"把插入的主键返回来-->
<insert id="insertSelective" parameterType="entity.Teacher"   useGeneratedKeys="true" keyProperty="tid">
insert into teacher
<trim prefix="(" suffix=")" suffixOverrides="," >
<if test="tid != null" >
Tid,
</if>
<if test="tname != null" >
Tname,
</if>
<if test="username != null" >
username,
</if>
<if test="password != null" >
password,
</if>
</trim>
<trim prefix="values (" suffix=")" suffixOverrides="," >
<if test="tid != null" >
\#{tid,jdbcType=INTEGER},
</if>
<if test="tname != null" >
\#{tname,jdbcType=VARCHAR},
</if>
<if test="username != null" >
\#{username,jdbcType=VARCHAR},
</if>
<if test="password != null" >
\#{password,jdbcType=VARCHAR},
</if>
</trim>
</insert>
<update id="updateByPrimaryKeySelective" parameterType="entity.Teacher" >
update teacher
<set >
<if test="tname != null" >
Tname = \#{tname,jdbcType=VARCHAR},
</if>
<if test="username != null" >
username = \#{username,jdbcType=VARCHAR},
</if>
<if test="password != null" >
password = \#{password,jdbcType=VARCHAR},
</if>
</set>
where Tid = \#{tid,jdbcType=INTEGER}
</update>
<update id="updateByPrimaryKey" parameterType="entity.Teacher" >
update teacher
set Tname = \#{tname,jdbcType=VARCHAR},
username = \#{username,jdbcType=VARCHAR},
password = \#{password,jdbcType=VARCHAR}
where Tid = \#{tid,jdbcType=INTEGER}
</update>
</mapper>

执行程序,查看数据库:
![](https://img-blog.csdn.net/20180202120909677?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvamllZXJsaWFuc2hhbg==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)
新的教师成功关联了一个新的同学
#### 倘若我要查询老师ID为1和2的信息（用list来查询,list集合里面包含1和2）
因为是mybatis是采用动态sql查询的  所以弄的时候麻烦了一点:

1.在TeacherMapper.xml里添加selectByTids查询语句:
TeacherMapper.xml:
<?xml version="1.0" encoding="UTF-8" ?>
<!DOCTYPE mapper PUBLIC "-//mybatis.org//DTD Mapper 3.0//EN" "http://mybatis.org/dtd/mybatis-3-mapper.dtd" >
<mapper namespace="dao.TeacherMapper" >
<!--一对一关联用association  一对多关联用collection-->
<resultMap id="BaseResultMap" type="entity.Teacher" >
<id column="Tid" property="tid" jdbcType="INTEGER" />
<result column="Tname" property="tname" jdbcType="VARCHAR" />
<result column="username" property="username" jdbcType="VARCHAR" />
<result column="password" property="password" jdbcType="VARCHAR" />
<collection property="stuList" resultMap="stuResultMap" />
</resultMap>

<resultMap id="stuResultMap" type="entity.Student" >
<id column="Sid" property="sid" jdbcType="VARCHAR" />
<result column="Sname" property="sname" jdbcType="VARCHAR" />
<result column="Sage" property="sage" jdbcType="TIMESTAMP" />
<result column="Ssex" property="ssex" jdbcType="VARCHAR" />
<result column="Tid" property="tid" jdbcType="INTEGER" />
<result column="SageNum" property="sagenum" jdbcType="INTEGER" />
</resultMap>

<sql id="Base_Column_List" >
t.Tid, Tname, username, password
</sql>

<select id="selectByPrimaryKeyWithStudent" parameterType="java.lang.Integer" resultMap="BaseResultMap">
select
<include refid="Base_Column_List" />,
Sid, Sname, Sage, Ssex, s.Tid, SageNum
from teacher t
LEFT JOIN student s ON s.Tid=t.Tid
where t.Tid = \#{tid,jdbcType=INTEGER}
</select>

<select id="selectByTids" resultMap="BaseResultMap" parameterType="list" >
select
<include refid="Base_Column_List" />
from teacher t
where t.Tid IN
<foreach collection="tids" item="it" open="(" close=")" separator="," >
\#{it}
</foreach>
</select>

<select id="selectByPrimaryKey" resultMap="BaseResultMap" parameterType="java.lang.Integer" >
select
<include refid="Base_Column_List" />
from teacher
where Tid = \#{tid,jdbcType=INTEGER}
</select>

<delete id="deleteByPrimaryKey" parameterType="java.lang.Integer" >
delete from teacher
where Tid = \#{tid,jdbcType=INTEGER}
</delete>
<insert id="insert" parameterType="entity.Teacher" >
insert into teacher (Tid, Tname, username,
password)
values (\#{tid,jdbcType=INTEGER}, \#{tname,jdbcType=VARCHAR}, \#{username,jdbcType=VARCHAR},
\#{password,jdbcType=VARCHAR})
</insert>

<!--useGeneratedKeys  是否返回主键   keyProperty  返回的主键设置到哪一个字段  useGeneratedKeys="true"把插入的主键返回来-->
<insert id="insertSelective" parameterType="entity.Teacher"   useGeneratedKeys="true" keyProperty="tid">
insert into teacher
<trim prefix="(" suffix=")" suffixOverrides="," >
<if test="tid != null" >
Tid,
</if>
<if test="tname != null" >
Tname,
</if>
<if test="username != null" >
username,
</if>
<if test="password != null" >
password,
</if>
</trim>
<trim prefix="values (" suffix=")" suffixOverrides="," >
<if test="tid != null" >
\#{tid,jdbcType=INTEGER},
</if>
<if test="tname != null" >
\#{tname,jdbcType=VARCHAR},
</if>
<if test="username != null" >
\#{username,jdbcType=VARCHAR},
</if>
<if test="password != null" >
\#{password,jdbcType=VARCHAR},
</if>
</trim>
</insert>
<update id="updateByPrimaryKeySelective" parameterType="entity.Teacher" >
update teacher
<set >
<if test="tname != null" >
Tname = \#{tname,jdbcType=VARCHAR},
</if>
<if test="username != null" >
username = \#{username,jdbcType=VARCHAR},
</if>
<if test="password != null" >
password = \#{password,jdbcType=VARCHAR},
</if>
</set>
where Tid = \#{tid,jdbcType=INTEGER}
</update>
<update id="updateByPrimaryKey" parameterType="entity.Teacher" >
update teacher
set Tname = \#{tname,jdbcType=VARCHAR},
username = \#{username,jdbcType=VARCHAR},
password = \#{password,jdbcType=VARCHAR}
where Tid = \#{tid,jdbcType=INTEGER}
</update>
</mapper>
2.在TeacherMapper里添加selectByTids方法:
TeacherMapper:
public interface TeacherMapper {
int deleteByPrimaryKey(Integer tid);
int insert(Teacher record);
int insertSelective(Teacher record);
Teacher selectByPrimaryKey(Integer tid);
Teacher selectByPrimaryKeyWithStudent(Integer tid);
//复杂类型查询要用@Param标签声明一下
List<Teacher> selectByTids(@Param("tids") List<Integer> tids);

int updateByPrimaryKeySelective(Teacher record);
int updateByPrimaryKey(Teacher record);
}

3.修改主函数:
public class Main {
public static void main(String[] args) throws Exception {
//加载配置文件  通过io流加载xml文件
InputStream in= Main.class.getResourceAsStream("resource/mybatis-cfg.xml");
SqlSessionFactory factory=new SqlSessionFactoryBuilder().build(in);
SqlSession session= factory.openSession();
TeacherMapper mapper=session.getMapper(TeacherMapper.class);
List<Teacher> teacherList=new ArrayList<>();
List<Integer> ids=new ArrayList<>();
//查询1号和2号老师的集合
ids.add(1);
ids.add(2);
teacherList= mapper.selectByTids(ids);
//mybatis自动把查询出的数据,自动映射,封装成对象  大大节省了时间
//现在看来   写StudentMapper  和StudentMapper.xml都很费时间  在后面,我会在博客里说如何自动生成Mapper和Mapper.xml
session.commit();
//关闭会话   避免占用资源
session.close();
}
}
执行程序,查询成功：
![](https://img-blog.csdn.net/20180202121335343?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvamllZXJsaWFuc2hhbg==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)












