# python爬虫 - wsp_1138886114的博客 - CSDN博客





2018年06月07日 22:28:23[SongpingWang](https://me.csdn.net/wsp_1138886114)阅读数：266
所属专栏：[python基础及相关](https://blog.csdn.net/column/details/23328.html)









### 一、入门

> 
##### 1.1 直接访问网址

python 3.x 包含(urllib+urllib2),并命名为urllib 
$~~~~~~~~~~~~~~~~~$urllib3不包含urllib2 
**(`python2.x`环境下)**
```
import urllib2
req = urllib2.Request("http://www.baidu.com")
res = urllib2.urlopen(req)
print(res.read())
```

**(`python3.x`环境下)**

```python
from urllib import request
req = request.Request("https://www.baidu.com")  #设置url地址
      #设置url地址 http则是直接获取源代码，https则会加密内容少
res = request.urlopen(req)              #处理浏览器返回的对象
print(res.read().decode("utf-8"))
print("方法属性:",dir(res))             #方法属性
print("访问网址:",res.url)              #访问网址
print("状态码：200访问成功",res.code)    #状态码：200访问成功
```

##### 1.2 包装成函数访问网址

**(`python2.x`环境下)**

```python
import urllib2
import random

def get_html(url):              #获取网页内容
    USER_AGENTS = []            #浏览器(末尾附浏览器)
    proxies = []                #代理IP(末尾附IP)
    req = urllib2.Request(url)  #设置url地址
    req.add_header('User-Agent',random.choice(USER_AGENTS))             #随机选取浏览器
    proxy_support=urllib2.ProxyHandler({"http":random.choice(proxies)}) #随机选取IP地址

    opener=urllib2.build_opener(proxy_support) #获取网站访问的对象
    urllib2.install_opener(opener)

    res = urllib2.urlopen(req)                #处理浏览器返回的对象
    html = res.read()
    return html

url = "https://www.douban.com/"               #需要浏览的网页
douban = get_html(url)
```

### 二、正则表达式

> 
##### 2.1 一般匹配案例

```
import re

pattern = re.compile("hello")
res1 = re.match(pattern,"hello123")      #以hello中h开头匹配完的为true
res2 = re.match(pattern,"hello QQ")
res3 = re.match(pattern,"hello ")
res4 = re.match(pattern,"hell")

print (res1)
print (res2)
print (res3)
print (res4)
>>>
<_sre.SRE_Match object at 0x025860C8>
<_sre.SRE_Match object at 0x025862F8>
<_sre.SRE_Match object at 0x02586678>
None

reg = re.compile("^he")             # ^ 开头符
reg1 = re.compile("jack$")          # $ 结尾附
print re.search(reg,"he is jack")
print re.search(reg1,"he is jack")
>>>
<_sre.SRE_Match object at 0x027866E8>
<_sre.SRE_Match object at 0x027866E8>

reg = re.compile("[a-z]\d[a-z]{2}")   #是否存在2个匹配项，用过的不能再用
print re.search(reg,"a3sd3f3asd3")

reg = re.compile("\d{2,4}")           #存在连续的2或者3或者4个数字
print re.search(reg,"a322647986")

reg = re.compile("\d{2}[a-z]\d{3}")   #存在连续的2个数字+1个字母+3个数字
print re.search(reg,"23k2346456")

reg = re.compile("\d+")               #一个或者以上的数字
print re.search(reg,"32")

reg = re.compile("\d{11}")            #连续11个数字
print re.search(reg,"134556633421231")

email = re.compile("\w+@\w+\.com")    #匹配邮箱
print re.search(email,"asdw@136.com")
```

###### 常用的正则表达式

```
1.  手机：/^0?1[3|4|5|8][0-9]\d{8}$/
2.  固话：/^0[\d]{2,3}-[\d]{7,8}$/
3.  电子邮箱：/^([a-z0-9_\.-]+)@([\da-z\.-]+)\.([a-z\.]{2,6})$/
    /^[a-z\d]+(\.[a-z\d]+)*@([\da-z](-[\da-z])?)+(\.{1,2}[a-z]+)+$/
4.  用户名：/^[a-z0-9_-]{3,16}$/
5.  密码：/^[a-z0-9_-]{6,18}$/
6.  URL：/^(https?:\/\/)?([\da-z\.-]+)\.([a-z\.]{2,6})([\/\w \.-]*)*\/?$/
```

##### 2.2 Python Re模块

```
以下为匹配所用函数
re.match(pattern, string[, flags])
re.search(pattern, string[, flags])
re.split(pattern, string[, maxsplit])
re.findall(pattern, string[, flags])
re.finditer(pattern, string[, flags])
re.sub(pattern, repl, string[, count])
re.subn(pattern, repl, string[, count])
```

##### 2.3 flag参数

参数flag是匹配模式，取值可以使用按位或运算符’|’表示同时生效，比如re.I | re.M。可选值有：

```
re.I: 忽略大小写
re.M: 多行模式，改变'^'和'$'的行为
re.S: 点任意匹配模式，改变'.'的行为
re.L: 使预定字符类 \w \W \b \B \s \S 取决于当前区域设定
re.U: 使预定字符类 \w \W \b \B \s \S \d \D 取决 于unicode定义的字符属性
re.X: 详细模式。这个模式下正则表达式可以是多行，忽略空白字符，并可以加入注释
```

##### 2.4 正则匹配符
|**字符**|**描述**|
|----|----|
|^|匹配输入字符串的开始位置。|
|$|匹配输入字符串的结束位置。$ 也匹配 ‘\n’ 或 ‘\r’ 之前的位置。|
|*|匹配前面的子表达式零次或多次。zo* 能匹配 “z” 以及 “zoo”。等价于{0,}。|
|+|匹配前面的子表达式一次或多次。’zo+’ 能匹配 “zo” 以及 “zoo”。等价于 {1,}。|
|?|匹配前面的子表达式零次或一次。”do(es)?” 可以匹配 “do” 或 “does” 。等价于 {0,1}。|
|\|匹配符：例如，’n’ 匹配字符 “n”。’\n’ 匹配一个换行符。序列 ‘\’ 匹配 “\” 而 “(” 则匹配 “(“。|
|x、y|匹配 x 或 y。例如，’z\food’ 能匹配 “z” 或 “food”。’(z\f)ood’ 则匹配 “zood” 或 “food”。|
|[xyz]|字符集合。匹配所包含的任意一个字符。例如， ‘[abc]’ 可以匹配 “plain” 中的 ‘a’。|
|[^xyz]|负值字符集合。匹配未包含的任意字符。例如， ‘[^abc]’ 可以匹配 “plain” 中的’p’、’l’、’i’、’n’。|
|[a-z]|字符范围。匹配指定范围内的任意字符。例如，’[a-z]’ 可以匹配 ‘a’ 到 ‘z’ 范围内的任意小写字母字符。|
|[^a-z]|负值字符范围。匹配任何不在指定范围内的任意字符。例如，’[^a-z]’ 可以匹配任何不在 ‘a’ 到 ‘z’ 范围内的任意字符。|
|\b|匹配一个单词边界，也就是指单词和空格间的位置。|
|\B|匹配非单词边界。|
|\cx|匹配由 x 指明的控制字符。例如， \cM 匹配一个 Control-M 或回车符。x 的值必须为 A-Z 或 a-z 之一。否则，将 c 视为一个原义的 ‘c’ 字符。|
|\d|匹配一个数字字符。等价于 [0-9]。|
|\D|匹配一个非数字字符。等价于 [^0-9]。|
|\f|匹配一个换页符。等价于 \x0c 和 \cL。|
|\n|匹配一个换行符。等价于 \x0a 和 \cJ。|
|\r|匹配一个回车符。等价于 \x0d 和 \cM。|
|\s|匹配任何空白字符，包括空格、制表符、换页符等等。等价于 [ \f\n\r\t\v]。|
|\S|匹配任何非空白字符。等价于 [^ \f\n\r\t\v]。|
|\t|匹配一个制表符。等价于 \x09 和 \cI。|
|\v|匹配一个垂直制表符。等价于 \x0b 和 \cK。|
|\w|匹配字母、数字、下划线。等价于’[A-Za-z0-9_]’。|
|\W|匹配非字母、数字、下划线。等价于 ‘[^A-Za-z0-9_]’。|
|\xn|匹配 n，其中 n 为十六进制转义值。十六进制转义值必须为确定的两个数字长。例如，’\x41’ 匹配 “A”。’\x041’ 则等价于 ‘\x04’ & “1”。正则表达式中可以使用 ASCII 编码。|
|**字符簇**|**描述**|
|----|----|
|[[:alpha:]]|任何字母|
|[[:digit:]]|任何数字|
|[[:alnum:]]|任何字母和数字|
|[[:space:]]|任何空白字符|
|[[:upper:]]|任何大写字母|
|[[:lower:]]|任何小写字母|
|[[:punct:]]|任何标点符号|
|[[:xdigit:]]|任何16进制的数字，相当于[0-9a-fA-F]|

##### 2.5 正则表达式

对网页进行正则表达式截取数据

```python
# 不用的信息  .*？替换，
 # 获取的信息 （.*？）替换
reg = re.compile(r'<p class="t1 ">.*?<a target="_blank" title="(.*?)" .*?'
                 r'<span class="t2"><a target="_blank" title="(.*?)" .*?'
                 r'<span class="t3">(.*?)</span>.*?'
                 r'<span class="t4">(.*?)</span>.*?'
                 r'<span class="t5">(.*?)</span>',re.S)       #re.S匹配多行
```

###### 案例1：获取51job职位信息（`python2.x`环境）

```python
import urllib2
import random
import re
import chardet
import xlwt

def get_html(url):         #获取网页内容
    USER_AGENTS = []       #浏览器(末尾附浏览器)
    proxies = []           #代理IP(末尾附IP)
    req = urllib2.Request(url)                   #设置url地址
    req.add_header('User-Agent',random.choice(USER_AGENTS))             #随机选取浏览器
    proxy_support=urllib2.ProxyHandler({"http":random.choice(proxies)}) #随机选取IP地址

    opener=urllib2.build_opener(proxy_support)   #获取网站访问的对象
    urllib2.install_opener(opener)

    res = urllib2.urlopen(req)                   #处理浏览器返回的对象
    html = res.read()
    return html

def get_Datalist(page_number,jobname):
    # 网址分析
    URL = "https://search.51job.com/list/020000,000000,0000,00,9,99,"\
           +str(jobname)+",2,"+str(page_number)+".html?lang=c&stype=&postchannel\
           =0000&workyear=99&cotype=99°reefrom=99&jobterm=99&companysize=\
           99&providesalary=99&lonlat=0%2C0&radius=-1&ord_field=0&confirmdate=\
           9&fromType=&dibiaoid=0&address=&line=&specialarea=00&from=&welfare="
    html = get_html(URL)                      # 传入需要分析网页
    code = chardet.detect(html)["encoding"]   # 获取网页编码
    html = html.decode(code).encode("utf-8")  # 解编码，转成utf-8编码
    # 设置正则表达式
    reg = re.compile(r'<p class="t1 ">.*?<a target="_blank" title="(.*?)" .*?'
                     r'<span class="t2"><a target="_blank" title="(.*?)" .*?'
                     r'<span class="t3">(.*?)</span>.*?'
                     r'<span class="t4">(.*?)</span>.*?'
                     r'<span class="t5">(.*?)</span>', re.S)
    result = re.findall(reg, html)
    return result

datalist = []                                   #全局数据列表

 #向全局变量添加数据
def solve_data(page_number,jobname):
    global datalist
    for k in range(int(page_number)):           #设置页数，循环获取
        data = get_Datalist(k+1,jobname)
        for i in data:
            datalist.append(i)

def save_Excel(jobname,filename):               #设置存储函数
    book = xlwt.Workbook(encoding="utf-8")      #创建工作簿
    sheet = book.add_sheet("51job"+str(jobname)+"职位信息")
    col = ('职位名','公司名','工作地点','薪资','发布时间')
    for i in range(len(col)):
        sheet.write(0,i,col[i])
    for i in range(len(datalist)):               #控制行
        for j in range(len(datalist[i])):        #控制列
            sheet.write(i+1,j,datalist[i][j])
    book.save(u'51job'+filename+u'职位信息.xls')

def save_Txt(filename):                          #存储文本文件
    for i in range(0, len(datalist)):
        data = datalist[i]
        with open(u'51job'+filename+u'职位信息.csv','a') as f:   #打开（创建）文本
            f.write(data[0]+'\t'+data[1]+'\t'+data[2]+'\t'+data[3]+'\t'+data[4]+'\n')
            f.close()

def main(jobname,page_number,filename):
    solve_data(page_number,jobname)
    save_Excel(jobname,filename)
    save_Txt(filename)

main("C++","2",u"C++语言职业")    #爬取职业，爬取多少页码，保存文件名
```

###### [（`python3`）环境，请点击](https://blog.csdn.net/wsp_1138886114/article/details/80633867)


### 三、利用requests及beautiful soup爬取数据

> 
#### 3.1  requests

requests是Python一个HTTP客户端库 

  ◆**下载及使用**
$~~~~~~$在C:\Python27\Scripts目录下输入cmd,执行`pip install requests`
$~~~~~~$或者：win+R输入cmd，执行cd C:\Python27\Scripts pip install requests 

  ◆**发送请求**
```
(1)requests.get(url,params = None,**kwargs)
    url:请求网址
    params:字典型请求参数
    kwargs:其他参数
    例子：response = response.get('http://www.baidu.com')

(2) requests.post(url,data = None,json = None,**keargs)
    例子：response = requests.post('http://http bin.org/post')
```

##### URls 中传递参数

```
r.text或r.content 获取相应内容。requests()会自动将内容转码
        Unicode字体都会无缝转码。替代了urllib2的urllib2.open().read()功能
    r.encoding来获取或设置网页编码
```

```python
import requests
r = requests.get("http://www.baidu.com",\
headers{'User-Aagent':'Mozilla/5.0','content-type':'text/html:charset = utf-8'}
print(r.url)
print(r.text)
print(r.encoding)
```

```
示例：
import requests
import random

USER_AGENTS = []                                #浏览器（末尾附浏览器）
proxies1 = {'HTTPS':'139.224.80.139:3128 ',
            'HTTPS':'122.72.18.34:80 ',
            'HTTPS':'120.76.231.27:3128',
            'HTTPS':'118.212.137.135:31288'}    #自动随机取IP

get = requests.get("http://www.baidu.com",
                   headers={"USER_AGENT":random.choice(USER_AGENTS)},
                   proxies=proxies1)
print(get)
>>>
<Response [200]>
```

#### 3.2、Beautiful Soup

```
Python内置HTML
安装并导入     easy install beautifulsoup4
              pip install beautifulsoup4
              from bs4 import BeautifulSoup

parse：解析器（html.parse、lxml、html5lib等）
```
|解析器|使用方法|优势|劣势|
|----|----|----|----|
|Python 标准库|BeautifulSoup(markup, “html.parser”)|Python内置标准库   执行速度适中   文档容错能力强|Python 2.7.3 or 3.2.2)前   的版本中文档容错能力差|
|lxml HTML 解析器|BeautifulSoup(markup, “lxml”)|速度快   文档容错能力强|需要安装C语言库|
|lxml XML 解析器|BeautifulSoup(markup, [“lxml”, “xml”])   BeautifulSoup(markup, “xml”)|速度快   唯一支持XML的解析器|需要安装C语言库|
|html5lib|BeautifulSoup(markup, “html5lib”)|最好的容错性   以浏览器的方式解析文档   生成HTML5格式的文档|速度慢   不依|

```
四大对象种类
    (1) BeautifulSoup 将复杂的HTML文档转换成一个复杂的树状结构，
        每个节点都是Python对象，所有对象可以归纳为4种
        -Tag        #标签两个属性：
                    name(输出的值为标签本身名字)
                    attrs（把标签所有属性放在一个字典内）
        -NavigableString
        -BeautifulSoup
        -Comment

    (2) NavigableString:获取标签内部的文字
        soup.标签名.string
        import resquests
        from bs4 import BeautifulSoup
        r = resquests.get("http://www.baidu.com",headers = {'User-Agent':'Mozilla/5.0})
        soup = BeautifulSoup(r.txt,"html.parser")
        print soup.prettify()
        print soup.script.parent    
        print soup.title.string     
    (3) Comment对象，是一个特殊类型的 NavigableString 对象，输出内容不包括注释符号
    (4) 常用方法
        #搜索当前Tag的所有tag子节点，判断是否符合过滤器条件
        1、find_all(name=None,attrs ={},recursive=True,text=None,limt=None,**kwargs)
        #……第一个tag子节点……
        2、find(name= None ,attrs = {},recursive = True ,text = None ,**kwargs)
        movieQuote = movieLi.find('span',attrs = {'class':'inq'})
        3直接子节点：content .children
        4所有子孙节点：descendants
        获取文本;   string 
        父节点：parent.parents（迭代器）
        兄弟节点：.next_string .previous_sibling 加s同上
        下一个与上一个要解析的元素：.next_elements previous_elements
```

```
from bs4 import BeautifulSoup

html = \"""
    <html><head><title>The Dormouse's story</title></head>
    <body>
    <p class="title" name="dromouse"><b>The Dormouse's story</b></p>
    <p class="story">Once upon a time there were three little sisters; and their names were
    <a href="http://example.com/elsie" class="sister" id="link1"><!-- Elsie --></a>,
    <a href="http://example.com/lacie" class="sister" id="link2">Lacie</a> and
    <a href="http://example.com/tillie" class="sister" id="link3">Tillie</a>;
    and they lived at the bottom of a well.</p>
    <p class="story">...</p>
"""
soup = BeautifulSoup(html,"html.parser")
print(soup.title)              #获取标签
print(soup.title.string)       #/soup.title.get_text()
print(soup.title.text)

print(soup.title.parent)       #获取父元素
p_children = soup.p.children   #获取子元素
for i in soup.p:               #遍历输出
    print(i)

print(soup.find('p'))          #查找第一个'p'
print(soup.find_all('p'))      #查找所有的'p'

print("第一个整个a元素：",soup.a)
print("元素a的名字：",soup.a.name)
print("元素a的属性：",soup.a.attrs)
print("元素a的id：",soup.a['id'])    #字典输出/soup.a.get('id')

print(soup.find_all("a",{'class':"sister"}))
```

##### 3.3 豆瓣爬虫案例

###### [案例：豆瓣电影Top250](https://blog.csdn.net/wsp_1138886114/article/details/80633867)

```
▶ 分析地址
url="https://movie.douban.com/top250?start="+str(P_num)+"&filter="

https://movie.douban.com/top250?start=0&filter=
https://movie.douban.com/top250?start=25&filter=

▶ 编写代码
 （1）创建getHymlTest(url,sterRow)函数
 #根据地址和开始页码获取对于页面的文本内容
 （2）设置datalist全局变量保存电影信息
 （3）创建getData(html)函数，根据传入的文本进行解析获取所需的数据
 （4）创建saveData(savePath)将数据保存到文件名为savepath的excel中
 （5）创建主程序：设置地址、0循环到250，获取url的文本信息，并从中获取需要的数据存入datalist，
      之后调用保存excel的方法。
 （6）调用主函数
```

```python
def getHtml(page_num):
    USER_AGENTS = []                              # 浏览器(末尾附浏览器)
    proxies = [{"HTTP": "117.63.78.64:6666"},
               {"HTTPS": "222.185.22.108:6666"}]  # 代理IP（临时的）
    url = "http://www.xx"+str(page_num)+"xx.com"  # 想要爬取的网页
    r = requests.get(url,
                     headers={"User-Agent":random.choice(USER_AGENTS)},
                     proxies=random.choice(proxies))
    code = chardet.detect(r.content)["encoding"]
    return r.content.decode(code)
```

```
爬取图像案例：
1、项目中创建photos文件夹（用于存放图片）
2、获取地址
3、获取网页图片列表所在ul
    images = oSoup.find('ul',attrs = {'class':'poster-col3 clearfix'})
4、获取列表所有img标签
5、创建文件夹，准备存储图片  #关于创建文件夹    http://www.runoob.com/python/os.file.methods.html
    if not os.path.isdir('photos'):
        os.mkdir('photos')
    os.chdir(os.path.join(os.getcwd(),'photos'))
6、为图片设置文件名，并存储图片
    exname = img['src'][-3:]
    image_name = str(x)+'.'exname
    urllib.request.urlretrieve(img['src'],image_name)
```

### 附：浏览器（USER_AGENTS）

> 
```bash
USER_AGENTS = [
    "Mozilla/5.0 (Windows NT 6.1; WOW64) AppleWebKit/537.1 (KHTML, like Gecko) Chrome/22.0.1207.1 Safari/537.1",
    "Mozilla/5.0 (X11; CrOS i686 2268.111.0) AppleWebKit/536.11 (KHTML, like Gecko) Chrome/20.0.1132.57 Safari/536.11",
    "Mozilla/5.0 (Windows NT 6.1; WOW64) AppleWebKit/536.6 (KHTML, like Gecko) Chrome/20.0.1092.0 Safari/536.6",
    "Mozilla/5.0 (Windows NT 6.2) AppleWebKit/536.6 (KHTML, like Gecko) Chrome/20.0.1090.0 Safari/536.6",
    "Mozilla/5.0 (Windows NT 6.2; WOW64) AppleWebKit/537.1 (KHTML, like Gecko) Chrome/19.77.34.5 Safari/537.1",
    "Mozilla/5.0 (X11; Linux x86_64) AppleWebKit/536.5 (KHTML, like Gecko) Chrome/19.0.1084.9 Safari/536.5",
    "Mozilla/5.0 (Windows NT 6.0) AppleWebKit/536.5 (KHTML, like Gecko) Chrome/19.0.1084.36 Safari/536.5",
    "Mozilla/5.0 (Windows NT 6.1; WOW64) AppleWebKit/536.3 (KHTML, like Gecko) Chrome/19.0.1063.0 Safari/536.3",
    "Mozilla/5.0 (Windows NT 5.1) AppleWebKit/536.3 (KHTML, like Gecko) Chrome/19.0.1063.0 Safari/536.3",
    "Mozilla/5.0 (Macintosh; Intel Mac OS X 10_8_0) AppleWebKit/536.3 (KHTML, like Gecko) Chrome/19.0.1063.0 Safari/536.3",
    "Mozilla/5.0 (Windows NT 6.2) AppleWebKit/536.3 (KHTML, like Gecko) Chrome/19.0.1062.0 Safari/536.3",
    "Mozilla/5.0 (Windows NT 6.1; WOW64) AppleWebKit/536.3 (KHTML, like Gecko) Chrome/19.0.1062.0 Safari/536.3",
    "Mozilla/5.0 (Windows NT 6.2) AppleWebKit/536.3 (KHTML, like Gecko) Chrome/19.0.1061.1 Safari/536.3",
    "Mozilla/5.0 (Windows NT 6.1; WOW64) AppleWebKit/536.3 (KHTML, like Gecko) Chrome/19.0.1061.1 Safari/536.3",
    "Mozilla/5.0 (Windows NT 6.1) AppleWebKit/536.3 (KHTML, like Gecko) Chrome/19.0.1061.1 Safari/536.3",
    "Mozilla/5.0 (Windows NT 6.2) AppleWebKit/536.3 (KHTML, like Gecko) Chrome/19.0.1061.0 Safari/536.3",
    "Mozilla/5.0 (X11; Linux x86_64) AppleWebKit/535.24 (KHTML, like Gecko) Chrome/19.0.1055.1 Safari/535.24",
    "Mozilla/5.0 (Windows NT 6.2; WOW64) AppleWebKit/535.24 (KHTML, like Gecko) Chrome/19.0.1055.1 Safari/535.24",
    "Mozilla/5.0 (Windows; U; Windows NT 5.1; en-US) AppleWebKit/531.21.8 (KHTML, like Gecko) Version/4.0.4 Safari/531.21.10",
    "Mozilla/5.0 (Windows; U; Windows NT 5.2; en-US) AppleWebKit/533.17.8 (KHTML, like Gecko) Version/5.0.1 Safari/533.17.8",
    "Mozilla/5.0 (Windows; U; Windows NT 6.1; en-US) AppleWebKit/533.19.4 (KHTML, like Gecko) Version/5.0.2 Safari/533.18.5",
    "Mozilla/5.0 (Windows; U; Windows NT 6.1; en-GB; rv:1.9.1.17) Gecko/20110123 (like Firefox/3.x) SeaMonkey/2.0.12",
    "Mozilla/5.0 (Windows NT 5.2; rv:10.0.1) Gecko/20100101 Firefox/10.0.1 SeaMonkey/2.7.1",
    "Mozilla/5.0 (Macintosh; U; Intel Mac OS X 10_5_8; en-US) AppleWebKit/532.8 (KHTML, like Gecko) Chrome/4.0.302.2 Safari/532.8",
    "Mozilla/5.0 (Macintosh; U; Intel Mac OS X 10_6_4; en-US) AppleWebKit/534.3 (KHTML, like Gecko) Chrome/6.0.464.0 Safari/534.3",
    "Mozilla/5.0 (Macintosh; U; Intel Mac OS X 10_6_5; en-US) AppleWebKit/534.13 (KHTML, like Gecko) Chrome/9.0.597.15 Safari/534.13",
    "Mozilla/5.0 (Macintosh; Intel Mac OS X 10_7_2) AppleWebKit/535.1 (KHTML, like Gecko) Chrome/14.0.835.186 Safari/535.1",
    "Mozilla/5.0 (Macintosh; Intel Mac OS X 10_6_8) AppleWebKit/535.2 (KHTML, like Gecko) Chrome/15.0.874.54 Safari/535.2",
    "Mozilla/5.0 (Macintosh; Intel Mac OS X 10_6_8) AppleWebKit/535.7 (KHTML, like Gecko) Chrome/16.0.912.36 Safari/535.7",
    "Mozilla/5.0 (Macintosh; U; Mac OS X Mach-O; en-US; rv:2.0a) Gecko/20040614 Firefox/3.0.0 ",
    "Mozilla/5.0 (Macintosh; U; PPC Mac OS X 10.5; en-US; rv:1.9.0.3) Gecko/2008092414 Firefox/3.0.3",
    "Mozilla/5.0 (Macintosh; U; Intel Mac OS X 10.5; en-US; rv:1.9.1) Gecko/20090624 Firefox/3.5",
    "Mozilla/5.0 (Macintosh; U; Intel Mac OS X 10.6; en-US; rv:1.9.2.14) Gecko/20110218 AlexaToolbar/alxf-2.0 Firefox/3.6.14",
    "Mozilla/5.0 (Macintosh; U; PPC Mac OS X 10.5; en-US; rv:1.9.2.15) Gecko/20110303 Firefox/3.6.15",
    "Mozilla/5.0 (Macintosh; Intel Mac OS X 10.6; rv:2.0.1) Gecko/20100101 Firefox/4.0.1",
    "Mozilla/4.0 (compatible; MSIE 6.0; Windows NT 5.1; SV1; AcooBrowser; .NET CLR 1.1.4322; .NET CLR 2.0.50727)",
    "Mozilla/4.0 (compatible; MSIE 7.0; Windows NT 6.0; Acoo Browser; SLCC1; .NET CLR 2.0.50727; Media Center PC 5.0; .NET CLR 3.0.04506)",
    "Mozilla/4.0 (compatible; MSIE 7.0; AOL 9.5; AOLBuild 4337.35; Windows NT 5.1; .NET CLR 1.1.4322; .NET CLR 2.0.50727)",
    "Mozilla/5.0 (Windows; U; MSIE 9.0; Windows NT 9.0; en-US)",
    "Mozilla/5.0 (compatible; MSIE 9.0; Windows NT 6.1; Win64; x64; Trident/5.0; .NET CLR 3.5.30729; .NET CLR 3.0.30729; .NET CLR 2.0.50727; Media Center PC 6.0)",
    "Mozilla/5.0 (compatible; MSIE 8.0; Windows NT 6.0; Trident/4.0; WOW64; Trident/4.0; SLCC2; .NET CLR 2.0.50727; .NET CLR 3.5.30729; .NET CLR 3.0.30729; .NET CLR 1.0.3705; .NET CLR 1.1.4322)",
    "Mozilla/4.0 (compatible; MSIE 7.0b; Windows NT 5.2; .NET CLR 1.1.4322; .NET CLR 2.0.50727; InfoPath.2; .NET CLR 3.0.04506.30)",
    "Mozilla/5.0 (Windows; U; Windows NT 5.1; zh-CN) AppleWebKit/523.15 (KHTML, like Gecko, Safari/419.3) Arora/0.3 (Change: 287 c9dfb30)",
    "Mozilla/5.0 (X11; U; Linux; en-US) AppleWebKit/527+ (KHTML, like Gecko, Safari/419.3) Arora/0.6",
    "Mozilla/5.0 (Windows; U; Windows NT 5.1; en-US; rv:1.8.1.2pre) Gecko/20070215 K-Ninja/2.1.1",
    "Mozilla/5.0 (Windows; U; Windows NT 5.1; zh-CN; rv:1.9) Gecko/20080705 Firefox/3.0 Kapiko/3.0",
    "Mozilla/5.0 (X11; Linux i686; U;) Gecko/20070322 Kazehakase/0.4.5",
    "Mozilla/5.0 (X11; U; Linux i686; en-US; rv:1.9.0.8) Gecko Fedora/1.9.0.8-1.fc10 Kazehakase/0.5.6",
    "Mozilla/5.0 (Windows NT 6.1; WOW64) AppleWebKit/535.11 (KHTML, like Gecko) Chrome/17.0.963.56 Safari/535.11",
    "Mozilla/5.0 (Macintosh; Intel Mac OS X 10_7_3) AppleWebKit/535.20 (KHTML, like Gecko) Chrome/19.0.1036.7 Safari/535.20",
    "Opera/9.80 (Macintosh; Intel Mac OS X 10.6.8; U; fr) Presto/2.9.168 Version/11.52",
    "Mozilla/5.0 (Windows NT 6.1; WOW64) AppleWebKit/536.11 (KHTML, like Gecko) Chrome/20.0.1132.11 TaoBrowser/2.0 Safari/536.11",
    "Mozilla/5.0 (Windows NT 6.1; WOW64) AppleWebKit/537.1 (KHTML, like Gecko) Chrome/21.0.1180.71 Safari/537.1 LBBROWSER",
    "Mozilla/5.0 (compatible; MSIE 9.0; Windows NT 6.1; WOW64; Trident/5.0; SLCC2; .NET CLR 2.0.50727; .NET CLR 3.5.30729; .NET CLR 3.0.30729; Media Center PC 6.0; .NET4.0C; .NET4.0E; LBBROWSER)",
    "Mozilla/4.0 (compatible; MSIE 6.0; Windows NT 5.1; SV1; QQDownload 732; .NET4.0C; .NET4.0E; LBBROWSER)",
    "Mozilla/5.0 (Windows NT 6.1; WOW64) AppleWebKit/535.11 (KHTML, like Gecko) Chrome/17.0.963.84 Safari/535.11 LBBROWSER",
    "Mozilla/4.0 (compatible; MSIE 7.0; Windows NT 6.1; WOW64; Trident/5.0; SLCC2; .NET CLR 2.0.50727; .NET CLR 3.5.30729; .NET CLR 3.0.30729; Media Center PC 6.0; .NET4.0C; .NET4.0E)",
    "Mozilla/5.0 (compatible; MSIE 9.0; Windows NT 6.1; WOW64; Trident/5.0; SLCC2; .NET CLR 2.0.50727; .NET CLR 3.5.30729; .NET CLR 3.0.30729; Media Center PC 6.0; .NET4.0C; .NET4.0E; QQBrowser/7.0.3698.400)",
    "Mozilla/4.0 (compatible; MSIE 6.0; Windows NT 5.1; SV1; QQDownload 732; .NET4.0C; .NET4.0E)",
    "Mozilla/4.0 (compatible; MSIE 7.0; Windows NT 5.1; Trident/4.0; SV1; QQDownload 732; .NET4.0C; .NET4.0E; 360SE)",
    "Mozilla/4.0 (compatible; MSIE 6.0; Windows NT 5.1; SV1; QQDownload 732; .NET4.0C; .NET4.0E)",
    "Mozilla/4.0 (compatible; MSIE 7.0; Windows NT 6.1; WOW64; Trident/5.0; SLCC2; .NET CLR 2.0.50727; .NET CLR 3.5.30729; .NET CLR 3.0.30729; Media Center PC 6.0; .NET4.0C; .NET4.0E)",
    "Mozilla/5.0 (Windows NT 5.1) AppleWebKit/537.1 (KHTML, like Gecko) Chrome/21.0.1180.89 Safari/537.1",
    "Mozilla/5.0 (Windows NT 6.1; WOW64) AppleWebKit/537.1 (KHTML, like Gecko) Chrome/21.0.1180.89 Safari/537.1",
    "Mozilla/5.0 (iPad; U; CPU OS 4_2_1 like Mac OS X; zh-cn) AppleWebKit/533.17.9 (KHTML, like Gecko) Version/5.0.2 Mobile/8C148 Safari/6533.18.5",
    "Mozilla/5.0 (Windows NT 6.1; Win64; x64; rv:2.0b13pre) Gecko/20110307 Firefox/4.0b13pre",
    "Mozilla/5.0 (X11; Ubuntu; Linux x86_64; rv:16.0) Gecko/20100101 Firefox/16.0",
    "Mozilla/5.0 (Windows NT 6.1; WOW64) AppleWebKit/537.11 (KHTML, like Gecko) Chrome/23.0.1271.64 Safari/537.11",
    "Mozilla/5.0 (X11; U; Linux x86_64; zh-CN; rv:1.9.2.10) Gecko/20100922 Ubuntu/10.10 (maverick) Firefox/3.6.10",
    "Mozilla/5.0 (Windows NT 10.0; Win64; x64) AppleWebKit/537.36 (KHTML, like Gecko) Chrome/58.0.3029.110 Safari/537.36",
    ]
```


### 附：IP地址(临时的）获取更多 IP 你可以访问：[西刺代理IP](http://www.xicidaili.com/)

> 
```bash
proxies = ["123.138.89.1339:999",
           "101.132.122.230:3128",
           "222.186.12.102:57624"]
```








