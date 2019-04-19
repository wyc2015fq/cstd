# win7 通过war包在浏览器上运行一个web 应用 - xqhrs232的专栏 - CSDN博客
2018年09月29日 14:33:15[xqhrs232](https://me.csdn.net/xqhrs232)阅读数：95
个人分类：[JavaWeb技术](https://blog.csdn.net/xqhrs232/article/category/8231541)
原文地址::[https://blog.csdn.net/yuzhiyun3536/article/details/52705054](https://blog.csdn.net/yuzhiyun3536/article/details/52705054)
相关文章
1、用tomcat运行war包----[https://blog.csdn.net/YJTMGH/article/details/78252906](https://blog.csdn.net/YJTMGH/article/details/78252906)
2、将项目打成war包并用tomcat部署的方法，步骤及注意点----[https://blog.csdn.net/zhanglf02/article/details/73565354](https://blog.csdn.net/zhanglf02/article/details/73565354)
一个war包对应着一个java web应用，得到一个war包之后如何运行这个应用呢？首先把war包copy到tomcat安装目录下的webapps下，比如：
E:\tomcat\apache-tomcat-8.0.32\webapps
然后双击E:\tomcat\apache-tomcat-8.0.32\bin下的startup.bat来启动tomcat,在浏览器端输入[http://localhost:8080/JPetStore/](http://localhost:8080/JPetStore/)即可访问到那个web 应用，（其中JPetStore是那个war包的名称）
--------------------- 本文来自 俞志云 的CSDN 博客 ，全文地址请点击：https://blog.csdn.net/yuzhiyun3536/article/details/52705054?utm_source=copy
