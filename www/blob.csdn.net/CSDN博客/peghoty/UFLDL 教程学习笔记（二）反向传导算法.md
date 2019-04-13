
# UFLDL 教程学习笔记（二）反向传导算法 - peghoty - CSDN博客


2013年09月03日 23:33:02[皮果提](https://me.csdn.net/peghoty)阅读数：12029


[UFLDL](http://deeplearning.stanford.edu/wiki/index.php/UFLDL_Tutorial)（Unsupervised Feature Learning and Deep Learning）Tutorial 是由 Stanford 大学的[Andrew Ng](http://cs.stanford.edu/people/ang/)教授及其团队编写的一套教程，内容深入浅出，有很强的实用性，学习起来，让人有种酣畅淋漓的感觉。[邓侃博士](http://weibo.com/kandeng)于今年 2 月 20 日起，在新浪微博上召集志愿者对该教程进行翻译，并于 4 月 8 日全部完成，非常感谢所有[参与者](http://blog.sina.com.cn/s/blog_46d0a3930101h6nf.html)的辛勤劳动。本系列文章主要是对这套教程资料的整理，部分内容加入了自己的一些理解和注释。

第一篇  稀疏自编码器

![](https://img-blog.csdn.net/20130903232840031)![](https://img-blog.csdn.net/20130903232922437)![](https://img-blog.csdn.net/20140523095625125)![](https://img-blog.csdn.net/20140523095631484)
![](https://img-blog.csdn.net/20130908234857968)![](https://img-blog.csdn.net/20130908233903687)
![](https://img-blog.csdn.net/20140116111256375?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvaXRwbHVz/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)


作者: peghoty
出处:[http://blog.csdn.net/itplus/article/details/11022243](http://blog.csdn.net/itplus/article/details/11022243)
欢迎转载/分享, 但请务必声明文章出处.

