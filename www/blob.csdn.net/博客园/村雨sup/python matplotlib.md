# python matplotlib - 村雨sup - 博客园








[博客园](https://www.cnblogs.com/)[首页](https://www.cnblogs.com/cunyusup/)[新随笔](https://i.cnblogs.com/EditPosts.aspx?opt=1)[联系](https://msg.cnblogs.com/send/%E6%9D%91%E9%9B%A8sup)[管理](https://i.cnblogs.com/)[订阅](https://www.cnblogs.com/cunyusup/rss)![订阅](https://www.cnblogs.com/images/xml.gif)





安装过程

下载Anaconda3,安装完成后发现直接在pycharm中不能导入matplotlib和numpy

解决方法：File->setting->project interpreter 点击右上角的小齿轮，选择add local,寻找Anaconda3中python.exe

![](https://images2017.cnblogs.com/blog/1196023/201708/1196023-20170815221253631-1346063542.png)



默认位置应该在这儿：

![](https://images2017.cnblogs.com/blog/1196023/201708/1196023-20170815221521037-829726879.png)

（有的时候会出现ProgramData 找不到的情况，请参考我下一篇文章。）

最后点击apply就可以了

第一课

绘制散点图

```
import matplotlib.pyplot as plt
import numpy as np

# x = [1,2,3,4,5]
# x1 = np.array(x)   将列表x化为np对象（暂时不知道有什么用）

# x = np.random.randn(1000)  随机数
# y = np.random.randn(1000)

power,money,ability = np.loadtxt("01.csv",skiprows=1,usecols=(1,2,4),unpack=True,delimiter=" ")
#跳过行，行是从1开始数的。使用列，列是从0开始数的。delimiter文件中的数据分割标志
#从文件中获取数据，unpack默认为False，改为True时能够将数据导入多个变量中


plt.scatter(x,y,edgecolors='red',c='g',s=20,marker='+')  #scatter 绘制散点图
plt.show()
```

绘制折线图

```
plt.plot(da,open,linestyle='-',color='red',marker='o')
#默认plot为折线图
```

绘制条状图（可以绘制在一起）

```
da = [1,2,3,4,5]
open = [3,4,2,1,5]
index = np.arange(5)
plt.bar(index,da,width=0.5)
plt.bar(index+0.5,open,width=0.5)#index+0.5是平移0.5避免重合
```

条状图叠在一起

```
da = [1,2,3,4,5]
open = [3,4,2,1,5]
index = np.arange(5)
plt.bar(index,da,width=0.5)
plt.bar(index,open,width=0.5,bottom=da)#index+0.5是平移0.5避免重合
```

绘制直方图

```
array1 = np.random.randn(20000)#中心在0的满足正态分布的20000个随机变量
plt.hist(array1,bins=100,normed=False)#bins横坐标的分组数，normed是否对数据进行标准化（频率or出现次数）
plt.show()
```

绘制2d直方图

颜色越深代表概率越大

```
array2 = np.random.randn(20000)+2
array3 = np.random.randn(20000)+3

plt.hist2d(array2,array3,bins=40)
plt.show()
```

![](https://images2017.cnblogs.com/blog/1196023/201708/1196023-20170816212255600-1910862288.png)好漂酿哦！

饼状图

```
label = 'A','B','C','D'
value = [4,5,7,9]

distance = [0,0.05,0,0]  #突出部分B被选为突出0.05

plt.pie(x=value,labels=label,autopct='%.0f%%',shadow=True,explode=distance)
                            #选择是否显示百分比，选择是否显示阴影，选择突出的部分

plt.show()
```

![](https://images2017.cnblogs.com/blog/1196023/201708/1196023-20170816214010209-244013826.png)



附：常用颜色blue/green/red/cyan/magenta/yellow/black/white

       线段样式：实线  -  虚线  --  点画线  -.  点线  ：

       常用23个点的样式：百度



      样式字符串：一个很pythonic的写法    颜色 点样式 线段样式

      eg.   cx--  mo:













