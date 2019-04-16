# [Python微信开发] 一.itchat入门知识及微信自动回复、微信签名词云分析 - 杨秀璋的专栏 - CSDN博客





2018年03月19日 23:22:55[Eastmount](https://me.csdn.net/Eastmount)阅读数：2691
所属专栏：[知识图谱、web数据挖掘及NLP](https://blog.csdn.net/column/details/eastmount-kgdmnlp.html)









最近准备学习微信小程序开发，偶然间看到了python与微信互动的接口itchat，简单学习了下，感觉还挺有意思的，故写了篇基础文章供大家学习。itchat是一个开源的微信个人号接口，使用python调用微信从未如此简单。使用不到三十行的代码，你就可以完成一个能够处理所有信息的微信机器人。

官网文档：[http://itchat.readthedocs.io/zh/latest/](http://itchat.readthedocs.io/zh/latest/)

本文主要讲解itchat扩展包的入门基础知识，包括：
1.itchat安装及入门知识
2.微信好友性别分析
3.微信自动回复及发送图片
4.获取微信签名并进行词云分析

基础性文章，希望对您有所帮助，后面将结合舆情分析、朋友圈等接口进行更一步的讲解。如果文章中存在错误或不足之处，还请海涵~

参考文章：
[https://zhuanlan.zhihu.com/p/26514576?group_id=839173221667446784](https://zhuanlan.zhihu.com/p/26514576?group_id=839173221667446784)
[https://www.cnblogs.com/jmmchina/p/6692149.html](https://www.cnblogs.com/jmmchina/p/6692149.html)
[http://blog.csdn.net/qinyuanpei/article/details/79360703](http://blog.csdn.net/qinyuanpei/article/details/79360703)





## 一. itchat安装及入门知识


安装通过 pip install itchat 命令实现，如下图所示：



![](https://img-blog.csdn.net/20180319215428596)

安装成功之后通过 import itchat 进行导入。


![](https://img-blog.csdn.net/20180319215540496)

下面给出我们第一个简单的代码：

```python
# -*- coding:utf-8 -*-
import itchat

# 登录
itchat.login()
# 发送消息
itchat.send(u'你好', 'filehelper')
```

首先调用itchat.login()函数登录微信，再通过itchat.send(u'你好', 'filehelper')函数发送信息给微信的“文件传输助手（filehelper）”。注意，执行代码过程中会弹出一张二维码图片，我们通过手机扫一扫登录后才能获取我们微信及好友的信息。

![](https://img-blog.csdn.net/20180319220744674)![](https://img-blog.csdn.net/2018031922081133)





输出结果如下图所示，可以看到给自己发送了一个“你好”。



![](https://img-blog.csdn.net/20180319221817916)



下面给出另一段代码：


```python
#-*- coding:utf-8 -*-
import itchat

# 先登录
itchat.login()

# 获取好友列表
friends = itchat.get_friends(update=True)[0:]
print u"昵称", u"性别", u"省份", u"城市"
for i in friends[0:]:
    print i["NickName"], i["Sex"], i["Province"], i["City"]
```
这里最重要的代码是获取好友列表，代码如下：
**friends = itchat.get_friends(update=True)[0:] **

再通过["NickName"]获取昵称、["Sex"]获取性别、["Province"]获取省份、["City"]获取城市。返回的结果如下所示，其中第一个friends[0]是作者本人，然后性别0表示未填写、1表示男性、2表示女性；省份和城市可以不设置。



![](https://img-blog.csdn.net/20180319222744405)








## 二. 微信好友性别分析


下面直接给出对微信好友性别分析绘图的代码：


```python
#-*- coding:utf-8 -*-
import itchat

#获取好友列表
itchat.login() #登录
friends = itchat.get_friends(update=True)[0:]

#初始化计数器
male = 0
female = 0
other = 0

#1男性,2女性,3未设定性别
for i in friends[1:]: #列表里第一位是自己，所以从"自己"之后开始计算
    sex = i["Sex"]
    if sex == 1:
        male += 1
    elif sex == 2:
        female += 1
    else:
        other += 1
#计算比例
total = len(friends[1:])
print u"男性人数：", male
print u"女性人数：", female
print u"总人数：", total
a = (float(male) / total * 100)
b = (float(female) / total * 100)
c = (float(other) / total * 100)
print u"男性朋友：%.2f%%" % a
print u"女性朋友：%.2f%%" % b
print u"其他朋友：%.2f%%" % c

#绘制图形
import matplotlib.pyplot as plt
labels = ['Male','Female','Unkown']
colors = ['red','yellowgreen','lightskyblue']
counts = [a, b, c]
plt.figure(figsize=(8,5), dpi=80)
plt.axes(aspect=1) 
plt.pie(counts, #性别统计结果
        labels=labels, #性别展示标签
        colors=colors, #饼图区域配色
        labeldistance = 1.1, #标签距离圆点距离
        autopct = '%3.1f%%', #饼图区域文本格式
        shadow = False, #饼图是否显示阴影
        startangle = 90, #饼图起始角度
        pctdistance = 0.6 #饼图区域文本距离圆点距离
)
plt.legend(loc='upper right',)
plt.rcParams['font.sans-serif']=['SimHei'] #用来正常显示中文标签
plt.title(u'微信好友性别组成')
plt.show()
```

这段代码获取好友列表后，从第二个好友开始统计性别，即friends[1:]，因为第一个是作者本人，然后通过循环计算未设置性别0、男性1和女性2，最后通过Matplotlib库绘制饼状图。如下所示，发现作者男性朋友66.91%，女性朋友26.98%。



![](https://img-blog.csdn.net/20180319223704718)








## 三. 微信自动回复及发送图片




微信发送信息调用send()函数实现，下面是发送文字信息、文件、图片和视频。


```python
# coding-utf-8
import itchat
itchat.login()
itchat.send("Hello World!", 'filehelper')
itchat.send("@fil@%s" % 'test.text')
itchat.send("@img@%s" % 'img.jpg', 'filehelper')
itchat.send("@vid@%s" % 'test.mkv')
```
比如给我的微信文件助手发了个“Hello World”和一张图片。


![](https://img-blog.csdn.net/20180319225047620)


如果想发送信息给指定好友，则核心代码如下：


```
#想给谁发信息，先查找到这个朋友
users = itchat.search_friends(name=u'通讯录备注名')
#找到UserName
userName = users[0]['UserName']
#然后给他发消息
itchat.send('hello',toUserName = userName)
```

下面这部分代码是自动回复微信信息，同时在文件传输助手也同步发送信息。

```python
#coding=utf8
import itchat
import time

# 自动回复
# 封装好的装饰器，当接收到的消息是Text，即文字消息
@itchat.msg_register('Text')
def text_reply(msg):
    if not msg['FromUserName'] == myUserName: # 当消息不是由自己发出的时候
        # 发送一条提示给文件助手
        itchat.send_msg(u"[%s]收到好友@%s 的信息：%s\n" %
                        (time.strftime("%Y-%m-%d %H:%M:%S", time.localtime(msg['CreateTime'])),
                         msg['User']['NickName'],
                         msg['Text']), 'filehelper')
        # 回复给好友
        return u'[自动回复]您好，我现在有事不在，一会再和您联系。\n已经收到您的的信息：%s\n' % (msg['Text'])

if __name__ == '__main__':
    itchat.auto_login()
    myUserName = itchat.get_friends(update=True)[0]["UserName"]
    itchat.run()
```
运行结果如下图所示：


![](https://img-blog.csdn.net/20180319231122930)![](https://img-blog.csdn.net/20180319231158349)







## 四. 获取微信签名并进行词云分析




最后给出获取微信好友的签名的词云分析，其friends[i]["Signature"]获取签名，最后调用jieba分词最后进行WordCloud词云分析。

```python
# coding:utf-8
import itchat
import re

itchat.login()
friends = itchat.get_friends(update=True)[0:]
tList = []
for i in friends:
    signature = i["Signature"].replace(" ", "").replace("span", "").replace("class", "").replace("emoji", "")
    rep = re.compile("1f\d.+")
    signature = rep.sub("", signature)
    tList.append(signature)

# 拼接字符串
text = "".join(tList)

# jieba分词
import jieba
wordlist_jieba = jieba.cut(text, cut_all=True)
wl_space_split = " ".join(wordlist_jieba)

# wordcloud词云
import matplotlib.pyplot as plt
from wordcloud import WordCloud, STOPWORDS, ImageColorGenerator   
import PIL.Image as Image
from scipy.misc import imread
from os import path
  
# 读取mask/color图片  
d = path.dirname(__file__)  
nana_coloring = imread(path.join(d, "test.png"))  

# 对分词后的文本生成词云  
my_wordcloud = WordCloud(background_color = 'white',      # 设置背景颜色  
                            mask = nana_coloring,          # 设置背景图片  
                            max_words = 2000,              # 设置最大现实的字数  
                            stopwords = STOPWORDS,         # 设置停用词  
                            max_font_size = 50,            # 设置字体最大值  
                            random_state = 30,             # 设置有多少种随机生成状态，即有多少种配色方案  
                            )
# generate word cloud   
my_wordcloud.generate(wl_space_split)  
  
# create coloring from image    
image_colors = ImageColorGenerator(nana_coloring)  
  
# recolor wordcloud and show    
my_wordcloud.recolor(color_func=image_colors)  
  
plt.imshow(my_wordcloud)    # 显示词云图  
plt.axis("off")             # 是否显示x轴、y轴下标  
plt.show()
```

输出结果如下图所示，注意这里作者设置了图片罩，生成的图形和那个类似，发现“个人”、“世界”、“生活”、“梦想”等关键词挺多的。



![](https://img-blog.csdn.net/20180319231617680)





(By:Eastmount 2018-03-19 晚上11点[http://blog.csdn.net/eastmount/](http://blog.csdn.net/eastmount/))







