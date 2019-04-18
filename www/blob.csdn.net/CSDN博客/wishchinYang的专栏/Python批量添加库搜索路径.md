# Python批量添加库搜索路径 - wishchinYang的专栏 - CSDN博客
2015年12月18日 18:18:49[wishchin](https://me.csdn.net/wishchin)阅读数：651
        被win10 给坑了，换回Win7. 重装系统后，继续使用Python，Eclipse不用重装，pydev不用重装，只需重装Python2.7.6 X64 for win即可。然后，默认已安装的Python包可以直接使用。
        重新配置PyDev , 设置好Python路径，添加了默认的库路径。
        又有一个问题，前期在D盘里编写了不少Python工程，需要把目录一个一个包含进去，太费劲了。
终于搜索到一个方法：
        在Python27\Lib\site-packages\ 目录下添加一个 *.pth的文本文件，把路径一行一行地填写进去，保存。打开PyDev的Eclipse 会自动弹出一个新增加目录的窗口，点击全选应用即可。
       完成。
