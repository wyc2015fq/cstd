# Python爬虫万金油，使用工具goose快速提取网页内容 - zhusongziye的博客 - CSDN博客





2018年10月18日 21:03:05[zhusongziye](https://me.csdn.net/zhusongziye)阅读数：308








爬虫抓取数据有两个头疼的点，写过爬虫的小伙伴们一定都深有体会：
- 
网站的**防抓取**机制。你要尽可能将自己伪装成“一个人”，骗过对方的服务器反爬验证。

- 
网站的**内容提取**。每个网站都需要你做不同的处理，而且网站一旦改版，你的代码也得跟着更新。


第一点没什么捷径可走，套路见得多了，也就有经验了。关于第二点，今天咱们就来介绍一个小工具，在某些需求场景下，或许可以给你省不少事。

**Goose**

**Goose** 是一个**文章内容提取器**，可以从任意资讯文章类的网页中提取**文章主体**，并提取**标题、标签、摘要、图片、视频**等信息，且**支持中文**网页。它最初是由 Gravity.com 用 Java 编写的。python-goose 是用 Python 重写的版本。

有了这个库，你从网上爬下来的网页可以直接获取正文内容，无需再用 bs4 或正则表达式一个个去处理文本。

项目地址：

（py2）

https://github.com/grangier/python-goose

（py3）

https://github.com/goose3/goose3

**安装**

网上大多数教程提到的 **python-goose** 项目目前只支持到 python 2.7。可以通过 pip 安装：
`pip install goose-extractor`
或者安装官网上的方法从源代码安装：

```
mkvirtualenv --no-site-packages goose
git clone https://github.com/grangier/python-goose.git
cd python-goose
pip install -r requirements.txt
python setup.py install
```

我找到一个 python 3 的版本 **goose3**：
`pip install goose3`
经过我一些简单的测试，未发现两个版本在结果上有太大的差异。

**快速上手**

这里使用 goose3，而 python-goose 只要把其中的 goose3 改成 goose 即可，接口都是一样的。以一篇文章 [如何用Python抓抖音上的小姐姐](http://mp.weixin.qq.com/s?__biz=MjM5MDEyMDk4Mw==&mid=2650167530&idx=1&sn=cf3740cedfe654b03e62b4162339d016&chksm=be4b5592893cdc84356139a95011d1876947d4554f047b72e539bd3a1a800f36dccfbd8b2c9a&scene=21#wechat_redirect) 为抓取目标来做个演示。

```
from goose3 import Goose
from goose3.text import StopWordsChinese
# 初始化，设置中文分词
g = Goose({'stopwords_class': StopWordsChinese})
# 文章地址
url = 'https://mp.weixin.qq.com/s/zflbcF5PS06QC5YJXpiviQ'
# 获取文章内容
article = g.extract(url=url)
# 标题
print('标题：', article.title)
# 显示正文
print(article.cleaned_text)
```

输出：

![](https://img-blog.csdn.net/20181018205515324?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3podXNvbmd6aXll/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)

除了标题 **title** 和正文 **cleaned_text** 外，还可以获取一些额外的信息，比如：
- 
**meta_description**：摘要

- 
**meta_keywords**：关键词

- 
**tags**：标签

- 
**top_image**：主要图片

- 
**infos**：包含所有信息的 dict

- 
**raw_html**：原始 HTML 文本


如有有些网站限制了程序抓取，也可以根据需要添加** user-agent **信息：

```
g = Goose({'browser_user_agent': 'Version/5.1.2 Safari/534.52.7'})
```

如果是 goose3，因为使用了 **requests** 库作为请求模块，因此还可以以相似方式配置**headers、proxies** 等属性。

在上述示例中使用到的 `StopWordsChinese` 为中文分词器，可一定程度上提高中文文章的识别准确率，但更耗时。

**其他说明**

1.

Goose 虽然方便，但并不能保证每个网站都能精确获取，因此**适合大规模文章的采集**，如热点追踪、舆情分析等。它只能从概率上保证大多数网站可以相对准确地抓取。我经过一些尝试后发现，抓取英文网站优于中文网站，主流网站优于小众网站，文本的提取优于图片的提取。

2.

从项目中的** requirements.txt** 文件可以看出，goose 中使用到了** Pillow、lxml、cssselect、jieba、beautifulsoup、nltk**，goose3 还用到了 **requests。**

3.

如果你是使用基于 python2 的 goose，有可能会遇到**编码**上的问题（尤其是 windows 上）。

4.

除了 goose 外，还有其他的正文提取库可以尝试，比如** python-boilerpipe、python-readability** 等。

**实例**

最后，我们来用 goose3 写小一段代码，自动抓取 **爱范儿、雷锋网、DoNews **上的新闻文章：

```
from goose3 import Goose
from goose3.text import StopWordsChinese
from bs4 import BeautifulSoup

g = Goose({'stopwords_class': StopWordsChinese})
urls = [
    'https://www.ifanr.com/',
    'https://www.leiphone.com/',
    'http://www.donews.com/'
]
url_articles = []
for url in urls:
    page = g.extract(url=url)
    soup = BeautifulSoup(page.raw_html, 'lxml')
    links = soup.find_all('a')
    for l in links:
        link = l.get('href')
        if link and link.startswith('http') and any(c.isdigit() for c in link if c) and link not in url_articles:
            url_articles.append(link)
            print(link)

for url in url_articles:
    try:
        article = g.extract(url=url)
        content = article.cleaned_text
        if len(content) > 200:
            title = article.title
            print(title)
            with open('homework/goose/' + title + '.txt', 'w') as f:
                f.write(content)
    except:
        pass
```

这段程序所做的事情就是：
- 
抓取网站首页

- 
从页面上提取地址中带有数字的链接

- 
抓取这些链接，提取正文。如果结果超过 200 个字，就保存成文件


效果：

![](https://img-blog.csdn.net/2018101820564563?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3podXNvbmd6aXll/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)

在此基础上，你可以继续改进这个程序，让它不停地去寻找新的地址并抓取文章，并对获取到的文章进行词频统计、生成词云等后续操作。



