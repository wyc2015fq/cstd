# 六行python代码的爱心曲线 - 我相信...... - CSDN博客





2017年04月10日 06:49:38[半吊子全栈工匠](https://me.csdn.net/wireless_com)阅读数：32974标签：[python																[可视化](https://so.csdn.net/so/search/s.do?q=可视化&t=blog)](https://so.csdn.net/so/search/s.do?q=python&t=blog)
个人分类：[编程语言																[数据挖掘与分析](https://blog.csdn.net/wireless_com/article/category/1510743)](https://blog.csdn.net/wireless_com/article/category/2540051)










[喔家ArchiSelf](https://mp.weixin.qq.com/s?__biz=MzAwOTcyNzA0OQ==&mid=2658971475&idx=1&sn=06626199bcb1762aaaea9bc0b2f35d09&chksm=80d33275b7a4bb632903368f012358e1116fa4ec543834025c0ba6c71dabe1aebb8e53fad0d7#%23)


前些日子在做绩效体系的时候，遇到了一件囧事，居然忘记怎样在Excel上拟合正态分布了，尽管在第二天重新拾起了Excel中那几个常见的函数和图像的做法，还是十分的惭愧。实际上，当时有效偏颇了，忽略了问题的本质，解决数据分析和可视化问题，其实也是Python的拿手好戏。




 例如，画出指定区间的一个多项式函数：

![](https://img-blog.csdn.net/20170409150957183?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvd2lyZWxlc3NfY29t/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)


Python 代码如下：

```
import numpy as np
import matplotlib.pyplot as plt
X = np.linspace(-4, 4, 1024)
Y = .25 * (X + 4.) * (X + 1.) * (X - 2.)
plt.title('$f(x)=\\frac{1}{4}(x+4)(x+1)(x-2)$')
plt.plot(X, Y, c = 'g')
plt.show()
```

通过numpy的linspace方法来确定横坐标x的取值范围，列出方程，然后调用matplotlib的pyplot画出函数曲线即可。numpy 是一个用python实现的科学计算包，包括一个强大的N维数组对象Array和成熟的函数库，有用于整合C/C++和Fortran代码的工具包，提供了实用的线性代数、傅里叶变换和随机数生成函数等工具，可以理解成Matlab。

记得中学的时候，我问老师三角函数到底有啥用？（无知者无畏）老师反问我，“如果给了你一块洋铁，怎样才能剪出煤炉烟囱的拐弯呢？”，现在仍然记得老师的这个例子，哪些看似抽象的数学公式，实际上是自己不知道她们的应用场景而已。 

![](https://img-blog.csdn.net/20170409151027339?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvd2lyZWxlc3NfY29t/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)


Python代码如下：

```
import numpy as np
import matplotlib.pyplot as plt
X = np.linspace(0, 2 * np.pi, 100)
YSinValues = np.sin(X)
YCosValues = np.cos(X)
plt.plot(X, YSinValues)
plt.plot(X, YCosValues)
plt.show()
```

Matplotlib 是一个 Python 的 2D绘图库，甚至可以生成出版质量级别的图形。

对于那些正态分布而言，Python 画起来也就相当简单了：

```
import numpy as np
import matplotlib.pyplot as plt
def pdf(X, mu, sigma):
    a = 1. / (sigma * np.sqrt(2. * np.pi))
    b = -1. / (2. * sigma ** 2)
    return a * np.exp(b * (X - mu) ** 2)
X = np.linspace(-6, 6, 1000)
for i in range(3):
    samples = np.random.standard_normal(10)
    mu, sigma = np.mean(samples), np.std(samples)
    plt.plot(X, pdf(X, mu, sigma), color = '.66')
plt.plot(X, pdf(X, 0., 1.), color = 'b')
plt.show()
```

为了不显得单调，这里多画了几条曲线。只要算出方差和均值，从excel中读出哪些数值就可以拟合正态分布了。

![](https://img-blog.csdn.net/20170409151056402?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvd2lyZWxlc3NfY29t/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)







回归到主题，关于爱心线，有这样一个凄美的爱情故事。

> 
迩来流浪于吴越，一片闲云空皎洁。


300多年前，斯德哥尔摩的街头，落魄的笛卡尔过着乞讨的生活，全部的财产破破烂烂的衣服和随身所带的几本数学书籍。清高的笛卡尔没有开口请求路人施舍，只是默默地低头在纸上写写画画，潜心于他的数学世界。 一个宁静的午后，笛卡尔照例坐在街头的阳光中研究数学问题，身边过往的人群，喧闹的车马队伍，都无法对他造成干扰。

> 
有美一人，婉如清扬。邂逅相遇，与子偕臧。 


　　“你在干什么呢？”扭过头，笛卡尔看到一张年轻秀丽的睑庞，一双清澈的眼睛如湛蓝的湖水，楚楚动人，她就是瑞典的小公主，国王最宠爱的女儿克里斯汀。 她蹲下身，拿过笛卡尔的数学书和草稿纸，和他交谈起来。言谈中，他发现，这个小女孩思维敏捷，对数学有着浓厚的兴趣。 

　　几天后，笛卡尔意外地接到通知，国王聘请他做小公主的数学老师。满心疑惑的笛卡尔跟随侍卫一起来到皇宫，他听到了从远处传来的银铃般的笑声。他看到了那天在街头偶遇的女孩子，从此，他当上了公主的数学老师。 　　

> 
情不知所起,一往而深


　　公主的数学在笛卡尔的悉心指导下突飞猛进，他们之间也开始变得亲密起来。笛卡尔向她介绍了直角坐标系，代数与几何可以结合起来，也就是日后笛卡尔创立的解析几何学雏形。 在笛卡尔的带领下，克里斯汀走进了奇妙的坐标世界，她对曲线着了迷。每天的形影不离也使他们彼此产生了爱慕之心。 　　

        在瑞典这个浪漫的国度里，一段纯粹而美好的爱情悄然萌发。 　　

> 
念去去，千里烟波，暮霭沉沉楚天阔。


　　他们的恋情传到了国王的耳中，国王大怒，下令将笛卡尔处死。在克里斯汀的苦苦哀求下，国王将他放逐回国，公主被软禁在宫中。 　　

        当时，欧洲大陆正在流行黑死病。笛卡尔回到法国后不久，便染上重病。在生命进入倒计时的那段日子，他日夜思念的还是街头偶遇的那张温暖笑脸。他每天坚持给她写信，盼望着她的回音。然而，这些信都被国王拦截下来，公主一直没有收到他的任何消息。 　　

> 
欲知心里事,看取腹中书. 


　　在笛卡尔给克里斯汀寄出第十三封信后，他永远地离开了这个世界。此时，被软禁在宫中的小公主依然徘徊在皇宫的走廊里，思念着远方的情人。 　  
　　这最后一封信上没有写一句话，只有一个方程：r=a(1-sinθ)。 　　
　　国王以为这个方程里隐藏着两个人的秘密，便把全城的数学家召集到皇宫，但是没有人能解开这个函数式。他不忍看着心爱的女儿每天闷闷不乐，便把这封信给了她。拿到信的克里斯汀欣喜若狂，她立即明白了恋人的意图，找来纸和笔，把图形画了出来，一颗心形图案出现在眼前，克里斯汀泪流满面，这条曲线就是著名的“心形线”。 　　

        国王去世后，克里斯汀继承王位，便立刻派人去法国寻找心上人的下落，收到的却是笛卡尔去世的消息，留下了一个永远的遗憾…… 　　这封享誉世界的另类情书，据说至今还保存在欧洲笛卡尔的纪念馆里。

这个故事的出处无从知道，网络上流传着各种各样的版本，甚至在百度百科也有着这个故事。后来，有人考证了真实性，认为这是一个美丽的谎言，但并不妨碍人们对爱心线喜爱。

在直角坐标系中，爱心线的方程的python 表达为：`x** 2+ y** 2 + a * x= a * sqrt(x** 2+y** 2) 和 x** 2+ y** 2 - a * x= a * sqrt(x** 2+y** 2）`通过x 来求对应的y值很麻烦，就像软件设计中的“万能层”那样，可以采用参数方程来表示：

```
x=a*(2*cos(t)-cos(2*t))
y=a*(2*sin(t)-sin(2*t))
```

具体的python代码如下：

```
import numpy as np
import matplotlib.pyplot as plt
a = 1
t = np.linspace(0 , 2 * np.pi, 1024)
X = a*(2*np.cos(t)-np.cos(2*t))
Y = a*(2*np.sin(t)-np.sin(2*t))
plt.plot(Y, X,color='r')
plt.show()
```

代表爱心的心形线来了： 

![](https://img-blog.csdn.net/20170409151126653?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvd2lyZWxlc3NfY29t/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)





但这不是六行代码呀？也不是r=a(1-sinθ)呀？ 的确如此，那是极坐标系，python 的matplotlib同样支持极坐标系的，爱心线的六行pyton代码如下：

```
import numpy as np
import matplotlib.pyplot as plt
T = np.linspace(0 , 2 * np.pi, 1024)
plt.axes(polar = True)
plt.plot(T, 1. - np.sin(T),color="r")
plt.show()
```

这样，得到的就是封面中的图像了：

![](https://img-blog.csdn.net/20170409151439030?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvd2lyZWxlc3NfY29t/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)




心形线确实是爱心满满，如果融入了忧伤会是怎样呢？

```
import numpy as np
import matplotlib.pyplot as plt

x = np.linspace(-8 , 8, 1024)
y1 = 0.618*np.abs(x) - 0.8* np.sqrt(64-x**2)
y2 = 0.618*np.abs(x) + 0.8* np.sqrt(64-x**2) 
plt.plot(x, y1, color = 'r')
plt.plot(x, y2, color = 'r')
plt.show()
```

这样就得到了另一个爱心线： 

![](https://img-blog.csdn.net/20170409151504780?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvd2lyZWxlc3NfY29t/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)


网络上还有关于爱心线的各种漂亮实现，也充满了各种各样的情绪，但对于每一种，基本上都可以用python 相对简洁的实现。




实际上，绘图很简单，难的是那些曲线方程的表达以及实际的应用场景需求，比如螺旋线。




![](https://img-blog.csdn.net/20170409151535246?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvd2lyZWxlc3NfY29t/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)





进一步，还可以画出各种的3维视图，例如： 




![](https://img-blog.csdn.net/20170409151608952?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvd2lyZWxlc3NfY29t/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)


作为数据分析乃至大数据处理的最后一个环节，就是所谓洞见的可视化，python 可以说是其中的一个简单实用的工具。

##### 参考阅读

http://matplotlib.org/

《Python 数据可视化编程实战》

《Python 数据分析实战》













![](https://mp.weixin.qq.com/mp/qrcode?scene=10000004&size=102&__biz=MzAwOTcyNzA0OQ==&mid=2658971475&idx=1&sn=06626199bcb1762aaaea9bc0b2f35d09&send_time=)
微信扫一扫
关注该公众号








