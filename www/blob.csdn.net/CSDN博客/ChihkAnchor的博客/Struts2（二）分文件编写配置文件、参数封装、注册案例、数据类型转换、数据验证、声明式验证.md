# Struts2（二）分文件编写配置文件、参数封装、注册案例、数据类型转换、数据验证、声明式验证 - ChihkAnchor的博客 - CSDN博客





2019年03月13日 09:17:37[Chihk-Anchor](https://me.csdn.net/weixin_40871455)阅读数：54








# ******一、分文件编写框架配置文件******

## ******1、不分文件开发可能产生的问题******

就类似于我们在写java类时，所有代码都写在一个类里，甚至写在一个方法里。

![](https://img-blog.csdnimg.cn/20190312082454977.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3dlaXhpbl80MDg3MTQ1NQ==,size_16,color_FFFFFF,t_70)

当3个人都checkout了struts.xml文件时，第一个人提交了，后面的人在没有更新就提交时，第一个人写的可能就白写了。

## ******2、分文件编写Struts2的配置文件******

![](https://img-blog.csdnimg.cn/20190312082535911.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3dlaXhpbl80MDg3MTQ1NQ==,size_16,color_FFFFFF,t_70)

# ******二、封装请求正文到对象中（非常重要）******

## ******1********、静态参数封装******

在struts.xml配置文件中，给动作类注入值。调用的是setter方法。

![](https://img-blog.csdnimg.cn/20190312082643891.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3dlaXhpbl80MDg3MTQ1NQ==,size_16,color_FFFFFF,t_70)

原因：是由一个staticParams的拦截器完成注入的。

![](https://img-blog.csdnimg.cn/20190312082718344.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3dlaXhpbl80MDg3MTQ1NQ==,size_16,color_FFFFFF,t_70)

![](https://img-blog.csdnimg.cn/20190312082746617.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3dlaXhpbl80MDg3MTQ1NQ==,size_16,color_FFFFFF,t_70)

## ******2、动态参数封装：开发时用到的******

通过用户的表单封装请求正文参数。

### ******2.1********、动作类作为实体模型******

实体模型：Entity，对应数据库中表的记录（注意类对应的是表结构，而对象对应的是一条记录）

![](https://img-blog.csdnimg.cn/20190312082845705.png)

![](https://img-blog.csdnimg.cn/20190312082911304.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3dlaXhpbl80MDg3MTQ1NQ==,size_16,color_FFFFFF,t_70)

原因：是由params拦截器完成的。

### ******2.2********、动作类和实体模型分开******

![](https://img-blog.csdnimg.cn/20190312083014532.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3dlaXhpbl80MDg3MTQ1NQ==,size_16,color_FFFFFF,t_70)

问题：

由于我们没有初始化user对象，默认为null，一调用setUser方法，就空指针异常了。但是框架却封装进去值了。

原因：

通过执行过程：

![](https://img-blog.csdnimg.cn/20190312083257831.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3dlaXhpbl80MDg3MTQ1NQ==,size_16,color_FFFFFF,t_70)

### ******2.3、模型驱动：建立动作类和模型分开的前提下（开发中采用的方式）******

****此处的学习目标：目前先记住怎么写，要想理解，必须等讲完OGNL表达式之后。****

![](https://img-blog.csdnimg.cn/20190312083428190.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3dlaXhpbl80MDg3MTQ1NQ==,size_16,color_FFFFFF,t_70)

****原因：是由一个********m********odelDriven拦截器做的。****

# ******三、用户注册案例（重点）******

## ******1、数据建模（实体模型和数据库）******

![](https://img-blog.csdnimg.cn/20190312083537917.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3dlaXhpbl80MDg3MTQ1NQ==,size_16,color_FFFFFF,t_70)

## ******2、建立业务层接口******

![](https://img-blog.csdnimg.cn/20190312083550278.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3dlaXhpbl80MDg3MTQ1NQ==,size_16,color_FFFFFF,t_70)

## ******3、建立持久层接口******

![](https://img-blog.csdnimg.cn/20190312083608213.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3dlaXhpbl80MDg3MTQ1NQ==,size_16,color_FFFFFF,t_70)

### ******4、数据源工具类******

![](https://img-blog.csdnimg.cn/20190312083656305.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3dlaXhpbl80MDg3MTQ1NQ==,size_16,color_FFFFFF,t_70)

![](https://img-blog.csdnimg.cn/20190312083800545.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3dlaXhpbl80MDg3MTQ1NQ==,size_16,color_FFFFFF,t_70)

## ******5、表现层使用Struts2框架实现******

### ******5.1、动作类：******

![](https://img-blog.csdnimg.cn/20190312083829220.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3dlaXhpbl80MDg3MTQ1NQ==,size_16,color_FFFFFF,t_70)

### ******5.2、配置文件******

![](https://img-blog.csdnimg.cn/20190312084133703.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3dlaXhpbl80MDg3MTQ1NQ==,size_16,color_FFFFFF,t_70)

### ******5.3、注册界面和结果视图******

****注册界面：****

![](https://img-blog.csdnimg.cn/20190312084146907.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3dlaXhpbl80MDg3MTQ1NQ==,size_16,color_FFFFFF,t_70)

****结果视图：****

![](https://img-blog.csdnimg.cn/20190312084210568.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3dlaXhpbl80MDg3MTQ1NQ==,size_16,color_FFFFFF,t_70)

# ******四、数据类型的转换（明白原理，实际开发中几乎不用）******

## ******1、开发中的情况：******

实际开发中用户通过浏览器输入的数据都是String或者String[]。

String/String[]————填充模型(set方法)————>POJO(plain old java object)  pojo中有java的数据类型。

POJO————————获取(get方法)————>页面展示：String

## ******2、类型转换情况******

写数据：（增，删，改）都是String或String[]数组转换为其他类型。

读数据：（查）其他类型转换为String。

## ******3、Struts2提供的常用类型转换******

a.基本数据类型自动转换。

b.日期类型：默认按照本地日期格式转换（yyyy-MM-dd）。

c.字符串数组：默认用逗号+空格，连接成一个字符串。

## ******4、自定义类型转换器（知道）******

示例：把日期格式按照 MM/dd/yyyy的格式转换

### ******4.1、Struts2中的类型转换器结构：******

![](https://img-blog.csdnimg.cn/20190312084352186.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3dlaXhpbl80MDg3MTQ1NQ==,size_16,color_FFFFFF,t_70)

![](https://img-blog.csdnimg.cn/20190312084511752.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3dlaXhpbl80MDg3MTQ1NQ==,size_16,color_FFFFFF,t_70)

![](https://img-blog.csdnimg.cn/20190312084539454.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3dlaXhpbl80MDg3MTQ1NQ==,size_16,color_FFFFFF,t_70)

![](https://img-blog.csdnimg.cn/20190312084626262.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3dlaXhpbl80MDg3MTQ1NQ==,size_16,color_FFFFFF,t_70)

### ******4.2、编写类型转换器（编写一个类继承StrutsTypeConverter，实现抽象方法）******

![](https://img-blog.csdnimg.cn/20190312084738688.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3dlaXhpbl80MDg3MTQ1NQ==,size_16,color_FFFFFF,t_70)

### ******4.3、注册类型转换器******

****局部类型转换器：只能指定javabean中的属性用****

按照属性来注册。在属性所属的javabean的包下建立一个.properties文件。文件名称：javabean名称-conversion.properties

![](https://img-blog.csdnimg.cn/20190312085031490.png)

全局类型转换器：（推荐）

按照要转换的数据类型来注册。

at the top op classpath，建立一个固定名称xwork-conversion.properties的属性文件。

![](https://img-blog.csdnimg.cn/20190312085102828.png)

![](https://img-blog.csdnimg.cn/20190312085108953.png)

## ******5、转换失败后的处理（需要掌握）******

当转换失败后，页面提示：

![](https://img-blog.csdnimg.cn/20190312085140432.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3dlaXhpbl80MDg3MTQ1NQ==,size_16,color_FFFFFF,t_70)

解决办法：配置回显结果视图

![](https://img-blog.csdnimg.cn/20190312085149411.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3dlaXhpbl80MDg3MTQ1NQ==,size_16,color_FFFFFF,t_70)

问题：

配置了回显视图后，当转换失败时，可以回到请求页面，但是表单数据都没了？

![](https://img-blog.csdnimg.cn/2019031208522055.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3dlaXhpbl80MDg3MTQ1NQ==,size_16,color_FFFFFF,t_70)

显示错误提示：借助Struts2的标签库。

![](https://img-blog.csdnimg.cn/20190312085227604.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3dlaXhpbl80MDg3MTQ1NQ==,size_16,color_FFFFFF,t_70)

回显数据：使用struts2的标签库生成表单。（建议使用）

![](https://img-blog.csdnimg.cn/20190312085237262.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3dlaXhpbl80MDg3MTQ1NQ==,size_16,color_FFFFFF,t_70)

![](https://img-blog.csdnimg.cn/20190312085341514.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3dlaXhpbl80MDg3MTQ1NQ==,size_16,color_FFFFFF,t_70)

错误信息中文提示：使用的是struts2的国际化。

![](https://img-blog.csdnimg.cn/20190312085352733.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3dlaXhpbl80MDg3MTQ1NQ==,size_16,color_FFFFFF,t_70)

![](https://img-blog.csdnimg.cn/201903120854370.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3dlaXhpbl80MDg3MTQ1NQ==,size_16,color_FFFFFF,t_70)

问题：

类型转换器当转换失败后，如何进入input视图的？

原因：

是由一个叫做conversionError的拦截器完成的。

注意：

要想使用类型转换中的错误处理，在定义Action时必须继承ActionSupport

# ******五、数据验证******

****用户的输入验证，必须做，且工作量巨大。****

## ******1、验证的方式******

客户端验证：javascript

服务端验证：逻辑验证（我们的代码）

注意：如果客户端和服务端二选一的话，服务器端的不能省。

实际开发中：客户端+服务端

## ******2、Struts2的服务端验证******

### ******2.1、编程式验证******

****前提：****

****动作类必须继承ActionSupport****

****在代码中编写验证规则。****

a、针对动作类中的所有动作方法进行验证：

在动作类中覆盖public void validate()方法。

![](https://img-blog.csdnimg.cn/20190312085541260.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3dlaXhpbl80MDg3MTQ1NQ==,size_16,color_FFFFFF,t_70)

![](https://img-blog.csdnimg.cn/20190312085930270.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3dlaXhpbl80MDg3MTQ1NQ==,size_16,color_FFFFFF,t_70)

![](https://img-blog.csdnimg.cn/20190312085953552.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3dlaXhpbl80MDg3MTQ1NQ==,size_16,color_FFFFFF,t_70)

但是当我们再写一个动作方法时：

![](https://img-blog.csdnimg.cn/2019031209002043.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3dlaXhpbl80MDg3MTQ1NQ==,size_16,color_FFFFFF,t_70)

![](https://img-blog.csdnimg.cn/20190312090031147.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3dlaXhpbl80MDg3MTQ1NQ==,size_16,color_FFFFFF,t_70)

![](https://img-blog.csdnimg.cn/20190312085839214.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3dlaXhpbl80MDg3MTQ1NQ==,size_16,color_FFFFFF,t_70)

![](https://img-blog.csdnimg.cn/20190312085856155.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3dlaXhpbl80MDg3MTQ1NQ==,size_16,color_FFFFFF,t_70)



由此可知，该验证方法会对动作类中的所有动作方法进行验证。

b、针对动作类中的某个动作方法进行验证

针对上面的问题，解决办法1：给不需要验证的动作方法添加一个@SkipValidation注解。

![](https://img-blog.csdnimg.cn/20190312090322934.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3dlaXhpbl80MDg3MTQ1NQ==,size_16,color_FFFFFF,t_70)

![](https://img-blog.csdnimg.cn/20190312090339711.png)

解决办法2：validation方法遵守书写规范。

![](https://img-blog.csdnimg.cn/20190312090348672.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3dlaXhpbl80MDg3MTQ1NQ==,size_16,color_FFFFFF,t_70)

![](https://img-blog.csdnimg.cn/20190312090412392.png)

解决办法1和解决办法2的使用时机：需要验证的动作方法少，用解决办法2。需要验证的方法多，用解决方式1。（简单一点：挑少的写）

****所有编程式验证的弊端：硬编码。****

### ******2.2、声明式验证（推荐）******

****通过编写验证规则的xml文件。需要验证时，编写xml文件，不要验证，就不写。****

****优势：解决了2.1编程式验证的弊端****

a、针对动作类中的所有动作进行验证：在动作类所在的包中，建立一个****ActionClassName-validation.xml****的文件，内容如下：

![](https://img-blog.csdnimg.cn/2019031209043172.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3dlaXhpbl80MDg3MTQ1NQ==,size_16,color_FFFFFF,t_70)

注意：它是针对动作类中的所有动作方法。

![](https://img-blog.csdnimg.cn/20190312090559668.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3dlaXhpbl80MDg3MTQ1NQ==,size_16,color_FFFFFF,t_70)

![](https://img-blog.csdnimg.cn/20190312090605930.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3dlaXhpbl80MDg3MTQ1NQ==,size_16,color_FFFFFF,t_70)

b、针对动作类中的某个动作进行验证：在动作类所在的包中建立一个xml文件，名称为****ActionClassName-ActionName-validation.xml****。内容如下：

![](https://img-blog.csdnimg.cn/20190312090700765.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3dlaXhpbl80MDg3MTQ1NQ==,size_16,color_FFFFFF,t_70)

它是针对指定动作方法进行验证：

![](https://img-blog.csdnimg.cn/20190312090742328.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3dlaXhpbl80MDg3MTQ1NQ==,size_16,color_FFFFFF,t_70)

![](https://img-blog.csdnimg.cn/20190312090757227.png)

### ******2.3、Struts2内置的常用声明式验证器******

******2.3.1位置：******

****xwork-core-2.3.15.3.jar\com\opensymphony\xwork2\validator\validator********\default.xml****![](https://img-blog.csdnimg.cn/20190312090902916.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3dlaXhpbl80MDg3MTQ1NQ==,size_16,color_FFFFFF,t_70)

******2.3.2、验证器注入参数******

例如：我们使用requiredstring，默认是去空格，当我们不想去空格时，就可以给验证器注入参数。

****基于字段的：****

![](https://img-blog.csdnimg.cn/20190312091000371.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3dlaXhpbl80MDg3MTQ1NQ==,size_16,color_FFFFFF,t_70)

****另一种基于验证器的：****

![](https://img-blog.csdnimg.cn/20190312091049686.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3dlaXhpbl80MDg3MTQ1NQ==,size_16,color_FFFFFF,t_70)

![](https://img-blog.csdnimg.cn/20190312091152953.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3dlaXhpbl80MDg3MTQ1NQ==,size_16,color_FFFFFF,t_70)

## ******3、常用验证器示例******

![](https://img-blog.csdnimg.cn/20190312091204220.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3dlaXhpbl80MDg3MTQ1NQ==,size_16,color_FFFFFF,t_70)

![](https://img-blog.csdnimg.cn/20190312091213163.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3dlaXhpbl80MDg3MTQ1NQ==,size_16,color_FFFFFF,t_70)

![](https://img-blog.csdnimg.cn/20190312091233328.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3dlaXhpbl80MDg3MTQ1NQ==,size_16,color_FFFFFF,t_70)

![](https://img-blog.csdnimg.cn/20190312091255330.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3dlaXhpbl80MDg3MTQ1NQ==,size_16,color_FFFFFF,t_70)

![](https://img-blog.csdnimg.cn/20190312091306656.png)

运行结果：

![](https://img-blog.csdnimg.cn/20190312091428489.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3dlaXhpbl80MDg3MTQ1NQ==,size_16,color_FFFFFF,t_70)

整理自黑马程序员课堂笔记  



