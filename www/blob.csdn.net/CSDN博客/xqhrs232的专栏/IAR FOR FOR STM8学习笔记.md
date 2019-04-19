# IAR FOR FOR STM8学习笔记 - xqhrs232的专栏 - CSDN博客
2015年01月30日 14:30:52[xqhrs232](https://me.csdn.net/xqhrs232)阅读数：1529
原文地址::[http://wenku.baidu.com/link?url=h6AGIPDskWEO8FAc1DHcG6HU903pPtoLYgtqZNiKRx4TRWCofxssCQRy0NGkkdxzZlViATk6-4XdKCMZmsJNqtRZ6LANCfSUIWYEs2dOsCG](http://wenku.baidu.com/link?url=h6AGIPDskWEO8FAc1DHcG6HU903pPtoLYgtqZNiKRx4TRWCofxssCQRy0NGkkdxzZlViATk6-4XdKCMZmsJNqtRZ6LANCfSUIWYEs2dOsCG)
相关文章
1、STM8系列C语言入门指导----[http://wenku.baidu.com/view/340e7addce2f0066f5332214.html](http://wenku.baidu.com/view/340e7addce2f0066f5332214.html)
2、STM8 开发入门教程----[http://wenku.baidu.com/view/e0919dc69ec3d5bbfd0a7491.html](http://wenku.baidu.com/view/e0919dc69ec3d5bbfd0a7491.html)
3、STM8的C语言编程----[http://wenku.baidu.com/view/340e7addce2f0066f5332214.html](http://wenku.baidu.com/view/340e7addce2f0066f5332214.html)
4、STM8中文芯片规格书----[http://wenku.baidu.com/view/ce64a81e6bd97f192279e976.html](http://wenku.baidu.com/view/ce64a81e6bd97f192279e976.html)
5、STM8单片机入门 V2.6----[http://wenku.baidu.com/view/776476b91a37f111f1855b0d.html](http://wenku.baidu.com/view/776476b91a37f111f1855b0d.html)
6、STM8在IAR上工程的建立----[http://wenku.baidu.com/view/8cf22d3667ec102de2bd89b4.html](http://wenku.baidu.com/view/8cf22d3667ec102de2bd89b4.html)
7、IAR EWSTM8工程创建----[http://wenku.baidu.com/view/f33ffc28e2bd960590c677dc.html](http://wenku.baidu.com/view/f33ffc28e2bd960590c677dc.html)
8、IAR_for_STM8详细步骤----[http://wenku.baidu.com/view/2fa3c61afad6195f312ba659.html](http://wenku.baidu.com/view/2fa3c61afad6195f312ba659.html)
9、如何开始STM8S系列单片机的研发----[http://wenku.baidu.com/link?url=I38S6xeG_Yd1hb__kA6Nu55Upi_cpakdco1sZaxGp3HDqZf8d97EbMIFRUrowYfunKQikSwMeAzvItI_wMeOawRNfiSFmwoWc8CupHI98Fy](http://wenku.baidu.com/link?url=I38S6xeG_Yd1hb__kA6Nu55Upi_cpakdco1sZaxGp3HDqZf8d97EbMIFRUrowYfunKQikSwMeAzvItI_wMeOawRNfiSFmwoWc8CupHI98Fy)
10、STVD和STVP软件工具包----[http://wenku.baidu.com/link?url=TBW33L7k7gWj4WlH0sgVRtGyIpty-qe39Etc99TI2_Y6PkinDyRgKX8dLfzskpYggehZ_uVXzriNh0r1IOu3s7ibNWRhxxXI7rqjz8oJeZi](http://wenku.baidu.com/link?url=TBW33L7k7gWj4WlH0sgVRtGyIpty-qe39Etc99TI2_Y6PkinDyRgKX8dLfzskpYggehZ_uVXzriNh0r1IOu3s7ibNWRhxxXI7rqjz8oJeZi)
11、ST Visual Develop (STVD) 4.2.1----[http://www.duxin.com.cn/tools/soft/st-linktools.html](http://www.duxin.com.cn/tools/soft/st-linktools.html)
12、[http://www.stmcu.org/document/list/index/category-17](http://www.stmcu.org/document/list/index/category-17)
13、[stm32f407之创建IAR工程](http://blog.csdn.net/w471176877/article/details/7957152)----[http://blog.csdn.net/w471176877/article/details/7957152](http://blog.csdn.net/w471176877/article/details/7957152)

//====================================================================================================
备注::
1>Keil2/keil3/keil4好像都不支持STM8S单片机的开发！！！
2>STM8单片机的编程可以使用IAR或者STVD IDE
3>STM8单片机不是基于51核的！！！
4>针对STM32开发要去找IAR FOR ARM 开发环境
