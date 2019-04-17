# Python十分钟制作属于你自己的个性logo - zhusongziye的博客 - CSDN博客





2018年05月12日 21:54:30[zhusongziye](https://me.csdn.net/zhusongziye)阅读数：467








词云的使用相信大家已经不陌生了，使用很简单，直接调用`wordcloud`包就可以了。它的主要功能是根据文本词汇和词汇频率生成图片，从中可以直观的看出各个词汇所占比重。最近正好想做一个人的logo，于是乎决定使用词云来制作完成。

#### wordcloud安装

```
pip install wordcloud
```

使用 pip 安装你肯定会遇到一个坑，安装过程中可能会报错，提示你安装 Microsoft Visual C++ 14.0，但是这个安装过程很耗时。

有另一个方法可以解决，就是下载相应的`whl`文件安装。博主下载的是  wordcloud-1.4.1-cp36-cp36m-win_amd64.whl 文件，如果下载不了，可以在公众号后台输入 “`wordcloud`” 获取。

下载链接：https://www.lfd.uci.edu/~gohlke/pythonlibs/#wordcloud

文件下载后，cmd 进入`whl`文件所在文件夹下，然后输入以下命令：

```
pip install wordcloud-1.4.1-cp36-cp36m-win_amd64.whl
```

![](https://img-blog.csdn.net/20180512215339555?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3podXNvbmd6aXll/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)

#### wordcloud代码使用

安装成功后我们马上开始制作我们的图片，代码如下：

```
from os import path
from PIL import Image
import numpy as np
import matplotlib.pyplot as plt
from wordcloud import WordCloud, STOPWORDS, ImageColorGenerator
d = path.dirname(__file__)
# 读文本文件
text = open(path.join(d, 'data.txt')).read()
# 读取自定义图片
alice_coloring = np.array(Image.open(path.join(d, "pic.jpg")))
# 你可以通过 mask 参数 来设置词云形状
wc = WordCloud(background_color="white",max_words=2000, 
                mask=alice_coloring, max_font_size=60,random_state=102,scale=8,
                font_path="C:\Windows\Fonts\msyhbd.ttf").generate(text)
wc.generate_from_text(text)
print('加载文本')
# 改变字体颜色
img_colors = ImageColorGenerator(alice_coloring)
# 字体颜色为背景图片的颜色
wc.recolor(color_func=img_colors)
# 显示词云图
plt.imshow(wc, interpolation="bilinear")
# 是否显示x轴、y轴下标
plt.axis('off')
plt.show()
# 获得模块所在的路径的
d = path.dirname(__file__)
# 将多个路径组合后返回
wc.to_file(path.join(d, "h16.jpg"))
print('生成词云成功!')
```

上面 text 文本内容是通过爬虫采集的，爬取的关于数据科学的一篇文章，有了文本源之后准备好你想要生成词云的图片就行了。

配置`wordcloud`的参数对于图片效果尤为重要，下面我们着重介绍一下`wordcloud`的参数含义：
- 
font_path : string字体路径，需要展现什么字体就把该字体路径+后缀名写上，如：font_path = '黑体.ttf'

- 
width : int (default=400)输出的画布宽度，默认为400像素

- 
height : int (default=200)输出的画布高度，默认为200像素

- 
prefer_horizontal : float (default=0.90)词语水平方向排版出现的频率，默认 0.9

- 
mask : nd-array or None (default=None)若参数为空，则正常绘制词云。如果 mask 非空，设置的宽高值将被忽略，形状被 mask 取代。除白色的部分将不会绘制，其余部分会用于绘制词云。

- 
scale : float (default=1)按照比例进行放大画布，如设置为2，则长和宽都是原来画布的2倍。

- 
min_font_size : int (default=4)显示的最小的字体大小

- 
font_step : int (default=1)字体步长，如果步长大于1，会加快运算但是可能导致结果出现较大的误差。

- 
max_words : number (default=200)要显示的词的最大个数

- 
stopwords : set of strings or None设置需要屏蔽的词，如果为空，则使用内置的STOPWORDS

- 
background_color : color value (default=”black”)背景颜色，如background_color='white',背景颜色为白色。

- 
max_font_size : int or None (default=None)显示的最大的字体大小

- 
mode : string (default=”RGB”)当参数为“RGBA”并且background_color不为空时，背景为透明。

- 
relative_scaling : float (default=.5)词频和字体大小的关联性

- 
color_func : callable, default=None生成新颜色的函数，如果为空，则使用 self.color_func

- 
regexp : string or None (optional)使用正则表达式分隔输入的文本

- 
collocations : bool, default=True是否包括两个词的搭配

- 
colormap : string or matplotlib colormap, default=”viridis”给每个单词随机分配颜色，若指定color_func，则忽略该方法。


#### 效果展示

使用的照片是博主自己的照片，效果图如下：



![](https://img-blog.csdn.net/20180512215359596?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3podXNvbmd6aXll/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)



