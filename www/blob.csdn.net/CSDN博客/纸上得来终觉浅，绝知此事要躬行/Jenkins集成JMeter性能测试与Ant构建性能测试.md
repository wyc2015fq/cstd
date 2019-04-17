# Jenkins集成JMeter性能测试与Ant构建性能测试 - 纸上得来终觉浅，绝知此事要躬行 - CSDN博客





2017年08月22日 13:32:57[boonya](https://me.csdn.net/boonya)阅读数：339








Jenkins集成JMeter性能测试，可以模拟做系统性能测试。这里通过Ant构建 和maven插件来实现。Jenkins任务设置建议设为单个的任务运行一个指标，所以JMeter我们在新建的Jenkins任务中进行测试，避免与构建发布等测试指标冲突造成不必要的发布时间过长等的消耗，适当地可以允许并发运行。

## 项目JMeter环境依赖和配置

### 项目整体结构

Github地址：[https://github.com/SunflowerOptDevs/webtest](https://github.com/SunflowerOptDevs/webtest)

![](https://img-blog.csdn.net/20170822130740694)

### 通过JMeter生成jmx测试计划

JMeter下载地址：[http://jmeter.apache.org/download_jmeter.cgi](http://jmeter.apache.org/download_jmeter.cgi)

windows下运行：apache-jmeter-3.2/bin/jmeter.bat文件


Linux下运行：apache-jmeter-3.2/bin/jmeter.sh文件

生成jmx请参考：[http://blog.csdn.net/u012938881/article/details/51755105](http://blog.csdn.net/u012938881/article/details/51755105)


注：项目中已加入Jmeter包，不需要再下载了。


### 编写Ant构建文件

build.xml



```
<?xml version="1.0" encoding="UTF-8"?>
<project default="jmeter" >
	
	<property name="jmeter.home" value="${basedir}/tools/apache-jmeter-3.2"  />  
	
	<path  id="ant.jmeter.classpath" >
		<pathelement location="${basedir}/tools/apache-jmeter-3.2/extras/ant-jmeter-1.1.1.jar" />
	</path>

	<taskdef name="jmeter" classname="org.programmerplanet.ant.taskdefs.jmeter.JMeterTask" classpathref="ant.jmeter.classpath" />  

	<target name="jmeter">
		<jmeter jmeterhome="${jmeter.home}" 
		    resultlog="${basedir}/target/jemeter-results.jtl"
			testplan="${basedir}/src/test/jemeter/hello.jmx">
			<jvmarg value="-Xmx512m" />
		</jmeter>
	</target>

</project>
```

### Maven antrun插件配置

pom.xml



```
<!-- jmeter -->
  <plugin>
       <artifactId>maven-antrun-plugin</artifactId>
       <version>1.4</version>
       <executions>
            <execution>
               <phase>integration-test</phase>
               <configuration>
                    <tasks>
                      <ant antfile="build.xml" target="jmeter"/>
                    </tasks>
               </configuration>
               <goals>
                      <goal>run</goal>
               </goals>
            </execution>
       </executions>
  </plugin>
```


注：使用的版本比较低，如果要使用高版本，请阅读github上项目的README.md文件。也可以参考官方的配置：[http://maven.apache.org/plugins/maven-antrun-plugin/usage.html](http://maven.apache.org/plugins/maven-antrun-plugin/usage.html)




## Jenkins配置



### Jenkins安装ANT

系统设置>Global Tool Configuration进行ANT安装![](https://img-blog.csdn.net/20170822130326560)





### Jenkins任务构建相关配置


![](https://img-blog.csdn.net/20170822130434586)




![](https://img-blog.csdn.net/20170822130446718)




![](https://img-blog.csdn.net/20170822130458014)

Jenkins 任务完整配置如下：

![](https://img-blog.csdn.net/20170822133746235)

### JenkinsJMeter性能测试


性能任务测试主界面：


![](https://img-blog.csdn.net/20170822134114692)

性能测试趋势图：

![](https://img-blog.csdn.net/20170822134211860)


## Windows下测试JMeter ANT构建


确保系统中已配置ANT环境，运行构建方式：

1、IDE右键 Run as > Ant Build


2、cmd自动定位到build.xml所在目录运行ant命令进行构建。

以下是构建输出：

![](https://img-blog.csdn.net/20170822131812860)


## Jenkins构建任务注意事项

尽量将各个测试指标放在不同的Jenkins任务中去执行，这样可以大大提升测试构建的效率。我这里将之前的配置重新整理之后如下图所示：

![](https://img-blog.csdn.net/20170822134359735)

看不清楚可以看下图：

![](https://img-blog.csdn.net/20170822141636776)





上面包含了：持续集成和部署、代码覆盖率、JMeter性能测试、TestNG Junit测试。










