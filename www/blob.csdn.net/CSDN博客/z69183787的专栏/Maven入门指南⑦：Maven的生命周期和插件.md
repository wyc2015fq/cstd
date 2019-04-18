# Maven入门指南⑦：Maven的生命周期和插件 - z69183787的专栏 - CSDN博客
2016年05月17日 11:53:04[OkidoGreen](https://me.csdn.net/z69183787)阅读数：382
个人分类：[项目构建-Maven](https://blog.csdn.net/z69183787/article/category/2265961)

　一个完整的项目构建过程通常包括清理、编译、测试、打包、集成测试、验证、部署等步骤，Maven从中抽取了一套完善的、易扩展的生命周期。Maven的生命周期是抽象的，其中的具体任务都交由插件来完成。Maven为大多数构建任务编写并绑定了默认的插件，如针对编译的插件：maven-compiler-plugin。用户也可自行配置或编写插件。
1 . 三套生命周期
　　Maven定义了三套生命周期：clean、default、site，每个生命周期都包含了一些阶段（phase）。三套生命周期相互独立，但各个生命周期中的phase却是有顺序的，且后面的phase依赖于前面的phase。执行某个phase时，其前面的phase会依顺序执行，但不会触发另外两套生命周期中的任何phase。
1 . 1 clean生命周期
- pre-clean    ：执行清理前的工作；
- clean    ：清理上一次构建生成的所有文件；
- post-clean    ：执行清理后的工作
1 . 2 default生命周期
default生命周期是最核心的，它包含了构建项目时真正需要执行的所有步骤。
- validate
- initialize
- generate-sources
- process-sources
- generate-resources
- process-resources    ：复制和处理资源文件到target目录，准备打包；
- compile    ：编译项目的源代码；
- process-classes
- generate-test-sources
- process-test-sources
- generate-test-resources
- process-test-resources
- test-compile    ：编译测试源代码；
- process-test-classes
- test    ：运行测试代码；
- prepare-package
- package    ：打包成jar或者war或者其他格式的分发包；
- pre-integration-test
- integration-test
- post-integration-test
- verify
- install    ：将打好的包安装到本地仓库，供其他项目使用；
- deploy    ：将打好的包安装到远程仓库，供其他项目使用；
1 . 3 site生命周期
- pre-site
- site    ：生成项目的站点文档；
- post-site
- site-deploy    ：发布生成的站点文档
2 . Maven的插件
Maven的核心文件很小，主要的任务都是由插件来完成。定位到：%本地仓库%\org\apache\maven\plugins，可以看到一些下载好的插件：
![](http://images.cnitblog.com/i/293735/201407/012038215279940.png)
Maven官网上有更详细的官方插件列表：
![](http://images.cnitblog.com/i/293735/201407/012038373404639.png)
2 . 1 插件的目标（Plugin Goals）
一个插件通常可以完成多个任务，每一个任务就叫做插件的一个目标。如执行mvn install命令时，调用的插件和执行的插件目标如下：
![](http://images.cnitblog.com/i/293735/201407/012039051375930.png)
每个插件都有哪些个目标，官方文档有更详细的说明：[Maven Plugins](http://maven.apache.org/plugins/index.html)
![](http://images.cnitblog.com/i/293735/201407/012052285122000.png)
3 . 将插件绑定到生命周期
　　Maven的生命周期是抽象的，实际需要插件来完成任务，这一过程是通过将插件的目标（goal）绑定到生命周期的具体阶段（phase）来完成的。如：将maven-compiler-plugin插件的compile目标绑定到default生命周期的compile阶段，完成项目的源代码编译：
![](http://images.cnitblog.com/i/293735/201407/012039514021164.png)
3 . 1 内置的绑定
　　Maven对一些生命周期的阶段（phase）默认绑定了插件目标，因为不同的项目有jar、war、pom等不同的打包方式，因此对应的有不同的绑定关系，其中针对default生命周期的jar包打包方式的绑定关系如下：
![](http://images.cnitblog.com/i/293735/201407/012040407939185.png)
第二列中，冒号后面即是绑定的插件目标，冒号前面是插件的前缀（prefix），是配置和使用插件的一种简化方式。[Plugin
 Prefix](http://maven.apache.org/guides/introduction/introduction-to-plugin-prefix-mapping.html)
3 . 2 自定义绑定
　　用户可以根据需要将任何插件目标绑定到任何生命周期的阶段，如：将maven-source-plugin的jar-no-fork目标绑定到default生命周期的package阶段，这样，以后在执行mvn package命令打包项目时，在package阶段之后会执行源代码打包，生成如：ehcache-core-2.5.0-sources.jar形式的源码包。
```
![复制代码](http://common.cnblogs.com/images/copycode.gif)
<build>
    <plugins>
        <plugin>
            <groupId>org.apache.maven.plugins</groupId>
            <artifactId>maven-source-plugin</artifactId>
            <version>2.2.1</version>
            <executions>
                <execution>
                    <id>attach-source</id>
                    <phase>package</phase><!-- 要绑定到的生命周期的阶段 -->
                    <goals>
                        <goal>jar-no-fork</goal><!-- 要绑定的插件的目标 -->
                    </goals>
                </execution>
            </executions>
        </plugin>
    </plugins>
    ……
</build>
![复制代码](http://common.cnblogs.com/images/copycode.gif)
```
4 . 配置插件
Maven插件高度易扩展，可以方便的进行自定义配置。如：配置maven-compiler-plugin插件编译源代码的JDK版本为1.7：
```
![复制代码](http://common.cnblogs.com/images/copycode.gif)
<plugin>
    <groupId>org.apache.maven.plugins</groupId>
    <artifactId>maven-compiler-plugin</artifactId>
    <configuration>
        <source>1.7</source>
        <target>1.7</target>
    </configuration>
</plugin>
![复制代码](http://common.cnblogs.com/images/copycode.gif)
```
也可以对插件的各个目标进行更具体的配置。[Configuring Plug-ins](http://maven.apache.org/guides/mini/guide-configuring-plugins.html)
5 . 插件仓库
跟其他构件一样，插件也是根据坐标存储在Maven仓库中。超级POM中Maven配置的默认插件远程仓库如下：
```
![复制代码](http://common.cnblogs.com/images/copycode.gif)
<pluginRepositories>
    <pluginRepository>
      <id>central</id>
      <name>Central Repository</name>
      <url>http://repo.maven.apache.org/maven2</url>
      <layout>default</layout>
      <snapshots>
        <enabled>false</enabled>
      </snapshots>
      <releases>
        <updatePolicy>never</updatePolicy>
      </releases>
    </pluginRepository>
</pluginRepositories>
![复制代码](http://common.cnblogs.com/images/copycode.gif)
```
