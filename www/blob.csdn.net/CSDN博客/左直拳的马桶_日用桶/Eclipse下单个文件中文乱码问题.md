# Eclipse下单个文件中文乱码问题 - 左直拳的马桶_日用桶 - CSDN博客
2014年08月14日 22:06:50[左直拳](https://me.csdn.net/leftfist)阅读数：5031
有时候用eclipse打开单个文件，会出现中文乱码问题。
这时可以点菜单栏 Edit -> Set Encoding，Other：UTF-8，通常可以解决问题。
----------------------------------------------------------
但UTF-8 分两种，带签名和无签名（？），如果选UTF-8还有乱码的话，不妨在这两种编码中切换下：
eclipse好像这方面功能较弱，UTF-8只显示一种。可以用其他编辑工具打开源代码，进行转换、保存。如微软的visual studio，nodepad++。
![](https://img-blog.csdn.net/20160531120352199?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQv/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)
