# wince 错误 Error: failed PB timebomb check - xqhrs232的专栏 - CSDN博客
2013年12月17日 15:16:10[xqhrs232](https://me.csdn.net/xqhrs232)阅读数：634
个人分类：[WINCE](https://blog.csdn.net/xqhrs232/article/category/906924)
原文地址::[http://blog.csdn.net/njuitjf/article/details/5582643](http://blog.csdn.net/njuitjf/article/details/5582643)

今天在搭ce6.0的环境时，碰到了一个错误。在Sysgen的时候，开始都没有出现错误，到 make run time image的时候， 弹出了如下的错误 ：
makeimg: run command: romimage F:/OSDesign/CHSOS/CHSOS/RelDir/DeviceEmulator_ARMV4I_Release/ce.bib
Error: failed PB timebomb check
Windows CE ROM Image Builder v4.0.120 Copyright (c) Microsoft Corporation
Built: Jul 25 2006 16:37:02
makeimg: FATAL ERROR: Command returned non-zero exit code 1 (dec).
makeimg: FATAL ERROR: Command returned non-zero exit code 1 (dec).
CHSOS - 3 error(s), 0 warning(s)
开始还以为是自己的环境配错了，是不是哪个补丁没有打，或者顺序大错了。后来到网上搜了一下，原来也有大虾遇到过这个问题，况且早已经被解招了。
解决方法有两个：
一是修改系统时间，把系统时间往回改些，冒充一把时间老人，让系统的时光倒流一下，呵呵。不过，这样比较麻烦，要经常去改系统时间。
第二个方法比较彻底，到pb的安装目录，如： C:/Program Files/Microsoft Platform Builder/6.00， 把文件 EtkRemov.inf 删除掉，然后新建一个文件，再将其重命名为 EtkRemov.inf。注意，不要直接新建一个命名为 EtkRemov.inf 的文件，否则由于filesystem tunnel，会导致新建的文件总是沿用老文件的创建时间。
这样，再去 make run time image， 就会发现一起都OK了。
