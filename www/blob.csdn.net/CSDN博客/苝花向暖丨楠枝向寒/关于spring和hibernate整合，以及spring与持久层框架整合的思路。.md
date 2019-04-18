# 关于spring和hibernate整合，以及spring与持久层框架整合的思路。 - 苝花向暖丨楠枝向寒 - CSDN博客

2018年05月27日 17:03:13[苝花向暖丨楠枝向寒](https://me.csdn.net/weixin_40247263)阅读数：165


一：配置数据库连接的数据源

首先在spring的配置文件中，需要配置数据源，如果单纯用hibernate的话，这个数据源是在hibernate.cfg.xml中配置

但是由于spring比hibernate对jdbc封装的要好，效率更高，所以我们通常用spring封装的jdbc。

![](https://img-blog.csdn.net/20180527162427705?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3dlaXhpbl80MDI0NzI2Mw==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)

下面这种是使用dbcp的数据库连接池。

![](https://img-blog.csdn.net/20180527162214792?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3dlaXhpbl80MDI0NzI2Mw==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)

二：给sessionFactory 中的一些属性赋初始值

我们都知道，hibernate对数据库的操作，都是通过session对象来实现的。

由于Spring 对hibernate的SessionFactory进行了封装，LocalSessionFactoryBean这个类里有许多对hibernate的SessionFactory这个类对象的一些操作。包括创建session工厂，创建session工厂就需要告诉工厂 dataSource是什么，他才能去创建session。

下面就是给 LocalSessionFactoryBean的 dataSource赋值。 <list>标签中引入的pojo的映射文件，这些东西以前是写在hibernate.cfg.xml 中的，所以用了spring以后，hibernate.cfg.xml这个文件就可以不用了。它里面的一些配置信息，全部写在了spring的配置文件当中。 包括下面的<prop>标签中的 配置 数据库方言（就是你用的是什么数据）以及是否显示sql语句。

![](https://img-blog.csdn.net/20180527162906421?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3dlaXhpbl80MDI0NzI2Mw==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)

三：给HibernateDaoSupport 中 创建 hibernateTemplate对象所需要的 参数 sessionFactory赋值

如果我们使用spring框架的话，以前对session对象的创建、关闭，都不需要在手动的写出来了，首先我们需要让我们的dao层实现类继承HibernateDaoSupport，然后通过getHibernateTemplate()方法获得一个hibernateTemplate对象，然后调用这个对象的方法就可以实现增删改查了。 一行代码而已。但是创建hibernateTemplate对象是需要sessionFactory这个对象。 所以我们需要在配置文件中，赋值。

先看一眼源码

![](https://img-blog.csdn.net/20180527165338443?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3dlaXhpbl80MDI0NzI2Mw==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)

赋值

![](https://img-blog.csdn.net/20180527165616665?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3dlaXhpbl80MDI0NzI2Mw==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)

这个 sessionFactory就是 之前配置的。


其实无论是 用mybatis 还是 hibernate 。与 spring整合的时候，核心操作无非就是 配置 一个 数据源，然后配置一下session工厂，把 映射文件 引入到 application.xml中。剩下的就都是一些 框架本身的一些细枝末节的配置。 比如mybatis自动生成dao层实现类，所以我们需要配置org.mybatis.spring.mapper.MapperScannerConfigurer 对dao层 进行扫包。hibernate 的方言，是否打印sql语句之类的。

