
# 反编译工具Reflector ILSpy - 萌萌的It人 www.itmmd.com - CSDN博客


2014年11月09日 20:32:57[Jlins](https://me.csdn.net/dyllove98)阅读数：2559


最近很长一段时间在为自己技术方便该如何做才会有新的长进而发愁，偶然，顿悟，决定通过反编译工具了解底层代码来进一步提升自己的能力。
于是有了周末宅在家里一天研究反编译工具。
不能浪费了一天的成果，至此，决定养成写博客的好习惯，记录下来。
ps：本人一直在搞.NET开发，自然是与VS打交道，百度，最常用的.NET反编译工具非Reflector莫属，介于本机上安装的是VS2013,所以非Reflector8.0+版本不可。
话说Reflector新版已然开始收费，不过已然无法阻止我泱泱大国破解之功力。
网上有一款相对新的.NET反编译工具ILSpy，开源，本想用它，后发现作为新生代，与VS的兼容失踪还是不如Reflector，而且实在不知道该如何用ILSpy进行反编译之后的源代码调试，故最终还是选择了破解Reflector。（若有大神精通ILSpy反编译调试，希望不吝赐教。）
本机最终选择的是Reflector8.2版本，网上破解之道很多，在此并不做详细说明，只补充一点：如果想要直接通过VS+Reflector进行代码的反编译debug功能。
激活Reflector时，必须选择以VS pro方式激活，否则是无法在vs中使用debug功能的。
令：需注意一点：本人在激活Reflector时不慎选择了基础标准模式，结果导致debug功能无法使用，反复卸载Reflector并重新安装，发现Reflector在卸载重装后不需要重新激活，这样以来也就无法重新选择激活Reflector的方式了，折腾一天之后，发现，只有将Reflector反激活（即取消激活）才能重新选择Reflector的激活方式，反激活的方式为删除
win7 64位体系，文件夹地位 C:\Users\{用户名}\AppData\Local\Red Gate\Licenses\.NET Reflector_v8_0.lic文件（需要先设置文件夹属性为显示隐藏文件）。
寄语：希望通过这种方式可以帮助自己以及大家在debug时可以看到源码，进而对net framework有更进一步的了解，从而对技术的长进有一定的帮助。
原创文章by[萌萌的it人](http://www.itmmd.com)www.itmmd.com

