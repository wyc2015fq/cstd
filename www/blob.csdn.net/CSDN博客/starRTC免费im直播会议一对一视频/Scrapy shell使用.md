# Scrapy shell使用 - starRTC免费im直播会议一对一视频 - CSDN博客
2017年11月23日 16:17:44[starRTC免费IM直播会议一对一视频](https://me.csdn.net/elesos)阅读数：197
个人分类：[scrapy](https://blog.csdn.net/elesos/article/category/7108617)
注意：容易出现403错误，实际爬取时不会出现。
response -
 a [**Response**](https://doc.scrapy.org/en/latest/topics/request-response.html#scrapy.http.Response) object
 containing the last fetched page
>>>**response****.****xpath**('//title/text()')**.**extract()
 return a list of selectors
**>>>for**index, link**in**enumerate(links):
...
args**=** (index,
link**.**xpath('@href')**.**extract(),
link**.**xpath('img/@src')**.**extract())...
**print**'Link number
%d points to url %s and image %s'**%**args
Link number
0 points to url [u'image1.html'] and image [u'image1_thumb.jpg']Link
 number 1 points to url [u'image2.html'] and image
 [u'image2_thumb.jpg']Link number
2 points to url [u'image3.html'] and image [u'image3_thumb.jpg']Link
 number 3 points to url [u'image4.html'] and image [u'image4_thumb.jpg']Link
 number 4
points to url [u'image5.html'] and image [u'image5_thumb.jpg']
enumerate() 函数一般用在 for 循环当中。
**普通的 for 循环**
>>>i
 = 0>>>
seq =
['one',
'two',
'three']>>>
forelementinseq:...
printi,
seq[i]...
i +=1...
0one1two2three
**for 循环使用 enumerate**
>>>seq
 = ['one',
'two',
'three']>>>
fori, elementinenumerate(seq):...
printi,
seq[i]...
0one1two2three
suppose you want to extract all <p> elements
 inside <div> elements. First, you would get all <div> elements:
>>>
divs**=**response**.**xpath('//div')
note the dot prefixing the .//p XPath):
>>>
**for**p**in**divs**.**xpath('.//p'):
*# extracts all <p>**inside*...
**print**p**.**extract()
Another common case would be to extract all
direct <p> children:
>>>
**for**p**in**divs**.**xpath('p'):...
**print**p**.**extract()
在程序中使用shell
**from**scrapy.shell**import**inspect_responseinspect_response(response,
self)
Ctrl-D (or Ctrl-Z in Windows) to exit the shell and resume the crawling:
xpath最外层最好用单引号！
shell 本地html，方便 调试（但别取名为index.html）
scrapyshell**./**path**/**to**/**file**.**html
 ,即使在本目录，也必须要加./，不能直接 shell file.htmlscrapyshell**../**other**/**path**/**to**/**file**.**htmlscrapyshell**/**absolute**/**path**/**to**/**file**.**html

