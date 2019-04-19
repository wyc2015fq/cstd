# hbm2java和hbm2ddl的使用步骤 - xqhrs232的专栏 - CSDN博客
2018年11月23日 18:06:44[xqhrs232](https://me.csdn.net/xqhrs232)阅读数：61
个人分类：[JavaWeb技术](https://blog.csdn.net/xqhrs232/article/category/8231541)
原文地址::[https://blog.csdn.net/jellins/article/details/2028192](https://blog.csdn.net/jellins/article/details/2028192)
相关文章
1、用hbm2java生成Hibernate类----[https://www.aliyun.com/jiaocheng/346447.html](https://www.aliyun.com/jiaocheng/346447.html)
最近在项目中使用了hibernate作为持久层技术方案,所以为了加快开发效率.我使用了一些工具,如hbm2java和hbm2ddl,ant
那什么是hbm2java呢,顾名思义就是**.hbm.xml自动生成javabean.而hbm2ddl就是**.hbm.xml生成特定数据库的SQL.那接下来我介绍这几个工具如何使用.
1.首先需要安装ant.
将ant下载后解压到任意目录下(如果有eclipse的话,本身就集成有ant,所以不用在下载).然后在环境变量中设置ANT_HOME,ANT_HOME为ant的安装路径,然后将ANT_HOME/lib加入到classpath中,将ANT_HOME/bin加到path中.这样就完成了ant的环境设置.
2.书写bulid.xml
 要使用ant,必须要编写bulid.xml文件,用下面的一个bulid.xml例子来说明怎么使hbm2java和hbm2ddl工作.
<?xml version="1.0"?>
 <project name="" basedir="." default="">
 <property name="source.root" value="src"/>
 <property name="class.root" value="classes"/>
 <property name="lib.dir" value="lib"></property>
 <property name="schema.dir" value="schema"></property> 
  <path id="project.path">
   <pathelement location="${class.root}"/>
    <fileset dir="WebRoot/WEB-INF/lib">
      <include name="*.jar"/>
    </fileset>
  </path>
 <target name="prepare" description="Set up bulid structures">
  <delete dir="${class.root}"></delete>
    <mkdir dir="${class.root}"/>
  <copy todir="${class.root}">
  <fileset dir="${source.root}">
   <include name="**/*.hbm.xml"/>
  </fileset>
  </copy>
 </target>
 <target name="compile" depends="hbm2java">
 <javac srcdir="src/com/hollycrm/testframework/bean"
        destdir="${class.root}"
        debug="on"
        optimize="off"
        deprecation="on">   
 <classpath refid="project.path"/>
</javac>
 </target>
    <target name="hbm2java" depends="prepare">
      <taskdef name="hbm2java" classname="net.sf.hibernate.tool.hbm2java.Hbm2JavaTask" classpathref="project.path"/>
//net.sf.hibernate.tool.hbm2java.Hbm2JavaTask类必须在项目lib下.
      <hbm2java config="hibernate.codegen.xml" output="${source.root}">
<!--hibernate.codegen.xml是一个辅助用的xml内容是
<codegen>
  <generate renderer="net.sf.hibernate.tool.hbm2java.BasicRenderer"/>
</codegen> 
-->
        <fileset dir="${source.root}">
          <include name="**/*.hbm.xml"/>   
        </fileset>
      </hbm2java>
    </target>
  <target name="hbm2ddl" depends="compile" description="Create SQL by *.hbm.xml">
        <taskdef name="hbm2ddlexport" 
   classname="net.sf.hibernate.tool.hbm2ddl.SchemaExportTask" 
   classpathref="project.path"/>
   <hbm2ddlexport properties="hibernate.properties"
     quiet="no" drop="no" text="yes" output="hibernate.sql" delimiter=";">
/**
hibernate.properties中设置生成何种数据库SQL比如生成MySQL脚本DDL则:
hibernate.dialect=net.sf.hibernate.dialect.MySQLDialect
hibernate.connection.dirver_class=org.gjt.mm.mysql.Driver
hibernate.connection.url=jdbc:mysql://localhost:3306/mytest
hibernate.connection.username=root
hibernate.connection.password=jordan
hibernate.show_sql=true
其他数据库以此类推
*/
     <fileset dir ="${class.root}">
     <include name="**/*.hbm.xml"/>
     </fileset>  
      </hbm2ddlexport>  
  </target> 
</project>
3.执行ant,在cmd中,到build.xml所在的路径下.执行 ant hbm2java则由**.hbm.xml 生成 javabean,执行ant hbm2ddl则生成该**.hbm.xml对应的ddl文件.
4.到此为之,hbm2java和hbm2ddl介绍完毕.如果想用ddl生成hbm.xml可以选择使用Middlegen.javabean生成映射文件则可以使用XDoclet来实现.
--------------------- 
作者：jellins 
来源：CSDN 
原文：https://blog.csdn.net/jellins/article/details/2028192 
版权声明：本文为博主原创文章，转载请附上博文链接！
