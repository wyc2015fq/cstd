# VC++调试常见错误总结 - fanyun的博客 - CSDN博客
2016年06月27日 13:45:16[老樊Lu码](https://me.csdn.net/fanyun_01)阅读数：2043
所属专栏：[C/C++/C#开发实战365](https://blog.csdn.net/column/details/c-plus-plus-01.html)
1、errorC2859 vc90.idb is not the idb file that was used when this precompiled headerwas created。
解决方法：选中工程-->点击右键属性-->ConfigurationProperties-->C/C++-->Output
 Files-->Program Database File Name中的$(IntDir)/vc90.pdb改为$(IntDir)/(工程名).pdb即可。
2、Failedto return new code element.
解决方法：关闭工程，删除工程中的.ncb文件，再重新打开工程即可。
3、makesure that the file is not open by another process and is not write-protected
解决方法：关闭工程，再重新打开即可。
网上有的说：Tools-->Options-->Projectsand Solutions-->Builde and Run-->将maximum
 number of parallelproject builds该为1即可，可是试试了并不起作用。
4、add/removeoperation is impossible,because the code element 'Cxxx' is read only
解决方法：关闭工程，删除工程中的.ncb和.suo两个文件，再重新打开工程即可。
5、errorC2471 cannot update program database …..debug\vc90.pdb
解决方法：在属性配置里(1)、C\C++-->General-->Debug
 Information format：改为C7 Compatible (/Z7)；(2)、C\C++-->Code
 Generation-->Enable String Pooling：改为Yes (/GF)；(3)、Linker-->Debuging-->GeneralDebug
 Info：改为Yes(/DEBUG)即可。
6、Errorspawning 'cmd.exe'
解决方法：Tools-->Options-->Projectsand Solutions-->VC++ Directories：点击New
 Line(类似文件夹图标)添加一行(cmd.exe所在目录)：C:\WINDOWS\system32\，OK即可。
7、断点调试时，出现thereis no source code available for the current location
解决方法：Tools-->Options-->Debugging-->General-->把Requiresource
 files to exactly match the original version前面的勾去掉即可。
