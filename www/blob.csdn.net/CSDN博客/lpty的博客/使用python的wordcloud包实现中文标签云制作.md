
# 使用python的wordcloud包实现中文标签云制作 - lpty的博客 - CSDN博客

2017年02月04日 17:29:24[lpty](https://me.csdn.net/sinat_33741547)阅读数：5343


一、前言
数据分析报告之中文标签云
在我的前一篇博客([python爬虫爬取拉勾网职业信息](http://blog.csdn.net/sinat_33741547/article/details/54847950))中，获得了一份格式化的职业信息数据，不清楚的可以看看我的上一篇博客。
python制作标签云的包有几个，我这里选用了wordcloud，直接pip install 即可。
二、实战
1、统计词频数
前面我们用爬虫已经抓取了一份格式化的数据，要制作标签云，要先把各个公司职位优势的词频数统计出来，代码如下：

```python
def wordCount(filename):
    '''
    简单计算词频的函数
    :param filename: 文件名
    :return: 词频
    '''
    wordCount = {}
    file = open(filename,'r')
    while True:
        line = file.readline()
        if line:
            wordlist = line.split(',[')
            if len(wordlist) == 1: continue
            wordlist = wordlist[1].split('],')[0].split(',')
            for word in wordlist:
                word = word.replace(' ', '').replace("'", '')
                if word in wordCount.keys():
                    wordCount[word] = wordCount[word]+1
                else:
                    wordCount[word] = 1
        else:
            break
    return [(k, wordCount[k]) for k in wordCount.keys()]
```
注意，上述代码只用于特定格式下，即爬虫抓取回来的职位信息中公司优势的词频统计2、生成标签云

```python
def generateCloud(filename,imagename,cloudname,fontname):
    '''
    生成标签云的函数
    '''
    coloring = imread(imagename)             # 读取背景图片
    wc = WordCloud(background_color="white", # 背景颜色max_words=2000,# 词云显示的最大词数
                   mask=coloring,            # 设置背景图片
                   stopwords=STOPWORDS,      # 停止词
                   font_path=fontname,       # 兼容中文字体
                   max_font_size=150)        # 字体最大值
    #计算好词频后使用generate_from_frequencies函数生成词云
    #txtFreq例子为[('词a', 100),('词b', 90),('词c', 80)]
    txtFreq = wordCount(filename)
    wc.generate_from_frequencies(txtFreq)
    # 生成图片
    plt.imshow(wc)
    plt.axis("off")
    # 绘制词云
    plt.figure()
    # 保存词云
    wc.to_file(cloudname)
```
这里需要注意的是，fontname这个参数，在ubuntu系统下，生成中文标签云默认字体可能会不兼容，所以要重新指定兼容的字体，直接在window系统下复制微软雅黑(msyh,tff)字体过来即可。3、运行

```python
if __name__ == '__main__':
    d = path.dirname(__file__)              # 获取当前文件路径
    fontname = path.join(d, 'msyh.ttf')     # 中文字体路径
    filename = path.join(d, '广州.txt')      # txt文件路径
    imagename = path.join(d, "circle.jpg")  # 背景图片路径
    cloudname = path.join(d, "cloud.png")   # 标签云路径
    generateCloud(filename, imagename, cloudname, fontname)
```
三、结果1、选用的背景图片--circle.jpg
![](https://img-blog.csdn.net/20170204174813891?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvc2luYXRfMzM3NDE1NDc=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)
2、生成的标签云--cloud.png
![](https://img-blog.csdn.net/20170204174828110?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvc2luYXRfMzM3NDE1NDc=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)
3、到这里就大工告成了，具体代码可以在我的github上找到：https://github.com/lpty

