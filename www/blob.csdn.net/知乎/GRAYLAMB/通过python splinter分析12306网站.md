# 通过python splinter分析12306网站 - 知乎
# 

上一篇文章，我们简单介绍了一下python splinter的安装。这一次，我们通过python和splinter分析一下12306网站。
这是一个很简单也很有趣的尝试。而且正值春运期间，也许会对您的出行有帮助～

当然本文纯属简单的入门级应用，大神请轻喷。

**一、12306网站的情况**

下面是2015年改版后的12306网站页面：

![](https://pic3.zhimg.com/2df94bfa7f8798b91a8e625d90c5c61a_b.png)点击右侧的新版售票按钮，可以进入到售票查询页面：

![](https://pic3.zhimg.com/ab2e154e4a6fc5edd6ef916f919a4b26_b.png)不必登录，输入出发地、目的地、出发日即可查询到车票情况。

![](https://pic2.zhimg.com/af8137f2108b77eaecc1b0c450cebfc9_b.png)二、我们的需求

对于火车票，我们最重要的需求就是查看有没有相应时间、相应地点的车票，而且肯定希望不要人工干预的自动查看。简单整理了一下需求的流程：

![](https://pic1.zhimg.com/aa8f166e7b2a992ab8ffaaf8fd350b94_b.jpg)三、python splinter的程序处理

按照上面分析的需求流程，下面开始逐一分析如何实现python splinter的程序处理。

在python程序的开头，需要导入splinter的库：

```
from splinter.browser import Browser
```

好了，从现在开始，后面的程序可以调用splinter的库函数了。splinter的库函数比较丰富，主要是针对浏览器（或浏览器页面控件）的操作。比如，查找某一控件、点击某一控件。各位感兴趣可以在下面的地址详细查阅：

[http://splinter.readthedocs.org/en/latest/drivers/chrome.html#api-docs](https://link.zhihu.com/?target=http%3A//splinter.readthedocs.org/en/latest/drivers/chrome.html%23api-docs)

下面进入正题：

1）进入查票页面

首先调起chrome浏览器，后面明确写了使用chrome的浏览器驱动：

```python
b = Browser(driver_name="chrome")
```

下面进入查票页面：

```
url = "https://kyfw.12306.cn/otn/leftTicket/init"
b.visit(url)
```

现在可以看到浏览器已经自己打开，并进入到查票页面：

![](https://pic1.zhimg.com/35dd334447155de7826c997cfadda998_b.png)这时候需要人工干预一下，要在出发地和目的地录入地点、时间，然后进行一下查询：

![](https://pic3.zhimg.com/a320393f40ea70e4517d08df92040d4e_b.png)这么做主要是获取一下这个页面的cookies信息：

![](https://pic3.zhimg.com/e0e18c54fb419c7cfc5f5487e77e21ca_b.png)可以看到有这些cookies信息：

_jc_save_fromDate------出发时间   cookie值：2016-02-06

_jc_save_fromStation------出发地   cookie值：%u5317%u4EAC%2CBJP（北京）

_jc_save_toStation------目的地    cookie值：%u4E0A%u6D77%2CSHH（上海）

下面，可以开始进入下一步操作了。

2）判断是否有票，如果没票则自动刷新页面：

刚才也看到，车票的出发时间、出发地、目的地等信息是藏在页面的cookies信息里，所以如果要查询自动刷新页面，查询特定日期、特定地点的车票，可以模拟插入带有这些信息的cookies值。

```
b.cookies.add({"_jc_save_fromDate":"2016-02-06"})
b.cookies.add({"_jc_save_fromStation":"%u5317%u4EAC%2CBJP"})
b.cookies.add({"_jc_save_toStation":"%u4E0A%u6D77%2CSHH"})
```

由于出发地、目的地的cookies值是加密过的，因此如果你想调整目的地信息，那可以像刚才一样先人工查询一下你期望的目的地信息，然后就可以立刻从cookies里找到相应地点的密文了，再粘贴到上面的语句中，以便插入cookies信息。

插入带有相关信息的cookies值后，就可以刷新页面了。用下面的语句。

`b.reload()`

reload以后就可以看到浏览器页面中填写的出发地、目的地、出发时间变成刚才我们添加的cookies的值了。

下面需要新增一个条件判断及一个循环，达到没票自动刷新查询的目的。

首先是条件判断，这里采用一个比较简单的判断：如果有票肯定有“预订”按钮。

splinter有专门的语句判断是否存在特定组件，我们可以用下面的语句实现对“预订”按钮的检查。

`b.is_element_present_by_text(u"预订")`

如果有“预订”按钮，则返回true，如果没有返回false。

或是否找不到“预订”按钮。

`b.is_element_not_present_by_text(u"预订")`
如果没有“预订”按钮，则返回true，如果有返回false。

这样就可以以此为条件，编写循环逻辑：

```
while(b.is_element_not_present_by_text(u"预订")):
    b.find_by_text(u"查询").click()
    time.sleep(3)
```

 即：如果不存在“预订”按钮，就重新点击查询，如果存在“预订”按钮，则跳出循环，进行下一步操作。为了防止过快刷页面新造成报错，可以增加time.sleep(3)，即：在每次点击“查询”后，休息3秒，再进入下一次循环。

下面，如果存在“预订”按钮，怎么处理呢？

这里，需要说明的是，因为有很多车次，所以就会有很多“预订”按钮，因此“预订”按钮的对象，是一个“组”。
![](https://pic2.zhimg.com/c18f3259e6e3a225dfe625f9b29e90f5_b.png)
这时，要操作特定的“预订”按钮时，必须制定时“组”中的第几个按钮。“组”是从0开始排序的，因此，我们每次先点击序号为“0”的“预订”按钮（排在最前面的“预订”按钮），即：有票的最早的车次。

`b.find_by_text(u"预订")[0].click()`

这样，这个程序就基本写好了。可以不断刷新页面，并检查是否有当天的火车票，如果有则点击预订。

```
from splinter.browser import Browser
import time
b = Browser(driver_name="chrome")
url = "https://kyfw.12306.cn/otn/leftTicket/init"
b.visit(url)
raw_input("请直接在页面输入目的地信息和出发时间，点击查询后，按任意键继续: ")
b.cookies.add({"_jc_save_fromDate":"2016-02-09"})
b.cookies.add({"_jc_save_fromStation":"%u5317%u4EAC%2CBJP"})
b.cookies.add({"_jc_save_toStation":"%u4E0A%u6D77%2CSHH"})
b.reload()
while(b.is_element_not_present_by_text(u"预订")):
    b.find_by_text(u"查询").click()
    time.sleep(3)
b.find_by_text(u"预订")[0].click()
exit();
```

这只是一个简单的示例，不包括对于验证码的处理。所以在订票的时候还得手工输入验证码。这个示例非常的简单，无法检查特定时间、特定车次、特定舱位（硬座、软座、商务）的车票，只能检查某一天整体的车票情况。如果需要将特定时间、特定车次、特定舱位（硬座、软座、商务）也作为条件进行查询，也是可以的，但是比较复杂，要用到python的json库。这是由于，12306对于查票返回的数据使用了json 格式的数据包：![](https://pic3.zhimg.com/54db25d4368f66eb14878933bba5302e_b.png)

而在这些json数据包中，可以找到特定时间、特定车次、特定舱位（硬座、软座、商务）是否有票的信息：

![](https://pic1.zhimg.com/52f7bb09decc2a9de090982aaf1128e4_b.png)而对于python来说，其json库就是专门用来分析json数据包。但过程会相对复杂一些，如果有人感兴趣，后续有机会再开篇文章专题介绍。

