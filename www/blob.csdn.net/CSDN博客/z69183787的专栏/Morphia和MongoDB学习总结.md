# Morphia和MongoDB学习总结 - z69183787的专栏 - CSDN博客
2016年10月26日 17:22:32[OkidoGreen](https://me.csdn.net/z69183787)阅读数：718
**Morphia** 是一个轻量级的类型安全的 Java 类库，用来将在MongoDB 和 Java 对象之间进行映射。
首先，做准备工作，使用morphia操作MongoDB数据需要的jar有：
1.morphia-0.99.jar；2.mongo-2.7.2.jar
其次，搭建工程，如图所示：
![](http://dl.iteye.com/upload/attachment/0065/7374/db5f6a05-b033-3e3b-8774-ece3a557b39e.png)
1.创建一个实体类MyEntity.jva，代码如下：
![](http://dl.iteye.com/upload/attachment/0065/7382/924fec5e-b7a4-3f72-8cc1-5e4142bdd3cb.png)
 2.再创建一个测试类Test.java，代码如下：
![](http://dl.iteye.com/upload/attachment/0065/7385/2b24f200-943e-3c48-a785-53e7c51e7308.png)
![](http://dl.iteye.com/upload/attachment/0065/7387/fa4804b5-b71b-30c0-88f5-d72a53b2a281.png)
**相关资料链接：**
1.morphia官方文档：[http://code.google.com/p/morphia/w/list](http://code.google.com/p/morphia/w/list)
2.mongodb的官方文档：[https://github.com/mongodb](https://github.com/mongodb)
学习之余，整理此文档，希望对初学者有所帮助，若有问题请留言！
