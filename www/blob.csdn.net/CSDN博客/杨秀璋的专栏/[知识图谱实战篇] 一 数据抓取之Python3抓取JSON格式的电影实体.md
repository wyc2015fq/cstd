# [知识图谱实战篇] 一.数据抓取之Python3抓取JSON格式的电影实体 - 杨秀璋的专栏 - CSDN博客





2019年01月31日 14:33:53[Eastmount](https://me.csdn.net/Eastmount)阅读数：450标签：[知识图谱																[网络爬虫																[电影数据																[Python3](https://so.csdn.net/so/search/s.do?q=Python3&t=blog)
个人分类：[知识图谱实战																[知识图谱																[关系图谱																[Python爬虫](https://blog.csdn.net/Eastmount/article/category/5758691)
所属专栏：[Python爬虫之Selenium+Phantomjs+CasperJS](https://blog.csdn.net/column/details/eastmount-spider.html)[知识图谱、web数据挖掘及NLP](https://blog.csdn.net/column/details/eastmount-kgdmnlp.html)](https://blog.csdn.net/Eastmount/article/category/7944204)




前面作者讲解了很多知识图谱相关的原理知识，包括知识图谱相关技术、Neo4j绘制关系图谱等，但还是缺少一个系统全面的实例。为了加深自己对知识图谱构建的认识，为后续创建贵州旅游知识图谱打下基础，作者学习了张宏伦老师的网易云课程（[星球系列电影](https://study.163.com/course/courseLearn.htm?courseId=1003528010)），强烈推荐大家去学习，并结合自己的理解和技术分享了该系列专栏，从数据采集、数据分析到知识图谱构建，文章后续会讲解中文数据的实体识别、关系抽取、知识展示等。

这是一篇基础性文章，希望对您有所帮助 ，尤其是刚入门的同学。同时也是因为最近准备博士考试，做题做吐了，写点新东西调节下心情，与君共勉，一起加油。

下载地址：[https://download.csdn.net/download/eastmount/10947419](https://download.csdn.net/download/eastmount/10947419)





### 文章目录
- [一.目标网站](#_47)
- [二.抓取电影films页面](#films_81)
- [三.抓取所有实体页面](#_198)



![](https://img-blog.csdnimg.cn/20190131143727293.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L0Vhc3Rtb3VudA==,size_16,color_FFFFFF,t_70)

**推荐作者的知识图谱前文：**
[知识图谱相关会议之观后感分享与学习总结](https://blog.csdn.net/Eastmount/article/details/46672701)
[中文知识图谱研讨会的学习总结 (上) 图谱引入、百度知心、搜狗知立方](https://blog.csdn.net/Eastmount/article/details/49785639)
[搜索引擎和知识图谱那些事 (上).基础篇](https://blog.csdn.net/Eastmount/article/details/46874155)
[基于VSM的命名实体识别、歧义消解和指代消解](https://blog.csdn.net/Eastmount/article/details/48566671)
[CSDN下载-第一届全国中文知识图谱研讨会演讲PPT 清华大学](https://download.csdn.net/download/eastmount/9255871)
[CSDN下载-知识图谱PDF资料 清华大学知识图谱研讨会汇报PPT](https://download.csdn.net/download/eastmount/9159689)

[[知识图谱构建] 一.Neo4j图数据库安装初识及药材供应图谱实例](https://blog.csdn.net/Eastmount/article/details/83268410)
[[知识图谱构建] 二.《Neo4j基础入门》基础学习之创建图数据库节点及关系](https://blog.csdn.net/Eastmount/article/details/85093828)
[[关系图谱] 一.Gephi通过共线矩阵构建知网作者关系图谱](https://blog.csdn.net/Eastmount/article/details/81746650)
[[关系图谱] 二.Gephi导入共线矩阵构建作者关系图谱](https://blog.csdn.net/Eastmount/article/details/85046305)![](https://img-blog.csdnimg.cn/20190131143937685.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L0Vhc3Rtb3VudA==,size_16,color_FFFFFF,t_70)
再次强烈推荐大家阅读张宏伦老师的网易云课程及Github源码，受益匪浅。
[https://github.com/Honlan/starwar-visualization/tree/master/star_war](https://github.com/Honlan/starwar-visualization/tree/master/star_war)
[https://study.163.com/course/courseLearn.htm?courseId=1003528010](https://study.163.com/course/courseLearn.htm?courseId=1003528010)
张老师最终成果如下所示：[http://zhanghonglun.cn/starwars/](http://zhanghonglun.cn/starwars/)
![](https://img-blog.csdnimg.cn/20190131145152464.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L0Vhc3Rtb3VudA==,size_16,color_FFFFFF,t_70)
# 一.目标网站

我们的目标是构建一个《星球大战》7部电影的知识图谱，首先需要获取电影数据信息，常规的方法是通过豆瓣进行抓取，但是豆瓣收录的信息不完整，这里爬取 SWAPI 这个网站。

网址：[https://swapi.co/documentation](https://swapi.co/documentation)
![](https://img-blog.csdnimg.cn/20190131144412638.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L0Vhc3Rtb3VudA==,size_16,color_FFFFFF,t_70)
这是全球首个量化的、可供编程使用的星球大战数据集，开发者汇总了星战系列电影中涉及的多个种类实体数据。它同时提供了Python编程语言包，swapi-python is built by the author of swapi, Paul Hallett。六个API对应六类实体：
- Films：[http://swapi.co/api/films/1](http://swapi.co/api/films/1)
- People：[http://swapi.co/api/people/1](http://swapi.co/api/people/1)
- Starships：[http://swapi.co/api/starships/1](http://swapi.co/api/starships/1)
- Vehicles：[http://swapi.co/api/vehicles/1](http://swapi.co/api/vehicles/1)
- Species：[http://swapi.co/api/species/1](http://swapi.co/api/species/1)
- Planets：[http://swapi.co/api/planets/1](http://swapi.co/api/planets/1)

如下图表示Films提供的JSON格式数据。
![](https://img-blog.csdnimg.cn/20190131144906636.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L0Vhc3Rtb3VudA==,size_16,color_FFFFFF,t_70)
通过http访问网址 [https://swapi.co/api/films/1/](https://swapi.co/api/films/1/)  即可获取如下所示的数据，包括电影标题《A New Hope》，这是第4部，人物实体characters对应的链接。
![](https://img-blog.csdnimg.cn/20190131145431104.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L0Vhc3Rtb3VudA==,size_16,color_FFFFFF,t_70)
# 二.抓取电影films页面

接着写爬虫获取基本电影的数据，循环遍历 [https://swapi.co/api/films/7](https://swapi.co/api/films/7) 七部电影的信息。

Python3 完整代码如下：
**get_files.py**

```python
# coding: utf-8
import urllib.request as urllib2

#定义数组存所有电影
films = []

#循环写入七部电影的链接《星球大战》
for x in range(1,8):
    films.append('https://swapi.co/api/films/'+str(x)+'/')
    
#定义headers 防止网站反扒 Window系统
headers = {}
headers["User-Agent"] = "Mozilla/5.0 (Windows NT 10.0; Win64; x64) AppleWebKit/537.36 (KHTML, like Gecko) Chrome/70.0.3538.102 Safari/537.36"

#写入文件
with open('films.csv','w') as file:
    for item in films:
        print(item)
        #请求访问网站
        request = urllib2.Request(url=item, headers=headers)
        #url打开
        response = urllib2.urlopen(request)
        result = response.read().decode('utf-8')
        print(result)
        file.write(result+"\n")
```

该程序获取了7部电影的JSON信息，并存储至films.csv文件中，如下图所示：
![](https://img-blog.csdnimg.cn/20190131150128656.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L0Vhc3Rtb3VudA==,size_16,color_FFFFFF,t_70)
其中一部电影的完整信息如下：

```
HTTP 200 OK
Content-Type: application/json
Vary: Accept
Allow: GET, HEAD, OPTIONS

{
    "title": "A New Hope", 
    "episode_id": 4, 
    "opening_crawl": "It is a period of civil war.\r\nRebel spaceships, striking\r\nfrom a hidden base....", 
    "director": "George Lucas", 
    "producer": "Gary Kurtz, Rick McCallum", 
    "release_date": "1977-05-25", 
    "characters": [
        "https://swapi.co/api/people/1/", 
        "https://swapi.co/api/people/2/", 
        "https://swapi.co/api/people/3/", 
        "https://swapi.co/api/people/4/", 
        "https://swapi.co/api/people/5/", 
        "https://swapi.co/api/people/6/", 
        "https://swapi.co/api/people/7/", 
        "https://swapi.co/api/people/8/", 
        "https://swapi.co/api/people/9/", 
        "https://swapi.co/api/people/10/", 
        "https://swapi.co/api/people/12/", 
        "https://swapi.co/api/people/13/", 
        "https://swapi.co/api/people/14/", 
        "https://swapi.co/api/people/15/", 
        "https://swapi.co/api/people/16/", 
        "https://swapi.co/api/people/18/", 
        "https://swapi.co/api/people/19/", 
        "https://swapi.co/api/people/81/"
    ], 
    "planets": [
        "https://swapi.co/api/planets/2/", 
        "https://swapi.co/api/planets/3/", 
        "https://swapi.co/api/planets/1/"
    ], 
    "starships": [
        "https://swapi.co/api/starships/2/", 
        "https://swapi.co/api/starships/3/", 
        "https://swapi.co/api/starships/5/", 
        "https://swapi.co/api/starships/9/", 
        "https://swapi.co/api/starships/10/", 
        "https://swapi.co/api/starships/11/", 
        "https://swapi.co/api/starships/12/", 
        "https://swapi.co/api/starships/13/"
    ], 
    "vehicles": [
        "https://swapi.co/api/vehicles/4/", 
        "https://swapi.co/api/vehicles/6/", 
        "https://swapi.co/api/vehicles/7/", 
        "https://swapi.co/api/vehicles/8/"
    ], 
    "species": [
        "https://swapi.co/api/species/5/", 
        "https://swapi.co/api/species/3/", 
        "https://swapi.co/api/species/2/", 
        "https://swapi.co/api/species/1/", 
        "https://swapi.co/api/species/4/"
    ], 
    "created": "2014-12-10T14:23:31.880000Z", 
    "edited": "2015-04-11T09:46:52.774897Z", 
    "url": "https://swapi.co/api/films/1/"
}
```

下面补充下如何获取headers值，浏览器审查元素查看Network中的Headers实现。
![](https://img-blog.csdnimg.cn/20190131150237372.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L0Vhc3Rtb3VudA==,size_16,color_FFFFFF,t_70)
# 三.抓取所有实体页面

最后需要获取每一部电影中所有实体信息，包括以下五种实体，然后构建这些实体之间的关系。
- ‘characters’ 人物
- ‘planets’ 星球
- ‘starships’ 飞船
- ‘vehicles’ 装备
- ‘species’ 种族

这些实体可以通过遍历电影films实体中的元素，如charaters属性如下，表示一个个链接。再通过Request获取每一个实体的详细信息。
![](https://img-blog.csdnimg.cn/20190131150923551.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L0Vhc3Rtb3VudA==,size_16,color_FFFFFF,t_70)
这里补充一个知识点，从文件读取的数据是字符串格式，需要转换为字典格式，才能供JSON查询，代码如下：

```python
import json

#json.loads函数：将字符串转化为字典
json_info = '{"age": "12"}'
dict1 = json.loads(json_info)
print(str(type(json_info)), json_info)
print(str(type(dict1)), dict1)
print(dict1['age'])
```

输出结果如下：

```python
<class 'str'> {"age": "12"}
<class 'dict'> {'age': '12'}
12
```

完整代码如下所示：
**get_details.py**

```python
# coding: utf-8
import urllib.request as urllib2
import json

#设置headers
headers = {}
headers["User-Agent"] = "Mozilla/5.0 (Windows NT 10.0; Win64; x64) AppleWebKit/537.36 (KHTML, like Gecko) Chrome/70.0.3538.102 Safari/537.36"

#读取文件-读取json信息并将json格式数据转换为字典
f = open('films.csv', 'r', encoding='utf-8')
films = []
for line in f.readlines():
    #print(line)
    print(line.strip('\n'))
    line = json.loads(line.strip('\n'))
    films.append(line)
f.close()

#遍历每部电影films的实体并获取其他实体
#获取 characters人物, planets星球, starships飞船, vehicles装备, species种族
targets = ['characters', 'planets', 'starships', 'vehicles', 'species']

for target in targets:
    print(target)
    #循环获取五类数据并存储至文件
    fw = open('film_' + target + '.csv', 'w')
    data = []
    #获取7部电影信息的实体名称
    for item in films:  
        tmp = item[target]  #实体对应的链接
        print(tmp)
        for t in tmp:
            if t in data: #如果实体已经存在则跳过 比如某部电影人物另一部也出现了
                continue
            else:
                data.append(t)
            
            #循环请求直到成功 防止网络延迟
            while 1:
                try:
                    print(t)
                    request = urllib2.Request(url=t, headers=headers)
                    response = urllib2.urlopen(request)
                    result = response.read().decode('utf-8')
                except Exception as e:
                    continue #请求失败循环继续
                else:
                    fw.write(result+"\n")
                    break #请求成功跳出循环
                finally:
                    pass
                
    #查看七部电影含这种实体多少个
    print(str(len(data)), target)
    fw.close()

print("success")
```

该程序共抓取87名角色、21颗星球、37艘飞船、39架战车、37个种族，以及这228个实体之间的1112次联系。
![](https://img-blog.csdnimg.cn/20190131151744524.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L0Vhc3Rtb3VudA==,size_16,color_FFFFFF,t_70)
获取的文件数据如下所示：
![](https://img-blog.csdnimg.cn/20190131151843391.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L0Vhc3Rtb3VudA==,size_16,color_FFFFFF,t_70)![](https://img-blog.csdnimg.cn/20190131151930363.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L0Vhc3Rtb3VudA==,size_16,color_FFFFFF,t_70)
讲到这里第一篇文章叙述完毕，快去尝试吧！后续将开展分析及实体、关系、属性的提取。

PS：最近参加CSDN2018年博客评选，希望您能投出宝贵的一票。我是59号，Eastmount，杨秀璋。投票地址：[https://bss.csdn.net/m/topic/blog_star2018/index](https://bss.csdn.net/m/topic/blog_star2018/index)
![](https://img-blog.csdnimg.cn/20190104155648543.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L0Vhc3Rtb3VudA==,size_16,color_FFFFFF,t_70)
五年来写了320篇博客，12个专栏，是真的热爱分享，热爱CSDN这个平台，也想帮助更多的人，专栏包括Python、数据挖掘、网络爬虫、图像处理、C#、Android等。现在也当了两年老师，更是觉得有义务教好每一个学生，让贵州学子好好写点代码，学点技术，“师者，传到授业解惑也”，提前祝大家新年快乐。2019我们携手共进，为爱而生。

（By:Eastmount 2019-01-31 下午2点   [http://blog.csdn.net/eastmount/）](http://blog.csdn.net/eastmount/%EF%BC%89)](https://blog.csdn.net/Eastmount/article/category/5613671)](https://blog.csdn.net/Eastmount/article/category/8653277)](https://so.csdn.net/so/search/s.do?q=电影数据&t=blog)](https://so.csdn.net/so/search/s.do?q=网络爬虫&t=blog)](https://so.csdn.net/so/search/s.do?q=知识图谱&t=blog)




