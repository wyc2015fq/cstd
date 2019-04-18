# 【MyBatis学习01】宏观上把握MyBatis框架 - z69183787的专栏 - CSDN博客
2017年07月18日 10:38:16[OkidoGreen](https://me.csdn.net/z69183787)阅读数：174
个人分类：[Mybatis-学习](https://blog.csdn.net/z69183787/article/category/7026142)
今天开始学习mybatis框架，博客主要记录学习过程中的一些总结，如有错误之处，欢迎留言指正~先用mybatis的鸟鸟来镇个楼，咳咳~~ 
![MyBatis](https://img-blog.csdn.net/20160604094247046)
mybatis框架是一个持久层框架，是Apache下的顶级项目。mybatis可以让开发者的主要精力放在sql上，通过mybatis提供的映射方式，自由灵活的生成满足需要的sql语句。MyBatis可以将向preparedStatement中的输入参数自动进行输入映射，将查询结果集灵活映射成java对象（输出映射）。 
下面在整体上把握一下mybatis框架，总结一下mybatis的执行流程。看一下这个流程图就清楚了（画了我20分钟）： 
![mybatis](https://img-blog.csdn.net/20160604102352766)
从图中可以看出，mybatis中首先要在配置文件中配置一些东西，然后根据这些配置去创建一个会话工厂，再根据会话工厂创建会话，会话发出操作数据库的sql语句，然后通过执行器操作数据，再使用mappedStatement对数据进行封装。这就是整个mybatis框架的执行情况。对mybatis有了这个宏观上的了解，下面就开始一步步深入学习了。 
