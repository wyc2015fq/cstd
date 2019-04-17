# 简单python爬虫淘宝图片+界面编程+打包成exe - 勿在浮砂筑高台 - CSDN博客





2017年03月21日 14:05:22[勿在浮砂筑高台](https://me.csdn.net/luoshixian099)阅读数：3899
个人分类：[Linux/Python](https://blog.csdn.net/luoshixian099/article/category/3100989)









淘宝图片爬虫exe下载：[http://download.csdn.net/detail/luoshixian099/9788470](http://download.csdn.net/detail/luoshixian099/9788470)


# 源码解析

新建GetImg.py文件：

第一步:编写爬虫代码



```python
import re
import urllib.request
def getHtml(url='') :
    page = urllib.request.urlopen(url)
    html = page.read().decode('GBK') #解析网页源代码
    return html


#print(html)

def getImg(html):
    #https://cbu01.alicdn.com/img/ibank/2016/693/443/3382344396_1109687136.jpg
    #reg = r'https://.*\.?alicdn\.com[^"]+\.jpg'
    reg = r'//[\w]*\.?alicdn\.com[^"\']+\.jpg'#正则表达式，搜索匹配字段
    imgre = re.compile(reg)
    imglist = re.findall(imgre,html)
    print(len(imglist))
    x = 1
    for imgurl in imglist:
        print(imgurl)
        imgurl = 'https:' + imgurl
        urllib.request.urlretrieve(imgurl,'save\\%s.jpg' %x) #按顺序保存在save文件夹
        x+=1

def DownLoadImg(url):
    html = getHtml(url)
    getImg(html)
```
通过调用DownLoadImg()函数，即可下载所有.jpg格式的图片
例如：DownLoadImg('https://detail.1688.com/offer/523861658213.htmlspm=b26110380.8015204.xshy005.37.q3aAzE')![](https://img-blog.csdn.net/20170320171312618)







第二步:编写界面



```python
class Application(Frame):
         def __init__(self, master=None):
                  Frame.__init__(self, master)
                  self.pack(fill=X,padx=100)
                  self.createWidgets()
         def createWidgets(self):
                  self.nameInput = Entry(self)
                  self.nameInput.pack(fill=X,pady=10)
                  self.nameInput.pack(fill=X,padx=5)
                  self.alertButton = Button(self, text=(' Please Input Website '), command=self.FUN)
                  self.alertButton.pack(fill=X,padx=30)
                  self.alertButton.pack(fill=X,pady=10)
         def FUN(self):
                  name = self.nameInput.get() or 'blank'
                  if name == 'blank':
                           messagebox.showinfo('Message', 'Website is invalid!')
                  else:
                           DownLoadImg(name)
                           messagebox.showinfo('Message', 'Success: see \"save\" folder')
app = Application()
app.master.title('DownLoad IMG From Web : [LSX]')
app.mainloop()
```





第三步：打包成exe
下载py2exe,选择对应python版本，仅支持python3.3以上版本，3.5版本用起来有点问题，所以本文采用python3.4。

两种方法把python程序打包成exe

1.在cmd命令窗口下，进入GetImg.py文件夹

运行



```cpp
build_exe GetImg.py
```


生成的exe文件被保存在dist文件中。


2.新建一个convert2exe.py



```python
from distutils.core import setup  
import py2exe  
setup(windows=[{"script": "GetImg.py"}])
```
运行





```python
python convert2exe.py py2exe
```




# 总结：
1.没有搞清楚解析html源码格式,'GBK','utf-8',对于'utf-8'格式的源码解析总是出现错误


2.对于py2exe软件，不能转换中文字符。

3.只能解析到部分图片，可能是正则表达式有问题。




参考：

[http://blog.csdn.net/wangxiaobupt/article/details/37936849](http://blog.csdn.net/wangxiaobupt/article/details/37936849)

[http://www.cnblogs.com/fnng/p/3576154.html](http://www.cnblogs.com/fnng/p/3576154.html)

[http://blog.csdn.net/liuxu0703/article/details/54428405](http://blog.csdn.net/liuxu0703/article/details/54428405)







