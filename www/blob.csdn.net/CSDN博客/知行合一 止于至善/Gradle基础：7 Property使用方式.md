
# Gradle基础：7:Property使用方式 - 知行合一 止于至善 - CSDN博客

2018年11月23日 07:31:12[liumiaocn](https://me.csdn.net/liumiaocn)阅读数：1319标签：[gradle																](https://so.csdn.net/so/search/s.do?q=gradle&t=blog)[property																](https://so.csdn.net/so/search/s.do?q=property&t=blog)[
							](https://so.csdn.net/so/search/s.do?q=gradle&t=blog)个人分类：[gradle																](https://blog.csdn.net/liumiaocn/article/category/8369185)



![在这里插入图片描述](https://img-blog.csdnimg.cn/2018111707041592.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L2xpdW1pYW9jbg==,size_16,color_FFFFFF,t_70)
在前面的文章中使用到了`rootProject.name`, 以及task中的description等变量进行直接设定，这些就是gradle内置的property，这篇文章将对gradle的property相关的知识进行介绍。
# 常用的property：内置
gradle有很多内置的属性，比如project和task相关的常见属性信息如下：
## project的property
|属性|说明|
|---|---|
|name|project的名称，可以在setting.xml中进行修改|
|description|项目描述，一般在build.gradle文件中设定|
|version|项目版本信息|
|parent|存在的情况下返回父项目信息|
|group|用户定义的项目分组信息|
|dependencies|项目依赖配置信息|
|repositories|项目配置库信息|
|defaultTasks|项目缺省运行的任务名称|
|buildDir|构建脚本(缺省状况下build.gradle)所在目录|
|buildFile|构建脚本名称(缺省状况下为build.gradle)，可使用-b选项修改|
## task的property
|属性|说明|
|---|---|
|name|task的名称|
|description|task的描述|
|group|设定任务的逻辑分组|
|enabled|设定任务enable或者是disable|
|dependson|设定dependencies的配置|
## 其他内置属性
使用如下命令可以查看更多gradle的内置属性
> 查看命令：gradle properties

## 内置属性设定方法
### 方式1: 直接设定
访问和设定内置属性，可以直接通过属性名进行设定，比如version，但是当出现可能的重名歧义时，通常需要使用限定内容对其歧义进行消除，比如description需要指定是task的description还是project的。
### 方式2: task的属性可以通过.configure方式定义
> 定义方式：在 task名称.configure{}中进行定义

> 格式：task名称.configure{ 属性名称 = 设定内容 }

> 而使用时直接使用定义中设定的属性名称即可

> 代码示例

> liumiaocn:property liumiao$ cat settings.gradle 
println "[Phase: initialization] : settings executed... "
rootProject.name='propertySample'
println "[default]:[rootProject.name] = " + rootProject.name
liumiaocn:property liumiao$ cat build.gradle 
println "[phase:configuration] build.gradle ..."
println "[phase:configuration] before setting [version] = " + version
task setDefaultProperty {
  group 'propertyTest'
  description 'set default property'
  version = 'alpha'
}
setDefaultProperty.configure {
  description = 'property using task.configure'
}
tasks.create("display") {
  group 'propertyTest'
  description 'display properties'
  doFirst() {
    println '[default]:[rootProject.name] = ' + rootProject.name
    println '[default]:[version] = ' + version
    println '[default]:[description] = ' + description
    println '[default/configure]:[setDefaultProperty.description] = ' + setDefaultProperty.description
  }
}
liumiaocn:property liumiao$
> 结果确认

> liumiaocn:property liumiao$ gradle display
[Phase: initialization] : settings executed... 
[default]:[rootProject.name] = propertySample
> Configure project :
[phase:configuration] build.gradle ...
[phase:configuration] before setting [version] = unspecified
> Task :display
[default]:[rootProject.name] = propertySample
[default]:[version] = alpha
[default]:[description] = display properties
[default/configure]:[setDefaultProperty.description] = property using task.configure
BUILD SUCCESSFUL in 0s
1 actionable task: 1 executed
liumiaocn:property liumiao$
> 注意：

> 在Gradle 5中rootProject.name已经不再建议中间使用空格，所以上述的名称为propertySample。

> 自定义属性

> gradle还提供了多种方式进行自定义属性

> 方式1: 通过ext进行自定义属性

> 普通方式定义：ext.属性名称 = 设定内容

> 或者使用

> 闭包方式定义: ext { 属性名称 = 设定内容 }

> 而使用时直接使用定义中设定的属性名称即可

> 代码示例

> ext {
  extProperty1 = 'perperty using ext closure'
}
ext.extProperty2 = 'perperty using ext.'
task displayExtProperties {
  group 'propertyTest'
  description 'display ext properties'
  doFirst() {
    println '[ext]:[extProperty1] = ' + extProperty1
    println '[ext]:[ext.extProperty2] = ' + extProperty2
  }
}
> 执行确认

> liumiaocn:property liumiao$ gradle displayExtProperties
[Phase: initialization] : settings executed... 
[default]:[rootProject.name] = propertySample
> Configure project :
[phase:configuration] build.gradle ...
[phase:configuration] before setting [version] = unspecified
> Task :displayExtProperties
[ext]:[extProperty1] = perperty using ext closure
[ext]:[ext.extProperty2] = perperty using ext.
BUILD SUCCESSFUL in 0s
1 actionable task: 1 executed
liumiaocn:property liumiao$
> 方式2: 通过-P参数传入自定义属性

> 定义方式：使用gradle执行时使用-P参数，形式为：

> -P 属性名称 = 设定内容

> 而使用时直接使用定义中设定的属性名称即可

> 代码示例

> task displayParaByP {
  group 'propertyTest'
  description 'display -P properties'
  doFirst() {
    println '[-P]:[pProperty1] = ' + pProperty1
  }
}
> 执行确认

> liumiaocn:property liumiao$ gradle -P pProperty1='perperty using -P ' displayParaByP
[Phase: initialization] : settings executed... 
[default]:[rootProject.name] = propertySample
> Configure project :
[phase:configuration] build.gradle ...
[phase:configuration] before setting [version] = unspecified
> Task :displayParaByP
[-P]:[pProperty1] = perperty using -P 
BUILD SUCCESSFUL in 0s
1 actionable task: 1 executed
liumiaocn:property liumiao$
> 方式3: 通过环境变量设定自定义属性

> gradle使用约定：环境变量以ORG_GRADLE_PROJECT_开头的，gradle会约定认为这是通过环境变量方式传入的属性名称。

> 定义方式：

> export ORG_GRADLE_PROJECT_属性名称 = 设定内容

> 而使用时直接使用定义中设定的属性名称即可

> 代码示例

> task displayParaByENV {
  group 'propertyTest'
  description 'display ENV properties'
  doFirst() {
    println '[ENV]:[envProperty1] = ' + envProperty1
  }
}
> 执行确认

> liumiaocn:property liumiao$ export ORG_GRADLE_PROJECT_envProperty1='property using ENV'
liumiaocn:property liumiao$ gradle displayParaByENV
[Phase: initialization] : settings executed... 
[default]:[rootProject.name] = propertySample
> Configure project :
[phase:configuration] build.gradle ...
[phase:configuration] before setting [version] = unspecified
> Task :displayParaByENV
[ENV]:[envProperty1] = property using ENV
BUILD SUCCESSFUL in 0s
1 actionable task: 1 executed
liumiaocn:property liumiao$
> 方式4: 通过JVM的-D传入自定义属性

> gradle使用约定：像环境变量的方式一样，使用JVM也需要gradle能够认出这是传入的属性名称，使用-D以org.gradle.project.开头的，gradle也会约定认为这是要传入的属性名称

> 定义方式：

> gradle -D org.gradle.project.属性名称 = 设定内容

> 而使用时直接使用定义中设定的属性名称即可

> 代码示例

> task displayParaByJVM {
  group 'propertyTest'
  description 'display JVM properties'
  doFirst() {
    println '[JVM]:[jvmProperty1] = ' + jvmProperty1
  }
}
> 执行确认

> liumiaocn:property liumiao$ gradle -Dorg.gradle.project.jvmProperty1='property using JVM Param' displayParaByJVM
[Phase: initialization] : settings executed... 
[default]:[rootProject.name] = propertySample
> Configure project :
[phase:configuration] build.gradle ...
[phase:configuration] before setting [version] = unspecified
> Task :displayParaByJVM
[JVM]:[jvmProperty1] = property using JVM Param
BUILD SUCCESSFUL in 0s
1 actionable task: 1 executed
liumiaocn:property liumiao$
> 总结

> 虽然是非常简单的一个特性，Property作为参数传递的重要方式之一，在gradle中的使用也是非常地灵活的，比如使用环境变量或者JVM参数的方式都比较灵活，即使是在持续构建中的一个脚本的实现，也应该多考虑一下如何将定耦合等，在DevOps实践中，这些正是延伸了的设计需要考虑的地方。


