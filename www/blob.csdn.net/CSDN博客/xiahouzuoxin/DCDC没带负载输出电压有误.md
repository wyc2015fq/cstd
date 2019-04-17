# DCDC没带负载输出电压有误 - xiahouzuoxin - CSDN博客





2013年12月27日 11:04:08[xiahouzuoxin](https://me.csdn.net/xiahouzuoxin)阅读数：3327








有些DCDC模块需要接上负载输出电压才会正确。




项目中使用到一款WRA0512YS-3WR2的DCDC模块，输入为4.5V~9.0V，在未接任何负载时输出电压为+2.2V和-1.94V左右。

原理图为

![](https://img-blog.csdn.net/20131227110159250?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQveGlhaG91enVveGlu/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)


当接上负载（比如LED灯后），输出电压正常，为±12V。




这是什么原因，至今未弄清楚，希望知晓的大神能提供帮助！



