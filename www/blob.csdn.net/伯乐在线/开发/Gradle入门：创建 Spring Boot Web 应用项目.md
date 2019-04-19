# Gradle入门：创建 Spring Boot Web 应用项目 - 文章 - 伯乐在线
本文由 [伯乐在线](http://blog.jobbole.com) - [JustinWu](http://www.jobbole.com/members/mybreeze77) 翻译，[Panblack](http://www.jobbole.com/members/upanblack) 校稿。未经许可，禁止转载！
英文出处：[Petri Kainulainen](http://www.petrikainulainen.net/programming/gradle/getting-started-with-gradle-creating-a-spring-boot-web-application-project/)。欢迎加入[翻译组](https://github.com/jobbole/translation-project)。
在一台远程服务器上运行Spring Web应用程序的传统做法是，将其打包为war文件并部署到servlet容器中。
虽然在过去这个方法很好很强大，然而要同时管理多个servlet容器总是有些繁琐。
Spring Boot针对该问题提供了一种解决方案，它允许我们将web应用程序打包为一个可执行的jar文件，这个文件可以使用嵌入式的servlet容器。
这篇博文将描述如何创建一个满足以下要求的Spring Boot web应用程序：
- Spring Boot应用程序必须使用Thymeleaf作为模版引擎。
- Spring Boot应用程序必须提供一种对其进行监控的方式。
- Gradle项目必须拥有独立的源文件和资源文件目录，以便进行单元和集成测试。
我们开始吧。
## 创建Java项目
由于我们需要创建的是一个Java项目，所以必须使用Java插件。通过以下步骤可以完成：
1.应用Gradle的Java插件。
2.设置Java源码的版本为1.8。
3.配置Gradle生成Java1.8的class文件。
*build.gradle*文件如下：

Java
```
apply plugin: 'java'
sourceCompatibility = 1.8
targetCompatibility = 1.8
```
Java插件会在我们的构建中添加新的约定（如默认的目录结构）、任务和属性集。如果你想了解更多内容，可以阅读以下博文：
[Gradle入门系列（2）：第一个Java项目](http://blog.jobbole.com/72558/)
我们继续，下面是如何在Gradle构建中添加集成测试。
## 在Gradle构建中添加集成测试
我们可以使用[Gradle测试集插件](https://github.com/unbroken-dome/gradle-testsets-plugin)（TestSets plugin）在构建中添加集成测试。由于我已经写过一篇描述如何使用该插件的[教程](http://www.petrikainulainen.net/programming/gradle/getting-started-with-gradle-integration-testing-with-the-testsets-plugin/)了，所以在这篇文章中我就不再赘述其配置了。
在实现了“[此文](http://www.petrikainulainen.net/programming/gradle/getting-started-with-gradle-integration-testing/#integration-test-requirements)”所提到的需求以后，*build.gradle *文件如下：

Java
```
buildscript {
    repositories {
        jcenter()
    }
    dependencies {
        classpath(
                'org.unbroken-dome.gradle-plugins:gradle-testsets-plugin:1.0.2'
        )
    }
}
apply plugin: 'java'
apply plugin: 'org.unbroken-dome.test-sets'
sourceCompatibility = 1.8
targetCompatibility = 1.8
testSets {
    integrationTest { dirName = 'integration-test' }
}
project.integrationTest {
    outputs.upToDateWhen { false }
}
check.dependsOn integrationTest
integrationTest.mustRunAfter test
tasks.withType(Test) {
    reports.html.destination = file("${reporting.baseDir}/${name}")
}
```
**扩展阅读**- [Getting Started With Gradle: Integration Testing With the TestSets Plugin](http://www.petrikainulainen.net/programming/gradle/getting-started-with-gradle-integration-testing-with-the-testsets-plugin/)
我们继续，下面将谈到如何在Gradle项目中添加Spring Boot的支持。
## 在Gradle项目中添加Spring Boot的支持
我们可以使用[Spring Boot Gradle插件](http://docs.spring.io/spring-boot/docs/1.2.x/reference/htmlsingle/#build-tool-plugins-gradle-plugin)在Gradle项目中添加Spring Boot的支持。通过以下步骤使用该插件：
1. 在构建脚本的classpath中添加Spring Boot Gradle插件（版本：1.2.5.RELEASE）。
2. 应用Spring Boot Gradle插件。
*build.gradle*文件的源代码如下：

Java
```
buildscript {
    repositories {
        jcenter()
    }
    dependencies {
        classpath(
                'org.springframework.boot:spring-boot-gradle-plugin:1.2.5.RELEASE',
                'org.unbroken-dome.gradle-plugins:gradle-testsets-plugin:1.0.2'
        )
    }
}
apply plugin: 'java'
apply plugin: 'org.unbroken-dome.test-sets'
apply plugin: 'spring-boot'
sourceCompatibility = 1.8
targetCompatibility = 1.8
testSets {
    integrationTest { dirName = 'integration-test' }
}
project.integrationTest {
    outputs.upToDateWhen { false }
}
check.dependsOn integrationTest
integrationTest.mustRunAfter test
tasks.withType(Test) {
    reports.html.destination = file("${reporting.baseDir}/${name}")
}
```
**扩展阅读**- [Spring Boot Reference Guide: 59.1 Including the plugin](http://docs.spring.io/spring-boot/docs/1.2.x/reference/htmlsingle/#build-tool-plugins-including-the-gradle-plugin)
我们无须使用[Bintray的Jcenter Maven仓库](http://jcenter.bintray.com/)，但是由于[Gradle测试集插件](http://www.petrikainulainen.net/programming/gradle/getting-started-with-gradle-integration-testing-with-the-testsets-plugin/)依赖于该仓库，因此本文中的演示程序也将其加入。
在应用Spring Boot Gradle插件后，我们可以：
- 将应用程序打包为可执行的jar文件。
- 使用*bootrun*任务运行程序。
- 省略Spring Boot依赖的版本信息。
- 将应用程序打包为war文件。
当然，我们也可以对Spring Boot Gradle插件进行配置，并自定义执行和打包应用程序的任务。
**扩展阅读**- [Spring Boot Reference Guide: 59.5 Running a project in-place](http://docs.spring.io/spring-boot/docs/1.2.x/reference/htmlsingle/#build-tool-plugins-gradle-running-applications)
- [Spring Boot Reference Guide: 59.6 Spring Boot plugin configuration](http://docs.spring.io/spring-boot/docs/1.2.x/reference/htmlsingle/#build-tool-plugins-gradle-global-configuration)
- [Spring Boot Reference Guide: 59.7 Repackage configuration](http://docs.spring.io/spring-boot/docs/1.2.x/reference/htmlsingle/#build-tool-plugins-gradle-repackage-configuration)
- [Spring Boot Reference Guide: 59.8 Repackage with custom Gradle configuration](http://docs.spring.io/spring-boot/docs/1.2.x/reference/htmlsingle/#build-tool-plugins-gradle-repackage-custom-configuration)
我们继续，下面是如何通过Gradle获取所需的依赖项。
## 获取所需的依赖项
我们可以通过所谓的starter POM来获取Spring Boot应用的依赖。[Spring Boot的参考指南将starter POM描述](http://docs.spring.io/spring-boot/docs/1.2.x/reference/htmlsingle/#using-boot-starter-poms)如下：
> 
starter POM是一族可以被包含到项目中的便捷依赖描述符。你可以一站式的获取所有需要的Spring和相关技术，无需苦苦寻找演示代码，也无需复制粘贴大量的依赖描述符。
换句话说，我们只需选择正确的starter POM，并将其加入到Gradle构建中即可。
我们可以通过以下步骤获取所需的依赖：
- 确保所有的依赖都从[Maven2的中央仓库](http://repo1.maven.org/maven2/)获取。
- 在*compile*配置里添加*spring-boot-starter-actuator*依赖，我们之所以需要这个依赖，是因为它提供了一种监控应用运行状态的方法。
- 在*compile*配置里添加*spring-boot-starter-thymeleaf*依赖，我们之所以需要该依赖，是因为我们需要使用Thymeleaf作为创建Web应用的模版引擎。
- 在*testCompile*配置里添加*spring-boot-starter-test*依赖，我们之所以需要该依赖，是因为我们需要在Web应用中编写单元测试和集成测试。
*build.gradle*的源代码如下：

Java
```
buildscript {
    repositories {
        jcenter()
    }
    dependencies {
        classpath(
                'org.springframework.boot:spring-boot-gradle-plugin:1.2.5.RELEASE',
                'org.unbroken-dome.gradle-plugins:gradle-testsets-plugin:1.0.2'
        )
    }
}
apply plugin: 'java'
apply plugin: 'org.unbroken-dome.test-sets'
apply plugin: 'spring-boot'
sourceCompatibility = 1.8
targetCompatibility = 1.8
repositories {
    mavenCentral()
}
dependencies {
    compile(
            'org.springframework.boot:spring-boot-starter-actuator',
            'org.springframework.boot:spring-boot-starter-thymeleaf'
    )
    testCompile('org.springframework.boot:spring-boot-starter-test')
}
testSets {
    integrationTest { dirName = 'integration-test' }
}
project.integrationTest {
    outputs.upToDateWhen { false }
}
check.dependsOn integrationTest
integrationTest.mustRunAfter test
tasks.withType(Test) {
    reports.html.destination = file("${reporting.baseDir}/${name}")
}
```
我们不需要设置Spring Boot依赖的版本，因为Spring Boot Gradle插件能够决定这些依赖的版本。换句话说，我们可以通过设置Spring Boot Gradle插件的版本来选择偏好的Spring Boot的版本。
**扩展阅读**- [Getting Started With Gradle: Dependency Management](http://www.petrikainulainen.net/programming/gradle/getting-started-with-gradle-dependency-management/)
- [Spring Boot Reference Guide: 13.4 Starter POMs](http://docs.spring.io/spring-boot/docs/1.2.x/reference/htmlsingle/#using-boot-starter-poms)
- [Spring Boot Reference Guide: 59.2 Declaring dependencies without versions](http://docs.spring.io/spring-boot/docs/1.2.x/reference/htmlsingle/#build-tool-plugins-gradle-dependencies-without-versions)
- [Spring Boot Reference Guide: Appendix E. Dependency versions](http://docs.spring.io/spring-boot/docs/1.2.x/reference/htmlsingle/#appendix-dependency-versions)
- [Spring Boot Reference Guide: Part V. Spring Boot Actuator: Production-ready features](http://docs.spring.io/spring-boot/docs/1.2.x/reference/htmlsingle/#production-ready)
我们继续，下面是如何运行Spring Boot应用程序。
## 运行Spring Boot 应用程序
我们可以使用以下任一方法，运行Spring Boot应用程序：
**首先**，可以使用Spring Boot Gradle插件中的*bootRun*任务运行应用程序，而无需创建jar文件。我们应当在开发阶段使用这个方法，因为它可以使我们静态的classpath资源（即：在*src/main/resources*下的文件）都成为可重载的资源。
换句话说，如果我们使用这个方法，就可以在Spring Boot应用程序运行时对这些文件进行更改，而且可以在不重启应用的情况下观察到变化。
通过在命令提示符中输入以下命令，就可以使用该方法了。

Java
```
gradle clean bootRun
```
**扩展阅读**- [Spring Boot Reference Guide: 59.5 Running a project in-place](http://docs.spring.io/spring-boot/docs/1.2.x/reference/htmlsingle/#build-tool-plugins-gradle-running-applications)
**其次**，我们可以将应用程序打包为一个可执行的jar文件，继而执行所创建的文件。如果想要在一台远程服务器上运行Spring Boot应用，应当采用这种方法。
通过在命令提示符中输入以下命令，就可以创建一个可执行的jar文件了。

Java
```
gradle clean build
```
这条命令会在*build/libs*目录下创建*spring-boot-web-application.jar*文件。在将其复制到远程服务器上后，可以通过以下命令运行应用程序。

Java
```
java -jar spring-boot-web-application.jar
```
**扩展阅读**- [Spring Boot Reference Guide: 59.4 Packaging executable jar and war files](http://docs.spring.io/spring-boot/docs/1.2.x/reference/htmlsingle/#build-tool-plugins-gradle-packaging)
我们继续来总结一下从这篇教程中的所学所得。
## 总结
这篇博文教会了我们以下四点：
- 我们可以使用Spring Boot Gradle插件在Gradle项目中添加Spring Boot的支持。
- 我们可以通过设置Spring Boot Gradle插件的版本来选择偏爱的Spring Boot的版本。这意味着我们不再需要设置每个Spring Boot依赖项的版本。
- 如果我们想要在开发环境运行Spring Boot应用程序，应当使用Spring Boot Gradle插件中的*bootRun*任务。
- 如果我们想要在远程服务器上运行Spring Boot应用程序，应当将其打包为可执行的jar文件，并复制到远程服务器上，运行即可。
P.S. 你可以[从Github上获取本文的演示程序](https://github.com/pkainulainen/gradle-examples/tree/master/spring-boot-web-application)。
