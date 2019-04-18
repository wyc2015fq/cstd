# ImageNet　数据集下载 - YZXnuaa的博客 - CSDN博客
2018年05月16日 13:21:19[YZXnuaa](https://me.csdn.net/YZXnuaa)阅读数：14354
一、获取ｕｒｌｓ
登录官网, 　www.image-net.org,　SEARCH自己需要的图片种类，　以搜索插座（ｓｏｃｋｅｔ）为例，必须输入英文（这是废话）。可能需要翻墙，不一定，如何翻墙请去隔壁下载ＶＰＮ
![](https://img-blog.csdn.net/20180516131318813)
点击需要的选项进入
![](https://img-blog.csdn.net/20180516131507336)
点击ｄｏｗｎｌｏａｄｓ标签，出现ＵＲＬＳ类
![](https://img-blog.csdn.net/20180516131534737)
点击出现很可怕的大量ｕｒｌ，下载保存在ｔｘｔ中备用。
二、ｐｙｔｈｏｎ脚本批量下载
用ｐｙｔｈｏｎ的ｕｒｌｉｂ库批量下载获取的ｕｒｌｓ，此处用的ｐｙｔｈｏｎ２．７环境，代码如下
# from urllib import request
import urllib2, urllib
import signal
path ='/home/hzc/Pictures/URL-TXT/watermeter.txt'
def handler(signum, frame):
    raise AssertionError
file = open(path)
for line in file:
    try:
        signal.signal(signal.SIGALRM, handler)
        signal.alarm(5)
        print(line)
        # # fake header
        # headers = {'User-Agent': 'Mozilla/5.0 (X11; Ubuntu; Linux x86_64; rv:52.0) Gecko/20100101 Firefox/52.0'}
        # req = urllib.request.Request(url=line, headers=headers)
        # urllib.request.urlopen(req).read()
try:
            f = urllib2.urlopen(line)
            data = f.read()
            with open('%s ' % line.split('/')[-1], "wb") as code:
                code.write(data)
        except:
            pass
# pic_link = line
        # save_path = r'/home/hzc/Pictures/%s.JPG '% line.split('/')[-1]
        # request.urlretrieve(pic_link, save_path)
except AssertionError:
        print("%s timeout " % line)
        continue
file.close()
得到一大串图片，重命名，加标签的请移步我的另一篇博客：
![](https://img-blog.csdn.net/20180516132028392?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L1laWG51YWE=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)
完毕
