# maven 生命周期 及 正确的集成命令-U -B -X -e 等 - z69183787的专栏 - CSDN博客
2016年10月31日 18:03:50[OkidoGreen](https://me.csdn.net/z69183787)阅读数：8149
在持续集成服务器上使用怎样的 mvn 命令集成项目，这个问题乍一看答案很显然，不就是 **mvn clean install** 么？事实上比较好的集成命令会稍微复杂些，下面是一些总结：
- 
**不要忘了clean：** clean能够保证上一次构建的输出不会影响到本次构建。
- 
**使用deploy而不是install：** 构建的SNAPSHOT输出应当被自动部署到私有Maven仓库供他人使用，这一点在前面已经详细论述。
- 
**使用-U参数：** 该参数能强制让Maven检查所有SNAPSHOT依赖更新，确保集成基于最新的状态，如果没有该参数，Maven默认以天为单位检查更新，而持续集成的频率应该比这高很多。
- 
**使用-e参数：**如果构建出现异常，该参数能让Maven打印完整的stack trace，以方便分析错误原因。
- 
**使用-Dmaven.repo.local参数：**如果持续集成服务器有很多任务，每个任务都会使用本地仓库，下载依赖至本地仓库，为了避免这种多线程使用本地仓库可能会引起的冲突，可以使用**-Dmaven.repo.local=/home/juven/ci/foo-repo/**这样的参数为每个任务分配本地仓库。
- 
**使用-B参数：**该参数表示让Maven使用批处理模式构建项目，能够避免一些需要人工参与交互而造成的挂起状态。
- 
**使用-X参数：开启DEBUG模式。**
综上，持续集成服务器上的集成命令应该为 **mvn clean deploy -B -e -U -Dmaven.repo.local=xxx** 。此外，定期清理持续集成服务器的本地Maven仓库也是个很好的习惯，这样可以避免浪费磁盘资源，几乎所有的持续集成服务器软件都支持本地的脚本任务，你可以写一行简单的shell或bat脚本，然后配置以天为单位自动清理仓库。需要注意的是，这么做的前提是你有私有Maven仓库，否则每次都从Internet下载所有依赖会是一场噩梦。
mvn clean install
生命周期阶段 描述|validate|验证项目是否正确，以及所有为了完整构建必要的信息是否可用|
|----|----|
|generate-sources|生成所有需要包含在编译过程中的源代码|
|process-sources|处理源代码，比如过滤一些值|
|generate-resources|生成所有需要包含在打包过程中的资源文件|
|process-resources|复制并处理资源文件至目标目录，准备打包|
|compile|编译项目的源代码|
|process-classes|后处理编译生成的文件，例如对Java类进行字节码增强（bytecode enhancement）|
|generate-test-sources|生成所有包含在测试编译过程中的测试源码|
|process-test-sources|处理测试源码，比如过滤一些值|
|generate-test-resources|生成测试需要的资源文件|
|process-test-resources|复制并处理测试资源文件至测试目标目录|
|test-compile|编译测试源码至测试目标目录|
|test|使用合适的单元测试框架运行测试。这些测试应该不需要代码被打包或发布|
|prepare-package|在真正的打包之前，执行一些准备打包必要的操作。这通常会产生一个包的展开的处理过的版本（将会在Maven 2.1+中实现）|
|package|将编译好的代码打包成可分发的格式，如JAR，WAR，或者EAR|
|pre-integration-test|执行一些在集成测试运行之前需要的动作。如建立集成测试需要的环境|
|integration-test|如果有必要的话，处理包并发布至集成测试可以运行的环境|
|post-integration-test|执行一些在集成测试运行之后需要的动作。如清理集成测试环境。|
|verify|执行所有检查，验证包是有效的，符合质量规范|
|install|安装包至本地仓库，以备本地的其它项目作为依赖使用|
|deploy|复制最终的包至远程仓库，共享给其它开发人员和项目（通常和一次正式的发布相关）|
mvn jar:jar     JAR打包默认的目标
生命周期阶段 目标
|process-resources|resources:resources|
|----|----|
|compile|compiler:compile|
|process-test-resources|resources:testResources|
|test-compile|compiler:testCompile|
|test|surefire:test|
|package|jar:jar|
|install|install:install|
|deploy|deploy:deploy|
POM打包默认的目标
生命周期阶段 目标
|package|site:attach-descriptor|
|----|----|
|install|install:install|
|deploy|deploy:deploy|
EJB打包默认的目标
生命周期阶段 目标
|process-resources|resources:resources|
|----|----|
|compile|compiler:compile|
|process-test-resources|resources:testResources|
|test-compile|compiler:testCompile|
|test|surefire:test|
|package|ejb:ejb|
|install|install:install|
|deploy|deploy:deploy|
Maven提供了对EJB 2和3的支持。必须配置EJB插件来为EJB3指定打包类型，否则该插件默认认为EJB为2.1，并寻找某些EJB配置文件是否存在。
WAR打包默认的目标
生命周期阶段 目标
|process-resources|resources:resources|
|----|----|
|compile|compiler:compile|
|process-test-resources|resources:testResources|
|test-compile|compiler:testCompile|
|test|surefire:test|
|package|war:war|
|install|install:install|
|deploy|deploy:deploy|
注意`war:war`插件需要一个`web.xml`配置文件在项目的`src/main/webapp/WEB-INF`目录中。
EAR打包默认的目标 生命周期阶段 目标|generate-resources|ear:generate-application-xml|
|----|----|
|process-resources|resources:resources|
|package|ear:ear|
|install|install:install|
|deploy|deploy:deploy|
