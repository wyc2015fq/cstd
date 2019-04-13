
# python抓取bing主页背景图片 - anlcy - 博客园






# [python抓取bing主页背景图片](https://www.cnblogs.com/camilla/p/7144768.html)
最初Python2写法：
\#!/usr/bin/env python\# -*- coding:utf-8 -*-
\# -*- author:nancy -*-
\# python2抓取bing主页所有背景图片
**import**urllib**,**re**,**sys**,**os
**def**get_bing_backphoto**():**
**if****(**os**.**path**.**exists**(**'photos'**)==****False****):**
os**.**mkdir**(**'photos'**)**
**for**i**in**range**(**0**,**1000**):**
url**=**'http://cn.bing.com/HPImageArchive.aspx?format=js&idx='**+**str**(**i**)**
**+**'&n=1&nc=1361089515117&FORM=HYLH1'
html**=**urllib**.**urlopen**(**url**).**read**()**
**if**html**==**'null'**:**
**print**'open & read bing error!'
sys**.**exit**(-**1**)**
reg**=**re**.**compile**(**'"url":"(.*?)","urlbase"'**,**re**.**S**)**
text**=**re**.**findall**(**reg**,**html**)**
\#http://s.cn.bing.net/az/hprichbg/rb/LongJi_ZH-CN8658435963_1366x768.jpg
**for**imgurl**in**text**:**
right**=**imgurl**.**rindex**(**'/'**)**
name**=**imgurl**.**replace**(**imgurl**[:**right**+**1**],**''**)**
savepath**=**'photos/'**+**name
urllib**.**urlretrieve**(**imgurl**,**savepath**)**
**print**name**+**' save success!'
get_bing_backphoto**()**
Python3与Python2的错误调整：
TypeError: can't use a string pattern on a bytes-like object原因为Python3 findall数据类型用bytes类型，因此在正则表达式前应添加html = html.decode('utf-8')。
“AttributeError: 'module' object has no attribute 'urlopen'”原因是Python3里的urllib模块已经发生改变，此处的urllib都应该改成urllib.request。
由于bing图片对外接口的图片json格式变了，python第三方库的导入格式有变化，因此代码调整如下：

\#!/usr/bin/env python
\# -*- coding:utf-8 -*-
\# -*- author:nancy-*-
\# python3抓取bing主页所有背景图片
**import**urllib**.**request**,**re**,**sys**,**os
**def**get_bing_backphoto**():**
**if****(**os**.**path**.**exists**(**'photos'**)==****False****):**
os**.**mkdir**(**'photos'**)**
**for**i**in**range**(**0**,**10**):**
url**=**'http://cn.bing.com/HPImageArchive.aspx?format=js&idx='**+**str**(**i**)+**'&n=1&nc=1361089515117&FORM=HYLH1'
html**=**urllib**.**request**.**urlopen**(**url**).**read**()**
**if**html**==**'null'**:**
**print****(**'open & read bing error!'**)**
sys**.**exit**(-**1**)**
html**=**html**.**decode**(**'utf-8'**)**
html**=**html**.**replace**(**'/az/'**,**'http://s.cn.bing.net/az/'**)**
reg**=**re**.**compile**(**'"url":"(.*?)","urlbase"'**,**re**.**S**)**
text**=**re**.**findall**(**reg**,**html**)**
**for**imgurl**in**text**:**
right**=**imgurl**.**rindex**(**'/'**)**
**print****(**imgurl**)**
name**=**imgurl**.**replace**(**imgurl**[:**right**+**1**],**''**)**
savepath**=**'photos/'**+**name
urllib**.**request**.**urlretrieve**(**imgurl**,**savepath**)**
**print****(**name**+**' save success!'**)**
get_bing_backphoto**()**





