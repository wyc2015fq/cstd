# Python拉勾网数据采集与可视化 - zhusongziye的博客 - CSDN博客





2018年05月08日 22:17:00[zhusongziye](https://me.csdn.net/zhusongziye)阅读数：166








全文简介

本文是先采集拉勾网上面的数据，采集的是Python岗位的数据，然后用Python进行可视化。主要涉及的是爬虫&数据可视化的知识。

爬虫部分

先用Python来抓取拉勾网上面的数据，采用的是简单好用的requests模块。主要注意的地方是，拉勾网属于动态网页，所以会用到浏览器的F12开发者工具进行抓包。抓包以后会发现，其实网页是一个POST的形式，所以要提交数据，提交的数据如下图：

![](https://img-blog.csdn.net/201805082205062?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3podXNvbmd6aXll/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)

真实网址是：

https://www.lagou.com/jobs/positionAjax.jsonneedAddtionalResult=false&isSchoolJob=0

在上图也可以轻松发现：kd是查询关键词，pn是页数，可以实现翻页。

代码实现

```python
import requests # 网络请求
import re
import time
import random

# post的网址
url = 'https://www.lagou.com/jobs/positionAjax.json?needAddtionalResult=false&isSchoolJob=0'

# 反爬措施
header = {'Host': 'www.lagou.com',
'User-Agent':'Mozilla/5.0 (Windows NT 6.1; WOW64) AppleWebKit/537.36 (KHTML, like Gecko) Chrome/63.0.3239.84 Safari/537.36',
'Accept': 'application/json, text/javascript, */*; q=0.01',
'Accept-Language': 'zh-CN,en-US;q=0.7,en;q=0.3',
'Accept-Encoding': 'gzip, deflate, br',
'Referer': 'https://www.lagou.com/jobs/list_Python?labelWords=&fromSearch=true&suginput=',
'Content-Type': 'application/x-www-form-urlencoded; charset=UTF-8',
'X-Requested-With': 'XMLHttpRequest',
'X-Anit-Forge-Token': 'None',
'X-Anit-Forge-Code': '0',
'Content-Length': '26',
'Cookie': 'user_trace_token=20171103191801-9206e24f-9ca2-40ab-95a3-23947c0b972a; _ga=GA1.2.545192972.1509707889; LGUID=20171103191805-a9838dac-c088-11e7-9704-5254005c3644; JSESSIONID=ABAAABAACDBABJB2EE720304E451B2CEFA1723CE83F19CC; _gat=1; LGSID=20171228225143-9edb51dd-ebde-11e7-b670-525400f775ce; PRE_UTM=; PRE_HOST=www.baidu.com; PRE_SITE=https%3A%2F%2Fwww.baidu.com%2Flink%3Furl%3DKkJPgBHAnny1nUKaLpx2oDfUXv9ItIF3kBAWM2-fDNu%26ck%3D3065.1.126.376.140.374.139.129%26shh%3Dwww.baidu.com%26sht%3Dmonline_3_dg%26wd%3D%26eqid%3Db0ec59d100013c7f000000055a4504f6; PRE_LAND=https%3A%2F%2Fwww.lagou.com%2F; LGRID=20171228225224-b6cc7abd-ebde-11e7-9f67-5254005c3644; index_location_city=%E5%85%A8%E5%9B%BD; TG-TRACK-CODE=index_search; SEARCH_ID=3ec21cea985a4a5fa2ab279d868560c8',
'Connection': 'keep-alive',
'Pragma': 'no-cache',
'Cache-Control': 'no-cache'}

for n in range(30):
    
    # 要提交的数据
    form = {'first':'false',
            'kd':'Python',
            'pn':str(n)}
    
    time.sleep(random.randint(2,5))
    
    # 提交数据
    html = requests.post(url,data=form,headers = header)
    
    # 提取数据
    data = re.findall('{"companyId":.*?,"positionName":"(.*?)","workYear":"(.*?)","education":"(.*?)","jobNature":"(.*?)","financeStage":"(.*?)","companyLogo":".*?","industryField":".*?","city":"(.*?)","salary":"(.*?)","positionId":.*?,"positionAdvantage":"(.*?)","companyShortName":"(.*?)","district"',html.text)
    
    # 转换成数据框
    
    data = pd.DataFrame(data)
    
    # 保存在本地
    data.to_csv(r'D:\Windows 7 Documents\Desktop\My\LaGouDataMatlab.csv',header = False, index = False, mode = 'a+')
```

注意：抓取数据的时候不要爬取太快，除非你有其他的反爬措施，比如更换IP等，另外不需登录，我在代码加入了time模块，用于限制爬取速度。

数据可视化

下载下来的数据长成这个样子：

![](https://img-blog.csdn.net/20180508220912604?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3podXNvbmd6aXll/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)

注意标题（也就是列明）是我自己添加的。

导入模块并配置绘图风格

```python
import pandas as pd # 数据框操作
import numpy as np 
import matplotlib.pyplot as plt # 绘图
import jieba # 分词
from wordcloud import WordCloud # 词云可视化
import matplotlib as mpl  # 配置字体
from pyecharts import Geo # 地理图

mpl.rcParams["font.sans-serif"] = ["Microsoft YaHei"]
# 配置绘图风格
plt.rcParams["axes.labelsize"] = 16.   
plt.rcParams["xtick.labelsize"] = 14.
plt.rcParams["ytick.labelsize"] = 14.
plt.rcParams["legend.fontsize"] = 12.
plt.rcParams["figure.figsize"] = [15., 15.]
```


注意：导入模块的时候其他都容易解决，除了wordcloud这个模块，这个模块我建议大家手动安装，如果pip安装的话，会提示你缺少C++14.0之类的错误，导致安装不上。手动下载whl文件就可以顺利安装了。

数据预览

```python
# 导入数据
data = pd.read_csv('D:\\Windows 7 Documents\\Desktop\\My\\LaGouDataPython.csv',encoding='gbk')  # 导入数据
data.head()
```

![](https://img-blog.csdn.net/20180508221137680?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3podXNvbmd6aXll/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)


read_csv路径不要带有中文

```python
data.tail()
```
![](https://img-blog.csdn.net/20180508221226822?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3podXNvbmd6aXll/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)




学历要求



```
data['学历要求'].value_counts().plot(kind='barh',rot=0)
plt.show()
```
![](https://img-blog.csdn.net/20180508221306402?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3podXNvbmd6aXll/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)

工作经验



```
data['工作经验'].value_counts().plot(kind='bar',rot=0,color='b')
plt.show()
```
![](https://img-blog.csdn.net/20180508221331105?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3podXNvbmd6aXll/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)

Python热门岗位



```
final = ''  
stopwords = ['PYTHON','python','Python','工程师','（','）','/'] # 停止词
for n in range(data.shape[0]):
    
    seg_list = list(jieba.cut(data['岗位职称'][n]))
   
    for seg in seg_list:  
        if seg not in stopwords:  
                final = final + seg + ' '
# final 得到的词汇
```
![](https://img-blog.csdn.net/20180508221424191?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3podXNvbmd6aXll/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)

工作地点

```python
data['工作地点'].value_counts().plot(kind='pie',autopct='%1.2f%%',explode = np.linspace(0,1.5,25))

plt.show()
```
![](https://img-blog.csdn.net/20180508221501350?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3podXNvbmd6aXll/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)

工作地理图




```
# 提取数据框
data2 = list(map(lambda x:(data['工作地点'][x],eval(re.split('k|K',data['工资'][x])[0])*1000),range(len(data))))
# 提取价格信息
data3 = pd.DataFrame(data2)
# 转化成Geo需要的格式
data4 = list(map(lambda x:(data3.groupby(0).mean()[1].index[x],data3.groupby(0).mean()[1].values[x]),range(len(data3.groupby(0)))))
# 地理位置展示
geo = Geo("全国Python工资布局", "制作人:挖掘机小王子", title_color="#fff", title_pos="left", width=1200, height=600,
background_color='#404a59')
attr, value = geo.cast(data4)
geo.add("", attr, value, type="heatmap", is_visualmap=True, visual_range=[0, 300], visual_text_color='#fff')
# 中国地图Python工资，此分布是最低薪资
geo
```
![](https://img-blog.csdn.net/2018050822155913?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3podXNvbmd6aXll/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)




> 
作者：挖掘机小王子

来源：：zhihu.com/people/WaJueJiPrince




