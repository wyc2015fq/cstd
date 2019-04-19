# Target runtime com.genuitec.runtime.generic.j2ee14 is not defined - xqhrs232的专栏 - CSDN博客
2018年09月30日 10:14:53[xqhrs232](https://me.csdn.net/xqhrs232)阅读数：231
原文地址::[https://blog.csdn.net/snails_zx/article/details/51958767](https://blog.csdn.net/snails_zx/article/details/51958767)
相关文章
1、Target runtime Apache Tomcat v6.0 is not defined.错误解决方法----[https://blog.csdn.net/xw13106209/article/details/5910358](https://blog.csdn.net/xw13106209/article/details/5910358)
eclipse编译项目报错：
Description Resource Path Location Type Target runtime com.genuitec.runtime.generic.j2ee14 is not defined. XXX Unknown Faceted Project Problem
解决方法
工程目录下的settings，打开org.eclipse.wst.common.project.facet.core.xml文件，把<runtime name="com.genuitec.runtime.generic.jee60" />，把这句话去掉
--------------------- 本文来自 snails_zx 的CSDN 博客 ，全文地址请点击：https://blog.csdn.net/snails_zx/article/details/51958767?utm_source=copy
