# 10行python代码的词云 - 我相信...... - CSDN博客





2017年03月06日 10:22:56[半吊子全栈工匠](https://me.csdn.net/wireless_com)阅读数：8919








## 关于词云wordcloud

什么是词云呢？词云又叫文字云，是对文本数据中出现频率较高的“关键词”在视觉上的突出呈现，形成关键词的渲染形成类似云一样的彩色图片，从而一眼就可以领略文本数据的主要表达意思。

现在，可以从网络上找到各种各样的词云，下面一图来自沈浩老师的微博：

![演员词云.png](http://upload-images.jianshu.io/upload_images/73516-91cd50d4493d593a.png?imageMogr2/auto-orient/strip%7CimageView2/2/w/1240)

从百度图片中可以看到更多制作好的词云，例如

![百度词云.png](http://upload-images.jianshu.io/upload_images/73516-a73cca8caef1ee46.png?imageMogr2/auto-orient/strip%7CimageView2/2/w/1240)

## 词云制作有很多工具…..

从技术上来看，词云是一种有趣的数据可视化方法，互联网上有很多的现成的工具：
- Wordle是一个用于从文本生成词云图而提供的游戏工具
- Tagxedo 可以在线制作个性化词云
- Tagul 是一个 Web 服务，同样可以创建华丽的词云
- Tagcrowd 还可以输入web的url，直接生成某个网页的词云
- ……

## 十行代码

但是作为一个老码农，还是喜欢自己用代码生成自己的词云，复杂么？需要很长时间么？ 很多文字都介绍过各种的方法，但实际上只需要10行python代码即可。   

```
import matplotlib.pyplot as plt
from wordcloud import WordCloud
import jieba

text_from_file_with_apath = open('/Users/hecom/23tips.txt').read()

wordlist_after_jieba = jieba.cut(text_from_file_with_apath, cut_all = True)
wl_space_split = " ".join(wordlist_after_jieba)

my_wordcloud = WordCloud().generate(wl_space_split)

plt.imshow(my_wordcloud)
plt.axis("off")
plt.show()
```

如此而已，生成的一个词云是这样的：

![研发管理词云.png](http://upload-images.jianshu.io/upload_images/73516-173f17d6842029a1.png?imageMogr2/auto-orient/strip%7CimageView2/2/w/1240)

看一下这10行代码：

1～3 行分别导入了画图的库，词云生成库和jieba的分词库；

4 行是读取本地的文件,代码中使用的文本是本公众号中的[《老曹眼中研发管理二三事》](https://mp.weixin.qq.com/s?__biz=MzAwOTcyNzA0OQ==&mid=2658971169&idx=1&sn=886407a5b5f9d0aa94523c88fc8717cb&chksm=80d33307b7a4ba11d26000994dfbdb3a2e1f9824159a11e5cce9573da93a99689b94d779321d#rd)。

5～6 行使用jieba进行分词，并对分词的结果以空格隔开；

7行对分词后的文本生成词云；

8～10行用pyplot展示词云图。

这是我喜欢python的一个原因吧，简洁明快。

## 执行环境

如果这十行代码没有运行起来，需要检查自己的执行环境了。对于完整的开发学习环境，可以参考本公众号[老曹眼中的开发学习环境](https://mp.weixin.qq.com/s?__biz=MzAwOTcyNzA0OQ==&mid=2658971039&idx=1&sn=b00d159b1f30ab10893f3448ba3a412f&chksm=80d330b9b7a4b9affde0f6550983ad4ee8c4e021bc33ea0fe03ff0e79599267d5685af9c20dd#rd)。 

对于面向python 的数据分析而言，个人喜欢Anaconda，可以去[https://www.continuum.io/downloads/](https://www.continuum.io/downloads/) 下载安装，安装成功后的运行界面如下：

![anaconda.png](http://upload-images.jianshu.io/upload_images/73516-ffa952507af7bb6b.png?imageMogr2/auto-orient/strip%7CimageView2/2/w/1240)

anaconda 是python 数据爱好者的福音。

安装wordcloud 和 jieba 两个库同样非常简单：

```
pip install wordcloud
pip install jieba
```

遇到的一个小坑，刚开始运行这十行代码的时候，只显式了若干彩色的小矩形框，中文词语显式不出来，以为是万恶的UTF8问题，debug一下，发现print 结巴分词的结果是可以显示中文的，那就是wordcloud 生成词语的字体库问题了。开源的好处来了，直接进入wordcloud.py 的源码，找字体库相关的代码

```
FONT_PATH = os.environ.get("FONT_PATH", os.path.join(os.path.dirname(__file__), "DroidSansMono.ttf"))
```

wordcloud 默认使用了DroidSansMono.ttf 字体库，改一下换成一个支持中文的ttf 字库， 重新运行一下这十行代码，就可以了。

## 看一下源码

既然进入了源码，就会忍不住好奇心，浏览一下wordcloud 的实现过程和方式吧。

wordcloud.py总共不过600行，其间有着大量的注释，读起来很方便。其中用到了较多的库，常见的random，os，sys，re（正则）和可爱的numpy，还采用了PIL绘图，估计一些人又会遇到安装PIL的那些坑.

生产词云的原理其实并不复杂，大体分成5步：
- 对文本数据进行分词，也是众多NLP文本处理的第一步，对于wordcloud中的process_text（）方法，主要是停词的处理
- 计算每个词在文本中出现的频率，生成一个哈希表。词频计算相当于各种分布式计算平台的第一案例wordcount， 和各种语言的hello world 程序具有相同的地位了，呵呵。
- 根据词频的数值按比例生成一个图片的布局，类IntegralOccupancyMap 是该词云的算法所在，是词云的数据可视化方式的核心。
- 将词按对应的词频在词云布局图上生成图片，核心方法是generate_from_frequencies,不论是generate（）还是generate_from_text（）都最终到generate_from_frequencies
- 完成词云上各词的着色,默认是随机着色

词语的各种增强功能大都可以通过wordcloud的构造函数实现，里面提供了22个参数，还可以自行扩展。

## 更多的小例子

看看一个准文言文的词云，本字来自本公众号去年的旧文——[妻](https://mp.weixin.qq.com/s?__biz=MzAwOTcyNzA0OQ==&tempkey=fvyv%2FjwflqEEOMJ5SzbIvx4prGrf0hg0ipeIkEn84%2BHjQ8wEpG0s1Dv1%2F6iKWpDQpP6R62J3Qx1IrznyKs7Oq7PDoAfKedp5tBdcz9laa8b4lRgoSDMvJodJCYYfYFAhT0%2FPwEcXqmxmdt9jqqVwpA%3D%3D&#rd)

![wife.png](http://upload-images.jianshu.io/upload_images/73516-514d5ec8b10663d5.png?imageMogr2/auto-orient/strip%7CimageView2/2/w/1240)

其中在构造函数中传入了关于大小的几个参数

```
width=800,height=400,max_font_size=84,min_font_size=16
```

自惭形秽，根本看不出文言文的色彩和对妻子的感情流露，不是好文字呀！

矩形的词云太简陋了，直接在图片上用词云来填充就有意思多了，wordcloud中采用的mask方式来实现的。换上一张自己的照片，用[在谈《全栈架构师》](https://mp.weixin.qq.com/s?__biz=MzAwOTcyNzA0OQ==&mid=2658971132&idx=1&sn=d1f5848d621ff0c894988665af00b4a2&chksm=80d330dab7a4b9ccec8a514a7a719e4c2907aa3ea318d23b20efa1e7fccd02238e0e99961fa1#rd)中的文字，词云出来的效果是这样的

![chw_fullstack.png](http://upload-images.jianshu.io/upload_images/73516-6416eadb46c48681.png?imageMogr2/auto-orient/strip%7CimageView2/2/w/1240)

较难看出肖像的特点了，还好，可以遮丑。其中增加了3行代码

```
from PIL import Image
import numpy as np
abel_mask = np.array(Image.open("/Users/hecom/chw.png"))
```

在构造函数的时候，将mask传递进去即可：

```
background_color="black", mask=abel_mask
```

自己做的这些词云图片还是太陋，这就是原型简单，好的产品困难呀！做好一个漂亮词云的图片，还是要在诸多细节上下功夫的。

例如： 

分词的处理，“就是”这样没有意义的词不应该出现在词云里呀？ 

所展示关键词的目的性选择？ 

如何选择一个合适的字库？ 

如何更好地自主着色？ 

图片的预处理，如何让图片和词云表达原图片的主要特征？ 

……
## 词云的背后

词云的背后实际上是数据集成处理的典型过程，我们所熟知的6C,如下图：

![6C.jpg](http://upload-images.jianshu.io/upload_images/73516-158fc2ad783ce5a0.jpg?imageMogr2/auto-orient/strip%7CimageView2/2/w/1240)
- Connect: 目标是从各种各样数据源选择数据，数据源会提供APIs,输入格式,数据采集的速率,和提供者的限制.
- Correct: 聚焦于数据转移以便于进一步处理，同时保证维护数据的质量和一致性
- Collect: 数据存储在哪，用什么格式，方便后面阶段的组装和消费
- Compose: 集中关注如何对已采集的各种数据集的混搭, 丰富这些信息能够构建一个引入入胜的数据驱动产品。
- Consume: 关注数据的使用、渲染以及如何使正确的数据在正确的时间达到正确的效果。
- Control: 这是随着数据、组织、参与者的增长，需要的第六个附加步骤，它保证了数据的管控。 

这十行代码构建的词云，没有通过API从公众号(wireless_com)直接获取,简化和抽象是工程化的典型方式，这里至今复制粘贴，甚至省略了correct的过程，直接将数据存储在纯文本文件中，通过jieba分词进行处理即compose，使用词云生成可视化图片用于消费consume，把一个个自己生成的词云组织到不同的文件目录便于检索算是初步的管控control吧。

#### 参考网站
- [https://www.continuum.io](https://www.continuum.io)
- [https://github.com/fxsjy/jieba](https://github.com/fxsjy/jieba)
- [https://github.com/amueller/word_cloud](https://github.com/amueller/word_cloud)

![这里写图片描述](https://img-blog.csdn.net/20170306110521803?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvd2lyZWxlc3NfY29t/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)




