# Scipy 高级 - wsp_1138886114的博客 - CSDN博客





2018年05月25日 21:14:31[SongpingWang](https://me.csdn.net/wsp_1138886114)阅读数：772
所属专栏：[科学计算库与可视化](https://blog.csdn.net/column/details/23452.html)









### 一、scipy在线性代数应用

> 
使用 `scipy.linalg` 模块提供标准线性代数运算 

   网址：[http://docs.scipy.org/doc/scipy/reference/linalg.html#module-scipy.linalg](http://docs.scipy.org/doc/scipy/reference/linalg.html#module-scipy.linalg)

计算行列式函数：$~~~~~~~~~~~~~$`linalg.det(arr)`

  计算矩阵的逆矩阵函数：$~~~$`linalg.inv(arr)`

  奇异值分解（SVD）函数：`uarr,spec,vharr = linalg.svd(arr)`
##### 示例：

```
from scipy import linalg
import numpy as np
arr1 = np.mat(np.array([[1,2],[3,4]]))

#行列式
print('行列式:',linalg.det(arr1))  
>>> 
行列式： -2.0

    # 验证你矩阵
    # AT = linalg.inv(arr1)
    # I = AT.dot(arr1)
print('矩阵的逆矩阵:',linalg.inv(arr1))
>>>
矩阵的逆矩阵: [[-2.   1. ]
             [ 1.5 -0.5]]

#奇异值分解 
print('奇异值分解(SVD):')
uarr,spec,vharr = linalg.svd(arr)
print('first:',uarr)
print('second:',spec)
print('third:',vharr)

sarr = np.diag(spec)             #调整spec
print sarr
svd_mat = uarr.dot(sarr).dot(vharr)

print(np.allclose(svd_mat,arr))  #匹配两个矩阵是否相同
print(svd_mat)

#求解线性方程
A= np.mat("1 -2 1;0 2 -8;-4 5 9")
b = np.array([0,8,-9])
X = linalg.solve(A,b)
print (X)
>>>
[29. 16.  3.]
```

### 二、计算逆矩阵

```
import numpy as np      #矩阵 × 逆矩阵 = 单位矩阵
A = np.mat("0 1 2;1 0 3;4 3 8")

inverse = np.linalg.inv(A)
print(u"A的逆矩阵=",inverse)
>>>
A的逆矩阵= [[-0.9 -0.2  0.3]
           [ 0.4 -0.8  0.2]
           [ 0.3  0.4 -0.1]]

I = A*inverse
print("单位矩阵=A*inverse",I)
>>>
单位矩阵=A*inverse: 
[[ 1.00000000e+00  0.00000000e+00 0.00000000e+00]
 [-1.11022302e-16  1.00000000e+00 -5.55111512e-17]
 [ 0.00000000e+00  0.00000000e+00  1.00000000e+00]]
```

### 三、scipy在图像上的处理

> 
###### 使用scipy.ndimage包进行图像处理.该函数包括各种图像滤波器工具函数

用`scipy.misc` 图像**输入和输出**

   用 `misc.ascent()` 获取用于测试的图像 

   用 `ndimage.median_filter(image,size=5)`# 中值滤波 

   用 `ndimage.rotate(image,90)`旋转90度 

   用 `ndimage.prewitt(image)` 用来检测边缘 

   用`plt.imshow(filter)`显示图像
```
1 导入所需要的包
    import matplotlib.pyplot as plt
    from scipy import misc
    from scipy import ndimage
    import numpy as np

2 显示原始图像
    image = misc.ascent()  #用于测试的图像
    plt.subplot(221)       #将图片截成2行2列，现在处理第1张
    plt.title("Original Image")
    img = plt.imshow(image)
    plt.axis("off")        #不显示坐标尺寸

    plt.subplot(222)
    plt.title("midian Filter")
    filtered = ndimage.median_filter(image,size=5) # 中值滤波
    plt.imshow(filtered)
    plt.axis("off")                                #不显示坐标尺寸
    plt.subplot(223)
    plt.title("Rotated")
    Rotated = ndimage.rotate(image,90)             #旋转90度
    plt.imshow(Rotated)
    plt.axis("off")     

    plt.subplot(224)
    plt.title("prewitt Filter")
    prewitt1 = ndimage.prewitt(image)              #检测边缘
    plt.imshow(prewitt1)
    plt.axis("off") 
    plt.show
```

### 四、scipy在声音上的处理

> 
##### 模块scipy.io 中的 wavfile: 对 wav 文件进行读写

`sample_rate,data = wavfile.read('Alarm01.wav')`#获取频率及采样数据

###### 将音频重复：repeated = np.array(list(data)*2)

`wavfile.write("repeated_yabody.wav",sample_rate,repeated)`


```
1 采样：
    sample_rate,data = wavfile.read('Alarm01.wav')
    print("data type",data.dtype,"shape",data.shape)

2 显示原始音频数据  
    plt = subplot(2,1,1)             #将音频截成2行1列，处理第一列
    plt.title("Original")
    plt.plot(data)
3 将音频数据复制
    repeated = np.array(list(data)*2)
4 显示复制后的音频数据
    plt.subplot(2,1,2)
    plt.title(repeated)
    plt.plot(repeated)
    wavfile.write("repeated_yabody.wav",sample_rate,repeated)
    plt.show()
```

```
完整代码：
    from scipy import wavfile
    import matplotlib.pyplot as plt
    import numpy as np
    from scipy import signal                      #导入上述包    

    sample_rate,data = wavfile.read('Alarm01.wav')#获取频率及采样数据
    print("data type",data.dtype,"shape",data.shape)
    plt = subplot(2,1,1)              #将音频截成2行1列，处理第一列
    plt.title("Original")

    plt.plot(data)  
    repeated = np.array(list(data)*2) #将音频数据复制

    plt.subplot(2,1,2)                #显示复制后的音频数据
    plt.title(repeated)
    plt.plot(repeated)
    wavfile.write("repeated_yabody.wav",sample_rate,repeated)
    plt.show()
```

### 五、检测中国平安固件的线性趋势

> 
**1**   编写代码获取 中国平安股票的收盘价和对应的日期数据 

      利用财经数据接口包tushare获取股票的数据

**Tushape -开源python财经数据接口包安装**

```
方法1：在Anaconda Prompt控制台(py27) C:\Users\lenovo>
 执行：pip install tushare
方法2 ： 访问http://pypi.python.org/pypi/Tushare/ 下载安装
```

```
常用方法
方法1：ts.get_k_data()     #获取个股历史交易数据（包括均线数据）
      参数设置：取 日k线 周k线 月k线 以及5min,15min,30min,60min的k线
      本接口3内年日线数据，适合搭配均线数据进行选股和分析

方法2：ts.get_h_data()     #获取个股全部或指定时间段历史交易数据

方法3：ts.get_today_all()  #一次性获取当前交易所有股票行情数据（如果时节假日，即为上一交易日）

代码如下：
    today = data.today()
    delta = datatime.timedelta(days=365)
    #start1 = today - delta
    start1 = start1.strftime('%Y-%m-%d')    #开始日期转字符串
    today = today.strftime('%Y-%m-%d')      #结束日期转字符串
    ZGPA_all = ts.get_h_data('000001',start = start1,end = today)#获取中国平安股票一年的数据

    date = ZGPA_all.index       #获取索引日期（以数组的形式输出）
    close = ZGPA_all['close']   #获取指定列的数据


实现对股票金融数据的 数据采集、加工清洗、数据储存
考虑到 python pandas 包在金融量化分析优势
Tushape 返回绝大部分数据格式 pandas DataFrame数据类型
便于用pandas/Numpy/scipy/matplotlib 进行数据分析和可视化
网站：http://tushare.org/index.html

2 去除信号中的线性趋势    ：去趋势（detrend）
from scipy import signal
#去除信号中的线性趋势

去趋势函数detrend函数-scipy.signal模块中包含滤波函数和B样条插值（B-spline interpolation）函数。
以detrend函数作为滤波器例子。该函数可以对信号进行线性拟合，然后从原始输入数据中去除这个线性趋势。

y = signal.detrend(close)

3 绘制股价数据以及将去除趋势后的信号从原始数据中减去所得到的潜在趋势
    plt.plot(date,close,'o',date,close -y,'-')
        X轴date
        Y轴close
4 显示图像
    plt.show()

5 获取dataframe的某一列：close = df['close']
```

> 
**2、示例**：获取实时电影票房([http://tushare.org/index.html](http://tushare.org/index.html))


```python
import tushare as ts
df = ts.realtime_boxoffice()    #获取实时电影票房
print(df)       
#若报错提示安装下面的包      ->    统一 pip install +包名
#import pandas as pd
#import numpy as np
#import datetime

#获取中国平安股票一年的数据
import tushare as ts
from datetime import date
import datetime
import matplotlib.pyplot as plt
from scipy import signal

end1 =date.today()
interva1 = datetime.timedelta(days = 365)
begin = end1 - interval
end1 =end1.strftime('%Y-%m-%d')   #将日期转成字符串
begin =begin.strftime('%Y-%m-%d') #将日期转成字符串
df = ts.get_h_data('601318',start = begin,end = end1)
print df
close = df['close']
#print (df['close']) #获取指定列的数据
#print (df.index)    #获取索引日期（以数组的形式输出）
date1 = df.index

f = signal.detrend(close)#滤波
#print(f)
plt.plot(date1,close,'o',date1,close -y,'-')
plt.show()  

◆作业1：获取昨日排行第一电影（名称，票房，上映天数） 
ts.day_boxoffice('日期')方法获取单日电影票房的数据

import tushare as ts
from datetime import date
import datetime
import matplotlib.pyplot as plt
from scipy import signal    

def getDate(x):
    today =date.today()
    delta = datetime.timedelta(day=x)
    start1 = today -delta
    start1 = start1.strftime('%Y-%m-%d')    #将日期转成字符串
    return start1

def getWeekBoxOffice():
    start1 = getDate(3)           #星期三
    df = ts.day_boxoffice(start1)
    movieName = df(0,1).MovieName #获取排行第一的电影名称/返回series格式
    movieName = movieName(0)      #获取0号元素
    start1 = getDate(8)
    data = []
    date1 = []
    for i in np.arange(0,7):
        df = ts.day_boxoffice(start1)
        date1.append(datetime.datetime.strptime(start1,'%Y-%m-%d')+datetime,timedelta(days = 1))
        for j in np.arange(0,10): #找出昨天电影排名第一的电影在最近一周每天的排名
            test = list(df[j:j+1].MovieName)
            test = test[0]
            if test == movieName:
                pos = j
                break
        date.append(int([pos:pos+1].Boxoffice))
        k =int(8-i-1)
        delta = datetime.timedelta(day = k)
        start1 = today -delta 
        start1 = start1.strftime('%Y-%m-%d')
    return data,date1,movieName
    (data,date1,movieName) = getWeekBoxOffice()
    y = signal.detrend(data)
    plt.figure(num=1,figsize=(8,6))
    movieName = '<<'+str(movieName)+'>>中国一周销量趋势线'

    plt.title(movieName,size = '24')
    plt.plot(delta,data)
    plt.plot(delta,data,'o',detal,data - y,'-')

    plt.show()

◆作业2：获取最近10天电影票房(2018-06-01)
import tushare as ts
from scipy import signal
import numpy as np
from datetime import date
import datetime
from matplotlib import pyplot as plt
from matplotlib.font_manager import FontProperties    #处理中文包

lastDay = ts.day_boxoffice()    #获取最近一天的票房
movieDay,movieName = lastDay.MovieDay,lastDay.MovieName  
                                #获取到昨日排行前十的电影的上映天数和电影名称
targetMovieName = None          #获取需要分析的电影的名称
for name,day in zip(movieName,movieDay):  
    if int(day)>7:
        targetMovieName = name
        break                  #通过循环，找到上映天数大于7天并且排名最靠前的电影名称
 #targetMovieName              #查看获取电影的名称

def date2num(n):               #定义函数实现获取n天之前的日期的字符串类型
    today = date.today()
    delta = datetime.timedelta(days=n)
    start = today - delta 
    return start.strftime("%Y-%m-%d")

targetBoxOffice = []                   #存放需要分析的电影的近10天票房数据
dates = []                             #存放近10天日期的字符串
for i in range(0,9):
    start = date2num(i+1)
    dates.append(start)                #将日期添加到日期列表中
    day = ts.day_boxoffice(start)      #循环获取前10天电影票房的排行信息
    dayMovieName = day.MovieName       #MovieName为数据表头
    dayBoxOffice = day.BoxOffice
    for name,boxOffice in zip(dayMovieName,dayBoxOffice):
        if name == targetMovieName:   #判断每日票房排行中的电影名称是否为需要分析的电影
            targetBoxOffice.append(boxOffice)
print(targetBoxOffice)
>>>['718', '1184', '1041', '1108', '1192', '3969', '5058', '2296', '1851']

targetBoxOffice = np.array(targetBoxOffice,dtype=np.float64) #将字符串转成浮点型
dates = np.array(dates)
newDates = dates[::-1]                 #切片倒序数组
newBoxOffice = targetBoxOffice[::-1]   #切片倒序数组
y = signal.detrend(newBoxOffice)       #添加趋势线

plt.figure(num=1, figsize=(10, 8))     #设置绘图参数
font_set = FontProperties(fname=r"c:\windows\fonts\simsun.ttc", size=15)
title = "《"+targetMovieName+"》近十天票房走势"
plt.title(title,size=25,fontproperties=font_set)
plt.plot(newDates,newBoxOffice,"-",newDates,newBoxOffice-y,"-")
```

![这里写图片描述](https://img-blog.csdn.net/20180601111741799?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3dzcF8xMTM4ODg2MTE0/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)








