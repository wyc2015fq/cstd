# 关于VS2005+OGRE的问题 - 逍遥剑客 - CSDN博客
2007年08月31日 23:23:00[xoyojank](https://me.csdn.net/xoyojank)阅读数：2502
 用惯了VS,还是想试试VS+OGRE是个啥感觉,于是乎就配置了一下:
1.安装VS2005 Professional + MSDN
2.安装VS2005 SP1,不装据说不能运行
3.安装DirectX9 SDK Jun2007
4.安装OGRE1.4.4
5.下载OGRE的时候看到一个[debug symbols](http://downloads.sourceforge.net/ogre/Ogre_PDBs_vc8_v1.4.3.zip),于是也下了下来,安装
6.安装OgreSDK Wizard80_Eihort_v1_4_2
OK,用向导创建一个工程
编译.........
通过
运行.........
报了个错,55555555
![](https://p-blog.csdn.net/images/p_blog_csdn_net/xoyojank/OGRE_DLL_ERROR.JPG)
疯掉,翻遍google也没找出个结果来
无奈,拖了N天之后,到今天想想,重装一下OGRE吧,这次没装上那个debug symbols
编译,运行,成功!!!
HOHO~仔细一看那个东西的文件名:Ogre_PDBs_vc8_v1.4.3,可能是跟OGRE版本不一致的原因吧
天知道ogre3d.org为什么把它们放在一块,这不是误导人么???
看看AppWizard的示例程序,效果不错哈:
![](https://p-blog.csdn.net/images/p_blog_csdn_net/xoyojank/OGRE_Setup.JPG)
![](https://p-blog.csdn.net/images/p_blog_csdn_net/xoyojank/OGRE_Head.JPG)
