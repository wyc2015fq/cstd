# H.264官方软件JM源代码简单分析-编码器lencod - 雷霄骅(leixiaohua1020)的专栏 - CSDN博客





2015年11月18日 14:56:04[雷霄骅](https://me.csdn.net/leixiaohua1020)阅读数：30985标签：[JM																[H.264																[视频编码																[源代码																[lencod](https://so.csdn.net/so/search/s.do?q=lencod&t=blog)
个人分类：[JM](https://blog.csdn.net/leixiaohua1020/article/category/5974709)







=====================================================

H.264/H.265 官方源代码分析文章：

[H.264官方软件JM源代码简单分析-编码器lencod](http://blog.csdn.net/leixiaohua1020/article/details/49822701)

[H.264官方软件JM源代码简单分析-解码器ldecod](http://blog.csdn.net/leixiaohua1020/article/details/49822613)

[HEVC官方软件HM源代码简单分析-编码器TAppEncoder](http://blog.csdn.net/leixiaohua1020/article/details/49912113)

[HEVC官方软件HM源代码简单分析-解码器TAppDecoder](http://blog.csdn.net/leixiaohua1020/article/details/49912013)

=====================================================


上一篇文章总结了H.264官方参考软件JM中的视频解码器ldecod的结构，本文继续分析JM中的视频编码器lencod。


## 函数调用关系图

JM中的H.264视频编码器lencod的函数调用关系图如下所示。

[![](https://img-blog.csdn.net/20151113202405502)](https://img-my.csdn.net/uploads/201511/16/1447638505_8114.png)


[单击查看更清晰的大图](https://my.csdn.net/leixiaohua1020/album/detail/1816477)


下面解释一下图中关键标记的含义。


> **函数背景色**
函数在图中以方框的形式表现出来。不同的背景色标志了该函数不同的作用：
白色背景的函数：不加区分的普通内部函数。
黄色背景函数：滤波函数（Filter）。用于环路滤波，半像素插值，SSIM/PSNR的计算。
绿色背景的函数：宏块编码函数（Encode）。通过对残差的DCT变换、量化等方式对宏块进行编码。
紫色背景的函数：熵编码函数（Entropy Coding）。对宏块编码后的数据进行CABAC或者CAVLC熵编码。

浅蓝色背景函数：码率控制函数（Rate Control）。对码率进行控制的函数。



**箭头线**
箭头线标志了函数的调用关系：
黑色箭头线：不加区别的调用关系。
黄色的箭头线：滤波函数（Filter）之间的调用关系。
绿色箭头线：宏块编码函数（Encode）之间的调用关系。
紫色箭头线：熵编码函数（Entropy Coding）之间的调用关系。

**函数所在的文件**
每个函数标识了它所在的文件路径。


下文记录结构图中几个关键的部分。





## 普通内部函数
普通内部函数指的是lencod中还没有进行分类的函数。例如：

> 编码器的main()函数中调用的参数配置函数Configure()、初始化编码器函数init_encoder()、编码函数encode_sequence()、释放编码器函数free_encoder()等。
编码器主要编码函数encode_one_frame()逐层调用的函数perform_encode_frame()、frame_picture()、code_a_picture()、code_a_plane()等。


## 宏块编码函数
宏块编码函数通过运动估计、DCT变换、量化等步骤对图像数据进行编码。编码的工作都是在Slice结构体中的encode_one_macroblock()中完成的。encode_one_macroblock()调用PartitionMotionSearch()完成帧间宏块运动估计的工作；调用mode_decision_for_I8x8_MB()、mode_decision_for_I4x4_MB()完成帧内宏块的模式选择；调用Slice结构体的luma_residual_coding()完成了残差编码的工作。
PartitionMotionSearch()调用了BlockMotionSearch()。BlockMotionSearch()调用了Macroblock结构体中的IntPelME()完成整像素的运动估计，又调用了SubPelME()完成了亚像素的运动估计。整像素的运动估计可以使用下面的方法：

> EPZS_motion_estimation()：EPZS算法；
full_search_motion_estimation()：全搜索算法；
UMHEXIntegerPelBlockMotionSearch()：UMHEX算法；
亚像素的运动估计可以使用下面的方法：

> EPZS_sub_pel_motion_estimation()：EPZS算法；
full_sub_pel_motion_estimation()：全搜索算法；
UMHEXSubPelBlockME()：UMHEX算法；


## 熵编码函数
熵编码函数使用CAVLC或者CABAC的方式对宏块编码后的数据进行熵编码。熵编码的工作都是在write_macroblock()中完成的。write_macroblock()调用了Slice结构体中的write_MB_layer()方法，根据Slice类型的不同，write_MB_layer()可以指向下面方法：
write_i_slice_MB_layer()：I Slice宏块熵编码；
write_p_slice_MB_layer()：P Slice宏块熵编码；
write_b_slice_MB_layer()：B Slice宏块熵编码；


## 环路滤波函数
环路滤波函数对重建帧数据进行滤波，去除方块效应。去块效应滤波是在DeblockPicture()中完成的。DeblockPicture()调用了DeblockMb()。而DeblockMb()中调用GetStrengthVer()、GetStrengthHor()函数获取滤波强度；调用EdgeLoopLumaVer()、EdgeLoopLumaHor()进行滤波。


## 码率控制函数
码率控制模块函数分布在lencod源代码不同的地方，包括rc_init_seq()、rc_init_GOP()、rc_init_frame()、rc_handle_mb()等。





**雷霄骅leixiaohua1020@126.comhttp://blog.csdn.net/leixiaohua1020**](https://so.csdn.net/so/search/s.do?q=源代码&t=blog)](https://so.csdn.net/so/search/s.do?q=视频编码&t=blog)](https://so.csdn.net/so/search/s.do?q=H.264&t=blog)](https://so.csdn.net/so/search/s.do?q=JM&t=blog)




