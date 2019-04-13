
# Gradle进阶：3:结合Jacoco进行覆盖率确认 - 知行合一 止于至善 - CSDN博客

2018年12月13日 06:52:18[liumiaocn](https://me.csdn.net/liumiaocn)阅读数：2132


![在这里插入图片描述](https://img-blog.csdnimg.cn/2018111707041592.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L2xpdW1pYW9jbg==,size_16,color_FFFFFF,t_70)
上篇文章我们了解到了如何使用Gradle结合Junit对创建的SpringBoot的应用进行单体测试，这篇文章来接着确认一些如何可视化地确认测试覆盖率。
[
](https://img-blog.csdnimg.cn/2018111707041592.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L2xpdW1pYW9jbg==,size_16,color_FFFFFF,t_70)
# 前提
[
](https://img-blog.csdnimg.cn/2018111707041592.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L2xpdW1pYW9jbg==,size_16,color_FFFFFF,t_70)Jacoco是Java Code Coverage Library的缩写，它是用于确认java应用单体测试覆盖率的有效工具，最重要的一个前提是需要有测试用例，所以本文是在前面一篇文章的基础之上的，关于如何使用gradle进行spring boot的单体测试这里不再赘述。
[
](https://img-blog.csdnimg.cn/2018111707041592.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L2xpdW1pYW9jbg==,size_16,color_FFFFFF,t_70)
# Gradle方式使用Jacoco
[
](https://img-blog.csdnimg.cn/2018111707041592.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L2xpdW1pYW9jbg==,size_16,color_FFFFFF,t_70)
## build.gradle添加
[
](https://img-blog.csdnimg.cn/2018111707041592.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L2xpdW1pYW9jbg==,size_16,color_FFFFFF,t_70)添加Jacoco的plugin
[
](https://img-blog.csdnimg.cn/2018111707041592.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L2xpdW1pYW9jbg==,size_16,color_FFFFFF,t_70)
> apply plugin: ‘jacoco’
[
](https://img-blog.csdnimg.cn/2018111707041592.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L2xpdW1pYW9jbg==,size_16,color_FFFFFF,t_70)添加使用设定
后面会给出同样方式使用maven和jacoco的方法，从直观的感受来说gradle写的更加人性化一些，从如下可以读出，jacoco的相关结果的报告是以html方式生成。当然根据需要还可以加上check.dependsOn jacocoTestReport的依赖，这样使用gradle build可以直接生成结果，较为方便。
jacocoTestReport {
reports {
xml.enabled false
html.enabled true
}
}
build.gradle
添加了上述内容的build.gradle为：
liumiaocn:springboot liumiao$ cat build.gradle 
buildscript {
	ext {
		springBootVersion = '2.1.1.RELEASE'
	}
	repositories {
		mavenCentral()
	}
	dependencies {
		classpath("org.springframework.boot:spring-boot-gradle-plugin:${springBootVersion}")
	}
}
apply plugin: 'java'
apply plugin: 'jacoco'
apply plugin: 'org.springframework.boot'
apply plugin: 'io.spring.dependency-management'
group = 'com.liumiaocn'
version = '0.0.1-SNAPSHOT'
sourceCompatibility = 1.8
repositories {
	mavenCentral()
}

dependencies {
	implementation('org.springframework.boot:spring-boot-starter-web')
        testImplementation('org.springframework.boot:spring-boot-starter-test')
}
jacocoTestReport {
    reports {
        xml.enabled false
        html.enabled true
    }
}
check.dependsOn jacocoTestReport
liumiaocn:springboot liumiao$执行命令
有多种方式可以生成jacoco的reports，这里由于依赖已经创建，可以直接使用gradle build即可，当然也可以按照顺序执行jacocoTestReport。
执行命令：gradle build
执行日志
liumiaocn:springboot liumiao$ gradle clean;
BUILD SUCCESSFUL in 0s
1 actionable task: 1 executed
liumiaocn:springboot liumiao$ gradle build
> Task :test
2018-12-13 06:42:10.567  INFO 52478 --- [       Thread-6] o.s.s.concurrent.ThreadPoolTaskExecutor  : Shutting down ExecutorService 'applicationTaskExecutor'
BUILD SUCCESSFUL in 5s
6 actionable tasks: 6 executed
liumiaocn:springboot liumiao$结果确认
上述gradle build执行的时候实际是执行了jacoco插件的jacocoTestReport任务，其生成的方式应该是html的，位置位于build/reports/jacoco:
liumiaocn:springboot liumiao$ tree build
build
├── classes
│   └── java
│       ├── main
│       │   └── com
│       │       └── liumiaocn
│       │           └── springbootdemo
│       │               └── SpringbootdemoApplication.class
│       └── test
│           └── com
│               └── liumiaocn
│                   └── springbootdemo
│                       └── SpringbootdemoApplicationTests.class
├── jacoco
│   └── test.exec
├── libs
│   └── springbootdemo-0.0.1-SNAPSHOT.jar
├── reports
│   ├── jacoco
│   │   └── test
│   │       └── html
│   │           ├── com.liumiaocn.springbootdemo
│   │           │   ├── SpringbootdemoApplication.html
│   │           │   ├── SpringbootdemoApplication.java.html
│   │           │   ├── index.html
│   │           │   └── index.source.html
│   │           ├── index.html
│   │           ├── jacoco-resources
│   │           │   ├── branchfc.gif
│   │           │   ├── branchnc.gif
│   │           │   ├── branchpc.gif
│   │           │   ├── bundle.gif
│   │           │   ├── class.gif
│   │           │   ├── down.gif
│   │           │   ├── greenbar.gif
│   │           │   ├── group.gif
│   │           │   ├── method.gif
│   │           │   ├── package.gif
│   │           │   ├── prettify.css
│   │           │   ├── prettify.js
│   │           │   ├── redbar.gif
│   │           │   ├── report.css
│   │           │   ├── report.gif
│   │           │   ├── session.gif
│   │           │   ├── sort.gif
│   │           │   ├── sort.js
│   │           │   ├── source.gif
│   │           │   └── up.gif
│   │           └── jacoco-sessions.html
│   └── tests
│       └── test
│           ├── classes
│           │   └── com.liumiaocn.springbootdemo.SpringbootdemoApplicationTests.html
│           ├── css
│           │   ├── base-style.css
│           │   └── style.css
│           ├── index.html
│           ├── js
│           │   └── report.js
│           └── packages
│               └── com.liumiaocn.springbootdemo.html
├── resources
│   └── main
│       └── application.properties
├── test-results
│   └── test
│       ├── TEST-com.liumiaocn.springbootdemo.SpringbootdemoApplicationTests.xml
│       └── binary
│           ├── output.bin
│           ├── output.bin.idx
│           └── results.bin
└── tmp
    ├── bootJar
    │   └── MANIFEST.MF
    ├── compileJava
    ├── compileTestJava
    └── expandedArchives
        └── org.jacoco.agent-0.8.1.jar_8059ed6e1ab8b88aac5d9097fad847bb
            ├── META-INF
            │   ├── MANIFEST.MF
            │   └── maven
            │       └── org.jacoco
            │           └── org.jacoco.agent
            │               ├── pom.properties
            │               └── pom.xml
            ├── about.html
            ├── jacocoagent.jar
            └── org
                └── jacoco
                    └── agent
                        └── AgentJar.class
42 directories, 48 files
liumiaocn:springboot liumiao$Jacoco的报告目录中也有一个index.html，通过它也可以一目了然地看到覆盖率的可视化的结果。
应用的整体覆盖率状况summary
![在这里插入图片描述](https://img-blog.csdnimg.cn/20181213064847269.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L2xpdW1pYW9jbg==,size_16,color_FFFFFF,t_70)
package单位的覆盖率状况summary
![在这里插入图片描述](https://img-blog.csdnimg.cn/20181213065000708.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L2xpdW1pYW9jbg==,size_16,color_FFFFFF,t_70)
class单位的覆盖率状况summary
![在这里插入图片描述](https://img-blog.csdnimg.cn/20181213065011517.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L2xpdW1pYW9jbg==,size_16,color_FFFFFF,t_70)
代码行粒度的覆盖率确认详细
![在这里插入图片描述](https://img-blog.csdnimg.cn/20181213065029572.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L2xpdW1pYW9jbg==,size_16,color_FFFFFF,t_70)
Maven方式使用Jacoco
同样是使用plugin方式，maven使用gradle的方法总结如下，可以与gradle进行一下简单的对比
Pom添加
<profiles>
		<profile>
			<id>sonar-jacoco-coverage</id>
			<activation>
				<activeByDefault>true</activeByDefault>
			</activation>
			<build>
				<pluginManagement>
					<plugins>
						<plugin>
							<groupId>org.jacoco</groupId>
							<artifactId>jacoco-maven-plugin</artifactId>
							<version>0.7.8</version>
						</plugin>
					</plugins>
				</pluginManagement>
				<plugins>
					<plugin>
						<groupId>org.jacoco</groupId>
						<artifactId>jacoco-maven-plugin</artifactId>
						<configuration>
							<append>true</append>
						</configuration>
						<executions>
							<execution>
								<id>jacoco-ut</id>
								<goals>
									<goal>prepare-agent</goal>
								</goals>
							</execution>
							<execution>
								<id>jacoco-it</id>
								<goals>
									<goal>prepare-agent-integration</goal>
								</goals>
							</execution>
							<execution>
								<id>jacoco-site</id>
								<phase>verify</phase>
								<goals>
									<goal>report</goal>
								</goals>
							</execution>
						</executions>
					</plugin>
				</plugins>
			</build>
		</profile>
	</profiles>执行命令
执行如下命令则可完成单体测试以及结合sonar对jacoco生成出来的内容进行分析与显示，会下载jacoco-maven-plugin进行实际的操作。
mvn test
详细可以参看maven+junit+spring boot+jacoco+sonarqube的简单示例，因为maven和gradle同属同类，所以使用的demo测试代码这里也尽量相同，以利于比较。
[https://blog.csdn.net/liumiaocn/article/details/83550284](https://blog.csdn.net/liumiaocn/article/details/83550284)
总结
这篇文章介绍了gradle中结合jacoco对spring boot的单体测试结果进行可视化展示，同时比较了maven的使用方式，后续进一步介绍如何与sonarqube进行集成。
参考内容
[https://www.eclemma.org/jacoco/trunk/index.html](https://www.eclemma.org/jacoco/trunk/index.html)

