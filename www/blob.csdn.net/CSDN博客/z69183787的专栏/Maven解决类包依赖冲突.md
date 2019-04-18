# Maven解决类包依赖冲突 - z69183787的专栏 - CSDN博客
2015年06月27日 21:55:26[OkidoGreen](https://me.csdn.net/z69183787)阅读数：6647
使用maven最烦人的可能就是类包之间的版本冲突引发的问题了，类包冲突的一个很大的原因即产类包之间的间接依赖引起的。每个显式声明的类包都会依赖于一些其它的隐式类包，这些隐式的类包会被maven间接引入进来，因而可能造成一个我们不想要的类包的载入，严重的甚至会引起类包之间的冲突。 
要解决这个问题，首先就是要查看pom.xml显式和隐式的依赖类包，然后通过这个类包树找出我们不想要的依赖类包，手工将其排除在外就可以了。 
下面，通过一个例子来说明： 
我的项目使用testng进行测试，使用了untilis，由于unitils的类包会隐式依赖于junit，这是我不想看到的，下面的目的就是找出junit会谁隐式载入了，然后exculte掉它。 
通过idea的maven依赖分析将不需要的依赖exclude掉 
打开maven的pom.xml，在某个<dependency>中通过右键菜单：maven->show dependency 打开分析的图形化页面，如下所示： 
![](http://dl.iteye.com/upload/attachment/0069/3002/ab0bcfc2-bf70-30a0-a913-b12e61773fd9.jpg)
通过菜单的exclude即解决这个间接依赖。 
通过这个依赖树，我们还可以看到Junit还通过“unitils-spring”的依赖间接载入了，如下所示： 
![](http://dl.iteye.com/upload/attachment/0069/3008/e669a29b-acac-3cf7-aa13-c5b3237a102a.jpg)
从上面的依赖树中，我们可以看出junit通过unitils-spring的unitils-database间接引入了，由于我的项目都不需要数据库的测试，因此，可以把unitils-database项整个exclude掉。 
如果有jar包冲突，idea会用红色标明
![Intellij idea 的maven依赖图 - ljhzzyx - 怀念外婆屋后的柚子树](http://img0.ph.126.net/WZRjQVhdh_iVRVMqA3V0fg==/6597848220890070351.jpg)
 鼠标点击冲突状态的图标，会有红色虚线标明是跟哪个jar包冲突了，在其上右键Exclude则可以快速的排除依赖。可见，idea的jar包查看和冲突排除比eclipse 插件要好，但idea下没有找到明显输入名字查找对应jar包的功能，只找到这个，然后输入字母就能够筛选了
![Intellij idea 的maven依赖图 - ljhzzyx - 怀念外婆屋后的柚子树](http://img0.ph.126.net/nT759oxOjWsnp3w-Ef3tcQ==/6597528263006380980.jpg)
下面，是处理完成后的pom.xml关键片断： 
Xml代码  ![收藏代码](http://stamen.iteye.com/images/icon_star.png)
- <dependency>
- <groupId>org.unitils</groupId>
- <artifactId>unitils-testng</artifactId>
- <version>${unitils.version}</version>
- <scope>test</scope>
- <exclusions>
- <exclusion>
- <artifactId>junit</artifactId>
- <groupId>junit</groupId>
- </exclusion>
- </exclusions>
- 
- </dependency>
- 
- <dependency>
- <groupId>org.unitils</groupId>
- <artifactId>unitils-spring</artifactId>
- <version>${unitils.version}</version>
- <scope>test</scope>
- <exclusions>
- <exclusion>
- <artifactId>unitils-database</artifactId>
- <groupId>org.unitils</groupId>
- </exclusion>
- </exclusions>
- </dependency>
- </dependencies>
这样，被间接隐式引入的junit就被我们exclude在外了。 
有时通过idea的依赖分析工具产生的树不够全，这时就需要使用mvn dependency:tree来查看依赖树了。 
通过mvn dependency:tree 查看依赖树 
引用
mvn dependency:tree
以下是使用这个工具产生的依赖树： 
引用
E:\01workspace\chenxh\09research\rop\rop>mvn dependency:tree 
[WARNING] 
[WARNING] Some problems were encountered while building the effective settings 
[WARNING] 'pluginRepositories.pluginRepository.id' must not be 'local', this identifier is reserved for the local re 
tory, using it for other repositories will corrupt your repository metadata. @ C:\Users\Administrator\.m2\settings.x 
[WARNING] 
[INFO] Scanning for projects... 
[INFO] 
[INFO] ------------------------------------------------------------------------ 
[INFO] Building rop 1.0-SNAPSHOT 
[INFO] ------------------------------------------------------------------------ 
[INFO] 
[INFO] --- maven-dependency-plugin:2.1:tree (default-cli) @ rop --- 
[INFO] com.rop:rop:jar:1.0-SNAPSHOT 
[INFO] +- javax.validation:validation-api:jar:1.0.0.GA:compile 
[INFO] +- org.hibernate:hibernate-validator:jar:4.2.0.Final:compile 
[INFO] +- org.codehaus.jackson:jackson-core-asl:jar:1.9.5:compile 
[INFO] +- org.codehaus.jackson:jackson-mapper-asl:jar:1.9.5:compile 
[INFO] +- org.codehaus.jackson:jackson-jaxrs:jar:1.9.5:compile 
[INFO] +- org.codehaus.jackson:jackson-xc:jar:1.9.5:compile 
[INFO] +- com.fasterxml.jackson.dataformat:jackson-dataformat-xml:jar:2.0.0-RC2:compile 
[INFO] |  +- com.fasterxml.jackson.core:jackson-core:jar:2.0.0-RC2:compile 
[INFO] |  +- com.fasterxml.jackson.core:jackson-annotations:jar:2.0.0-RC2:compile 
[INFO] |  +- com.fasterxml.jackson.core:jackson-databind:jar:2.0.0-RC2:compile 
[INFO] |  +- com.fasterxml.jackson.module:jackson-module-jaxb-annotations:jar:2.0.0-RC2:compile 
[INFO] |  \- org.codehaus.woodstox:stax2-api:jar:3.1.1:compile 
[INFO] |     \- javax.xml.stream:stax-api:jar:1.0-2:compile 
[INFO] +- org.slf4j:slf4j-api:jar:1.6.1:compile 
[INFO] +- org.slf4j:slf4j-log4j12:jar:1.6.1:compile 
[INFO] +- log4j:log4j:jar:1.2.16:compile 
[INFO] +- org.springframework:spring-core:jar:3.1.1.RELEASE:compile 
[INFO] |  +- org.springframework:spring-asm:jar:3.1.1.RELEASE:compile 
[INFO] |  \- commons-logging:commons-logging:jar:1.1.1:compile 
[INFO] +- org.springframework:spring-context:jar:3.1.1.RELEASE:compile 
[INFO] |  +- org.springframework:spring-aop:jar:3.1.1.RELEASE:compile 
[INFO] |  +- org.springframework:spring-beans:jar:3.1.1.RELEASE:compile 
[INFO] |  \- org.springframework:spring-expression:jar:3.1.1.RELEASE:compile 
[INFO] +- org.springframework:spring-context-support:jar:3.1.1.RELEASE:compile 
[INFO] +- org.springframework:spring-web:jar:3.1.1.RELEASE:compile 
[INFO] |  \- aopalliance:aopalliance:jar:1.0:compile 
[INFO] +- org.springframework:spring-test:jar:3.1.1.RELEASE:compile 
[INFO] +- org.springframework:spring-webmvc:jar:3.1.1.RELEASE:compile 
[INFO] +- org.testng:testng:jar:6.3:test 
[INFO] |  +- org.beanshell:bsh:jar:2.0b4:test 
[INFO] |  +- com.beust:jcommander:jar:1.12:test 
[INFO] |  \- org.yaml:snakeyaml:jar:1.6:test 
[INFO] +- org.mockito:mockito-all:jar:1.8.5:test 
[INFO] +- javax.servlet:servlet-api:jar:2.5:provided 
[INFO] +- org.unitils:unitils-core:jar:3.3:test 
[INFO] |  +- commons-lang:commons-lang:jar:2.3:test 
[INFO] |  +- commons-collections:commons-collections:jar:3.2:test 
[INFO] |  \- ognl:ognl:jar:2.6.9:test 
[INFO] +- org.unitils:unitils-testng:jar:3.3:test 
[INFO] |  \- org.testng:testng:jar:jdk15:5.8:test 
[INFO] |     \- junit:junit:jar:3.8.1:test 
[INFO] \- org.unitils:unitils-spring:jar:3.3:test 
[INFO]    +- org.springframework:spring-tx:jar:2.5.2:test 
[INFO]    \- org.unitils:unitils-database:jar:3.3:test 
[INFO]       +- org.unitils:unitils-dbmaintainer:jar:3.3:test 
[INFO]       |  +- org.hibernate:hibernate:jar:3.2.5.ga:test 
[INFO]       |  |  +- net.sf.ehcache:ehcache:jar:1.2.3:test 
[INFO]       |  |  +- asm:asm-attrs:jar:1.5.3:test 
[INFO]       |  |  +- dom4j:dom4j:jar:1.6.1:test 
[INFO]       |  |  +- antlr:antlr:jar:2.7.6:test 
[INFO]       |  |  +- cglib:cglib:jar:2.1_3:test 
[INFO]       |  |  \- asm:asm:jar:1.5.3:test 
[INFO]       |  \- org.dbunit:dbunit:jar:2.2.2:test 
[INFO]       |     +- junit-addons:junit-addons:jar:1.4:test 
[INFO]       |     |  +- xerces:xercesImpl:jar:2.6.2:test 
[INFO]       |     |  \- xerces:xmlParserAPIs:jar:2.6.2:test 
[INFO]       |     +- poi:poi:jar:2.5.1-final-20040804:test 
[INFO]       |     \- org.slf4j:slf4j-nop:jar:1.4.3:test 
[INFO]       +- commons-dbcp:commons-dbcp:jar:1.2.2:test 
[INFO]       |  \- commons-pool:commons-pool:jar:1.3:test 
[INFO]       \- org.springframework:spring-jdbc:jar:2.5.2:test 
[INFO] ------------------------------------------------------------------------ 
[INFO] BUILD SUCCESS 
[INFO] ------------------------------------------------------------------------ 
[INFO] Total time: 8.250s 
[INFO] Finished at: Fri Jun 08 09:08:09 CST 2012 
[INFO] Final Memory: 7M/245M 
[INFO] ------------------------------------------------------------------------ 
我原来一个使用idea分析不出的隐式依赖就是通用mvn dependency:tree找到的。
