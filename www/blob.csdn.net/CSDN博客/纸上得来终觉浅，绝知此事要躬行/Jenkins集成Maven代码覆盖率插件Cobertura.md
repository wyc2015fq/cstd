# Jenkins集成Maven代码覆盖率插件Cobertura - 纸上得来终觉浅，绝知此事要躬行 - CSDN博客





2017年08月21日 13:48:40[boonya](https://me.csdn.net/boonya)阅读数：4069








Jenkins集成Maven代码覆盖率插件Cobertura可以很好的测试到没有做单元测试的代码，代码测试覆盖率也是一个衡量代码质量的因素。Maven为我们提供了Cobertura插件，可以很好地配置到pom.xml文件中集成。除了Cobertura，还有一款商业测量代码覆盖率插件Clover。

## pom.xml依赖配置



```
<build>
		<plugins>
			.....
			<!-- code cover rage -->
			<plugin>
				<groupId>org.codehaus.mojo</groupId>
				<artifactId>cobertura-maven-plugin</artifactId>
				<version>2.6</version>
				<configuration>
					<formats>
						<format>html</format>
						<format>xml</format>
					</formats>
				</configuration>
			</plugin>
		</plugins>
	</build>
```




## Jenkins Cobertura插件安装

![](https://img-blog.csdn.net/20170821133732117)

注意：安装后重启Jenkins生效。


## Jenkins构建配置Cobertura


使用命令：mvn cobertura:cobertura 后面加了-Pmetrics表示生成报告。


![](https://img-blog.csdn.net/20170821134104601)

注意：如果包含模块功能，命令使用：mvn site   请参考[http://www.mkyong.com/qa/maven-cobertura-code-coverage-example/](http://www.mkyong.com/qa/maven-cobertura-code-coverage-example/)


## Jenkins构建后 Cobertura代码测试覆盖率报告配置


这里按照提示配置就可以了：

![](https://img-blog.csdn.net/20170821134238506)


## 测试代码覆盖率报告查看

到任务主界面查看：

![](https://img-blog.csdn.net/20170821134444055)

点击左侧Coverage Report查看：

![](https://img-blog.csdn.net/20170821134620451)

## 商业的代码覆盖率插件Clover


Clover是一款来自Atlassian 优秀的商业代码覆盖率工具。


官方地址：[https://www.atlassian.com/software/clover](https://www.atlassian.com/software/clover)


下载地址：[https://www.atlassian.com/software/clover/download](https://www.atlassian.com/software/clover/download)


Jenkins Clover插件Github地址：[https://github.com/atlassian/clover-jenkins-plugin](https://github.com/atlassian/clover-jenkins-plugin)

注：使用方法类似与Cobertura。

POM中配置（使用此版本的插件，不需要licence，但是只能运行在linux下的机器上）：



```java
<build>
        <plugins>
            <plugin>
                <groupId>com.atlassian.maven.plugins</groupId>
                <artifactId>maven-clover2-plugin</artifactId>
                <version>1.0.0.nl-SNAPSHOT</version>
                <configuration>
                    <contextFilters>catch,static</contextFilters>
                    <includes>
                        <include>*.java</include>
                    </includes>
                    <includesTestSourceRoots>false</includesTestSourceRoots>
                    <generateHistorical>true</generateHistorical>
                    <encoding>GBK</encoding>
                </configuration>
            </plugin>
        </plugins>
    </build>
```




问题：如果按照以上操作没有正确生成覆盖率文件，初步判断是 jar包中的lisence没有生效。window 系统下，当官网上注册生成自己的证书，不过只能用30天，主POM中配置：



```java
<plugin>
                <groupId>com.atlassian.maven.plugins</groupId>
                <artifactId>maven-clover2-plugin</artifactId>
                <version>2.5.1</version>
                <configuration>
                    <includesTestSourceRoots>false</includesTestSourceRoots>
                    <generateHistorical>true</generateHistorical>
                    <encoding>GBK</encoding>
                    <licenseLocation>D:/clover.license</licenseLocation>
                </configuration>
            </plugin>
```



带有单元测试集的子POM配置：（跳过开发的单元测试集，我们自己的qatest就不需要添加这些代码了）



```java
<plugin>
<groupId>org.apache.maven.plugins</groupId>
<artifactId>maven-surefire-plugin</artifactId>   
        <version>2.4.2</version>   
        <configuration>   
          <skipTests>true</skipTests>   
        </configuration>   
</plugin>
```

构建命令：





```java
clover2:setup test clover2:aggregate clover2:clover -Dmaven.test.failure.ignore
```















