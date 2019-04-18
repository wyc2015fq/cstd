# MAVEN项目标准目录结构 - z69183787的专栏 - CSDN博客
2014年05月15日 16:40:09[OkidoGreen](https://me.csdn.net/z69183787)阅读数：591
  1.标准目录结构：
src
  -main
      –bin 脚本库
      –java java源代码文件
      –resources 资源库，会自动复制到classes目录里
      –filters 资源过滤文件
      –assembly 组件的描述配置（如何打包）
      –config 配置文件
      –webapp web应用的目录。WEB-INF、css、js等
  -test
      –java 单元测试java源代码文件
      –resources 测试需要用的资源库
      –filters 测试资源过滤库
  -site Site（一些文档）
target
LICENSE.txt Project’s license
README.txt Project’s readme
工程根目录下就只有src和target两个目录
target是有存放项目构建后的文件和目录，jar包、war包、编译的class文件等。
target里的所有内容都是maven构建的时候生成的
参照：http://breath.iteye.com/blog/1005447
++++++++++++++++++++++++++++++++++++++++++++
**Maven****项目的标准目录介绍**
Maven提倡使用一个共同的标准目录结构，使开发人员能在熟悉了一个Maven工程后，对其他的Maven工程也能清晰了解。这样做也省去了很多设置的麻烦。
以下的文档介绍是Maven希望的目录结构，并且也是目录创建工程是采用的目录结构。Maven推荐大家尽可能的遵守这样的目录结构。
|**src/main/java**|**Application/Library sources**|
|----|----|
|src/main/resources|Application/Library resources|
|src/main/filters|Resource filter files|
|src/main/assembly|Assembly descriptors|
|src/main/config|Configuration files|
|src/main/webapps|Web application sources|
|src/test/java|Test sources|
|src/test/resources|Test resources|
|src/test/filters|Test resource filter files|
|src/site|Site|
|LICENSE.txt|Project's license|
|README.txt|Project's readme|
在顶级目录上是工程的描述文件pom.xml（如果使用Ant则还包括其他属性文件，maven.xml或build.xml）,另外还包括提供给最终用户的文件，如，README.txt,LICENSE.txt等等。
顶级目录还包括两个子目录：src，target。顶级目录下可能出现的其他目录仅仅是CVS或.svn和其他多模块工程的工程目录，最好不要再有其他目录。
Target目录是所有工程编译构建的输出目录。
Src目录包含所有工程的源码文件，配置文件，资源文件等等。它下面的子目录一般包含main（主要的工程源文件），test（测试文件），site（项目站点文件）。
**项目构建的生命周期的介绍**
Maven 2是围绕着构建生命周期概念设计的。这意味着，构建或者发布的过程已经被清晰的定义了。
当我们使用Maven构建工程时，我们只需要了解几个Maven定义好的命令即可，其他的工作则交给POM来完成。
以下给出Maven提供的构建生命周期列表：
|validate|validate the project is correct and all necessary information is available.|
|----|----|
|generate-sources|generate any source code for inclusion in compilation.|
|process-sources|process the source code, for example to filter any values.|
|generate-resources|generate resources for inclusion in the package.|
|process-resources|copy and process the resources into the destination directory, ready for packaging.|
|compile|compile the source code of the project.|
|process-classes|post-process the generated files from compilation, for example to do bytecode enhancement on Java classes.|
|generate-test-sources|generate any test source code for inclusion in compilation.|
|process-test-sources|process the test source code, for example to filter any values.|
|generate-test-resources|create resources for testing.|
|process-test-resources|copy and process the resources into the test destination directory.|
|test-compile|compile the test source code into the test destination directory|
|test|run tests using a suitable unit testing framework. These tests should not require the code be packaged or deployed.|
|package|take the compiled code and package it in its distributable format, such as a JAR.|
|pre-integration-test|perform actions required before integration tests are executed. This may involve things such as setting up the required environment.|
|integration-test|process and deploy the package if necessary into an environment where integration tests can be run.|
|post-integration-test|perform actions required after integration tests have been executed. This may including cleaning up the environment.|
|verify|run any checks to verify the package is valid and meets quality criteria.|
|install|install the package into the local repository, for use as a dependency in other projects locally.|
|deploy|done in an integration or release environment, copies the final package to the remote repository for sharing with other developers and projects.|
因此，当我们构建一个项目时，只需要了解自己希望做什么，然后执行以上对应的生命周期即可。
例如，我们希望编译我们的工程。在命令行状态下进入到工程的pom.xml文件所在的目录中，使用命令:mvn compile；希望构建打包我们的工程，使用mvn package即可。
当然了，maven的构建生命周期也是可以扩展和自定义的，这里就先不做介绍了。
参照：http://hi.baidu.com/mylovechangchu/blog/item/fbda36da3644a6dfb6fd48d6.html
+++++++++++++++++++++++++++++++
APACHE原址：http://maven.apache.org/guides/introduction/introduction-to-the-standard-directory-layout.html
