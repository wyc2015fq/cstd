# sublime + mingw 搭建环境 - V__KING__的专栏 - CSDN博客





2014年05月22日 16:01:22[v__king__](https://me.csdn.net/V__KING__)阅读数：2932








http://www.52pojie.cn/thread-194691-1-1.html







本人菜鸟，本来一直在用VS2012来学习C，结果发现编译出来的结果经常会跟VC++6.0有出入。甚至是很离谱的出入。
所以现在改用了Sublime-Text3这个非常顺手的编译器。感谢论坛的某位大大发出的破解[汉化版](http://www.52pojie.cn/)。

但是写完C代码后，编译会出现这样的错误提示：

[Decode error - output not utf-8][Finished in 0.0s with exit code 1]
[shell_cmd: g++ "C:\Users\Administrator\Desktop\6.c" -o "C:\Users\Administrator\Desktop/6"]
[dir: C:\Users\Administrator\Desktop]
[path: C:\Program Files (x86)\NVIDIA Corporation\PhysX\Common;C:\Program Files\Broadcom\Broadcom 802.11 Network Adapter;;C:\Windows\system32;C:\Windows;C:\Windows\System32\Wbem;C:\Windows\System32\WindowsPowerShell\v1.0\;C:\Program
 Files\Microsoft\Web Platform Installer\;C:\Program Files (x86)\Microsoft ASP.NET\ASP.NET Web Pages\v1.0\;C:\Program Files (x86)\Windows Kits\8.0\Windows Performance Toolkit\;C:\Program Files\Microsoft SQL Server\110\Tools\Binn\]


主要的错误就是第句：[Decode error - output not utf-8]           翻译就是：[错误]不是UTF-8解码输出

网上找了一些解决方案，完全没有关于Sublime-Text3的，都是Sublime-Text2的。或许可以套用，但是本人水平实在不够，英文更是烂的没有下限。根本无法解决。

找到了一个挺详细的Sublime-Text2搭建C程序开发环境的教程，跟大家分享一下：


**首先去[官方网站](http://www.sublimetext.com/2)下载软件，或者直接在网上下载一个汉化+破解的文件。然后是安装·······**

**下面就开始介绍怎么在Windows上搭建C/C++环境了。**

**因为我们用到的C/C++编译器使用的是gcc/g++，所以需要下载安装MinGW,**

**1.下载地址[http://sourceforge.net/projects/mingw/](http://sourceforge.net/projects/mingw/)。这个是边下载边安装的，下载完成即安装完成。因为是下载安装的，所以需要的时间会很长。特别是全部都选中的情况下。**

**2，设置环境变量。右击我的电脑，点属性->高级->环境变量。然后：1、在PATH里加入C:\MinGW\bin，记得，如果里面还有其他的变量，记得要加个分号啊，分号得在英文输入模式下输入的。2、新建LIBRARY_PATH变量，如果有的话，在值中加入C:\MinGW\lib，这是标准库的位置。3、新建C_INCLUDEDE_PATH变量，值设为C:\MinGW\include。具体路径请根据你的MinGW选择。（这个是看到栋栋哥的日志学到的）**

**下面就是要判断一下我们的MinGW是否安装成功。直接运行cmd命令行，输入g++ -v**
![](http://www.52pojie.cn/data/attachment/forum/201305/01/154402hjnnj4qponeqnmln.png)


**如果出现上面的画面，说明了你安装成功了，否则就是环境变量设置的问题。需要修改一下环境变量。在已经存在的环境变量前面添加上mingw的路径。因为Windows的环境变量修改之后不会立马生效，需要重启电脑。如果想省事，不想重启电脑的话可以直接在cmd中输入set path c:\这样环境变量就会被强制刷新。**

**Windows下，要在Sublime Text 2中实现编译、运行C/C++代码，需要修改或新建一个C++编译配置。**

**具体是：**

**Sublime Text 2中Tools –> Build System –> New Build System…**

**输入如下内容，并将文件保存为C++Bulider.sublime-bulid。**



[AppleScript] *纯文本查看 复制代码*



[?](http://www.52pojie.cn/#)

```
`01`

`{`

`02`

`     ``"cmd"``:``[``"g++"``,``"${file}"``,``"-o"``,``"${file_path}/${file_base_name}"``]``,`

`03`

`     ``"file_regex"``:``"^(..[^:]*):([0-9]+):?([0-9]+)?:? (.*)$"``,`

`04`

`     ``"working_dir"``:``"${file_path}"``,`

`05`

`     ``"selector"``:``"source.c, source.c++"``,`

`06`

`     ``"shell"``:``true``,`

`07`

`     ``"variants"``:`

`08`

`     ``[`

`09`

`          ``{`

`10`

`               ``"name"``:``"Run"``,`

`11`

`               ``"cmd"``:`
```
[
```
`"start"``,``"${file_path}/${file_base_name}.exe"``]`

`12`

`          ``}`

`13`

`     ``]`

`14`

`}`
```




[backcolor=rgb(248, 248, 248) !important]
**在Windows中，该文件被保存在Sublime Text 2目录下的Data\Packages\User中。(下面的代码不能直接运行)**


[AppleScript] *纯文本查看 复制代码*



[?](http://www.52pojie.cn/#)

```
`01`

`{`

`02`

`     ``"cmd"``:``[``"g++"``,``"${file}"``,``"-o"``,``"${file_path}/${file_base_name}"``]``,`

`03`

`     ``"file_regex"``:``"^(..[^:]*):([0-9]+):?([0-9]+)?:? (.*)$"``,`

`04`

`     ``"working_dir"``:``"${file_path}"``,`

`05`

`     ``"selector"``:``"source.c, source.c++"``,`

`06`

`     ``"shell"``:``true``,`

`07`

`     ``"variants"``:`

`08`

`     ``[`

`09`

`          ``{`

`10`

`               ``"name"``:``"Run"``,`

`11`

`               ``"cmd"``:`
```
[
```
`"start"``,``"${file_path}/${file_base_name}.exe"``]`

`12`

`          ``}`

`13`

`     ``]`

`14`

`}`
```








**搭建好C/C++编译环境后，Sublime Text 2中编译运行C/C++代码了。**

**ctrl+B构建，ctrl+shift+B运行。**

**至于插件，自己搜一下就好了，网上一大堆。**

**保存一下自己建的模版**


[AppleScript] *纯文本查看 复制代码*



[?](http://www.52pojie.cn/#)

```
`01`

`<``snippet``>`

`02`

`    ``<``description``>``cpp_modle``<``/``description``>`

`03`

`    ``<``content``>``<``![CDATA[`

`04`

`/``*``*`

`05`

`*``*``author ``:``Skymoon ``*``*`

`06`

`*``*``*``*`

`07`

`*``*``--题**`

`08`

`*``*``2013``年月日``*``*`

`09`

`*``*``/`

`10`

`#include<iostream>`

`11`

`#include<algorithm>`

`12`

`#include<cstring>`

`13`

`#include<cstdio>`

`14`

`#include<queue>`

`15`

`#include<cstdlib>`

`16`

`using``namespace std;`

`17`

` `

`18`

`int ``main``(``)`

`19`

`{`

`20`

`    ``freopen``(``"Text.txt"``,``"r"``,``stdin``)``;`

`21`

`    ``int i``,``j``,``k ``,``l ``,``m``,`
```
n
```
`,``T ;`

`22`

`    `

`23`

`     `

`24`

`    ``return`
```
0
```
`;`

`25`

`}``]]``>``<``/``content``>`

`26`

`    ``<``tabTrigger``>``hear``<``/``tabTrigger``>`

`27`

`    ``<``scope``>``source.c``,``source.objc``,``source.c``+``+``,``source.objc``+``+``<``/``scope``>`

`28`

`<``/``snippet``>`
```







**其实写到这里就应该算是结束了。但是我之前试图用VS2012的C++库直接用，结果没有成功。弄得VS2012也不能用了。卸载重装或者修复都不行。每次构建工程的时候都会提示**



**后来才发现其实是自己把mspdb.dll这个文件扔到了system32文件夹中。后来删除之后，就没有问题了········**
作者： Skymoon 嬠/color][Life](http://www.skymoon.biz/?cat=105),[Study](http://www.skymoon.biz/?cat=3)
 嬠/color]


**限于水平我只看得懂一部分，而且我也安装了VS2012，怕因为个种种问题使VS2012损坏。因为之前损坏过，若是修复不成功的话就得重装系统重新安装一遍。非常麻烦。所以在此顺便问题下sublime text 3是不是不支持直接写C程序的。还有出现[Decode error - output not utf-8]的解决办法。注意是C，不是JAVA。请不要随手将百度上JAVA出现[Decode error - output not utf-8]的解决方法随手复制过来。Thank’s   再次祝论坛越来越好！**


