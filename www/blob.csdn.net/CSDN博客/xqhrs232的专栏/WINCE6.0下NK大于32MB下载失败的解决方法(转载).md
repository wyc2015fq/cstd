# WINCE6.0下NK大于32MB下载失败的解决方法(转载) - xqhrs232的专栏 - CSDN博客
2010年02月06日 12:58:00[xqhrs232](https://me.csdn.net/xqhrs232)阅读数：1288
原文地址：[http://www.cnblogs.com/feishanm/archive/2009/08/04/1538284.html](http://www.cnblogs.com/feishanm/archive/2009/08/04/1538284.html)
CSDN相关讨论：[http://topic.csdn.net/u/20100104/08/bbdadd2d-3b43-41cb-9e39-51880108e438.html?46447](http://topic.csdn.net/u/20100104/08/bbdadd2d-3b43-41cb-9e39-51880108e438.html?46447)
通常情况，wince系统默认的image大小是32M，一般这个大小是够了。但如果添加的组件比较多，生成的NK.bin就会超过32M，这时就需要做一些修改了。
1、将项目的配置属性 "Run-time image can be larger than 32MB (IMGRAM64=1)"打上勾。
2、修改Eboot下的loader.h-----image_cfg.h/inc两个文件跟这个相关
#define ROM_RAMIMAGE_SIZE      0x02300000
这里我将image调整到35M
3、修改config.bib
       NK      80200000 02300000 RAMIMAGE    
       RAM     82500000 01B00000 RAM
       ……
       ……
       ROMSTART = 80200000
       ROMWIDTH = 32
       ROMSIZE = 02300000
修改规则：
NK start 80200000为image开始，与Eboot对应
NK size 2300000 为image大小，与Ebbot对应
RAM start 82500000为RAM开始地址，NKstart+NKsize=82500000
RAM size 01B00000为RAM大小，84000000（64M flash）- RAMSTART=01B00000 
ROMSTART = NKstart
ROMSIZE = NKsize
由此可知，NK调大，RAM就会减小。此外ROMSIZE还是设置NK.nbX的大小。
这时你的内核只要不超过35M就OK了。
此外，CSDN上的codewarrior朋友建议将一些大的组件(如汉字)库放到user store(如SD卡、Flash等)来减少内核大小，这也是个好办法，NK加载时要到user store去加载，需要注意的是：在加载显示驱动前就要加载保存字体的存贮设备的驱动，否则中文字体不能加载。将字体保存到注册表保存的存贮设备上，因为该存贮设备驱动很早就加载了。   
