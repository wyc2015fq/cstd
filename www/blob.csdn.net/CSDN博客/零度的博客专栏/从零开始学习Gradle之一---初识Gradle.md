# 从零开始学习Gradle之一---初识Gradle - 零度的博客专栏 - CSDN博客
2016年08月09日 10:51:49[零度anngle](https://me.csdn.net/zmx729618)阅读数：2126
前提： 安装Gradle。安装过程非常简单: 
(1)下载Gradle
(2)将GRADLE_HOME/bin/gradle加入$PATH。
**1. 基本概念(Project 和 Task)**
Gradle中有两个基本的概念：project和task。每个Gradle的构建由一个project构成，它代表着需要被构建的组件或者构建的整个项目。每个project由一个或者多个task组成。task代表着Gradle构建过程中可执行的最小单元。例如当构建一个组件时，可能需要先编译、打包、然后再生成文档或者发布等，这其中的每个步骤都可以定义成一个task。
**2. 构建第一个Task**
和Ant运行时读取build.xml类似，Gradle运行时默认会读取build.gradle这个文件, 当然你也可以使用参数"-b"来指定其他的xxx.gradle
下面，让我们新建一个build.gradle文件，然后输入如下内容:
  task hello {
      doLast{
           println "hello world"
      }
 }
 这个构建的脚本很简单，就是输出hello world。为了运行这个构建，我们应该在当前目录下执行 "gradle hello"，即gradle TaskName。
doLast意思是定义一个行为(映射Gradle中的Action类)，放在当前task的最后，类似的，还有doFirst, 表示将定义的行为放在当前task最前面，例如
task hello {
  doLast{
      println "Hello world"
    }   
  doFirst{
      println "I am xxx"
    }   
}
执行gradle hello, 将输出
"I am xxx"
"Hello world"
另外，你也可以使用如下更简洁的方式来定义task：
task hello <<  {
     println "hello world"
}
这里也许大家可能会觉得很奇怪，为什么可以用"<<"来定义Task的执行内容呢，还是让我们看看Gradle的代码是怎么实现的吧:
 1publicabstractclass AbstractTask implements TaskInternal, DynamicObjectAware {
 2private List<Action<? super Task>> actions = new ArrayList<Action<?   super Task>>();
 3
 4public Task doFirst(Action<? super Task> action) {
 5if (action == null) {
 6thrownew InvalidUserDataException("Action must not be null!");
 7         }
 8         actions.add(0, wrap(action));
 9returnthis;
10     }
11
12public Task doLast(Action<? super Task> action) {
13if (action == null) {
14thrownew InvalidUserDataException("Action must not be null!");
15         }
16         actions.add(wrap(action));
17returnthis;
18     }
从上面的代码可以看出，Task类里有个Action的集合actions，当使用doFirst或者doLast时，实际上是将定义的执行部分实例化成Action的对象,然后添加到actions集合里。
明白了这一点，接下来让我们看看为什么可以使用<<定义Task--------Groovy作为强大的支持DSL的动态语言，早已经重载了 << 操作符，使得我们可以方便的使用<<向集合添加元素。
说道这，相信真相已经大白了:原来就是使用Groovy的特性，往集合里添加Action而已。对，这就是Gradle的语法，利用Groovy的DSL特性，帮助我们更容易的定义我们的构建脚本。
不过也许大家会觉得，这个例子实在是没有什么代表性，只是一个简单的 hello world，说明不了什么问题。好吧，别着急，下次我们会继续研究Gradle的其他部分，不过先记住：作为一个构建工具，Gradle真的很强大哦！
            
