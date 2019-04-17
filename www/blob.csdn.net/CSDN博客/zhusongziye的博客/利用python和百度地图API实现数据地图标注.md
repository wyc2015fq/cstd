# 利用python和百度地图API实现数据地图标注 - zhusongziye的博客 - CSDN博客





2018年05月18日 22:15:48[zhusongziye](https://me.csdn.net/zhusongziye)阅读数：4658








如题，先上效果图：

![](https://img-blog.csdn.net/20180518221334281?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3podXNvbmd6aXll/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)

主要分为两大步骤
- 
使用python语句，通过百度地图API，对已知的地名抓取经纬度

- 
使用百度地图API官网的html例程，修改数据部分，实现呈现效果


## 一、使用python语句，通过百度地图API，获取经纬度
- 
### 读取文件信息


```
import pandas as pd
data = pd.read_excel('test_baidu.xlsx')
data
```

![](https://img-blog.csdn.net/20180518221354131?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3podXNvbmd6aXll/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)

图中可以看出，原始数据并没有经纬度。

### 2. 构建抓取经纬度函数

```
import json
from urllib.request import urlopen, quote
import requests
def getlnglat(address):
    url = 'http://api.map.baidu.com/geocoder/v2/'
    output = 'json'
    ak = '你的百度地图ak' # 百度地图ak，具体申请自行百度，提醒需要在“控制台”-“设置”-“启动服务”-“正逆地理编码”，启动
    address = quote(address) # 由于本文地址变量为中文，为防止乱码，先用quote进行编码
    uri = url + '?' + 'address=' + address  + '&output=' + output + '&ak=' + ak 
    req = urlopen(uri)
    res = req.read().decode() 
    temp = json.loads(res)
    lat = temp['result']['location']['lat']
    lng = temp['result']['location']['lng']
    return lat,lng   # 纬度 latitude   ，   经度 longitude  ，
```

#### 3. 抓取经纬度

```
for indexs in data.index:
    get_location = getlnglat(data.loc[indexs,'圈定区域'])
    get_lat = get_location[0]
    get_lng = get_location[1]
    data.loc[indexs,'纬度'] = get_lat
    data.loc[indexs,'经度'] = get_lng

data
```

![](https://img-blog.csdn.net/20180518221406250?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3podXNvbmd6aXll/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)

已经自动查找到对应的经纬度（不排除部分搜索不准，但是大概看过，还是挺准的。）

#### 4. 生成HTML适配的格式

```
data_html = pd.DataFrame(columns=['content'])

for indexs in data.index:
    data_html.loc[indexs,'content'] = '{' + \
                                      '"lat":' + str(data.loc[indexs,'纬度']) + ',' +  \
                                      '"lng":' + str(data.loc[indexs,'经度']) + ',' +  \
                                      '"quyu":' + '"' + str(data.loc[indexs,'圈定区域']) +'"' +   \
                                      '}' + ','

data_html.to_csv ("data_html.csv",encoding="gbk")
data_html
```

![](https://img-blog.csdn.net/20180518221415660?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3podXNvbmd6aXll/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)

生成对应的格式，然后就copy出来了。

## 二、使用百度地图API官网的html例程，修改数据部分，实现呈现效果

### 1. copy百度地图API官网的HTML例程

地址：http://developer.baidu.com/map/jsdemo.htm#c1_19





![](https://img-blog.csdn.net/20180518221424673?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3podXNvbmd6aXll/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)

### 2. 修改部分内容
- 
放上自己的百度地图AK

- 
修改一下地图初始化显示的中心和缩放的系数

- 
放上自己需要显示的信息，记得把数据中最后一个的逗号删除


修改前：

![](https://img-blog.csdn.net/20180518221436653?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3podXNvbmd6aXll/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)

修改后：

![](https://img-blog.csdn.net/20180518221447206?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3podXNvbmd6aXll/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)

题外：

其实一开始是使用folium进行尝试的，因为可以直接生成html文件，只需要Python就行了，不需要再写HTML，但是遇到两个暂时无法解决的问题：

1、marker不能正常显示，官网的marker都不行




![](https://img-blog.csdn.net/2018051822150636?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3podXNvbmd6aXll/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)

2、openstreetmap的细致程度，比不上百度地图，这个有点致命。

![](https://img-blog.csdn.net/20180518221521586?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3podXNvbmd6aXll/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)作者：StarsOcean

链接：https://www.jianshu.com/p/1a9531d2f153



