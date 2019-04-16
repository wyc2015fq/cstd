# Python3网络爬虫(十)：这个帅哥、肌肉男横行的世界（爬取帅哥图） - Jack-Cui - CSDN博客





2017年05月21日 12:28:51[Jack-Cui](https://me.csdn.net/c406495762)阅读数：21418
所属专栏：[Python3网络爬虫入门](https://blog.csdn.net/column/details/15321.html)









**转载请注明作者和出处：**[http://blog.csdn.net/c406495762](http://blog.csdn.net/c406495762)
**运行平台：** Windows 
**Python版本：** Python3.x 
**IDE：** Sublime text3


- [前言](#1-前言)
- [预备知识](#2-预备知识)
- [实战](#3-实战)- [1 背景](#31-背景)
- [2 requests安装](#32-requests安装)
- [3 爬取单页目标连接](#33-爬取单页目标连接)
- [4 爬取多页目标连接](#34-爬取多页目标连接)
- [5 单张图片下载](#35-单张图片下载)
- [6 整体代码](#36-整体代码)

- [总结](#4-总结)




# 1 前言

    之前，感觉网上类似于《爬取妹子图》这样的爬虫教程有很多，所以我就没有写爬取图片的实战教程。 最近，有关注我爬虫教程的朋友说，希望我可以出个爬取图片的教程。那么，今天就谈一谈如何爬取图片吧。其实爬取图片相对于一些抓包分析的东西，还是简单很多的，只要找到图片的地址，我们就能够下载下来。别人的爬取图片教程都是爬取《妹子图》，有爬“煎蛋”网的，有爬“妹子图”网的，妹子图片那叫一个劲爆啊！可谓目不暇接。看的我身体也一天不如一天了。出于对广大朋友身体的考虑，今天咱就不爬妹子图了，咱爬《帅哥图》！（PS：我不会告诉你，我是想看看有没有美女程序员光临！）

# 2 预备知识

    为了也能够学习到新知识，本次爬虫教程使用requests第三方库，这个库可不是Python3内置的urllib.request库，而是一个强大的基于urllib3的第三方库。

    requests库的基础方法如下：





![](https://img-blog.csdn.net/20170521121032110?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvYzQwNjQ5NTc2Mg==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)





    官方中文教程地址：[http://docs.python-requests.org/zh_CN/latest/user/quickstart.html](http://docs.python-requests.org/zh_CN/latest/user/quickstart.html)

    因为官方给出的《快速上手》教程已经整理的很好了，并且本次教程使用的也是最简单的requests.get()，因此第三方库requests的使用方法，不再累述。详情请看官方中文教程，有urllib2基础的人，还是好上手的。

# 3 实战

## 3.1 背景

    爬取《帅啊》网的帅哥图片！

    URL : [http://www.shuaia.net/index.html](http://www.shuaia.net/index.html)

    先看一眼网站的样子：



![](https://img-blog.csdn.net/20170521121231569?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvYzQwNjQ5NTc2Mg==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)



## 3.2 requests安装

    在cmd中，使用如下指令安装第三方库requests：
`pip3 install requests`
    或者：
`easy_install requests`
## 3.3 爬取单页目标连接

    通过审查元素，我们不难发现，目标的地址存储在class属性为”item-img”的`<a>`标签的href属性中。这时候，有人可能会问为啥不用下面的`<img>`标签的src属性？因为这个图片是首页的浏览图片，根据这个地址保存下来的图片，太小了，并且不清清楚。秉承着热爱“高清无码”的精神，这种图片可不是我想要的。因此，先获取目标的地址，也就是我们点击图片之后，进入的网页地址，然后根据下一个网页，找到图片的地址。





![](https://img-blog.csdn.net/20170521121440034?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvYzQwNjQ5NTc2Mg==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)





    代码：

```
# -*- coding:UTF-8 -*-
from bs4 import BeautifulSoup
import requests

if __name__ == '__main__':
    url = 'http://www.shuaia.net/index.html'
    headers = {
            "User-Agent":"Mozilla/5.0 (Windows NT 6.1; Win64; x64) AppleWebKit/537.36 (KHTML, like Gecko) Chrome/58.0.3029.110 Safari/537.36"
    }
    req = requests.get(url = url,headers = headers)
    req.encoding = 'utf-8'
    html = req.text
    bf = BeautifulSoup(html, 'lxml')
    targets_url = bf.find_all(class_='item-img')
    list_url = []
    for each in targets_url:
        list_url.append(each.img.get('alt') + '=' + each.get('href'))
    print(list_url)
```

    我们将爬取的信息保存到list中，图片名字和图片地址使用”=”连接，运行结果：



![](https://img-blog.csdn.net/20170521121615323?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvYzQwNjQ5NTc2Mg==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)



## 3.4 爬取多页目标连接

    翻到第二页的时候，很容易就发现地址变为了:www.shuaia.net/index_2.html。第三页、第四页、第五页依此类推。





![](https://img-blog.csdn.net/20170521121727325?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvYzQwNjQ5NTc2Mg==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)





    代码：

```
# -*- coding:UTF-8 -*-
from bs4 import BeautifulSoup
import requests

if __name__ == '__main__':
    list_url = []
    for num in range(1,20):
        if num == 1:
            url = 'http://www.shuaia.net/index.html'
        else:
            url = 'http://www.shuaia.net/index_%d.html' % num
        headers = {
                "User-Agent":"Mozilla/5.0 (Windows NT 6.1; Win64; x64) AppleWebKit/537.36 (KHTML, like Gecko) Chrome/58.0.3029.110 Safari/537.36"
        }
        req = requests.get(url = url,headers = headers)
        req.encoding = 'utf-8'
        html = req.text
        bf = BeautifulSoup(html, 'lxml')
        targets_url = bf.find_all(class_='item-img')

        for each in targets_url:
            list_url.append(each.img.get('alt') + '=' + each.get('href'))
    print(list_url)
```

    我们少爬取一些，爬取前19页的目标连接：



![](https://img-blog.csdn.net/20170521121837177?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvYzQwNjQ5NTc2Mg==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)



## 3.5 单张图片下载

    进入目标地址，审查元素。可以看到，图片地址保存在了class属性为”wr-single-content-list “的div->div->img的src属性中。





![](https://img-blog.csdn.net/20170521121936116?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvYzQwNjQ5NTc2Mg==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)





    代码：

```
target_url = 'http://www.shuaia.net/rihanshuaige/2017-05-18/1294.html'
filename = '张根硕拍摄机车型男写真帅气十足' + '.jpg'
headers = {
    "User-Agent":"Mozilla/5.0 (Windows NT 6.1; Win64; x64) AppleWebKit/537.36 (KHTML, like Gecko) Chrome/58.0.3029.110 Safari/537.36"
    }
img_req = requests.get(url = target_url,headers = headers)
img_req.encoding = 'utf-8'
img_html = img_req.text
img_bf_1 = BeautifulSoup(img_html, 'lxml')
img_url = img_bf_1.find_all('div', class_='wr-single-content-list')
img_bf_2 = BeautifulSoup(str(img_url), 'lxml')
img_url = 'http://www.shuaia.net' + img_bf_2.div.img.get('src')
if 'images' not in os.listdir():
    os.makedirs('images')
urlretrieve(url = img_url,filename = 'images/' + filename)
print('下载完成！')
```

    我们将图片保存在程序文件所在目录的imgase目录下：



![](https://img-blog.csdn.net/20170521122107585?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvYzQwNjQ5NTc2Mg==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)




![](https://img-blog.csdn.net/20170521122141523?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvYzQwNjQ5NTc2Mg==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)



## 3.6 整体代码

    已经获取到了每张图片的连接，我们就可以下载了。整合下代码，先少下载一点，下载前2页的图片。

```
# -*- coding:UTF-8 -*-
from bs4 import BeautifulSoup
from urllib.request import urlretrieve
import requests
import os
import time

if __name__ == '__main__':
    list_url = []
    for num in range(1,3):
        if num == 1:
            url = 'http://www.shuaia.net/index.html'
        else:
            url = 'http://www.shuaia.net/index_%d.html' % num
        headers = {
                "User-Agent":"Mozilla/5.0 (Windows NT 6.1; Win64; x64) AppleWebKit/537.36 (KHTML, like Gecko) Chrome/58.0.3029.110 Safari/537.36"
        }
        req = requests.get(url = url,headers = headers)
        req.encoding = 'utf-8'
        html = req.text
        bf = BeautifulSoup(html, 'lxml')
        targets_url = bf.find_all(class_='item-img')

        for each in targets_url:
            list_url.append(each.img.get('alt') + '=' + each.get('href'))

    print('连接采集完成')

    for each_img in list_url:
        img_info = each_img.split('=')
        target_url = img_info[1]
        filename = img_info[0] + '.jpg'
        print('下载：' + filename)
        headers = {
            "User-Agent":"Mozilla/5.0 (Windows NT 6.1; Win64; x64) AppleWebKit/537.36 (KHTML, like Gecko) Chrome/58.0.3029.110 Safari/537.36"
        }
        img_req = requests.get(url = target_url,headers = headers)
        img_req.encoding = 'utf-8'
        img_html = img_req.text
        img_bf_1 = BeautifulSoup(img_html, 'lxml')
        img_url = img_bf_1.find_all('div', class_='wr-single-content-list')
        img_bf_2 = BeautifulSoup(str(img_url), 'lxml')
        img_url = 'http://www.shuaia.net' + img_bf_2.div.img.get('src')
        if 'images' not in os.listdir():
            os.makedirs('images')
        urlretrieve(url = img_url,filename = 'images/' + filename)
        time.sleep(1)

    print('下载完成！')
```

    运行结果如下：





![](https://img-blog.csdn.net/20170521122323428?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvYzQwNjQ5NTc2Mg==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)





    最终下载的图片：





![](https://img-blog.csdn.net/20170521122401947?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvYzQwNjQ5NTc2Mg==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)





# 4 总结

    图片是不是很帅？还算满意吧？

    这种爬取方法是比较简单的，速度慢。服务器有防爬虫程序，所以不能爬太快，每下载一个图片需要加个1秒延时，否则会被服务器断开连接。当然，解决办法还是有的，因为不是本文重点，以后有机会再细说。

    爬取图片的原理就是这样了，如果想爬取妹子图的可以去《煎蛋网》看看，包你满意。

**PS： 如果觉得本篇本章对您有所帮助，欢迎关注、评论、顶！**





