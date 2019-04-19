# IAR for AVR设置生成HEX文件 - xqhrs232的专栏 - CSDN博客
2015年04月29日 23:17:11[xqhrs232](https://me.csdn.net/xqhrs232)阅读数：1797
原文地址::[http://blog.163.com/lbc_tl@126/blog/static/7245120720108311212871/](http://blog.163.com/lbc_tl@126/blog/static/7245120720108311212871/)
相关文章
1、IAR中如何生成hex文件----[http://blog.sina.com.cn/s/blog_69871f290100jrxg.html](http://blog.sina.com.cn/s/blog_69871f290100jrxg.html)
 刚刚接触IAR for AVR，苦于不能生成HEX文件，通过摸索，生成HEX文件的设置如下：
1、首先选择项目
![IAR for AVR设置生成HEX文件 - 默鸣 - 流浪天涯亦默鸣的博客](http://img.ph.126.net/DKXHrnVMN0yuzpky1F4xUw==/3361374171879682693.jpg)
2、Project   ->  Options 
![IAR for AVR设置生成HEX文件 - 默鸣 - 流浪天涯亦默鸣的博客](http://img.ph.126.net/o5RiaiREf7g9RpHNnP-Vpg==/2357352929952192355.jpg)
3、在选项卡中如下图设置
![IAR for AVR设置生成HEX文件 - 默鸣 - 流浪天涯亦默鸣的博客](http://img306.ph.126.net/phggqfJ2SDdriXxD4D2MZQ==/3858458980749982768.jpg)
4、在选项卡中如下图设置，点击【OK】即可。
![IAR for AVR设置生成HEX文件 - 默鸣 - 流浪天涯亦默鸣的博客](http://img.ph.126.net/Sn0uSLpU9V08JzmVWv5gaw==/2343560656093425052.jpg)
经测试生成的HEX文件可以直接用ProgISP通过USBASP下载、运行。
