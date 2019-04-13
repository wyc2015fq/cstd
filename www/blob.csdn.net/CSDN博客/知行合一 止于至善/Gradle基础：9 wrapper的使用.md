
# Gradle基础：9:wrapper的使用 - 知行合一 止于至善 - CSDN博客

2018年11月25日 06:12:22[liumiaocn](https://me.csdn.net/liumiaocn)阅读数：491


![在这里插入图片描述](https://img-blog.csdnimg.cn/2018111707041592.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L2xpdW1pYW9jbg==,size_16,color_FFFFFF,t_70)
Gradle Wrapper是gradle建议的使用方式，这篇文章将会结合具体的例子来说明一下如何使用。
[
](https://img-blog.csdnimg.cn/2018111707041592.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L2xpdW1pYW9jbg==,size_16,color_FFFFFF,t_70)
# Gradle Wrapper
[
](https://img-blog.csdnimg.cn/2018111707041592.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L2xpdW1pYW9jbg==,size_16,color_FFFFFF,t_70)Gradle Wrapper实际上就是一个脚本，使用它可以下载和使用指定版本的gradle，根据需要进行在使用之前进行下载，有效避免本地机器的设定等环境一致性问题。
虽然gradle的安装已经非常简单，但是使用gradle wrapper是的开发这能够以一种更为标准化的方式创建gradle项目。
# 使用方式
Gradle Wrapper的使用方式主要是按照如下步骤：
Step 1: 下载gradle
Step 2: 将下载的gradle解压并存储至GRADLE_USER_HOME所指定的目录
Step 3: 使用解压的gradle
![在这里插入图片描述](https://img-blog.csdnimg.cn/20181124102536685.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L2xpdW1pYW9jbg==,size_16,color_FFFFFF,t_70)
事前准备
使用下面的代码示例，内容不再说明，具体说明信息请参看：
[https://blog.csdn.net/liumiaocn/article/details/84230280](https://blog.csdn.net/liumiaocn/article/details/84230280)
liumiaocn:wrapper liumiao$ ls
build.gradle
liumiaocn:wrapper liumiao$ cat build.gradle 
println "[phase:configuration] build.gradle ..."
task compile {
  group 'compile'
  description 'compile task'
  println "[phase:configuration] compile"
  doFirst {
    println "[phase:execution] compile :doFirst()"
  }
}
tasks.create(name: 'test',dependsOn: compile) {
  group 'test'
  description 'test task'
  println "[phase:configuration] test"
  doLast {
    println "[phase:execution] test:doLast()"
  }
}
tasks.create("packaging") {
  group 'packaging'
  description 'packaging task'
  dependsOn test
  enabled   true
  println "[phase:configuration] packaging"
  doLast {
    println "[phase:execution] packaging:doLast()"
  }
}
class Install extends DefaultTask{
  String installObjectName
  @TaskAction
  void checkObject() {
    println "[phase:execution] install:checkObject   (${installObjectName})"
  }
  @TaskAction
  void installObject() {
    println "[phase:execution] install:installObject (${installObjectName})"
  }
}
task install(type: Install) {
  group 'install'
  description 'install task'
  installObjectName 'test.jar'
  println "[phase:configuration] install"
  doFirst {
    println "[phase:execution] install:doFirst()"
  }
  doLast {
    println "[phase:execution] install:doLast()"
  }
}
install.dependsOn packaging
install.onlyIf { packaging.enabled }
liumiaocn:wrapper liumiao$gradle wrapper
通过gradle tasks可以确认到在Build Setup tasks中有wrapper这样一个内建的任务。
liumiaocn:wrapper liumiao$ gradle tasks
...省略
------------------------------------------------------------
All tasks runnable from root project
------------------------------------------------------------
Build Setup tasks
-----------------
init - Initializes a new Gradle build.
wrapper - Generates Gradle wrapper files.
...省略
BUILD SUCCESSFUL in 0s
1 actionable task: 1 executed
liumiaocn:wrapper liumiao$wrapper的说明是这个任务是用来创建gradle wrapper文件，接下里我们来看一下具体创建了那些文件
liumiaocn:wrapper liumiao$ ls
build.gradle
liumiaocn:wrapper liumiao$ gradle wrapper
> Configure project :
[phase:configuration] build.gradle ...
[phase:configuration] compile
[phase:configuration] test
[phase:configuration] packaging
[phase:configuration] install
BUILD SUCCESSFUL in 0s
1 actionable task: 1 executed
liumiaocn:wrapper liumiao$ ls
build.gradle gradle       gradlew      gradlew.bat
liumiaocn:wrapper liumiao$ tree
.
├── build.gradle
├── gradle
│   └── wrapper
│       ├── gradle-wrapper.jar
│       └── gradle-wrapper.properties
├── gradlew
└── gradlew.bat
2 directories, 5 files
liumiaocn:wrapper liumiao$文件说明：
gradlew：linux或者Unix下用于执行wrapper命令的Shell脚本
gradlew.bat：Windows下用于执行wrapper命令的批处理脚本
gradle-wrapper.jar：用于下载Gradle的相关代码实现
gradle-wrapper.properties：wrapper所使用的配置信息，比如gradle的版本等信息
缺省状况下的gradle-wrapper.properties的信息如下
liumiaocn:wrapper liumiao$ cat gradle/wrapper/gradle-wrapper.properties 
distributionBase=GRADLE_USER_HOME
distributionPath=wrapper/dists
distributionUrl=https\://services.gradle.org/distributions/gradle-4.10.2-bin.zip
zipStoreBase=GRADLE_USER_HOME
zipStorePath=wrapper/dists
liumiaocn:wrapper liumiao$GRADLE_USER_HOME可以通过环境变量的方式设定，缺省的GRADLE_USER_HOME为
$$
{HOME}/.gradle，根据这里的信息可以看到，下载的gradle将会解压到
$$
{HOME}/.gradle/wrapper/dists
使用场景
当前系统安装的gradle的版本为4.10.2，但是在当前的project里面希望使用5.0的gradle，这时gradle wrapper就可以一显身手了
事前确认
当前gradle的版本为4.10.2
liumiaocn:wrapper liumiao$ gradle -v
------------------------------------------------------------
Gradle 4.10.2
------------------------------------------------------------
Build time:   2018-09-19 18:10:15 UTC
Revision:     b4d8d5d170bb4ba516e88d7fe5647e2323d791dd
Kotlin DSL:   1.0-rc-6
Kotlin:       1.2.61
Groovy:       2.4.15
Ant:          Apache Ant(TM) version 1.9.11 compiled on March 23 2018
JVM:          1.8.0_191 (Oracle Corporation 25.191-b12)
OS:           Mac OS X 10.14 x86_64
liumiaocn:wrapper liumiao$设定wrapper使用的gradle版本
可以使用命令来修改wrapper所使用的gradle版本信息
liumiaocn:wrapper liumiao$ gradle wrapper --gradle-version 5.0-rc-5 --distribution-type bin
Starting a Gradle Daemon (subsequent builds will be faster)
> Configure project :
[phase:configuration] build.gradle ...
[phase:configuration] compile
[phase:configuration] test
[phase:configuration] packaging
[phase:configuration] install
BUILD SUCCESSFUL in 3s
1 actionable task: 1 executed
liumiaocn:wrapper liumiao$可以看到，实际这条命令只是修改了gradle wrapper的配置文件的下载文件的信息
liumiaocn:wrapper liumiao$ cat gradle/wrapper/gradle-wrapper.properties 
distributionBase=GRADLE_USER_HOME
distributionPath=wrapper/dists
distributionUrl=https\://services.gradle.org/distributions/gradle-5.0-rc-5-bin.zip
zipStoreBase=GRADLE_USER_HOME
zipStorePath=wrapper/dists
liumiaocn:wrapper liumiao$当然设定也可以通过gradle-distribution-url选项或者直接修改配置文件来更快实现，至于具体版本，gradle的安装文件包符合：
gradle-版本信息-bin|all.zip的格式，按照这种格式即可
![在这里插入图片描述](https://img-blog.csdnimg.cn/20181125055631933.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L2xpdW1pYW9jbg==,size_16,color_FFFFFF,t_70)
wrapper使用
只需要设定下载url所指定的gradle版本信息，wrapper的准备就已经就绪，随便使用任何一个wrapper的命令，第一次执行都会首先下载gradle并解压到${HOME}/.gradle/wrapper/dists，在在执行之前我们可以看到目前还没有任何内容
liumiaocn:wrapper liumiao$ ls
build.gradle gradle       gradlew      gradlew.bat
liumiaocn:wrapper liumiao$ ls ${HOME}/.gradle/wrapper/dists
ls: /Users/liumiao/.gradle/wrapper/dists: No such file or directory
liumiaocn:wrapper liumiao$gradle wrapper之所以有被叫做wrapper，就是因为封了一层，所以gradle的基本命令在这里都可以使用，这里直接使用上文确认gradle版本所用的-v
liumiaocn:wrapper liumiao$ ./gradlew -v
Downloading https://services.gradle.org/distributions/gradle-5.0-rc-5-bin.zip
.................................................................................
Welcome to Gradle 5.0-rc-5!
Here are the highlights of this release:
 - Kotlin DSL 1.0
 - Task timeouts
 - Dependency alignment aka BOM support
 - Interactive `gradle init`
For more details see https://docs.gradle.org/5.0-rc-5/release-notes.html

------------------------------------------------------------
Gradle 5.0-rc-5
------------------------------------------------------------
Build time:   2018-11-22 19:58:45 UTC
Revision:     42e59c2484a49702234030881640aa8d8bfa6d43
Kotlin DSL:   1.0.4
Kotlin:       1.3.10
Groovy:       2.5.4
Ant:          Apache Ant(TM) version 1.9.13 compiled on July 10 2018
JVM:          1.8.0_191 (Oracle Corporation 25.191-b12)
OS:           Mac OS X 10.14 x86_64
liumiaocn:wrapper liumiao$所以看到第一次执行有Downloading https://services.gradle.org/distributions/gradle-5.0-rc-5-bin.zip的动作，而具体的版本正是之前所设定的，gradle的下载和解压对我们来说都是透明的，为什么wrapper能找到这个5.0的版本呢
liumiaocn:wrapper liumiao$ ls ${HOME}/.gradle/wrapper/dists
gradle-5.0-rc-5-bin
liumiaocn:wrapper liumiao$可以看到，wrapper把它下载并解压到这里了，当然也可以使用系统已经安装的gradle，这种方式直接设定GRADLE_USER_HOME即可。而且下载和设定的动作只会发生一次，除非你手动再次将其删除，现在再次使用gradlew就会清晰的多了
liumiaocn:wrapper liumiao$ ./gradlew -v
------------------------------------------------------------
Gradle 5.0-rc-5
------------------------------------------------------------
Build time:   2018-11-22 19:58:45 UTC
Revision:     42e59c2484a49702234030881640aa8d8bfa6d43
Kotlin DSL:   1.0.4
Kotlin:       1.3.10
Groovy:       2.5.4
Ant:          Apache Ant(TM) version 1.9.13 compiled on July 10 2018
JVM:          1.8.0_191 (Oracle Corporation 25.191-b12)
OS:           Mac OS X 10.14 x86_64
liumiaocn:wrapper liumiao$接下来就可以想使用gradle一样使用gradlew了，比如需要执行install的任务
liumiaocn:wrapper liumiao$ ./gradlew install
Starting a Gradle Daemon (subsequent builds will be faster)
> Configure project :
[phase:configuration] build.gradle ...
[phase:configuration] compile
[phase:configuration] test
[phase:configuration] packaging
[phase:configuration] install
> Task :compile
[phase:execution] compile :doFirst()
> Task :test
[phase:execution] test:doLast()
> Task :packaging
[phase:execution] packaging:doLast()
> Task :install
[phase:execution] install:doFirst()
[phase:execution] install:installObject (test.jar)
[phase:execution] install:checkObject   (test.jar)
[phase:execution] install:doLast()
BUILD SUCCESSFUL in 3s
4 actionable tasks: 4 executed
liumiaocn:wrapper liumiao$可以看到同样的脚本在gradle 5中也能正常运行
总结
gradle wrapper对于较为熟悉设定用户来说，似乎更像一个鸡肋的功能，因为gradle本身的设定就非常简单，但是对于团队开发来说，这种事情如果能够坐等透明化和标准化对于项目来说自然是很不错的一个方式，也是我们在推行DevOps时对于开发环境标准化的一个小的实践。
参考内容
[https://docs.gradle.org/current/userguide/gradle_wrapper.html](https://docs.gradle.org/current/userguide/gradle_wrapper.html)

