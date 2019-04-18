# python Wing IDE编辑器的中文显示解决方法 - bigfacesafdasgfewgf - CSDN博客





2015年04月01日 10:41:39[bigface1234fdfg](https://me.csdn.net/puqutogether)阅读数：2615








在安装好Wing IDE编辑器之后，输入中文的时候会出现小方格，也就是默认不能显示中文。这个时候我们需要修改编辑器默认的字体。 

解决方法如下： 

解决的办法是找到 pango.aliases 文件。 

如：C:\Program Files\Wing IDE 4.1\bin\gtk-bin\etc\pango目录下 

编辑pango.aliases可以用记事本打开，也可以选择你熟悉的任何代码编辑器 

修改为如下：
```
courier = "microsoft yahei"

sans = "microsoft yahei"
serif = "microsoft yahei"
monospace = "microsoft yahei"
```

也就是把字体显示为“微软雅黑”。

其余支持中文显示的字体还有： 

Windows的一些字体：

新細明體：PMingLiU 

細明體：MingLiU 

標楷體：DFKai-SB 

黑体：SimHei 

宋体：SimSun 

新宋体：NSimSun 

仿宋：FangSong 

楷体：KaiTi 

仿宋_GB2312：FangSong_GB2312 

楷体_GB2312：KaiTi_GB2312 

微軟正黑體：Microsoft JhengHei 

微软雅黑体：Microsoft YaHei




