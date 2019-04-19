# 为什么一个javaweb项目里有多个main函数? - xqhrs232的专栏 - CSDN博客
2018年11月12日 11:23:47[xqhrs232](https://me.csdn.net/xqhrs232)阅读数：507
原文地址::[https://zhidao.baidu.com/question/1704581714638407700.html](https://zhidao.baidu.com/question/1704581714638407700.html)
相关文章
1、一个java文件为什么可以有两个main函数----[https://zhidao.baidu.com/question/2120745906443829787.html](https://zhidao.baidu.com/question/2120745906443829787.html)
2、web项目没有main方法，怎么运行的？----[https://blog.csdn.net/lyt_7cs1dn9/article/details/52469472](https://blog.csdn.net/lyt_7cs1dn9/article/details/52469472)
类中的[main函数](https://www.baidu.com/s?wd=main%E5%87%BD%E6%95%B0&tn=SE_PcZhidaonwhc_ngpagmjz&rsv_dl=gh_pc_zhidao)，在[java](https://www.baidu.com/s?wd=java&tn=SE_PcZhidaonwhc_ngpagmjz&rsv_dl=gh_pc_zhidao) web应用中，没有任何的作用，web应用启动是不会执行类中的main方法的，因为只会调用tomcat或者其它应用服务器的main，然后通过web.xml的配置去加载其它类，公司的[java](https://www.baidu.com/s?wd=java&tn=SE_PcZhidaonwhc_ngpagmjz&rsv_dl=gh_pc_zhidao)[web项目](https://www.baidu.com/s?wd=web%E9%A1%B9%E7%9B%AE&tn=SE_PcZhidaonwhc_ngpagmjz&rsv_dl=gh_pc_zhidao)的main方法，纯属测试用的。
