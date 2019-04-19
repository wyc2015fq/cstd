# Signing for "xxx" requires a development team. Select a development team - xqhrs232的专栏 - CSDN博客
2018年12月12日 23:03:36[xqhrs232](https://me.csdn.net/xqhrs232)阅读数：178
原文地址::[https://blog.csdn.net/wujakf/article/details/70171240](https://blog.csdn.net/wujakf/article/details/70171240)
相关文章
1、【iOS】Signing for requires a development team. Select a development team in the project editor. Code----[https://blog.csdn.net/liuchuo/article/details/52554997](https://blog.csdn.net/liuchuo/article/details/52554997)
2、【iOS】Signing for requires a development team. Select a development team in the project editor. Code----[https://www.aliyun.com/jiaocheng/385243.html](https://www.aliyun.com/jiaocheng/385243.html)
出现这个问题的原因是你的team没有设置对。
解决办法，TARGETS---->Genernal----->Signing------>team,选择合适的选项，编译即可
或者更改一下Bundle Identifier 。
//===============================================================================
本人备注::
1>xcode加了自己的apple id，在创建工程的时候,team选择自己的apple id就可以了！！！
