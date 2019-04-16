# [知识图谱实战篇] 二.Json+Seaborn可视化展示电影实体 - 杨秀璋的专栏 - CSDN博客





2019年02月01日 13:20:40[Eastmount](https://me.csdn.net/Eastmount)阅读数：272
所属专栏：[知识图谱、web数据挖掘及NLP](https://blog.csdn.net/column/details/eastmount-kgdmnlp.html)









前面作者讲解了很多知识图谱原理知识，包括知识图谱相关技术、Neo4j绘制关系图谱等，但仍缺少一个系统全面的实例。为了加深自己对知识图谱构建的认识，为后续创建贵州旅游知识图谱打下基础，作者深入学习了张宏伦老师的网易云课程（[星球系列电影](https://study.163.com/course/courseLearn.htm?courseId=1003528010)），并结合自己的理解和技术分享了该系列专栏，从数据采集、数据展示、数据分析到知识图谱构建，文章后续还会讲解中文数据的实体识别、关系抽取、知识计算等。

前文介绍了Python3抓取电影实体知识，这篇文章主要讲解Python+Seaborn可视化展示电影信息，它首先需要通过文件读取获取JSON所需数据，再绘制相关图形。这是一系列基础性文章，希望对您有所帮助 ，尤其是对知识图谱感兴趣和编程刚入门的同学。同时也因为最近准备博士考试，做题做吐了，写点新东西调节下心情，与君共勉，一起加油。

代码下载地址：[https://download.csdn.net/download/eastmount/10955326](https://download.csdn.net/download/eastmount/10955326)

**前文：**
[[知识图谱实战篇] 一.数据抓取之Python3抓取JSON格式的电影实体](https://blog.csdn.net/eastmount/article/details/86714051)





### 文章目录
- [一.柱状图对比电影实体数量](#_42)
- [二.散点图显示不同性别的身高体重](#_196)
- [三.Seaborn可视化展示](#Seaborn_283)





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
# 一.柱状图对比电影实体数量

《星球大战》电影共七部，实体包括characters人物、planets星球、starships飞船、vehicles装备、species种族。接下来准备通过柱状图对比各部电影不同实体的数量。

**1.提取各电影各实体的数量**

七部电影存储至 films.csv 文件中，其中一部电影JSON格式如下。

```python
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

通过下面的Python3代码获取各部电影的实体数量。
**get_jsonfilms.py**

```python
import json

#读取数据
fr = open("films.csv", "r")

#写文件
fw = open("stat_basic.csv", "w")
fw.write('title,key,value'+'\n')

#统计每部电影中出现各类实体多少个
for line in fr:
    #tmp['title']获取电影名称
    tmp = json.loads(line.strip('\n'))
    fw.write(tmp['title'] + ',' + 'characters' + ',' + str(len(tmp['characters'])) + '\n')
    fw.write(tmp['title'] + ',' + 'planets' + ',' + str(len(tmp['planets'])) + '\n')
    fw.write(tmp['title'] + ',' + 'starships' + ',' + str(len(tmp['starships'])) + '\n')
    fw.write(tmp['title'] + ',' + 'vehicles' + ',' + str(len(tmp['vehicles'])) + '\n')
    fw.write(tmp['title'] + ',' + 'species' + ',' + str(len(tmp['species'])) + '\n')

fr.close()
fw.close()
```

运行结果如下图所示：
![](https://img-blog.csdnimg.cn/20190201134934835.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L0Vhc3Rtb3VudA==,size_16,color_FFFFFF,t_70)
**2.柱状图绘制**

下面这段代码是获取 stat_basic.csv 文件中各部电影各实体的数量。重点是调用barplot()函数，参数x表示实体、y表对应数量、hue是不同类别的电影、data是读取的数据，sns.color_palette(“hls”, 8) 是最常见的颜色空间，8中以上的颜色。
**show_hist.py**
```python
# -*- coding: utf-8 -*-
import matplotlib.pyplot as plt  
import seaborn as sns
import pandas as pd

#获取数据
films_data = pd.read_csv('stat_basic.csv')  
fig, ax = plt.subplots(1,1)
print(films_data['key'])

#设置绘图风格
sns.set_style("whitegrid")

#最常见的是使用hls颜色空间
sns.palplot(sns.color_palette('hls',8))

#绘制柱状图
sns.barplot(x="key", y="value", hue="title", data=films_data, ax=ax,
            palette=sns.color_palette("hls", 8))

#设置Axes的标题
ax.set_title('Star Wars Entity Statistics')

plt.show()
```

绘制图形如下所示：
![](https://img-blog.csdnimg.cn/20190201140414412.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L0Vhc3Rtb3VudA==,size_16,color_FFFFFF,t_70)
如果使用下面这段代码，将绘制的横轴为各电影。
![](https://img-blog.csdnimg.cn/20190201140536263.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L0Vhc3Rtb3VudA==,size_16,color_FFFFFF,t_70)
推荐两篇文章：
[Python数据可视化-seaborn](https://www.cnblogs.com/gczr/p/6767175.html)
[python数据可视化seaborn（一）—— 整体样式与调色板](https://blog.csdn.net/ICERON/article/details/85088582)
# 二.散点图显示不同性别的身高体重

接下来获取 film_characters.csv 人物文件中，各人物尸体的身高、体重、性别、家乡，然后统计不同性别的身高-体重图。

**1.获取人物属性**

Python3完整代码如下：
**get_jsondetails.py**
```python
#-*- coding: utf-8 -*-
import json

#获取人物的身高和体重的关系
fr = open("film_characters.csv", "r")
fw = open("stat_character.csv", "w")

#姓名 身高 体重 性别 家乡
fw.write('name,height,mass,gender,homeworld'+'\n')

for line in fr:
    print(line.strip('\n'))
    tmp = json.loads(line.strip('\n'))
    #身高体重值unknown设置为-1
    if tmp['height'] == 'unknown':
        tmp['height'] = '-1'
    if tmp['mass'] == 'unknown':
        tmp['mass'] = '-1'
    if tmp['gender'] == 'n/a':
        tmp['gender'] = 'none'
    fw.write(tmp['name'] + ',' + tmp['height'] + ',' + tmp['mass'] + ',' + tmp['gender'].strip() + ',' + tmp['homeworld'] + '\n')
    
fr.close()
fw.close()
```

输出结果如下图所示：
![](https://img-blog.csdnimg.cn/20190201141447602.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L0Vhc3Rtb3VudA==,size_16,color_FFFFFF,t_70)
**2.matplotlib绘制散点图**

该代码的难点是数据集中横坐标为height，纵坐标为mass，而不同性别的颜色不同。包括四种性别：男性、女性、雌雄同体、未知。

PS：读取的数据中可能有异常值，需要进行检查下。
**show_scatter.py**

```python
# -*- coding: utf-8 -*-
import matplotlib.pyplot as plt  
import pandas as pd

#获取数据
df = pd.read_csv('stat_character.csv')  
fig = plt.figure()
axes = fig.add_subplot(111)

#显示数据
print(df["height"])
print(df["mass"])

height = df["height"]
mass = df["mass"]
sex = df["gender"]

for i in range(len(sex)):
    if sex[i] == "male":
        axes.scatter(height[i], mass[i], color = 'red', s=100, marker='o')
    if sex[i] == "female":
        axes.scatter(height[i], mass[i], color = 'green', s=100, marker='s')
    if sex[i] == "hermaphrodite":
        axes.scatter(height[i], mass[i], color = 'black', s=100, marker='*')
    if sex[i] == "none":
        axes.scatter(height[i], mass[i], color = 'blue', s=200, marker='p')
        
plt.xlabel('height')
plt.ylabel('mass')
plt.show()
```

绘制结果如下图所示，最下面一层为 height 为-1的散点。
![](https://img-blog.csdnimg.cn/20190201142247278.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L0Vhc3Rtb3VudA==,size_16,color_FFFFFF,t_70)
# 三.Seaborn可视化展示

最后讲解Seaborn库分析人物身高和体重相关信息 ，代码如下，核心函数：

sns.jointplot()
- x=df[‘A’]  #设置x轴
- y=df[‘B’]  #设置y轴
- data=df  #设置数据
- color=‘b’ #设置颜色
- s=50  #设置散点大小，参数类型：numeric
- edgecolor=‘w’  #设置散点边缘颜色，默认无色，w为白色
- linewidth=1  #设置散点宽度(只针对scatter)，参数类型：numeric
- kind=‘scatter’  #设置类型：‘scatter’,‘reg’,‘resid’,‘kde’,‘hex’
- space=0.1  #设置散点图和布局图的间距
- size=8  #设置图表大小(自动调整为正方形)，默认6
- ratio=5  #中心图与侧边图的比例，越大中心图占比越大
- marginal_kws=dict(bins=15, rug =True)  #设置柱状图箱数，是否设置rug
- annot_kws  #注释的信息
- {x, y}lim   #x、y轴的范围，参数类型two-tuples

安装如下：pip install seaborn
![](https://img-blog.csdnimg.cn/20190201144553802.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L0Vhc3Rtb3VudA==,size_16,color_FFFFFF,t_70)
**show_height_mass.py**

**1.散点图**

```python
# -*- coding: utf-8 -*-
import matplotlib.pyplot as plt  
import seaborn as sns
import pandas as pd

#读取数据
df = pd.read_csv('stat_character.csv')  
fig, ax = plt.subplots(1,1)

#name height mass gender homeworld
#散点图
sns.jointplot(x="height", y="mass", data=df, color="b", s=50, kind='scatter',
              space = 0.1, size = 8, ratio = 5)
plt.show()
```

输出如下图所示：
![](https://img-blog.csdnimg.cn/20190201144453230.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L0Vhc3Rtb3VudA==,size_16,color_FFFFFF,t_70)
**2.回归图**

```python
# -*- coding: utf-8 -*-
import matplotlib.pyplot as plt  
import seaborn as sns
import pandas as pd

#读取数据 name height mass gender homeworld
df = pd.read_csv('stat_character.csv')  
fig, ax = plt.subplots(1,1)

#散点图
#sns.jointplot(x="height", y="mass", data=df, color="b", s=50, kind='scatter',
#              space = 0.1, size = 8, ratio = 5)

#回归图
sns.jointplot(x="height", y="mass", data=df, color="b", kind='reg')

plt.show()
```

输出结果如下图所示，可以看到拟合的直线及区间。
![](https://img-blog.csdnimg.cn/20190201145132634.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L0Vhc3Rtb3VudA==,size_16,color_FFFFFF,t_70)
**3.六角形**

```python
# -*- coding: utf-8 -*-
import matplotlib.pyplot as plt  
import seaborn as sns
import pandas as pd

#读取数据 name height mass gender homeworld
df = pd.read_csv('stat_character.csv')  
fig, ax = plt.subplots(1,1)

#散点图
#sns.jointplot(x="height", y="mass", data=df, color="b", s=50, kind='scatter',
#              space = 0.1, size = 8, ratio = 5)

#回归图
#sns.jointplot(x="height", y="mass", data=df, color="b", kind='reg')

#六角形
sns.jointplot(x="height", y="mass", data=df, color="b", kind='hex')

plt.show()
```

输出结果如下图所示：
![](https://img-blog.csdnimg.cn/20190201145432402.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L0Vhc3Rtb3VudA==,size_16,color_FFFFFF,t_70)
**4.KDE图**

```python
# -*- coding: utf-8 -*-
import matplotlib.pyplot as plt  
import seaborn as sns
import pandas as pd

#读取数据 name height mass gender homeworld
df = pd.read_csv('stat_character.csv')  
fig, ax = plt.subplots(1,1)

#散点图
#sns.jointplot(x="height", y="mass", data=df, color="b", s=50, kind='scatter',
#              space = 0.1, size = 8, ratio = 5)

#回归图
#sns.jointplot(x="height", y="mass", data=df, color="b", kind='reg')

#六角形
#sns.jointplot(x="height", y="mass", data=df, color="b", kind='hex')

#KDE 图
sns.jointplot(x="height", y="mass", data=df, kind="kde", space=0, color="#6AB27B")

plt.show()
```

输出结果如下图所示：
![](https://img-blog.csdnimg.cn/20190201145603935.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L0Vhc3Rtb3VudA==,size_16,color_FFFFFF,t_70)
**5.散点图+KDE 图**

```python
# -*- coding: utf-8 -*-
import matplotlib.pyplot as plt  
import seaborn as sns
import pandas as pd

#读取数据 name height mass gender homeworld
df = pd.read_csv('stat_character.csv')  
fig, ax = plt.subplots(1,1)

#散点图
#sns.jointplot(x="height", y="mass", data=df, color="b", s=50, kind='scatter',
#              space = 0.1, size = 8, ratio = 5)

#回归图
#sns.jointplot(x="height", y="mass", data=df, color="b", kind='reg')

#六角形
#sns.jointplot(x="height", y="mass", data=df, color="b", kind='hex')

#KDE 图
#sns.jointplot(x="height", y="mass", data=df, kind="kde", space=0, color="#6AB27B")

#散点图+KDE 图
g = (sns.jointplot(x="height", y="mass", data=df, color="k")
      .plot_joint(sns.kdeplot, zorder=0, n_levels=6))

plt.show()
```

输出结果如下图所示：
![](https://img-blog.csdnimg.cn/20190201145955810.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L0Vhc3Rtb3VudA==,size_16,color_FFFFFF,t_70)
**6.pairplot**

```python
# -*- coding: utf-8 -*-
import matplotlib.pyplot as plt  
import seaborn as sns
import pandas as pd

#读取数据 name height mass gender homeworld
df = pd.read_csv('stat_character.csv')  
fig, ax = plt.subplots(1,1)

#散点图
#sns.jointplot(x="height", y="mass", data=df, color="b", s=50, kind='scatter',
#              space = 0.1, size = 8, ratio = 5)

#回归图
#sns.jointplot(x="height", y="mass", data=df, color="b", kind='reg')

#六角形
#sns.jointplot(x="height", y="mass", data=df, color="b", kind='hex')

#KDE 图
#sns.jointplot(x="height", y="mass", data=df, kind="kde", space=0, color="#6AB27B")

#散点图+KDE 图
#g = (sns.jointplot(x="height", y="mass", data=df, color="k")
#      .plot_joint(sns.kdeplot, zorder=0, n_levels=6))

sns.pairplot(df)
            
plt.show()
```

输出如下图所示：
![](https://img-blog.csdnimg.cn/20190201150257780.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L0Vhc3Rtb3VudA==,size_16,color_FFFFFF,t_70)
**show_height_mass2.py**

```python
# -*- coding: utf-8 -*-
import matplotlib.pyplot as plt  
import seaborn as sns
import pandas as pd

#读取数据 name height mass gender homeworld
df = pd.read_csv('stat_character.csv')  
fig, ax = plt.subplots(1,1)

#绘制矩阵散点图 pairplot
g = sns.pairplot(df,                         #数据
                 vars = ['height', 'mass'], #获取数据列
                 kind = 'reg',                  #散点图/回归分布图{'scatter', 'reg'})
                 diag_kind = 'kde',         #直方图/密度图{'hist'， 'kde'}
                 hue = 'gender',              #按照某一字段进行分类
                 palette = 'husl'              #设置调色板
                 )
          
plt.show()
```

运行结果如下图所示：
![](https://img-blog.csdnimg.cn/20190201151255143.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L0Vhc3Rtb3VudA==,size_16,color_FFFFFF,t_70)
```python
# -*- coding: utf-8 -*-
import matplotlib.pyplot as plt  
import seaborn as sns
import pandas as pd

#读取数据 name height mass gender homeworld
df = pd.read_csv('stat_character.csv')  
fig, ax = plt.subplots(1,1)

"""
#绘制矩阵散点图 pairplot
g = sns.pairplot(df,                         #数据
                 vars = ['height', 'mass'], #获取数据列
                 kind = 'reg',                  #散点图/回归分布图{'scatter', 'reg'})
                 diag_kind = 'kde',         #直方图/密度图{'hist'， 'kde'}
                 hue = 'gender',              #按照某一字段进行分类
                 palette = 'husl'              #设置调色板
                 )
"""

sns.pairplot(df, kind = 'scatter',diag_kind = 'hist', hue = 'gender',
             palette = 'husl', markers = ['o', 's', 'D', '*'], size = 2)
plt.show()
```
![](https://img-blog.csdnimg.cn/20190201151652570.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L0Vhc3Rtb3VudA==,size_16,color_FFFFFF,t_70)
最后推荐几篇文章供大家学习：
[python可视化进阶—seaborn1.4 分布数据可视化 - 散点图 jointplot() / pairplot()](https://blog.csdn.net/qq_42554007/article/details/82625118)
[Seaborn-04-Jointplot两变量图](https://www.jianshu.com/p/2da7668abd8c)
本文代码下载地址：[https://download.csdn.net/download/eastmount/10955326](https://download.csdn.net/download/eastmount/10955326)

PS：最近参加CSDN2018年博客评选，希望您能投出宝贵的一票。我是59号，Eastmount，杨秀璋。投票地址：[https://bss.csdn.net/m/topic/blog_star2018/index](https://bss.csdn.net/m/topic/blog_star2018/index)
![](https://img-blog.csdnimg.cn/20190104155648543.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L0Vhc3Rtb3VudA==,size_16,color_FFFFFF,t_70)
五年来写了320篇博客，12个专栏，是真的热爱分享，热爱CSDN这个平台，也想帮助更多的人，专栏包括Python、数据挖掘、网络爬虫、图像处理、C#、Android等。现在也当了两年老师，更是觉得有义务教好每一个学生，让贵州学子好好写点代码，学点技术，“师者，传到授业解惑也”，提前祝大家新年快乐。2019我们携手共进，为爱而生。
![](https://img-blog.csdnimg.cn/20190201151929796.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L0Vhc3Rtb3VudA==,size_16,color_FFFFFF,t_70)![](https://img-blog.csdnimg.cn/20190201151941901.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L0Vhc3Rtb3VudA==,size_16,color_FFFFFF,t_70)
**随笔**

又是十一点归家，十五天，我看完两本专业书，还差四本专业书+四本英语书，女神也陪我看了两本文学书。谢谢你的陪伴。不过真的快看吐了，浏览了下博友的回复和邮件，很温暖，深夜赶紧写点代码和文章舒缓下心情。

假期的每一次启程，都重温着忐忑和期待交织的兴奋，每一次归来，都是满满的力量回归。只要沿着路向前走，时间、自然、书卷会赐予你最好的风光。哪怕有风雨，反正怎么都美。只要在路上，在奋斗中你还会不断地认识自己，原来认识自己，也是一个漫长的过程呀！

但远方的她笑起来了，眼睛里散发着的光芒将周遭的一切都涂抹上甜蜜的色调。

面对未知，我能做的就是爱你和向前走，与君共勉。
最后分享一段女神的读后感，相互勉励。

一直以来对萧红的认识仅限于大学时老师的介绍和对她叛逆而又颠沛的一生的好奇，看过电影《黄金时代》后，汤唯抽着烟伏案写作的样子大概就固化成了我对她的印象吧。而今真正读了她的作品，对其精神世界的浪漫与冷漠才开始略有所感，也才认识到，为何鲁迅会对她如此看重，评价甚高。

呼兰河，一个普通的北方小城，在萧红细腻的笔触下栩栩如生又死气沉沉，翻开书来，小城的街道、河流、各色人等一一出场，眼前即是这被寒冬冻裂了的大地，有着大泥坑子的东二道街上，叫卖声、拉马声、嚷着去看戏的嘈杂声，大地是白的、房檐是灰的，人们突突地呼着热气唠着哪家又跳大神了哪家的老婆又跳井死了，感慨唏嘘一番，一觉醒来日子照旧，仿佛那死掉的人不曾来过一般。热闹非凡又透着麻木与冷漠，看戏的人只怕和鲁迅笔下《示众》里的人是同一种人吧。

呼兰河是萧红的故乡，也是她打小看惯世态炎凉的地方，她回忆家里荒凉的园子和祖父的宠爱，写呼兰河的生生死死日复一日，笔触淡定，娓娓道来，但荒凉感却是力透纸背溢进人心了。

“呼兰河的人们就是这样，冬天来了就穿棉衣裳，夏天来了就穿单衣裳。就好像太阳出来了就起来，太阳落了就睡觉似的。”“春夏秋冬，一年四季来回循环的走。风霜雨雪，受得住的就过去了，受不住的就寻求着自然的结果。那自然的结果不大好，把一个人默默的一声不响的就拉着离开了这人间的世界了。”

被作家记录下来的小城，平平无奇，无奇到你甚至以为那似乎也曾是你生活过的村庄。

（By:Eastmount 2019-02-01 下午2点   [http://blog.csdn.net/eastmount/）](http://blog.csdn.net/eastmount/%EF%BC%89)

`












