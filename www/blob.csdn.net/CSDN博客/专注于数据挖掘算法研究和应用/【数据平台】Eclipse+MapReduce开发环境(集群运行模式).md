# 【数据平台】Eclipse+MapReduce开发环境(集群运行模式) - 专注于数据挖掘算法研究和应用 - CSDN博客





2017年09月11日 17:50:58[fjssharpsword](https://me.csdn.net/fjssharpsword)阅读数：238
个人分类：[Big data](https://blog.csdn.net/fjssharpsword/article/category/6309934)









### Eclipse+MapReduce开发环境

集群部署centos+jdk1.8+Hadoop2.7.2；

开发端windows+Eclipse Mars.1 Release (4.5.1)

1）hadoop-eclipse-plugin插件

需要编译，可用网上现成，这里找到hadoop-eclipse-plugin-2.7.2.jar下载（要注意编译plugin的jdk版本，后面eclipse环境和centos下jdk环境也要一致），放在eclipse的plugins文件夹。

下载hadoop-2.7.4.tar.gz解压到D:/dev/hadoop-2.7.4目录，配置到：

![](https://img-blog.csdn.net/20170911175049105?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvZmpzc2hhcnBzd29yZA==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)




2）插件配置

打开Map/Reduce的Perspective：

![](https://img-blog.csdn.net/20170911174830249?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvZmpzc2hhcnBzd29yZA==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)




点击小象，设置集群信息：

![](https://img-blog.csdn.net/20170911174849692?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvZmpzc2hhcnBzd29yZA==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)






![](https://img-blog.csdn.net/20170911174909715?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvZmpzc2hhcnBzd29yZA==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)


连接成功：

![](https://img-blog.csdn.net/20170911174920012?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvZmpzc2hhcnBzd29yZA==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)




3）WordCount示例

代码参考官网，运行时设置：

![](https://img-blog.csdn.net/20170911174933173?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvZmpzc2hhcnBzd29yZA==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)







 备注：提示winutils.exe问题，去下载一个放到hadoop路径bin下就可以。



