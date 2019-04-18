# 【MyBatis学习05】SqlMapConfig.xml文件中的配置总结 - z69183787的专栏 - CSDN博客
2017年07月25日 11:27:29[OkidoGreen](https://me.csdn.net/z69183787)阅读数：534
经过上两篇博文的总结，对mybatis中的dao开发方法和流程基本掌握了，这一节主要来总结一下mybatis中的全局配置文件SqlMapConfig.xml在开发中的一些常用配置，首先看一下该全局配置文件中都有哪些可以配置的东西：
|配置内容|作用|
|----|----|
|`<properties>`|用来加载属性文件|
|`<settings>`|用来设置全局参数|
|`<typeAliases>`|用来设置类型的别名|
|`<typeHandlers>`|用来设置类型处理器|
|`<objectFactory>`|用来设置对象工厂|
|`<plugins>`|用来设置插件|
|`<environments>`|用来设置mybatis的环境|
|`<mappers>`|用来配置映射文件|
从前面的博文中可以看出，`<environments>`是用来配置mybatis的环境的，在和[spring](http://lib.csdn.net/base/javaee)整合前是由mybatis自己管理的，但是和Spring整合后，这些配置都将被废弃掉，`<environments>`部分在前面已经有提到过，这里就不再赘述了。然后objectFactory和plugins一般我们不用，所以在这里就不总结了，`<typeHandlers>`类型处理器是mybatis中用来完成jdbc类型和[Java](http://lib.csdn.net/base/java)类型之间转换的，mybatis默认支持的类型处理器基本够我们使用了，一般不需要自定义。这篇博文主要来总结一下剩下的几个配置的用法。
## 1. properties加载属性文件
我们之前连接[数据库](http://lib.csdn.net/base/mysql)的一些配置都是在`<environments>`中写死的，在实际开发中肯定不是这样子的，我们需要将数据库连接参数单独配在db.properties文件中，然后在SqlMapConfig.xml中加载db.properties的属性值即可，这就用到了`<properties>`了。即：
```xml
<properties resource="db.properties">
```
- 1
- 1
然后environment中做一下对应的修改即可： 
![配置](https://img-blog.csdn.net/20160607160209321)
在`<properties>`内部还可以定义属性值：
```
<property
 name="" value=""/>
```
，在property中定义的属性也可以被`<evironments>`中来加载，这就涉及到了mybatis加载属性的顺序了：
> - 在`<properties>`元素体内定义的首先被读取
- 然后读取`<properties>`元素中resource或者url加载的属性，它会覆盖已读取的同名属性
- 最后读取parameterType传递的属性(使用${}这种方式，不是#{}这种方式)，它会覆盖已读的同名属性
建议：从上面可以看出来，如果在`<properties>`中定义属性的话，有点乱，搞不好就会出错，所以开发中不要在`<properties>`元素体内添加任何属性值，只将属性值定义在properties文件中。另外在properties文件中定义的属性名要有一定的特殊性，如xxx.xxx，这样不容易和其他的一些属性值起冲突。
## 2. settings全局参数配置
mybatis框架在运行时可以调整一些运行参数，比如：开启二级缓存、开启延迟加载等，有个settings全局参数配置文档： 
![settings配置](https://img-blog.csdn.net/20160607162158616)
这些全局参数的配置会影响mybatis的运行行为，需要的时候再设置，不需要的时候不能乱设置，这里就不做详细介绍了，等后面用到的时候我再针对相应的做一下介绍。说实在的，这么多我也懒得去看……
## 3. typeAliases类型别名
这是配置中的一个重点，我们知道，在mapper.xml中定义了很多的statement，statement需要parameterType来指定输入参数的类型、需要resultType来指定输出结果的类型。如果在指定类型时输入类型全路径，有时候会很长，不方便进行开发，那么我们就可以可以针对parameterType或resultType指定的类型定义一些别名，在mapper.xml中通过别名`<typeAliases>`来定义，方便开发。 
mybatis有默认支持的一些别名，一般基本类型都有别名，如下： 
![别名](https://img-blog.csdn.net/20160607170001340)
但是针对pojo的需要我们自定义别名了，比如我们将自己定义的User对象取个别名为user，如下：
```xml
<!-- 别名的定义 -->
<typeAliases>
    <!-- 针对单个别名的定义。type：类型的路径； alias：别名 -->     
    <typeAlias type="mybatis.po.User" alias="user"/>
</typeAliases>
```
- 1
- 2
- 3
- 4
- 5
- 1
- 2
- 3
- 4
- 5
然后输入参数或者输出结果为mybatis.po.User时，就可以用user来代替了，如下： 
![别名](https://img-blog.csdn.net/20160607170340127)
但是问题来了，如果工程中有很多pojo，那岂不是完蛋……这得定义多少个啊，所以mybatis帮我们解决了这个问题，它提供了批量别名的定义，如下：
```xml
<!-- 批量别名定义(这个常用)
指定一个包名，mybatis会自动的扫描包中po类，自动定义别名，别名就是类名（首字母大写或小写都可以） -->
<package name="mybatis.po"/>
```
- 1
- 2
- 3
- 1
- 2
- 3
这就爽了，如果进行了上面这样设置，那么所有放在mybatis.po包下的pojo都可以不用定义了，直接使用类名就可以了，所以我们直接使用user即可。毫无疑问，开发中肯定使用这个批量的别名定义。
## 3. mappers映射配置
在前面的几篇博文中可以看到，`<mappers>`标签是通过resource方法加载单个映射文件的，即：
```xml
<mappers>
    <mapper resource="sqlmap/User.xml" />
    <mapper resource="mapper/UserMapper.xml" />
</mappers>
```
- 1
- 2
- 3
- 4
- 1
- 2
- 3
- 4
当然咯，除了resource外，也可以用url来加载，只不过url指定的是绝对路径，硬盘中的路径。这里就不举例了，这里介绍另一种加载映射文件的方式：通过mapper接口来加载。通过mapper接口来加载的话要遵循一个规范：
> 
规范：需要将mapper接口类名和mapper.xml映射文件名称保持一致，且在一个目录中 
这个规范的前提是：使用的是mapper代理的方法
所以上面的配置就可以改成
```xml
<mappers>
    <mapper resource="sqlmap/User.xml" />
    <mapper class="mybatis.mapper.UserMapper"/>
</mappers>
```
- 1
- 2
- 3
- 4
- 1
- 2
- 3
- 4
- 
所以要把原来的UserMapper.xml从config/mapper中的拖到和UserMapper.java一个目录下且同名，如下： 
![mapper接口配置](https://img-blog.csdn.net/20160607175003349)
但是还是有个弊端，就跟上面那个别名的配置一样，如果现在有好多个mapper咋整，所以mybatis也给我们提供了批量加载方法：我们只要指定mapper接口的包名，mybatis自动扫描包下面所有的mapper接口进行加载。当然，还是要遵循上面的那个规范。所以我们可以修改上面的配置：
```xml
<mappers>
    <mapper resource="sqlmap/User.xml" />
     <package name="mybatis.mapper"/>
</mappers>
```
- 1
- 2
- 3
- 4
- 1
- 2
- 3
- 4
这样mybatis.mapper包下的所有mapper.xml映射文件都可以加载了，这就更加简洁了。在开发中推荐使用这种批量加载映射文件的方法。 
关于SqlMapConfig.xml文件中的配置就总结这么多吧~ 
　　相关阅读：[http://blog.csdn.net/column/details/smybatis.html](http://blog.csdn.net/column/details/smybatis.html)
