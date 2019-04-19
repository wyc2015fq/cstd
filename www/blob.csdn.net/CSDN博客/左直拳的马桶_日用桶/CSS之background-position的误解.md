# CSS之background-position的误解 - 左直拳的马桶_日用桶 - CSDN博客
2011年09月08日 18:29:09[左直拳](https://me.csdn.net/leftfist)阅读数：1106标签：[css																[图形](https://so.csdn.net/so/search/s.do?q=图形&t=blog)](https://so.csdn.net/so/search/s.do?q=css&t=blog)
个人分类：[Web](https://blog.csdn.net/leftfist/article/category/674421)
对于background-position这个属性，原先我以为是对合成背景图来说的。就是将网站的一些常用图片合成一张，然后利用background-position定位在复合背景图中的某一位置，从而得到想要的图形。
但事实上不是的，这个**background-postion，其实是指背景图片位于容器的什么位置**。
比如说：background-postion:left center，是指背景图片位于容器中的水平靠左、垂直居中这个位置。
至于这个属性可以用来“定位”复合背景图片的某一部分，一定要与width、height相结合才行。
