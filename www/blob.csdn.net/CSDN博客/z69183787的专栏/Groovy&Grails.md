# Groovy&Grails - z69183787的专栏 - CSDN博客
2015年01月05日 19:02:32[OkidoGreen](https://me.csdn.net/z69183787)阅读数：703
*在公司内部分享中准备了Groovy&Grails的一些内容，在这里列出PPT中的核心内容，以便有兴趣的朋友对Groovy和Grails有初步的印象，后面会就这一主题有更深入的分享。*
**RoR（Ruby on Rails）的革命与Web开发的新时代 **
开发效率是J2EE的5倍以上。
Java的困惑：繁琐的配置、耗时的环境、可移植性差、资源占用大。
动态语言：动态类型、动态结构（元编程）。
RoR的设计思想是简化开发过程，各层的设计只有一个目标，就是简单。
与PHP比，它的组织结构更加清晰。
约定优于配置。
*动态类型：运行时确定变量的类型信息。
*动态结构：利用“元编程”技术，在程序运行时对类的属性和方法进动态增减。
*元编程：编写或者操纵其他程序(或者自身)作为它们的数据，或者在运行时完成部分本应在编译时完成的工作。
**RoR并不完美**
Ruby语言方面的不足：执行性能差、GC缺陷；与java区别大。
对历史遗留项目的支持较为困难。
**Grails（Groovy on Rails）的诞生解决了一些遗憾**
和RoR一样简单，甚至 在某些方面更加简单强大；遵循约定优于配置。
Groovy语言：虽然Groovy也是脚本语言，但经是以java字节码的方式运行在java虚拟机上的。两方面好处：一、较好的性能。二、语言级别透明地调用java。
Grails站在巨人的肩膀之上。Spring -> MVC;Hibernate ->GORM;SiteMesh->页面的统一和简化。可靠性得么保障，复杂性得到降低，更高的起点，更有效地解决问题。
良好的扩展性。插件化的设计架构。
**什么是Groovy**
是一个基于Java虚拟机的敏捷动态语言。 
构建在强大的Java语言之上 并添加了从Python，Ruby和Smalltalk等语言中学到的诸多特征。 
为Java开发者提供了现代最流行的编程语言特性，而且学习成本很低 
支持DSL（Domain Specific Languages领域专用语言）和其它简洁的语法，让你的代码变得易于阅读和维护。 
Groovy拥有处理原生类型，面向对象以及一个Ant DSL，使得创建Shell Scripts变的非常简单。 
在开发Web，GUI，数据库或控制台程序时通过减少框架性代码 大大提高了开发者的效率。 
支持单元测试和Mock（对象），可以简化测试。 
无缝集成 所有已经存在的 Java对象和类库。 
直接编译成Java字节码，这样可以在任何使用Java的地方使用Groovy。
*所谓领域专用语言（domain specific language / DSL），其基本思想是“求专不求全”，不像通用目的语言那样目标范围涵盖一切软件问题，而是专门针对某一特定问题的计算机语言。DSL之于程序员正如伽南地之于以色列人，是最初也是最终的梦想。几乎自计算机发明伊始，人们就开始谈论DSL使用DSL了。而前几年随着被誉为“Web开发领域专用语言”的Ruby on Rails迅速走红，DSL又一次成为人们讨论的热点话题。很多人都认为，DSL将会是软件业的“next big thing”。
**对Grails的一些误解**
Grails还不够成熟。已有网站、支持公司、招聘、技术。
Grails是否只是Rails的克隆产物。加入了许多Ruby并不存在的特性和概念。
有了Jruby on Rails之后，谁还要Grails?不是为了在Groovy上实现一个Rails,而是将业界最为强悍的组件以最佳方式组合起来的一个实践。DRY.
**Grails目标：超越其它语言在web领域的框架**
**Grails安装**
JDK;下载；解压；环境变量
**创建Grails工程**
grails create-app HelloGrails
grails create-controller hello
def say={render(‘Hello World!Hello Grails’)}闭包、action
grails run-app
http://localhost:8080/HelloGrails/hello/say
def say={return [text1:’World’, text2:’Grails’]}
grails create-domain-class Student
**Grails MVC**
**Groovy VS Java**
|**Groovy**|**Java**|
|----|----|
|println “Hello Groovy!”|System.out.println(“Hello Groovy!”);|
|9.times{print 8}|for(int i=0;i<9;i++){     System.out.print(8);}|
|9.downto(5){print 8}|for(int i=9;i>4;i--){     System.out.print(8);}|
|def list=[100,888,783]for(i in list){print i}|List list = new ArrayList();list.add(100);…..for(…){…}|
|[“Sunday”,”Saturday”,”Wednesaday”].each{day->         println “Today is ${day}”}|...|
 在Groovy可以用def定义无类型的变量(定义变量方面def与JavaScript中的var相似)，和返回值为无类型的方法，而在Java中没有def
 Groovy: 
   class Man {
        def name = "张三“
         def introduce() {
              return "I'm $name" // return可以省略
         }
  }
Groovy中switch语句与Java中相同，不过支持更多类型了，比如String 
map在Groovy中 def m = [a: 5, b: 2] 
Groovy不仅有? :三元操作符，还有?:两元操作符，但Java只有? :三元操作符 def result = a ?: "default result“
多重赋值
     [a, b] = [1, 2]
     [a, b] = [b, a]  // 交换a和b的值
