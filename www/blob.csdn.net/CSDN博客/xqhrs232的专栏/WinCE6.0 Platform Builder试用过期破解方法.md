# WinCE6.0 Platform Builder试用过期破解方法 - xqhrs232的专栏 - CSDN博客
2016年09月17日 00:40:51[xqhrs232](https://me.csdn.net/xqhrs232)阅读数：331
原文地址::[http://blog.chinaunix.net/uid-17188120-id-2820900.html](http://blog.chinaunix.net/uid-17188120-id-2820900.html)

WinCE6.0 Platform Builder有试用时间限制，使用此方法破解限制。
1、"在PB安装目录下有个etkremov.inf，试用期是以此文件的创建日期来计算的。将其删除，然后新建一个文件（注：具体创建方法见步骤2），再重命名为etkremov.inf，不要直接新建这个文件，否则由于filesystem tunnel，会导致新建的文件总是沿用老文件的创建时间。
2、在HKEY_LOCAL_MACHINE\SOFTWARE\Microsoft\Platform Builder\6.00\Directories项里面有Install Dir与IDE Install Dir字符串子项，如果没有创建。里面的值是一个路径，比如D:\WINCE600\,注意，一定要带最后一个“\”（注：这就是etkremov.inf文件的路径）。
修改系统时间，比如到2012.12.24日，然后在这个路径下创建文件etkremov.inf。里面不用写任何东西。然后把时间改回来，你的安装时间就相当于2012.12.24日了。
//=====================================================================================
备注::
1、platform builder 180过期的解决方法
   //1111----C:\Program Files\Microsoft Platform Builder\6.00\EtkRemov.inf
   //2222----新建一个文件,再手动改其名称为EtkRemov.inf------先要将老的EtkRemov.inf删除
   //3333----platform builder 180过期跟VS2005是不是正式版是没有关系的
