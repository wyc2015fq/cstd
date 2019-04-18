# StyleAI：印象坐标-感情色彩量化/感情近邻关系 - wishchinYang的专栏 - CSDN博客
2018年09月05日 15:18:16[wishchin](https://me.csdn.net/wishchin)阅读数：211
       色彩的搭配：怎样才算好？根据艺术设计理论，艺术也可以量化，感情色彩也是如此，找到合适的量化方式，就可以以计算机形式重新构建。
        进行图像感情色彩量化，使用了 OCR + 图方法，结果使用OpenCV在HSV色盘上画出来。这个色盘比 **潘通色卡** 和所谓的 莫兰迪色 配色强大多了。
       wishchin的花瓣链接：[http://huaban.com/p7viaradakf/](http://huaban.com/p7viaradakf/)
### 印象坐标
        参考：[新手学配色](https://www.uisdc.com/color-design-reading-notes-1).**配色印象坐标**
        精心筛选出能够表达人们对颜色的感觉的多个形容词，然后利用SD方法对不同工作、生活环境中的1000个人进行了相关的测试，用于测试的颜色是色相&色调（Tue & Tone）表中的116种不同的颜色。“单色印象空间”就是对这次测试的数据进行科学整理后得到的结果。从结果可以获知，人们对颜色进行评价时，最习惯使用的4个形容词是“柔和（Soft）”、“生硬（Hard）”、“动态（Dynamic）”、“静态（Static）。
![](https://img-blog.csdn.net/20180920152839220?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3dpc2hjaGlu/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)
### HS的印象坐标      
       一个关于漆器的颜色介绍：【珍藏版】最美配色致桼彩纷呈的你（第二辑），在网路上找到了源头——-西安生漆研究所-公众号   。关注微信号链接：[没有桼的VCR](http://www.a-site.cn/article/754236.html)[。](http://www.a-site.cn/article/754236.html)
![](https://img-blog.csdn.net/20180905150354440?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3dpc2hjaGlu/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)
一个色彩和近邻关系-以富足为例
![](https://img-blog.csdn.net/20180905150502113?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3dpc2hjaGlu/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)
一些关系-富足的灰调
![](https://img-blog.csdn.net/2018090515060774?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3dpc2hjaGlu/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)
量化后的结果——在HSV的色盘上
备注：因CSDN图片大小限制，对图片进行了压缩，jpg格式产生色差，原始PNG无损图片没有色差。H的取值范围为uchar[0,255]，S取值范围为double[0,255]，V值取255，最后转化为8bit-RGB图像。
![](https://img-blog.csdn.net/20180905151603945?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3dpc2hjaGlu/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)
近邻色的联系图
![](https://img-blog.csdn.net/20180905171910422?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3dpc2hjaGlu/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)
此图稍大....
