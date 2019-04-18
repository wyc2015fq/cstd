# Maven入门指南③：坐标和依赖 - z69183787的专栏 - CSDN博客
2016年05月17日 11:33:33[OkidoGreen](https://me.csdn.net/z69183787)阅读数：321
个人分类：[项目构建-Maven](https://blog.csdn.net/z69183787/article/category/2265961)
1 . 坐标
　　maven 的所有构件均通过坐标进行组织和管理。maven 的坐标通过 5 个元素进行定义，其中 groupId、artifactId、version 是必须的，packaging 是可选的（默认为jar），classifier 是不能直接定义的。
- groupId：定义当前 Maven 项目所属的实际项目，跟 Java 包名类似，通常与域名反向一一对应。
- artifactId：定义当前 Maven 项目的一个模块，默认情况下，Maven 生成的构件，其文件名会以 artifactId 开头，如 hibernate-core-3.6.5.Final.jar。
- version：定义项目版本。
- packaging：定义项目打包方式，如 jar，war，pom，zip ……，默认为 jar。
- classifier：定义项目的附属构件，如 hibernate-core-3.6.6.Final-sources.jar，hibernate-core-3.6.6.Final-javadoc.jar，其中
 sources 和 javadoc 就是这两个附属构件的 classifier。classifier 不能直接定义，通常由附加的插件帮助生成。
2 . 依赖
　　使用 Maven 可以方便的管理依赖，如下是一段在 pom.xml 文件中声明依赖的代码示例：
```
![复制代码](http://common.cnblogs.com/images/copycode.gif)
 1 <dependencies>
 2     <dependency>
 3         <groupId>org.springframework</groupId>
 4         <artifactId>spring-test</artifactId>
 5         <version>3.2.0.RELEASE</version>
 6         <type>jar</type>
 7         <scope>test</scope>
 8         <systemPath>${java.home}/lib/rt.jar</systemPath>
 9         <optional>false</optional>
10         <exclusions>
11             <exclusion></exclusion>
12         </exclusions>
13     </dependency>
14 </dependencies>
![复制代码](http://common.cnblogs.com/images/copycode.gif)
```
- type：依赖类型，对应构件中定义的 packaging，可不声明，默认为 jar；
- scope：依赖范围；
- optional：依赖是否可选；
- exclusions：排除传递依赖。
2 . 1 . 依赖范围
　　执行不同的 Maven 命令（mvn package，mvn test，mvn install ……），会使用不同的 classpath，Maven 对应的有三套 classpath：编译classpath、测试classpath，运行classpath。scope 选项的值，决定了该依赖构件会被引入到哪一个 classpath 中。
- compile：编译依赖范围，默认值。此选项对编译、测试、运行三种 classpath 都有效，如 hibernate-core-3.6.5.Final.jar，表明在编译、测试、运行的时候都需要该依赖；
- test：测试依赖范围。只对测试有效，表明只在测试的时候需要，在编译和运行时将无法使用该类依赖，如 junit；
- provided：已提供依赖范围。编译和测试有效，运行无效。如 servlet-api ，在项目运行时，tomcat 等容器已经提供，无需 Maven 重复引入；
- runtime：运行时依赖范围。测试和运行有效，编译无效。如 jdbc 驱动实现，编译时只需接口，测试或运行时才需要具体的 jdbc 驱动实现；
- system：系统依赖范围。和 provided 依赖范围一致，需要通过 <systemPath> 显示指定，且可以引用环境变量；
- import：导入依赖范围。使用该选项，通常需要 <type>pom</type>，将目标 pom 的 dependencyManagement 配置导入合并到当前 pom 的  dependencyManagement 元素。
2 . 2 . 依赖传递
![](http://images.cnitblog.com/i/293735/201406/130104280149660.png)
　　如上图所示，hibernate-core 依赖 hibernate-commons-annotations ，而 hibernate-commons-annotations 又依赖 slf4j-api ，hibernate-core 对 slf4j-api 的依赖就是传递依赖。我们只需要引入 hibernate-core
 构件的依赖，不用考虑它还有其它的依赖， 也不用担心会引入多余或冲突的依赖，Maven 会自动为我们引入依赖及传递依赖。
2 . 3 . 依赖传递和依赖范围
　　如上图 2.2 所示，几种依赖关系分别叫做第一直接依赖、第二直接依赖和传递性依赖，其中第一直接依赖和第二直接依赖的依赖范围，决定了传递性依赖的依赖范围。
![](http://images.cnitblog.com/i/293735/201406/130126051701838.png)
2 . 4 . 依赖冲突
　　通常我们不需要关心传递性依赖，当多个传递性依赖中有对同一构件不同版本的依赖时，如何解决呢？
- 短路径优先：假如有以下依赖：A -> B -> C ->X(版本 1.0) 和 A -> D -> X(版本 2.0)，则优先解析较短路径的 X(版本 2.0)；
- 先声明优先：若路径长度相同，则谁先声明，谁被解析。
2 . 5 . 依赖排除
　　针对依赖冲突中的“短路径优先”，如果我们想使用长路径的依赖怎么办呢？这时可以使用依赖排除 <exclusions> 元素，显示排除短路径依赖。在非冲突的情况下，这种方法同样有效。
2 . 6 . 依赖归类
　　通常在项目中，我们会同时依赖同一个构件的不同模块，如 spring-orm-3.2.0，spring-context-3.2.0，且多个模块版本相同，为了维护和升级方便，我们可以对其同一管理，这时可以使用到 Maven 属性，类似于变量的概念。
```
![复制代码](http://common.cnblogs.com/images/copycode.gif)
 1 <properties>
 2     <springframework.version>3.2.0.RELEASE</springframework.version>
 3 </properties>
 4 
 5 <dependencies>
 6     <dependency>
 7         <groupId>org.springframework</groupId>
 8         <artifactId>spring-orm</artifactId>
 9         <version>${springframework.version}</version>
10     </dependency>
11     <dependency>
12         <groupId>org.springframework</groupId>
13         <artifactId>spring-context</artifactId>
14         <version>${springframework.version}</version>
15     </dependency>
16 </dependencies>
![复制代码](http://common.cnblogs.com/images/copycode.gif)
```
