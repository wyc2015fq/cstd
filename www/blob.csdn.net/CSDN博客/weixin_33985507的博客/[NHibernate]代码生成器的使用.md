# [NHibernate]代码生成器的使用 - weixin_33985507的博客 - CSDN博客
2014年11月15日 20:23:00[weixin_33985507](https://me.csdn.net/weixin_33985507)阅读数：4
### 目录
[写在前面](#t)
[文档与系列文章](#t)
[代码生成器的使用](#t)
[总结](#t)
### 写在前面
前面的文章介绍了nhibernate的相关知识，都是自己手敲的代码，有时候显得特别的麻烦，比如你必须编写持久化类，映射文件等等，举得例子比较简单，字段比较少，如果一个数据表有几十个字段，你手敲的话烦都烦死了，不过为了学习nhibernate的语法，初学的过程还是建议自己动手，这样敲的多了就对每个过程，及映射文件中的节点的含义有个大致印象。前面举的例子，你也会发现这种方式的步骤就是首先创建数据库，然后编写持久化类，映射文件，最后编写操作数据的代码，这种方式是典型的数据库驱动开发（DbDD，Database-Driven Developent)技术。这个步骤能不能反过来？就好比EF中的Code First方式，先编码，然后再生成数据库。当然可以，可以借助SchemaExport工具把数据库生成了。这里还是先说说前面使用过得MyGeneration代码生成器的使用把。
什么是数据库驱动模型？
> 
对象模型随着数据库架构的变化而变化。那为什么还需要这种模型？
一：数据库是遗留的系统使用的，并且数据库中已有大量的数据，不能随便更改。
二：你的数据库架构按照需求分析基本上确定了，不要做任何改动，在系统设计初期已经把数据库建好了。
使用数据库驱动模型流程就是当数据库架构修改时候，映射文件和持久化类通过代码生成工具重新生成一下。我们只要修改应用程序即可。
参考：[http://www.cnblogs.com/lyj/archive/2008/11/10/1330542.html](http://www.cnblogs.com/lyj/archive/2008/11/10/1330542.html)
### 文档与系列文章
**[[Nhibernate]体系结构](http://www.cnblogs.com/wolf-sun/p/3694592.html)**
**[[NHibernate]ISessionFactory配置](http://www.cnblogs.com/wolf-sun/p/3694901.html)**
**[[NHibernate]持久化类(Persistent Classes)](http://www.cnblogs.com/wolf-sun/p/3704012.html)**
**[[NHibernate]O/R Mapping基础](http://www.cnblogs.com/wolf-sun/p/3705229.html)**
**[[NHibernate]集合类（Collections）映射](http://www.cnblogs.com/wolf-sun/p/3714577.html)**
**[[NHibernate]关联映射](http://www.cnblogs.com/wolf-sun/p/3720259.html)**
**[[NHibernate]Parent/Child](http://www.cnblogs.com/wolf-sun/p/3721528.html)**
**[[NHibernate]缓存（NHibernate.Caches)](http://www.cnblogs.com/wolf-sun/p/3724052.html)**
**[[NHibernate]NHibernate.Tool.hbm2net](http://www.cnblogs.com/wolf-sun/p/3734249.html)**
**[[NHibernate]Nullables](http://www.cnblogs.com/wolf-sun/p/3734313.html)**
**[[NHibernate]Nhibernate如何映射sqlserver中image字段](http://www.cnblogs.com/wolf-sun/p/3956802.html)**
**[[NHibernate]基本配置与测试](http://www.cnblogs.com/wolf-sun/p/4028392.html)**
**[[NHibernate]HQL查询](http://www.cnblogs.com/wolf-sun/p/4034510.html)**
**[[NHibernate]条件查询Criteria Query](http://www.cnblogs.com/wolf-sun/p/4046672.html)**
**[[NHibernate]增删改操作](http://www.cnblogs.com/wolf-sun/p/4048048.html)**
**[[NHibernate]事务](http://www.cnblogs.com/wolf-sun/p/4049716.html)**
**[[NHibernate]并发控制](http://www.cnblogs.com/wolf-sun/p/4050714.html)**
**[[NHibernate]组件之依赖对象](http://www.cnblogs.com/wolf-sun/p/4067026.html)**
**[[NHibernate]一对多关系（级联删除，级联添加）](http://www.cnblogs.com/wolf-sun/p/4068749.html)**
**[[NHibernate]一对多关系（关联查询）](http://www.cnblogs.com/wolf-sun/p/4070935.html)**
**[[NHibernate]多对多关系（关联查询）](http://www.cnblogs.com/wolf-sun/p/4074654.html)**
**[[NHibernate]延迟加载](http://www.cnblogs.com/wolf-sun/p/4077226.html)**
**[[NHibernate]立即加载](http://www.cnblogs.com/wolf-sun/p/4082432.html)**
**[[NHibernate]视图处理](http://www.cnblogs.com/wolf-sun/p/4082899.html)**
**[[NHibernate]N+1 Select查询问题分析](http://www.cnblogs.com/wolf-sun/p/4083402.html)**
**[[NHibernate]存储过程的使用（一）](http://www.cnblogs.com/wolf-sun/p/4085314.html)**
**[[NHibernate]存储过程的使用（二）](http://www.cnblogs.com/wolf-sun/p/4088288.html)**
**[[NHibernate]存储过程的使用（三）](http://www.cnblogs.com/wolf-sun/p/4093539.html)**
### 代码生成器的使用
前面文章中已经介绍了一种开源的代码生成器MyGeneration，那么本篇文章我们继续介绍如何使用开源MyGeneration代码生成工具根据数据库架构生成映射文件和持久化类。（假设你已经安装了MyGeneration），由于代码生成器操作步骤类似，这里就引用李永京的文章中图进行说明了，也不考虑版本之间的差异了，知道使用的步骤就可以了。
1、点击“模板浏览器”窗口的第三个“在线更新”按钮：来在线下载模板。
![](https://images0.cnblogs.com/blog/511616/201411/150940554062625.x-png)
 2、出现“在线模板库”窗口，在“在线模板库”中提供了各种各样的模板，找到NHibernate节点：
![](https://images0.cnblogs.com/blog/511616/201411/152017279069934.x-png)
3、展开NHibernate节点，找到“NHibernate Sharp 1.4 [sharpm64]”模板，右键点击“保存”。这个模板就自动保存到本地模板文件夹中。
![](https://images0.cnblogs.com/blog/511616/201411/152020314536327.x-png)
4、点击“模板浏览器”窗口的第一个“刷新”按钮，这个模板就在“模板浏览器”可以看见了，展开NHibernate节点，右击“执行”NHibernate Sharp1.4模板。
![](https://images0.cnblogs.com/blog/511616/201411/152021241713988.x-png)
5、这就是NHibernate OHM界面窗口，右面显示表、视图、操作标签，在表标签界面上，右边有不映射、映射等按钮，在表中不同的颜色代表字段不同的属性。
![](https://images0.cnblogs.com/blog/511616/201411/152022193431531.x-png)
6、点击“CustomerId”列，右边显示了这个表，并在状态栏显示了“CustomerId”列的属性：int类型、主键。另外可以自己摸索一下：
![](https://images0.cnblogs.com/blog/511616/201411/152023426871670.x-png)
7、点击“视图”标签，设置CustomerId为主键。
![](https://images0.cnblogs.com/blog/511616/201411/152024241877524.x-png)
8、点击“操作”标签，我们具体设置程序集名称和命名空间名称，这里我设置GenDomainModel，设置生成工程，由于这个模板不支持VS2008，所以选择生成IDE版本为VS2005。设置输出路径。点击OK按钮。
![](https://images0.cnblogs.com/blog/511616/201411/152025433126651.x-png)
9、这时打开文件夹，代码生成工具根据数据库架构生成了相应的持久化类和映射文件。
![](https://images0.cnblogs.com/blog/511616/201411/152026225782623.x-png)
10、用VS2008打开，VS2008自动升级GenDomainModel.csproj解决方案文件，解决方案项目文件如下
![](https://images0.cnblogs.com/blog/511616/201411/152027550463940.x-png)
11、这时就可以使用了，可以把这个类库利用添加现有项目功能添加到我们的项目中作为实体持久层，我们利用这个实体持久层编写数据访问层方法实现对数据库的CRUD操作，当然在使用前要搞清楚这个项目的类库架构，另外由于模板本身还不是很完善（例如这个模板不支持版本控制映射、不支持多对多直接映射），所以我们还需要按照实际情况去修改持久化类和映射，显然无形中增加了一些负担。
注意不要忘记在hibernate.cfg.xml中修改<mapping assembly="DomainModel"/>为<mapping assembly="GenDomainModel"/>。
### 总结
关于代码生成器的部分就说到这里，毕竟是一个工具，在项目中我用的是动软的代码生成器，工具是一个辅助的作用，少敲点代码，哪个用的顺手用哪个！
来源：[http://www.cnblogs.com/lyj/archive/2008/11/10/1330542.html](http://www.cnblogs.com/lyj/archive/2008/11/10/1330542.html)
