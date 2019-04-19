# RVDS2.2 的破解方法！ - xqhrs232的专栏 - CSDN博客
2010年05月14日 17:43:00[xqhrs232](https://me.csdn.net/xqhrs232)阅读数：2911
原文地址::
[http://iwillfy2003.blog.sohu.com/103784497.html](http://iwillfy2003.blog.sohu.com/103784497.html)
下载一个crack破解文件，里面有几个文件：keygen.exe, patch.exe,readme.txt!
其中readme.txt说的不是很全，这里按照下面的破解方法即可。
1）用generate产生license   file   （注意自己的系统时间，最好是真实的当前时间，如果时间比较旧的话，产生的license   file   将不能注册。license   file   和系统时间、网卡物理地址、硬盘的序列号有关）   
2）安装软件   
3）license   Wizard   选   Install   Wizard   ...   选择license   file   目录   
4）应用补丁注入工具Patch.exe给下边列出的文件注入校验和。文件目录见下边。 
%Install   Path%/IDEs/CodeWarrior/CodeWarrior/5.6.1/1592/win_32-pentium/bin/Plugins/License/oemlicense.dll   
%Install   Path%/IDEs/CodeWarrior/RVPlugins/1.0/86/win_32-pentium/oemlicense/oemlicense.dll   
%Install   Path%/RDI/armsd/1.3.1/66/win_32-pentium/armsd.exe   
%Install   Path%/RDI/AXD/1.3.1/98/win_32-pentium/axd.exe   
%Install   Path%/RVCT/Programs/2.2/349/win_32-pentium/armasm.exe   
%Install   Path%/RVCT/Programs/2.2/349/win_32-pentium/armcc.exe   
%Install   Path%/RVCT/Programs/2.2/349/win_32-pentium/armcpp.exe   
%Install   Path%/RVCT/Programs/2.2/349/win_32-pentium/armlink.exe   
%Install   Path%/RVCT/Programs/2.2/349/win_32-pentium/fromelf.exe   
%Install   Path%/RVCT/Programs/2.2/349/win_32-pentium/tcc.exe   
%Install   Path%/RVCT/Programs/2.2/349/win_32-pentium/tcpp.exe   
%Install   Path%/RVD/Core/1.8/734/win_32-pentium/bin/tvs.exe   
%Install   Path%/RVD/Core/1.8/734/win_32-pentium/bin/xry100.dll   
%Install   Path%/RVARMulator/ARMulator/1.4.1/206/win_32-pentium/armiss.sdi   
%Install   Path%/RVARMulator/ARMulator/1.4.1/206/win_32-pentium/armulate.sdi   
%Install   Path%/RVARMulator/ARMulator/1.4.1/206/win_32-pentium/v6armiss.sdi   
%Install   Path%/RVARMulator/v6ARMulator/1.4.1/238/win_32-pentium/v6thumb2.sdi   
%Install   Path%/RVARMulator/v6ARMulator/1.4.1/238/win_32-pentium/v6trustzone.sdi 
Patch后，应该就可以使用了。
--------------------------------------------------------------------
网上还有直接破解Flexlm的方法，太强了，现贴出来。这个方法我没有试过，不知道好用不。
先安装RVCT2。2 ，在网上下载 最新的616版板，注册机需考贝到安装的根目录。或C:/flexlm，运行生成license.dat，运行PATCH.exe。逐个选择下面所列的文件，就可以了。 
%Install  Path%/IDEs/CodeWarrior/CodeWarrior/5.6.1/1592/win_32-pentium/bin/Plugins/License/oemlicense.dll  
%Install  Path%/IDEs/CodeWarrior/RVPlugins/1.0/86/win_32-pentium/oemlicense/oemlicense.dll  
%Install  Path%/RDI/armsd/1.3.1/66/win_32-pentium/armsd.exe  
%Install  Path%/RDI/AXD/1.3.1/98/win_32-pentium/axd.exe  
%Install  Path%/RVCT/Programs/2.2/349/win_32-pentium/armasm.exe  
%Install  Path%/RVCT/Programs/2.2/349/win_32-pentium/armcc.exe  
%Install  Path%/RVCT/Programs/2.2/349/win_32-pentium/armcpp.exe  
%Install  Path%/RVCT/Programs/2.2/349/win_32-pentium/armlink.exe  
%Install  Path%/RVCT/Programs/2.2/349/win_32-pentium/fromelf.exe  
%Install  Path%/RVCT/Programs/2.2/349/win_32-pentium/tcc.exe  
%Install  Path%/RVCT/Programs/2.2/349/win_32-pentium/tcpp.exe  
%Install  Path%/RVD/Core/1.8/734/win_32-pentium/bin/tvs.exe  
%Install  Path%/RVD/Core/1.8/734/win_32-pentium/bin/xry100.dll  
%Install  Path%/RVARMulator/ARMulator/1.4.1/206/win_32-pentium/armiss.sdi  
%Install  Path%/RVARMulator/ARMulator/1.4.1/206/win_32-pentium/armulate.sdi  
%Install  Path%/RVARMulator/ARMulator/1.4.1/206/win_32-pentium/v6armiss.sdi  
%Install  Path%/RVARMulator/v6ARMulator/1.4.1/238/win_32-pentium/v6thumb2.sdi  
%Install  Path%/RVARMulator/v6ARMulator/1.4.1/238/win_32-pentium/v6trustzone.sdi 
------------------------------------------------------------------------
祝大家好运。
参考链接：[http://topic.csdn.net/u/20070309/21/f2092c6e-c3e3-49fc-814e-4a28b9c56908.html](http://topic.csdn.net/u/20070309/21/f2092c6e-c3e3-49fc-814e-4a28b9c56908.html)
