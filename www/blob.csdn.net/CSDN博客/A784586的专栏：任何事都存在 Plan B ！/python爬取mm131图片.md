# python爬取mm131图片 - A784586的专栏：任何事都存在 Plan B ！ - CSDN博客





2017年06月08日 13:34:18[QuJack](https://me.csdn.net/A784586)阅读数：19037








python爬取mm131图片

python3



```python
#!/usr/bin/python
#encoding: utf-8
"""
http://www.mm131.com
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
	
#002   获得总页码数
def get_page_num(html,url_num):
    szurlre = re.compile(r'<a href=\'list_'+str(url_num)+'_(\d+).html\' class="page-en">末页')
    szresult = re.findall(szurlre, html)
    if len(szresult) == 0:
        page_num = 0
    else:
        page_num = int(szresult[0])
    print("pagenum:",page_num)
    return page_num
	
#003 获得相册
def get_ablum_list(base_url,html):
	#http://www.mm131.com/qipao/2288.html
    print(base_url)
    szurlre = re.compile(base_url+r'(\d+.html)')
    ablum_list = re.findall(szurlre, html);
    print("----------len-----------:",len(ablum_list))
    return ablum_list	
	
#004 获得单页的图片
def get_photo(html, photo_num):
    imgre = re.compile(r'(http://\S+.jpg)')
    imglist = re.findall(imgre, html)
    #print("len_imglist",len(imglist))
    for imgurl in imglist:
        try:
            socket.setdefaulttimeout(2)
            #urllib.request.urlretrieve(imgurl, unicode('.\\photo\\%s\%05d.jpg'%(dir, photo_num), "utf8"))
            store_path='.\\mm131\\' + '\\'+ md5(imgurl.encode("utf8")).hexdigest() + "." + 'jpg'
            #print("#######:",store_path)
            urllib.request.urlretrieve(imgurl, store_path)
            print("正在下载第%s张图片"%photo_num)
            photo_num = photo_num + 1
        except:
            continue
    return photo_num	
	
try:
    os.mkdir("mm131")
except:
    print ("目录已经存在，继续下载")
"""
#test
html=get_html("http://www.mm131.com/chemo/")
get_page_num(html,3)
"""
url_list=["http://www.mm131.com/qingchun/","http://www.mm131.com/xiaohua/","http://www.mm131.com/chemo/","http://www.mm131.com/qipao/","http://www.mm131.com/mingxing/","http://www.mm131.com/xinggan/"]
url_num=1
for baseurl in url_list:
	base_url=baseurl
	print(base_url)
	html=get_html(base_url)
	print(url_num)
	page_num=get_page_num(html,url_num)
	for i in range(1,page_num):
		try:
			if i!=1: 
				baseurl=baseurl+"list_"+str(url_num)+"_"+str(i)+".html"
				html=get_html(baseurl)
			else:
				pass
			ablumlist=get_ablum_list(base_url,html)
			for find_url in ablumlist:
				photo_html=get_html(base_url+find_url)	
				photo_num=0
				photo_num = get_photo(photo_html, photo_num)
		except:
			continue		
	url_num=url_num+1
```





