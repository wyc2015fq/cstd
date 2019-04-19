# 都是Hibernate的hbm文件惹的祸 - aisoo的专栏 - CSDN博客
2010年09月17日 14:46:00[aisoo](https://me.csdn.net/aisoo)阅读数：341标签：[hibernate																[数据库																[服务器](https://so.csdn.net/so/search/s.do?q=服务器&t=blog)](https://so.csdn.net/so/search/s.do?q=数据库&t=blog)](https://so.csdn.net/so/search/s.do?q=hibernate&t=blog)
项目发布需要分布式部署，机构A,机构B，中心，服务器不够用，一个用户下创建了3个数据库，应用程序部署后，发现访问机构A,机构B，中心时的数据都是来自一个数据源，怎么寻思也不可能，刚想要换个策略，建3个数据库在3个服务器上，后来发现了hbm文件中的每个对象有个属性catalog指定了对应的数据库，这才恍然大悟。将那个属性去掉后，搞定！
