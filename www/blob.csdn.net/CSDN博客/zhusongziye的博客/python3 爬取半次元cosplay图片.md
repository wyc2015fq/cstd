# python3 爬取半次元cosplay图片 - zhusongziye的博客 - CSDN博客





2018年11月27日 20:32:48[zhusongziye](https://me.csdn.net/zhusongziye)阅读数：228








```
作者：叶耀荣
源自：https://zhuanlan.zhihu.com/p/42728565
```

不知道有没有小哥哥、小姐姐喜欢cosplay的，今天小编就分享一个关于爬取cosplay图片，emmmm，先来几张图活跃一下气氛！

![](https://img-blog.csdnimg.cn/20181124141321421.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3podXNvbmd6aXll,size_16,color_FFFFFF,t_70)





咳咳咳.....接下来咱们就进入正题！

首先，进入半次元，点击COS，热门推荐

![](https://img-blog.csdnimg.cn/20181124141359630.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3podXNvbmd6aXll,size_16,color_FFFFFF,t_70)

点击F12，可以看到开发者工具窗口

![](https://img-blog.csdnimg.cn/20181124141421692.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3podXNvbmd6aXll,size_16,color_FFFFFF,t_70)



我们以第一张COS照片的代码进行分析....额...第二张好看，还是从第二张开始吧。

![](https://img-blog.csdnimg.cn/2018112414143492.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3podXNvbmd6aXll,size_16,color_FFFFFF,t_70)

红框里面就是这张图片的html代码，然后我们以正常访问方式点击图片进入网页,可以看到这张图片分辨率更高。

我们与之前HTML代码的图片的URL进行比较

![](https://img-blog.csdnimg.cn/20181124141456703.png)

可以看见，2X3是我们第一次进网页时第一张COS照片得到的URL，w650是进入COS照片详细页面后得到的URL，发现他们的区别是URL代码中的最后一段。

其他COS照片以此类推

我们在第一次进入的页面继续往下滑，发现该网页滚到结尾时自动更新，可以确定网页使用了AJAX技术，我们回到置顶刷新界面，等网页加载好后按F12打开开发者工具，操作如图

![](https://img-blog.csdnimg.cn/20181124141527335.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3podXNvbmd6aXll,size_16,color_FFFFFF,t_70)

点击XHR

我们继续往下划，等到页面更新时发现新条码 点击条目

![](https://img-blog.csdnimg.cn/20181124141542857.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3podXNvbmd6aXll,size_16,color_FFFFFF,t_70)

在Headers页面往下滑，看见X-Reauested-With:XMLHttpRequest，表明是AJAX请求，找到Query String Parameters，这就是AJAX请求的数据，在Preview中可以看见AJAX返回的数据。

继续往下划，让网页数据更新，发现Network中又新出现的几个新条目

我们比较Query String Parameters的AJAX请求数据，发现
- 
grid_type: 

	flow

- 
sort: 

	hot


3.tag_id: 

399

这三条数据和其他Network条目是相同的，但是since不相同，和其他条目对比http://25853.xxx其中xxx这三个数字是不规律的，其中since中25853小数点后的数据为565、523、483、428（以实际情况为准，仅供参考），意味着我们在接下来图片爬取中since数据要手动输入

```
import re
import requests
from pyquery import PyQuery as pq
import time
from urllib.parse import urlencode

Filepath='cosplay' #目录文件名 可以修改 注意不要含有"/"

def get_html(url):
    proxies = {"https": "https://183.129.207.73:14823", "https": "https://114.215.95.188:3128", }
    headers = {'User-Agent': 'Mozilla/5.0 (Windows NT 10.0; Win64; x64) AppleWebKit/537.36 (KHTML, like Gecko) Chrome/68.0.3440.106 Safari/537.36'}
    getweb = requests.get(str(url),headers=headers,proxies=proxies)
    try:
        return getweb.text
    except Exception as e:
        print(e)
    except IOError as e1:
        print(e1)

def DownloadFileWithFilename(url,filename,path):
    import requests
    import os
    import re
    try:
        if not os.path.exists(path):
            os.mkdir(path)
        if not os.path.exists(path):
            r = requests.get(url)
        r = requests.get(url)
        with open(str(path) + "/"+str(filename), "wb") as code:
            code.write(r.content)
            print('Downloaded!',str(path) + "/"+str(filename))
    except IOError as e:
        print("Download Failed!")
        print(e)
    except Exception as e1:
        print(e1)
        print("Download Failed!")


def getStaticHtmlImage(): #获取没有AJAX更新时网页的COSPLAY图片
    global Filepath
    web_static_state='https://bcy.net/coser'
    doc = pq(web_static_state)
    image = doc('li.js-smallCards._box a.db.posr.ovf img.cardImage').items()

    for i in image:  # 爬取ajax网页数据
        i = str(i.attr('src')).rstrip('/2X3')  # 这里的i是把获取的URL最后一段/2x3去除
        filename = str(re.search('[^/]+(?!.*/)', i).group(0))  # filename是URL的最后一段:xxx.jpg
        i2 = i + str('/w650')  # i2是高清图片URL:xxxxx/w650
        DownloadFileWithFilename(i2, filename, Filepath)
        time.sleep(3) #休眠三秒 防止封IP

def getDynamicHtmlImage(since1): #获取ajax更新数据的COSPLAY图片
    global Filepath
    ajax_get_data = {
        'since':since1,
        'grid_type':'flow',
        'sort':'hot',
        'tag_id':'399',
    }

    proxies = {"https": "https://183.129.207.73:14823", "https": "https://114.215.95.188:3128", }
    headers = {
        'User-Agent': 'Mozilla/5.0 (Windows NT 10.0; Win64; x64) AppleWebKit/537.36 (KHTML, like Gecko) Chrome/68.0.3440.106 Safari/537.36'}

    web_dynamic = requests.get('https://bcy.net/circle/timeline/showtag?'+urlencode(ajax_get_data),headers=headers, proxies=proxies).text
    doc = pq(web_dynamic)
    image = doc('li.js-smallCards._box a.db.posr.ovf img.cardImage').items()

    for i in image:  # 爬取ajax网页数据
        i = str(i.attr('src')).rstrip('/2X3')  # 这里的i是把获取的URL最后一段/2x3去除
        filename = str(re.search('[^/]+(?!.*/)', i).group(0))  # filename是URL的最后一段:xxx.jpg
        i2 = i + str('/w650')  # i2是高清图片URL:xxxxx/w650
        DownloadFileWithFilename(i2, filename, Filepath)
        time.sleep(3) #休眠三秒 防止封IP

getStaticHtmlImage()

list_since = ['25861.565','25861.523','25861.483','25861.428'] #ajax请求的since

for i in list_since:
    print(i)
    getDynamicHtmlImage(i)
```



爬取下来的小姐姐

![](https://img-blog.csdnimg.cn/20181124141603715.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3podXNvbmd6aXll,size_16,color_FFFFFF,t_70)



很激动......



