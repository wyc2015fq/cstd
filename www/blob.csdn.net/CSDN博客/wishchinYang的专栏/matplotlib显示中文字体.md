# matplotlib显示中文字体 - wishchinYang的专栏 - CSDN博客
2013年08月08日 14:58:33[wishchin](https://me.csdn.net/wishchin)阅读数：2115
[原始地址：http://zanyongli.i.sohu.com/blog/view/195716528.htm]()
**matplotlib 1.0.0版**
对于3.0的可能不太适用，要注意语法结构！
```python
C:/Python26/Lib/site-packages/matplotlib/mpl-data/matplotlibrc文件(Windows)
/usr/share/matplotlib/mpl-data/matplotlibrc (Linux)：
font.family         : sans-serif         #打开该选项
font.sans-serif     : Microsoft YaHei, Bitstream Vera Sans, Lucida Grande, Verdana, Geneva, Lucid, Arial, Helvetica, Avant Garde, 
sans-serif          #添加“Microsoft YaHei”,该行指示缺省字体
```
**注：在font.sans-serif中添加的字体名称必须正确，可以在**
```python
$HOME/.matplotlib/fontList.cache
C:/Users/用户名/.matplotlib/fontList.cache
```
文件中 ，通过查找文件名找到对应的名称，如上面的"Microsoft YaHei"的字体文件名称为“msyh.ttf"，搜索一下就找到了。一些可用的字体名称见示例代码.
如果还不行，试着将字体文件copy到
/usr/share/matplotlib/mpl-data/fonts/ttf(Windows)
C:/Python26/Lib/site-packages/matplotlib/mpl-data/fonts/ttf(Linux)
目录下，再试。示例代码如下：
```python
# -*- coding: utf-8 -*-
import numpy as np
import matplotlib.pyplot as plt
plt.figure(figsize=(8,4))
a = plt.text(0.05, 0.05, u"直线和二次曲线的交点")
#Microsoft YaHei,FangSong,YouYuan,SimHei,STKaiti,STSong,SimSun-ExtB,Webdings
plt.text(0.05, 0.95, u "STSong直线", fontproperties='STSong' )
plt.text(0.05, 0.85, u"STKaiti直线", fontproperties='STKaiti')
plt.text(0.05, 0.75, u"FangSong直线", fontproperties='FangSong')
plt.text(0.05, 0.65, u"YouYuan直线", fontproperties='YouYuan')
plt.text(0.05, 0.55, u"SimHei直线", fontproperties='SimHei')
plt.text(0.05, 0.45, u"Microsoft YaHei微软雅黑", fontproperties='Microsoft YaHei')
plt.text(0.05, 0.35, u"STCaiyun华文彩云", fontproperties='STCaiyun')
plt.show()
```
注意：字符串,都用u"..."的形式.（文件编码utf-8 加上" # coding = utf-8  "一行.）
