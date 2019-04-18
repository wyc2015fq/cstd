# Python入门--13--爬虫一 - 默槑 - 博客园







# [Python入门--13--爬虫一](https://www.cnblogs.com/modaidai/p/6901546.html)





URL的格式一般为（带方括号的是可选的）：

protocol://hostname[:port]/path/[;parameters][?query]#fragment



URL由三部分组成：

　　第一部分是协议：http　https  ftp  file  ed2k

　　第二部分是存放资源的放武器的域名系统或IP地址（有时候要包含端口号）

　　　　各种传输协议都有默认的端口号，http的默认端口号就是80

　　第三部分是资源的具体地址，如目录或文件名等



python3.0版本之后就把urllib和urllib2合并了，变为一个urllib包

下面先用urllib.request写个小程序

import urllib.request

wangye = urllib.request.urlopen('http://www.baidu.com')

html=wangye.read()

print (html)

##输出的全是乱码是不是  ，因为他是二进制的格式

import urllib.request

wangye = urllib.request.urlopen('http://www.baidu.com')

html=wangye.read()

html=html.decode('utf-8')

print (html)

##这就转化为你在浏览器的网页里面按F12的效果，是他们的源代码



一、抓取一只猫

import urllib.request

wangye=urllib.request.urlopen('http://placekitten.com/g/500/500')

cat_img = wangye.read()

with open('cat_500_600'+'.jpg','wb') as f:

　　f.write(cat_img)



#reques有两种用法 一种是'urlopen'后面加上网址，另外一种是urllib.request.Request加上网址



import urllib.request

pro_wangye=urllib.request.Request('http://placekitten.com/g/500/500')

wangye=urllib.request.open(pro_wangye)

cat_img = wangye.read()

with open('cat_500_600'+'.jpg','wb') as f:

　　f.write(cat_img)

所以直接用urlopen就好

二、一些函数
wangye.geturl()　　#输出的是http://placekitten.com/g/500/500

print(wangye.info())　　#输出一堆网络的属性什么的

wangye.getcode()　　#如果输出200说明服务器状态没问题，正常相应

三、在编一个小程序：有道翻译

 注意：

1、这个urlopen有个参数data，如果其被赋值，则函数就会post，来取代get

2、data的格式必须是特定的，需要使用：urllib.parse.urlencode()进行一下编码，编译成url的格式

　　　　而且这个urllib.parse是一个模块的 需要载入



























四、一些知识点

remote address：服务器ip地址+打开的端口号

request url：打开的地址

status code：服务器的状态，200是正常，404是页面不见了

request header：客户端、浏览器的意思 #一般通过这个里面的users-agent来判断是代码访问还是人的访问

　　　　　　使用:  1、req=urllib.request.Request(url,data,header) 　　　　#提前写好header,这是个字典

　　　　或者使用：2、req=urllib.request.Request(url,data)

　　　　　　　　　　　req.add_header('UserAgent','Mozilla/5.0(WindowsNT6.1;WOW64)AppleWebKit/537.36 　　　　　　　　　　　　　　　　　　　　　　　　(KHTML, like Gecko) Chrome/50.0.2661.102 Safari/537.36')



JSON：这是一种轻量级的数据交换格式，说白了就是这里就是字符串把Python的数据结构封装起来，便于储存和使用



HTTP有好几种方法（GET、POST、PUT、HEAD、DELETE、OPTIONS、CONNECT），请问如何知道python使用的哪一种：

答：使用get_method()方法获取request对象具体使用那种方法访问服务器，当request的Data参数被赋值的时候，get_method()返回的'POST'，否则返回'GET'














