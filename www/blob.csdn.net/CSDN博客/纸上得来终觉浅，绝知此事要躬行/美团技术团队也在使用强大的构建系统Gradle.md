# 美团技术团队也在使用强大的构建系统Gradle - 纸上得来终觉浅，绝知此事要躬行 - CSDN博客





2016年03月17日 13:06:56[boonya](https://me.csdn.net/boonya)阅读数：2028








## 前言

构建，软件生命周期中重要的一环，在现代软件开发过程中，起着越来越重要的作用。过去在Java或类Java的世界里，[Ant](http://ant.apache.org)、[Maven](http://maven.apache.org)再熟悉不过了，Maven凭借其强大的依赖配置战胜Ant，基本上成为了Java构建的标准。而在现代，系统日益复杂，构建的灵活性要求越来越高，比如：构建过程中需要打包上传到服务器，Maven无法很好地支持这种复杂的系统构建，所以，我选择了[Gradle](http://www.gradle.org)，一个基于[Groovy](http://groovy.codehaus.org)，更灵活更强大的构建系统，能帮助我们构建更复杂的项目。

## 为什么选择Gradle

#### 从框架方向来看：
- Gradle是很成熟的技术，可以处理大规模构建
- Gradle对多语言、多平台有更natural的支持
- Gradle关注在构建效率上
- Gradle发布很频繁，重要feature开发计划透明化
- Gradle社区很活跃，并且增加迅速

#### 从语言特性来看：
- 代码很精简
- Gradle基于Groovy，能完成复杂系统的构建任务
- [DSL](http://en.wikipedia.org/wiki/Domain-specific_language)比XML更简洁高效

#### Gradle在开源项目中的使用

现在使用Gradle构建的开源项目很多，我有过接触的比如：[Grails](https://grails.org/),[Griffon](http://griffon.codehaus.org/),[Groovy](http://groovy.codehaus.org),[Hibernate](http://hibernate.org/),[Spring](http://spring.io/)

还有很多其它开源项目也都在用Gradle，比如[Tapestry](http://tapestry.apache.org/)，[Qi4J](http://qi4j.org/)，[Netflix](https://github.com/Netflix)下所有开源项目（python、c++、html等除外）等等。

#### Gradle在企业中的使用

现在使用Gradle来做构建体系的公司也越来越多，linkedin就很早开始切换到Gradle。

## Gradle体验

Gradle的安装非常方便，下载ZIP包，解压到本地目录，设置 GRADLE_HOME 环境变量并将 GRADLE_HOME/bin 加到 PATH 环境变量中，安装就完成了。用户可以运行gradle -v命令验证安装，这些初始的步骤和Maven没什么两样。我这里安装的Gradle版本是1.10，详细信息见下：

```
bob [10:42]  ᐅ gradle -v

------------------------------------------------------------
Gradle 1.10
------------------------------------------------------------
Build time:   2013-12-17 09:28:15 UTC
Build number: none
Revision:     36ced393628875ff15575fa03d16c1349ffe8bb6
Groovy:       1.8.6
Ant:          Apache Ant(TM) version 1.9.2 compiled on July 8 2013
Ivy:          2.2.0
JVM:          1.7.0_45 (Oracle Corporation 24.45-b08)
OS:           Mac OS X 10.9.2 x86_64
```

Gradle的Features很多，[官网doc](http://www.gradle.org/docs/current/userguide/overview.html#sec:special_feature_of_gradle)介绍很详细，我这里就不多说。下面简单介绍一下Gradle构建相关的东西。

### Gradle基础

1，Gradle有两个最基本的概念：project和task。Gradle里面的所有东西都基于这两个概念。project通常指一个项目，而task指构建过程中的任务。一次构建可以有1到n个project，每个project有1到n个task。

2，Gradle有一个类似Maven中pom.xml的配置文件：build.gradle。功能也基本一样，负责当前project的构建定义。看一个build.gradle的简单例子：

```
bob [10:46]  ᐅ pwd
/Users/bob/framework/gradle-1.10/samples/userguide/tutorial/hello
// 在你安装的gradle根目录下有对应的samples目录，里面有很多例子

bob [10:46]  ᐅ cat build.gradle
task hello {
    doLast {
        println 'Hello world!'
    }
}
```

文件中定义了一个task：hello，task的内容是 "println 'Hello world!'"，我们来执行一下：

```
bob [10:49]  ᐅ gradle -q hello
Hello world!
```

可以看到，输出了"Hello world!"，这里-q的意思是quiet模式，只输出构建中的必要信息。

gradle里可以定义多个task，task之间也可以有依赖关系，还可以定义默认task，看一个例子：

带有task依赖关系：

```
bob [10:53]  ᐅ cat userguide/tutorial/lazyDependsOn/build.gradle
task taskX(dependsOn: 'taskY') << {
    println 'taskX'
}
task taskY << {
    println 'taskY'
}
```

带有默认task例子：

```
bob [10:59]  ᐅ cat userguide/tutorial/defaultTasks/build.gradle
defaultTasks 'clean', 'run'


task clean << {
    println 'Default Cleaning!'
}

task run << {
    println 'Default Running!'
}

task other << {
    println "I'm not a default task!"
}
```

看看执行情况：

```
bob [10:59]  ᐅ gradle -q
Default Cleaning!
Default Running!

bob [11:00]  ᐅ gradle -q other
I'm not a default task!
```

默认task，当没有task指定时，则会执行默认的task。

### Gradle依赖

Gradle和Maven在依赖管理上几乎差不多，核心的概念是一样的，只不过Gradle语法更精简，并且多了一些更灵活的自定义配置。我们先看一个例子，Maven的pom.xml：

```
<dependencies>
        <dependency>
            <groupId>org.springframework</groupId>
            <artifactId>spring-core</artifactId>
        </dependency>
        <dependency>
            <groupId>org.springframework</groupId>
            <artifactId>spring-beans</artifactId>
        </dependency>
        <dependency>
            <groupId>org.springframework</groupId>
            <artifactId>spring-context</artifactId>
        </dependency>
        <dependency>
            <groupId>junit</groupId>
            <artifactId>junit</artifactId>
        </dependency>
    </dependencies>
```

更换成Gradle脚本，结果是这样：

```
dependencies {
    compile('org.springframework:spring-core:3.2.4.RELEASE')
    compile('org.springframework:spring-beans:3.2.4.RELEASE')
    compile('org.springframework:spring-context:3.2.4.RELEASE')
    testCompile('junit:junit:4.7')
}
```

代码块少了很多。试想，生产环境下的中、大型应用如果用都用Gradle替换Maven，那势必会大大减少配置文件代码块，并有更强的可读性，也就意味着系统更加稳健。

1，Gradle在依赖配置上面，和Maven一样，支持传递性依赖，然后和Maven不同的是，它还支持排除传递性依赖以及关闭传递性依赖。

2，Gradle的依赖scope，也基本和Maven一样，不过它是通过配置来定义，plugin来支撑和加强的，所以除了基本的compile、runtime等scope外，Gradle还可以自定义出很多配置，针对不同的配置写不同的task来完成更复杂更灵活的构建任务。

依赖相关的仓库配置很灵活，支持多种repository，看下面repository定义例子：

```
bob [11:07]  ᐅ cat userguide/artifacts/defineRepository/build.gradle

repositories {
    mavenCentral()          // 定义仓库为maven中心仓库
}
repositories {
    jcenter()               // 定义仓库为jcenter仓库
}
repositories {
    maven {
        url "http://repo.mycompany.com/maven2"      // 定义依赖包协议是maven，地址是公司的仓库地址
    }
}
repositories {                              // 定义本地仓库目录
    flatDir {
        dirs 'lib'
    }
}
repositories {                              // 定义ivy协议类型的仓库
    ivy {
        url "http://repo.mycompany.com/repo"
    }
}
```

可以看到，对于常用的maven、ivy、local以及jcenter的repository都有支持，语法很简单。而且还可以通过编写task来支持更复杂的repository，更多详情可以查看安装包里的对应目录下文件查看。

### Gradle构建

和Maven一样，Gradle也是通过artifact来打包构建的。得益于上述的Gradle本身的特性，artifact在Gradle里实现得更灵活一些。看一个例子：

```
bob [13:00]  ᐅ cat userguide/artifacts/uploading/build.gradle

## jar类型的artifact
task myJar(type: Jar)
artifacts {
    archives myJar
}
## file类型的artifact
def someFile = file('build/somefile.txt')
artifacts {
    archives someFile
}

## 根据自定义task来完成artifact
task myTask(type:  MyTaskType) {
    destFile = file('build/somefile.txt')
}
artifacts {
    archives(myTask.destFile) {
        name 'my-artifact'
        type 'text'
        builtBy myTask
    }
}

## 根据自定义task来完成artifact
task generate(type:  MyTaskType) {
    destFile = file('build/somefile.txt')
}
artifacts {
    archives file: generate.destFile, name: 'my-artifact', type: 'text', builtBy: generate
}
```

这样就简单地定义了好几种artifact生成的定义，根据不同的场景需求，生成文本文件、jar包或者zip，还可以再上传到服务器上。

一般情况下，常用的插件，比如说"Java plugin"都默认定义了"jar"这样的artifact task，所以一般不需要额外开发。但是，针对于一些复杂情况，或者在plugin基础上增强的话，自定义artifact task还是非常有用的。

Gradle构建的项目，发布到仓库中，也非常容易：

```
apply plugin: 'maven'

uploadArchives {
    repositories {
        ivy {
            credentials {
                username "username"
                password "pw"
            }
            url "http://repo.mycompany.com"
        }
    }
}
```

### Gradle 插件

上面简介介绍了一下Gradle的一些概念和配置，要用到项目中run起来，现在还还要一步，就是本节介绍的Gradle插件。Gradle现在已经支持很多插件，这给开发者带来极大的便利，先说说Java插件吧。

1，使用Java plugin，只需要在build.gradle中加入这句话：
`apply plugin: 'java'`
2，了解或设置Java project布局。Gradle和Maven一样，采用了“约定优于配置”的方式对Java project布局，并且布局方式是和Maven一样的，此外，Gradle还可以方便的自定义布局。在Gradle中，一般把这些目录叫做source set。看下官方的答案：

![gradle source set](http://tech.meituan.com/img/gradle/source_set.png)

这里要注意，每个plugin的source set可能都不一样。

同样的，Java plugin还定义好了一堆task，让我们可以直接使用，比如：clean、test、build等等。这些task都是围绕着Java plugin的构建生命周期的：
![javaPluginTasks](http://tech.meituan.com/img/gradle/javaPluginTasks.png)

图中每一块都是一个task，箭头表示task执行顺序/依赖，比如执行task jar，那么必须先执行task compileJava和task processResources。另外可以看到，Gradle的Java plugin构建生命周期比较复杂，但是也表明了更加灵活，而且，在项目中，一般只使用其中常用的几个：clean test check build 等等。

gradle构建过程中，所有的依赖都表现为配置，比如说系统运行时的依赖是runtime，gradle里有一个依赖配置叫runtime，那么系统运行时会加载这个依赖配置以及它的相关依赖。这里说的有点绕，可以简单理解依赖和maven类似，只不过gradle用configuration实现，所以更灵活，有更多选择。下图是依赖配置关系图以及和task调用的关系图：

![javaPluginConfigurations](http://tech.meituan.com/img/gradle/javaPluginConfigurations.png)

可以看到，基本和Maven是一样的。其实Gradle里面这些依赖(scope)都是通过configuration来实现的，这里就不细说，有兴趣的可以研究一下官方资料。

关于“约定优于配置”，还有很多东西，这里不细说，官方doc已经说的很详细了。

### Gradle 其它不错的特性

1，所有声明都是一等公民

2，多project构建

3，引用外部/通用构建脚本

4，Gradle wrapper

### 小结

1，Gradle非常简洁，项目本身的配置代码非常少。

2，Gradle在外部project构建也支持很好，整体构建简单，并且通过公用外部构建脚本，让配置内容尽量没有冗余。

3，Gradle很灵活，可以方面的增加和修改构建过程。而Maven却需要开发插件来支持。

4，Gradle是基于Groovy的，也就是说配置中可以编写自定义代码，能适应更复杂的场景，能完成更强大的功能，比如说：自动上传、分发、部署等等。

## 项目实战

Gradle介绍了那么多，可以看出，gradle是非常灵活的，可以适应各种复杂环境。建议各位从架构角度考虑gradle构建，而不仅仅把它当作一个构建工具。下面来说说我们实际项目中的Gradle改造工作。

### 背景：

我们的项目经过一个半Q的迅速发展，整个项目已经由1个简易后台变成4个系统＋若干脚本任务了，项目中存在很多冗余代码和重复配置。我们使用上面介绍的方法对项目进行了改造，以解决这两个问题。

### 步骤：

要解决冗余代码和通用配置的问题，最简单的做法就是抽取出共同部分，作为其它所有项目的parent/common项目。方法：

### 1，使用git submodule

将所有系统中公共的类库和通用的配置，放到独立的仓库Common中。因为我们用git来管理代码，而git本身提倡多branch，多仓库，所以采用git submodule方式，其它项目需要添加Common这个submodule：
`git submodule add yourGitRepo deps/Common`
最后的"deps/Common"是自定义的，意思就是在当前的deps目录下用Common名字来当作submodule的clone。

如果你clone别的带有submodule的项目时，默认情况下，当前的project并不会把submodule的代码都clone下来，可以执行：
`git submodule foreach git pull`
以下这段一般大家经常会遇到：

当你clone项目时，submodule会以最新的master分支上的commit id作为本次的tag下载，类似一个副本，因为一般大家都是用submodule，而不是修改它。所以当你的submodule需要更新的时候，需要先执行这段代码：
`git submodule foreach git checkout master`
让submodule切换到master分支了，然后就可以用上面的submodule pull来更新了。

### 2，gradle构建：

鉴于上文对gradle优点的描述，我们采用gradle来构建。我们的项目最初都是基于maven来构建的，从maven切换到gradle很简单，在项目根目录下，先执行（假设你的机器已经安装了gradle环境，一般负责构建的人首次需要安装，开发人员可以不安装）：
`gradle init wrapper`
这样，就会自动生成相关的gradlew，build.gradle，settings.gradle等文件和相关目录，并会自动下载对应版本的gradle binary包（所以以后不需要安装）。Gradle会自动识别Maven里的配置，并相应的导入进来，有少量部分配置可能需要修改。

注：在已有的gradle项目里，尽量使用生成的gradlew这个wrapper，因为它会自动下载对应版本的Gradle，也就是说团队合作的其他人开发机上是不需要手动安装Gradle的，并且wrapper也让大家的Gradle版本一致，避免问题。

### 3，gradle脚本修改

上面执行完之后，环境已经准备好了，现在要做的就是修改构建脚本：

因为已经通过git submodule把公共项目放到独立目录（deps/Common）了，并且它本身也是独立可构建的项目，那么也就是说当前有两个项目了，一个是当前project，一个是Common项目，要做的就是告诉gradle，要多项目构建，编辑settings.gradle，增加项目配置：
`include "deps:Common"`
以上就是把Common引入到当前项目了。

根据项目的不同，然后对应修改build.gradle，就大功告成了。看一个例子：

```
// 这一段主要是把公共库Common的构建脚本引入，因为一般会有通用的配置在里面
def userGradleScript = file("deps/Common/build.gradle")
if (userGradleScript.exists()) {
    apply from: userGradleScript
}
// 使用war插件，这样就默认引入了java插件
apply plugin: 'war'
// for jetty
apply plugin: 'jetty'
stopKey = 'yourStopKey' // 自定义的stopkey
stopPort = xxxx     // 停止端口
httpPort = xxxx     // 启动http端口

// 项目属性
group = 'yourApp'
version = '1.0.0'
description = """这里描述你的项目"""


// checkstyle config文件地址
checkstyle {
    configFile = file("deps/Common/config/checkstyle/checkstyle.xml")
}
// lib依赖
dependencies {
    // 依赖公共库Common，compile是和maven里的compile scope一样
    compile project(':deps:Common')
    compile 'commons-validator:commons-validator:1.4.0'
    compile('javax.servlet.jsp.jstl:jstl-api:1.2') {
        exclude(module: 'servlet-api')      // 防止版本冲突
    }
    compile 'javax.persistence:persistence-api:1.0.2'
    runtime 'mysql:mysql-connector-java:5.1.26'
    providedCompile 'org.apache.tomcat:tomcat-servlet-api:7.0.30'
    // providedCompile 这个conf在java插件里是报错的，war里是正确的
    providedCompile 'javax.servlet.jsp:jsp-api:2.1'
    ...
}
```

我们再来简单看下公共项目Common的构建脚本：

```
// 定义一堆基础插件
apply plugin: 'java'
apply plugin: 'maven'
apply plugin: "jacoco"
apply plugin: 'checkstyle'
apply plugin: 'pmd'
apply plugin: 'findbugs'
apply plugin: 'eclipse'
apply plugin: 'idea'
// 定义项目属性
group = 'Common'
version = '1.0.0'
description = """Giant common library"""

// 定义依赖仓库
repositories {
    mavenCentral()
}
// project的额外属性，这里用于定义profile属性，模拟maven的profile
ext {
    if (project.hasProperty('profile')) {
        profile = project['profile']
    } else {
        profile = "dev"
    }
    println "profile:" + profile
}
// 额外增加source path
sourceSets {
    main {
        resources {
            srcDir "src/main/profiles/${profile}"
        }
    }
}
// project依赖
dependencies {
    compile 'ch.qos.logback:logback-core:1.0.13'
    compile 'ch.qos.logback:logback-classic:1.0.13'
    compile 'ch.qos.logback:logback-access:1.0.13'
    compile 'commons-io:commons-io:2.0.1'
    compile 'commons-lang:commons-lang:2.6'
    compile 'joda-time:joda-time:1.6.2'
    compile 'org.testng:testng:6.8.7'
    compile 'com.googlecode.jmockit:jmockit:1.5'
    ...
}
// task配置
checkstyle {
    ignoreFailures = true
    sourceSets = [sourceSets.main]
}
findbugs {
    ignoreFailures = true
    sourceSets = [sourceSets.main]
}
pmd {
    ruleSets = ["basic", "braces", "design"]
    ignoreFailures = true
    sourceSets = [sourceSets.main]
}
jacocoTestReport {
    reports {
        xml.enabled true
        html.enabled true
        csv.enabled false
    }
    sourceSets sourceSets.main
}
tasks.withType(Compile) {
    options.encoding = "UTF-8"
}
test {
    useTestNG()
    jacoco {
        excludes = ["org.*"]
    }
}
```

这样，就可以在公共项目里配置好一堆基础的task，dependencies等等，而使用这个公共项目的其它项目则可以直接使用，无需再额外配置。

### 4，run

脚本修改完了，就可以开始构建了（不需要安装gradle，直接使用生成的gradlew就行）：

```
./gradlew build

// 基于profile构建
./gradlew -Pprofile=dev build
```

常用构建命令：

clean：清除之前的构建

test：执行测试

compileJava：编译java

check：在test之后做一个check，一般代码检查插件，都是在这个阶段做的

build：构建打包

## 总结

随着公司业务的发展，软件系统变得日益复杂和庞大，这就要求有更灵活、更高效的构建系统来支撑。现代构建系统Gradle提供了强大的功能、简洁的语法、灵活的配置，能适应各种复杂的构建环境。利用多project构建，让整个系统模块化，管理更高效。

点赞

[系统](http://ju.outofmemory.cn/tag/%E7%B3%BB%E7%BB%9F/)[gradle](http://ju.outofmemory.cn/tag/gradle/)
作者：[美团技术团队](http://ju.outofmemory.cn/feed/1644/)
Web bulid Meituan 



