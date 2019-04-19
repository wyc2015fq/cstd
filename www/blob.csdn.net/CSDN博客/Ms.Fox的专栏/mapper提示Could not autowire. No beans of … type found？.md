# mapper提示Could not autowire. No beans of … type found？ - Ms.Fox的专栏 - CSDN博客
2019年01月18日 10:50:22[lulu-lu](https://me.csdn.net/smbluesky)阅读数：59
工具及背景： IntelliJ IDEA 2016.1.3 Ultimate。spring boot, maven项目，利用mybatis 注解的方式查询mysql
在自动生成工具生成代码后，service层调用Dao层出现了这样一个错误；检查mapper文件和配置文件等都没问题；
如何解决这一问题呢？  加@Repository
