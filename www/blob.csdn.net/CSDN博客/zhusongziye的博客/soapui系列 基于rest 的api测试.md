# soapui系列|基于rest 的api测试 - zhusongziye的博客 - CSDN博客





2018年12月08日 13:35:15[zhusongziye](https://me.csdn.net/zhusongziye)阅读数：58








SoapUI是一个开源测试工具，通过soap/http来检查、调用、实现Web Service的功能/负载/符合性测试。该工具既可作为一个单独的测试软件使用，也可利用插件集成到Eclipse，maven2.X，Netbeans 和intellij中使用。



本文介绍基于rest的接口测试，从创建项目到编写case到断言，一步步教会你如何写一个接口测试用例。

**第一节：创建要测试****restapi**
- 
创建一个rest project

![](https://img-blog.csdnimg.cn/20181208133348165.png)

- 
创建一个rest 服务

![](https://img-blog.csdnimg.cn/20181208133354487.png)

- 
创建一级资源

![](https://img-blog.csdnimg.cn/20181208133403715.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3podXNvbmd6aXll,size_16,color_FFFFFF,t_70)

- 
创建二级资源

![](https://img-blog.csdnimg.cn/2018120813341229.png)

- 
创建多级资源:继续添加child resource即可

![](https://img-blog.csdnimg.cn/20181208133419442.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3podXNvbmd6aXll,size_16,color_FFFFFF,t_70)




**第二节：根据创建好的****rest****服务生成****testsuit,testcase****和****teststep**
- 
选中最低级子资源的request右键

![](https://img-blog.csdnimg.cn/2018120813342790.png)


   2.选择add to TestCase

![](https://img-blog.csdnimg.cn/20181208133435870.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3podXNvbmd6aXll,size_16,color_FFFFFF,t_70)

    3.创建新的testsuite

![](https://img-blog.csdnimg.cn/20181208133444132.png)

    4.创建新的testcase

![](https://img-blog.csdnimg.cn/20181208133449862.png)

    5.创建新的teststep

![](https://img-blog.csdnimg.cn/201812081334568.png)



**第三节：补充测试相关脚本，数据库查询，断言**

1.添加断言完成设置

![](https://img-blog.csdnimg.cn/20181208133506339.png)



