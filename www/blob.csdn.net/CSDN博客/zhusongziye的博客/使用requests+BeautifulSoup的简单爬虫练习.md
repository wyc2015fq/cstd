# 使用requests+BeautifulSoup的简单爬虫练习 - zhusongziye的博客 - CSDN博客





2018年04月08日 21:08:28[zhusongziye](https://me.csdn.net/zhusongziye)阅读数：233








今篇文章就是利用上篇的知识来爬取我们今天的主题网站：猫眼电影top100。这个网站也挺容易的，所以大家可以先自己爬取下，遇到问题再来看下这篇文章哈。




这篇文章主要是练习而已，别无用处，大佬请绕道哈！




1、本文用到的库及网站

- 
requests

- 
BeautifulSoup

- 
目标网站：http://maoyan.com/board/4





2、分析目标网站




![](https://img-blog.csdn.net/20180408210637409?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3podXNvbmd6aXll/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)

很容易找到我们想要的信息，上面的5的箭头都是我们想要的信息，分别是电影图片地址、电影名字、主演、上演时间和评分。内容有了，接下来就是获取下一页的链接。




![](https://img-blog.csdn.net/20180408210655651?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3podXNvbmd6aXll/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)

这里有两种方法，第一种就是在首页获取所有页的链接，第二种方法就是获取每个页面的下一页的链接。在这里由于只是给了部分页面的链接出来，所以我们获取的是下一页的链接，这样子方便点。




好了，分析完毕，接下来代码撸起。

3.敲代码

什么都不管，立即来个get请求




```
import requests
from bs4 import BeautifulSoup

url_start = 'http://maoyan.com/board/4'
response = requests.get(url_start)
if response.status_code == 200:
    soup = BeautifulSoup(response.text, 'lxml')
print(response.text)
```




输出结果：




![](https://img-blog.csdn.net/20180408210730396?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3podXNvbmd6aXll/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)

惊不惊喜，意不意外？如果你经常玩爬虫的，这个就见怪不怪了，我们被反爬了。我们试下加个请求头试试。




```
url_start = 'http://maoyan.com/board/4'
headers = {'User-Agent':'Mozilla/5.0 (Windows NT 10.0; WOW64) AppleWebKit/537.36 (KHTML, like Gecko) Chrome/64.0.3282.140 Safari/537.36'}
response = requests.get(url_start, headers=headers)
```




这样就可以正常返回了，因为一般的网站都会在请求头上加个反爬的，所以遇到了反爬也不要着急，加个请求头试试？




接下来用BeautifulSoupL来获取内容

```
imgs = soup.select('dd .board-img')  # 这是获取图片链接
titles = soup.select('dd .board-item-main .name')  # 这是获取电影名字
starses = soup.select('dd .board-item-main .movie-item-info .star')  # 这是获取电影主演
times = soup.select('dd .board-item-main .movie-item-info .releasetime')  # 这是获取电影上映时间
scores = soup.select('dd .board-item-main .score-num')  # 这是获取评分
```




这里每个获取的语句都包含了每个不同电影的信息，这样就不能和正则那样一次把每个电影的信息都在同一个字符里面了。就比如我获取的图片，一个语句获取的是这个页面的所有电影图片的链接，我们存储的时候就要分别取出来了。这里我用到的是for循环0到9把相同的坐标的信息存进同一个字典里面。

```
films = []  # 存储一个页面的所有电影信息
    for x in range(0, 10):
        # 这个是获取属性的链接
        img = imgs[x]['data-src']
        # 下面的都是获取标签的内容并去掉两端空格
        title = titles[x].get_text().strip()
        stars = starses[x].get_text().strip()[3:]  # 使用切片是去掉主演二字
        time = times[x].get_text().strip()[5:]  # 使用切片是去掉上映时间二字
        score = scores[x].get_text().strip()
        film = {'title': title, 'img': img, 'stars': stars, 'time': time, 'score': score}
        films.append(film)
```




接下来就是获取每一页的链接

```
pages = soup.select('.list-pager li a')  # 可以看到下一页的链接在最后一个a标签
    page = pages[len(pages)-1]['href']
```




后面的就简单了，就是利用循环把所有页面的内容都去取出来就可以了，代码就不贴出来了。




写在最后

这个就是BeautifulSoup库的小练习，用到昨天的内容不多，只是用到了选择器部分和获取文本内容和属性部分，感觉还是正则比较好用点哈，我一个正则就可以获取每个电影的详细内容了，如下:

```
<dd>.*?board-index.*?>([\d]{1,3})</i>.*?title="(.*?)".*?class="star">(.*?)</p>.*?class="releasetime">(.*?)</p>.*?class="integer">(.*?)</i>.*?class="fraction">(.*?)</i>
```




还需要用到个匹配模式哈：re.S就可以了。所以本人推荐使用正则表达式哈。




需要完整代码的请查看我的github哈！

github：https://github.com/SergioJune/gongzhonghao_code/blob/master/python3_spider/index.py






