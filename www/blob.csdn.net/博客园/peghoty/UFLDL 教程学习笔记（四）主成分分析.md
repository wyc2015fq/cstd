
# UFLDL 教程学习笔记（四）主成分分析 - peghoty - 博客园






# [UFLDL 教程学习笔记（四）主成分分析](https://www.cnblogs.com/peghoty/p/3798545.html)
UFLDL（Unsupervised Feature Learning and Deep Learning）Tutorial 是由 Stanford 大学的Andrew Ng教授及其团队编写的一套教程，内容深入浅出，有很强的实用性，学习起来，让人有种酣畅淋漓的感觉。邓侃博士于今年 2 月 20 日起，在新浪微博上召集志愿者对该教程进行翻译，并于 4 月 8 日全部完成，非常感谢所有参与者的辛勤劳动。本系列文章主要是对这套教程资料的整理，部分内容加入了自己的一些理解和注释。
第二篇  预处理: 主成分分析和白化

![](http://img.blog.csdn.net/20130909072901140)![](http://img.blog.csdn.net/20130909072916765)![](http://img.blog.csdn.net/20130909072928109)![](http://img.blog.csdn.net/20130909072839109)![](http://img.blog.csdn.net/20130909072953687)![](http://img.blog.csdn.net/20130909073007015)![](http://img.blog.csdn.net/20130909073020312)![](http://img.blog.csdn.net/20130909072922421)

PCA 的推导中涉及较多的线性代数知识，如矩阵、特征值与特征向量、基表示、旋转等，此外还有一些概率统计方面的知识，如均值、方差、协方差等，为更好地理解文中 (2.1.1) 式关于协方差矩阵的理解，我在文章[《关于协方差矩阵的理解》](http://blog.csdn.net/itplus/article/details/11452743)中给出了较详细的推导和说明，有需要的读者可参考。
作者: peghoty
出处:[http://blog.csdn.net/itplus/article/details/11451327](http://blog.csdn.net/itplus/article/details/11451327)
欢迎转载/分享, 但请务必声明文章出处.





