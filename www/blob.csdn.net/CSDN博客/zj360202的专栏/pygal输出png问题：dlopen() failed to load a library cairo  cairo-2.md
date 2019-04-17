# pygal输出png问题：dlopen() failed to load a library: cairo / cairo-2 - zj360202的专栏 - CSDN博客





2018年01月17日 17:20:17[zj360202](https://me.csdn.net/zj360202)阅读数：541








## 背景


最近一个比较棘手的问题，需要python程序可以直接根据数据生成图片。而之前做数据可视化，无论是echart还是其他和js相关的可视化库，都是通过js在浏览器中生成交互图像的。都不能直接生成图片，而py最常用的Matplotlib虽然数据图像准确度够了，但是美化太麻烦……最后找了一圈，发现了pygal这个，能生成本地图片美化程度也不错的库……然后又坑了

## pygal的坑


pygal的结果输出默认为svg，想要输出其他格式们还需要其他库的支持。官方建议安装库有： 

1. pip install lxml 

2. pip install cairosvg 

3. pip install tinycss 

4. pip install cssselect

## cairo的坑


而至少在windows下，cairosvg又是一个大坑，pip安装没有任何问题，然而试着import ，直接报错 

OS ERROR:dlopen() failed to load a library: cairo / cairo-2

## 解决方案


搜索N多结果，终于找到解决方案： 

下载GTK+（看清楚电脑x64还是x86，我下的是最新的） 
[http://win32builder.gnome.org/](http://win32builder.gnome.org/)

然后解压缩在比如c:/gtk文件夹下，官方建议别用winzip，但是我用好压软件解压的没有问题。 

将其中bin文件夹，加入进系统环境变量 

在cmd中运行： **pkg-config –cflags gtk+-3.0**

检查这两个是否正确 

c:\gtk\etc\pango\pango.modules 

c:\gtk\lib\gdk-pixbuf-2.0\2.10.0\loaders.cache 

c:\gtk\lib\gtk-3.0\3.0.0\immodules.cache 

再cmd运行：**gtk3-demo**

运行成功，说明没问题了。之后重新启动你的python环境，import cairo，不会报错。使用pygal也不会报错了。



