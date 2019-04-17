# 用Selenium+PhantomJS来抓取煎蛋网妹子图 - zhusongziye的博客 - CSDN博客





2018年09月01日 16:41:49[zhusongziye](https://me.csdn.net/zhusongziye)阅读数：230










那天有朋友找我一起抓妹子图片，像我这么正派的人，肯定是要抱着学习的目的……打开网页，哇！

话不多说，先看看网页吧！

![](https://image.uc.cn/o/wemedia/s/upload/2018/3d5f75c1f8151efe103b3755b24cdda5x550x284x28.jpeg;,3,jpegx;3,700x.jpg)

熟练的打开网页，找到妹子图，然后F12打开开发者工具，定位，ok，图片地址找到了，so easy~!，我们写入代码看看

![](https://image.uc.cn/o/wemedia/s/upload/2018/7e60aea22bf7662e840c24d2d400e3c8x548x194x15.jpeg;,3,jpegx;3,700x.jpg)

WTF~!

返回了个空列表？加了header也是这样，为啥呢，来打开源码看看

![](https://image.uc.cn/o/wemedia/s/upload/2018/1df2ae78bb9be95000b545132809b492x1214x101x37.jpeg;,3,jpegx;3,700x.jpg)

原来是js数据，这就麻烦了，难道先去学习下js语法，拿js文件，分析逻辑，找最终的url吗？

让我们用selenium+PhantomJS试试吧！

安装请自行百度（有些麻烦，需要下载各种浏览器的相关驱动文件，网上一堆教程）

我们先获取源码后找找刚才的标签看看效果！

![](https://image.uc.cn/o/wemedia/s/upload/2018/57367315a92cbd87cd8931ef383b1ee9x958x84x24.jpeg;,3,jpegx;3,700x.jpg)

吼吼~乖乖的出现了吧！

下面的流程就简单了，找到URL中翻页按钮，循环它，直到没有为止！然后写个获取图片并下载的函数，不到40行代码搞定！剩下的就是静静等待了！

完整代码和下载效果如下：

![](https://image.uc.cn/o/wemedia/s/upload/2018/bdbf7e9a381fee33ff8b46374a6b803dx545x532x34.jpeg;,3,jpegx;3,700x.jpg)



![](https://image.uc.cn/o/wemedia/s/upload/2018/0dcfbfd0f357ad0ef07b67bf0936d835x785x395x22.jpeg;,3,jpegx;3,700x.jpg)

![](https://image.uc.cn/o/wemedia/s/upload/2018/5ead276f5ccf40756984be8c0dc51223x1032x607x178.jpeg;,3,jpegx;3,700x.jpg)

源码：

```python
from lxml import etree
import requests,time,random
from selenium import webdriver

# 获取页面所有的图片url下载到本地
def down_img(url,num):
    i=1
    driver=webdriver.PhantomJS()
    # driver=webdriver.Chrome()
    driver.get(url)
    html=driver.page_source
    req = etree.HTML(html)
    img_url_list=req.xpath("//div[@class='text']/p/a/@href")
    for img_url in img_url_list:
        try:
            time.sleep(random.random())
            name=img_url.split('/')[-1]
            url_img='http://' + img_url[2:]
            data = requests.get(url_img).content
            with open(name,'wb') as f:
                f.write(data)
            print('成功下载第%d页的%d张图片' % num,i)
            i += 1
        except:
            pass
k=1
url = 'http://jiandan.net/ooxx'
#查找翻页地址并循环
while k:
    down_img(url,k)
    driver = webdriver.PhantomJS()
    # driver = webdriver.Chrome()
    driver.get(url)
    html =driver.page_source
    req = etree.HTML(html)
    page_url = "http://" + req.xpath("//a[@class='previous-comment-page']/@href")[0][2:]
    if page_url:
        url=page_url
        k+=1
else:
    print("出错了")
    exit()
```







