# 使用Eclipse从数据库逆向生成实体类 - xqhrs232的专栏 - CSDN博客
2018年10月31日 11:26:37[xqhrs232](https://me.csdn.net/xqhrs232)阅读数：76
原文地址::[https://blog.csdn.net/qq_20797031/article/details/51887947](https://blog.csdn.net/qq_20797031/article/details/51887947)
相关文章
1、java 从数据库表反射出实体类，自动生成实体类----[https://blog.csdn.net/ruiguang21/article/details/78248079](https://blog.csdn.net/ruiguang21/article/details/78248079)
2、[使用Sql语句快速将数据表转换成实体类](https://www.cnblogs.com/bayes/p/6206297.html)----[https://www.cnblogs.com/bayes/p/6206297.html](https://www.cnblogs.com/bayes/p/6206297.html)
3、eclipse自动生成实体类----[https://blog.csdn.net/qq_21911655/article/details/78955269](https://blog.csdn.net/qq_21911655/article/details/78955269)
4、eclipse逆向生成实体类----[https://blog.csdn.net/hsg_happyLearning/article/details/80281081](https://blog.csdn.net/hsg_happyLearning/article/details/80281081)
5、eclipse 使用JPA Tools 根据数据库表自动生成实体类----[https://blog.csdn.net/guoxin91/article/details/31801919](https://blog.csdn.net/guoxin91/article/details/31801919)
我们都知道，一个项目的实体类少则十几个，多则几十个。一个类里面又有很多属性。对于我来说，我更希望怎么把代码写好。而不是把时间浪费在这些毫无意义的事情上。
在MyEclipse中，正逆向工程操作比较简单。在eclipse中的操作程序跟在my里面不一样。我也看了一下 使用hibernate生成实体类 个人感觉有点复杂。 接下来看教程
   1. 我们先建立一个data source（我以mysql为例）
database 设置成你需要反向生成的 数据库 设置好之后就会看到
table 就是相当你需要方向生产的对象了 。
2. 然后在eclipse建立一个JPA项目。至于project name 什么的乱写就好了。 反正只是用来用一个映射实体的临时项目而已
然后 鼠标点击项目 右键 看到JPA tools 然后看到 generate entities form tables ； 这个就是 根据数据库表创建实体；
 这个好像没办法截图。 我就不截图了。
点进去可以看到
connection 就是刚刚让你们创建的data source 。下拉框选择它，然后就可以看到你需要映射的实体的表了。旁边有全选的按钮 。 选完之后点击next
这个界面就是映射关系的里面。 可以添加 查看。选择需不需要映射这些实体之间的关系 然后点击next
key generator 就是 id的生成策略。 这个看个人需要。package 就是决定把生成的model 放在哪个文件夹下面。 如果要继承 或者 是实现什么接口的话可以在哪里选择
然后点击 finish 就可以了
这个就是生成之后的文件。
--------------------- 
作者：男神啦啦啦 
来源：CSDN 
原文：https://blog.csdn.net/qq_20797031/article/details/51887947 
版权声明：本文为博主原创文章，转载请附上博文链接！
