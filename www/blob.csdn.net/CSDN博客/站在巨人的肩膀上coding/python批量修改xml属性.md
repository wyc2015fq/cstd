# python批量修改xml属性 - 站在巨人的肩膀上coding - CSDN博客





2018年01月25日 21:48:26[coding思想](https://me.csdn.net/LOVE1055259415)阅读数：8090








今天来说说xml那些事儿.如何批量修改指定文件夹下的xml文件的指定属性.分三步走,首先,我们先看看如何读写单个

的xml文件;第二步,来看看如何遍历指定文件夹下的所有文件,获取到所有文件的文件名;第三步,我们来看看一二之间

该如何衔接.好,lets do it


step1:对单个xml文件进行读写


给定一个xml文件:

```
<?xml version="1.0" encoding="utf-8"?>
<catalog>
       <maxid>4</maxid>
       <login username="pytest" passwd='123456'>
            　　<caption>Python</caption>
             <item id="4">
                    <caption>测试</caption>
            </item>
    </login>
    <item id="2">
            <caption>Zope</caption>
    </item>
</catalog>
```

来看看代码,怎么读取里面的属性(大家先照着注释理解一遍,有空我再来详细说明)



```python
#coding=utf-8
import xml.dom.minidom

#打开xml文档
dom=xml.dom.minidom.parse('test.xml')

#得到文档元素对象
root=dom.documentElement
print root.nodeName
print root.nodeValue
print root.nodeType
print root.ELEMENT_NODE

#1.获取maxid 这一node名字(没有属性值),如何获取里面的文本?
bb=root.getElementsByTagName('maxid')
b=bb[0]
print b.nodeName

#2.获取login 这一node名字及相关属性值
login=root.getElementsByTagName('login')
login=login[0]  #获取login的相关属性值
un=login.getAttribute("username")
print un
pd=login.getAttribute("passwd")
print pd
#修改先关属性值


#3.获取节点名为item的相关属性值
item=root.getElementsByTagName('item') #获取了所有名字为item的node
item=item[0]  #拿到第一个item,获取相关属性值
i=item.getAttribute("id")#获取id的值
print i

#4.获取标签对之间的数据,并修改为新的值
caption=root.getElementsByTagName('caption')
c0=caption[0]
print c0.firstChild.data  #firstChild属性返回被选节点的第一个子节点,.data表示获取该节点数据

c1=caption[1]
print c1.firstChild.data

c2=caption[2]  #caption节点有三个!!!
print c2.firstChild.data
#修改标签对之间的数据,直接对节点数据赋值
c2.firstChild.data='dhhdlh'
print c2.firstChild.data
```

好了,看完了demo,我们现在来实战操练一番,使用通用的VOC2007标注数据集,xml文件长这个样子:



```
<annotation verified="no">
  <folder>row_img</folder>
  <filename>000002</filename>
  <path>/home/nvidia/labelImg-master/img_change/row_img/000002.jpg</path>
  <source>
    <database>Unknown</database>
  </source>
  <size>
    <width>1200</width>
    <height>800</height>
    <depth>3</depth>
  </size>
  <segmented>0</segmented>
  <object>
    <name>qwe</name>
    <pose>Unspecified</pose>
    <truncated>0</truncated>
    <difficult>0</difficult>
    <bndbox>
      <xmin>513</xmin>
      <ymin>265</ymin>
      <xmax>921</xmax>
      <ymax>663</ymax>
    </bndbox>
  </object>
  <object>
    <name>wieoiwpe</name>
    <pose>Unspecified</pose>
    <truncated>0</truncated>
    <difficult>0</difficult>
    <bndbox>
      <xmin>513</xmin>
      <ymin>265</ymin>
      <xmax>921</xmax>
      <ymax>663</ymax>
    </bndbox>
  </object>
</annotation>
```

那么,我们该如何修改呢?请看:



```python
#coding=utf-8
import xml.dom.minidom

###批量读取xml文件



###读取单个xml文件
dom=xml.dom.minidom.parse('000002.xml')

root=dom.documentElement

#获取标签对name/pose之间的值
name=root.getElementsByTagName('name')
pose=root.getElementsByTagName('pose')
#原始信息
print '原始信息'
n0=name[0]
print n0.firstChild.data
n1=name[1]
print n1.firstChild.data

p0=pose[0]
print p0.firstChild.data
p1=pose[1]
print p1.firstChild.data

#修改标签对之间的值
n0.firstChild.data='circle'
n1.firstChild.data='circle'

p0.firstChild.data='ok'
p1.firstChild.data='ok'
#打印输出
print '修改后的 name'
print n0.firstChild.data
print n1.firstChild.data
print '修改后的 pose'
print p0.firstChild.data
print p1.firstChild.data
```

好了,现在我们学会了如何对单个文件进行修改,那么多个文件呢?

step2:遍历指定路径下的文件:



```python
#coding=utf-8
import os
import os.path
import xml.dom.minidom

path="/home/nvidia/xmlReader/xml/"
files=os.listdir(path)  #得到文件夹下所有文件名称
s=[]
for xmlFile in files: #遍历文件夹
    if not os.path.isdir(xmlFile): #判断是否是文件夹,不是文件夹才打开
        print xmlFile
```

(path下我放的是几个xml文件),打印xmlFile我们发现是这样的:

![](https://img-blog.csdn.net/20180125215457346?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvTE9WRTEwNTUyNTk0MTU=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)


看到没,看到这个的话就说明我们已经成功一半了!!!接下来我们把之前写的读取单个xml文件的代码放进去



```python
#coding=utf-8
import os
import os.path
import xml.dom.minidom

path="/home/nvidia/xmlReader/xml/"
files=os.listdir(path)  #得到文件夹下所有文件名称
s=[]
for xmlFile in files: #遍历文件夹
    if not os.path.isdir(xmlFile): #判断是否是文件夹,不是文件夹才打开
        print xmlFile

	#TODO
	#xml文件读取操作

	#将获取的xml文件名送入到dom解析
	dom=xml.dom.minidom.parse(xmlFile)
        root=dom.documentElement
        #获取标签对name/pose之间的值
        name=root.getElementsByTagName('name')
        pose=root.getElementsByTagName('pose')
        #原始信息
        print '原始信息'
        n0=name[0]
	print n0.firstChild.data
	n1=name[1]
	print n1.firstChild.data

	p0=pose[0]
	print p0.firstChild.data
	p1=pose[1]
	print p1.firstChild.data
```

直接运行,报错!!我...

不要急,我们一点点来解决,,遇到问题是很正常的嘛!!!首先我们看看遇到什么错?

![](https://img-blog.csdn.net/20180125215425773?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvTE9WRTEwNTUyNTk0MTU=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)


打印除了000001.xml但是在实际读取的时候出错了!!还说找不到在这个文件?why??仔细想想发现,这里可能要传入的是

每个xml文件的具体路径,有了这个想法之后我们再来看看:

这个时候就设涉及到Python路径拼接的知识了:



```python
path="/home/nvidia/xmlReader/xml/"
xmlFile也是几个字符串
os.path.join(path,xmlFile)
#os.path.join("/home/test","test.xml")
```

那么,我们就拼接好了.然后就执行看看:

![](https://img-blog.csdn.net/20180125215330818?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvTE9WRTEwNTUyNTk0MTU=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)



啊哈?!居然对了!!哈哈哈,大功告成!接下来就是先将图像分好类,然后就可以批量修改文件了


![](https://img-blog.csdn.net/20180125215304314?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvTE9WRTEwNTUyNTk0MTU=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)


好了,让我们开看看最终的代码:



```python
#coding=utf-8
import os
import os.path
import xml.dom.minidom

path="/home/nvidia/xmlReader/xml/"
files=os.listdir(path)  #得到文件夹下所有文件名称
s=[]
for xmlFile in files: #遍历文件夹
    if not os.path.isdir(xmlFile): #判断是否是文件夹,不是文件夹才打开
        print xmlFile

	#TODO
	#xml文件读取操作

	#将获取的xml文件名送入到dom解析
	dom=xml.dom.minidom.parse(os.path.join(path,xmlFile))  ###最核心的部分,路径拼接,输入的是具体路径
        root=dom.documentElement
        #获取标签对name/pose之间的值
        name=root.getElementsByTagName('name')
        pose=root.getElementsByTagName('pose')
        #原始信息
        print '原始信息'
        n0=name[0]
	print n0.firstChild.data

	p0=pose[0]
	print p0.firstChild.data
	
	#修改
	n0.firstChild.data='circle'
	p0.firstChild.data='ok'
	#打印输出
	print '修改后的 name'
	print n0.firstChild.data

	print '修改后的 pose'
	print p0.firstChild.data
	print '~~~~~'
```


其实,就我个人来讲,还有个需求,就是文件匹配:根据A文件夹中的文件名,在B文件夹匹配同名但不同格式的文件,然后将他们单独

拎出来,复制到C文件夹保存,具体该怎么做呢?下一篇博客即将揭晓,拭目以待.



******************2018.1.27更新*******************

  上面说的方法基于我们已经知道了xml文件中有几个pose属性几个object属性,但是一般在修改之前我们是不知道的,那么如何自动的去识别并修改每个属性的值呢?接下来我们开看看

这里要用到一个重要的关系:Python中的迭代(不同于c++/C,不能写成for i in len(object))而要写成

```python
for i in range(len(pose)):
       print pose[i].firstChild.data
```

这样的话,即使我们不知道有几处要修改的地方,但是我们都能够找出来,修改掉(这里每个pose都修改成同样的属性)

修改后,还需要保存到xml文件,修改后的代码为:



```python
#coding=utf-8
import os
import os.path
import xml.dom.minidom

path="/home/nvidia/xmlReader/xml/"
files=os.listdir(path)  #得到文件夹下所有文件名称
s=[]
for xmlFile in files: #遍历文件夹
    if not os.path.isdir(xmlFile): #判断是否是文件夹,不是文件夹才打开
        print xmlFile

	#TODO
	#xml文件读取操作

	#将获取的xml文件名送入到dom解析
	dom=xml.dom.minidom.parse(os.path.join(path,xmlFile))  ###最核心的部分os.path.join(path,xmlFile),路径拼接,输入的是具体路径
        root=dom.documentElement
        #获取标签对name/pose之间的值
        name=root.getElementsByTagName('name')
        pose=root.getElementsByTagName('pose')
        #重命名class name
        for i in range(len(name)):	
            print name[i].firstChild.data
            name[i].firstChild.data='circle'
            print name[i].firstChild.data
	
        for j in range(len(pose)):	
            print pose[j].firstChild.data
            pose[j].firstChild.data='ok'
            print pose[j].firstChild.data
        
        #保存修改到xml文件中
 	with open(os.path.join(path,xmlFile),'w') as fh:
            dom.writexml(fh)
            print('写入name/pose OK!')


#  with open('dom_write.xml','w',encoding='UTF-8') as fh:
#             # 4.writexml()第一个参数是目标文件对象，第二个参数是根节点的缩进格式，第三个参数是其他子节点的缩进格式，
#             # 第四个参数制定了换行格式，第五个参数制定了xml内容的编码。
#             dom.writexml(fh,indent='',addindent='\t',newl='\n',encoding='UTF-8')
#             print('写入xml OK!')
```


参考博客:https://www.cnblogs.com/wcwnina/p/7222180.html






