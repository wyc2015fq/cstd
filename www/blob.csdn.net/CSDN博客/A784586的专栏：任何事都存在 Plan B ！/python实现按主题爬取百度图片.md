# python实现按主题爬取百度图片 - A784586的专栏：任何事都存在 Plan B ！ - CSDN博客





2017年06月08日 13:27:34[QuJack](https://me.csdn.net/A784586)阅读数：706








**    python实现按主题爬取百度图片**


python3：

说明：按主题爬取百度图片




```python
code:
#!/usr/bin/python
#encoding: utf-8
import urllib.request
#from urllib.request import urlretrieve,urlcleanup
#from urllib2 import *
import json
from hashlib import md5
import os
import skimage
import skimage.io
import socket
import importlib
import sys
import re
#importlib.reload(sys)
#sys.setdefaultencoding('utf-8')
#from http://outofmemory.cn/code-snippet/35203/picture
class Baiduimage():
    """
    """
	#tag 图片类型  ； path保存路径
    def __init__(self , tag,num,path):
        self.tag = tag
		#num=100
        self.number = num
        self.path = path
        print ("work start")
        print ("pages(self.number) in " , self.path , "are :" , self.number)
		
	#004 拼结url
    def make_url(self,number):#number=60
	#http://image.baidu.com/search/flip?tn=baiduimage&ie=utf-8&word=风景&pn=60   由于python3  汉子改为硬编码URL更适合
        url = "http://image.baidu.com/search/flip?tn=baiduimage&ie=utf-8&word=" + self.tag + "&pn=" + str(number)
        #print("this time number is: ",number)
        #print ("baidu url is :" , url)
        return url
		
	#003 请求网页内容
    def request_body(self,number):#number=60
        #print("------- before make_url -------")
        url=self.make_url(number)
        #print("------- after make_url -------")
        #print("------- before urlopen -------")
        req = urllib.request.urlopen(url)
        #print("-------### after urlopen ### -------")
        content = req.read()
        content=content.decode("utf8")
        return content
		
	#002  解析请求的内容
    def parse_body(self , number):#num=60,120,。。。。
        w1 = '"objURL":"'
        w2 = '",'
        try:      
            #print("---- before request_body ----")
            content = self.request_body(number)
            #print("---- after request_body ----")
            parse = re.compile(w1 + '(.*?)' + w2 , re.S)
            result = parse.findall(content)#findall函数返回正则表达式在字符串中所有匹配结果的列表list    
            return result
        except:
            return []
    #001
    def download_image(self):
        if not os.path.exists(self.path):
            os.makedirs(self.path)
		#base_path为各个类别下的文件夹下
        base_path = self.path + "/"
        ind = 0
        ind_w = 0
        ind_r = 0
		#遍历 0 --- 100
        for i in range(self.number):
            result = self.parse_body(i*20)
            #print("result:",str(result))
            #print ("pics in page :" ,i , "are :" , len(result))
            #print ("type of result:" ,type(result))
            if len(result) == 0:
                #print ("the page :" , i , "is null")
                break;
            for url in result:
                ind = ind + 1
                try:
                    image_url = url.split(".")
                    image_path = base_path + md5(url.encode("utf8")).hexdigest() + "." + image_url[-1]
                    socket.setdefaulttimeout(30)
					#url为下载图片路径，image_path为图片保存路径
                    urllib.request.urlretrieve(url,image_path)
					#加载一张图片
                    skimage.io.imread(image_path)
					#清除由于urlretrieve()所产生的缓存
                    urllib.request.urlcleanup()
                    #print ("the effective url is :" , url)
                except IOError:
                    try:
                        ind_r = ind_r + 1
                        os.remove(image_path)
                    except:
                        pass
                except:
                    ind_w = ind_w + 1
                    #print ("the wrong url is :" , url)
                    pass
        print ("sum of pictures " , ind)
        print ("sum of wrong pics" , ind_w)
        print ("sum of remove pics" , ind_r)

if __name__ == "__main__":
	# list_tag = ["尤物","证件照","真人","风景" , "自然" , "山水", "景色","美女"]  这地方顺序问题，爬图片后对应改名称
    list_tag = ["%e5%b0%a4%e7%89%a9","%e8%af%81%e4%bb%b6%e7%85%a7","%e7%9c%9f%e4%ba%ba","%e9%a3%8e%e6%99%af" , "%e8%87%aa%e7%84%b6" , "%e5%b1%b1%e6%b0%b4", "%e6%99%af%e8%89%b2","%e7%be%8e%e5%a5%b3"]
    list_path = ["./meinv","./youwu","./zhengjianzhao","./zhenren","./fengjing" , "./ziran" , "./shanshui", "./jingse"]
    print ("len(list_tag):" , len(list_tag))
    print ("len(list_path):" , len(list_path))
    if not len(list_tag) == len(list_path):
        print ("tag length is not equal to path length")
        os._exit(0)
    for i in range(len(list_tag)):
        Baiduimage(list_tag[i] , 100 , list_path[i]).download_image()
```





