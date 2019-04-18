# 使用maven的profile和filter插件管理配置项 - z69183787的专栏 - CSDN博客
2015年09月25日 15:04:22[OkidoGreen](https://me.csdn.net/z69183787)阅读数：808
个人分类：[项目构建-Maven](https://blog.csdn.net/z69183787/article/category/2265961)
# 1.概述
    按照项目开发的流程，项目工程需要经历开发人员本地测试环境(**dev**),进入测试阶段后有测试环境(**test**),测试无误后需要将程序发布到生产环境(**production**)下，而对于这三个不同的项目环境，应用程序要在该环境下运行，必须有不同的配置项，如开发和生产环境使用不同的数据库地址，不同的线程池参数，因此需要对不同的项目环境进行不同的配置，为了简化和规范这些参数配置，需要对其进行统一规范管理。
# 2.配置项管理
## 2.1三个环境
按照项目所处阶段，分为
开发环境(dev)
    该环境下的配置项只影响开发人员本地代码配置，在项目初期代码本地编写调试时启用，如可以设置更低的Log级别帮助开发人员查看更为详细的log信息。
测试环境(test)
    该环境配置影响整个团队的测试环境。
正式生产环境(production)
    程序最终发布后所需要的参数配置，该环境下的配置项修改将直接影响最终用户的使用和应用程序的运行。
系统的配置项统一放在src/main/filters/目录下，三个环境的配置文件分别为
filter-dev.properties
filter-test.properties
filter-production.properties
这三个文件的用法见2.3.2节介绍
## 2.2识别配置项
    开发人员在做日常需求时，需要对程序的配置项进行识别，严格禁止将变量写死代码的情况出现。下列情况下需要考虑将变量作为系统的配置项
1)  一些系统级参数、
数据库连接参数，日志级别
2)  外部依赖webservice链接地址
对于不同的环境下所依赖的外部webservice也应该根据所处环境不同依赖不同的地址，如开发环境下依赖外部系统开发环境的地址。
3)  一些业务数据
系统可能要使用一些业务数据，如对某一个岗位信息特殊处理，该岗位信息在生产开发环境数据库中的id不一致，这种情况下也需要将其作为配置项处理。
## 2.3Maven配置
   项目工程统一使用maven的profile插件定义不同的项目构建环境(dev, test, production),通过filter插件为不同环境下的配置项注入对应的参数值来实现动态配置目标。
### 2.3.1定义profile
在POM.xml中配置3个profile，对应项目所处的3个不同的环境-dev, test, production, profile的id属性即为每个环境赋予一个唯一的标示，<properties>元素的内容则是以key-value的形式出现的键值对，如我们定义了一个变量<env>，其值在不同的环境下(不同id)被赋予了不同的值(dev, production, test),要激活不同的环境打包，我们可以在命令行通过mvn package –P${profileId}来让其运行，为了开发便利，默认激活的是dev开发环境，即开发人员不需要通过命令行手动输入-p参数也能运行dev环境的打包。
    <!-- 不同的打包环境 -->
    <profiles>
       <!-- 开发环境，默认激活 -->
       <profile>
           <id>**dev**</id>
           <properties>
              <env>dev</env>
           </properties>
           <activation>
              <activeByDefault>true</activeByDefault><!--默认启用的是dev环境配置-->
           </activation>
       </profile>
       <!-- 生产环境 -->
       <profile>
           <id>production</id>
           <properties>
              <env>production</env>
           </properties>
       </profile>
       <!-- 测试环境 -->
       <profile>
           <id>test</id>
           <properties>
              <env>test</env>
           </properties>
       </profile>
    </profiles>
### 2.3.2定义filter
可以使用Maven来对项目资源进行变量替换。在资源过滤被激活的时候，Maven会扫描
资源，寻找由${}包围的Maven属性的引用。一旦它找到这些引用，它就会使用合适的值去替换它们，当需要根据目标部署平台使用不同的配置来参数化一个构建的时候，这就非常有用。
通常一个在src/main/resources目录下的.properties文件或者XML文档会包含对外部资源的引用，如需要根据目标部署平台进行不同配置的数据库或网络地址。例如，一个从数据库读取数据的系统有一个XML文档，其包含了数据库的JDBC URL以及安全凭证。如果你在开发和产品环境使用不同的数据库，使用Maven Profile来为不同的部署环境定义不同的配置。
Filter的配置(在<build>元素下添加节点)如下：
首先引入配置文件的地址，2.1节已经介绍过统一放在src/main/filters目录下，注意这个filter的配置有个filter-**${env}**.properties,这个**${env}**变量就是在父pom.xml中定义的profile的id,当通过命令行mvn package –P${profileId}时，按${env}变量就会用实际传入的值替换，从而达到针对不同环境采用不同配置文件的目的。
    <filters>
        <filter>src/main/filters/filter-${**env**}.properties</filter>
    </filters>
    配置需要被替换的资源文件地址
    <resources>
        <resource>
            <directory>src/main/resources</directory>
            <filtering>true</filtering>
        </resource>
    </resources>
如在src/main/resources/目录下的env.properties文件的内容如下
### database connection configuration
jdbc.driverClassName=${jdbc.driverClassName}
jdbc.url=${jdbc.url}
jdbc.username=${jdbc.username}
jdbc.password=${jdbc.password}
当执行完毕打包操作后，位于war包中的env.properties文件中所有被的${}都会被filters下文件中的值所替换。
