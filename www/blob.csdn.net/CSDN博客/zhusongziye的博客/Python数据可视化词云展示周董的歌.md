# Python数据可视化词云展示周董的歌 - zhusongziye的博客 - CSDN博客





2018年06月10日 17:09:43[zhusongziye](https://me.csdn.net/zhusongziye)阅读数：546








## 准备工作

环境：Windows + Python3.6
IDE：根据个人喜好，自行选择
模块：
Matplotlib是一个 Python 的 2D数学绘图库

```
pip install matplotlib
import matplotlib.pyplot as plt
```

jieba中文分词库

```
pip install jieba
import jieba
```

wordcloud词云库

```
pip install wordcloud
from wordcloud import WordCloud
```




### 实战具体步骤：

1、读入txt文本数据。
2、结巴中文分词，生成字符串默认精确模式，如果不通过分词，无法直接生成正确的中文词云。
3、生成词云图，这里需要注意的是WordCloud默认不支持中文，所以这里需已下载好的中文字库。
（无自定义背景图：需要指定生成词云图的像素大小，默认背景颜色为黑色,统一文字颜色：mode='RGBA'和colormap='pink'）
4、显示图片。

总体而言只有四个步骤还是比较简单的那咱们先看一下实现的效果图吧！

![](https://img-blog.csdn.net/20180609180429897?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3podXNvbmd6aXll/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)

![](https://img-blog.csdn.net/20180609180438619?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3podXNvbmd6aXll/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)

看到效果图后发现里面的秘密没？

### 完整代码

```
# Matplotlib是一个 Python 的 2D数学绘图库
# 安装：pip install matplotlib
import matplotlib.pyplot as plt
# jieba中文分词库
# 安装：pip install jieba
import jieba
# wordcloud词云库
# 安装pip install wordcloud
from wordcloud import WordCloud
# 1.读入txt文本数据
text = open(r'.\jay.txt', "r").read()
#print(text)
# 2.结巴中文分词，生成字符串，默认精确模式，如果不通过分词，无法直接生成正确的中文词云
cut_text = jieba.cut(text)
# print(type(cut_text))
# print(next(cut_text))
# print(next(cut_text))
# print(next(cut_text))
# 必须给个符号分隔开分词结果来形成字符串,否则不能绘制词云
result = " ".join(cut_text)
#print(result)
# 3.生成词云图，这里需要注意的是WordCloud默认不支持中文，所以这里需已下载好的中文字库
# 无自定义背景图：需要指定生成词云图的像素大小，默认背景颜色为黑色,统一文字颜色：mode='RGBA'和colormap='pink'
wc = WordCloud(
        # 设置字体，不指定就会出现乱码
        font_path=r'.\simhei.ttf',
        # 设置背景色
        background_color='white',
        # 设置背景宽
        width=500,
        # 设置背景高
        height=350,
        # 最大字体
        max_font_size=50,
        # 最小字体
        min_font_size=10,
               )  # max_words=1000 ),mode='RGBA',colormap='pink')
# 产生词云
wc.generate(result)
# 保存图片
wc.to_file(r".\wordcloud.png")  # 按照设置的像素宽高度保存绘制好的词云图，比下面程序显示更清晰
# 4.显示图片
# 指定所绘图名称
plt.figure("jay")
# 以图片的形式显示词云
plt.imshow(wc)
# 关闭图像坐标系
plt.axis("off")
plt.show()
```





