# 详解 NXP Cortex-M3 加密设置 - xqhrs232的专栏 - CSDN博客
2017年06月04日 22:52:58[xqhrs232](https://me.csdn.net/xqhrs232)阅读数：497
个人分类：[ARM/STM32/LPC17/单片机技术](https://blog.csdn.net/xqhrs232/article/category/939440)
原文地址::[http://blog.csdn.net/chengfeng135/article/details/49638495](http://blog.csdn.net/chengfeng135/article/details/49638495)
相关文章
1、Code Read Protection (CRP)----[https://community.nxp.com/message/630586](https://community.nxp.com/message/630586)

详解**NXPCortex-M3 **加密设置
——[嵌入式](http://lib.csdn.net/base/embeddeddevelopment)软硬件同盟协会
 相信最近手上有NXPCortex-M3的 LPC13XXLPC17XX类的器件或者开发板的不少
了，但是新手对NXP 的加密方式往往比较费解，和别的如51、AVR的加密方式不太一样了，
 通常51、AVR的加密往往通过编程器，在写入FLASH 的过程中对其IC的配置字或加密位
 进行修改，但是，NXPCortex-M3的加密在下载软件中却没有选项或说明，那么到底如何对我们的器件进行加密呢？这将是我们的话题。
其实通过NXP的“AN10851”的阅读，我们即可以了解应该如何设置了，http://tiyubisai.com/video_news/news_135631.html 但如何在我
 们自己的工程项目中进行设置呢，往往我们拿到的工程项目中没有该设置项，如图：
如此，打开startup_LPC13xx.s 或者与CPU 所对应的的汇编文件，找到有这么一段：
||IF|:LNOT::DEF:NO_CRP|
|----|----|----|
|AREA||.ARM.__at_0x02FC|, CODE, READONLY| |
|CRP_Key|DCD|0xFFFFFFFF|
||ENDIF||
我们可以看到，这有几个关键的地方“NO_CRP”、0x02FC和0xFFFFFFFF，如果
我们在前面定义有“NO_CRP”，那么我们后面的代码也就不起作用了，所以在需要加密的时候前面就一定不能再定义了，除了文件里的定义我们还要注意这里：
在ASM的汇编定义项里，我们需要删除此定义，0x02FC是在启用加密后，其加密值
所在的地址，0xFFFFFFFF和定义“NO_CRP”的效果一样，但是代码不一样，大家可以自
 己通过修改来看效果，0xFFFFFFFF是无加密，手动修改前面的代码为如下：
;//<h>CodeReadProtectionlevel
 (CRP)
|;//;//;//;//|<o>|CRP_Level:<0xFFFFFFFF=> Disabled<0x12345678=>CRP1<0x87654321=>CRP2|
|----|----|----|
|;//||<0x43218765=>CRP3(OTP 模式，你确认吗?)|
|;//|</h>||
|CRP_Level|EQU|0xFFFFFFFF|
||IF|:LNOT::DEF:NO_CRP|
||AREA||.ARM.__at_0x02FC|, CODE, READONLY|
|CRP_Key|DCD|CRP_Level|
||ENDIF||
打开右边的“ConfigurationWizard”项，这是“Option”中会多出一个选项，此时
的加密级别为默认的“Disabled”，即代码中所定义的CRP_Level EQU0xFFFFFFFF,。
设置加密级别为“CRP2”，保存并编译。
得到的文件中我们可以看到在0x2FC 的位置有我们的加密值了，
例如将文件烧写到LPC1343器件，再按“BOOT”键进入U盘BOOT模式，打开
 文件可以看到所有数据“00”了，说明加密成功。如图：
CRP3为OTP模式，一般作为不再修改的产品或有写好的IAP做升级时使用，否则请
 设置为低的加密模式。
http://blog.csdn[.NET](http://lib.csdn.net/base/dotnet)/chengfeng135/article/details/49638435
http://blog.csdn[.Net](http://lib.csdn.net/base/dotnet)/chengfeng135/
