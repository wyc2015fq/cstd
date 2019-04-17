# Python爬取手机壁纸 - zhusongziye的博客 - CSDN博客





2018年11月26日 19:30:12[zhusongziye](https://me.csdn.net/zhusongziye)阅读数：85








小编就爬取知乎上面那些好看的手机壁纸吧，各式各样都有！

# 代码很简单 只有14行，只有14行，只有14行！重要的事情说三遍！！！

```
from urllib import request  #从urllib库里导入request模块
from bs4 import BeautifulSoup   #从BeautifulSoup4(bs4)库里导入BeautifulSoup模块
import re   #导入正则表达式模块re模块
import time     #导入time模块

url = "https://www.zhihu.com/question/64252714"
html = request.urlopen(url).read().decode('utf-8')
soup = BeautifulSoup(html, 'html.parser')


links = soup.find_all('img', 'origin_image zh-lightbox-thumb', src = re.compile(r'jpg$'))
print(links)

path = r'E:\文章\5S分辨率\images'  #保存到某个文件夹下
for link in links:
    print(link.attrs['src'])
    request.urlretrieve(link.attrs['src'], path + '\%s.jpg' % time.time())
```

# 运行结果

![](https://img-blog.csdnimg.cn/20181124155316194.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3podXNvbmd6aXll,size_16,color_FFFFFF,t_70)



# 风景

![](https://img-blog.csdnimg.cn/20181124155325217.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3podXNvbmd6aXll,size_16,color_FFFFFF,t_70)



# 星空

![](https://img-blog.csdnimg.cn/20181124155337485.png)



# 美女

![](https://img-blog.csdnimg.cn/20181124155344192.png)



# 文字控

![](https://img-blog.csdnimg.cn/20181124155350665.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3podXNvbmd6aXll,size_16,color_FFFFFF,t_70)



# 动漫少女心

![](https://img-blog.csdnimg.cn/20181124155357445.png)



等等等

你看这么多类型，想换随时都是可以换的嘛。

想要更换和手机壳一样的颜色主题？教你用几行Python代码轻松搞定！为所欲为！

Emmmm...最后小编还想说一句：人生苦短，我用Python



