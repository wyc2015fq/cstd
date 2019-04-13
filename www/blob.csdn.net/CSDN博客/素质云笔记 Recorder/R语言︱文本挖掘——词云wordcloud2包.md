
# R语言︱文本挖掘——词云wordcloud2包 - 素质云笔记-Recorder... - CSDN博客

2016年06月09日 11:15:08[悟乙己](https://me.csdn.net/sinat_26917383)阅读数：33782所属专栏：[R语言与自然语言处理](https://blog.csdn.net/column/details/13670.html)




**每每以为攀得众山小，可、每每又切实来到起点，大牛们，缓缓脚步来俺笔记葩分享一下吧，please~**![](https://img-blog.csdn.net/20161213101203247)
———————————————————————————
笔者看到微信公众号探数寻理中提到郎大为Chiffon老师的wordcloud2，于是尝鲜准备用一下。但是在下载的时候，遇见很多问题，安装问题困扰着。。。
包中函数本身很好用，很简单，而且图形众多。

————————————————————————————————————————————

# 一、wordcloud2包的安装

官方郎大为老师github博客链接：https://github.com/Lchiffon/wordcloud2
老师的中文博客链接：http://lchiffon.github.io/2016/06/01/wordcloud2.html

官方下载方法：
`install.packages('devtools')
devtools::install_github("lchiffon/wordcloud2")`
这里我是下载不了，出现以下的报错：
`Downloading GitHub repo lchiffon/wordcloud2@master
from URL https://api.github.com/repos/lchiffon/wordcloud2/zipball/master
Error in curl::curl_fetch_memory(url, handle = handle) : 
  Problem with the SSL CA cert (path? access rights?)`
然后借鉴了博客（[ 安装包（上传数据）失败时的解决方法](http://blog.csdn.net/wangishero/article/details/50859670)），

`library(RCurl)
library(httr)
set_config( config( ssl_verifypeer = 0L ) )`
解决了curl这个问题。但是又出现报错：
`Error in read.dcf(file.path(pkgname, "DESCRIPTION"), c("Package", "Type")) : 
  无法打开链结`
于是找到了R-3.3.0中一个叫jsonlite的包，删除并重新安装之后。直接devtools::install_github("lchiffon/wordcloud2")就可以顺利安装成功了。
问题的关键可能是：jsonlite这个包以及curl中的一些设置。

————————————————————————————————————————————


# 二、
# wordcloud2函数说明

`wordcloud2(data, size = 1, minSize = 0, gridSize =  0,
    fontFamily = NULL, fontWeight = 'normal',
    color = 'random-dark', backgroundColor = "white",
    minRotation = -pi/4, maxRotation = pi/4, rotateRatio = 0.4,
    shape = 'circle', ellipticity = 0.65, widgetsize = NULL)`
常用参数：
（1）data：词云生成数据，包含具体词语以及频率；
（2）size：字体大小，默认为1，一般来说该值越小，生成的形状轮廓越明显；
（3）fontFamily：字体，如‘微软雅黑’；
（4）fontWeight：字体粗细，包含‘normal’，‘bold’以及‘600’；；
（5）color：字体颜色，可以选择‘random-dark’以及‘random-light’，其实就是颜色色系；
（6）backgroundColor：背景颜色，支持R语言中的常用颜色，如‘gray’，‘blcak’，但是还支持不了更加具体的颜色选择，如‘gray20’；
（7）minRontatin与maxRontatin：字体旋转角度范围的最小值以及最大值，选定后，字体会在该范围内随机旋转；
（8）rotationRation：字体旋转比例，如设定为1，则全部词语都会发生旋转；
（9）shape：词云形状选择，默认是‘circle’，即圆形。还可以选择‘cardioid’（苹果形或心形），‘star’（星形），‘diamond’（钻石），‘triangle-forward’（三角形），‘triangle’（三角形），‘pentagon’（五边形）；
————————————————————————————————————————————


# 三、官方包中的几个案例

## 1、案例一：星星图

`library(wordcloud2)
wordcloud2(demoFreq, size = 1,shape = 'star')`
![](https://img-blog.csdn.net/20160609110513734)

## 案例二：中文词云

`wordcloud2(demoFreqC, size = 2, fontFamily = "微软雅黑",
           color = "random-light", backgroundColor = "grey")`
![](https://img-blog.csdn.net/20160609110608564)



## 案例三：

`wordcloud2(demoFreq, size = 2, minRotation = -pi/2, maxRotation = -pi/2)`
![](https://img-blog.csdn.net/20160609110848301)

## 案例四：

`wordcloud2(demoFreq, size = 2, minRotation = -pi/6, maxRotation = -pi/6,
  rotateRatio = 1)`
![](https://img-blog.csdn.net/20160609110921893)


## 案例五：浩彬老撕的高考案例

数据来源：链接: http://pan.baidu.com/s/1jI4gEnc 密码: uids

`data <- read.csv('c:/data.csv', sep="," ,header = T)
#读取原始数据
data
#检查数据，查看是否存在乱码的情况
wordcloud2(data, size = 1, shape='star',color = 'random-dark',
backgroundColor = "white",fontFamily = "微软雅黑")
#绘制文字云，其中data就是我们读取的数据，size是对应文字大小，shape是绘制形状`
![](https://img-blog.csdn.net/20160609111046659)




## 案例六：浩彬老撕的高考案例


`wordcloud2(demoFreqC, size = 1,color = 'random-light',
backgroundColor = "gray", fontWeight='bold',fontFamily = "微软雅黑",
minRotation = -pi/3, maxRotation = pi/3,rotateRatio = 0.8)`
在上面例子，我们重新设定了字体为粗体，字体旋转角度在正负60度之间，旋转比例为80%。
![](https://img-blog.csdn.net/20160609111137894)


——————————————————————————————————————————————————————————

# 6月12日更新——自定义图片+文字云

从浩彬老师公众号，看到wordcloud2更新了自定义图片形状、文字词云形状。
已经安装过wordcloud2包的童鞋，因为已经更新了，所以需要重新安装。

`install.packages('devtools')
devtools::install_github("lchiffon/wordcloud2")`

## 1、自定义图片代码

`batman = system.file("examples/batman.png",package = "wordcloud2")
###读取形状图片，注意图片默认放在wordclou2的sample包中，浩彬老撕的路径如下："d:/Program Files/R/R-3.3.0/library/wordcloud2/examples/batman.png"
wordcloud2(demoFreq, figPath = batman, size = 1,color = "black")
###绘制云此图，其中demoFreg即为所用的数据，figPath = batman为所用图片`
其中需要把图片，放到wordcloud2的examples包中，可以直接调用。
![](https://img-blog.csdn.net/20160612153429512)



## 2、文字云
有些时候，我们除了需要自定义形状，可能也需要形成文字，这就需要用到wordcloud2中的新函数letterCloud
代码：
`letterCloud(demoFreq, word = "高考", wordSize = 2,color = 'random-dark',backgroundColor = "snow")
###其中demoFreqC为需要用的词语以及词频数据，word是需要可视化的文字`
![](https://img-blog.csdn.net/20160612153515731)
————————————————————————————————————————————————————————

# 应用一：报错Error in gsub

`Error in gsub("</", "\\u003c/", payload, fixed = TRUE) : 
  invalid multibyte string at '<88>,<32>3754","鍟婂晩","杩欎釜","鍠滄","浠€涔<88>,6390","鎴戠殑","鍙埍","鍝堝搱","鍗фЫ","寮瑰箷","鎬庝箞","涓€涓<aa>,4357","浣犱滑","琛ㄧ櫧","鎰熻","杩欎箞","鍙互","娌℃湁","瑙夊緱","鍓嶉潰","閭ｄ釜","鐭ラ亾","涓轰粈涔<88>,2586","鑷繁","杩欐牱","濂界湅","鐪嬪埌","鎵垮寘","鍑烘潵","濂藉儚","涓€鐩<b4>,2051","鍘夊浜<86>,2007","寮€濮<8b>,1975","鎴戜滑","鑰佽強","鑰佸笀","杩欓噷","宸茬粡","鐜板湪","鏃跺€<99>,1739","澹伴煶","濡瑰瓙","鍝ュ摜","鍘夊浜嗘垜鐨<84>,1586","鑰佸﹩","姝ょ敓","鏈夌偣","涓€鏍<b7>,1497","琛ㄧず","蹇冪柤","璁告効","鐪熺殑","閭ｄ箞","涓嶉敊","绛夌瓑","鍒蛋","涓嶈兘","杩欑","纭竵","鍘夊","濂藉ソ","鎵€浠<a5>,1234","瀛楀箷","涓浗","鍏跺疄","娓告垙","鍥犱负","灞呯劧","涓€璧<b7>,1168","澶у","浠ヤ负","搴旇","璧锋潵","涓€涓<8b>,1132","涓嶈","灏村艾","濂崇","鍥炴潵","涓栫晫","瀛╁瓙","濡傛灉","鍗佸厓","琛ｆ湇","鍙戠幇","涓轰簡","濡堝","鐖哥埜","鍚堝奖","浣嗘槸","瑙嗛","绯诲垪","濮愬","婕備寒","绗竴`

笔者在尝试wordcloud2的时候，本来是从来没有遇到过报错问题，但是公司电脑里面是低版本的R（3.1），随后就出现了这样的报错， 是格式问题，需要把中文格式转化为UTF8才能塞入wordcloud2当中。于是笔者也在低版本下尝试了很多种可能性譬如：
1、强行转化格式成UTF8，变成了一对文字乱码；
2、导出后转化为UTF8格式，导出没问题，但是导入的时候出现了一堆乱码的情况，还是失败；

于是乎，换了高版本的R之后，问题自然而然就解决了...  原因呢，笔者自然是不知道为什么...

**来看看官方、作者的解决方案：**
方法一:将文字转化为UTF-8
方法二：使用前先：Sys.setlocale("LC_CTYPE","eng")
笔者应用方法二完美解决问题了




**每每以为攀得众山小，可、每每又切实来到起点，大牛们，缓缓脚步来俺笔记葩分享一下吧，please~**![](https://img-blog.csdn.net/20161213101203247)
———————————————————————————


