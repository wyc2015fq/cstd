# Java学习笔记24：Mvn常用命令 - 自娱自乐的代码人，专注Java、c、php - CSDN博客





2012年12月19日 05:47:58[initphp](https://me.csdn.net/initphp)阅读数：1508
所属专栏：[Java学习笔记](https://blog.csdn.net/column/details/javago.html)








- mvn archetype:create ：创建 Maven 项目
- mvn compile ：编译源代码 

- mvn test-compile ：编译测试代码
- mvn test ： 运行应用程序中的单元测试 

- mvn site ： 生成项目相关信息的网站 

- mvn clean ：清除目标目录中的生成结果
- mvn package ： 依据项目生成 jar 文件
-  mvn install ：在本地 Repository 中安装 jar
-  mvn eclipse:eclipse -U：生成 Eclipse 项目文件
- 建一个 JAVA 项目 ： mvn archetype:create -DgroupId=com.happyelements -DartifactId=payment-store
- 建一个 web 项目 ： mvn archetype:create -DgroupId=com.happyelements -DartifactId=payment-store -DarchetypeArtifactId=maven-archetype-webapp




转：




Maven库：

[http://repo2.maven.org/maven2/](http://repo2.maven.org/maven2/)

Maven依赖查询：

[http://mvnrepository.com/](http://mvnrepository.com/)

Maven常用命令： 

1. 创建Maven的普通java项目： 

   mvn archetype:create 

   -DgroupId=packageName 

   -DartifactId=projectName  

2. 创建Maven的Web项目：   

    mvn archetype:create 

    -DgroupId=packageName    

    -DartifactId=webappName 

    -DarchetypeArtifactId=maven-archetype-webapp    

3. 编译源代码： mvn compile 

4. 编译测试代码：mvn test-compile    

5. 运行测试：mvn test   

6. 产生site：mvn site   

7. 打包：mvn package   

8. 在本地Repository中安装jar：mvn install 

9. 清除产生的项目：mvn clean   

10. 生成eclipse项目：mvn eclipse:eclipse  

11. 生成idea项目：mvn idea:idea  

12. 组合使用goal命令，如只打包不测试：mvn -Dtest package   

13. 编译测试的内容：mvn test-compile  

14. 只打jar包: mvn jar:jar  

15. 只测试而不编译，也不测试编译：mvn test -skipping compile -skipping test-compile 

      ( -skipping 的灵活运用，当然也可以用于其他组合命令)  

16. 清除eclipse的一些系统设置:mvn eclipse:clean  


ps： 


一般使用情况是这样，首先通过cvs或svn下载代码到本机，然后执行mvn eclipse:eclipse生成ecllipse项目文件，然后导入到eclipse就行了；修改代码后执行mvn compile或mvn test检验，也可以下载eclipse的maven插件。 


mvn -version/-v  显示版本信息 

mvn archetype:generate        创建mvn项目 

mvn archetype:create -DgroupId=com.oreilly -DartifactId=my-app   创建mvn项目 


mvn package            生成target目录，编译、测试代码，生成测试报告，生成jar/war文件 

mvn jetty:run            运行项目于jetty上, 

mvn compile                    编译 

mvn test                    编译并测试 

mvn clean                    清空生成的文件 

mvn site                    生成项目相关信息的网站 

mvn -Dwtpversion=1.0 eclipse:eclipse        生成Wtp插件的Web项目 

mvn -Dwtpversion=1.0 eclipse:clean        清除Eclipse项目的配置信息(Web项目) 

mvn eclipse:eclipse                将项目转化为Eclipse项目 


在应用程序用使用多个存储库 

<repositories>    

    <repository>      

        <id>Ibiblio</id>      

        <name>Ibiblio</name>      

        <url>http://www.ibiblio.org/maven/</url>    

    </repository>    

    <repository>      

        <id>PlanetMirror</id>      

        <name>Planet Mirror</name>      

        <url>http://public.planetmirror.com/pub/maven/</url>    

    </repository>  

</repositories> 



mvn deploy:deploy-file -DgroupId=com -DartifactId=client -Dversion=0.1.0 -Dpackaging=jar -Dfile=d:\client-0.1.0.jar -DrepositoryId=maven-repository-inner -Durl=ftp://xxxxxxx/opt/maven/repository/ 



发布第三方Jar到本地库中： 


mvn install:install-file -DgroupId=com -DartifactId=client -Dversion=0.1.0 -Dpackaging=jar -Dfile=d:\client-0.1.0.jar 



-DdownloadSources=true 


-DdownloadJavadocs=true 


mvn -e            显示详细错误 信息. 


mvn validate        验证工程是否正确，所有需要的资源是否可用。 

mvn test-compile    编译项目测试代码。 。 

mvn integration-test     在集成测试可以运行的环境中处理和发布包。 

mvn verify        运行任何检查，验证包是否有效且达到质量标准。     

mvn generate-sources    产生应用需要的任何额外的源代码，如xdoclet。 


本文来自CSDN博客，转载请标明出处：http://blog.csdn.net/lifxue/archive/2009/10/14/4662902.aspx 


常用命令： 

mvn -v 显示版本 

mvn help:describe -Dplugin=help 使用 help 插件的  describe 目标来输出 Maven Help 插件的信息。 

mvn help:describe -Dplugin=help -Dfull 使用Help 插件输出完整的带有参数的目标列 

mvn help:describe -Dplugin=compiler -Dmojo=compile -Dfull 获取单个目标的信息,设置  mojo 参数和  plugin 参数。此命令列出了Compiler 插件的compile 目标的所有信息 

mvn help:describe -Dplugin=exec -Dfull 列出所有 Maven Exec 插件可用的目标 

mvn help:effective-pom 看这个“有效的 (effective)”POM，它暴露了 Maven的默认设置 


mvn archetype:create -DgroupId=org.sonatype.mavenbook.ch03 -DartifactId=simple -DpackageName=org.sonatype.mavenbook 创建Maven的普通java项目，在命令行使用Maven Archetype 插件 

mvn exec:java -Dexec.mainClass=org.sonatype.mavenbook.weather.Main Exec 插件让我们能够在不往 classpath 载入适当的依赖的情况下，运行这个程序 

mvn dependency:resolve 打印出已解决依赖的列表 

mvn dependency:tree 打印整个依赖树 


mvn install -X 想要查看完整的依赖踪迹，包含那些因为冲突或者其它原因而被拒绝引入的构件，打开 Maven 的调试标记运行 

mvn install -Dmaven.test.skip=true 给任何目标添加maven.test.skip 属性就能跳过测试 

mvn install assembly:assembly 构建装配Maven Assembly 插件是一个用来创建你应用程序特有分发包的插件 


mvn jetty:run 调用 Jetty 插件的 Run 目标在 Jetty Servlet 容器中启动 web 应用 

mvn compile 编译你的项目 

mvn clean install 删除再编译 


mvn hibernate3:hbm2ddl 使用 Hibernate3 插件构造数据库






Maven常用命令有：

1. 创建Maven的普通java项目：
** mvn archetype:create  -DgroupId=com.codeline.commons  -DartifactId=pjoName **

2. 创建Maven的Web项目：****** mvn archetype:create  -DgroupId=com.mycompany.app  -DartifactId=my-webapp  -DarchetypeArtifactId=maven-archetype-webapp**

3. 编译源代码： **mvn compile**

4. 编译测试代码：**mvn test-compile **

5. 运行测试：**mvn test**

6. 产生site：**mvn site**

7. 打包：**mvn package**

8. 在本地Repository中安装jar：**mvn install**9. 清除产生的项目：**mvn clean **

10. 生成eclipse项目：**mvn eclipse:eclipse **11. 生成idea项目：**mvn idea:idea**

12. 组合使用goal命令，如只打包不测试：**mvn -Dtest package **13. 编译测试的内容：**mvn test-compile **

14. 只打jar包: **mvn jar:jar **15. 只测试而不编译，也不测试编译：**mvn test -skipping compile -skipping test-compile **

 ( -skipping 的灵活运用，当然也可以用于其他组合命令) 

16. 清除eclipse的一些系统设置:**mvn eclipse:clean **

****

**Maven下载Jar包同时下载源文件和文档**

**mvn dependency:sourcesmvn dependency:resolve -Dclassifier=javadoc 注册到本地库**

** mvn install:install-file -DgroupId=dbpool -DartifactId=dbpool -Dversion=0.0.1-SNAPSHOT -Dpackaging=jar -Dfile=/home/.../dbpool-0.0.1-SNAPSHOT.jar**






1、<project>元素为pom.xml的根元素，以下列表列出了它所有的子元素
|parent|父项目的位置。父项目的位置由group ID，artifact ID和version指定。|
|----|----|
|modelVersion|声明pom.xml文件遵从的版本|
|groupId|全局性唯一的项目标识符。通常使用完全限定的包名来和其它项目区隔（比如，org.apache.maven）|
|artifactId|在给定gorupId内唯一的产品标识符。Artifact就是由项目输出的，或者由项目所使用的东西。比如由Maven项目输出的artifacts包括：JAR文件，war文件，ear文件等。|
|packaging|此项目输出的artifact的类型，比如jar,war,ear pom。插件能够创建他们自己的packaging，所以，这里没有列出所有可能的类型。|
|name|项目的名称|
|version|此项目输出的artifact的当前版本。|
|description|此项目的详细描述，被maven用来在需要的地方描述项目，比如web站点。而此元素可以使用CDATA，让description里可以包含HTML标签。|
|url|项目主页的URL。|
|prerequisites|描述此项目构建环境的先决条件|
|issueManagement|描述此项目的发布管理系统信息|
|ciManagement|此项目的持续集成信息|
|inceptionYear|此项目开始年份，用四位整数指定。当产生版权信息时使用|
|mailingLists|项目的邮件列表信息|
|developers|项目的开发小组|
|contributors|项目的贡献者，但不属于开发小组|
|licenses|本项目的许可。这里的许可是对本项目的许可，而不是对依赖项的许可。如果有多个许可，那么用户可以选择其中之一，而不必全部。|
|scm|指定此项目使用的源代码控制系统，如SVN等等。用于持续集成|
|organization|描述此项目所属的机构。|
|build|构建此项目所需的信息。|
|profiles|项目本地构建文件列表，如果使用，可改变默认构建过程。|
|modules|作为本项目一部分的模块（子项目）。|
|repositories|依赖项和扩展项的远程仓库。|
|pluginRepositories|构建所需插件的远程仓库。|
|dependencies|此项目的所有依赖项列表。这些依赖项在构建过程中用于构造所需的classpath。这些依赖项在构建时会自动下载。|
|reports|Deprecated。Maven现在忽略此元素。|
|reporting|此元素包含用于在Maven产生的项目站点中产生报表的插件的规格。这些报表在执行mvn site命令时运行，所有报表出现在导航条上。|
|dependencyManagement|所有项目的依赖信息继承于此。这一节中的依赖项并不马上解析。事实上，当一个继承于此的POM使用groupId和artifactId定义一个依赖项时，当版本信息和其他信息没有指定时，才会使用这里的依赖项定义。|
|distributionManagement|此项目的分发信息。|
|properties|在POM可以用作替代，以及用于资源过滤。|

2、parent
|artifactId|父项目的artifact标识符|
|----|----|
|groupId|父项目的group标识符|
|version|父项目的版本|
|relativePath|父项目的pom.xml文件的相对路径。默认值为../pom.xml。maven首先从当前构建项目开始查找父项目的pom文件，然后从本地仓库，最有从远程仓库。RelativePath允许你选择一个不同的位置。|

3、prerequisites 项目先决条件
|maven|构建此项目所需的maven的最低版本|
|----|----|

4、issueManagement 用于管理此项目的发布和跟踪（暂时不了解其用途）
|system|构建此项目所需的maven的最低版本|
|----|----|
|url|此项目使用的发布管理系统的URL。|
|||

5、ciManagement 
|system|持续集成系统的名称，比如hudson。|
|----|----|
|url|此项目使用的持续集成系统的URL，如果有WEB界面的话。|
|notifiers|配置用户信息和通知模式，当构建成功通知开发人员/用户，|

6、notifier 配置通知方法，当自动构建中断时，通知用户/开发人员，用于持续集成。
|Type|投递通知的机制|
|----|----|
|SendOnError|是否发送错误通知|
|SendOnFailure|是否发送失败通知|
|SendOnSuccess|是否发送成功通知|
|SendOnWarning|是否发送警告通知|
|Address|Deprecated。发送通知的地址，通常为email地址|
|Configuration|附加配置信息|

7、mailingList此项目的邮件列表。自动产生的站点将引用此信息，用于持续集成。
|Name|邮件列表的名称|
|----|----|
|Subscribe|订阅此邮件列表的email地址或连接。如果是email地址，产生文档时会自动生成[mailto:连接](mailto:%E8%BF%9E%E6%8E%A5)。|
|Unsubscribe|退订此邮件列表的email地址或连接。如果是email地址，产生文档时会自动生成[mailto:连接](mailto:%E8%BF%9E%E6%8E%A5)。|
|Post|可以投递到此邮件列表的email地址或连接。如果是email地址，产生文档时会自动生成[mailto:连接](mailto:%E8%BF%9E%E6%8E%A5)。|
|Archive|可以浏览到邮件列表存档信息的URL。|
|OtherArchives|可选的替代URLs，用于浏览邮件存档信息列表|





