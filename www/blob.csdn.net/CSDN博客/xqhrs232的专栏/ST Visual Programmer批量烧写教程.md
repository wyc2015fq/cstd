# ST Visual Programmer批量烧写教程 - xqhrs232的专栏 - CSDN博客
2017年07月20日 15:54:47[xqhrs232](https://me.csdn.net/xqhrs232)阅读数：533
原文地址::[http://www.cnblogs.com/alanfeng/p/4915315.html](http://www.cnblogs.com/alanfeng/p/4915315.html)
相关文章
1、STVP建立自动烧录工程的方法----[https://wenku.baidu.com/view/de19a0bcec3a87c24028c44b.html](https://wenku.baidu.com/view/de19a0bcec3a87c24028c44b.html)
2、[使用ST TOOLS的STVP烧写程序快速入门](http://blog.csdn.net/chengfeng135/article/details/49638353)----[http://blog.csdn.net/chengfeng135/article/details/49638353](http://blog.csdn.net/chengfeng135/article/details/49638353)
3、在STVP里面解除STM8读保护----[https://wenku.baidu.com/view/f0edb005168884868662d674.html](https://wenku.baidu.com/view/f0edb005168884868662d674.html)
首先要说下为什么要建立烧写工程呢- -原因只有一个。因为你不可能将[源码](http://cpro.baidu.com/cpro/ui/uijs.php?adclass=0&app_id=0&c=news&cf=1001&ch=0&di=128&fv=19&is_app=0&jk=c70f2f21e0a264bb&k=%D4%B4%C2%EB&k0=%D4%B4%C2%EB&kdi0=0&luki=7&n=10&p=baidu&q=98059059_cpr&rb=0&rs=1&seller_id=1&sid=bb64a2e0212f0fc7&ssp2=1&stid=0&t=tpclicked3_hc&td=1831118&tu=u1831118&u=http%3A%2F%2Fwww.51hei.com%2Fbbs%2Fdpj-33634-1.html&urlid=0)工程交给别人去帮你烧程序。
- -然后要吐槽下ST官方的IDE。真的。用得我极度不爽。所以后来转战IAR。结果发现IAR没法批量生产- -因为IAR少程序貌似一定要在工程下。不能直接将HEX文件烧写进板子里。所以最后还是要用STVP来批量烧。
首先要准备好你的烧写文件。HEX或者S19。文件。（用IAR或者STVD生成的，前提必须保证你程序没问题- -这个肯定不用说）。
**第一步：然后打开STVP **
。打开之后是这样的 
**第二步：点击OPTION BYTE** 。ROP ON。这个是每次烧写完将FLASH锁住。以免别人读你的IC。
还有如果你晶振是24M的。WAITSTATE 要打开。然后点File->Save。然后保存。切记这个保存的是OPTIOIN BYTE！！如果你时钟是24M。或者程序要加锁。一定要生成这个HEX文件。
保存OPTION BYTE
**第三部：** Project->New 建立自己个烧写工程。保存。然后会弹出Project Edition。
第一个菜单。Conifguration。选择。ST-LINK。USB。SWIM。[芯片](http://cpro.baidu.com/cpro/ui/uijs.php?adclass=0&app_id=0&c=news&cf=1001&ch=0&di=128&fv=19&is_app=0&jk=c70f2f21e0a264bb&k=%D0%BE%C6%AC&k0=%D0%BE%C6%AC&kdi0=0&luki=3&n=10&p=baidu&q=98059059_cpr&rb=0&rs=1&seller_id=1&sid=bb64a2e0212f0fc7&ssp2=1&stid=0&t=tpclicked3_hc&td=1831118&tu=u1831118&u=http%3A%2F%2Fwww.51hei.com%2Fbbs%2Fdpj-33634-1.html&urlid=0)型号
然后选择Properties。勾选Unlock projtection....(这个是每次烧录前先解锁 自动保护。再进行烧录）
勾选Prompt for new [program](http://cpro.baidu.com/cpro/ui/uijs.php?adclass=0&app_id=0&c=news&cf=1001&ch=0&di=128&fv=19&is_app=0&jk=c70f2f21e0a264bb&k=program&k0=program&kdi0=0&luki=8&n=10&p=baidu&q=98059059_cpr&rb=0&rs=1&seller_id=1&sid=bb64a2e0212f0fc7&ssp2=1&stid=0&t=tpclicked3_hc&td=1831118&tu=u1831118&u=http%3A%2F%2Fwww.51hei.com%2Fbbs%2Fdpj-33634-1.html&urlid=0)ming
 cycle. (这个是每次烧写完之后会弹出询问是否进行下一次烧写）
然后点OPTION BYTE。ADD选择你第一步生成的那个OPTION BYTE.HEX文件。（这个hex文件是专门配置你OPTION BYTE信息的）
然后如果你有要EEPROM要烧的。就选择DATA MEMORY。步骤同上。
最后是PROGRAM  MEMORY。选择你STVD或者IAR生成的程序文件。然后点击确定
然后关闭STVD。会提示你是否保存。点击是 。
重新打开STVD。点击Project->Open.打开你刚才生成的工程文件。 
然后 板子连接上ST-LINK.上电。
在点击。PROGRAM  ALL TABS烧写。
选择”是“
选择”是“
到这里。就烧写完毕。然后就提示是否进行下一次烧写。这时候你只要连上新的板子。点击”是“就可以开始新的一次的烧写。
**很多人会说。为什么我烧写完之后。没有任何反应。我想一定无数人被这个问题坑死过。妈的。STVP稍烧完程序后。除非。你把ST-LINK拔掉。从别的地方给你板子供电。或者。你在电脑上把STVP关闭。然后在重新给板子上电。板子上的程序才能跑起来。因为程序你板子跟STVP的连接还没有断开！！太坑爹了。IAR就不会这样！！也许因为IAR烧完之后必须要点击GO。或者run。才会运行- -。。这个设计的很不合理。。。。 但是无力吐槽。。。。。。然后如果有什么错。请指出- -。**

