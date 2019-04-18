# maven打包时，同时打包源码 Build a source-JAR from the current project. - z69183787的专栏 - CSDN博客
2016年07月04日 10:41:00[OkidoGreen](https://me.csdn.net/z69183787)阅读数：1748
Plugin: [http://maven.apache.org/plugins/maven-source-plugin/](http://maven.apache.org/plugins/maven-source-plugin/)
The Source Plugin has five goals:
- [source:aggregate](http://maven.apache.org/plugins/maven-source-plugin/aggregate-mojo.html) aggregrates sources for all modules in an
 aggregator project.
- [source:jar](http://maven.apache.org/plugins/maven-source-plugin/jar-mojo.html) is used to bundle the main sources of the project into
 a jar archive.
- [source:test-jar](http://maven.apache.org/plugins/maven-source-plugin/test-jar-mojo.html) on the other hand, is used to bundle the test
 sources of the project into a jar archive.
- [source:jar-no-fork](http://maven.apache.org/plugins/maven-source-plugin/jar-no-fork-mojo.html) is similar to **jar** but
 does not fork the build lifecycle.
- [source:test-jar-no-fork](http://maven.apache.org/plugins/maven-source-plugin/test-jar-no-fork-mojo.html) is similar to **test-jar** but
 does not fork the build lifecycle.
1. 打包 main 目录代码到 jar 归档文件
mvn source:jar
2. 打包 test 目录代码到 jar 归档文件
mvn source:test-jar
3. 如果不想每次都传入上面的参数，可以将插件目标绑定到mvn生命周期的阶段，
将maven-source-plugin的jar-no-fork目标绑定到default生命周期的verify阶段，
以后执行mvn install命令时，在verify之后install之前会自动打包源代码，
生成somenamexxx-1.0.0-sources.jar文件
**[html]**[view
 plain](http://blog.csdn.net/chs_jdmdr/article/details/42419191#)[copy](http://blog.csdn.net/chs_jdmdr/article/details/42419191#)
![在CODE上查看代码片](https://code.csdn.net/assets/CODE_ico.png)[](https://code.csdn.net/snippets/572092/fork)
- <project>
-   ...  
- <build>
- <plugins>
- <plugin>
- <groupId>org.apache.maven.plugins</groupId>
- <artifactId>maven-source-plugin</artifactId>
- <version>2.4</version>
- <executions>
- <execution>
- <id>attach-sources</id>
- <phase>verify</phase><!--  要绑定到的生命周期的阶段 在verify之后，install之前执行下面指定的goal -->
- <goals>
- <goal>jar-no-fork</goal><!-- 类似执行mvn source:jar -->
- </goals>
- </execution>
- </executions>
- </plugin>
- </plugins>
- </build>
-   ...  
- </project>
- 
