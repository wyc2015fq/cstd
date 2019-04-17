# Struts2（四）ContextMap、Struts2对EL表达式的改变、常用标签、防止表单重复提交 - ChihkAnchor的博客 - CSDN博客





2019年03月15日 08:58:15[Chihk-Anchor](https://me.csdn.net/weixin_40871455)阅读数：12标签：[Struts2](https://so.csdn.net/so/search/s.do?q=Struts2&t=blog)
个人分类：[Java																[Javaweb](https://blog.csdn.net/weixin_40871455/article/category/8734001)](https://blog.csdn.net/weixin_40871455/article/category/8560856)








# ******一、contextMap中的数据操作******

![](https://img-blog.csdnimg.cn/20190314075311260.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3dlaXhpbl80MDg3MTQ1NQ==,size_16,color_FFFFFF,t_70)
******1、存数据：******

需要熟悉ActionContext和valueStack的API。框架为我们存数据。

### ******1.1、利用ActionContext存数据******

![](https://img-blog.csdnimg.cn/20190314075357705.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3dlaXhpbl80MDg3MTQ1NQ==,size_16,color_FFFFFF,t_70)

### ******1********.2、利用ValueStack存数据******

a、如何获取ValueStack：

![](https://img-blog.csdnimg.cn/20190314075647364.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3dlaXhpbl80MDg3MTQ1NQ==,size_16,color_FFFFFF,t_70)

b、ValueStack中的getRoot()方法：

![](https://img-blog.csdnimg.cn/20190314080114597.png)

c、CompoundRoot是什么：

![](https://img-blog.csdnimg.cn/201903140801241.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3dlaXhpbl80MDg3MTQ1NQ==,size_16,color_FFFFFF,t_70)

d、栈操作：

![](https://img-blog.csdnimg.cn/20190314080213267.png)

## ******2、取数据：用Struts2的标签（OGNL表达式)在JSP上（用的最多）******

使用OGNL表达式来取，struts2的OGNL表达式必须写在struts2标签中。

### ******2.1、使用s:property取数据******

****a********、取contextMap中的数据，需使用#****

![](https://img-blog.csdnimg.cn/20190314080303165.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3dlaXhpbl80MDg3MTQ1NQ==,size_16,color_FFFFFF,t_70)![](https://img-blog.csdnimg.cn/20190314080312397.png)

![](https://img-blog.csdnimg.cn/20190314080350508.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3dlaXhpbl80MDg3MTQ1NQ==,size_16,color_FFFFFF,t_70)

****b********、取********contextMap********里面ValueStack中对象的属性：直接写属性名****

![](https://img-blog.csdnimg.cn/20190314080411524.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3dlaXhpbl80MDg3MTQ1NQ==,size_16,color_FFFFFF,t_70)

![](https://img-blog.csdnimg.cn/20190314080444797.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3dlaXhpbl80MDg3MTQ1NQ==,size_16,color_FFFFFF,t_70)

****如果遇有对象属性重名，可以通过OGNL表达式，选择查找的起始位置****

![](https://img-blog.csdnimg.cn/20190314080500614.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3dlaXhpbl80MDg3MTQ1NQ==,size_16,color_FFFFFF,t_70)

![](https://img-blog.csdnimg.cn/20190314080550466.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3dlaXhpbl80MDg3MTQ1NQ==,size_16,color_FFFFFF,t_70)![](https://img-blog.csdnimg.cn/20190314080633654.png)

![](https://img-blog.csdnimg.cn/20190314080711403.png)

****特殊说明：当s:property不给定value属性时，默认取栈顶对象。****

![](https://img-blog.csdnimg.cn/2019031408073324.png)

![](https://img-blog.csdnimg.cn/20190314080753687.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3dlaXhpbl80MDg3MTQ1NQ==,size_16,color_FFFFFF,t_70)

****OGNL的使用总结：****

****1.取********根********中对象的********属性********，********不使用#********。****

****2.取********contextMap********中********key********的值，********要使用#********。****

### ******2.2********、ValueStack的其他方法********：******

****a********、setValue方法****

![](https://img-blog.csdnimg.cn/20190314080819616.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3dlaXhpbl80MDg3MTQ1NQ==,size_16,color_FFFFFF,t_70)![](https://img-blog.csdnimg.cn/20190314080843381.png)

![](https://img-blog.csdnimg.cn/20190314080913217.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3dlaXhpbl80MDg3MTQ1NQ==,size_16,color_FFFFFF,t_70)

在Jsp页面上获取【李四】

![](https://img-blog.csdnimg.cn/20190314080935453.png)![](https://img-blog.csdnimg.cn/2019031408094241.png)

****b********、set方法****

![](https://img-blog.csdnimg.cn/20190314081127970.png)

![](https://img-blog.csdnimg.cn/20190314081209993.png)![](https://img-blog.csdnimg.cn/2019031408120591.png)

****细节问题：****

****当按照如下方式，往根中存放数据时，根中元素是什么顺序？****

![](https://img-blog.csdnimg.cn/20190314081253319.png)

****使用<s:debug/>标签查看：****

![](https://img-blog.csdnimg.cn/20190314081328358.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3dlaXhpbl80MDg3MTQ1NQ==,size_16,color_FFFFFF,t_70)

****c********、findValue：我们在Jsp上调用的都是findValue****

![](https://img-blog.csdnimg.cn/20190314081420829.png)

![](https://img-blog.csdnimg.cn/20190314081459198.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3dlaXhpbl80MDg3MTQ1NQ==,size_16,color_FFFFFF,t_70)

# ******二、Struts2对EL的改变******

## ******1、Struts2中使用EL的问题：******

****前提：****

我们应该知道，如果我们没有往值栈(根)中放入数据的话，那么我们的动作类默认是在值栈的栈顶。

![](https://img-blog.csdnimg.cn/20190314081551967.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3dlaXhpbl80MDg3MTQ1NQ==,size_16,color_FFFFFF,t_70)![](https://img-blog.csdnimg.cn/20190314082318240.png)![](https://img-blog.csdnimg.cn/20190314081612574.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3dlaXhpbl80MDg3MTQ1NQ==,size_16,color_FFFFFF,t_70)

![](https://img-blog.csdnimg.cn/2019031408163454.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3dlaXhpbl80MDg3MTQ1NQ==,size_16,color_FFFFFF,t_70)![](https://img-blog.csdnimg.cn/2019031408240115.png)

![](https://img-blog.csdnimg.cn/20190314081657268.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3dlaXhpbl80MDg3MTQ1NQ==,size_16,color_FFFFFF,t_70)

问题：

我们放到请求域中的属性，使用EL表达式取出来了。但是放到应用域中的属性，使用EL表达式没取出来。

## ******2、关于EL问题的分析：******

****分析：****我们知道EL表达式是从四大域对象中依次查找属性。搜索范围是由小到大。page Scope————>request Scope————>sessionScope————>application Scope

但是通过测试发现，搜索完request范围后就没有继续搜索，而是返回了ValueStack中栈顶对象name属性的值。

![](https://img-blog.csdnimg.cn/20190314081842175.png)

![](https://img-blog.csdnimg.cn/20190314081910589.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3dlaXhpbl80MDg3MTQ1NQ==,size_16,color_FFFFFF,t_70)

## ******3、Struts2中EL查找顺序改变总结：******

****  EL表达式： page Scope————>request Scope————>sessionScope————>application Scope****

****OGNL表达式：********page Scope————>request Scope————>********valueStack（根中）————>contextMap********————>sessionScope————>application Scope****

## ******4、OGNL的特殊说明:******

![](https://img-blog.csdnimg.cn/20190314082024915.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3dlaXhpbl80MDg3MTQ1NQ==,size_16,color_FFFFFF,t_70)![](https://img-blog.csdnimg.cn/2019031408250990.png)![](https://img-blog.csdnimg.cn/20190314082039249.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3dlaXhpbl80MDg3MTQ1NQ==,size_16,color_FFFFFF,t_70)

****注意：以下内容知道即可。****

![](https://img-blog.csdnimg.cn/20190314082151443.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3dlaXhpbl80MDg3MTQ1NQ==,size_16,color_FFFFFF,t_70)

# ******三、OGNL配合通用标签的其他使用******

## ******1********、iterator标签（很重要）******

![](https://img-blog.csdnimg.cn/20190314082612321.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3dlaXhpbl80MDg3MTQ1NQ==,size_16,color_FFFFFF,t_70)![](https://img-blog.csdnimg.cn/20190314082709600.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3dlaXhpbl80MDg3MTQ1NQ==,size_16,color_FFFFFF,t_70)

![](https://img-blog.csdnimg.cn/2019031408275789.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3dlaXhpbl80MDg3MTQ1NQ==,size_16,color_FFFFFF,t_70)

## ******2********、OGNL投影（了解）******

### ******2********.1、使用过滤条件投影******

![](https://img-blog.csdnimg.cn/20190314083014604.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3dlaXhpbl80MDg3MTQ1NQ==,size_16,color_FFFFFF,t_70)

### ******2.2、投影指定属性******

![](https://img-blog.csdnimg.cn/2019031408302728.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3dlaXhpbl80MDg3MTQ1NQ==,size_16,color_FFFFFF,t_70)

## ******3、********Struts2********中#,$,%符号的使用********(********重要********)******

### ******3.1********、#******

a、取contextMap中key时使用，例如<s:property value="#name" />

b、OGNL中创建Map对象时使用，例如：<s:radio list="#{'male':'男','female':'女'}" />

### ******3.2、$******

a、在JSP中使用EL表达式时使用，例如${name}

b、在xml配置文件中，编写OGNL表达式时使用，例如文件下载时，文件名编码。

struts.xml——>${@java.net.URLEncoder.encode(filename)}

### ******3.3、%******

在struts2中，有些标签的value属性取值就是一个OGNL表达式，例如<s:property value="OGNL Expression" />

还有一部分标签，value属性的取值就是普通字 符串，例如<s:textfield value="username"/>，如果想把一个普通的字符串强制看成时OGNL，就需要使用%{}把字符串套起来。

例如<s:textfield value="%{username}"/>。当然在<s:property value="%{OGNL Expression}" />也可以使用，但不会这么用。

## ******4、其他标签******

### ******4.1、set标签******

![](https://img-blog.csdnimg.cn/20190314083314864.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3dlaXhpbl80MDg3MTQ1NQ==,size_16,color_FFFFFF,t_70)![](https://img-blog.csdnimg.cn/2019031408381481.png)

### ******4.2********、action标签******

![](https://img-blog.csdnimg.cn/20190314083819869.png)![](https://img-blog.csdnimg.cn/20190314083825988.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3dlaXhpbl80MDg3MTQ1NQ==,size_16,color_FFFFFF,t_70)

### ******4.3********、if标签，elseif标签 else标签******

![](https://img-blog.csdnimg.cn/20190314083909322.png)![](https://img-blog.csdnimg.cn/20190314084007905.png)

### ******4.4、url和a标签（很有用）******

![](https://img-blog.csdnimg.cn/20190314084015277.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3dlaXhpbl80MDg3MTQ1NQ==,size_16,color_FFFFFF,t_70)

# ******四、Struts2的UI标签和主题******

## ******1********、Struts2中UI标签的优势******

自动的数据回显和错误提示功能

自带的简单样式和排版

## ******2、表单标签的通用属性******

 说明：UI标签中value的取值一般都是字符串。

### ******2.1、UI标签的通用属性******

![](https://img-blog.csdnimg.cn/20190314084217993.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3dlaXhpbl80MDg3MTQ1NQ==,size_16,color_FFFFFF,t_70)

### ******2.2、关于checkboxlist的使用：******

![](https://img-blog.csdnimg.cn/20190314084235934.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3dlaXhpbl80MDg3MTQ1NQ==,size_16,color_FFFFFF,t_70)

![](https://img-blog.csdnimg.cn/20190314084304850.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3dlaXhpbl80MDg3MTQ1NQ==,size_16,color_FFFFFF,t_70)

![](https://img-blog.csdnimg.cn/20190314084324313.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3dlaXhpbl80MDg3MTQ1NQ==,size_16,color_FFFFFF,t_70)

![](https://img-blog.csdnimg.cn/20190314084335289.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3dlaXhpbl80MDg3MTQ1NQ==,size_16,color_FFFFFF,t_70)

### ******2.3、UI标签的小案例以及模型驱动的分析******

![](https://img-blog.csdnimg.cn/20190314084437850.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3dlaXhpbl80MDg3MTQ1NQ==,size_16,color_FFFFFF,t_70)

![](https://img-blog.csdnimg.cn/20190314084800154.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3dlaXhpbl80MDg3MTQ1NQ==,size_16,color_FFFFFF,t_70)

![](https://img-blog.csdnimg.cn/20190314084817539.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3dlaXhpbl80MDg3MTQ1NQ==,size_16,color_FFFFFF,t_70)![](https://img-blog.csdnimg.cn/20190314084949682.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3dlaXhpbl80MDg3MTQ1NQ==,size_16,color_FFFFFF,t_70)

## ******3********、UI标签的模板（主题）******

### ******3.1、struts2中默认主题******

****默认主题********的名称是XHTML，都是在struts的默认属性文件中定义着:default.properties****

![](https://img-blog.csdnimg.cn/20190314085033294.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3dlaXhpbl80MDg3MTQ1NQ==,size_16,color_FFFFFF,t_70)

![](https://img-blog.csdnimg.cn/20190314085053434.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3dlaXhpbl80MDg3MTQ1NQ==,size_16,color_FFFFFF,t_70)![](https://img-blog.csdnimg.cn/20190314085059598.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3dlaXhpbl80MDg3MTQ1NQ==,size_16,color_FFFFFF,t_70)

### ******3.2、更改默认主题******

a、更改表单某个元素的默认主题：使用的是表单元素的theme属性。

![](https://img-blog.csdnimg.cn/20190314085108537.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3dlaXhpbl80MDg3MTQ1NQ==,size_16,color_FFFFFF,t_70)

b、更改表单所有主题：使用的是form标签的theme属性。

![](https://img-blog.csdnimg.cn/20190314085137504.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3dlaXhpbl80MDg3MTQ1NQ==,size_16,color_FFFFFF,t_70)

c、更改全站所有表单主题：是在struts.xml配置文件中，覆盖原有主题的设置。

![](https://img-blog.csdnimg.cn/20190314085206533.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3dlaXhpbl80MDg3MTQ1NQ==,size_16,color_FFFFFF,t_70)

# ******五、防止表单重复提交（拦截器）******

## ******1********、回顾之前的解决办法：******

![](https://img-blog.csdnimg.cn/20190314085251589.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3dlaXhpbl80MDg3MTQ1NQ==,size_16,color_FFFFFF,t_70)

## ******2、Struts2中的解决办法：******

### ******2.1、使用重定向******

![](https://img-blog.csdnimg.cn/20190314085318235.png)

****遗留的问题：防不住后退，再提交。****

### ******2.2********、使用<s:token********/>********生成令牌配合token拦截器******

![](https://img-blog.csdnimg.cn/20190314085336524.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3dlaXhpbl80MDg3MTQ1NQ==,size_16,color_FFFFFF,t_70)

![](https://img-blog.csdnimg.cn/2019031408535360.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3dlaXhpbl80MDg3MTQ1NQ==,size_16,color_FFFFFF,t_70)

点击后退的时候，会提示：

![](https://img-blog.csdnimg.cn/20190314085439763.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3dlaXhpbl80MDg3MTQ1NQ==,size_16,color_FFFFFF,t_70)

****配置结果视图：****

![](https://img-blog.csdnimg.cn/20190314085446805.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3dlaXhpbl80MDg3MTQ1NQ==,size_16,color_FFFFFF,t_70)

****遗留的问题：此种解决方式，是产生了错误之后再告知用户，你错了。****

### ******2.3********、********使用<s:token********/>********生成令牌配合tokensession拦截器******

![](https://img-blog.csdnimg.cn/20190314085512406.png)

整理自黑马程序员课堂笔记  




