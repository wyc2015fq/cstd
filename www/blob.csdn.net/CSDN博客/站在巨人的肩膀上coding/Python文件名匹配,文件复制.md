# Python文件名匹配,文件复制 - 站在巨人的肩膀上coding - CSDN博客





2018年01月26日 02:55:39[coding思想](https://me.csdn.net/LOVE1055259415)阅读数：5517








文件名的匹配,实际上就是相当于获取文件名(不含后缀),然后利用获取到的文件名到另外一个文件夹中去寻找对应的文件,然后将文件取出,放置到指定文件夹下.概括的来说,分三个步骤:一是取出遍历目录A,得到各个文件文件名;二是利用该文件名,与指定路径B拼接,并加上后缀,产生目标文件名;三是根据拼接产生的目标文件名,将相应文件复制到指定目录C.

好,那么我们开始写代码吧~~~

step1:获取指定目录A下面的所有文件名.不包含文件后缀.主要基于以下思想:



```python
def GetFileNameAndExt(filename):
 import os
 (filepath,tempfilename) = os.path.split(filename);
 (shotname,extension) = os.path.splitext(tempfilename);
 return shotname,extension

测试代码
print(GetFileNameAndExt('c:\jb51\index.html'))
返回结果：('index', '.html')
```

实际代码如下

```java
#coding=utf-8
import os
import os.path

def GetFileNameAndExt(filename):
    (filepath,tempfilename) = os.path.split(filename);
    (shotname,extension) = os.path.splitext(tempfilename);
    return shotname,extension

source_dir='/home/nvidia/xmlReader/circle'
label_dir='/home/nvidia/xmlReader/label'
annotion_dir='/home/nvidia/xmlReader/annocation'

##1.将指定A目录下的文件名取出,并将文件名文本和文件后缀拆分出来
img=os.listdir(source_dir)  #得到文件夹下所有文件名称
s=[]
for fileNum in img: #遍历文件夹
    if not os.path.isdir(fileNum): #判断是否是文件夹,不是文件夹才打开
        print fileNum  #打印出文件名
        imgname= os.path.join(source_dir,fileNum)
        print imgname  #打印出文件路径
        (imgpath,tempimgname) = os.path.split(imgname); #将路径与文件名分开
        (shotname,extension) = os.path.splitext(tempimgname); #将文件名文本与文件后缀分开
	print shotname,extension
	print '~~~~'
```

step2:二是利用该文件名,与指定路径B拼接,并加上后缀,产生目标文件名



```python
##2.将取出来的文件名文本与特定后缀拼接,在于路径拼接,得到B目录下的文件
	xmlname=os.path.join(label_dir,shotname,'.xml')
	print xmlname
```

但是得到的输出是有分隔符的.

![](https://img-blog.csdn.net/20180126022449848?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvTE9WRTEwNTUyNTk0MTU=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)


如何去掉分隔符呢?或者说如何拼接文件名文本和后缀呢?基于以下Python基础



```python
'%d.txt'%fname

这样基本上可以表示比如120.txt这样的字符串了。
```

代码如下:



```java
##2.将取出来的文件名文本与特定后缀拼接,在于路径拼接,得到B目录下的文件	
	tempxmlname='%s.xml'%shotname	
	xmlname=os.path.join(label_dir,tempxmlname)
	print xmlname
```

我们来看看输出:

![](https://img-blog.csdn.net/20180126023410924?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvTE9WRTEwNTUyNTk0MTU=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)

大功告成,现在我们得到了想要的数据格式,开始到这个指定的路径去寻找文件匹配文件名了~~~~


step3:定位到目标文件名,将其复制到指定目录下,保持文件名不变.(Python文件复制)



```python
##3.根据得到的xml文件名,将对应文件拷贝到指定目录C
	shutil.copy(xmlname,annotion_dir)
```

这样,与图片相关的标注文件就全部拷贝过来了~~~


![](https://img-blog.csdn.net/20180126024611532?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvTE9WRTEwNTUyNTk0MTU=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)

最后,我们来看看我们最终的代码:



```python
#coding=utf-8
import os
import os.path
import shutil  #Python文件复制相应模块

def GetFileNameAndExt(filename):
    (filepath,tempfilename) = os.path.split(filename);
    (shotname,extension) = os.path.splitext(tempfilename);
    return shotname,extension

source_dir='/home/nvidia/xmlReader/circle'
label_dir='/home/nvidia/xmlReader/label'
annotion_dir='/home/nvidia/xmlReader/annocation'

##1.将指定A目录下的文件名取出,并将文件名文本和文件后缀拆分出来
img=os.listdir(source_dir)  #得到文件夹下所有文件名称
s=[]
for fileNum in img: #遍历文件夹
    if not os.path.isdir(fileNum): #判断是否是文件夹,不是文件夹才打开
        print fileNum  #打印出文件名
        imgname= os.path.join(source_dir,fileNum)
        print imgname  #打印出文件路径
        (imgpath,tempimgname) = os.path.split(imgname); #将路径与文件名分开
        (shotname,extension) = os.path.splitext(tempimgname); #将文件名文本与文件后缀分开
	print shotname,extension
	print '~~~~'
##2.将取出来的文件名文本与特定后缀拼接,再与路径B拼接,得到B目录下的文件	
	tempxmlname='%s.xml'%shotname	
	xmlname=os.path.join(label_dir,tempxmlname)
	print xmlname
##3.根据得到的xml文件名,将对应文件拷贝到指定目录C
	shutil.copy(xmlname,annotion_dir)
```

至此,大功告成
![](https://img-blog.csdn.net/20180126025500357?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvTE9WRTEwNTUyNTk0MTU=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)







