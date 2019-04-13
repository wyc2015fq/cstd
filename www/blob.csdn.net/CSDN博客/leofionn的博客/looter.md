
# looter - leofionn的博客 - CSDN博客


2018年08月02日 09:34:29[leofionn](https://me.csdn.net/qq_36142114)阅读数：384


如今，网上的爬虫教程可谓是泛滥成灾了，从urllib开始讲，最后才讲到requests和selenium这类高级库，实际上，根本就不必这么费心地去了解这么多无谓的东西的。只需记住爬虫总共就三大步骤：发起请求——解析数据——存储数据，这样就足以写出最基本的爬虫了。诸如像Scrapy这样的框架，可以说是集成了爬虫的一切，但是新人可能会用的不怎么顺手，看教程可能还会踩各种各样的坑，而且Scrapy本身体积也有点大。因此，本人决定亲手写一个轻量级的爬虫框架————[looter](https://github.com/alphardex/looter)，里面集成了调试和爬虫模板这两个核心功能，利用looter，你就能迅速地写出一个高效的爬虫。另外，本项目的函数文档也相当完整，如果有不明白的地方可以自行阅读源码（一般都是按Ctrl+左键或者F12）。
## 安装
`$ pip install looter`仅支持Python3.6及以上版本。
## 快速开始
让我们先来撸一个非常简单的图片爬虫：首先，用shell获取网站
`$ looter shell konachan.com/post`然后用2行代码就可以将图片抓取到本地
`>>> imgs = tree.cssselect('a.directlink')
>>> save_imgs(imgs)`或者只用1行也行:d
`>>> save_imgs(links(res, search='jpg'))`
## 工作流
如果你想迅速撸出一个爬虫，那么你可以用looter提供的模板来自动生成一个
`$ looter genspider <name> <tmpl> [--async]`在这行代码中，tmpl是模板，分为data和image两种模板。
async是一个备用的选项，它使得生成的爬虫核心用asyncio而非线程池。
在生成的模板中，你可以自定义domain和tasklist这两个变量。
什么是tasklist？实际上它就是你想要抓取的页面的所有链接。
以konachan.com为例，你可以使用列表推导式来创建自己的tasklist：
`domain = 'https://konachan.com'
tasklist = [f'{domain}/post?page={i}' for i in range(1, 9777)]`然后你就要定制你的crawl函数，这是爬虫的核心部分。
`def crawl(url):
    tree = lt.fetch(url)
    items = tree.cssselect('ul li')
    for item in items:
        data = dict()
        # data[...] = item.cssselect(...)
        pprint(data)`在大多数情况下，你所要抓取的内容是一个列表（也就是HTML中的ul或ol标签），可以用css选择器将它们保存为items变量。
然后，你只需使用for循环来迭代它们，并抽取你想要的数据，将它们存储到dict中。
但是，在你写完这个爬虫之前，最好用looter提供的shell来调试一下你的cssselect代码是否正确。
`>>> items = tree.cssselect('ul li')
>>> item = items[0]
>>> item.cssselect(anything you want to crawl)
# 注意代码的输出是否正确！`调试完成后，你的爬虫自然也就完成了。怎么样，是不是很简单:)
当然，本人也编写了好几个[爬虫例子](https://github.com/alphardex/looter/tree/master/looter/examples)，可供参考。
## 函数
looter为用户提供了很多实用的函数。
### view
在爬取页面前，你最好确认一下页面的渲染是否是你想要的
`>>> view(url)`
### save_imgs
当你获取了一堆图片链接时，用它可以直接将它们保存到本地
`>>> img_urls = [...]
>>> save_imgs(img_urls)`
### alexa_rank
可以获取网站的reach和popularity指数（人气度），此函数返回一个元组(url, reach_rank, popularity_rank)
`>>> alexa_rank(url)`
### links
获取网页的所有链接
`>>> links(res)                  # 获取所有链接
>>> links(res, absolute=True)   # 获取绝对链接
>>> links(res, search='text')   # 查找指定链接`同样地，你也可以用正则表达式来获取匹配的链接
`>>> re_links(res, r'regex_pattern')`
### save_as_json
将所得结果保存为json文件，支持按键值排序
`>>> total = [...]
>>> save_as_json(total, name='text', sort_by='key')`
### parse_robots
用于爬取网站robots.txt上的所有链接。这个在做全站爬虫或者递归式url爬虫时颇为有效
`>>> parse_robots(url)`
### login
有一些网站必须要先登录才能爬取，于是就有了login函数，本质其实就是建立session会话向服务器发送带有data的POST请求。
但是，每个网站的登录规则都各不相同，想要找到合适的postdata还是要费一番功夫的，而且更有甚者还要你构造param或header参数。
不过幸运的是在github上已经有人整理好了各大网站的模拟登录方法——[fuck-login](https://github.com/xchaoinfo/fuck-login)，本人很是佩服。
总之考验各位抓包的能力了，以下为模拟登录网易126邮箱（要求参数：postdata和param）
`>>> params = {'df': 'mail126_letter', 'from': 'web', 'funcid': 'loginone', 'iframe': '1', 'language': '-1', 'passtype': '1', 'product': 'mail126',
 'verifycookie': '-1', 'net': 'failed', 'style': '-1', 'race': '-2_-2_-2_db', 'uid': 'webscraping123@126.com', 'hid': '10010102'}
>>> postdata = {'username': 你的用户名, 'savelogin': '1', 'url2': 'http://mail.126.com/errorpage/error126.htm', 'password': 你的密码}
>>> url = "https://mail.126.com/entry/cgi/ntesdoor?"
>>> res, ses = login(url, postdata, params=params) # res为post请求后的页面，ses为请求会话
>>> index_url = re.findall(r'href = "(.*?)"', res.text)[0] # 在res中获取重定向主页的链接
>>> index = ses.get(index_url) # 用ses会话访问重定向链接，想确认成功的话print下即可`

