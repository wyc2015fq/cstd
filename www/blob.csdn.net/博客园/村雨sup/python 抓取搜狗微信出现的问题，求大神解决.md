# python 抓取搜狗微信出现的问题，求大神解决 - 村雨sup - 博客园








[博客园](https://www.cnblogs.com/)[首页](https://www.cnblogs.com/cunyusup/)[新随笔](https://i.cnblogs.com/EditPosts.aspx?opt=1)[联系](https://msg.cnblogs.com/send/%E6%9D%91%E9%9B%A8sup)[管理](https://i.cnblogs.com/)[订阅](https://www.cnblogs.com/cunyusup/rss)![订阅](https://www.cnblogs.com/images/xml.gif)





爬取到的data不是想要获取文章页面的源码，而是跳转到验证码的页面的源码。我网上查了一下是由于访问过于频繁导致的，我也加了time.sleep和改了请求头但还是显示不了，求大神支招，除了识别验证码的方式还能怎么做？？

```
import re
import urllib.request
import time
import urllib.error

headers = {'User-Agent','Mozilla/5.0 (Windows NT 6.1; WOW64; rv:23.0) Gecko/20100101 Firefox/23.0'}
opener = urllib.request.build_opener()
opener.addheaders = [headers]
#设置一个列表listurl储存文章网址列表
listurl = []
#函数：使用代理IP
def use_proxy(proxy_addr,url):
    try:
        import urllib.request
        proxy = urllib.request.ProxyHandler({'http':proxy_addr})
        opener = urllib.request.build_opener(proxy,urllib.request.HTTPHandler)
        urllib.request.install_opener(opener)
        data = urllib.request.urlopen(url).read().decode('utf-8')
        return data
    except urllib.error.URLError as e:
        if hasattr(e,"code"):
            print(e.code)
        if hasattr(e,"e.reason"):
            print(e.reason)
        time.sleep(10)
    except Exception as e:
        print("expection:"+str(e))
        time.sleep(1)

#函数：获取所有文章的连接
def getlisturl(key,pagestart,pageend,proxy):
    try:
        #page = pagestart
        #编码关键词
        keycode = urllib.request.quote(key)
        #编码"&page"
        #pagecode = urllib.request.quote("&page")
        #循环爬取各页的文章链接
        for page in range(pagestart,pageend+1,1):
            #每次循环构建各页的url
            url = 'http://weixin.sogou.com/weixin?&type=2&ie=utf8&query='+keycode+"&&page="+str(page)
            #用换IP函数获得data
            data1 = use_proxy(proxy,url)
            time.sleep(1)
            #匹配的正则表达式
            pattern1 = '<div class="txt-box">.*?(http://.*?)"'
            listurl.append(re.compile(pattern1,re.S).findall(data1))
        print("共获取到"+str(len(listurl))+"页")#便于调试
        return listurl
    except urllib.error.URLError as e:
        if hasattr(e,"code"):
            print(e.code)
        if hasattr(e,"reason"):
            print(e.reason)
        #若为URLerror异常，延迟10s进行
        time.sleep(10)
    except Exception as e:
        print("exception:"+str(e))
        #若为Exception异常，延迟1s进行
        time.sleep(1)

#函数：通过链接获取对应的内容
def getcontent(listurl,proxy):
    i = 0
    #设置本地文件中的开始html编码
    html1 = '''<!DOCTYPE html PUBLIC "-//W3C//DTD XHTML 1.0 Transitional//EN" "http://www.w3.org/TR/xhtm11/DTD/xhtml1-transitional.dtd">
<html xmlns="http://www.w3.org/1999/xhtml"
<head>
<meta http-equiv="Content-Type" content="text/html; charset=utf-8" /> 
<title>微信文章页面</title>
</head>
<body>'''
    with open("D:/WEB/1.html","wb")as f:
        f.write(html1.encode("utf-8"))
    #再次以追加写入的方式打开文件，以写入对应文章内容
    with open("D:/WEB/1.html","ab")as ff:
        #此时listurl为二维列表，形如listurl[][]，第一维存储信息跟第几页相关，第二维存的跟该页第几个文章链接相关
        for i in range(0,len(listurl)):
            for j in range(0,len(listurl[i])):
                try:
                    url = listurl[i][j]
                    #处理真实url，亦可观察对应网址的关系自行分析，采集网址比真实网址多了一串amp
                    url = url.replace("amp;","")
                    #使用代理去爬取对应网址的内容
                    data = use_proxy(proxy,url)
                    #文章标题正则表达式
                    titlepat = "<title>(.*?)</title>"
                    #文章内容正则表达式
                    contentpat = 'id="js_content">(.*?)id="js_sg_bar"'
                    #找到标题并赋给列表title
                    title = re.compile(titlepat).findall(data)
                    #找到内容并赋给列表content
                    content = re.compile(contentpat,re.S).findall(data)
                    #初始化标题与内容
                    thistitle = "此次没有获取到"
                    thiscontent= "此次没有获取到"
                    #如果标题列表不为空，说明找到了标题，取列表第0个元素，即此次标题赋给变量thistitle
                    if (title!=[]):
                        thistitle = title[0]
                    if (content!=[]):
                        thiscontent = content[0]
                    #将标题与内容汇总赋给变量dataall
                    dataall = '<p>标题为:'+thistitle+'</p><p>内容为：'+thiscontent+'</p><br>'
                    #将文章标题与内容写入对应文件
                    ff.write(dataall.encode("utf-8"))
                    print("第"+str(i)+"个网页第"+str(j)+"次处理")#便于调试
                except urllib.error.URLError as e:
                    if hasattr(e,"code"):
                        print(e.code)
                    if hasattr(e,"reason"):
                        print(e.reason)
                    time.sleep(10)
                except Exception as e:
                    print("exception:"+str(e))
                    time.sleep(1)
    html2 = '''</body>
</html>
    '''
    with open("D:/WEB/1.html","ab")as f:
        f.write(html2.encode("utf-8"))

#设置关键词
key = "物联网"
#设置代理服务器
proxy = "122.72.32.73:80"
#可以为getlisturl()与getcontent设置不同的代理服务器，此处没有启用该项设置
proxy2 = ""
#启示页
pagesatrt = 1
#爬取到哪页
pageend = 2
listurl = getlisturl(key,pagesatrt,pageend,proxy)
getcontent(listurl,proxy)
```













