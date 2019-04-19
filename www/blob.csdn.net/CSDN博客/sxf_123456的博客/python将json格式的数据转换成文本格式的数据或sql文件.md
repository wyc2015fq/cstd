# python将json格式的数据转换成文本格式的数据或sql文件 - sxf_123456的博客 - CSDN博客
2017年08月13日 16:31:39[sxf_0123](https://me.csdn.net/sxf_123456)阅读数：329
个人分类：[python](https://blog.csdn.net/sxf_123456/article/category/7078196)

python如何将json格式的数据快速的转化成指定格式的数据呢？或者转换成sql文件？
下面的例子是将json格式的数据准换成以#_#分割的文本数据，也可用于生成sql文件。
```
[root@bogon tutorial]# vi json2txt.py 
#-*- coding: UTF-8 -*-
import json
data = []
with open('./tencent_test.json') as f:
    for line in f:
        data.append(json.loads(line))
#print json.dumps(data, ensure_ascii=False)
import codecs
file_object = codecs.open('tencent.txt', 'w' ,"utf-8")
str = "\r\n"
splitstr = "#_#"
for item in data:
    #print json.dumps(item)
    #str = str + "insert into tencent(name,catalog,workLocation,recruitNumber,detailLink,publishTime) values "
    #str = str + "'%s','%s','%s','%s','%s'\r\n" % (item['parentTitle'],item['parentLink'],item['author'],item['link'],item['title'])
    #print json.loads(item['author']) + "\r\n"
    str = "%s#_#%s#_#%s#_#%s#_#%s\r\n" % (item['parentTitle'],item['parentLink'],item['author'],item['link'],item['title'].strip())
    file_object.write(str)
#import codecs
#file_object = codecs.open('tencent.txt', 'w' ,"utf-8")
#file_object.write(str)
file_object.close()
print "success"
```
注：如果数据量过大不宜全部放在str里一次性写入文件。可以考虑逐行写入，或者到达一定程度时批量写入。
另外：python通过strip(rm)替换字符串，当rm为空时，默认删除空白符（包括'\n', '\r',  '\t',  ' ')。
测试：
json格式的数据
```
[root@bogon tutorial]# vi tencent_test.json 
{"author": "作者", "parentTitle": "父标题", "title": "标题", "pageUrl": "pageurl", "link": "linkurl", "parentLink": "parenturl"}
{"author": "作者", "parentTitle": "父标题", "title": "标题", "pageUrl": "pageurl", "link": "linkurl", "parentLink": "parenturl"}
{"author": "作者", "parentTitle": "父标题", "title": "标题", "pageUrl": "pageurl", "link": "linkurl", "parentLink": "parenturl"}
{"author": "作者", "parentTitle": "父标题", "title": "标题", "pageUrl": "pageurl", "link": "linkurl", "parentLink": "parenturl"}
{"author": "作者", "parentTitle": "父标题", "title": "标题", "pageUrl": "pageurl", "link": "linkurl", "parentLink": "parenturl"}
{"author": "作者", "parentTitle": "父标题", "title": "标题", "pageUrl": "pageurl", "link": "linkurl", "parentLink": "parenturl"}
{"author": "作者", "parentTitle": "父标题", "title": "标题", "pageUrl": "pageurl", "link": "linkurl", "parentLink": "parenturl"}
{"author": "作者", "parentTitle": "父标题", "title": "标题", "pageUrl": "pageurl", "link": "linkurl", "parentLink": "parenturl"}
{"author": "作者", "parentTitle": "父标题", "title": "标题", "pageUrl": "pageurl", "link": "linkurl", "parentLink": "parenturl"}
{"author": "作者", "parentTitle": "父标题", "title": "标题", "pageUrl": "pageurl", "link": "linkurl", "parentLink": "parenturl"}
{"author": "作者", "parentTitle": "父标题", "title": "标题", "pageUrl": "pageurl", "link": "linkurl", "parentLink": "parenturl"}
```
运行python json2txt.py
```
[root@bogon tutorial]# python json2txt.py 
success
[root@bogon tutorial]# more tencent.txt 
父标题#_#parenturl#_#作者#_#linkurl#_#标题
父标题#_#parenturl#_#作者#_#linkurl#_#标题
父标题#_#parenturl#_#作者#_#linkurl#_#标题
父标题#_#parenturl#_#作者#_#linkurl#_#标题
父标题#_#parenturl#_#作者#_#linkurl#_#标题
父标题#_#parenturl#_#作者#_#linkurl#_#标题
父标题#_#parenturl#_#作者#_#linkurl#_#标题
父标题#_#parenturl#_#作者#_#linkurl#_#标题
父标题#_#parenturl#_#作者#_#linkurl#_#标题
父标题#_#parenturl#_#作者#_#linkurl#_#标题
父标题#_#parenturl#_#作者#_#linkurl#_#标题
```
