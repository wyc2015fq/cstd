# Struts2（一）搭建开发环境、入门案例、配置文件主要元素（package action result） - ChihkAnchor的博客 - CSDN博客





2019年03月12日 09:16:19[Chihk-Anchor](https://me.csdn.net/weixin_40871455)阅读数：32








# ******一、Struts2简介******

## ******1、Struts2概述******

Struts2是Apache发行的MVC开源框架。注意：它只是表现层（MVC）框架。

## ******2、Struts2的来历******

Struts1：也是apache开发的一套mvc的开源框架。在2005年之前非常流行。

弊端：Struts1的核心控制器就是一个Servlet。随着使用者的增多，弊端开始出现。

Struts2：在long long ago，有一个设计超前的框架XWork，后来推出了XWork1和WebWork2。Struts2就是apache和OpenSymphony组织合并开发出来。

里面包含了WebWork2的核心及Struts的一些特性和功能。除此之外，和Struts1没有任何关系了。

# ******二、搭建Struts2开发环境******

## ******1、下载Struts2开发包******

http://struts.apache.org 或 [https://mirrors.tuna.tsinghua.edu.cn/apache/struts/](https://mirrors.tuna.tsinghua.edu.cn/apache/struts/)

## ******2、开发包目录结构******

![](https://img-blog.csdnimg.cn/20190307203858306.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3dlaXhpbl80MDg3MTQ1NQ==,size_16,color_FFFFFF,t_70)

## ******3、搭建开发环境******

### ******3.1、拷贝必要jar包到classpath中******

![](https://img-blog.csdnimg.cn/20190307203937849.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3dlaXhpbl80MDg3MTQ1NQ==,size_16,color_FFFFFF,t_70)

拷贝哪些jar包？

找到Struts2自带的例子中，struts-blank的war包，在它的\WEB-INF\lib目录下的jar包全拷贝。

### ******3.2、建立Struts2的配置文件******

at the top of classpath（在最顶层的构建路径）,建立一个默认名称为****struts.xml****的配置文件。

![](https://img-blog.csdnimg.cn/20190307204037330.png)

注意：

1.文件名大小写。

2.创建位置。

3.该名称允许修改，但是我们一般不改。

### ******3.3、配置控制器******

a、配置位置：在web.xml中

b、配置什么： struts2已经写好了的一个过滤器。

结论：

struts2比struts1优秀的一个体现就是，它用了更为强大的过滤器作为控制器了。

![](https://img-blog.csdnimg.cn/20190307204125153.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3dlaXhpbl80MDg3MTQ1NQ==,size_16,color_FFFFFF,t_70)



### ******3.4、验证是否成功******

部署应用，启动Tomcat，不报错表示搭建成功。

![](https://img-blog.csdnimg.cn/20190307204157853.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3dlaXhpbl80MDg3MTQ1NQ==,size_16,color_FFFFFF,t_70)



# ******三、第一个Struts2案例******

以下步骤是日后实际开发中经常重复的。

## ******1、建立一个jsp文件******

![](https://img-blog.csdnimg.cn/20190307204242837.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3dlaXhpbl80MDg3MTQ1NQ==,size_16,color_FFFFFF,t_70)



## ******2、在struts.xml文件中配置******

![](https://img-blog.csdnimg.cn/20190307204322875.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3dlaXhpbl80MDg3MTQ1NQ==,size_16,color_FFFFFF,t_70)

## ******3、建立动作类和动作方法******

![](https://img-blog.csdnimg.cn/20190307204406764.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3dlaXhpbl80MDg3MTQ1NQ==,size_16,color_FFFFFF,t_70)

## ******4、结果视图页面******

![](https://img-blog.csdnimg.cn/2019030720442084.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3dlaXhpbl80MDg3MTQ1NQ==,size_16,color_FFFFFF,t_70)

## ******5、测试：******

![](https://img-blog.csdnimg.cn/20190307204434534.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3dlaXhpbl80MDg3MTQ1NQ==,size_16,color_FFFFFF,t_70)

## ******6、关于struts.xml没有提示的问题******

分析原因：没有找到对应的dtd约束文件。

解决办法：

a.上网

b.不能上网：

![](https://img-blog.csdnimg.cn/20190307204532536.png)



![](https://img-blog.csdnimg.cn/20190307204553806.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3dlaXhpbl80MDg3MTQ1NQ==,size_16,color_FFFFFF,t_70)

# ******四、第一个案例的执行过程******



![](https://img-blog.csdnimg.cn/20190307204710338.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3dlaXhpbl80MDg3MTQ1NQ==,size_16,color_FFFFFF,t_70)

****明确：****

![](https://img-blog.csdnimg.cn/20190307204753450.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3dlaXhpbl80MDg3MTQ1NQ==,size_16,color_FFFFFF,t_70)

# ******五、Struts2的配置文件******

## ******1、加载时机：******

当应用被tomcat加载的时候，struts2的配置文件就已经被加载过了。

## ******2、加载顺序******
|顺序|配置文件名|所在位置|说明|
|----|----|----|----|
|1|default.properties|struts2-core-2.3.15.3.jar\org\apache\struts2|不能修改|
|2|struts-default.xml|struts2-core-2.3.15.3.jar|不能修改|
|3|strtuts-plugin.xml|在struts2提供的插件jar包中|不能修改|
|4|struts.xml|我们的应用中|我们修改的：推荐|
|5|struts.properties|我们的应用中|我们修改的|
|6|web.xml|我们的应用中|我们修改的，可以给过滤器配置参数|

****注意：****

1、Struts2提供了两种配置的方式。一种是key=value的方式，即使用.properties文件。另一种是xml文件配置。我们推荐使用xml文件（它能描述层级关系）。

2、当多个配置文件中，有相同的参数，后面的会把前面的值给覆盖了。

# ******六、Struts2框架提供的常量******

## ******1、常用的常量******

常量定义在了default.properties配置文件中，体现形式都是key=value。所有的struts2应用都会用到这些常量。

常用的：
|常量名|常量值|说明|
|----|----|----|
|struts.i18n.encoding|UTF-8|应用中使用的编码|
|struts.objectFactory.spring.autoWire|name|和spring框架整合有关|
|struts.multipart.parser|jakarta|指定文件上传用的组件|
|struts.multipart.maxSize|2097152|文件上传总文件大小限制：2M|
|struts.action.extension|action,,|能进入Struts2框架内部的url地址后缀名。多个值用逗号分隔|
|struts.enable.DynamicMethodInvocation|false|是否允许动态方法调用|
|struts.devMode|false|是否是开发模式。开发模式：改了配置文件，不需要重启。输出更多的错误信息。开发阶段建议为true。|
|struts.ui.theme|xhtml|页面展示用的主题|



## ******2、在struts.xml中覆盖常量******

使用<constant name="" value=""></constant>元素进行覆盖

例如：

![](https://img-blog.csdnimg.cn/20190307205036330.png)

![](https://img-blog.csdnimg.cn/2019030720504373.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3dlaXhpbl80MDg3MTQ1NQ==,size_16,color_FFFFFF,t_70)

## ******3、创建struts.properties文件覆盖******

在应用的构建路径中创建一个struts.properties的文件。

![](https://img-blog.csdnimg.cn/20190307205052960.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3dlaXhpbl80MDg3MTQ1NQ==,size_16,color_FFFFFF,t_70)

## ******3、在web.xml中配置过滤器参数******

![](https://img-blog.csdnimg.cn/20190307205103355.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3dlaXhpbl80MDg3MTQ1NQ==,size_16,color_FFFFFF,t_70)

# ******七、xml配置文件的主要元素******

## ******1、package元素******

### ******1.1、作用：******

在struts2的配置文件中引入了面向对象思想，使用了分包管理。易于管理动作类。便于模块化开发动作类。

### ******1.2、属性：******

name：包的名称。必须写。且必须唯一。

extends：一般情况下需要继承****struts-default****包，但不是必须的。不过如果不继承的话，将无法使用struts2提供的核心功能。

struts-default.xml中定义着struts-default这个包。而struts-default.xml是在我们的struts.xml加载之前加载。

abstract：把包声明为抽象包，抽象包就是用来被继承的。只要是没有<action>元素的包，就可以声明为抽象包。

namespace：名称空间。

名称空间 + 动作名称 = 访问路径

![](https://img-blog.csdnimg.cn/20190307205138328.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3dlaXhpbl80MDg3MTQ1NQ==,size_16,color_FFFFFF,t_70)



![](https://img-blog.csdnimg.cn/20190307205144787.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3dlaXhpbl80MDg3MTQ1NQ==,size_16,color_FFFFFF,t_70)

### ******1.3、packege中的namespace详解******

namespace的默认值：

a.不写该属性

b.写了该属性，取值是一个"" ，空字符串.

注意：默认值不是/

动作类的搜索顺序：

![](https://img-blog.csdnimg.cn/2019030720521999.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3dlaXhpbl80MDg3MTQ1NQ==,size_16,color_FFFFFF,t_70)

## ******2、action元素******

### ******2.1、作用：******

配置动作用的。

### ******2.2、属性：******

name：动作名称

class：动作类全名。默认的动作类是：com.opensymphony.xwork2.ActionSupport是在struts-default.xml中定义的

![](https://img-blog.csdnimg.cn/20190307205308302.png)

要想替换默认动作类：

在应用的struts.xml中，package中加入：

![](https://img-blog.csdnimg.cn/20190307205323582.png)

method：动作类中的方法名称。默认是public String execute(){}

要求：

1.public的

2.返回值必须是String

3.没有参数

### ******2.3、动作类******

a.方式一：动作类就是一个POJO（Plain Old Java Object 原始的java对象），非常简单的javabean。

![](https://img-blog.csdnimg.cn/20190307205354428.png)



b.方式二：动作类实现com.opensymphony.xwork2.Action接口。

常量：给动作方法返回值用的。用常量可以使你的应用规范和统一。
|常量变量名|对应的值|说明|
|----|----|----|
|SUCCESS|success|动作方法执行一切OK|
|ERROR|error|动作方法执行时遇到了异常|
|INPUT|input|回显|
|LOGIN|login|一般转向登陆页面|
|NONE|none|不转向任何的视图|

c.方式三：动作类继承com.opensymphony.xwork2.ActionSupport推荐使用

![](https://img-blog.csdnimg.cn/20190307205432764.png)

### ******2.4、动作的访问******

a.使用通配符：

![](https://img-blog.csdnimg.cn/20190307205449569.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3dlaXhpbl80MDg3MTQ1NQ==,size_16,color_FFFFFF,t_70)

升级版：

![](https://img-blog.csdnimg.cn/20190307205454557.png)

优先级：绝对匹配优先。使用通配符的按照在配置文件中的先后顺序进行匹配的。

b.动态方法调用

![](https://img-blog.csdnimg.cn/20190307205510187.png)

![](https://img-blog.csdnimg.cn/20190307205515634.png)

# ******八、结果类型视图********(********逻辑结果视图********)******

前奏：该部分内容指的就是struts配置文件中的result元素的使用

## ******1、result元素******

作用：

为动作指定结果视图

属性：

name：逻辑视图的名称，对应着动作方法的返回值。默认值是success。 

![](https://img-blog.csdnimg.cn/20190307205540176.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3dlaXhpbl80MDg3MTQ1NQ==,size_16,color_FFFFFF,t_70)

type：结果类型，指的就是用什么方式转到定义的页面。默认是dispatcher。

![](https://img-blog.csdnimg.cn/20190307205611291.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3dlaXhpbl80MDg3MTQ1NQ==,size_16,color_FFFFFF,t_70)

## ******2、result元素中type的取值******

type属性的取值在struts-default.xml中定义着。

![](https://img-blog.csdnimg.cn/20190307205620483.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3dlaXhpbl80MDg3MTQ1NQ==,size_16,color_FFFFFF,t_70)

常用结果类型介绍：

dispatcher：(默认值)

使用请求转发，转向一个页面。

redirect：

使用重定向，转向一个页面。

![](https://img-blog.csdnimg.cn/20190307205655658.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3dlaXhpbl80MDg3MTQ1NQ==,size_16,color_FFFFFF,t_70)

redirectAction：注意：使用的是重定向。

a.重定向到另一个相同名称空间的动作。

![](https://img-blog.csdnimg.cn/20190307205837973.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3dlaXhpbl80MDg3MTQ1NQ==,size_16,color_FFFFFF,t_70)

b.重定向到不同名称空间的动作

![](https://img-blog.csdnimg.cn/20190307205808246.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3dlaXhpbl80MDg3MTQ1NQ==,size_16,color_FFFFFF,t_70)

chain：注意： 使用的是请求转发。

a.转发到另一个相同名称空间的动作。

![](https://img-blog.csdnimg.cn/20190307205905697.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3dlaXhpbl80MDg3MTQ1NQ==,size_16,color_FFFFFF,t_70)

b.请求转发到不同名称空间的动作

![](https://img-blog.csdnimg.cn/20190307205927707.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3dlaXhpbl80MDg3MTQ1NQ==,size_16,color_FFFFFF,t_70)

## ******3、result元素中param子元素******

 在转发或者重定向到不同包下的动作时，都用到了result元素的子元素param。

 param元素的作用：依赖注入(Dependence Injection)思想

我们通过struts-default.xml中的resultTypes元素中配置可以看出，每个结果类型视图其实都是靠一个类来实现的。而param元素就是将配置的参数，注入到该类中。

调用的是对应类的setter方法进行注入的。

例如：redirectAction结果视图

![](https://img-blog.csdnimg.cn/2019030720595045.png)

该类中肯定会有对actionName和namespace属性的注入方法（setter方法）。

![](https://img-blog.csdnimg.cn/20190307205956897.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3dlaXhpbl80MDg3MTQ1NQ==,size_16,color_FFFFFF,t_70)

再比如：默认结果视图dispatcher

![](https://img-blog.csdnimg.cn/20190307210003769.png)

![](https://img-blog.csdnimg.cn/2019030721001016.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3dlaXhpbl80MDg3MTQ1NQ==,size_16,color_FFFFFF,t_70)

## ******4、自定义结果类型******

 通过前面的内容，我们看出，其实结果类型就是一个类，这些类都实现了com.opensymphony.xwork2.Result接口。或者继承自该接口的实现类org.apache.struts2.dispatcher.StrutsResultSupport。这些类都有一个doExecute方法，用于执行结果视图。

 综上：我们也可以自己写一个结果视图。

 例子：

输出CAPTCHA图像的结果类型。CAPTCHA(Completely Automated Public Turing Test to Tell Computers and Humans Apart 全自动区分计算机和人类的图灵测试)————>简称：验证码。

第一步：写一个类，实现接口或者继承接口的实现类

![](https://img-blog.csdnimg.cn/20190307210048509.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3dlaXhpbl80MDg3MTQ1NQ==,size_16,color_FFFFFF,t_70)

第二步：在struts.xml文件中配置结果类型

![](https://img-blog.csdnimg.cn/20190307210054475.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3dlaXhpbl80MDg3MTQ1NQ==,size_16,color_FFFFFF,t_70)

第三步：在action配置时引用

![](https://img-blog.csdnimg.cn/20190307210059365.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3dlaXhpbl80MDg3MTQ1NQ==,size_16,color_FFFFFF,t_70)

最终结果：

![](https://img-blog.csdnimg.cn/20190307210107606.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3dlaXhpbl80MDg3MTQ1NQ==,size_16,color_FFFFFF,t_70)

扩展：通过可配置的参数，实现图像大小的调整

![](https://img-blog.csdnimg.cn/20190307210114872.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3dlaXhpbl80MDg3MTQ1NQ==,size_16,color_FFFFFF,t_70)

## ******5、全局视图和局部视图******

### ******5.1、局部视图******

![](https://img-blog.csdnimg.cn/20190307210122738.png)

### ******5.2、全局视图******

![](https://img-blog.csdnimg.cn/2019030721013114.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3dlaXhpbl80MDg3MTQ1NQ==,size_16,color_FFFFFF,t_70)

运行结果：

![](https://img-blog.csdnimg.cn/20190307210140768.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3dlaXhpbl80MDg3MTQ1NQ==,size_16,color_FFFFFF,t_70)

# ******九、在动作类中访问Servlet的API******

第一种方式：使用ServletActionContext类

![](https://img-blog.csdnimg.cn/2019030721023552.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3dlaXhpbl80MDg3MTQ1NQ==,size_16,color_FFFFFF,t_70)

第二种方式：使用实现接口的方式

![](https://img-blog.csdnimg.cn/20190307210244430.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3dlaXhpbl80MDg3MTQ1NQ==,size_16,color_FFFFFF,t_70)

![](https://img-blog.csdnimg.cn/2019030721025750.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3dlaXhpbl80MDg3MTQ1NQ==,size_16,color_FFFFFF,t_70)

整理自黑马程序员课堂笔记



