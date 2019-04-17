# python 爬取京东手机图 - 村雨sup - 博客园








[博客园](https://www.cnblogs.com/)[首页](https://www.cnblogs.com/cunyusup/)[新随笔](https://i.cnblogs.com/EditPosts.aspx?opt=1)[联系](https://msg.cnblogs.com/send/%E6%9D%91%E9%9B%A8sup)[管理](https://i.cnblogs.com/)[订阅](https://www.cnblogs.com/cunyusup/rss)![订阅](https://www.cnblogs.com/images/xml.gif)





初学urllib，高手勿喷...

```
import re
import urllib.request
#函数：每一页抓取的30张图片
def craw(url,page):
    imagelist = []#这里每次都要重新定义新的空列表，第一次没有定义结果爬取的都是一样的图片
    html1 = urllib.request.urlopen(url)
    data = str(html1.read())
    patter1 = '<li class="gl-item".+?</li>'
    result1 = re.compile(patter1).findall(data)
    patter2 = '//img.+?.jpg'#用.+?代表的是中间可是是任意多个字符
    for i in range(30):
        result_temp = result1[i]
        imagelist.append(re.compile(patter2).findall(result_temp))#用compile和findall得出的是列表，再将得到的列表加入整个的一个列表
    for i in range(1,30):#一开始调试用的是笨办法一个个的点，后来发现可以在断点处设置i==4
        try:#在imagelist中有存在图片抓取失败的情况，最懒的解决办法就是用try——except，无论出现什么情况都pass掉
            imageurl = "http:"+str(imagelist[i][0])
            imagename = "D:/picture/"+str(page)+str(i)+".jpg"
            result = urllib.request.urlopen(imageurl)#得到图片的地址后，**还是用打开连接用read()得到data的方法获取图片
            if (result.getcode() != 200):#如果链接不正常，则跳过这个链接
                pass
            else:
                data = result.read()  # 否则开始下载到本地
                with open(imagename, "wb") as code:
                    code.write(data)
                    code.close()
        except:
            pass



url = 'https://search.jd.com/Search?keyword=%E6%89%8B%E6%9C%BA&enc=utf-8&qrst=1&rt=1&stop=1&vt=2&wq=%E6%89%8B%E6%9C%BA&cid2=653&cid3=655&page='
for i in range(2,10):#先从2页爬取到9页
    url_new = url + str(2*i-1)#分析每页的地址将page后的无用的地方删去，再加上（2n-1），n为页数
    craw(url_new,i)
```













