# MIUI类ROM如何正确修改dpi - ggabcda的博客 - CSDN博客





2017年09月10日 18:29:59[ggabcda](https://me.csdn.net/ggabcda)阅读数：2737








（以下以MIUI为例）



    在miui上，如果通过简单的修改build.prop会导致图标重绘错误，App图标分裂。此时配合一条简单的命令即可实现完美无bug的dpi修改。

1.使用终端模拟器执行su

2.执行wm density <dpi>（例：wm density 420）

3.修改build.prop的ro.sf.lcd_density=<你要修改的dpi>

4.重新启动

5.启动后若图标分裂，重新应用主题即可。



以上方法可完美解决miui下修改dpi导致的图标错位，图标撕裂以及天气、系统升级、文件管理白条问题。



