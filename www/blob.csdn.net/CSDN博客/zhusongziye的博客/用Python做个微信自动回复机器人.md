# 用Python做个微信自动回复机器人 - zhusongziye的博客 - CSDN博客





2018年08月13日 21:26:37[zhusongziye](https://me.csdn.net/zhusongziye)阅读数：478








微信已经成为我们生活不可或缺的一部分，每天有380亿条消息从微信上发出，其中6亿条是语音消息，有350万个活跃的公众账号，并存在着8亿活跃粉丝。微信是一个庞大的生态群体。

今天我们来玩转一下微信，微信有关的第三方框架很多，比如wxpy，wxBot等。今天要使用的是其中著名的itchat Python模块来做两件有趣的事情。



**一个自动回复的机器人**



微信自上线以来，一直没有自动回复的功能，想必是有他们的理念。但是有些人群，确实对此功能有一定需求，我举两个栗子：
- 
不愿时刻被消息打扰的人

- 
消息需要批量处理的人们（比如微商）


**功能列表:**
- 
收到消息立即自动回复

- 
收到消息延迟指定时间回复

- 
对不同好友定制不同的回复内容

- 
在手机端随时进行控制



**1.启用itchat模块**

itchat的原理是通过扫码进行远程微信网页端的登录，然后通过在移动端进行操作，网页端进行响应，来实现一些功能，虽然似乎绕了个弯子，但是在微信的限制下，这似乎已经是一种最佳的方式了。

首先我们需要考虑别的问题是：程序如何在微信端接收到你的指令。此时出现了一个非常重要的角色：文件管理器。（当网页端微信登陆后，消息列表会出现）此时，文件管理器充当了移动端和web端的桥梁。

我们可以先通过设定几个全局变量来做作为功能的开关和保存数据的容器。

![](https://img-blog.csdn.net/20180813211249717?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3podXNvbmd6aXll/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)

然后通过判断web端在”文件管理器“中接收到的字符串指令来进行不同操作，假如此时我们收到了朋友的消息，需要程序给出自动回复。

![](https://img-blog.csdn.net/20180813211309169?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3podXNvbmd6aXll/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)



收到朋友消息即时进行自动回复是很简单的，但是如何去做延时发送回复消息呢？我先谈一谈我的想法，抛砖引玉：


- 
一般发送消息需要用到队列，进行入队和出队，我在这里设置了一个字典来保存消息发送者的数据。

- 
键为消息发送者的昵称，值是一个长度为2的数组，分别保存消息发送者的微信id和接收消息时的时间戳。

- 
这样我将每条发送过来的朋友信息保存在这个字典中，再通过将设定延迟时间同消息时间戳求和与当前时间戳进行对比，若当前时间戳较大，那么执行发送消息的操作。

- 
此时再开启一个线程作为定时任务，定时去检测字典中每条数据是否到达了发送的临界要求（当前时间戳>=消息时间戳+设定的延迟时间）。

- 
Python中有个专门做定时任务的模块叫sched，但是我尝试了一下，sched会阻塞当前主线程，也会阻塞itchat的线程，所以并不合适。

- 
这里我还是采用了threading的Timer来充当定时器，不过要注意使用递归，否则将会出现运行一次就结束的情况。




![](https://img-blog.csdn.net/20180813211335539?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3podXNvbmd6aXll/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)



到此为止，主要的功能已经实现了，我用一个测试账号对我的微信进行了各种测试，看一下以下截图：

![](https://img-blog.csdn.net/20180813211353354?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3podXNvbmd6aXll/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)



![](https://img-blog.csdn.net/20180813211412149?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3podXNvbmd6aXll/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)





**2.增强功能**

这时功能基本已经完成了，这就结束了吗？别着急，**再想想有没有需要完善一下的地方？**用过微信web端的同学应该知道，当web端长期处于未操作的状态下会失去连接。



在我们这个情况下，**假如你长时间未收到微信消息，后台程序将会与微信失去连接**，再次开启需要登上服务器重启程序，这显然非常麻烦。有没有什么简单的解决办法呢?



我想到一些应用的后台通常会做**一道心跳检测机制**，那我就模仿这个思路，定时给我的”文件管理器“发一个字符串，来保持连接。



```
def keep_alive():
   text="保持登录"
   itchat.send(text, toUserName="filehelper")
   global timer2
   timer2 = threading.Timer(60*60,keep_alive)
   timer2.start()
```



最后，我们需要将这个程序发布在服务器上，让它全天候为我的微信服务。



这里需要注意，如果仅用python xxxx.py来运行的话，关闭shell会导致进程结束，所以我们需要使用nohup python xxxx.py &来全方位守护进程，这里啰嗦一句，nohup和&的功能是不一样的，很多人容易混淆，感兴趣的话可以去查下资料区分一下。



**   简单分析微信好友信息**



上文提到，既然我们能通过itchat来获取好友的信息，**那让我们看看都有哪些好玩的信息**。这是以json形式返回的我的信息，同理我的好友的这些公开信息我也能获取到。



![](https://img-blog.csdn.net/20180813211507130?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3podXNvbmd6aXll/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)



我们就来进行一些简单的的数据抓取，清洗与呈现。我看中的字段是：Sex、City、Province、Signature。我想做的是通过图表来直观地展示我微信好友中的**性别比例**，**家乡分布**（当然不一定准确，很多人都是胡乱设置的），**我的江苏朋友的市级分布**，以及**好友个性签名的词云**。



这部分内容主要是需要熟悉诸如pyecharts、jieba、wordcloud模块的API调取，难度不大，但需要细心调试。我这里就不详细赘述了。



**1).性别比例**

![](https://img-blog.csdn.net/20180813211520158?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3podXNvbmd6aXll/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)



**用饼图可视化一下：**



![](https://img-blog.csdn.net/20180813211529523?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3podXNvbmd6aXll/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)



### **2).好友省级分布**

```
def get_data(type):
   result=[]
   my_friends = itchat.get_friends(update=True)[0:]
   for item in my_friends:
       result.append(item[type])
   return result

def friends_province():
   # 获取好友省份
   province= get_data("Province")
   # 分类
   province_distribution = {}
   for item in province:
       #删除英文省份，因为中国地图表中没有
       if bool(re.search('[a-z]',item)):
           continue
       elif not province_distribution.__contains__(item):
           province_distribution[item] = 1
       else:
           province_distribution[item] += 1
   #将省份名为空的删除
   province_distribution.pop('')
   #提取地图接口需要的数据格式
   province_keys=province_distribution.keys()
   province_values=province_distribution.values()

   return province_keys,province_values
   
if __name__ == '__main__':
   itchat.auto_login(True)

   微信好友省份分布
   attr,value=friends_province()
   map = Map("我的微信好友分布", "@寒食君",width=1200, height=600)
   map.add("", attr, value, maptype='china', is_visualmap=True,
           visual_text_color='#000')
   map.render()
```



**用地图来显示一下：**

![](https://img-blog.csdn.net/2018081321154262?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3podXNvbmd6aXll/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)



### **3).个性签名词云**

```
def friends_signature():
   signature = get_data("Signature")
   wash_signature=[]
   for item in signature:
       #去除emoji表情等非文字
       if "emoji" in item:
           continue
       rep = re.compile("1f\d+\w*|[<>/=【】『』♂ω]")
       item=rep.sub("", item)
       wash_signature.append(item)

   words="".join(wash_signature)
   wordlist = jieba.cut(words, cut_all=True)
   word_space_split = " ".join(wordlist)
   coloring = np.array(Image.open("C:/Users/casua/Desktop/test1.JPG"))
   my_wordcloud = WordCloud(background_color="white", max_words=800,
                            mask=coloring, max_font_size=80, random_state=30, scale=2,font_path="C:/Windows/Fonts/STKAITI.ttf").generate(word_space_split)

   image_colors = ImageColorGenerator(coloring)
   plt.imshow(my_wordcloud.recolor(color_func=image_colors))
   plt.imshow(my_wordcloud)
   plt.axis("off")
   plt.show()
```



**看一下词云图：**

![](https://img-blog.csdn.net/20180813211555285?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3podXNvbmd6aXll/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)





