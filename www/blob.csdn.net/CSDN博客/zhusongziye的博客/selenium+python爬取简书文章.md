# selenium+python爬取简书文章 - zhusongziye的博客 - CSDN博客





2018年04月01日 18:33:03[zhusongziye](https://me.csdn.net/zhusongziye)阅读数：533








## 页面加载逻辑

当你兴致勃勃地从网上学习了基本的爬虫知识后就像找个目标实践下，拥有大量文章的简书包含了大量的有价值信息，所以自然成为了你的选择目标，如果你尝试之后会发现并没有想象的那么简单，因为里面包含了很多js相关的数据传输。 让我先使用传统的爬虫演示一下吧: >

> 
打开简书首页，似乎没有什么特别的


![](https://img-blog.csdn.net/2018040118244252?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3podXNvbmd6aXll/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)

jianshu首页

> 
打开`chrome`的开发者模式，发现文章的标题，`href`都在`a`标签里，似乎也没有什么不一样的


![](https://img-blog.csdn.net/2018040118245437?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3podXNvbmd6aXll/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)

a.png

> 
接下来就是寻找页面上所有的`a`标签，但是且慢如果你仔细观察就会发现滑轮滚到一半的时候页面就会加载更多，这样的步骤会重复三次知道底部出现`阅读更多`的按钮


![](https://img-blog.csdn.net/20180401182507722?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3podXNvbmd6aXll/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)

滑轮

> 
不仅如此底部的阅读更多的`href`并没有告诉我们加载剩下的页面信息，唯一的办法是`不断点击阅读更多这个按钮`


![](https://img-blog.csdn.net/20180401182535352?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3podXNvbmd6aXll/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)

load_more.png

什么，将滑轮重复三次滑倒页面的中央并且不断点击按钮这种操作`http`请求可做不到，这更像是js操作？没错，简书的文章并不是常规的http请求，我们不能根据不同url不断重定向，而是页面的一些动作来加载页面信息。

![](https://img-blog.csdn.net/20180401182600393?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3podXNvbmd6aXll/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)

# selenium介绍

selenium是一个web自动化测试工具，支持很多种语言，我们在这里可以使用python的selenium做爬虫使用，爬取简书的过程中，它的工作原理是不断注入js代码，让页面源源不断的加载，最后再提取所有的`a`标签。首先你得在python中下载selenium包
`>>> pip3 install selenium`
## chromedriver

selenium必须搭载浏览器使用，这里我使用的是chromedriver，Chrome的开源测试版，它可以使用headless模式不需要显示前段来访问网页，算是最大特性了。

## python中操作

在写代码之前一定要把chromedriver同一文件夹内，因为我们需要引用PATH，这样方便点。首先我们的第一个任务是刷出`加载更多`的按钮，需要做3次将滑轮重复三次滑倒页面的中央，这里方便起见我滑到了底部
`from selenium import webdriver`
```
import time
browser = webdriver.Chrome("./chromedriver")
browser.get("https://www.jianshu.com/")for i in range(3):
    browser.execute_script("window.scrollTo(0, document.body.scrollHeight);") // execute_script是插入js代码的
    time.sleep(2) //加载需要时间，2秒比较合理
```

看看效果

![](https://img-blog.csdn.net/20180401182625151?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3podXNvbmd6aXll/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)

刷出了按钮

接下来就是不断点击按钮加载页面，继续加入刚才的py文件之中

```
for j in range(10):  //这里我模拟10次点击    
    try: 
        button = browser.execute_script("var a = document.getElementsByClassName('load-more'); a[0].click();")
        time.sleep(2)    except:        pass'''
 上面的js代码说明一下
 var a = document.getElementsByClassName('load-more');选择load-more这个元素
 a[0].click(); 因为a是一个集合，索引0然后执行click()函数
'''
```

这个我就不贴图了，成功之后就是不断地加载页面 ，知道循环完了为止，接下来的工作就简单很多了，就是寻找`a`标签，`get`其中的`text`和`href`属性,这里我直接把它们写在了txt文件之中.
`titles = browser.find_elements_by_class_name("title")with open("article_jianshu.txt", "w", encoding="utf-8") as f:``    for t in titles:``        try:``            f.write(t.text + " " + t.get_attribute("href"))``            f.write("\n")        ``        except TypeError:            ``            pass`
最终结果

![](https://img-blog.csdn.net/20180401182944528?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3podXNvbmd6aXll/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)

简书文章

## headless模式

不断加载页面肯定也很烦人，所以我们测试成功之后并不想把浏览器显示出来，这需要加上headless模式

```
options = webdriver.ChromeOptions()
options.add_argument('headless')
browser = webdriver.Chrome("./chromedriver", chrome_options=options) //把上面的browser加入chrome_options参数
```

## 总结

当我们没办法使用正常的http请求爬取时，可以使用selenium操纵浏览器来抓取我们想要的内容，这样有利有弊，比如
- 
优点

- 
可以暴力爬虫

- 
简书并不需要cookie才能查看文章，不需要费劲心思找代理，或者说我们可以无限抓取并且不会被ban

- 
首页应该为ajax传输，不需要额外的http请求


缺点
- 
爬取速度太满，想象我们的程序，点击一次需要等待2秒那么点击600次需要1200秒, 20分钟...


## 附加

这是所有完整的代码

```
from selenium import webdriverimport time

options = webdriver.ChromeOptions()
options.add_argument('headless')
browser = webdriver.Chrome("./chromedriver", chrome_options=options)

browser.get("https://www.jianshu.com/")for i in range(3):
    browser.execute_script("window.scrollTo(0, document.body.scrollHeight);")
    time.sleep(2)# print(browser)for j in range(10):    try:
        button = browser.execute_script("var a = document.getElementsByClassName('load-more'); a[0].click();")
        time.sleep(2)    except:        pass#titles = browser.find_elements_by_class_name("title")with open("article_jianshu.txt", "w", encoding="utf-8") as f:    for t in titles:        try:
            f.write(t.text + " " + t.get_attribute("href"))
            f.write("\n")        except TypeError:            pass
```

```

```

如果你喜欢本篇文章的话， 快点击关注，喜欢，打赏我吧  



