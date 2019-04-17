# python 抓取开源中国上阅读数大于 1000 的优质文章 - zhusongziye的博客 - CSDN博客





2018年11月24日 14:08:36[zhusongziye](https://me.csdn.net/zhusongziye)阅读数：31标签：[python 抓取开源中国上阅读数大于 1000 的优质文章](https://so.csdn.net/so/search/s.do?q=python 抓取开源中国上阅读数大于 1000 的优质文章&t=blog)
个人分类：[Python爬虫](https://blog.csdn.net/zhusongziye/article/category/7611456)









Python 现在越来越火，连小学生都在学习 Python了 ^ ^，为了跟上时代，赶个时髦，秉承活到老学到老的精神，慢慢也开始学习 Python；理论是实践的基础，把 Python 相关语法看了，就迫不及待，大笔一挥来个 Hello world 压压惊，理论终归是理论，实践还是要要的嘛，动手了之后，才能更好的掌（chui）握（niu）基（bi）础；发车了..........

```
'''
作者：tsmyk0715
源自：https://my.oschina.net/mengyuankan/blog/1934171
人生苦短，我用Python
'''
```

首先浏览器输入 https://www.oschina.net/ 进入开源中国官网，点击顶部导航栏的 “博客” 选项进入博客列表页面，之后点击左侧 “服务端开发与管理” 选项，我们要爬取的是服务端相关的文章，如下图所示：

![](https://img-blog.csdnimg.cn/2018112414052835.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3podXNvbmd6aXll,size_16,color_FFFFFF,t_70)

接下来分析文章列表的布局方式，按 F12 打开调试页面，如下所示：

![](https://img-blog.csdnimg.cn/20181124140549176.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3podXNvbmd6aXll,size_16,color_FFFFFF,t_70)

可以看到，一篇文章的相关信息就是一个 div, class 属性为 item blog-item，打开该 div，如下：

![](https://img-blog.csdnimg.cn/20181124140613174.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3podXNvbmd6aXll,size_16,color_FFFFFF,t_70)

我们要抓取的是文章的标题，描述，URL，和阅读数，标题和URL可以通过 a 标签来获取，描述通过 <dev class=description> 来获取，而阅读数则要麻烦些，是第三个 <div class=item>，

通过以下代码就可以获取到以上到相关信息：

```
# 获取每个文章相关的 DIV
        articles_div = beautiful_soup.find_all("div", class_="item blog-item")
        # 处理每个 DIV
        for article_div in articles_div:
            content_div = article_div.find("div", class_="content")
            header_div = content_div.find("a", class_="header")
            # 文章URL
            url = header_div["href"]
            # 文章标题
            title = header_div["title"]
            # 文章描述
            description_str = content_div.find("div", class_="description").find("p", class_="line-clamp")
            if description_str is None:
                continue
            description = description_str.text
            # 文章阅读数
            read_count_div = content_div.find("div", class_="extra").find("div", class_="ui horizontal list").find("div", class_="item")
            # find_next_sibling 获取兄弟节点
            read_count_i = read_count_div.find_next_sibling("div", class_="item").find_next_sibling("div", class_="item")
            read_count = read_count_i.getText()
```

上述代码就是主要的获取相关信息的逻辑，因为阅读数没有唯一id，或者 class ，所有可以通过 find_next_sibling 来获取兄弟节点；

接下来就对获取到到文章进行处理，如按照阅读数大于等于1000过滤文章，并按照阅读数从高到低低排序，并且写到文件中：

首先要定义一个文章类，用来表示文章的相关信息，如下：

```
"""
文章实体类
@authon:tsmyk0715
"""
class Article:

    def __init__(self, title, url, content="", read_cnt=0):
        """
        文章类构造方法
        :param title:文章标题
        :param url: 文章 URL
        :param content: 文章内容
        :param read_cnt: 文章阅读数
        """
        self.title = title
        self.url = url
        self.content = content
        self.read_cnt = read_cnt

    def __str__(self):
        return u"文章：标题《{0}》，阅读数：{1}，链接：{2}".format(self.title, self.read_cnt, self.url)
```

之后，定义文章的处理类 OschinaArticle ，相关处理逻辑在该类中实现：

```
import requests
# 使用 BeautifulSoup 库来解析 HTML 页面
from bs4 import BeautifulSoup
import logging
import time
# 导入定义的文章实体类
from Article import Article

class OschinaArticle:

    def __init__(self):
        # 日志
        self.log = logging
        # 设置日志级别为 INFO
        self.log.basicConfig(level=logging.INFO)
        # 把文章写到文件的行号
        self.file_line_num = 1
```

接下来获取 BeautifulSoup 对象：

```
def getSoup(self, url):
        """
        根据 url 获取 BeautifulSoup 对象
        """
        # 请求头
        headers = {
            "User-Agent": "Mozilla/5.0 (Windows NT 10.0; Win64; x64) AppleWebKit/537.36 (KHTML, like Gecko) Chrome/68.0.3440.75 Safari/537.36",
            "Accept": "text/html,application/xhtml+xml,application/xml;q=0.9,image/webp,image/apng,*/*;q=0.8",
            "Host": "www.oschina.net"}
        # 请求的时候，需要加上头部信息，表示是人在操作，而不是机器，不加头部信息会报错
        response = requests.get(url, headers=headers)
        return BeautifulSoup(response.text, "html.parser")
```

之后，通过 BeautifulSoup 来解析 HTML 页面，获取文章相关信息，之后，根据相关信息创建文章对象，放到集合中进行返回：

```
def get_articles(self, url):
        # 存放文章的集合，进行返回
        article_list = []
        beautiful_soup = self.getSoup(url)
        self.log.info(u"开始解析 HTML 页面...")
        articles_div = beautiful_soup.find_all("div", class_="item blog-item")
        for article_div in articles_div:
            content_div = article_div.find("div", class_="content")
            header_div = content_div.find("a", class_="header")
            # 文章URL
            url = header_div["href"]
            # 文章标题
            title = header_div["title"]
            # 文章描述
            description_str = content_div.find("div", class_="description").find("p", class_="line-clamp")
            if description_str is None:
                continue
            description = description_str.text
            # 文章阅读数
            read_count_div = content_div.find("div", class_="extra").find("div", class_="ui horizontal list").find("div", class_="item")
            # find_next_sibling 获取兄弟节点
            read_count_i = read_count_div.find_next_sibling("div", class_="item").find_next_sibling("div", class_="item")
            read_count = read_count_i.getText()
            # 根据相关信息创建文章对象，放到集合中，进行返回
            __article = Article(title, url, description, read_count)
            article_list.append(__article)

            self.log.info(u"文章：标题《{0}》，阅读数：{1}，链接：{2}".format(title, read_count, url))

        return article_list
```

因为文章的阅读数如果超过 1000 的话，就用 K 来表示，为了在后面筛选指定阅读数的文章，所以需要进行处理，把 K 转换为 1000，代码如下：

```
def handler_read_count(self, article_list):
        """
        处理阅读数：把 K 转换为 1000
        :param article_list:文章列表
        """
        if article_list is None or len(article_list) == 0:
            self.log.info(u"文章列表为空...")
            return
        for article in article_list:
            read_count_str = article.read_cnt.strip()
            read_count = 0
            if isinstance(read_count_str, str):
                if read_count_str.endswith("K"):
                    read_count_str = read_count_str[:-1]  # 去掉K
                    read_count = int(float(read_count_str) * 1000)
                else:
                    read_count = int(read_count_str)
            article.read_cnt = read_count
```

接下来就是文章根据阅读数进行筛选和排序了，筛选出阅读数大于等于指定值并且按照阅读数从高到低排序，代码如下：

```
def get_article_by_read_count_sort(self, article_list, min_read_cnt):
        """
        获取大于等于指定阅读数的文章信息, 并按照阅读数从高到低排序
        :param article_list: 文章列表
        :param minx_read_cnt: 最小阅读数
        :return:
        """
        if article_list is None or len(article_list) == 0:
            self.log.info(u"文章列表为空...")
            return
        article_list_return = []
        for article in article_list:
            if article.read_cnt >= min_read_cnt:
                article_list_return.append(article)
        # 使用 Lambda 对集合中的对象按照 read_cnt 属性进行排序
        article_list_return.sort(key=lambda Article: Article.read_cnt, reverse=True)
        return article_list_return
```

以上就可以获取到我们想要的文章信息了，此外，我们可以把信息写到文件文件中，代码如下：

```
def write_file(self, article_list, file_path):
        # 创建 IO 对象
        file = open(file_path + "/articles.txt", "a")

        for article in article_list:
            _article_str = str(article)
            file.write("（" + str(self.file_line_num) + "）" + _article_str)
            file.write("\n")
            file.write("--------------------------------------------------------------------------------------------------------------------------------------------------------")
            file.write("\n")
            # 文件行号
            self.file_line_num += 1 
            time.sleep(0.2)  # 休眠 200 毫秒
        file.close()
```

之后，把上面的方法整合在一起，代码如下：

```
def run(self, url, min_read_count):
        # 获取所有文章
        article_list = self.get_articles(url)
        # 对阅读数进行处理
        self.handler_read_count(article_list)
        # 筛选阅读数大于等于指定值，并按阅读数从高到低排序
        _list = self.get_article_by_read_count_sort(article_list, min_read_count)
        # 写文件
        self.write_file(_list, "G:/python")
        # 打印控制台
        for a in _list:
            self.log.info(a)
```

main 方法测试一下，地址输入：https://www.oschina.net/blog?classification=428640, 文章阅读数要大于等于1000

```
if __name__ == '__main__':
    article = OschinaArticle()
    main_url = "https://www.oschina.net/blog?classification=428640"
    min_read_count = 1000
    article.run(main_url, min_read_count)
```

控制台日志打印如下：

![](https://img-blog.csdnimg.cn/20181124140709387.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3podXNvbmd6aXll,size_16,color_FFFFFF,t_70)

写入到文件中的内容如下：

![](https://img-blog.csdnimg.cn/20181124140727137.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3podXNvbmd6aXll,size_16,color_FFFFFF,t_70)

你以为到这里就完了吗，no, no, no.............，通过上述方式只能获取到首页的文章，如果想获取更多的文章怎么办？开源中国的博客文章列表没有分页，是通过滑动鼠标滚轮来获取更多的页，可是人家的地址导航栏却没有丝毫没有变动，但是可以通过 F12 来看呀，按 F12 后，通过 NetWork 来查看相关的请求和响应情况：

![](https://img-blog.csdnimg.cn/20181124140735547.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3podXNvbmd6aXll,size_16,color_FFFFFF,t_70)



通过滚动几下鼠标滚轮之后，可以发现请求的 URL 还是有规律的：

https://www.oschina.net/blog/widgets/_blog_index_recommend_list?classification=428640&p=2&type=ajax

https://www.oschina.net/blog/widgets/_blog_index_recommend_list?classification=428640&p=2&type=ajax

https://www.oschina.net/blog/widgets/_blog_index_recommend_list?classification=428640&p=3&type=ajax

https://www.oschina.net/blog/widgets/_blog_index_recommend_list?classification=428640&p=4&type=ajax

https://www.oschina.net/blog/widgets/_blog_index_recommend_list?classification=428640&p=5&type=ajax

可以看到除了 p 的参数值不同的话，其他的都相同，p 就是分页标识，p=2就表示第二页，p=3就等于第三页，以此类推，就可以获取到更多的文章啦：

```
def main(self, min_read_count, page_size):
        # 首页 URL
        self.log.info("首页##########################")
        main_url = "https://www.oschina.net/blog?classification=428640"
        self.run(main_url, min_read_count)
        # 第2页到第page_size页
        for page in range(2, page_size):
            self.log.info("第 {0} 页##########################".format(str(page)))
            page_url = "https://www.oschina.net/blog/widgets/_blog_index_recommend_list?classification=428640&p=" + str(page) + "&type=ajax"
            self.run(page_url, min_read_count)
            time.sleep(2)
```

测试：

```
if __name__ == '__main__':
    article = OschinaArticle()
    # 获取到20页的相关文章，并且阅读数要大于等于1000
    article.main(1000, 21)
```

日志控制台打印如下：

![](https://img-blog.csdnimg.cn/20181124140757296.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3podXNvbmd6aXll,size_16,color_FFFFFF,t_70)

写到文件中如下：

![](https://img-blog.csdnimg.cn/20181124140806116.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3podXNvbmd6aXll,size_16,color_FFFFFF,t_70)

可以看到，在 1-20 页中，阅读数大于等 1000 的文章有 114 篇，之后就可以 copy URL 到地址栏进行阅读啦....................

**完整代码如下：**

**OschinaArticle 处理逻辑类：**

```
# -*- coding:utf-8 -*-

import requests
from bs4 import BeautifulSoup
import logging
import time
from Article import Article

"""
    爬取开源中国上的文章，且阅读数大于等于1000
"""


class OschinaArticle:

    def __init__(self):
        self.log = logging
        self.log.basicConfig(level=logging.INFO)
        self.file_line_num = 1

    def getSoup(self, url):
        """
        根据 url 获取 BeautifulSoup 对象
        """
        headers = {
            "User-Agent": "Mozilla/5.0 (Windows NT 10.0; Win64; x64) AppleWebKit/537.36 (KHTML, like Gecko) Chrome/68.0.3440.75 Safari/537.36",
            "Accept": "text/html,application/xhtml+xml,application/xml;q=0.9,image/webp,image/apng,*/*;q=0.8",
            "Host": "www.oschina.net"}
        response = requests.get(url, headers=headers)
        return BeautifulSoup(response.text, "html.parser")

    def get_articles(self, url):
        article_list = []
        beautiful_soup = self.getSoup(url)
        self.log.info(u"开始解析 HTML 页面...")
        articles_div = beautiful_soup.find_all("div", class_="item blog-item")
        for article_div in articles_div:
            content_div = article_div.find("div", class_="content")
            header_div = content_div.find("a", class_="header")
            # 文章URL
            url = header_div["href"]
            # 文章标题
            title = header_div["title"]
            # 文章描述
            description_str = content_div.find("div", class_="description").find("p", class_="line-clamp")
            if description_str is None:
                continue
            description = description_str.text
            # 文章阅读数
            read_count_div = content_div.find("div", class_="extra").find("div", class_="ui horizontal list").find("div", class_="item")
            # find_next_sibling 获取兄弟节点
            read_count_i = read_count_div.find_next_sibling("div", class_="item").find_next_sibling("div", class_="item")
            read_count = read_count_i.getText()
            __article = Article(title, url, description, read_count)
            article_list.append(__article)
            # self.log.info(u"文章：标题《{0}》，阅读数：{1}，链接：{2}".format(title, read_count, url))
        return article_list

    def handler_read_count(self, article_list):
        """
        处理阅读数：把 K 转换为 1000
        :param article_list:文章列表
        """
        if article_list is None or len(article_list) == 0:
            self.log.info(u"文章列表为空...")
            return
        for article in article_list:
            read_count_str = article.read_cnt.strip()
            read_count = 0
            if isinstance(read_count_str, str):
                if read_count_str.endswith("K"):
                    read_count_str = read_count_str[:-1]  # 去掉K
                    read_count = int(float(read_count_str) * 1000)
                else:
                    read_count = int(read_count_str)
            article.read_cnt = read_count

    def get_article_by_read_count_sort(self, article_list, min_read_cnt):
        """
        获取大于等于指定阅读数的文章信息, 并按照阅读数从高到低排序
        :param article_list: 文章列表
        :param minx_read_cnt: 最小阅读数
        :return:
        """
        if article_list is None or len(article_list) == 0:
            self.log.info(u"文章列表为空...")
            return
        article_list_return = []
        for article in article_list:
            if article.read_cnt >= min_read_cnt:
                article_list_return.append(article)
        article_list_return.sort(key=lambda Article: Article.read_cnt, reverse=True)
        return article_list_return

    def write_file(self, article_list, file_path):
        file = open(file_path + "/articles.txt", "a")

        for article in article_list:
            _article_str = str(article)
            file.write("（" + str(self.file_line_num) + "）" + _article_str)
            file.write("\n")
            file.write("--------------------------------------------------------------------------------------------------------------------------------------------------------")
            file.write("\n")
            self.file_line_num += 1
            time.sleep(0.2)  # 休眠 200 毫秒
        file.close()

    def run(self, url, min_read_count):
        # 获取所有文章
        article_list = self.get_articles(url)
        # 对阅读数进行处理
        self.handler_read_count(article_list)
        # 筛选阅读数大于等于指定值，并按阅读数从高到低排序
        _list = self.get_article_by_read_count_sort(article_list, min_read_count)
        # 写文件
        self.write_file(_list, "G:/python")
        # 打印控制台
        for a in _list:
            self.log.info(a)

    def main(self, min_read_count, page_size):
        # 首页 URL
        self.log.info("首    页##########################")
        main_url = "https://www.oschina.net/blog?classification=428640"
        self.run(main_url, min_read_count)
        # 第2页到第page_size页
        for page in range(2, page_size):
            self.log.info("第 {0} 页##########################".format(str(page)))
            page_url = "https://www.oschina.net/blog/widgets/_blog_index_recommend_list?classification=428640&p=" + str(page) + "&type=ajax"
            self.run(page_url, min_read_count)
            time.sleep(2)


if __name__ == '__main__':
    article = OschinaArticle()
    # 获取到20页的相关文章，并且阅读数要大于等于1000
    article.main(1000, 21)
```





