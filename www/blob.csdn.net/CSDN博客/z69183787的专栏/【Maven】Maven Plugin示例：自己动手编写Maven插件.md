# 【Maven】Maven Plugin示例：自己动手编写Maven插件 - z69183787的专栏 - CSDN博客
2016年10月31日 14:37:26[OkidoGreen](https://me.csdn.net/z69183787)阅读数：2533
# **需求：**
在Maven编译项目的时候，统计代码量，即项目中的文件数目、代码行数，包括[Java](http://lib.csdn.net/base/javaee)文件和配置文件两种；其中配置文件（sql、xml、properties）代码行数/4
 处理。
# **创建项目：**
首先确保已安装m2eclipse插件：http://eclipse.org/m2e/
eclipse - new - Maven Project，选择archetype = maven-archetype-plugin：（或用命令 mvn archetype:generate，再按提示操作）
![](https://img-my.csdn.net/uploads/201302/26/1361859590_6109.jpg)
下一步设置好坐标信息，即可创建一个maven plugin工程。
生成的pom.xml文件如下（有手工改动，参见注释）：
**[html]**[view
 plain](http://blog.csdn.net/vking_wang/article/details/8612981#)[copy](http://blog.csdn.net/vking_wang/article/details/8612981#)
[print](http://blog.csdn.net/vking_wang/article/details/8612981#)[?](http://blog.csdn.net/vking_wang/article/details/8612981#)
- <projectxmlns="http://maven.apache.org/POM/4.0.0"xmlns:xsi="http://www.w3.org/2001/XMLSchema-instance"
- xsi:schemaLocation="http://maven.apache.org/POM/4.0.0 http://maven.apache.org/xsd/maven-4.0.0.xsd">
- <modelVersion>4.0.0</modelVersion>
- 
- <groupId>com.alpha.wang</groupId>
- <artifactId>maven-statis-plugin</artifactId>
- <version>0.0.1-SNAPSHOT</version>
- <packaging>maven-plugin</packaging>
- 
- <name>alpha-statis-plugin Maven Plugin</name>
- <url>http://blog.csdn.net/vking_wang</url>
- 
- <properties>
- <project.build.sourceEncoding>UTF-8</project.build.sourceEncoding>
- </properties>
- 
- <dependencies>
- <dependency>
- <groupId>org.apache.maven</groupId>
- <artifactId>maven-plugin-api</artifactId>
- <version>2.0</version>
- </dependency>
- <!-- 否则Mojo中的org.apache.maven.model.Resource，无法解析 -->
- <dependency>
- <groupId>org.apache.maven</groupId>
- <artifactId>maven-model</artifactId>
- <version>2.2.1</version>
- </dependency>
- <dependency>
- <groupId>junit</groupId>
- <artifactId>junit</artifactId>
- <version>3.8.1</version>
- <scope>test</scope>
- </dependency>
- </dependencies>
- 
- <build>
- <plugins>
- <plugin>
- <groupId>org.apache.maven.plugins</groupId>
- <artifactId>maven-plugin-plugin</artifactId>
- <version>2.5.1</version>
- <configuration>
- <!--[WARNING]Goal prefix is specified as: 'maven-statis-plugin'. Maven currently expects it to be 'statis'.-->
- <!-- goalPrefix>maven-statis-plugin</goalPrefix-->
- <goalPrefix>statis</goalPrefix>
- </configuration>
- <executions>
- <execution>
- <id>generated-helpmojo</id>
- <goals>
- <goal>helpmojo</goal>
- </goals>
- </execution>
- </executions>
- </plugin>
- <!-- generics are not supported in -source 1.3 (use -source 5 or higher to enable generics)-->
- <plugin>
- <groupId>org.apache.maven.plugins</groupId>
- <artifactId>maven-compiler-plugin</artifactId>
- <version>2.5.1</version>
- <configuration>
- <source>1.5</source>
- <target>1.5</target>
- </configuration>
- </plugin>
- </plugins>
- </build>
- </project>
注意packing为maven-plugin；并有对maven-plugin-api的依赖。
还会自动生成一个public class MyMojo    extends AbstractMojo的源文件。
# 编写Mojo
Mojo = Maven Old [Java](http://lib.csdn.net/base/java)Object，需要继承AbstractMojo，并实现其execute方法。
上一步已经自动生成了一个Mojo，我们删掉重新创建一个：
**[java]**[view
 plain](http://blog.csdn.net/vking_wang/article/details/8612981#)[copy](http://blog.csdn.net/vking_wang/article/details/8612981#)
[print](http://blog.csdn.net/vking_wang/article/details/8612981#)[?](http://blog.csdn.net/vking_wang/article/details/8612981#)
- import org.apache.maven.model.Resource;  
- import org.apache.maven.plugin.AbstractMojo;  
- import org.apache.maven.plugin.MojoExecutionException;  
- /**
-  *
-  * @goal count
-  * 
-  * @phase process-sources
-  */
- publicclass CountMojo extends AbstractMojo  
- {  
- privatestaticfinal String[] INCLUDES_DEFAULT = {"java", "xml", "sql", "properties"};    
- privatestaticfinal String[] RATIOS_DEFAULT = {"1.0", "0.25", "0.25", "0.25"};  
- privatestaticfinal String DOT = ".";  
- /**
-      * @parameter expression="${project.basedir}"
-      * @required
-      * @readonly
-      */
- private File basedir;  
- /**
-      * @parameter expression="${project.build.sourceDirectory}"
-      * @required
-      * @readonly
-      */
- private File sourcedir;  
- /**
-      * @parameter expression="${project.build.testSourceDirectory}"
-      * @required
-      * @readonly
-      */
- private File testSourcedir;  
- /**
-      * @parameter expression="${project.resources}"
-      * @required
-      * @readonly
-      */
- private List<Resource> resources;      
- //private List<File> resources;
- /**
-      * @parameter expression="${project.testResources}"
-      * @required
-      * @readonly
-      */
- private List<Resource> testResources;  
- //private List<File> testResources;
- /**
-      * @parameter
-      */
- private String[] includes;  
- /**
-      * @parameter
-      */
- private String[] ratios;//TODO 定义为double[],从xml读取时提示java.lang.ClassCastException: [D cannot be cast to [Ljava.lang.Object;
- 
- private Map<String, Double> ratioMap = new HashMap<String, Double>();  
- privatelong realTotal;  
- privatelong fakeTotal;  
- 
- publicvoid execute() throws MojoExecutionException  
-     {  
-         initRatioMap();  
- try{  
-             countDir(sourcedir);  
-             countDir(testSourcedir);  
- 
- for(Resource res : resources){  
-                 countDir(new File(res.getDirectory()));  
-             }  
- for(Resource res : testResources){  
-                 countDir(new File(res.getDirectory()));  
-             }  
- 
-             getLog().info("TOTAL LINES:"+fakeTotal+ " ("+realTotal+")");  
- 
-         }catch (IOException e){  
- thrownew MojoExecutionException("Unable to count lines of code", e);  
-         }  
- 
-     }  
- }  
所调用的工具方法定义如下：
**[java]**[view
 plain](http://blog.csdn.net/vking_wang/article/details/8612981#)[copy](http://blog.csdn.net/vking_wang/article/details/8612981#)
[print](http://blog.csdn.net/vking_wang/article/details/8612981#)[?](http://blog.csdn.net/vking_wang/article/details/8612981#)
- privatevoid initRatioMap() throws MojoExecutionException{  
- if(includes == null || includes.length == 0){  
-         includes = INCLUDES_DEFAULT;  
-         ratios = RATIOS_DEFAULT;  
-     }  
- if(ratios == null || ratios.length == 0){  
-         ratios = new String[includes.length];  
- for(int i=0; i<includes.length; i++){  
-             ratios[i] = "1.0";  
-         }  
-     }  
- if(includes.length != ratios.length){  
- thrownew MojoExecutionException("pom.xml error: the length of includes is inconsistent with ratios!");  
-     }  
-     ratioMap.clear();  
- for(int i=0; i<includes.length; i++){  
-         ratioMap.put(includes[i].toLowerCase(), Double.parseDouble(ratios[i]));  
-     }  
- }  
- 
- privatevoid countDir(File dir) throws IOException {  
- if(! dir.exists()){  
- return;  
-     }  
-     List<File> collected = new ArrayList<File>();  
-     collectFiles(collected, dir);  
- 
- int realLine = 0;  
- int fakeLine = 0;  
- for(File file : collected){  
- int[] line =  countLine(file);  
-         realLine += line[0];  
-         fakeLine += line[1];  
-     }  
- 
-     String path = dir.getAbsolutePath().substring(basedir.getAbsolutePath().length());  
-     StringBuilder info = new StringBuilder().append(path).append(" : ").append(fakeLine).append(" ("+realLine+")")  
-             .append(" lines of code in ").append(collected.size()).append(" files");  
-     getLog().info(info.toString());       
- 
- }  
- 
- privatevoid collectFiles(List<File> collected, File file)  
- throws IOException{  
- if(file.isFile()){  
- if(isFileTypeInclude(file)){  
-             collected.add(file);  
-         }  
-     }else{  
- for(File files : file.listFiles()){  
-             collectFiles(collected, files);  
-         }  
-     }  
- }  
- 
- privateint[] countLine(File file)  
- throws IOException{  
-     BufferedReader reader = new BufferedReader(new FileReader(file));  
- int realLine = 0;  
- try{  
- while(reader.ready()){  
-             reader.readLine();  
-             realLine ++;  
-         }  
-     }finally{  
-         reader.close();  
-     }  
- int fakeLine = (int) (realLine * getRatio(file));  
-     realTotal += realLine;  
-     fakeTotal += fakeLine;  
- 
-     StringBuilder info = new StringBuilder().append(file.getName()).append("  : ").append(fakeLine).append(" ("+realLine+")")  
-             .append(" lines");  
-     getLog().debug(info.toString());  
- 
- returnnewint[]{realLine, fakeLine};  
- }  
- 
- privatedouble getRatio(File file){  
- double ratio = 1.0;  
-     String type = getFileType(file);  
- if(ratioMap.containsKey(type)){  
-         ratio = ratioMap.get(type);  
-     }         
- return ratio;  
- }  
- 
- privateboolean isFileTypeInclude(File file){  
- boolean result = false;  
-     String fileType = getFileType(file);  
- if(fileType != null && ratioMap.keySet().contains(fileType.toLowerCase())){           
-         result = true;            
-     }         
- return result;  
- }  
- 
- private String getFileType(File file){  
-     String result = null;  
-     String fname = file.getName();  
- int index = fname.lastIndexOf(DOT);  
- if(index > 0){  
-         String type = fname.substring(index+1);  
-         result = type.toLowerCase();  
-     }  
- return result;  
- }  
注意此处annotation的使用，例如@parameter expression="${project.build.sourceDirectory}"就会自动将项目源代码路径赋值给sourcedir变量。
特别注意**@goal**，每个插件可有多个目标，在用mvn命令时需要用到这个goal。
# 执行
可使用mvn clean install将自定义的这个插件安装到本地仓库。
**[plain]**[view
 plain](http://blog.csdn.net/vking_wang/article/details/8612981#)[copy](http://blog.csdn.net/vking_wang/article/details/8612981#)
[print](http://blog.csdn.net/vking_wang/article/details/8612981#)[?](http://blog.csdn.net/vking_wang/article/details/8612981#)
- mvn clean install  
然后在需要统计的项目目录下，调用如下命令即可统计代码行：
**[plain]**[view
 plain](http://blog.csdn.net/vking_wang/article/details/8612981#)[copy](http://blog.csdn.net/vking_wang/article/details/8612981#)
[print](http://blog.csdn.net/vking_wang/article/details/8612981#)[?](http://blog.csdn.net/vking_wang/article/details/8612981#)
- mvn com.alpha.wang:maven-statis-plugin:0.0.1-SNAPSHOT:count  
最后一个冒号后面，就是上面定义的@goal。
显然这个命令太长了，使用很不方便，可在settings.xml中配置如下：
**[html]**[view
 plain](http://blog.csdn.net/vking_wang/article/details/8612981#)[copy](http://blog.csdn.net/vking_wang/article/details/8612981#)
[print](http://blog.csdn.net/vking_wang/article/details/8612981#)[?](http://blog.csdn.net/vking_wang/article/details/8612981#)
- <pluginGroups>
- <pluginGroup>com.alpha.wang</pluginGroup>
- </pluginGroups>
这样上述命令可以简写为：
**[plain]**[view
 plain](http://blog.csdn.net/vking_wang/article/details/8612981#)[copy](http://blog.csdn.net/vking_wang/article/details/8612981#)
[print](http://blog.csdn.net/vking_wang/article/details/8612981#)[?](http://blog.csdn.net/vking_wang/article/details/8612981#)
- mvn statis:count  
# 提供配置点
Mojo的includes、ratios变量标记为@parameter，表示用户可在pom.xml中配置该字段；例如可在目标项目的pom.xml中增加如下内容：
**[html]**[view
 plain](http://blog.csdn.net/vking_wang/article/details/8612981#)[copy](http://blog.csdn.net/vking_wang/article/details/8612981#)
[print](http://blog.csdn.net/vking_wang/article/details/8612981#)[?](http://blog.csdn.net/vking_wang/article/details/8612981#)
- <build>
- <plugins>
- <plugin>
- <groupId>com.alpha.wang</groupId>
- <artifactId>maven-statis-plugin</artifactId>
- <version>0.0.1-SNAPSHOT</version>
- <configuration>
- <includes>
- <!--include>java</include-->
- <include>properties</include>
- </includes>
- <ratios>
- <ratio>1.5</ratio>
- </ratios>
- </configuration>
- </plugin>
再在该项目上运行mvn statis:count时就只会统计properties文件了，并将代码量*1.5。
D:\>mvn statis:count
[INFO] Scanning for projects...
[INFO] Searching repository for plugin with prefix: 'statis'.
[INFO] ------------------------------------------------------------------------
[INFO] Building com.。。。
[INFO]    task-segment: [statis:count]
[INFO] ------------------------------------------------------------------------
[INFO] [statis:count]
**[INFO] \src\main\java : 0 (0) lines of code in 0 files[INFO] \src\main\resources : 18 (12) lines of code in 1 files[INFO] TOTAL LINES:18 (12)**
[INFO] ------------------------------------------------------------------------
[INFO] BUILD SUCCESSFUL
[INFO] ------------------------------------------------------------------------
[INFO] Total time: < 1 second
[INFO] Finished at: Tue Feb 26 17:11:22 CST 2013
[INFO] Final Memory: 6M/215M
[INFO] ------------------------------------------------------------------------
# 绑定插件
那么每次都必须手工执行命令来统计代码量吗？我们可以绑定插件到生命周期的某个阶段，例如install阶段，那么只要每次install该项目的时候，就会自动统计代码量。
修改目标项目的pom文件，注意**<phase>、<goals>**标签：
**[html]**[view
 plain](http://blog.csdn.net/vking_wang/article/details/8612981#)[copy](http://blog.csdn.net/vking_wang/article/details/8612981#)
[print](http://blog.csdn.net/vking_wang/article/details/8612981#)[?](http://blog.csdn.net/vking_wang/article/details/8612981#)
- <build>
- <plugins>
- <plugin>
- <groupId>com.alpha.wang</groupId>
- <artifactId>maven-statis-plugin</artifactId>
- <version>0.0.1-SNAPSHOT</version>
- <configuration>
- <includes>
- <include>java</include>
- <include>properties</include>
- </includes>
- <ratios>
- <ratio>1.0</ratio>
- <ratio>0.5</ratio>
- </ratios>
- </configuration>
- <executions>
- <execution>
- <id>count line number</id>
- <phase>install</phase>
- <goals>
- <goal>count</goal>
- </goals>
- </execution>
- </executions>
- </plugin>
- 
