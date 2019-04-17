# 50行Python代码教你爬取猫眼电影TOP100榜所有信息 - zhusongziye的博客 - CSDN博客





2018年04月23日 22:21:41[zhusongziye](https://me.csdn.net/zhusongziye)阅读数：488








> 
来源： https://zhuanlan.zhihu.com/c_149865214


对于Python初学者来说，爬虫技能是应该是最好入门，也是最能够有让自己有成就感的，今天，恋习Python的手把手系列，手把手教你入门Python爬虫，爬取猫眼电影TOP100榜信息，将涉及到基础爬虫架构中的HTML下载器、HTML解析器、数据存储器三大模块：
- 
HTML下载器：利用requests模块下载HTML网页;

- 
HTML解析器：利用re正则表达式解析出有效数据;

- 
数据存储器：将有效数据通过文件或者数据库的形式存储起来;


#### 一、构造HTML下载器

```
1import requests
 2from requests.exceptions import RequestException
 3headers = {'User-Agent':'Mozilla/5.0 '}
 4def get_one_page(url):
 5    try:
 6        res = requests.get(url,headers = headers)
 7        if res.status_code == 200:
 8            return res.text
 9        return None
10    except RequestException:
11        return None
```

#### 二、构造HTML解析器

```
1def parse_one_page(html):
 2    pattern = re.compile('<dd>.*?board-index.*?>(\d+)</i>.*?data-src="(.*?)".*?name"><a'
 3                         +'.*?>(.*?)</a>.*?star">(.*?)</p>.*?releasetime">(.*?)</p>'
 4                         +'.*?integer">(.*?)</i>.*?fraction">(.*?)</i>.*?</dd>',re.S)
 5    items = re.findall(pattern,html)
 6    for item in items:
 7        yield{
 8            'index': item[0],
 9            'image': item[1],
10            'title': item[2],
11            'actor': item[3].strip()[3:],
12            'time': item[4].strip()[5:],
13            'score': item[5] + item[6]
14        }
```

> 
注意：
- 
在函数中本来该return的地方用yield，如果用return，在第一轮循环就会跳出，结果文件只会有一部电影。如果用yield，函数返回的就是一个生成器，而生成器作为一种特殊的迭代器，可以用for——in方法，一次一次的把yield拿出来；

- 
re.findall(pattern,string[,flags])：搜索整个string，以列表的形式返回能匹配的全部子串，其中参数是匹配模式，如re.S表示点任意匹配模式，改变“.”的行为。



#### 三、构造数据存储器

```
1def write_to_file(content):
2    with open ('result.txt', 'a',encoding='utf-8') as f:
3        f.write(json.dumps(content,ensure_ascii=False) + '\n')
4        f.close()
```

> 
注意事项：
- 
为什么ensure_ascii=False？原因是json默认是以ASCII来解析code的，由于中文不在ASCII编码当中，因此就不让默认ASCII生效；

- 
要写入特定编码的文本文件，请给open()函数传入encoding参数，将字符串自动转换成指定编码。细心的童鞋会发现，以'w'模式写入文件时，如果文件已存在，会直接覆盖（相当于删掉后新写入一个文件）。如果我们希望追加到文件末尾怎么办？可以传入'a'以追加（append）模式写入。



接下来就是构造主函数，初始化各个模块，传入入口URL，按照运行流程执行上面三大模块：

```
1def main(offset):
2    url = 'http://maoyan.com/board/4?offset=' + str(offset)
3    html = get_one_page(url)
4    for item in parse_one_page(html):
5        print(item)
6        write_to_file(item)
7if __name__ == '__main__':
8    p = Pool()
9    p.map(main,[i*10 for i in range(10)])
```

> 
注意：
为了提高速度，我们引入Pool模块，用多线程并发抓取。


执行代码后，结果如下：

![](https://img-blog.csdn.net/20180423222047159?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3podXNvbmd6aXll/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)


本文中介绍了基础爬虫架构主要的的三个模块（HTML下载器、HTML解析器、数据存储器），无论大型还是小型爬虫都不会脱离这三个模块，也希望大家通过这个小小的练习对整个爬虫有个清晰的认识，欢迎大家一起谈论学习交流。



