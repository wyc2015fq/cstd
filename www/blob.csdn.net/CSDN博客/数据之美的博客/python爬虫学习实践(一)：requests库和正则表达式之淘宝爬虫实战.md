# python爬虫学习实践(一)：requests库和正则表达式之淘宝爬虫实战 - 数据之美的博客 - CSDN博客
2018年02月24日 15:52:13[看穿数据之美](https://me.csdn.net/oppo62258801)阅读数：997

使用requests库是需要安装的，requests库相比urllib 库来说更高级方便一点，同时与scrapy相比较还是不够强大，本文主要介绍利用requests库和正则表达式完成一项简单的爬虫小项目----淘宝商品爬虫。
有关于更多requests库的使用方法请参考：官方文档
第一步：我们先打开淘宝网页
![](https://img-blog.csdn.net/20180206125928981?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvcG9udG9vbg==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast&mprfK=http%3A%2F%2Fblog.csdn.net%2F)
然后搜索你想爬取的商品信息，比如：“手机”。然后可以看到各个商品的“商品名称”、“商品价格”。我们可以爬取手机的名称和价格，从而达到“货比三家”的功能。
![](https://img-blog.csdn.net/20180206130538044?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvcG9udG9vbg==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast&mprfK=http%3A%2F%2Fblog.csdn.net%2F)
第二步：鼠标右键点击网页的空白处，选中“查看页面源代码”，从页面源代码中获取商品名称和商品价格对应的字典中的key值，从而利用正则表达式来爬取所有商品的信息。
![](https://img-blog.csdn.net/20180206130855749?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvcG9udG9vbg==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast&mprfK=http%3A%2F%2Fblog.csdn.net%2F)
鼠标选中“查看页面源代码”后，会出现以下页面：
![](https://img-blog.csdn.net/20180206131036532?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvcG9udG9vbg==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast&mprfK=http%3A%2F%2Fblog.csdn.net%2F)
HTML格式的源代码，为了查找商品名称和商品价格对应的标签，也就是字典中的key，我们可以直接复制淘宝某一个商品的名称或者价格信息，在页面源代码中查找相对应的信息。不同的浏览器中，按Ctrl+F键可进入查找模式。
现在复制粘贴其中一个商品的名称信息“荣耀 荣耀9青春版”，得到如下结果：
![](https://img-blog.csdn.net/20180206131508228?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvcG9udG9vbg==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast&mprfK=http%3A%2F%2Fblog.csdn.net%2F)
可以发现其中的键值对，“title”是“荣耀 荣耀9青春版”的key，现在我们就得到了商品名称所对应的标签，同样的方法可以查找出商品价格对应的标签，这里就不过多阐述，直接给出：
![](https://img-blog.csdn.net/20180206132612298?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvcG9udG9vbg==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast&mprfK=http%3A%2F%2Fblog.csdn.net%2F)
现在得到了商品名称和价格对应的标签：“title”和“price”。
如果这样进行爬虫的话，只能爬取一个网页中的商品，如果我们还想爬第二页，第三页就要寻找其中每翻一次页中的网址对应的变化信息。
先来看看淘宝“手机”商品中第二页和第三页中的网址变化信息。
第二页：
![](https://img-blog.csdn.net/20180206133455106?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvcG9udG9vbg==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast&mprfK=http%3A%2F%2Fblog.csdn.net%2F)第三页：
![](https://img-blog.csdn.net/20180206133518038?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvcG9udG9vbg==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast&mprfK=http%3A%2F%2Fblog.csdn.net%2F)
我们看到，无论是第二页第三页，只有最后的“&s=**”部分不一致，同时第二页对应的s=48，第三页对应的s=96，可以推测出第i页的s=48*（i-1），我们只需要把这个url添加到我们的爬取程序上，用for循环实现这个url的更新即可。
第三步：进入编写实例
这里用python语言编写程序实现爬虫功能。
由于需要用到正则表达式和requests库，所以先引入。
```python
import requests
import re
```
先定义好一个主函数main，把各个功能模块化，然后再去编写各个模块的具体代码。
```python
def main():
    goods = '手机'
    depth = 3 #爬取3页
    start_url = 'http://s.taobao.com/search?q=' + goods   #爬取商品的url
    infoList = []  #初始化一个列表，方便把爬取的键值对添加到里面，然后读取
    for i in range(depth):  #for循环更新url
        try:   #处理异常
            url = start_url + '&s=' + str(48*i) #每页的url
            html = getHTMLText(url) #获取页面信息
            parsePage(infoList, html) #解析获取的信息，捕捉商品信息的键值对并且添加到列表中
        except:
            continue
    printGoodsList(infoList)  #打印列表中的键值对信息
```
主函数写完了，我们把各功能的模块具体化。
首先是getHTMLText（）函数，获取url页面的信息文本：
```python
def getHTMLText(url):
    try:
        r = requests.get(url, timeout=30) #get方法请求url链接，如果超过30秒没有响应就退出
        r.raise_for_status() #状态码，如果是200则请求成功
        r.encoding = r.apparent_encoding #将编码方式更改为捕捉到的内容的编码方式，中文网页一般utf-8
        return r.text
    except:
        return ""
```
然后是 parsePage( )函数，解析页面信息，将商品名称和商品价格组合成一对键值对，然后添加到列表里。需要用到正则表达式。
```python
def parsePage(ilt, html):
    try:
        plt = re.findall(r'\"price\"\:\"[\d\.]*\"',html) #finfall功能将所有的value数字整合在plt列表
        tlt = re.findall(r'\"title\"\:\".*?\"',html) #将所有title的value整合在tlt列表
        for i in range(len(plt)): #基于列表plt的长度使用for循环
            price = eval(plt[i].split(':')[1]) #split用“：”切割key和value；eval可以转化格式
            title = eval(tlt[i].split(':')[1])
            ilt.append([price , title]) #将商品的价格和名称作为键值对添加到ilt列表
            
    except:
        print("")
```
最后是打印函数printGoodsList( )，输出爬取的结果：
```python
def printGoodsList(ilt):
    tplt = "{:4}\t{:8}\t{:16}"
    tplt2 = "{:8}\t{:16}\t{:14}"
    print(tplt2.format("序号", "价格", "商品名称")) #格式化输出函数
    count = 0 #序号
    for g in ilt:
        count = count + 1
        print(tplt.format(count, g[0], g[1])) #依次输出序号，价格，商品名称
```
最后执行主函数：main（），查看结果
```
序号      	名称              	价格            
   1	896     	小米 红米5 Plus     
   2	1013    	魅族 魅蓝S6         
   3	1374    	vivo Y75        
   4	1385    	华为 畅享7S         
   5	746     	小米 红米5          
   6	2696    	荣耀 V10          
   7	1323    	荣耀 畅玩7X         
   8	5661    	苹果 iPhone 8 Plus 
   9	7219    	苹果 iPhone X     
  10	5226    	苹果 iPhone 8     
  11	1752    	小米 Note3        
  12	4514    	华为 Mate 10 Pro  
  13	2998    	vivo X20        
  14	1942    	vivo X9s        
  15	2144    	荣耀 荣耀9          
  16	3776    	华为 Mate 10      
  17	4971    	苹果 iPhone 7 Plus
  18	2144    	小米 小米手机6        
  19	1163    	小米 5X           
  20	2883    	小米 MIX2         
  21	560     	小米 红米5A         
  22	796     	华为 畅享7          
  23	954     	荣耀 V9 PLAY      
  24	2794    	一加手机5T          
  25	2904    	华为 P10          
  26	4184    	苹果 iPhone 7     
  27	4500    	三星 Galaxy S8    
  28	5452    	三星 Galaxy Note8 
  29	903     	魅族 魅蓝note6      
  30	4606    	三星 Galaxy S8+   
  31	584     	小米 红米Note 5A    
  32	1394    	360手机 N6 Pro    
  33	1104    	华为 畅享7 Plus     
  34	1192    	vivo Y66i       
  35	2230    	荣耀 V9           
  36	1830    	锤子 坚果Pro 2      
  37	1274    	小米 Max2         
  38	3298    	vivo X20Plus    
  39	1392    	诺基亚 诺基亚 6 第二代   
  40	737     	荣耀 畅玩6A         
  41	3299    	OPPO R11s Plus  
  42	3371    	美图 T8s          
  43	534     	小米 红米4A         
  44	1815    	魅族 PRO7         
  45	1086    	荣耀 荣耀8青春版       
  46	2099    	OPPO A79        
  47	791     	小米 红米Note 4X    
  48	1932    	华为 麦芒6          
  49	1987    	vivo Y79        
  50	2524    	努比亚 Z17         
  51	1299    	OPPO A57        
  52	691     	魅族 魅蓝6          
  53	1226    	努比亚 Z17 mini    
  54	5308    	美图 V6           
  55	736     	魅族 魅蓝note5      
  56	1699    	OPPO A73        
  57	2202    	美图 M8           
  58	3281    	华为 P10 Plus     
  59	829     	360手机 N6 Lite   
  60	1226    	荣耀 荣耀9青春版       
  61	1474    	诺基亚 诺基亚7        
  62	2546    	华为 NOVA 2S      
  63	1073    	锤子 坚果Pro        
  64	2653    	美图 M8S          
  65	1592    	华为 NOVA 2       
  66	597     	荣耀 畅玩6          
  67	761     	华为 畅享6S         
  68	3186    	华为 Mate 9 Pro   
  69	2663    	华为 Mate 9       
  70	3770    	索尼 Xperia XZ Premium
  71	1399    	OPPO A83        
  72	773     	魅族 魅蓝E2         
  73	2843    	OPPO R11s       
  74	2100    	三星 Galaxy C9 Pro
  75	2257    	魅族 PRO7 Plus    
  76	1028    	荣耀 畅玩6X         
  77	651     	魅族 魅蓝5s         
  78	1326    	华为 NOVA 青春版     
  79	2223    	vivo X9s Plus   
  80	2807    	努比亚 Z17S        
  81	2900    	美图 T8           
  82	1109    	360手机 N6        
  83	657     	小米 红米4X         
  84	1117    	vivo Y66        
  85	1668    	魅族 PRO 6 Plus   
  86	543     	天语 X11          
  87	150     	诺基亚 新105        
  88	1083    	华为 NOVA         
  89	2077    	华为 NOVA 2 PLUS  
  90	3435    	vivo Xplay6     
  91	1883    	vivo X9         
  92	194     	创星 S1           
  93	3041    	索尼 Xperia XZ1   
  94	859     	vivo Y53        
  95	171     	波导 A520         
  96	1412    	vivo Y67        
  97	935     	vivo Y55        
  98	2563    	小米 MIX          
  99	2355    	OPPO R11        
 100	1966    	小米 Note 2       
 101	1402    	魅族 PRO 6S       
 102	537     	魅族 魅蓝 A5        
 103	2360    	谷歌 Pixel XL     
 104	175     	纽曼 F9           
 105	2499    	金立 M7           
 106	398     	誉品 5A           
 107	1988    	vivo X9Plus     
 108	1999    	金立 S10          
 109	2036    	OPPO R9S        
 110	1275    	荣耀 荣耀8          
 111	1646    	小米 5S           
 112	2092    	OPPO R9s Plus   
 113	677     	华为 畅享6          
 114	1089    	360手机 N5s       
 115	1752    	苹果 iPhone SE    
 116	4590    	HTC U11+        
 117	437     	誉品 I7S          
 118	387     	索野 7S           
 119	2996    	黑莓 KEYone       
 120	125     	纽曼 M560         
 121	1172    	酷派 Cool S1      
 122	3888    	中兴 天机Axon M     
 123	899     	小辣椒 红辣椒S6       
 124	1735    	努比亚 Z17miniS    
 125	1260    	OPPO A59S       
 126	1750    	三星 Galaxy C8    
 127	9730    	三星 SM-W2017     
 128	897     	欧加 S11 Plus     
 129	2999    	欧奇 OKP6 PRO     
 130	892     	魅族 魅蓝X          
 131	3358    	vivo X20Plus屏幕指纹版
 132	3864    	HTC U11         
 133	583     	魅族 魅蓝5          
 134	112     	小辣椒 G108        
 135	1266    	华为 MATE8        
 136	3299    	HTC U11 EYEs    
 137	737     	360手机 VIZZA     
 138	1767    	三星 Galaxy S7 edge
 139	95      	天语 T2           
 140	202     	创星 R9           
 141	1791    	小米 5s Plus      
 142	2603    	三星 Galaxy S7    
 143	2633    	一加手机5           
 144	1599    	金立 大金钢2
```
