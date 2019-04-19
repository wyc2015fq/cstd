# eclipse 重新加载项目遇到的问题----The entity has no primary key attribute defined - xqhrs232的专栏 - CSDN博客
2018年11月08日 16:36:22[xqhrs232](https://me.csdn.net/xqhrs232)阅读数：175
原文地址::[https://blog.csdn.net/baidu_26550817/article/details/51752085](https://blog.csdn.net/baidu_26550817/article/details/51752085)
相关文章
1、[JPA Error : The entity has no primary key attribute defined](https://stackoverflow.com/questions/13190997/jpa-error-the-entity-has-no-primary-key-attribute-defined)----[https://stackoverflow.com/questions/13190997/jpa-error-the-entity-has-no-primary-key-attribute-defined](https://stackoverflow.com/questions/13190997/jpa-error-the-entity-has-no-primary-key-attribute-defined)
The entity has no primary key attribute defined 
实体没有定义主键属性
初步解决办法： 
Windows->Persistence->Java Persistence->JPA->Errors/Warnings->Type,将选项Entity has no primary key设为警告或更低的提示级别。（错误提示变成了警告提示，治标不治本）
Referenced file contains errors (,,,,,,,,,,,,,,,,,,,). For more information, right click on the message in the Problems View and select “Show Details…”
将 Preferences > XML > XML Files > Validation中”Honour all XML schema locations”前的对号去掉。禁用指向不同schema位置相同命名空间引用的验证，仅以第一次找到的可验证的XML文件为结果。
--------------------- 
作者：tsf_1993 
来源：CSDN 
原文：https://blog.csdn.net/baidu_26550817/article/details/51752085 
版权声明：本文为博主原创文章，转载请附上博文链接！
