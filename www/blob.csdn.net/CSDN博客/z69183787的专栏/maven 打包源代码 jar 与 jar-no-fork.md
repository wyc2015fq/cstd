# maven 打包源代码 jar 与 jar-no-fork - z69183787的专栏 - CSDN博客
2017年01月11日 16:04:30[OkidoGreen](https://me.csdn.net/z69183787)阅读数：6663
官方解释：
### Goals Overview
The Source Plugin has five goals:
- [source:aggregate](http://maven.apache.org/plugins/maven-source-plugin/aggregate-mojo.html) aggregrates sources for all modules in
 an aggregator project.
- [source:jar](http://maven.apache.org/plugins/maven-source-plugin/jar-mojo.html) is used to bundle the main sources of the project
 into a jar archive.
- [source:test-jar](http://maven.apache.org/plugins/maven-source-plugin/test-jar-mojo.html) on the other hand, is used to bundle the
 test sources of the project into a jar archive.
- [source:jar-no-fork](http://maven.apache.org/plugins/maven-source-plugin/jar-no-fork-mojo.html) is similar to **jar** but
 does not fork the build lifecycle.
- [source:test-jar-no-fork](http://maven.apache.org/plugins/maven-source-plugin/test-jar-no-fork-mojo.html) is similar to **test-jar** but
 does not fork the build lifecycle.
前言
maven"内置插件"中没有打包源代码的插件，所以，需要开发者配置插件。apache提供了maven-source-plugin插件用于打包源代码。
maven-source-plugin
The Source Plugin has five goals:
- [source:aggregate](http://maven.apache.org/plugins/maven-source-plugin/aggregate-mojo.html) aggregrates
 sources for all modules in an aggregator project.
- [source:jar](http://maven.apache.org/plugins/maven-source-plugin/jar-mojo.html) is
 used to bundle the main sources of the project into a jar archive.
- [source:test-jar](http://maven.apache.org/plugins/maven-source-plugin/test-jar-mojo.html) on
 the other hand, is used to bundle the test sources of the project into a jar archive.
- [source:jar-no-fork](http://maven.apache.org/plugins/maven-source-plugin/jar-no-fork-mojo.html) is
 similar to jar but does not fork the build lifecycle.
- [source:test-jar-no-fork](http://maven.apache.org/plugins/maven-source-plugin/test-jar-no-fork-mojo.html) is
 similar to test-jar but does not fork the build lifecycle
配置
在《maven实战》一书中，配置使用的是jar-no-fork
```
<plugins>
			<plugin>
				<artifactId>maven-source-plugin</artifactId>
				<version>2.1.1</version>
				<!-- 配置插件参数 -->
				<executions>
					<execution>
						<id>attach-sources</id>
						<phase>verify</phase>
						<goals>
							<goal>jar-no-fork</goal>
						</goals>
					</execution>
				</executions>
			</plugin>
		</plugins>
```
jar-no-fork与jar的区别
jar，在执行goal之前，执行generate-sources阶段，也就是说，如果，jar绑定的目标在generate-sources之后(比如verify)的话，generate-sources会执行两遍。
- The goal is thread-safe and supports parallel builds.
- Binds by default to the [lifecycle
 phase](http://maven.apache.org/ref/current/maven-core/lifecycles.html): package
- Invokes the execution of the [lifecycle
 phase](http://maven.apache.org/ref/current/maven-core/lifecycles.html)generate-sources prior to executing itself.
jar-no-fork，没有其余动作，在绑定的phase执行。原文是，
- The goal is thread-safe and supports parallel builds.
- Binds by default to the [lifecycle
 phase](http://maven.apache.org/ref/current/maven-core/lifecycles.html): package.
为什么会有jar-no-fork?
临时，我也没找到它存在的理由。
参考
[maven-source-plugin
 doc](http://maven.apache.org/plugins/maven-source-plugin/)
[difference
 between maven-source-plugin-jar-and-jar-no-fork-goal](http://stackoverflow.com/questions/10567551/difference-between-maven-source-plugin-jar-and-jar-no-fork-goal)
