
# Spring基础：快速入门spring boot（10）：spring boot + sonarqube +jacoco - 知行合一 止于至善 - CSDN博客

2018年11月04日 05:41:35[liumiaocn](https://me.csdn.net/liumiaocn)阅读数：2239所属专栏：[spring快速入门](https://blog.csdn.net/column/details/13907.html)



![Spring基础：快速入门spring boot（10）：spring boot + sonarqube +jacoco](https://img-blog.csdn.net/20161203082310637)
上篇文章我们了解到了如何使用SonarQube对创建的SpringBoot的应用进行分析，这篇文章来接着确认一些如何可视化地确认测试覆盖率。
[
](https://img-blog.csdn.net/20161203082310637)
# SpringBootTest
[
](https://img-blog.csdn.net/20161203082310637)需要测试覆盖率，自然，在此之前需要有测试用例，在前面的例子中已经简单讲述了在SpringBoot应用中进行测试的方法。
[
](https://img-blog.csdn.net/20161203082310637)
# Jacoco
[
](https://img-blog.csdn.net/20161203082310637)Jacoco是Java Code Coverage Library的缩写，详细的原理这里不再展开，本文重点讲述一下如何使用，这里在pom文件中添加如下内容
[
](https://img-blog.csdn.net/20161203082310637)`<profiles>
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
	</profiles>`[
](https://img-blog.csdn.net/20161203082310637)
# 执行命令
[
](https://img-blog.csdn.net/20161203082310637)执行如下命令则可完成单体测试以及结合sonar对jacoco生成出来的内容进行分析与显示，当然，会下载jacoco-maven-plugin进行实际的操作。
[
](https://img-blog.csdn.net/20161203082310637)
> Step 1: mvn test
[
](https://img-blog.csdn.net/20161203082310637)
> Step 2: mvn sonar:sonar -Dsonar.host.url=http://localhost:32003
[
](https://img-blog.csdn.net/20161203082310637)
# 结果确认
[
](https://img-blog.csdn.net/20161203082310637)可以看到demo的springbootdemo应用已经变成橙色了
![在这里插入图片描述](https://img-blog.csdnimg.cn/20181102134636678.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L2xpdW1pYW9jbg==,size_16,color_FFFFFF,t_70)
# 测试覆盖率和测试用例
这次执行的结果中已经看到了Coverage的内容，20%的测试覆盖率，1个测试用例
![在这里插入图片描述](https://img-blog.csdnimg.cn/20181102134823320.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L2xpdW1pYW9jbg==,size_16,color_FFFFFF,t_70)
# 测试覆盖信息详细
![在这里插入图片描述](https://img-blog.csdnimg.cn/2018110213494359.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L2xpdW1pYW9jbg==,size_16,color_FFFFFF,t_70)
# 参考内容
[https://www.eclemma.org/jacoco/trunk/index.html](https://www.eclemma.org/jacoco/trunk/index.html)

