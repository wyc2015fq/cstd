# Boost-QT兼容问题：#define FUSION_HASH # - wishchinYang的专栏 - CSDN博客
2017年10月29日 15:11:36[wishchin](https://me.csdn.net/wishchin)阅读数：2960
 使用原始的MSVC10+QT48很长时间，需要把PCL升级到新的版本，不再使用自行编译的PCL1.7.2版本。
 在使用MSVC2012的时候，使用MSVC12-的PCL1.8.0版本，出现了一个不大不小的问题。
 在使用QT编译项目，包含Boost库头文件的时候，产生了编译错误。
 #define FUSION_HASH #
 尝试了各种方法，都没有解决问题。
 没有办法，不再使用VS2013，使用Vs2015，使用QT5.8.0VC14版本，使用新的PCL1.8.1版本的库文件，问题得到解决![微笑](http://static.blog.csdn.net/xheditor/xheditor_emot/default/smile.gif)
