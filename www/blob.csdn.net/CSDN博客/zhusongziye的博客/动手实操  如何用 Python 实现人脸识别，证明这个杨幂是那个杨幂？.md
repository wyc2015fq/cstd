# 动手实操 | 如何用 Python 实现人脸识别，证明这个杨幂是那个杨幂？ - zhusongziye的博客 - CSDN博客





2018年11月26日 19:29:52[zhusongziye](https://me.csdn.net/zhusongziye)阅读数：342








### **人脸特征提取的步骤**

真正的人脸识别需要很多的知识，大体上粗略的可以分为以下步骤：

　　1.人脸检测（从图片中找到人脸）：返回人脸位置和大小的参数。

　　2.人脸特征定位：一般 69 点或者 106 个点对人脸的特征定位，技术上有 Adaboost&haar，以及 MSRA 的 alignment。

　　3.人脸特征归一化（几何归一／灰度归一）：前者对图像进行仿射变化使得不同的脸可以进行比对，后者则能使图像展现更多的细节以及减弱光线光照的应用。

　　4.特征提取-特征后期融合。（基于特征近似度的多特征融合）

　　5.特征距之间的距离来比对相似度、三氏距离。（马氏、欧氏 、巴氏）

### **操作过程中的注意事项**
- 
每张图片要先上传到 bucket 中才可以利用。

- 
API 对人脸匹配再返回很大程度依赖于用户的网络带宽。

- 
当用户网络不好的情况下需要很久的时间才能得到返回结果。

- 
要经过压缩处理，一般识别的较为准确最多可将图片压缩至 25 kb 左右。




**我们要进行如下的步骤：**

1)得到我们刚刚通过摄像头取得的人像

2)对我们的人像图片进行压缩

3)上传我们的人像图片到我们的 bucket 中

(这里要申明一点，七牛所有支持的 API 都要求文件在华东的 Bucket 下)

4)得到我们上传的图片的链接地址

5)对链接进行 urlbase64 加密（这里只要 import python 的 base64 库即可）

6)请求 API

7)得到网页的 JSON 格式数据

8)通过 JSON 库对数据进行分析

9)判断人脸的相似度是否符合，输出结果

10)删除 Bucket 中上传的临时图片



### 返回的 JSON 格式分析
`{"status":"ok","confidence":0.73065597}复制代码`
我们可以看到返回的 JSON 信息很简单， status 的意思是成功和不成功，而 confidence 则是相似度，所以我们对返回的信息进行分析会很简单。 status 如若成功则为 ok，不成功则为 invalid。

### 人脸相似度的判断

