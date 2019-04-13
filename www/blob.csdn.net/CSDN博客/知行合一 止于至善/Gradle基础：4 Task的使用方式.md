
# Gradle基础：4:Task的使用方式 - 知行合一 止于至善 - CSDN博客

2018年11月21日 05:25:47[liumiaocn](https://me.csdn.net/liumiaocn)阅读数：1674


![在这里插入图片描述](https://img-blog.csdnimg.cn/2018111707041592.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L2xpdW1pYW9jbg==,size_16,color_FFFFFF,t_70)
Task是Gradle的基础单元，这篇文章总结和整理一下关于task的常见的使用方式。
[
](https://img-blog.csdnimg.cn/2018111707041592.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L2xpdW1pYW9jbg==,size_16,color_FFFFFF,t_70)
# 常见的属性和方法
[
](https://img-blog.csdnimg.cn/2018111707041592.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L2xpdW1pYW9jbg==,size_16,color_FFFFFF,t_70)在前面的例子中，我们已经使用task的description属性进行设定gradle task中的显示信息，将task进行分组显示，同时使用了doFirst和doLast方法进行回调。除了这些之外，还有一些其他的属性和方法，简单整理如下：
[
](https://img-blog.csdnimg.cn/2018111707041592.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L2xpdW1pYW9jbg==,size_16,color_FFFFFF,t_70)|属性/方法|说明|
|---|---|
|name|task的名称|
|description|task的描述|
|group|设定任务的逻辑分组|
|enabled|设定任务enable或者是disable|
|dependson|设定dependencies的配置|
|doFirst|在task的开始的回调方法|
|doLast|在task的结束的回调方法|
|onlyIf|条件执行|
# 示例介绍
[
](https://img-blog.csdnimg.cn/2018111707041592.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L2xpdW1pYW9jbg==,size_16,color_FFFFFF,t_70)这篇文章会通过对上文的例子进行重新改写，来介绍gradle对于task操作的常见方式，还是如下4个任务：
[
](https://img-blog.csdnimg.cn/2018111707041592.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L2xpdW1pYW9jbg==,size_16,color_FFFFFF,t_70)

编译: compile
测试:test
打包:packaging
安装:install
[
](https://img-blog.csdnimg.cn/2018111707041592.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L2xpdW1pYW9jbg==,size_16,color_FFFFFF,t_70)
# 创建task
[
](https://img-blog.csdnimg.cn/2018111707041592.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L2xpdW1pYW9jbg==,size_16,color_FFFFFF,t_70)gradle非常灵活，在task创建上都有很多的方式，这里列举出常见的几种，明白这几种，然后再看大部分gradle的介绍文档时就会较为轻松了。
[
](https://img-blog.csdnimg.cn/2018111707041592.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L2xpdW1pYW9jbg==,size_16,color_FFFFFF,t_70)
## 方式1： task task名称 {}
[
](https://img-blog.csdnimg.cn/2018111707041592.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L2xpdW1pYW9jbg==,size_16,color_FFFFFF,t_70)在前面的文章中主要使用这种方式进行演示，比如：
[
](https://img-blog.csdnimg.cn/2018111707041592.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L2xpdW1pYW9jbg==,size_16,color_FFFFFF,t_70)`task compile {
  group 'compile'
  description 'compile task'
  println "[phase:configuration] compile"
  doFirst {
    println "[phase:execution] compile :doFirst()"
  }
}`[
](https://img-blog.csdnimg.cn/2018111707041592.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L2xpdW1pYW9jbg==,size_16,color_FFFFFF,t_70)
## 方式2: tasks.create(name: ‘task名称’) {}
[
](https://img-blog.csdnimg.cn/2018111707041592.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L2xpdW1pYW9jbg==,size_16,color_FFFFFF,t_70)比如，将前文中的test段的例子进行修改：
[
](https://img-blog.csdnimg.cn/2018111707041592.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L2xpdW1pYW9jbg==,size_16,color_FFFFFF,t_70)`tasks.create(name: 'test') {
  group 'test'
  description 'test task'
  println "[phase:configuration] test"
  doLast {
    println "[phase:execution] test:doLast()"
  }
}`[
](https://img-blog.csdnimg.cn/2018111707041592.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L2xpdW1pYW9jbg==,size_16,color_FFFFFF,t_70)当然这种方式也可以简写称tasks.create(‘test’)，这里就不再赘述。
[
](https://img-blog.csdnimg.cn/2018111707041592.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L2xpdW1pYW9jbg==,size_16,color_FFFFFF,t_70)
## 方式3: task task名称 << {}
[
](https://img-blog.csdnimg.cn/2018111707041592.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L2xpdW1pYW9jbg==,size_16,color_FFFFFF,t_70)关于<<的解释有很多，简单来说，<<就是doLast的快捷方式。所以它会在execution阶段被执行。不过在本月底即将全面推出的Gradle 5中，这种写法已经deprecated了，以后初入者就可以对这个语法不再纠结了。
[
](https://img-blog.csdnimg.cn/2018111707041592.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L2xpdW1pYW9jbg==,size_16,color_FFFFFF,t_70)`task packaging << {
  group 'packaging'
  description 'packaging task'
  println "[phase:execution] in << closure"
}`[
](https://img-blog.csdnimg.cn/2018111707041592.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L2xpdW1pYW9jbg==,size_16,color_FFFFFF,t_70)
## 方式4: 继承DefaultTask
[
](https://img-blog.csdnimg.cn/2018111707041592.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L2xpdW1pYW9jbg==,size_16,color_FFFFFF,t_70)在前面的文章中介绍gradle特性的时候提到过groovy在gradle中的作用，虽然之前的例子一再展示，但是这种方式会让熟悉groovy或者java的开发者更加适应。
这里我们把前文中的install的task用这种方式进行改写一下：
`class Install extends DefaultTask{
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
}`另外，熟悉kotlin的也可以使用kotlin的方式来进行改写，这里就不再展开。相较于前面的几种方式，这种方式略显复杂，简单说明如下：
groovy或者kotlin作为gradle的DSL，继承诸如DefaultTask可以直接进行扩展。
除了DefaultTask之外虽然也有其他的方式可以对task进行扩展，直接继承DefaultTask可能是最为常见的一种方式。
创建任务时通过type建立扩展类和任务之间的关联
通过installObjectName将数据传入task的执行阶段，主要用于需要进行信息交互时，这也是常见的使用场景。
TaskAction注解为缺省的任务活动，当有多个时会顺次执行
这个例子中请结合注意doFirst和doLast的执行顺序：

doFirst
checkObject
installObject
doLast
执行结果确认
`liumiaocn:hello liumiao$ gradle install
[Phase: initialization] : settings executed... 
> Configure project :
[phase:configuration] build.gradle ...
[phase:configuration] compile
[phase:configuration] test
[phase:configuration] install
> Task :install
[phase:execution] install:doFirst()
[phase:execution] install:checkObject   (test.jar)
[phase:execution] install:installObject (test.jar)
[phase:execution] install:doLast()
Deprecated Gradle features were used in this build, making it incompatible with Gradle 5.0.
Use '--warning-mode all' to show the individual deprecation warnings.
See https://docs.gradle.org/4.10.2/userguide/command_line_interface.html#sec:command_line_warnings
BUILD SUCCESSFUL in 0s
1 actionable task: 1 executed
liumiaocn:hello liumiao$`
# 结果确认
## task确认
将任务进行了不同的分组，更贴近实际使用的状况：
`liumiaocn:hello liumiao$ gradle tasks
[Phase: initialization] : settings executed... 
> Configure project :
[phase:configuration] build.gradle ...
[phase:configuration] compile
[phase:configuration] test
[phase:configuration] install
> Task :tasks
------------------------------------------------------------
All tasks runnable from root project
------------------------------------------------------------
Build Setup tasks
-----------------
init - Initializes a new Gradle build.
wrapper - Generates Gradle wrapper files.
Compile tasks
-------------
compile - compile task
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
Install tasks
-------------
install - install task
Test tasks
----------
test - test task
To see all tasks and more detail, run gradle tasks --all
To see more detail about a task, run gradle help --task <task>
Deprecated Gradle features were used in this build, making it incompatible with Gradle 5.0.
Use '--warning-mode all' to show the individual deprecation warnings.
See https://docs.gradle.org/4.10.2/userguide/command_line_interface.html#sec:command_line_warnings
BUILD SUCCESSFUL in 0s
1 actionable task: 1 executed
liumiaocn:hello liumiao$`
## 执行确认
`liumiaocn:hello liumiao$ gradle compile test packaging install
[Phase: initialization] : settings executed... 
> Configure project :
[phase:configuration] build.gradle ...
[phase:configuration] compile
[phase:configuration] test
[phase:configuration] install
> Task :compile
[phase:execution] compile :doFirst()
> Task :test
[phase:execution] test:doLast()
> Task :packaging
[phase:execution] in << closure
> Task :install
[phase:execution] install:doFirst()
[phase:execution] install:checkObject   (test.jar)
[phase:execution] install:installObject (test.jar)
[phase:execution] install:doLast()
Deprecated Gradle features were used in this build, making it incompatible with Gradle 5.0.
Use '--warning-mode all' to show the individual deprecation warnings.
See https://docs.gradle.org/4.10.2/userguide/command_line_interface.html#sec:command_line_warnings
BUILD SUCCESSFUL in 0s
4 actionable tasks: 4 executed
liumiaocn:hello liumiao$`
# 参考内容
[https://guides.gradle.org/writing-gradle-tasks/](https://guides.gradle.org/writing-gradle-tasks/)
[
						](https://img-blog.csdnimg.cn/2018111707041592.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L2xpdW1pYW9jbg==,size_16,color_FFFFFF,t_70)
[
	](https://img-blog.csdnimg.cn/2018111707041592.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L2xpdW1pYW9jbg==,size_16,color_FFFFFF,t_70)
