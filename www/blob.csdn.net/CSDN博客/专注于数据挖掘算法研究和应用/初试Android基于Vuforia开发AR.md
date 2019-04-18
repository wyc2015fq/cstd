# 初试Android基于Vuforia开发AR - 专注于数据挖掘算法研究和应用 - CSDN博客





2016年03月04日 15:57:22[fjssharpsword](https://me.csdn.net/fjssharpsword)阅读数：3794








1、Vuforia官网https://developer.vuforia.com/  

       下载vuforia-sdk-android库和vuforia samples




2、Android开发环境：eclipse+adt+android4.2.2，新建android工程

      1）工程libs下加上\vuforia-sdk-android-5-0-10\build\lib\armeabi-v7a\libVuforia.so

      2）工程库引入\vuforia-sdk-android-5-0-10\build\java\vuforia\Vuforia.jar库




3、导入samples代码：VuforiaSamples-5-0-10是gradle打包，因不想在eclipse上多安装gradle插件，所以直接将sample下的源码复制到新工程，具体编译通过的工程包见http://download.csdn.net/detail/fjssharpsword/9452917

     CSDN上也有很多类似的资源。




后续如要深入开发，需要研究其vuforia基础源码。





