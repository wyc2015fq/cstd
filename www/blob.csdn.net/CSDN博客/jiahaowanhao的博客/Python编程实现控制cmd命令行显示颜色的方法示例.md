
# Python编程实现控制cmd命令行显示颜色的方法示例 - jiahaowanhao的博客 - CSDN博客


2018年06月23日 18:22:00[数据分析技术](https://me.csdn.net/jiahaowanhao)阅读数：132


[Python编程实现控制cmd命令行显示颜色的方法示例](http://cda.pinggu.org/view/25902.html)
本文实例讲述了python通过pil为png图片填充上背景颜色的方法。分享给大家供大家参考。具体分析如下：
png图片有些是没有背景颜色，如果希望以单色（比如白色）填充背景，可以使用下面的代码，这段代码将当前目录下的 jb51.net.png图片填充了白色背景。
使用指定的颜色的背景色即可，然后把该图片用alpha通道填充到该单色背景上。
比如下面使用白色背景：
im = Image.open('jb51.net.png')
x,y = im.size
try:
\# 使用白色来填充背景 from：www.jb51.net
\# (alpha band as paste mask).
p = Image.new('RGBA', im.size, (255,255,255))
p.paste(im, (0, 0, x, y), im)
p.save('jb51.net.png')
except:
pass
希望本文所述对大家的Python程序设计有所帮助。

