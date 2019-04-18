# Scrapy爬虫入门教程二 官方提供Demo - YZXnuaa的博客 - CSDN博客
2018年02月11日 15:39:10[YZXnuaa](https://me.csdn.net/YZXnuaa)阅读数：388
[Python版本管理：pyenv和pyenv-virtualenv](http://blog.csdn.net/inke88/article/details/59761696)
[Scrapy爬虫入门教程一 安装和基本使用](http://blog.csdn.net/inke88/article/details/60145707)
[Scrapy爬虫入门教程二 官方提供Demo](http://blog.csdn.net/inke88/article/details/60321729)
[Scrapy爬虫入门教程三 命令行工具介绍和示例](http://blog.csdn.net/inke88/article/details/60323856)
[Scrapy爬虫入门教程四 Spider（爬虫）](http://blog.csdn.net/Inke88/article/details/60573507)
[Scrapy爬虫入门教程五 Selectors（选择器）](http://blog.csdn.net/Inke88/article/details/60589170)
[Scrapy爬虫入门教程六 Items（项目）](http://blog.csdn.net/Inke88/article/details/60751098)
[Scrapy爬虫入门教程七 Item Loaders（项目加载器）](http://blog.csdn.net/inke88/article/details/60761853)
[Scrapy爬虫入门教程八 交互式 shell 方便调试](http://blog.csdn.net/inke88/article/details/60765273)
[Scrapy爬虫入门教程九 Item Pipeline（项目管道）](http://blog.csdn.net/inke88/article/details/60766985)
[Scrapy爬虫入门教程十 Feed exports（导出文件）](http://blog.csdn.net/inke88/article/details/60768602)
[Scrapy爬虫入门教程十一 Request和Response（请求和响应）](http://blog.csdn.net/inke88/article/details/61204180)
[Scrapy爬虫入门教程十二 Link Extractors（链接提取器）](http://blog.csdn.net/Inke88/article/details/61204579)
**开发环境：**
`Python 3.6.0 版本` （当前最新） 
`Scrapy 1.3.2 版本` （当前最新） 
[toc]
今天研究下官方给出的案例，大家可以多看看，多模仿模仿。
### 例子
最好的学习方法是使用示例，Scrapy也不例外。因此，有一个名为quotesbot的 Scrapy项目示例，请访问[https://github.com/scrapy/quotesbot](https://github.com/scrapy/quotesbot)，一个使用CSS选择器，另一个使用XPath表达式，此项目仅用于教育目的。
#### 提取的数据
提取的数据看起来像这个示例：
```bash
{
    'author': 'Douglas Adams',
    'text': '“I may not have gone where I intended to go, but I think I ...”',
    'tags': ['life', 'navigation']
}
```
- 1
- 2
- 3
- 4
- 5
#### 爬虫
此项目包含两个爬虫，您可以使用list 命令列出它们：
$ scrapy list 
toscrape-css 
toscrape-xpath
两个爬虫都从同一网站提取相同的数据，但toscrape-css 使用CSS选择器，而toscrape-xpath使用XPath表达式。
### 运行爬虫
您可以使用scrapy crawl命令运行爬虫，如： 
`$ scrapy crawl toscrape-css`
如果要将已抓取的数据保存到文件，可以传递-o选项： 
`$ scrapy crawl toscrape-css -o quotes.json`
