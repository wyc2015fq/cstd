# maven基础 - weixin_33985507的博客 - CSDN博客
2017年05月07日 21:51:00[weixin_33985507](https://me.csdn.net/weixin_33985507)阅读数：3
# 坐标
maven管理项目依赖的底层基础－坐标。坐标元素：groupId、artifactId、version、packaging、classifier。
```
<dependency>
         <groupId>org.apache.pig</groupId>
         <artifactId>pig</artifactId>
         <classifier>h2</classifier>
         <version>0.15.0</version>
         <scope>provided</scope>
</dependency>
```
**groupId**:定义当前maven项目隶属的实际项目。
**artifactId**:定义实际项目中的一个maven项目(模块),推荐的做法是使用实际项目名作为artifactId的前缀。
**version**:定义maven项目当前所处的版本。
**packaging**:定义maven项目的打包方式。默认使用jar。
**classifier**:定义用来帮助定义构建输出的一些附属构件。例如xxx-1.0-javadoc.jar和xxx-1.0-sources.jar就包含了java文档和源代码。注意：不能直接定义项目的classifier,因为附属构件不是项目直接默认生成的，而是由附加的插件帮助生成的。
groupId、artifactId、version是必须定义的，packaging是可选的，classifier是不能直接定义的。
项目构件的文件名一般的规则为artifactId-version [-classifier].packaging。
# 依赖
## 依赖配置
```
<dependency>
                <groupId>org.apache.storm</groupId>
                <artifactId>storm-core</artifactId>
                <version>${storm.version}</version>
                <scope>provided</scope>
                <type>jar</type>
                <optional>true</optional>
                <exclusions>
                    <exclusion>
                        <groupId>org.slf4j</groupId>
                        <artifactId>slf4j-api</artifactId>
                    </exclusion>
                    <exclusion>
                        <groupId>org.slf4j</groupId>
                        <artifactId>slf4j-log4j12</artifactId>
                    </exclusion>
                </exclusions>
            </dependency>
```
**type**：依赖的类型，对应于项目坐标定义的packaging。
**scope**:依赖的范围。
**optional**:标记依赖是否可选。
**exclusions**:用来排除传递性依赖。
## 依赖范围
缘由：首先，maven在编译项目主代码的时候需要使用一套classpath，需要的文件以依赖的方式被引入到classpath中。其次，maven在编译和执行测试的时候会使用另外一套classpath。最后，实际运行maven项目的时候，又会使用一套classpath。
依赖范围就是用来控制依赖与这三种classpath（编译classpath、测试classpath、运行classpath）的关系。
**compile**:编译依赖范围。默认的依赖范围。对于编译、测试、运行三种classpath都有效。
**test**:测试依赖范围。只对测试classpath有效。
**provided**：已提供依赖范围。对于编译和测试classpath有效，但在运行时无效。例如servlet-api。
**runtime**:运行时依赖范围。对于测试和运行时classpath有效，对编译主代码时无效。例如：jdbc，编译时只需要jdk提供的jdbc接口，只有在执行测试或者运行项目的时候才需要实现该接口的具体jdbc驱动。
**system**:系统依赖范围。同provided依赖范围。使用时必须通过systemPath元素显式指定依赖的文件的路径。此依赖不是通过maven仓库解析的，往往与本机系统绑定。
**import**：导入依赖范围。不会对三种classpath产生实际的影响。与dependencyManagement有关。
![2267418-8457ff481e9185e7.png](https://upload-images.jianshu.io/upload_images/2267418-8457ff481e9185e7.png)
依赖范围与classpath的关系
## 传递性依赖
假如A依赖于B，B依赖于C，那么A对于B第一直接依赖，B对于C是第二直接依赖，A对于C是传递性依赖。
依赖范围不仅可以控制依赖与三种classpath的关系，还对传递性依赖产生影响。
![2267418-ae87e9c1e6fb720a.png](https://upload-images.jianshu.io/upload_images/2267418-ae87e9c1e6fb720a.png)
依赖范围影响传递性依赖
注：左一列表示第一直接依赖范围，最上一行表示第二直接直接依赖范围，中间的交叉单元格则表示传递性依赖范围。
## 依赖调解
第一原则：路径最近者优先。
第二原则：第一声明者优先。在依赖路径长度相等的前提下，在pom中依赖声明的顺序决定了谁会被解析使用，顺序最靠前的那个依赖优胜。
## 可选依赖
可选依赖，可能项目实现了两个特性，特性一依赖于X，特性二依赖于Y，而且这两个特性是互斥的，用户不可能同时使用两个特性。
可选依赖不会被传递，需要显式声明依赖。
## 依赖冲突
- 使用exclusion元素
- 直接引入需要的依赖
## 优化依赖
mvn dependency:list
mvn dependency:tree 详细了解项目中所有依赖的具体信息
mvn dependency:analyze 可以分析项目中使用到的，但没有显式声明的依赖；项目中未使用的，但显式声明的依赖。注意：只会分析编译主代码和测试代码需要用到的依赖，一些执行测试和运行时需要的依赖发现不了。
# 仓库
本地仓库
远程仓库：中央仓库，私服，其他公共库
SNAPSHOT
RELEASE
# 生命周期和插件
maven的生命周期是为了对所有的构建过程进行抽象和统一。这个生命周期包含了项目的清理、初始化、编译、测试、打包、集成测试、验证、部署和站点生成等几乎所有构建步骤。几乎所有项目的构建，都能映射到这样一个生命周期上。
maven的生命周期是抽象的，实际的任务都交由插件来完成。例如：maven-compiler-plugin针对编译的插件，maven-surefire-plugin针对测试的插件。
## 生命周期
maven有三套相互独立的生命周期，clean、default、site。clean生命周期的目的是清理项目，default生命周期的目的是构建项目，site生命周期的目的是建立项目站点。
每个生命周期包含一些阶段(phase)，这些阶段是有顺序的，并且后面的阶段依赖于前面的阶段。执行某个阶段的时候，会顺序执行该阶段之前的所有阶段。
### clean生命周期
pre-clean：执行一些清理前需要完成的工作
clean:清理上一次构建生成的文件
post-clean:执行一些清理后需要完成的工作
### default生命周期
validate
initialize
generate-sources
process-sources:处理项目主资源文件
generate-resources
process-resources
compile:编译项目的主源码
process-classes
generate-test-sources
process-test-sources:处理项目测试资源文件
generate-test-resources
process-test-resources
test-compile:编译项目的测试代码
process-test-classes
test:使用单元测试框架运行测试，测试代码不会被打包或部署
prepare-package
package:接受编译好的代码，打包成可发布的格式
pre-integration-test
integration-test
post-integration-test
verify
install:将包安装到maven本地仓库，供本地其他maven项目使用
deploy:将最终的包复制到远程仓库，供其他开发人员和maven项目使用
### site生命周期
pre-site:执行一些在生成项目站点之前需要完成的工作
site:生成项目站点文档
post-site:执行一些在生成项目站点之后需要完成的工作
site-deploy:将生成的项目站点发布到服务器上
### 命令行与生命周期
从命令行执行maven任务的最主要方式就是调用maven的生命周期阶段。
mvn clean
mvn test
mvn clean install
mvn clean deploy site-deploy
## 插件
### 插件目标
每个插件有很多功能，每个功能就是一个插件目标。
例如：maven-dependency-plugin的插件目标有dependency:analyze,dependency:tree等。冒号前面是插件前缀，冒号后面是该插件的目标。
### 插件绑定
maven的生命周期与插件项目绑定，用以完成实际的构建任务。
### 插件配置
# 聚合和继承
## 聚合
```
<modules>
        <module>cpspool_index_base</module>
        <module>cpspool_index_batch</module>
        <module>cpspool_index_realtime</module>
        <module>cpspool_index_batch_signal_node</module>
        <module>cpspool_index_batch_create_index_node</module>
        <module>cpspool_index_batch_node</module>
        <module>cpspool_index_batch_online_index_node</module>
    </modules>
```
- 目标
一次构建多个项目，而不是到多个项目目录下分别构建。
- 打包方式
对于聚合模块，其打包方式packaging的值必须是pom，否则无法构建。
- 目录结构
可以是父子关系，可以是平行关系。
## 继承
```
<parent>
        <artifactId>union_cpspool_index</artifactId>
        <groupId>com.jd.ads.union.cps</groupId>
        <version>1.0-SNAPSHOT</version>
    </parent>
```
- 目标
消除重复和潜在的问题
- 打包方式
作为父模块的POM，打包类型也必须是pom。
- 继承元素
**groupId**
**version**
**properties**:自定义的maven属性
**dependencies**：项目的依赖配置
**dependencyManagement**：项目的依赖管理配置
**repositories**：项目的仓库配置
**build**：包括项目的源码目录配置、输出目录配置、插件配置、插件管理配置等。
- dependencies和dependencyManagement
继承父POM的dependencies后，可以移除子POM中的依赖配置，从而简化配置，但是无法确定将来添加的子模块是否一定需要父POM的所有配置。
dependencyManagement元素既能让子模块继承到父模块的依赖配置，又能保证子模块依赖使用的灵活性。在dependencyManagement元素下的依赖声明不会引入实际的依赖，不过它能够约束dependencies下的依赖使用。
import依赖范围，只在dependencyManagement元素下有效，使用该范围的依赖通常指向一个POM，作用是将目标POM中的dependencyManagement配置导入并合并到当前POM的dependencyManagement元素中。
## 聚合与继承的关系
聚合主要是为了方便快速构建项目，继承主要是为了消除重复配置。
对于聚合模块来说，它知道有哪些被聚合的模块，但那些被聚合的模块不知道这个聚合模块的存在。
对于继承关系的父POM来说，它不知道有哪些子模块继承于它，但那些子模块都必须知道自己的父POM是什么。
## 构建顺序
maven按序读取POM，如果该POM没有依赖模块，那么就构建该模块，否则就先构建其依赖模块，如果该依赖还依赖于其他模块，则进一步先构建依赖的依赖。
# 灵活性三大特性
## 属性
使用properties元素定义，使用${属性名称}的方式引用，可以消除重复。
**内置属性**:${basedir}表示项目跟目录，即包含pom.xml文件的目录；${version}表示项目的版本。
**POM属性**:可以引用POM文件对应的元素的值。${project.artifactId}
**自定义属性**:用户在properties元素自定义的属性。
**Settings属性**:引用settings.xml文件中XML元素的值，${settings.localRepository}
**Java系统属性**:
**环境变量属性**:
## profile
根据环境不同进行不同的配置，例如数据库配置。
可以使用-P参数激活profile
```
<profiles>
        <profile>
            <id>dev</id>
            <properties>
                <deploy.type>dev</deploy.type>
            </properties>
        </profile>      
        <profile>
            <id>prod</id>
            <properties>
                <deploy.type>prod</deploy.type>
            </properties>
        </profile>    
    </profiles>
```
## 资源过滤
# 推荐
官网：[http://maven.apache.org/](https://link.jianshu.com?t=http://maven.apache.org/)
<<maven权威指南>>
教程：[http://www.yiibai.com/maven/](https://link.jianshu.com?t=http://www.yiibai.com/maven/)
