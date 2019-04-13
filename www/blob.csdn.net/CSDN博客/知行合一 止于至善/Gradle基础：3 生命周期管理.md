
# Gradle基础：3:生命周期管理 - 知行合一 止于至善 - CSDN博客

2018年11月20日 05:53:03[liumiaocn](https://me.csdn.net/liumiaocn)阅读数：1654


Maven中的生命周期的管理使用了COC，以此为中心的pom.xml文件成为了重中之重，优点是不同项目之间的经验共享变得更加容易，大家大部分都是可以使用类似的套路，缺点则是灵活性稍微降低以及对于pom.xml细节的学习需要较多时间。Gradle则将这些再次放开，给更多的权限与开发者，这篇文章来简单看一下Gradle生命周期管理的一些基础知识。
# gradle构建的阶段
gradle的构建分为初期化/配置/执行三个阶段：

初期化:initialization
配置:configuration
执行:execution

# 构建的生命周期
在Maven中大部分项目的生命周期基本使用pom规范中定义的那些即可，而在Gradle中，可以通过Task的定义来定制项目自己所需要的方式。这里将会把前面的HelloWorld的例子进行简单的修改，模拟常见的maven项目经常会用到的几个阶段：

编译: compile
测试:test
打包:packaging
安装:install

## 代码示例
settings.gradle
`liumiaocn:hello liumiao$ cat settings.gradle 
println "[Phase: initialization] : settings executed... "
rootProject.name='helloPorject'
liumiaocn:hello liumiao$`build.gradle
在gradle中上述的四个阶段，可以使用如下4个task来进行模拟:
liumiaocn:hello liumiao$ cat build.gradle 
println "[phase:configuration] build.gradle ..."
task compile() {
  group 'customized'
  description 'compile task'
  println "[phase:configuration] compile"
  doFirst {
    println "[phase:execution] compile :doFirst()"
  }
}
task test {
  group 'customized'
  description 'test task'
  println "[phase:configuration] test"
  doLast {
    println "[phase:execution] test:doLast()"
  }
}
task packaging {
  group 'customized'
  description 'packaging task'
  println "[phase:configuration] packaging"
  doLast {
    println "[phase:execution] packaging:doLast()"
  }
}
task install {
  group 'customized'
  description 'install task'
  println "[phase:configuration] install"
  doFirst {
    println "[phase:execution] install:doFirst()"
  }
  doLast {
    println "[phase:execution] install:doLast()"
  }
}
liumiaocn:hello liumiao$执行确认
确认task
liumiaocn:hello liumiao$ gradle tasks
[Phase: initialization] : settings executed... 
> Configure project :
[phase:configuration] build.gradle ...
[phase:configuration] compile
[phase:configuration] test
[phase:configuration] packaging
[phase:configuration] install
> Task :tasks
------------------------------------------------------------
All tasks runnable from root project
------------------------------------------------------------
Build Setup tasks
-----------------
init - Initializes a new Gradle build.
wrapper - Generates Gradle wrapper files.
Customized tasks
----------------
compile - compile task
install - install task
packaging - packaging task
test - test task
Help tasks
----------
buildEnvironment - Displays all buildscript dependencies declared in root project 'helloPorject'.
components - Displays the components produced by root project 'helloPorject'. [incubating]
dependencies - Displays all dependencies declared in root project 'helloPorject'.
dependencyInsight - Displays the insight into a specific dependency in root project 'helloPorject'.
dependentComponents - Displays the dependent components of components in root project 'helloPorject'. [incubating]
help - Displays a help message.
model - Displays the configuration model of root project 'helloPorject'. [incubating]
projects - Displays the sub-projects of root project 'helloPorject'.
properties - Displays the properties of root project 'helloPorject'.
tasks - Displays the tasks runnable from root project 'helloPorject'.
To see all tasks and more detail, run gradle tasks --all
To see more detail about a task, run gradle help --task <task>
BUILD SUCCESSFUL in 0s
1 actionable task: 1 executed
liumiaocn:hello liumiao$确认执行
需要注意的上文所讲到的生命周期，通过对任务的执行可以看地清清楚楚，无论对哪个任务进行执行，初始化和配置部分的代码都会被执行，这点是在使用gradle时需要特别注意的。gradle的groovy或者kotlin的代码在什么阶段会被执行，一定要理解和注意。
liumiaocn:hello liumiao$ gradle compile
[Phase: initialization] : settings executed... 
> Configure project :
[phase:configuration] build.gradle ...
[phase:configuration] compile
[phase:configuration] test
[phase:configuration] packaging
[phase:configuration] install
> Task :compile
[phase:execution] compile :doFirst()
BUILD SUCCESSFUL in 0s
1 actionable task: 1 executed
liumiaocn:hello liumiao$doFrist与doLast
doFirst和doLast都是gradle预留的回调，在执行阶段会被调用，两者的顺序是doFirst() -> doLast()
doFirst
doLast
liumiaocn:hello liumiao$ gradle install
[Phase: initialization] : settings executed... 
> Configure project :
[phase:configuration] build.gradle ...
[phase:configuration] compile
[phase:configuration] test
[phase:configuration] packaging
[phase:configuration] install
> Task :install
[phase:execution] install:doFirst()
[phase:execution] install:doLast()
BUILD SUCCESSFUL in 0s
1 actionable task: 1 executed
liumiaocn:hello liumiao$多个任务同时执行
多个任务同时执行，就有点类似maven的动作了
liumiaocn:hello liumiao$ gradle compile test packaging install
[Phase: initialization] : settings executed... 
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
[phase:execution] install:doLast()
BUILD SUCCESSFUL in 0s
4 actionable tasks: 4 executed
liumiaocn:hello liumiao$总结
本系列中的例子的使用方式均是为了解释Gradle的基础概念和基本使用方式所整理，实际项目中如何进行最佳实践的落地，需要自行探索和研究。这篇文章强调的是gradle使用的时候的生命周期的三个阶段，一定要多留意这些与maven之类不同的地方。
参考内容
[https://docs.gradle.org/current/userguide/build_lifecycle.html](https://docs.gradle.org/current/userguide/build_lifecycle.html)

