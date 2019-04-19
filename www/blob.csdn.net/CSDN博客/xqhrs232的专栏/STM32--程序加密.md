# STM32--程序加密 - xqhrs232的专栏 - CSDN博客
2018年04月28日 09:26:17[xqhrs232](https://me.csdn.net/xqhrs232)阅读数：737
原文地址::[https://blog.csdn.net/zhangxufei/article/details/52353527](https://blog.csdn.net/zhangxufei/article/details/52353527)
相关文章
1、关于STm32加密【转】----[https://blog.csdn.net/sinat_36568888/article/details/52984056](https://blog.csdn.net/sinat_36568888/article/details/52984056)
2、STM32芯片烧写和加密解密----[https://wenku.baidu.com/view/5de150dc3186bceb19e8bb2a.html](https://wenku.baidu.com/view/5de150dc3186bceb19e8bb2a.html)
3、STM32 的唯一 ID 可以修改! 解密的发现了这个!(ID加密失效)----[http://bbs.ic37.com/bbsview-29680.htm](http://bbs.ic37.com/bbsview-29680.htm)
4、STM32加密之我见----[http://www.360doc.com/content/14/1218/22/6828497_434009249.shtml](http://www.360doc.com/content/14/1218/22/6828497_434009249.shtml)
所有STM32的芯片都提供对Flash的保护，防止对Flash的非法访问 - 写保护和读保护。读保护即大家通常说的“加密”，是作用于整个Flash存储区域。一旦设置了Flash的读保护，内置的Flash存储区只能通过程序的正常执行才能读出，而不能通过这两种方式中的任何一种方式读出：通过调试器(JTAG或SWD)和从RAM中启动并执行的程序。我们这里程序加密的目的是防止板子中的程序被其他人读出使用，这里有两种方法可供使用：
       1.禁止读出Flash存储器内容；
       2.STM32用ID加密程序；
这里我们分别对这两种方法进行介绍。
        一.禁止读出Flash存储器内容
一旦设置了Flash的读保护，内置的Flash存储区只能通过程序的正常执行才能读出，而不能通过这两种方式中的任何一种方式读出：通过调试器(JTAG或SWD)和从RAM中启动并执行的程序。这是也无法对flash进行烧写程序了，所以使用时需要谨慎。下面简单介绍下使用方法。
       1.在程序中包含#include "stm32f10x_flash.h"头文件 并在这条语句前面加入 这一行话：#define _FLASH_PROG;   //flash读写保护时需要加入的
       2.实现flash的读保护：
           /* 设置Flash读保护 */ 
           if (FLASH_GetReadOutProtectionStatus()!=SET) 
           { 
                /* 会擦除Flash */
                FLASH_Unlock(); //写保护时可以不用这句话，可用可不用
                FLASH_ReadOutProtection(ENABLE); 
            }
清除flash的读保护：
       主要就是以上三步，具体如何在程序中使用，看自己的需求。
STM32用ID加密程序
下面简单介绍下使用方法。
        1.读取目标板子的ID号，并修改程序将ID号保存在程序中：
             
 U8 sys_id[12]={0};  //存储stm32芯片的ID号的数组
               for(i=0;i<12;i++)
               {
                  sys_id[i]=*(u8*)(0x1FFFF7E8+i);  //读取STM32芯片的唯一ID
              }
        2.将修改后的程序再下载到目标板子上这时就可以使用上面读出ID来对比当前板子上ID是否一致来确定程序是否执行。
       这个方法就算程序被其他人读取也无法在其他板子上运行，因为ID不一致。麻烦的地方是每块板子的程序的ID不一样造成程序维护麻烦。
       实际怎么使用还是要看当时情况，也可能用不上这两种方法。
参考文章：
        http://www.cnblogs.com/hujianhua/archive/2012/07/03/2574917.html
        http://bbs.elecfans.com/jishu_388272_1_1.html

