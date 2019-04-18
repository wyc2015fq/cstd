# Ant全攻略 - 深之JohnChen的专栏 - CSDN博客

2009年01月09日 17:38:00[byxdaz](https://me.csdn.net/byxdaz)阅读数：1130标签：[ant																[path																[build																[structure																[file																[java](https://so.csdn.net/so/search/s.do?q=java&t=blog)](https://so.csdn.net/so/search/s.do?q=file&t=blog)](https://so.csdn.net/so/search/s.do?q=structure&t=blog)](https://so.csdn.net/so/search/s.do?q=build&t=blog)](https://so.csdn.net/so/search/s.do?q=path&t=blog)](https://so.csdn.net/so/search/s.do?q=ant&t=blog)
个人分类：[软件测试](https://blog.csdn.net/byxdaz/article/category/506291)


**Ant全攻略**

文章出处：BEA论坛 作者：powerise 

1.Ant是什么？
Ant是一种基于Java和XML的build工具。

2 下载、安装Ant
安装Ant
下载.zip文件，解压缩到c:/ant1.3(后面引用为%ANT_HOME%)

2.1 在你运行Ant之前需要做一些配置工作。
· 将bin目录加入PATH环境变量。
· 设定ANT_HOME环境变量，指向你安装Ant的目录。在一些OS上，Ant的脚本可以猜测ANT_HOME（Unix和Windos NT/2000）－但最好不要依赖这一特性。
· 可选地，设定JAVA_HOME环境变量（参考下面的高级小节），该变量应该指向你安装JDK的目录。
注意：不要将Ant的ant.jar文件放到JDK/JRE的lib/ext目录下。Ant是个应用程序，而lib/ext目录是为JDK扩展使用的（如JCE，JSSE扩展）。而且通过扩展装入的类会有安全方面的限制。
2.2 运行Ant

运行Ant非常简单，当你正确地安装Ant后，只要输入ant就可以了。

n 没有指定任何参数时，Ant会在当前目录下查询build.xml文件。如果找到了就用该文件作为buildfile。如果你用 -find 选项。Ant就会在上级目录中寻找buildfile，直至到达文件系统的根。要想让Ant使用其他的buildfile，可以用参数 -buildfile file，这里file指定了你想使用的buildfile。

n 可以指定执行一个或多个target。当省略target时，Ant使用标签<project>的default属性所指定的target。

命令行选项总结：
ant [options] [target [target2 [target3] ...]]
Options:
-help print this message
-projecthelp print project help information
-version print the version information and exit
-quiet be extra quiet
-verbose be extra verbose
-debug print debugging information
-emacs produce logging information without adornments
-logfile file use given file for log output
-logger classname the class that is to perform logging
-listener classname add an instance of class as a project listener
-buildfile file use specified buildfile
-find file search for buildfile towards the root of the filesystem and use the first one found
-Dproperty=value set property to value 
例子
ant
使用当前目录下的build.xml运行Ant，执行缺省的target。
ant -buildfile test.xml
使用当前目录下的test.xml运行Ant，执行缺省的target。
ant -buildfile test.xml dist
使用当前目录下的test.xml运行Ant，执行一个叫做dist的target。
ant -buildfile test.xml -Dbuild=build/classes dist
使用当前目录下的test.xml运行Ant，执行一个叫做dist的target，并设定build属性的值为build/classes。

3 编写build.xml 

Ant的buildfile是用XML写的。每个buildfile含有一个project。

buildfile中每个task元素可以有一个id属性，可以用这个id值引用指定的任务。这个值必须是唯一的。（详情请参考下面的Task小节）

3.1 Projects

project有下面的属性：
Attribute Description Required
name 项目名称. No
default 当没有指定target时使用的缺省target Yes
basedir 用于计算所有其他路径的基路径。该属性可以被basedir property覆盖。当覆盖时，该属性被忽略。如果属性和basedir property都没有设定，就使用buildfile文件的父目录。 No
项目的描述以一个顶级的<description>元素的形式出现（参看description小节）。

一个项目可以定义一个或多个target。一个target是一系列你想要执行的。执行Ant时，你可以选择执行那个target。当没有给定target时，使用project的default属性所确定的target。

3.2 Targets

一个target可以依赖于其他的target。例如，你可能会有一个target用于编译程序，一个target用于生成可执行文件。你在生成可执行文件之前必须先编译通过，所以生成可执行文件的target依赖于编译target。Ant会处理这种依赖关系。

然而，应当注意到，Ant的depends属性只指定了target应该被执行的顺序－如果被依赖的target无法运行，这种depends对于指定了依赖关系的target就没有影响。

Ant会依照depends属性中target出现的顺序（从左到右）依次执行每个target。然而，要记住的是只要某个target依赖于一个target，后者就会被先执行。
<target name="A"/>
<target name="B" depends="A"/>
<target name="C" depends="B"/>
<target name="D" depends="C,B,A"/>
假定我们要执行target D。从它的依赖属性来看，你可能认为先执行C，然后B，最后A被执行。错了，C依赖于B，B依赖于A，所以先执行A，然后B，然后C，最后D被执行。

一个target只能被执行一次，即时有多个target依赖于它（看上面的例子）。

如果（或如果不）某些属性被设定，才执行某个target。这样，允许根据系统的状态（java version, OS, 命令行属性定义等等）来更好地控制build的过程。要想让一个target这样做，你就应该在target元素中，加入if（或unless）属性，带上target因该有所判断的属性。例如：
<target name="build-module-A" if="module-A-present"/>
<target name="build-own-fake-module-A" unless="module-A-present"/>
如果没有if或unless属性，target总会被执行。

可选的description属性可用来提供关于target的一行描述，这些描述可由-projecthelp命令行选项输出。

将你的tstamp task在一个所谓的初始化target是很好的做法，其他的target依赖这个初始化target。要确保初始化target是出现在其他target依赖表中的第一个target。在本手册中大多数的初始化target的名字是"init"。

target有下面的属性：
Attribute Description Required
name target的名字 Yes
depends 用逗号分隔的target的名字列表，也就是依赖表。 No
if 执行target所需要设定的属性名。 No
unless 执行target需要清除设定的属性名。 No
description 关于target功能的简短描述。 No

3.3 Tasks

一个task是一段可执行的代码。

一个task可以有多个属性（如果你愿意的话，可以将其称之为变量）。属性只可能包含对property的引用。这些引用会在task执行前被解析。

下面是Task的一般构造形式：
<name attribute1="value1" attribute2="value2" ... />
这里name是task的名字，attributeN是属性名，valueN是属性值。

有一套内置的（built-in）task，以及一些可选task，但你也可以编写自己的task。

所有的task都有一个task名字属性。Ant用属性值来产生日志信息。

可以给task赋一个id属性：
<taskname id="taskID" ... />
这里taskname是task的名字，而taskID是这个task的唯一标识符。通过这个标识符，你可以在脚本中引用相应的task。例如，在脚本中你可以这样：
<script ... >
task1.setFoo("bar");
</script>
设定某个task实例的foo属性。在另一个task中（用java编写），你可以利用下面的语句存取相应的实例。
project.getReference("task1").
注意1：如果task1还没有运行，就不会被生效（例如：不设定属性），如果你在随后配置它，你所作的一切都会被覆盖。

注意2：未来的Ant版本可能不会兼容这里所提的属性，因为很有可能根本没有task实例，只有proxies。

3.4 Properties

一个project可以有很多的properties。可以在buildfile中用property task来设定，或在Ant之外设定。一个property有一个名字和一个值。property可用于task的属性值。这是通过将属性名放在"${"和"}"之间并放在属性值的位置来实现的。例如如果有一个property builddir的值是"build"，这个property就可用于属性值：${builddir}/classes。这个值就可被解析为build/classes。

内置属性

如果你使用了<property> task 定义了所有的系统属性，Ant允许你使用这些属性。例如，${os.name}对应操作系统的名字。

要想得到系统属性的列表可参考the Javadoc of System.getProperties。

除了Java的系统属性，Ant还定义了一些自己的内置属性：
basedir project基目录的绝对路径 (与<project>的basedir属性一样)。
ant.file buildfile的绝对路径。
ant.version Ant的版本。
ant.project.name 当前执行的project的名字；由<project>的name属性设定.
ant.java.version Ant检测到的JVM的版本；目前的值有"1.1", "1.2", "1.3" and "1.4".

例子
<project name="MyProject" default="dist" basedir="."> 

<!-- set global properties for this build -->
<property name="src" value="."/>
<property name="build" value="build"/>
<property name="dist" value="dist"/> 

<target name="init">
<!-- Create the time stamp -->
<tstamp/>
<!-- Create the build directory structure used by compile -->
<mkdir dir="${build}"/>
</target>

<target name="compile" depends="init">
<!-- Compile the java code from ${src} into ${build} -->
<javac srcdir="${src}" destdir="${build}"/>
</target>

<target name="dist" depends="compile">
<!-- Create the distribution directory -->
<mkdir dir="${dist}/lib"/>
<!-- Put everything in ${build} into the MyProject-${DSTAMP}.jar file -->
<jar jarfile="${dist}/lib/MyProject-${DSTAMP}.jar" basedir="${build}"/>
</target>

<target name="clean">
<!-- Delete the ${build} and ${dist} directory trees -->
<delete dir="${build}"/>
<delete dir="${dist}"/>
</target>

</project>

3.5 Path-like Structures

你可以用":"和";"作为分隔符，指定类似PATH和CLASSPATH的引用。Ant会把分隔符转换为当前系统所用的分隔符。

当需要指定类似路径的值时，可以使用嵌套元素。一般的形式是
<classpath>
<pathelement path="${classpath}"/>
<pathelement location="lib/helper.jar"/>
</classpath>
location属性指定了相对于project基目录的一个文件和目录，而path属性接受逗号或分号分隔的一个位置列表。path属性一般用作预定义的路径－－其他情况下，应该用多个location属性。

为简洁起见，classpath标签支持自己的path和location属性。所以：
<classpath>
<pathelement path="${classpath}"/>
</classpath>
可以被简写作：
<classpath path="${classpath}"/>
也可通过<fileset>元素指定路径。构成一个fileset的多个文件加入path-like structure的顺序是未定的。
<classpath>
<pathelement path="${classpath}"/>
<fileset dir="lib">
<include name="**/*.jar"/>
</fileset>
<pathelement location="classes"/>
</classpath>
上面的例子构造了一个路径值包括：${classpath}的路径，跟着lib目录下的所有jar文件，接着是classes目录。

如果你想在多个task中使用相同的path-like structure，你可以用<path>元素定义他们（与target同级），然后通过id属性引用－－参考Referencs例子。

path-like structure可能包括对另一个path-like structurede的引用（通过嵌套<path>元素）：
<path id="base.path">
<pathelement path="${classpath}"/>
<fileset dir="lib">
<include name="**/*.jar"/>
</fileset>
<pathelement location="classes"/>
</path>
<path id="tests.path">
<path refid="base.path"/>
<pathelement location="testclasses"/>
</path>
前面所提的关于<classpath>的简洁写法对于<path>也是有效的，如：
<path id="tests.path">
  <path refid="base.path"/>
<pathelement location="testclasses"/>
</path>
可写成：
<path id="base.path" path="${classpath}"/>
命令行变量

有些task可接受参数，并将其传递给另一个进程。为了能在变量中包含空格字符，可使用嵌套的arg元素。
Attribute Description Required
value 一个命令行变量；可包含空格字符。只能用一个
line 空格分隔的命令行变量列表。
file 作为命令行变量的文件名；会被文件的绝对名替代。
path 一个作为单个命令行变量的path-like的字符串；或作为分隔符，Ant会将其转变为特定平台的分隔符。

例子
<arg value="-l -a"/>
是一个含有空格的单个的命令行变量。
<arg line="-l -a"/>
是两个空格分隔的命令行变量。
<arg path="/dir;/dir2:/dir3"/>
是一个命令行变量，其值在DOS系统上为/dir;/dir2;/dir3；在Unix系统上为/dir:/dir2:/dir3 。

3.6 References

buildfile元素的id属性可用来引用这些元素。如果你需要一遍遍的复制相同的XML代码块，这一属性就很有用－－如多次使用<classpath>结构。

下面的例子：
<project ... >
<target ... >    
<rmic ...>      
<classpath>        
<pathelement location="lib/"/>        
<pathelement path="${java.class.path}/"/>        
<pathelement path="${additional.path}"/>      
</classpath>    
</rmic>  
</target>
<target ... >
<javac ...>
<classpath>
<pathelement location="lib/"/>
<pathelement path="${java.class.path}/"/>
<pathelement path="${additional.path}"/>
</classpath>
</javac>
</target>
</project>
可以写成如下形式：
<project ... > 
<path id="project.class.path">  
<pathelement location="lib/"/>
<pathelement path="${java.class.path}/"/>   
<pathelement path="${additional.path}"/> 
</path>
<target ... >
<rmic ...>
<classpath refid="project.class.path"/>
</rmic>
</target>
<target ... > 
<javac ...>
<classpath refid="project.class.path"/>
</javac>
</target>
</project>
所有使用PatternSets, FileSets 或 path-like structures嵌套元素的task也接受这种类型的引用。
================================================================
**Ant****全攻略（续）－－－Ant中的内置任务**

4.1 File（Directory）类
4.1.1 Mkdir
n 创建一个目录，如果他的父目录不存在，也会被同时创建。
n 例子：
<mkdir dir="build/classes"/>
n 说明：如果build不存在，也会被同时创建
4.1.2 Copy
n 拷贝一个（组）文件、目录
n 例子：
1. 拷贝单个的文件：
<copy file="myfile.txt" tofile="mycopy.txt"/>
2. 拷贝单个的文件到指定目录下
<copy file="myfile.txt" todir="../some/other/dir"/>
3. 拷贝一个目录到另外一个目录下
  <copy todir="../new/dir">
    <fileset dir="src_dir"/>
  </copy>
4. 拷贝一批文件到指定目录下
  <copy todir="../dest/dir">
    <fileset dir="src_dir">
      <exclude name="**/*.java"/>
    </fileset>
  </copy>

  <copy todir="../dest/dir">
    <fileset dir="src_dir" excludes="**/*.java"/>
  </copy>
5. 拷贝一批文件到指定目录下，将文件名后增加。Bak后缀
  <copy todir="../backup/dir">
    <fileset dir="src_dir"/>
    <mapper type="glob" from="*" to="*.bak"/>
  </copy>
6. 拷贝一组文件到指定目录下，替换其中的@标签@内容
  <copy todir="../backup/dir">
    <fileset dir="src_dir"/>
    <filterset>
      <filter token="TITLE" value="Foo Bar"/>
    </filterset>
  </copy>
4.1.3 Delete
n 删除一个（组）文件或者目录
n 例子
1. 删除一个文件
<delete file="/lib/ant.jar"/>
2. 删除指定目录及其子目录
  <delete dir="lib"/>
3. 删除指定的一组文件
  <delete>
    <fileset dir="." includes="**/*.bak"/>
  </delete>
4. 删除指定目录及其子目录，包括他自己
  <delete includeEmptyDirs="true">
    <fileset dir="build"/>
  </delete>
4.1.4 Move
n 移动或重命名一个（组）文件、目录
n 例子：
1. 移动或重命名一个文件
<move file="file.orig" tofile="file.moved"/>
2. 移动或重命名一个文件到另一个文件夹下面
<move file="file.orig" todir="dir/to/move/to"/>
3. 将一个目录移到另外一个目录下
<move todir="new/dir/to/move/to">
<fileset dir="src/dir"/>
</move>
4. 将一组文件移动到另外的目录下
<move todir="some/new/dir">
<fileset dir="my/src/dir">
<include name="**/*.jar"/>
<exclude name="**/ant.jar"/>
</fileset>
</move>
5. 移动文件过程中增加。Bak后缀
<move todir="my/src/dir">
<fileset dir="my/src/dir">
<exclude name="**/*.bak"/>
</fileset>
<mapper type="glob" from="*" to="*.bak"/>
</move>

================================================================
**Ant****全攻略  --  Java相关任务**
4.2.1 Javac
n 编译java原代码
n 例子
1. <javac srcdir="${src}"
         destdir="${build}"
         classpath="xyz.jar"
         debug="on"
  />
编译${src}目录及其子目录下的所有。Java文件，。Class文件将放在$｛build｝指定的目录下，classpath表示需要用到的类文件或者目录，debug设置为on表示输出debug信息
2. <javac srcdir="${src}:${src2}"
         destdir="${build}"
         includes="mypackage/p1/**,mypackage/p2/**"
         excludes="mypackage/p1/testpackage/**"
         classpath="xyz.jar"
         debug="on"
  />
编译${src}和${src2}目录及其子目录下的所有。Java文件，但是package/p1/**,mypackage/p2/**将被编译，而mypackage/p1/testpackage/**将不会被编译。Class文件将放在$｛build｝指定的目录下，classpath表示需要用到的类文件或者目录，debug设置为on表示输出debug信息
3. <property name="classpath" value=".;./xml-apis.jar;../lib/xbean.jar;./easypo.jar"/>

<javac srcdir="${src}"
         destdir="${src}"
classpath="${classpath}"
debug="on"
/>
路径是在property中定义的
4.2.2 java
n 执行指定的java类
n 例子：
1. <java classname="test.Main">
         <classpath>
           <pathelement location="dist/test.jar"/>
           <pathelement path="${java.class.path}"/>
         </classpath>
       </java>
classname中指定要执行的类，classpath设定要使用的环境变量
2. <path id="project.class.path">
    <pathelement location="lib/"/>
    <pathelement path="${java.class.path}/"/>
    <pathelement path="${additional.path}"/>
  </path>

  <target ... >
    <rmic ...>
      <classpath refid="project.class.path"/>
    </rmic>
  </target>

================================================================

**Ant****全攻略  --     通过JDBC执行SQL语句**

4.5 执行SQL语句

n 通过jdbc执行SQL语句
n 例子：
1. <sql
    driver="org.gjt.mm.mysql.Driver"
    url="jdbc:mysql://localhost:3306/mydb"
    userid="root"
    password="root"
    src="data.sql"
/>
2. <sql
    driver="org.database.jdbcDriver"
    url="jdbc:database-url"
    userid="sa"
    password="pass"
    src="data.sql"
    rdbms="oracle"
    version="8.1."
    >
</sql>
只有在oracle、版本是8.1的时候才执行

================================================================
**Ant ****全攻略之－－－发送邮件**

4.6 发送邮件
n 使用SMTP服务器发送邮件
n 例子：
<mail mailhost="smtp.myisp.com" mailport="1025" subject="Test build">
  <from address="me@myisp.com"/>
  <to address="all@xyz.com"/>
  <message>The ${buildname} nightly build has completed</message>
  <fileset dir="dist">
    <includes name="**/*.zip"/>
  </fileset>
</mail>
l mailhost： SMTP服务器地址
l mailport：服务器端口
l subject：主题
l from：发送人地址
l to：接受人地址
l message：发送的消息
l fileset：设置附件


