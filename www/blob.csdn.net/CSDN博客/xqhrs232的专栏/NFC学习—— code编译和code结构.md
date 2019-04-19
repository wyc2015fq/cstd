# NFC学习—— code编译和code结构 - xqhrs232的专栏 - CSDN博客
2014年09月03日 16:53:17[xqhrs232](https://me.csdn.net/xqhrs232)阅读数：737
个人分类：[Android/Java](https://blog.csdn.net/xqhrs232/article/category/906925)
原文地址::[http://blog.csdn.net/nxh_love/article/details/8721596](http://blog.csdn.net/nxh_love/article/details/8721596)
相关文章
1、[NFC学习——NfcService开机自启动](http://blog.csdn.net/nxh_love/article/details/8721018)----[http://blog.csdn.net/nxh_love/article/details/8721018](http://blog.csdn.net/nxh_love/article/details/8721018)
2、[NFC学习——NfcService 启动过程分析](http://blog.csdn.net/nxh_love/article/details/8725466)----[http://blog.csdn.net/nxh_love/article/details/8725466](http://blog.csdn.net/nxh_love/article/details/8725466)
3、[NFC学习——NFC Enable 过程分析(一)](http://blog.csdn.net/nxh_love/article/details/8771200)----[http://blog.csdn.net/nxh_love/article/details/8771200](http://blog.csdn.net/nxh_love/article/details/8771200)
4、[NFC学习——NFC Enable 过程分析(二)](http://blog.csdn.net/nxh_love/article/details/8776049)----[http://blog.csdn.net/nxh_love/article/details/8776049](http://blog.csdn.net/nxh_love/article/details/8776049)
5、[NFC学习——NFC Enable 过程分析(三)](http://blog.csdn.net/nxh_love/article/details/8776152)----[http://blog.csdn.net/nxh_love/article/details/8776152](http://blog.csdn.net/nxh_love/article/details/8776152)
6、[NFC 基础](http://blog.csdn.net/nxh_love/article/details/8808455)----[http://blog.csdn.net/nxh_love/article/details/8808455](http://blog.csdn.net/nxh_love/article/details/8808455)

  对于NFC代码编译，目前为止，我见过两家不同方案商的书写方式，都是在device下的XXXX.mk文件中书写，XXXX不同的方案商不同的产品名称不同。下面具体来看：
方式A：
**[html]**[view
 plain](http://blog.csdn.net/nxh_love/article/details/8721596#)[copy](http://blog.csdn.net/nxh_love/article/details/8721596#)
- PRODUCT_COPY_FILES += \  
- frameworks/native/data/etc/android.hardware.nfc.xml:system/etc/permissions/android.hardware.nfc.xml  
**[html]**[view
 plain](http://blog.csdn.net/nxh_love/article/details/8721596#)[copy](http://blog.csdn.net/nxh_love/article/details/8721596#)
- # NFC packages  
- PRODUCT_PACKAGES += \  
-                 libnfc \  
-                 libnfc_jni \  
-                 Nfc \  
-                 Tag  
方式B：
**[html]**[view
 plain](http://blog.csdn.net/nxh_love/article/details/8721596#)[copy](http://blog.csdn.net/nxh_love/article/details/8721596#)
- #NXP NFC   
- PRODUCT_COPY_FILES += \  
- frameworks/native/data/etc/android.hardware.nfc.xml:system/etc/permissions/android.hardware.nfc.xml  
**[html]**[view
 plain](http://blog.csdn.net/nxh_love/article/details/8721596#)[copy](http://blog.csdn.net/nxh_love/article/details/8721596#)
- PRODUCT_PACKAGES += \  
-         nfc.XXXX \  
-         libnfc \  
-         libnfc_jni \  
-         Nfc \  
-         Tag   
       对比这两种方式，可以很明显发现其中的不同点nfc.XXXX，下面就两者的不同之处在分析：
       nfc.XXXX 是nfc_hw.c 编译生成的nfc.XXXX.so包，在out/目录下。方式A中并没有把nfc.XXXX放进去，它放在modules.mk中编译。nfc_hw.c不同的方案也具体放在不同的位置。总之，它的目的是生成nfc.XXXX.so以供调用。
         除了上述之外的东西，还需要在init.rc中配置：
**[html]**[view
 plain](http://blog.csdn.net/nxh_love/article/details/8721596#)[copy](http://blog.csdn.net/nxh_love/article/details/8721596#)
- # NFC  
-     setprop ro.nfc.port "I2C"  
         至此，NFC编译部分都配置完毕了。
         通过以上的分析，NFC 在android 中的code 结构都差不多出来了，下面分析下：![](https://img-my.csdn.net/uploads/201303/26/1364286546_6728.jpg)
 客户端：android提供了两个API包给apk，分别是android.nfc.tech、android.nfc，实现了NFC的应用接口，代码路径frameworks/base/core/java/android/nfc/tech、frameworks/base/core/java/android/nfc。
服务端：packages/apps/Nfc是一个类似电话本的应用，这个程序在启动后自动运行，并一直运行，作为NFC的服务进程存在，是NFC的核心。
在这个程序代码中，有个JNI库，供NfcService调用，代码路径是packages/apps/Nfc/jni/.编译文件中，还有个Tag部分，代码路径是packages/apps/Tag.
库文件：代码路径是external/libnfc-nxp,用C编写的库文件，有两个库，分别是libnfc.so和libnfc_ndef.so。libnfc.so是一个主要的库，实现了NFC
 stack的大部分功能，主要提供NFC的服务进程调用。libnfc_ndef是一个很小的库，主要是实现NDEF消息的解析，供framework调用
    nfc_hw.c：这个文件的具体路径不确定。
    pn544.c：这个文件是具体nfc芯片的驱动，一般都是drivers下。
由于本人初学，能力有限，有错误的地方欢迎指出。
参考文章：
## [NFC framework introduce(一](http://blog.sina.com.cn/s/blog_89f592f501013ypd.html))
