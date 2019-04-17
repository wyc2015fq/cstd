# python爬取5442网站图片 - A784586的专栏：任何事都存在 Plan B ！ - CSDN博客





2017年06月08日 13:32:12[QuJack](https://me.csdn.net/A784586)阅读数：851标签：[python爬虫																[爬虫图片																[爬虫](https://so.csdn.net/so/search/s.do?q=爬虫&t=blog)
个人分类：[python](https://blog.csdn.net/A784586/article/category/6961704)





**    python爬取5442网站图片**





python3环境下：






```python
#!/usr/bin/python
#encoding: utf-8
"""
本次爬虫使用的是python3的版本，并且本次的目标网站并没有采用js来加载图片，所以没有涉及对js脚本的解析，都是通过来分析html文件通过正则来一步步提取图片网址，然后存起来。
首先这个网站有很多分类，到美女图这个子网页，可以发现有很多页，同时每页有多个相册，每个相册点进去就会有多个页，每页有多张照片
流程大概是这样
找到所有页数
----遍历所有的页数
----遍历当前页的所有相册(给每个相册建立一个目录)
----遍历当前相册的所有图片(遍历此相册的所有页(遍历当前页的所有照片并找到图片的url))
----获得图片url就存起来
"""
import urllib.request
import re
import os
import time
import socket
from hashlib import md5
#001获得整个页面
def get_html(url):
    socket.setdefaulttimeout(10)
    papg = urllib.request.urlopen(url)
    html = papg.read()
    html = html.decode("gbk")
    #html = unicode(html, "gbk").encode("utf8")
    return html

def get_img(html):
    imgre = re.compile(r'<img src="(.*?)"')
    imglist = re.findall(imgre, html)
    for imgurl in imglist:
        #print (imgurl)
        global x
        urllib.request.urlretrieve(imgurl, '.\\photo\%05d.jpg'%x)
        x += 1
        print("正在下载第%s张图片"%x)

def get_tag_list(html):
    szurlre = re.compile(r'<a href="(http://www.5442.com/tag/.*?.html)" class')
    tag_list = re.findall(szurlre, html)
    return tag_list
#002   获得总页码数
def get_page_num(html):
    szurlre = re.compile(r'(\d+).html\'>末页')
    szresult = re.findall(szurlre, html)
    if len(szresult) == 0:
        page_num = 0
    else:
        page_num = int(szresult[0])
    #print (page_num)
    return page_num
#004 获取相册的页数
def get_page_num2(html):
    szurlre = re.compile(r'共(\d+)页')
    szresult = re.findall(szurlre, html)
    if len(szresult) == 0:
        page_num = 0
    else:
        page_num = int(szresult[0])
    #print (page_num)
    return page_num

#003 获得单页的相册
def get_ablum_list(html):
	#"http://www.5442.com/meinv/20170602/44873.html" title="白皙丰满少女画室写真 甜美少女白嫩巨乳诱人美照" target="_blank">白皙丰满少女画室写</a>#
    #szurlre = re.compile(r'(http://www.5442.com/meinv/2\d+/\d+.html)" target=')
    szurlre = re.compile(r'(http://www.5442.com/meinv/2\d+/\d+.html)')
    ablum_list = re.findall(szurlre, html);
    #print("----------len-----------:",len(ablum_list))
    return ablum_list
	
#获得相册的名称
def get_ablum_name(html):
    szurlre = re.compile(r'<title>(\S+)</title>')
    ablum_name = re.findall(szurlre, html)
    return ablum_name[0]

#005  获得单页的图片
def get_photo(html, dir, photo_num):
    imgre = re.compile(r'点击图片进入下一页\' ><img src=\'(http://\S+.jpg)\' alt=')
    imglist = re.findall(imgre, html)
    #print("len_imglist",len(imglist))
    for imgurl in imglist:
        try:
            socket.setdefaulttimeout(2)
            #urllib.request.urlretrieve(imgurl, unicode('.\\photo\\%s\%05d.jpg'%(dir, photo_num), "utf8"))
            store_path='.\\photo\\' + '\\'+ md5(imgurl.encode("utf8")).hexdigest() + "." + 'jpg'
            #print("#######:",store_path)
            urllib.request.urlretrieve(imgurl, store_path)
            print("正在下载第%s张图片"%photo_num)
            photo_num = photo_num + 1
        except:
            continue
    return photo_num

url = "http://www.5442.com/meinv/"
baseurl = "http://www.5442.com"
html = get_html(url)
page_num = get_page_num(html)
print ("一共有%s页"%page_num)
ablum_num = 0
try:
    os.mkdir("photo")
except:
    print ("目录已经存在，继续下载")
#遍历所有的页
for i in range(1, page_num):
    if i != 1:
        url = "http://www.5442.com/meinv/list_1_%s.html"%i
        try:
            html = get_html(url)
        except:
            continue
    ablum_list = get_ablum_list(html)
    #遍历当前页的所有相册
    for ablum_url in ablum_list:
        ablum_num = ablum_num + 1
        try:
            photo_html = get_html(ablum_url)
        except:
            continue
        url_part = ablum_url[0:-5]
        photo_page_num = get_page_num2(photo_html)
        #获取相册名有点问题，直接以数字来创建更加方便，便于分
        #ablum_name = get_ablum_name(photo_html)
        ablum_name = "picture" + "%05d" % ablum_num
        print (ablum_name)
        photo_num = 0
        #创建相册对应的目录
        ui_ablum_name = ablum_name#unicode(ablum_name, "utf8")
        #try:
            #os.mkdir(".\\photo\\"+ui_ablum_name)
        #except:
        ##   continue
        for i in range(1, photo_page_num):
            if i != 1:
                ablum_url = url_part + "_%d"%i + ".html"
                try:
                    photo_html = get_html(ablum_url)
                    #print("###获得photo地址###")
                except:
                    continue
            #进行存储操作 包含照片的网页，存储相册名称，存储第几张
            photo_num = get_photo(photo_html, ablum_name, photo_num)
```](https://so.csdn.net/so/search/s.do?q=爬虫图片&t=blog)](https://so.csdn.net/so/search/s.do?q=python爬虫&t=blog)




