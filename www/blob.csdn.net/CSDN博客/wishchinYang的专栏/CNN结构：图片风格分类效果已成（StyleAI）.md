# CNN结构：图片风格分类效果已成（StyleAI） - wishchinYang的专栏 - CSDN博客
2017年12月29日 15:21:17[wishchin](https://me.csdn.net/wishchin)阅读数：382
           CNN结构：图片风格分类效果已成。可以在色彩空间对图片风格进行分类，并进行目标分类。
           StyleAI构架：FasterRCnn + RandomTrees
          为何不使用MaskRCNN？ MaskRCNN-Windows-C++版本未翻译完成![哭](http://static.blog.csdn.net/xheditor/xheditor_emot/default/cry.gif)
![](https://img-blog.csdn.net/20171229151820874?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvd2lzaGNoaW4=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)
结果分析：
        一个检测框架+一个分类框架，主要作用在于特征提取。
