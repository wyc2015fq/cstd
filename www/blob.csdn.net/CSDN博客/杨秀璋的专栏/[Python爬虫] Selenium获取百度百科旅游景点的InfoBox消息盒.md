
# [Python爬虫] Selenium获取百度百科旅游景点的InfoBox消息盒 - 杨秀璋的专栏 - CSDN博客

2015年09月06日 02:11:57[Eastmount](https://me.csdn.net/Eastmount)阅读数：13192所属专栏：[Python爬虫之Selenium+Phantomjs+CasperJS](https://blog.csdn.net/column/details/eastmount-spider.html)



前面我讲述过如何通过BeautifulSoup获取维基百科的消息盒，同样可以通过Spider获取网站内容，最近学习了Selenium+Phantomjs后，准备利用它们获取百度百科的旅游景点消息盒（InfoBox），这也是毕业设计实体对齐和属性的对齐的语料库前期准备工作。希望文章对你有所帮助~
**源代码**

```python
# coding=utf-8  
""" 
Created on 2015-09-04 @author: Eastmount  
"""  
  
import time          
import re          
import os  
import sys
import codecs
from selenium import webdriver      
from selenium.webdriver.common.keys import Keys      
import selenium.webdriver.support.ui as ui      
from selenium.webdriver.common.action_chains import ActionChains  
  
#Open PhantomJS  
driver = webdriver.PhantomJS(executable_path="G:\phantomjs-1.9.1-windows\phantomjs.exe")  
#driver = webdriver.Firefox()  
wait = ui.WebDriverWait(driver,10)
global info #全局变量
#Get the infobox of 5A tourist spots  
def getInfobox(name):  
    try:  
        #create paths and txt files
        global info
        basePathDirectory = "Tourist_spots_5A"  
        if not os.path.exists(basePathDirectory):  
            os.makedirs(basePathDirectory)  
        baiduFile = os.path.join(basePathDirectory,"BaiduSpider.txt")  
        if not os.path.exists(baiduFile):  
            info = codecs.open(baiduFile,'w','utf-8')  
        else:  
            info = codecs.open(baiduFile,'a','utf-8')  
      
        #locate input  notice: 1.visit url by unicode 2.write files  
        print name.rstrip('\n') #delete char '\n'  
        driver.get("http://baike.baidu.com/")  
        elem_inp = driver.find_element_by_xpath("//form[@id='searchForm']/input")  
        elem_inp.send_keys(name)  
        elem_inp.send_keys(Keys.RETURN)  
        info.write(name.rstrip('\n')+'\r\n')  #codecs不支持'\n'换行
        time.sleep(2)
        print driver.current_url
        print driver.title
  
        #load infobox basic-info cmn-clearfix
        elem_name = driver.find_elements_by_xpath("//div[@class='basic-info cmn-clearfix']/dl/dt")  
        elem_value = driver.find_elements_by_xpath("//div[@class='basic-info cmn-clearfix']/dl/dd")
        for e in elem_name:
            print e.text
        for e in elem_value:
            print e.text
  
        #create dictionary key-value
        #字典是一种散列表结构,数据输入后按特征被散列,不记录原来的数据,顺序建议元组
        elem_dic = dict(zip(elem_name,elem_value)) 
        for key in elem_dic:  
            print key.text,elem_dic[key].text  
            info.writelines(key.text+" "+elem_dic[key].text+'\r\n')  
        time.sleep(5)  
          
    except Exception,e: #'utf8' codec can't decode byte  
        print "Error: ",e  
    finally:  
        print '\n'  
        info.write('\r\n')  
  
#Main function  
def main():
    global info
    #By function get information   
    source = open("Tourist_spots_5A_BD.txt",'r')  
    for name in source:  
        name = unicode(name,"utf-8")  
        if u'故宫' in name: #else add a '?'  
            name = u'北京故宫'  
        getInfobox(name)  
    print 'End Read Files!'  
    source.close()  
    info.close()  
    driver.close()  
  
main()
```


**运行结果**
主要通过从F盘中txt文件中读取国家5A级景区的名字，再调用Phantomjs.exe浏览器依次访问获取InfoBox值。同时如果存在编码问题“'ascii' codec can't encode characters”则可通过下面代码设置编译器utf-8编码，代码如下：
```python
#设置编码utf-8
import sys 
reload(sys)  
sys.setdefaultencoding('utf-8')
#显示当前默认编码方式
print sys.getdefaultencoding()
```
![](https://img-blog.csdn.net/20150906015640197)
![](https://img-blog.csdn.net/20150906033038750)

**对应源码**
其中对应的百度百科InfoBox源代码如下图，代码中基础知识可以参考我前面的博文或我的Python爬虫专利，Selenium不仅仅擅长做自动测试，同样适合做简单的爬虫。
![](https://img-blog.csdn.net/20150906020720315)

**编码问题**
此时你仍然可能遇到“'ascii' codec can't encode characters”编码问题。
![](https://img-blog.csdn.net/20150922153651858)
它是因为你创建txt文件时默认是ascii格式，此时你的文字确实'utf-8'格式，所以需要转换通过如下方法。
```python
import codecs
#用codecs提供的open方法来指定打开的文件的语言编码,它会在读取的时候自动转换为内部unicode
if not os.path.exists(baiduFile):  
    info = codecs.open(baiduFile,'w','utf-8')  
else:  
    info = codecs.open(baiduFile,'a','utf-8')
    
#该方法不是io故换行是'\r\n'
info.writelines(key.text+":"+elem_dic[key].text+'\r\n')
```

**总结**
你可以代码中学习基本的自动化爬虫方法、同时可以学会如何通过for循环显示key-value键值对，对应的就是显示的属性和属性值，通过如下代码实现：
elem_dic = dict(zip(elem_name,elem_value))
但最后的输出结果不是infobox中的顺序，why?
最后希望文章对你有所帮助，还有一篇基础介绍文章，但是发表时总会引发CSDN敏感系统自动锁定，而且不知道哪里引起的触发。推荐你可以阅读~
[[python爬虫] Selenium常见元素定位方法和操作的学习介绍](http://blog.csdn.net/eastmount/article/details/48108259)
（By:Eastmount 2015-9-6 深夜2点半[http://blog.csdn.net/eastmount/](http://blog.csdn.net/eastmount/)）


