# Python爬取视频之日本爱情电影 - zhusongziye的博客 - CSDN博客





2018年11月01日 21:34:01[zhusongziye](https://me.csdn.net/zhusongziye)阅读数：1788








肉身翻墙后，感受一下外面的肮脏世界。墙内的朋友叫苦不迭，由于某些原因，VPN能用的越来越少。上周我的好朋友狗子和我哭诉说自己常用的一个VPN终于也寿终正寝了，要和众多的日本小姐姐说再见了。作为“外面人”，我还是要帮他一把……

```
作者:永无乡
源自：https://blog.csdn.net/JosephPai/article/details/78897370
```

**初探**



狗子给我的网站还算良心，只跳了五个弹窗就消停了。 

然后看到的就是各种穿不起衣服的女生的卖惨视频，我赶紧闭上眼睛，默念了几句我佛慈悲。 

Tokyo真的有那么hot？ 

给狗子发了一张大的截图，狗子用涂鸦给我圈出了其中一个。 

我和狗子说“等着吧” 

（放心网站截图我是打了码也不敢放的。。。）

![](https://img-blog.csdnimg.cn/20181101213406414.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3podXNvbmd6aXll,size_16,color_FFFFFF,t_70)

点进去之后，可以在线播放。 

右下角有一个 Download 按钮，点击之后需要注册付费。 

当时我就火了，这种卖惨视频毒害我兄弟精神，还敢收钱？！ 

自己动手，丰衣足食！



**环境 & 依赖**



Win10 64bit 

IDE: PyCharm 

Python 3.6 

python-site-packegs: requests + BeautifulSoup + lxml + re + m3u8



在已经安装pip的环境下均可直接命令行安装



网站解析



将链接复制到Chrome浏览器打开 

（我平时用猎豹，也是Chrome内核，界面比较舒服，但是这个时候必须大喊一声谷歌大法好） 

菜单——更多工具——开发者选项（或者快捷键F12）进入Chrome内置的开发者模式 

大概界面是这样

![](https://img-blog.csdnimg.cn/20181101213420461.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3podXNvbmd6aXll,size_16,color_FFFFFF,t_70)



（唉打码真的累。。。。） 

然后，根据提示，逐层深入标签找到视频所在具体位置 

这个网站存放的位置是 …->flash->video_container->video-player 

显然视频就放在这个这个video-player中 

在这个标签中，有一个名字为 source 的链接，src=”http://#%@就不告诉你#@￥” 

Easy好吧！ 

这点小把戏还难得到我？我已经准备和狗子要红包了 

复制该链接到地址栏粘贴并转到，然后，神奇的一幕出现了!!![](https://img-blog.csdnimg.cn/20181101213437621.png)



What？？？

这是什么？？？

![](https://img-blog.csdnimg.cn/20181101213449339.png)

为啥这么小？？？



![](https://img-blog.csdnimg.cn/20181101213458975.png)



科普概念如上，那也就是说，m3u8记录了真实的视频所在的地址。



Network Traffic



想要从源码直接获得真实下载地址怕是行不通了。 

这时候再和我一起读“谷歌大法好！” 

很简单，浏览器在服务器中Get到视频呈现到我们面前，那这个过程必定经过了解析这一步。 

那我们也可以利用浏览器这个功能来进行解析



依旧在开发者模式，最上面一行的导航栏，刚刚我们在Elements选项卡，现在切换到Network 

我们监听视频播放的时候的封包应该就可以得到真实的视频地址了，试试看！

![](https://img-blog.csdnimg.cn/2018110121351585.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3podXNvbmd6aXll,size_16,color_FFFFFF,t_70)

我们惊喜的发现，一个又一个的 .ts 文件正在载入了 

（如果在图片里发现任何url请友情提醒我谢谢不然怕是水表难保）

![](https://img-blog.csdnimg.cn/20181101213528938.png)



知识点！这都是知识点！（敲黑板！）

点开其中的一个.ts文件看一下 

![](https://img-blog.csdnimg.cn/20181101213539578.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3podXNvbmd6aXll,size_16,color_FFFFFF,t_70)

这里可以看到请求头，虽然url被我走心的码掉了，但这就是真实的视频地址了



复制这个URL到地址栏，下载 

9s。。。。。 

每一个小视频只有9s，难道要一个又一个的去复制吗？



视频片段爬取



答案是当然不用。 

这里我们要请出网络数据采集界的装逼王：Python爬虫！！！



![](https://img-blog.csdnimg.cn/20181101213554403.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3podXNvbmd6aXll,size_16,color_FFFFFF,t_70)



首先进行初始化，包括路径设置，请求头的伪装等。



采集部分主要是将requests的get方法放到了for循环当中 

这样做可行的原因在于，在Network监听的图中我们可以看到.ts文件的命名是具有规律的 seg-i-v1-a1，将i作为循环数



那么问题又来了，我怎么知道循环什么时候结束呢？也就是说我怎么知道i的大小呢？ 

等等，我好像记得在视频播放的框框右下角有时间来着？ 

在开发者模式中再次回到Element选项卡，定位到视频框右下角的时间，标签为duration，这里的时间格式是 时：分：秒格式的，我们可以计算得到总时长的秒数 

但是呢，这样需要我们先获取这个时间，然后再进行字符串的拆解，再进行数学运算，太复杂了吧，狗子已经在微信催我了 

Ctrl+F全局搜索duration 

![](https://img-blog.csdnimg.cn/20181101213605593.png)

Yes!!!



好了，可以点击运行然后去喝杯咖啡，哦不，我喜欢喝茶。

![](https://img-blog.csdnimg.cn/20181101213613490.png)

一杯茶的功夫，回来之后已经下载完成。我打开文件夹check一下，发现从编号312之后的clip都是只有573字节，打开播放的话，显示的是数据损坏。 

没关系，从312开始继续下载吧。然而下载得到的结果还是一样的573字节，而且下了两百多个之后出现了拒绝访问错误。



动态代理



显然我的IP被封了。之前的多个小项目，或是因为网站防护不够严格，或是因为数据条目数量较少，一直没有遇到过这种情况，这次的数据量增加，面对这种情况采取两种措施，一种是休眠策略，另一种是动态代理。现在我的IP已经被封了，所以休眠也为时已晚，必须采用动态IP了。 

主要代码如下所示 

![](https://img-blog.csdnimg.cn/20181101213625439.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3podXNvbmd6aXll,size_16,color_FFFFFF,t_70)



**合并文件**



然后，我们得到了几百个9s的.ts小视频



然后，在cmd命令行下，我们进入到这些小视频所在的路径 

执行



copy/b %s\*.ts %s\new.ts

很快，我们就得到了合成好的视频文件 

当然这个前提是这几百个.ts文件是按顺序排列好的。



成果如下

![](https://img-blog.csdnimg.cn/20181101213634805.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3podXNvbmd6aXll,size_16,color_FFFFFF,t_70)

优化—调用DOS命令 + 解析m3u8



为了尽可能的减少人的操作，让程序做更多的事 

我们要把尽量多的操作写在code中



引用os模块进行文件夹切换，在程序中直接执行合并命令 

并且，在判断合并完成后，使用清除几百个ts文件

![](https://img-blog.csdnimg.cn/20181101213643508.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3podXNvbmd6aXll,size_16,color_FFFFFF,t_70)

这样，我们运行程序后，就真的可以去喝一杯茶，回来之后看到的就是没有任何多余的一个完整的最终视频



也就是说，要获得一个完整的视频，我们现在需要输入视频网页链接，还需要使用chrome的network解析得到真实下载地址。第二个部分显然不够友好，还有提升空间。



所以第一个尝试是，可不可以有一个工具或者一个包能嗅探到指定网页的network traffic，因为我们刚刚已经看到真实地址其实就在requestHeader中，关键在于怎样让程序自动获取。



查阅资料后，尝试了Selenium + PhantomJS的组合模拟浏览器访问，用一个叫做browsermobProxy的工具嗅探保存HAR（HTTP archive）。在这个上面花费了不少时间，但是关于browsermobProxy的资料实在是太少了，即使是在google上，搜到的也都是基于java的一些资料，面向的python的API也是很久没有更新维护了。此路不通。



在放弃之前，我又看一篇网站的源码，再次把目光投向了m3u8，上面讲到这个文件应该是包含文件真实地址的索引，索引能不能把在这上面做些文章呢？



Python不愧是万金油语言，packages多到令人发指，m3u8处理也是早就有熟肉。



pip install m3u8

这是一个比较小众的包，没有什么手册，只能自己读源码。

![](https://img-blog.csdnimg.cn/20181101213656184.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3podXNvbmd6aXll,size_16,color_FFFFFF,t_70)

这个class中已经封装好了不少可以直接供使用的数据类型，回头抽时间可以写一写这个包的手册。



现在，我们可以从requests获取的源码中，首先找到m3u8的下载地址，首先下载到本地，然后用m3u8包进行解析，获取真实下载地址。



并且，解析可以得到所有地址，意味着可以省略上面的获取duration计算碎片数目的步骤。



最终



最终，我们现在终于可以，把视频网页链接丢进url中，点击运行，然后就可以去喝茶了。 

再来总结一下实现这个的几个关键点： 

- 网页解析 

- m3u8解析 

- 动态代理设置 

- DOS命令行的调用



动手是最好的老师，尤其这种网站，兼具趣味性和挑战性，就是身体一天不如一天。。。



**完整代码**

```
# -*- coding:utf-8 -*-
import requests
from bs4 import BeautifulSoup
import os
import lxml
import time
import random
import re
import m3u8

class ViedeoCrawler():
    def __init__(self):
        self.url = ""
        self.down_path = r"F:\Spider\VideoSpider\DOWN"
        self.final_path = r"F:\Spider\VideoSpider\FINAL"
        try:
            self.name = re.findall(r'/[A-Za-z]*-[0-9]*',self.url)[0][1:]
        except:
            self.name = "uncensord"
        self.headers = {
            'Connection': 'Keep-Alive',
            'Accept': 'text/html, application/xhtml+xml, */*',
            'Accept-Language': 'en-US,en;q=0.8,zh-Hans-CN;q=0.5,zh-Hans;q=0.3',
            'User-Agent':'Mozilla/5.0 (Linux; U; Android 6.0; zh-CN; MZ-m2 note Build/MRA58K) AppleWebKit/537.36 (KHTML, like Gecko) Version/4.0 Chrome/40.0.2214.89 MZBrowser/6.5.506 UWS/2.10.1.22 Mobile Safari/537.36'
        }

    def get_ip_list(self):
        print("正在获取代理列表...")
        url = 'http://www.xicidaili.com/nn/'
        html = requests.get(url=url, headers=self.headers).text
        soup = BeautifulSoup(html, 'lxml')
        ips = soup.find(id='ip_list').find_all('tr')
        ip_list = []
        for i in range(1, len(ips)):
            ip_info = ips[i]
            tds = ip_info.find_all('td')
            ip_list.append(tds[1].text + ':' + tds[2].text)
        print("代理列表抓取成功.")
        return ip_list

    def get_random_ip(self,ip_list):
        print("正在设置随机代理...")
        proxy_list = []
        for ip in ip_list:
            proxy_list.append('http://' + ip)
        proxy_ip = random.choice(proxy_list)
        proxies = {'http': proxy_ip}
        print("代理设置成功.")
        return proxies

    def get_uri_from_m3u8(self,realAdr):
        print("正在解析真实下载地址...")
        with open('temp.m3u8', 'wb') as file:
            file.write(requests.get(realAdr).content)
        m3u8Obj = m3u8.load('temp.m3u8')
        print("解析完成.")
        return m3u8Obj.segments

    def run(self):
        print("Start!")
        start_time = time.time()
        os.chdir(self.down_path)
        html = requests.get(self.url).text
        bsObj = BeautifulSoup(html, 'lxml')
        realAdr = bsObj.find(id="video-player").find("source")['src']

        # duration = bsObj.find('meta', {'property': "video:duration"})['content'].replace("\"", "")
        # limit = int(duration) // 10 + 3

        ip_list = self.get_ip_list()
        proxies = self.get_random_ip(ip_list)
        uriList = self.get_uri_from_m3u8(realAdr)
        i = 1   # count
        for key in uriList:
            if i%50==0:
                print("休眠10s")
                time.sleep(10)
            if i%120==0:
                print("更换代理IP")
                proxies = self.get_random_ip(ip_list)
            try:
                resp = requests.get(key.uri, headers = self.headers, proxies=proxies)
            except Exception as e:
                print(e)
                return
            if i < 10:
                name = ('clip00%d.ts' % i)
            elif i > 100:
                name = ('clip%d.ts' % i)
            else:
                name = ('clip0%d.ts' % i)
            with open(name,'wb') as f:
                f.write(resp.content)
                print('正在下载clip%d' % i)
            i = i+1
        print("下载完成！总共耗时 %d s" % (time.time()-start_time))
        print("接下来进行合并……")
        os.system('copy/b %s\\*.ts %s\\%s.ts' % (self.down_path,self.final_path, self.name))
        print("合并完成，请您欣赏！")
        y = input("请检查文件完整性，并确认是否要删除碎片源文件？(y/n)")
        if y=='y':
            files = os.listdir(self.down_path)
            for filena in files:
                del_file = self.down_path + '\\' + filena
                os.remove(del_file)
            print("碎片文件已经删除完成")
        else:
            print("不删除，程序结束。")

if __name__=='__main__':
    crawler = ViedeoCrawler()
    crawler.run()
```





