
# UFLDL 教程学习笔记（三）自编码与稀疏性 - peghoty - 博客园






# [UFLDL 教程学习笔记（三）自编码与稀疏性](https://www.cnblogs.com/peghoty/p/3798547.html)
UFLDL（Unsupervised Feature Learning and Deep Learning）Tutorial 是由 Stanford 大学的Andrew Ng教授及其团队编写的一套教程，内容深入浅出，有很强的实用性，学习起来，让人有种酣畅淋漓的感觉。邓侃博士于今年 2 月 20 日起，在新浪微博上召集志愿者对该教程进行翻译，并于 4 月 8 日全部完成，非常感谢所有参与者的辛勤劳动。本系列文章主要是对这套教程资料的整理，部分内容加入了自己的一些理解和注释。
第一篇  稀疏自编码器

![](http://img.blog.csdn.net/20130919091533796)![](http://img.blog.csdn.net/20130919091547062)![](http://img.blog.csdn.net/20130919091552000)
![](http://img.blog.csdn.net/20140116110725078?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvaXRwbHVz/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)![](http://img.blog.csdn.net/20140116110739046?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvaXRwbHVz/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)![](http://img.blog.csdn.net/20140116110745953?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvaXRwbHVz/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)![](http://img.blog.csdn.net/20140116110751296?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvaXRwbHVz/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)


作者: peghoty
出处:[http://blog.csdn.net/itplus/article/details/11449055](http://blog.csdn.net/itplus/article/details/11449055)
欢迎转载/分享, 但请务必声明文章出处.





