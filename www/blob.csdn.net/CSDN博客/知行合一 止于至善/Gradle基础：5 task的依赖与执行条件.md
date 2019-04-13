
# Gradle基础：5:task的依赖与执行条件 - 知行合一 止于至善 - CSDN博客

2018年11月21日 06:58:39[liumiaocn](https://me.csdn.net/liumiaocn)阅读数：2851


![在这里插入图片描述](https://img-blog.csdnimg.cn/2018111707041592.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L2xpdW1pYW9jbg==,size_16,color_FFFFFF,t_70)
在使用maven的时候，compile/test/install的先后顺序和依赖关系都十分清晰，而在前面的例子中，可以通过gradle compile和gradle test来进行不同的任务执行，但是实际这些任务之间是有依赖关系的，就像我们在maven里面执行mvn install之前会做什么那样，这篇文章会继续来介绍task之间的依赖与执行条件。
[
](https://img-blog.csdnimg.cn/2018111707041592.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L2xpdW1pYW9jbg==,size_16,color_FFFFFF,t_70)
# 示例介绍
[
](https://img-blog.csdnimg.cn/2018111707041592.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L2xpdW1pYW9jbg==,size_16,color_FFFFFF,t_70)这篇文章会通过对上文的例子进行重新改写，来介绍gradle对于task操作的依赖和执行条件，还是如下4个任务：
[
](https://img-blog.csdnimg.cn/2018111707041592.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L2xpdW1pYW9jbg==,size_16,color_FFFFFF,t_70)

编译: compile
测试:test
打包:packaging
安装:install
[
](https://img-blog.csdnimg.cn/2018111707041592.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L2xpdW1pYW9jbg==,size_16,color_FFFFFF,t_70)
# 事前准备
[
](https://img-blog.csdnimg.cn/2018111707041592.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L2xpdW1pYW9jbg==,size_16,color_FFFFFF,t_70)修改之前的代码示例如下所示，对前文中的task的<<使用方式进行了修改，以消除warning信息
[
](https://img-blog.csdnimg.cn/2018111707041592.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L2xpdW1pYW9jbg==,size_16,color_FFFFFF,t_70)代码示例
[
](https://img-blog.csdnimg.cn/2018111707041592.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L2xpdW1pYW9jbg==,size_16,color_FFFFFF,t_70)`liumiaocn:hello liumiao$ cat build.gradle 
println "[phase:configuration] build.gradle ..."
task compile {
  group 'compile'
  description 'compile task'
  println "[phase:configuration] compile"
  doFirst {
    println "[phase:execution] compile :doFirst()"
  }
}
tasks.create(name: 'test') {
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
liumiaocn:hello liumiao$`[
](https://img-blog.csdnimg.cn/2018111707041592.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L2xpdW1pYW9jbg==,size_16,color_FFFFFF,t_70)执行确认
[
](https://img-blog.csdnimg.cn/2018111707041592.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L2xpdW1pYW9jbg==,size_16,color_FFFFFF,t_70)`liumiaocn:hello liumiao$ gradle compile test packaging install
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
](https://img-blog.csdnimg.cn/2018111707041592.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L2xpdW1pYW9jbg==,size_16,color_FFFFFF,t_70)
# 依赖设定
[
](https://img-blog.csdnimg.cn/2018111707041592.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L2xpdW1pYW9jbg==,size_16,color_FFFFFF,t_70)前面准备的例子有一个主要的问题，task之间的依赖关系实际是没有的，上文显示的执行顺序实际是因为输入的顺序产生的，在gradle里面也非常容易地通过dependsOn进行依赖关系的设定，这里将常用的方式进行整理如下：
[
](https://img-blog.csdnimg.cn/2018111707041592.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L2xpdW1pYW9jbg==,size_16,color_FFFFFF,t_70)
## 方式1: 通过()传入
[
](https://img-blog.csdnimg.cn/2018111707041592.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L2xpdW1pYW9jbg==,size_16,color_FFFFFF,t_70)`tasks.create(name: 'test',dependsOn: compile) {
  group 'test'
  description 'test task'
  println "[phase:configuration] test"
  doLast {
    println "[phase:execution] test:doLast()"
  }
}`[
](https://img-blog.csdnimg.cn/2018111707041592.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L2xpdW1pYW9jbg==,size_16,color_FFFFFF,t_70)执行结果确认信息如下，可以看到test在执行时compile的任务被事先执行了
[
](https://img-blog.csdnimg.cn/2018111707041592.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L2xpdW1pYW9jbg==,size_16,color_FFFFFF,t_70)
编译: compile
测试:test
[
](https://img-blog.csdnimg.cn/2018111707041592.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L2xpdW1pYW9jbg==,size_16,color_FFFFFF,t_70)`liumiaocn:hello liumiao$ gradle test
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
BUILD SUCCESSFUL in 0s
2 actionable tasks: 2 executed
liumiaocn:hello liumiao$`[
](https://img-blog.csdnimg.cn/2018111707041592.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L2xpdW1pYW9jbg==,size_16,color_FFFFFF,t_70)
## 方式2: 在{}中设定
[
](https://img-blog.csdnimg.cn/2018111707041592.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L2xpdW1pYW9jbg==,size_16,color_FFFFFF,t_70){}中设定的是任务的配置代码或者执行阶段代码的详细信息，在这里可以进行设定，比如：
[
](https://img-blog.csdnimg.cn/2018111707041592.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L2xpdW1pYW9jbg==,size_16,color_FFFFFF,t_70)`tasks.create("packaging") {
  group 'packaging'
  description 'packaging task'
  dependsOn test
  println "[phase:configuration] packaging"
  doLast {
    println "[phase:execution] packaging:doLast()"
  }
}`[
](https://img-blog.csdnimg.cn/2018111707041592.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L2xpdW1pYW9jbg==,size_16,color_FFFFFF,t_70)执行结果确认信息如下，注意由于test任务依赖于compile，所以在执行packaging的时候，会同时执行：
[
](https://img-blog.csdnimg.cn/2018111707041592.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L2xpdW1pYW9jbg==,size_16,color_FFFFFF,t_70)
编译: compile
测试:test
打包:packaging
[
](https://img-blog.csdnimg.cn/2018111707041592.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L2xpdW1pYW9jbg==,size_16,color_FFFFFF,t_70)`liumiaocn:hello liumiao$ gradle packaging
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
BUILD SUCCESSFUL in 0s
3 actionable tasks: 3 executed
liumiaocn:hello liumiao$`[
](https://img-blog.csdnimg.cn/2018111707041592.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L2xpdW1pYW9jbg==,size_16,color_FFFFFF,t_70)
## 方式3: 在{}外设定
[
](https://img-blog.csdnimg.cn/2018111707041592.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L2xpdW1pYW9jbg==,size_16,color_FFFFFF,t_70)当然也可以在{}进行指定，由于在{}之外，缺乏上下文，自然需要指定是谁依赖谁，而不是像前两种方式那样只需要设定依赖对象即可。
[
](https://img-blog.csdnimg.cn/2018111707041592.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L2xpdW1pYW9jbg==,size_16,color_FFFFFF,t_70)`task install(type: Install) {
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
install.dependsOn packaging`[
](https://img-blog.csdnimg.cn/2018111707041592.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L2xpdW1pYW9jbg==,size_16,color_FFFFFF,t_70)这样执行起来，依赖关系就完整了。
[
](https://img-blog.csdnimg.cn/2018111707041592.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L2xpdW1pYW9jbg==,size_16,color_FFFFFF,t_70)

编译: compile
测试:test
打包:packaging
安装:install
[
](https://img-blog.csdnimg.cn/2018111707041592.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L2xpdW1pYW9jbg==,size_16,color_FFFFFF,t_70)`liumiaocn:hello liumiao$ gradle install
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
](https://img-blog.csdnimg.cn/2018111707041592.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L2xpdW1pYW9jbg==,size_16,color_FFFFFF,t_70)
# 代码示例
[
](https://img-blog.csdnimg.cn/2018111707041592.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L2xpdW1pYW9jbg==,size_16,color_FFFFFF,t_70)`liumiaocn:hello liumiao$ cat build.gradle 
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
liumiaocn:hello liumiao$`[
](https://img-blog.csdnimg.cn/2018111707041592.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L2xpdW1pYW9jbg==,size_16,color_FFFFFF,t_70)
# enable属性
[
](https://img-blog.csdnimg.cn/2018111707041592.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L2xpdW1pYW9jbg==,size_16,color_FFFFFF,t_70)在前文介绍task常用属性和方法时，还提及到一个enable属性，这里通过示例来演示一下使用方式：
[
](https://img-blog.csdnimg.cn/2018111707041592.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L2xpdW1pYW9jbg==,size_16,color_FFFFFF,t_70)`tasks.create("packaging") {
  group 'packaging'
  description 'packaging task'
  dependsOn test
  enabled   false
  println "[phase:configuration] packaging"
  doLast {
    println "[phase:execution] packaging:doLast()"
  }
}`[
](https://img-blog.csdnimg.cn/2018111707041592.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L2xpdW1pYW9jbg==,size_16,color_FFFFFF,t_70)这样执行起来，由于enabled被设定为false，执行的过程则被跳过了
[
](https://img-blog.csdnimg.cn/2018111707041592.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L2xpdW1pYW9jbg==,size_16,color_FFFFFF,t_70)
编译: compile
测试:test
安装:install
[
](https://img-blog.csdnimg.cn/2018111707041592.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L2xpdW1pYW9jbg==,size_16,color_FFFFFF,t_70)虽然执行过程被跳过了，但是配置还是会执行的，这个需要注意一下。
[
](https://img-blog.csdnimg.cn/2018111707041592.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L2xpdW1pYW9jbg==,size_16,color_FFFFFF,t_70)`liumiaocn:hello liumiao$ gradle install
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
> Task :install
[phase:execution] install:doFirst()
[phase:execution] install:installObject (test.jar)
[phase:execution] install:checkObject   (test.jar)
[phase:execution] install:doLast()
BUILD SUCCESSFUL in 0s
3 actionable tasks: 3 executed
liumiaocn:hello liumiao$`[
](https://img-blog.csdnimg.cn/2018111707041592.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L2xpdW1pYW9jbg==,size_16,color_FFFFFF,t_70)当然使用方式也有很多种，比如也可以使用packaging.enabled = false在{}外设定，这里就不再赘述了。
[
](https://img-blog.csdnimg.cn/2018111707041592.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L2xpdW1pYW9jbg==,size_16,color_FFFFFF,t_70)
# onlyIf属性
[
](https://img-blog.csdnimg.cn/2018111707041592.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L2xpdW1pYW9jbg==,size_16,color_FFFFFF,t_70)onlyIf属性进行条件判断，比如install任务根据packaging的enabled是否为true进行判断当前install任务是否执行的时候，即可加上如下设定即可
[
](https://img-blog.csdnimg.cn/2018111707041592.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L2xpdW1pYW9jbg==,size_16,color_FFFFFF,t_70)`install.onlyIf { packaging.enabled }`[
](https://img-blog.csdnimg.cn/2018111707041592.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L2xpdW1pYW9jbg==,size_16,color_FFFFFF,t_70)这样在执行的时候，packaging的enabled如果是false的话，install也不会执行了
[
](https://img-blog.csdnimg.cn/2018111707041592.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L2xpdW1pYW9jbg==,size_16,color_FFFFFF,t_70)
编译: compile
测试:test
[
](https://img-blog.csdnimg.cn/2018111707041592.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L2xpdW1pYW9jbg==,size_16,color_FFFFFF,t_70)`liumiaocn:hello liumiao$ gradle install
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
BUILD SUCCESSFUL in 0s
2 actionable tasks: 2 executed
liumiaocn:hello liumiao$`[
](https://img-blog.csdnimg.cn/2018111707041592.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L2xpdW1pYW9jbg==,size_16,color_FFFFFF,t_70)
# 代码示例
[
](https://img-blog.csdnimg.cn/2018111707041592.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L2xpdW1pYW9jbg==,size_16,color_FFFFFF,t_70)由于演示enabled和onlyIf属性，代码略有变化，再次贴出如下：
[
](https://img-blog.csdnimg.cn/2018111707041592.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L2xpdW1pYW9jbg==,size_16,color_FFFFFF,t_70)`liumiaocn:hello liumiao$ cat build.gradle 
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
  enabled   false
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
](https://img-blog.csdnimg.cn/2018111707041592.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L2xpdW1pYW9jbg==,size_16,color_FFFFFF,t_70)
# 总结
[
](https://img-blog.csdnimg.cn/2018111707041592.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L2xpdW1pYW9jbg==,size_16,color_FFFFFF,t_70)使用gradle的dependsOn/enabled/onlyIf等特性，结合groovy语言进行简单编码，gradle能够实现非常灵活的用法，可以在具体项目中进行不断实践。
[
            ](https://img-blog.csdnimg.cn/2018111707041592.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L2xpdW1pYW9jbg==,size_16,color_FFFFFF,t_70)

