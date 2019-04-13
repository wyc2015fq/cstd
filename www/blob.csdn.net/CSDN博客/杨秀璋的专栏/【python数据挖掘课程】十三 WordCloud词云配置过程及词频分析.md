
# 【python数据挖掘课程】十三.WordCloud词云配置过程及词频分析 - 杨秀璋的专栏 - CSDN博客

2017年03月21日 14:38:14[Eastmount](https://me.csdn.net/Eastmount)阅读数：9174所属专栏：[知识图谱、web数据挖掘及NLP](https://blog.csdn.net/column/details/eastmount-kgdmnlp.html)



这篇文章是学习了老曹的微信直播，感觉WordCloud对我的《Python数据挖掘课程》非常有帮助，作者学习后准备下次上课分享给我的学生，让他们结合词频分析来体会下词云。希望这篇基础文章对你有所帮助，同时自己也是词云的初学者，强烈推荐老曹的博客供大家学习。如果文章中存在不足或错误的地方，还请海涵~
老曹地址：[10行python代码的词云 - 半吊子全栈工匠](http://blog.csdn.net/wireless_com/article/details/60571394)
前文推荐：
[【Python数据挖掘课程】一.安装Python及爬虫入门介绍](http://blog.csdn.net/eastmount/article/details/52577215)
[【Python数据挖掘课程】二.Kmeans聚类数据分析及Anaconda介绍](http://blog.csdn.net/eastmount/article/details/52777308)
[【Python数据挖掘课程】三.Kmeans聚类代码实现、作业及优化](http://blog.csdn.net/eastmount/article/details/52793549)
[【Python数据挖掘课程】四.决策树DTC数据分析及鸢尾数据集分析](http://blog.csdn.net/eastmount/article/details/52820400)
[【Python数据挖掘课程】五.线性回归知识及预测糖尿病实例](http://blog.csdn.net/eastmount/article/details/52929765)
[【Python数据挖掘课程】六.Numpy、Pandas和Matplotlib包基础知识](http://blog.csdn.net/eastmount/article/details/53144633)
[【Python数据挖掘课程】七.PCA降维操作及subplot子图绘制](http://blog.csdn.net/eastmount/article/details/53285192)
[【Python数据挖掘课程】八.关联规则挖掘及Apriori实现购物推荐](http://blog.csdn.net/eastmount/article/details/53368440)
[【Python数据挖掘课程】九.回归模型LinearRegression简单分析氧化物数据](http://blog.csdn.net/eastmount/article/details/60468818)
[【python数据挖掘课程】十.Pandas、Matplotlib、PCA绘图实用代码补充](http://blog.csdn.net/eastmount/article/details/60675865)
[【python数据挖掘课程】十一.Pandas、Matplotlib结合SQL语句可视化分析](http://blog.csdn.net/eastmount/article/details/62489186)
[【python数据挖掘课程】十二.Pandas、Matplotlib结合SQL语句对比图分析](http://blog.csdn.net/eastmount/article/details/64127445)


## 一. 安装WordCloud
在使用WordCloud词云之前，需要使用pip安装相应的包。
**pip install WordCloud**
**pip install jieba**
其中WordCloud是词云，jieba是结巴分词工具。
jieba参考我的博客：[[python] 使用Jieba工具中文分词及文本聚类概念](http://blog.csdn.net/eastmount/article/details/50256163)
问题：在安装WordCloud过程中，你可能遇到的第一个错误如下。
error: Microsoft Visual C++ 9.0 is required. Get it from http://asa.ms/vcpython27

![](https://img-blog.csdn.net/20170321104142189)
解决方法也很简单，下载VCForPython27安装（Microsoft Visual C++ Compiler for Python 2.7）。但是在微软下载总是没响应。微软地址如下：
[https://www.microsoft.com/en-us/download/details.aspx?id=44266](https://www.microsoft.com/en-us/download/details.aspx?id=44266)
这是最大的问题，下面我自己提供一个CSDN的地址供大家下载。下载完成，可以进行安装响应的库函数。
资源地址：[http://download.csdn.net/detail/eastmount/9788218](http://download.csdn.net/detail/eastmount/9788218)
![](https://img-blog.csdn.net/20170321104506269)
![](https://img-blog.csdn.net/20170321104556050)

安装完成之后，可以正常运行代码啦。


## 二. 简单词云代码
下面这部分代码参考老曹的，希望对你有所帮助。
老曹说：什么是词云呢？词云又叫文字云，是对文本数据中出现频率较高的“关键词”在视觉上的突出呈现，形成关键词的渲染形成类似云一样的彩色图片，从而一眼就可以领略文本数据的主要表达意思。
代码如下：

```python
# -*- coding: utf-8 -*-
import jieba
import sys
import matplotlib.pyplot as plt
from wordcloud import WordCloud
#打开本体TXT文件
text = open('test.txt').read()
print type(text)
#结巴分词 cut_all=True 设置为全模式 
wordlist = jieba.cut(text, cut_all = True)
#使用空格连接 进行中文分词
wl_space_split = " ".join(wordlist)
print wl_space_split
#对分词后的文本生成词云
my_wordcloud = WordCloud().generate(wl_space_split)
#显示词云图
plt.imshow(my_wordcloud)
#是否显示x轴、y轴下标
plt.axis("off")
plt.show()
```
运行结果如下所示：
![](https://img-blog.csdn.net/20170321120936137)

这是中文编码问题，下面讲解解决方法。


## 三. 中文编码错误及解决
在WordCloud安装的目录下找到WordCloud.py文件，对源码进行修改。

![](https://img-blog.csdn.net/20170321121132184)
编辑wordcloud.py，找到FONT_PATH，将DroidSansMono.ttf修改成msyh.ttf。这个msyh.ttf表示微软雅黑中文字体。
![](https://img-blog.csdn.net/20170321121433564)
注意，此时运行代码还是报错，因为需要在同一个目录下放置msyh.ttf字体文件供程序调用，如下图所示，这是原来的字体DroidSansMono.ttf。
![](https://img-blog.csdn.net/20170321121621858)
此时的运行结果如下所示，这是分析CSDN多篇博客的主题，"阅读"和"评论"比较多。
![](https://img-blog.csdn.net/20170321121941219)

也可以采用下面的代码：
**wordcloud = WordCloud(font_path = 'MSYH.TTF').fit_words(word)**


## 四. 照片背景的词云代码
下面进一步深入，假设存在一个图 "sss3.png"，核心代码如下：

```python
# -*- coding: utf-8 -*-
from os import path
from scipy.misc import imread  
import jieba
import sys
import matplotlib.pyplot as plt
from wordcloud import WordCloud, STOPWORDS, ImageColorGenerator  
# 打开本体TXT文件
text = open('weixin.txt').read()
# 结巴分词 cut_all=True 设置为全模式 
wordlist = jieba.cut(text)     #cut_all = True
# 使用空格连接 进行中文分词
wl_space_split = " ".join(wordlist)
print wl_space_split
# 读取mask/color图片
d = path.dirname(__file__)
nana_coloring = imread(path.join(d, "sss3.png"))
# 对分词后的文本生成词云
my_wordcloud = WordCloud( background_color = 'white',      # 设置背景颜色
                            mask = nana_coloring,          # 设置背景图片
                            max_words = 2000,              # 设置最大现实的字数
                            stopwords = STOPWORDS,         # 设置停用词
                            max_font_size = 50,            # 设置字体最大值
                            random_state = 30,             # 设置有多少种随机生成状态，即有多少种配色方案
                            )
# generate word cloud 
my_wordcloud.generate(wl_space_split)
# create coloring from image  
image_colors = ImageColorGenerator(nana_coloring)
# recolor wordcloud and show  
my_wordcloud.recolor(color_func=image_colors)
plt.imshow(my_wordcloud)    # 显示词云图
plt.axis("off")             # 是否显示x轴、y轴下标
plt.show()
# save img  
my_wordcloud.to_file(path.join(d, "cloudimg.png"))
```
运行结果如下图所示，显示我加宝宝我两最近两月的聊天记录。
![](https://img-blog.csdn.net/20170321142645200)![](https://img-blog.csdn.net/20170321142917376)

一弦一柱思华年，一co一ding梦严贤。
希望文章对你有所帮助，尤其是结合数据库做数据分析的人。还是那句话，如果刚好需要这部分知识，你就会觉得非常有帮助，否则只是觉得好玩，这也是在线笔记的作用。如果文章中存在不足或错误的地方，还请海涵~
(By:Eastmount 2017-03-21 下午2点[http://blog.csdn.net/eastmount/](http://blog.csdn.net/eastmount/))



