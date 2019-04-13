
# Gradle基础：6:多工程构建 - 知行合一 止于至善 - CSDN博客

2018年11月22日 06:09:35[liumiaocn](https://me.csdn.net/liumiaocn)阅读数：1605


![在这里插入图片描述](https://img-blog.csdnimg.cn/2018111707041592.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L2xpdW1pYW9jbg==,size_16,color_FFFFFF,t_70)
Maven中通过使用module来实现多模块的组合，在Gradle中可以类似通过多project进行聚合来实现，这篇文章主要介绍一下具体的使用方式。
[
](https://img-blog.csdnimg.cn/2018111707041592.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L2xpdW1pYW9jbg==,size_16,color_FFFFFF,t_70)
# 多工程结构
[
](https://img-blog.csdnimg.cn/2018111707041592.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L2xpdW1pYW9jbg==,size_16,color_FFFFFF,t_70)创建如下所示3层结构
![在这里插入图片描述](https://img-blog.csdnimg.cn/20181122053923802.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L2xpdW1pYW9jbg==,size_16,color_FFFFFF,t_70)
每个project下均有如下四个任务：
[
](https://img-blog.csdnimg.cn/20181122053923802.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L2xpdW1pYW9jbg==,size_16,color_FFFFFF,t_70)

编译: compile
测试:test
打包:packaging
安装:install
[
](https://img-blog.csdnimg.cn/20181122053923802.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L2xpdW1pYW9jbg==,size_16,color_FFFFFF,t_70)
# 事前准备
[
](https://img-blog.csdnimg.cn/20181122053923802.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L2xpdW1pYW9jbg==,size_16,color_FFFFFF,t_70)在前文的示例基础上，相关的settings和build信息如下：
[
](https://img-blog.csdnimg.cn/20181122053923802.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L2xpdW1pYW9jbg==,size_16,color_FFFFFF,t_70)settings.xml
[
](https://img-blog.csdnimg.cn/20181122053923802.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L2xpdW1pYW9jbg==,size_16,color_FFFFFF,t_70)`liumiaocn:hello liumiao$ cat settings.gradle 
println "[Phase: initialization] : settings executed... "
rootProject.name='helloPorject'
liumiaocn:hello liumiao$`[
](https://img-blog.csdnimg.cn/20181122053923802.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L2xpdW1pYW9jbg==,size_16,color_FFFFFF,t_70)build.gradle
[
](https://img-blog.csdnimg.cn/20181122053923802.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L2xpdW1pYW9jbg==,size_16,color_FFFFFF,t_70)`liumiaocn:hello liumiao$ cat build.gradle 
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
liumiaocn:hello liumiao$`[
](https://img-blog.csdnimg.cn/20181122053923802.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L2xpdW1pYW9jbg==,size_16,color_FFFFFF,t_70)执行确认
[
](https://img-blog.csdnimg.cn/20181122053923802.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L2xpdW1pYW9jbg==,size_16,color_FFFFFF,t_70)`liumiaocn:hello liumiao$ gradle install
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
[phase:execution] install:installObject (test.jar)
[phase:execution] install:checkObject   (test.jar)
[phase:execution] install:doLast()
BUILD SUCCESSFUL in 0s
4 actionable tasks: 4 executed
liumiaocn:hello liumiao$`[
](https://img-blog.csdnimg.cn/20181122053923802.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L2xpdW1pYW9jbg==,size_16,color_FFFFFF,t_70)
# 多项目构建准备
[
](https://img-blog.csdnimg.cn/20181122053923802.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L2xpdW1pYW9jbg==,size_16,color_FFFFFF,t_70)创建subProjectA subProjectA/subProjectAA subProjectB三个目录，然后将build.gradle分别拷贝进去即做完了演示准备。
[
](https://img-blog.csdnimg.cn/20181122053923802.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L2xpdW1pYW9jbg==,size_16,color_FFFFFF,t_70)`liumiaocn:hello liumiao$ ls
build.gradle    settings.gradle
liumiaocn:hello liumiao$ mkdir -p subProjectA subProjectA/subProjectAA subProjectB
liumiaocn:hello liumiao$ cp build.gradle subProjectA
liumiaocn:hello liumiao$ cp build.gradle subProjectA/subProjectAA/
liumiaocn:hello liumiao$ cp build.gradle subProjectB
liumiaocn:hello liumiao$`[
](https://img-blog.csdnimg.cn/20181122053923802.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L2xpdW1pYW9jbg==,size_16,color_FFFFFF,t_70)这样每个project下有四个任务的准备已经完成：
[
](https://img-blog.csdnimg.cn/20181122053923802.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L2xpdW1pYW9jbg==,size_16,color_FFFFFF,t_70)

编译: compile
测试:test
打包:packaging
安装:install
[
](https://img-blog.csdnimg.cn/20181122053923802.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L2xpdW1pYW9jbg==,size_16,color_FFFFFF,t_70)构成结构
[
](https://img-blog.csdnimg.cn/20181122053923802.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L2xpdW1pYW9jbg==,size_16,color_FFFFFF,t_70)`liumiaocn:hello liumiao$ tree .
.
├── build.gradle
├── settings.gradle
├── subProjectA
│   ├── build.gradle
│   └── subProjectAA
│       └── build.gradle
└── subProjectB
    └── build.gradle
3 directories, 5 files
liumiaocn:hello liumiao$`[
](https://img-blog.csdnimg.cn/20181122053923802.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L2xpdW1pYW9jbg==,size_16,color_FFFFFF,t_70)
# 设定settings.xml
[
](https://img-blog.csdnimg.cn/20181122053923802.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L2xpdW1pYW9jbg==,size_16,color_FFFFFF,t_70)唯一需要设定的是rootProject下的settings.xml，使用到的语句是include，添加如下一行信息：
[
](https://img-blog.csdnimg.cn/20181122053923802.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L2xpdW1pYW9jbg==,size_16,color_FFFFFF,t_70)`include 'subProjectA', 'subProjectB','subProjectA:subProjectAA'`[
](https://img-blog.csdnimg.cn/20181122053923802.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L2xpdW1pYW9jbg==,size_16,color_FFFFFF,t_70)当然，写法可能还有很多种，比如引入3层结构的时候可以使用:subProjectA:subProjectAA等方式，这里就不再额外展开。这样仅仅添加了一行，就已经完成了三层结构的关联：
[
](https://img-blog.csdnimg.cn/20181122053923802.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L2xpdW1pYW9jbg==,size_16,color_FFFFFF,t_70)![在这里插入图片描述](https://img-blog.csdnimg.cn/20181122053923802.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L2xpdW1pYW9jbg==,size_16,color_FFFFFF,t_70)
# 结果确认
使用gradle projects可以清楚地看到3层的工程构成状况：
![在这里插入图片描述](https://img-blog.csdnimg.cn/2018112206001126.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L2xpdW1pYW9jbg==,size_16,color_FFFFFF,t_70)
详细执行日志信息如下：
[
](https://img-blog.csdnimg.cn/2018112206001126.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L2xpdW1pYW9jbg==,size_16,color_FFFFFF,t_70)`liumiaocn:hello liumiao$ gradle projects
[Phase: initialization] : settings executed... 
> Configure project :
[phase:configuration] build.gradle ...
[phase:configuration] compile
[phase:configuration] test
[phase:configuration] packaging
[phase:configuration] install
> Configure project :subProjectA
[phase:configuration] build.gradle ...
[phase:configuration] compile
[phase:configuration] test
[phase:configuration] packaging
[phase:configuration] install
> Configure project :subProjectB
[phase:configuration] build.gradle ...
[phase:configuration] compile
[phase:configuration] test
[phase:configuration] packaging
[phase:configuration] install
> Configure project :subProjectA:subProjectAA
[phase:configuration] build.gradle ...
[phase:configuration] compile
[phase:configuration] test
[phase:configuration] packaging
[phase:configuration] install
> Task :projects
------------------------------------------------------------
Root project
------------------------------------------------------------
Root project 'helloPorject'
+--- Project ':subProjectA'
|    \--- Project ':subProjectA:subProjectAA'
\--- Project ':subProjectB'
To see a list of the tasks of a project, run gradle <project-path>:tasks
For example, try running gradle :subProjectA:tasks
BUILD SUCCESSFUL in 0s
1 actionable task: 1 executed
liumiaocn:hello liumiao$`[
](https://img-blog.csdnimg.cn/2018112206001126.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L2xpdW1pYW9jbg==,size_16,color_FFFFFF,t_70)当然也可以查看某个子工程的详细信息
[
](https://img-blog.csdnimg.cn/2018112206001126.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L2xpdW1pYW9jbg==,size_16,color_FFFFFF,t_70)`liumiaocn:hello liumiao$ gradle subProjectA:projects
[Phase: initialization] : settings executed... 
> Configure project :
[phase:configuration] build.gradle ...
[phase:configuration] compile
[phase:configuration] test
[phase:configuration] packaging
[phase:configuration] install
> Configure project :subProjectA
[phase:configuration] build.gradle ...
[phase:configuration] compile
[phase:configuration] test
[phase:configuration] packaging
[phase:configuration] install
> Configure project :subProjectB
[phase:configuration] build.gradle ...
[phase:configuration] compile
[phase:configuration] test
[phase:configuration] packaging
[phase:configuration] install
> Configure project :subProjectA:subProjectAA
[phase:configuration] build.gradle ...
[phase:configuration] compile
[phase:configuration] test
[phase:configuration] packaging
[phase:configuration] install
> Task :subProjectA:projects
------------------------------------------------------------
Project :subProjectA
------------------------------------------------------------
Project ':subProjectA'
\--- Project ':subProjectA:subProjectAA'
To see a list of the tasks of a project, run gradle <project-path>:tasks
For example, try running gradle :subProjectA:subProjectAA:tasks
To see a list of all the projects in this build, run gradle :projects
BUILD SUCCESSFUL in 0s
1 actionable task: 1 executed
liumiaocn:hello liumiao$`[
](https://img-blog.csdnimg.cn/2018112206001126.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L2xpdW1pYW9jbg==,size_16,color_FFFFFF,t_70)
# 执行构建
[
](https://img-blog.csdnimg.cn/2018112206001126.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L2xpdW1pYW9jbg==,size_16,color_FFFFFF,t_70)执行rootProject构建
可以看到对根目录进行构建的时候，其他所有部分也会一同被构建
liumiaocn:hello liumiao$ gradle install
[Phase: initialization] : settings executed... 
> Configure project :
[phase:configuration] build.gradle ...
[phase:configuration] compile
[phase:configuration] test
[phase:configuration] packaging
[phase:configuration] install
> Configure project :subProjectA
[phase:configuration] build.gradle ...
[phase:configuration] compile
[phase:configuration] test
[phase:configuration] packaging
[phase:configuration] install
> Configure project :subProjectB
[phase:configuration] build.gradle ...
[phase:configuration] compile
[phase:configuration] test
[phase:configuration] packaging
[phase:configuration] install
> Configure project :subProjectA:subProjectAA
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
> Task :subProjectA:compile
[phase:execution] compile :doFirst()
> Task :subProjectA:test
[phase:execution] test:doLast()
> Task :subProjectA:packaging
[phase:execution] packaging:doLast()
> Task :subProjectA:install
[phase:execution] install:doFirst()
[phase:execution] install:installObject (test.jar)
[phase:execution] install:checkObject   (test.jar)
[phase:execution] install:doLast()
> Task :subProjectB:compile
[phase:execution] compile :doFirst()
> Task :subProjectB:test
[phase:execution] test:doLast()
> Task :subProjectB:packaging
[phase:execution] packaging:doLast()
> Task :subProjectB:install
[phase:execution] install:doFirst()
[phase:execution] install:installObject (test.jar)
[phase:execution] install:checkObject   (test.jar)
[phase:execution] install:doLast()
> Task :subProjectA:subProjectAA:compile
[phase:execution] compile :doFirst()
> Task :subProjectA:subProjectAA:test
[phase:execution] test:doLast()
> Task :subProjectA:subProjectAA:packaging
[phase:execution] packaging:doLast()
> Task :subProjectA:subProjectAA:install
[phase:execution] install:doFirst()
[phase:execution] install:installObject (test.jar)
[phase:execution] install:checkObject   (test.jar)
[phase:execution] install:doLast()
BUILD SUCCESSFUL in 0s
16 actionable tasks: 16 executed
liumiaocn:hello liumiao$执行子工程的构建
也可以通过gradle来指定子工程进行构建，比如执行第3层的子工程的任务
liumiaocn:hello liumiao$ gradle subProjectA:subProjectAA:install
[Phase: initialization] : settings executed... 
> Configure project :
[phase:configuration] build.gradle ...
[phase:configuration] compile
[phase:configuration] test
[phase:configuration] packaging
[phase:configuration] install
> Configure project :subProjectA
[phase:configuration] build.gradle ...
[phase:configuration] compile
[phase:configuration] test
[phase:configuration] packaging
[phase:configuration] install
> Configure project :subProjectB
[phase:configuration] build.gradle ...
[phase:configuration] compile
[phase:configuration] test
[phase:configuration] packaging
[phase:configuration] install
> Configure project :subProjectA:subProjectAA
[phase:configuration] build.gradle ...
[phase:configuration] compile
[phase:configuration] test
[phase:configuration] packaging
[phase:configuration] install
> Task :subProjectA:subProjectAA:compile
[phase:execution] compile :doFirst()
> Task :subProjectA:subProjectAA:test
[phase:execution] test:doLast()
> Task :subProjectA:subProjectAA:packaging
[phase:execution] packaging:doLast()
> Task :subProjectA:subProjectAA:install
[phase:execution] install:doFirst()
[phase:execution] install:installObject (test.jar)
[phase:execution] install:checkObject   (test.jar)
[phase:execution] install:doLast()
BUILD SUCCESSFUL in 0s
4 actionable tasks: 4 executed
liumiaocn:hello liumiao$总结
在gradle中通过简单的include语句就实现了结构的关联，当然相较于父子关系的引入方式，在结构上这种方式的实现可能在集成关系，修改的影响范围等方面各有利弊，本文主要用于介绍gradle下多层模块的构成方式，虽然可以使用N层进行构成，是否使用建议慎重，这方面使用者可在项目中自行体会。

[
](https://img-blog.csdnimg.cn/2018112206001126.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L2xpdW1pYW9jbg==,size_16,color_FFFFFF,t_70)
