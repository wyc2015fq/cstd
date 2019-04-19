# The import javax.servlet cannot be resolved - xqhrs232的专栏 - CSDN博客
2018年11月08日 16:11:53[xqhrs232](https://me.csdn.net/xqhrs232)阅读数：61
个人分类：[JavaWeb技术](https://blog.csdn.net/xqhrs232/article/category/8231541)
原文地址::[https://blog.csdn.net/niu_hao/article/details/8306386](https://blog.csdn.net/niu_hao/article/details/8306386)
今天将别人的项目导入eclipse之后，出现了“The import javax.servlet cannot be resolved”错误，出现这个错误的原因是：没有servlet-api.jar这个包，将这个包导入项目里面就可以了。那么这个包一般到那里去找呢？我们可以到tomcat的lib目录下面找到这个包，然后在eclipse中，右击项目，选择Java Build Path>Libraries>Add External JARS,找到你计算机中的tomcat的安装路径，在common\lib文件夹下，选中"servlet-api.jar",添加点击“确定”,就行了。
--------------------- 
作者：niu_hao 
来源：CSDN 
原文：https://blog.csdn.net/niu_hao/article/details/8306386 
版权声明：本文为博主原创文章，转载请附上博文链接！