![](https://img-blog.csdnimg.cn/20181124160257325.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3podXNvbmd6aXll,size_16,color_FFFFFF,t_70)

上表中所用的图片都来自己互联网，从表数据我们可以大致将本人的相似度以 0.7 为分界线。不过有趣的是，不知道为什么按杨幂进行整容的为什么相似度竟然低于杨幂和范冰冰的相似度（其他女星照的图片为范冰冰的人像），不同性别的相似度差异明显，所以就粗略的根据表格定为 >0.7 即可认为是本人。

### 安装所需的库

在 python 中，我分享的这个例子总共需要引入 6 个库

```
#import SDK
from qiniu import Auth,put_file,etag
import qiniu.config
import requestsimport base64
import json
from PIL import Image
import os
```

requests 库，json 库，PIL 库请自行安装；

base64 库和 OS 库为自带所以无需安装。



**程序的实现**

请见下方的代码，采集到的人脸为 face.jpg。 (这里用了杨幂的两张图片作为示例)

```
#七牛云"人脸识别"功能的python实现方法：by xlxw
#请得到自己的Secret和Access key用于上传图片到空间中进行处理
#人像识别是七牛云的一项收费项目，价格为 ￥1.5/1000次 测试时请先存2元避免意外

#import SDK
from qiniu import Auth,put_file,etag
import qiniu.config
import requests
import base64
import json
from PIL import Image
import os

#上传
def upload(bucket,path,filename,key,url):
    token = key.upload_token(bucket, filename, 3600)
    print('正在上传..')
    reform,inform = put_file(token, filename, path)
    if reform != None:
        print('已经成功地将{}->>{}'.format(filename,bucket))
        print("正在处理您的图片...")
        url=url + '/' + filename
        path=path.split('/')[-1]
    else:
        print('这里出现了一个小错误.无法上传..')

#调用API
def apiget(urlbucket,url):
    try:
        url=urlbucket + '/001.jpg' + '?face-analyze/verification/url/' + url
        #标准对比的图片地址，名称为001.jpg
        r=requests.get(url)
        r.raise_for_status()
        r.encoding=r.apparent_encoding
        return r.text
    except:
        print("网络发生故障，请重试..")

#base64 Encode
def base64encode(url):
    try:
        print("正在加密链接..")
        enurl=base64.urlsafe_b64encode(bytes(url, "utf-8"))
        print("加密完成")
        enurl=str(enurl)
        enurl=enurl.split("'")[1]
        return enurl
    except:
        print("这里出现了一个问题，请重试..")

#PIL 图片压缩
def pilresize(per,path):
    im=Image.open(path)
    imsize=im.size
    sizex=int(imsize[0]*per)
    sizey=int(imsize[1]*per)
    im=im.resize((sizex,sizey))
    im.save('trans.jpg','JPEG')
    print('图片压缩完成，输出成功')
    print('{}->>({},{})'.format(imsize,sizex,sizey))

def pilwork(path):
    try:
        size=os.path.getsize(path)
        size = float(size)
        kb=size/1024
        per=10/kb
        pilresize(per,path)
    except:
        print("请检查您的地址是否输入错误")


#JSON分析
def jsonanal(jtext):
    print("正在分析,请稍后..")
    rj=json.loads(jtext)
    stat=rj['status']
    confi=rj['confidence']
    return stat + ',' +str(confi)

#主体
def main():
    #填写你的 AK 和 SK
    accesskey = input('请输入您在七牛云的AccessKey：')
    secretkey = input('请输入您在七牛云的SecretKey：')

    #鉴定身份
    keyq=Auth(accesskey,secretkey)

    #所要操作的空间
    bucketname =input("请输入要操作的空间(公开)名字：")

    #所要操作空间的外链地址
    urlbucket = input("请输入空间所绑定的域名或者默认外链地址：")

    #判定操作类型
    while 1:
        order=input('请输入你需要进行的操作：')
        mode=order.split(' ')[0]
        if mode == '识别':
            path=order.split(' ')[1]
            fname=path.split('/')[-1:][0]
            unrl=urlbucket+'/trans.jpg'
            print('正在压缩图片.请稍后..')
            #调用函数
            pilwork(path)  #压缩图片
            print("正在上传token,请稍后..")
            upload(bucketname,'./trans.jpg','trans.jpg',keyq,urlbucket) #上传文件
            enurl=base64encode(unrl)   #base64加密
            jtext=apiget(urlbucket,enurl) #调用七牛api并得到返回的json数据
            result=jsonanal(jtext)  #分析返回的json，得到最终相似度
            if result.split(',')[0] == 'invalid':
                print('识别发生了错误')
            else:
                if eval(result.split(',')[1]) >= 0.7:
                    print("识别成功，鉴定为本人，相似度为{:.1f}".format(eval(result.split(',')[1])*100))
                else:
                    print("识别成功，鉴定不是本人，相似度过低")
        if mode == '退出':
            print("欢迎您的使用..")
            break

#终端提示显示
print("+----------------------------------------+")
print("|        欢迎使用七牛的人脸识别功能      |")
print("+----------------------------------------+")
print("|本程序须知：                            |")
print("|1.本程序测试图片为杨幂的人像,见face.jpg |")
print("|2.您需要提供服务的Accesskey，Secretkey  |")
print("|3.您需要提供 bucket名字和bucket外链地址 |")
print("+----------------------------------------+")
print("|使用方法:                               |")
print("|1.识别输入格式： 识别 图片位置(包括后缀)|")
print("|2.退出输入格式： 退出                   |")
print("+----------------------------------------+")
main()
```



程序运行的截图：

![](https://img-blog.csdnimg.cn/20181124160348717.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3podXNvbmd6aXll,size_16,color_FFFFFF,t_70)



用到的杨幂的两张照片为：

（均来自百度图片）

![](https://img-blog.csdnimg.cn/20181124160411665.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3podXNvbmd6aXll,size_16,color_FFFFFF,t_70)

（用于比对的标准人像图片）

（已用 PIL 压缩 x*0.3，y*0.3）

![](https://img-blog.csdnimg.cn/20181124160426844.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3podXNvbmd6aXll,size_16,color_FFFFFF,t_70)



（用于比对的图片）



作者：七牛云

源自：https://juejin.im/post/59719caef265da6c4741cdd7



