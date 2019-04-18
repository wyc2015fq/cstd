# 在BAE上搭建python,django环境小记 - wishchinYang的专栏 - CSDN博客
2014年07月06日 17:50:00[wishchin](https://me.csdn.net/wishchin)阅读数：618
个人分类：[Django](https://blog.csdn.net/wishchin/article/category/2379183)
原文链接：[http://www.yihaomen.com/article/python/368.htm](http://www.yihaomen.com/article/python/368.htm)
## 作者:轻舞肥羊 日期:2013-03-13
字体大小: 小中
大
![](http://www.yihaomen.com/images/weather/hn2_sunny.gif)![](http://www.yihaomen.com/images/weather/hn2_t_sunny.gif)![](http://www.yihaomen.com/images/level3.gif)
BAE 是百度推出的一个云平台，类似于谷歌GAE,新浪SAE, 但谷歌经常被 “和谐”，而SAE的使用我也未能申请成功，这里PS 一下新浪. 好在百度 今年3.9日正式开放了BAE 云平台，所以就注册了开发者，而且还抢到了1000块的代金卷。申请了一个 合作网站类型的 python 应用。
![](http://www.yihaomen.com/attachments/month_1303/w20133131045.jpg)
百度有详细的说明文档，如何部署django 的应用，详细请参考官方文档：[http://developer.baidu.com/wiki/index.php?title=docs/cplat/rt/python](http://developer.baidu.com/wiki/index.php?title=docs/cplat/rt/python)
这里我想总结下要注意的几点事项。
1. 在settings.py 中数据库的配置,我这里配置是区分本地环境和BAE 环境的。用了：if 'SERVER_SOFTWARE' in os.environ 来做判断.
![程序代码](http://www.yihaomen.com/images/code.gif) 程序代码
if 'SERVER_SOFTWARE' in os.environ:
    from bae.core import const
    DATABASES = {
        'default': {
            'ENGINE': 'django.db.backends.mysql', 
            'NAME': 'you_apply_database_name',
            'USER': const.MYSQL_USER, 
            'PASSWORD': const.MYSQL_PASS,  
            'HOST': const.MYSQL_HOST,   
            'PORT': const.MYSQL_PORT, 
        }
    }
else:
    DATABASES = {
        'default': {
            'ENGINE': 'django.db.backends.mysql', 
            'NAME': 'xieyin', 
            'USER': 'root',
            'PASSWORD': 'password',    
            'HOST': 'localhost',                   
            'PORT': '3306',                      
        }
    }
当然，在此之前，你还得需要在BAE 上申请一个 云数据库，用PHPADMIN 管理等。注意编码一般改为UTF-8的。
2. BAE 云环境是不能上传文件的，上传文件，必须通过个人云存储来实现。而 BAE 提供了PYTHON 云存储的SDK. 可以直接使用。当然你必须先在云存储中创建bucket,在bucket里面，你还可以创建文件夹等.
![](http://www.yihaomen.com/attachments/month_1303/s201331395137.jpg)
在python 中的代码如下：
![程序代码](http://www.yihaomen.com/images/code.gif) 程序代码
import time
import os
from bae.core import const
from bae.api import bcs
HOST = const.BCS_ADDR
AK = const.ACCESS_KEY
SK = const.SECRET_KEY
def test_bcs():
    ### 首先通过云存储管理界面，创建一个bucket
    bname = 'yihaomen'
    ### 创建BCS管理对象
    baebcs = bcs.BaeBCS(HOST, AK, SK)
    ### 读取一个测试文件的内容
    filename = os.path.dirname(__file__) + "/favicon.ico"
    with open(filename) as fd:
        data = fd.read()
    ### 将文件内容上传到 '/obj1' 下
    o1 = '/upload/201303/obj1'
    e, d = baebcs.put_object(bname, o1, data)
    assert e == 0
    ### 上传文件有一定的延迟
    time.sleep(1)
    ### 从 '/obj1' 获取数据
    e, d = baebcs.get_object(bname, o1)
    assert e == 0
    assert d == data
    ### 也可以直接上传文件
    o2 = '/obj2'
    e, d = baebcs.put_file(bname, o2, filename)
    ### 将object内容保存到临时目录下
    tmpdir = (const.APP_TMPDIR if const.APP_TMPDIR else "/tmp")
    filename2 = tmpdir + "/favicon.ico"
    e, d = baebcs.get_to_file(bname, o2, filename2)
    assert e == 0
    ### 列出所有的object
    e, d = baebcs.list_objects(bname)
    assert e == 0
参考文档: [http://pythondoc.duapp.com/bcs.html](http://pythondoc.duapp.com/bcs.html)
当然，还有一个独立的SDK 可以参考，可以在BAE 外部环境使用的SDK:
[http://developer.baidu.com/wiki/index.php?title=%E5%B8%AE%E5%8A%A9%E6%96%87%E6%A1%A3%E9%A6%96%E9%A1%B5/%E4%BA%91%E5%AD%98%E5%82%A8](http://developer.baidu.com/wiki/index.php?title=%E5%B8%AE%E5%8A%A9%E6%96%87%E6%A1%A3%E9%A6%96%E9%A1%B5/%E4%BA%91%E5%AD%98%E5%82%A8)#Python_SDK
3. 对于图片放大，缩小，裁剪等的处理
![程序代码](http://www.yihaomen.com/images/code.gif) 程序代码
from bae.api.image import BaeImage
def app(env, start_response):
    status = "200 OK"
    headers = [('Content-type', 'image/gif')]
    start_response(status, headers)
    img = BaeImage()
    ### 设置待处理图片
    img.setSource("[http://www.baidu.com/img/baidu_sylogo1.gif](http://www.baidu.com/img/baidu_sylogo1.gif)")
    ### 设置目标图片尺寸
    img.setZooming(BaeImage.ZOOMING_TYPE_PIXELS, 100000)
    ### 设置裁剪参数
    img.setCropping(0, 0, 2000, 2000)
    ### 设置旋转角度
    img.setRotation(10)
    ### 设置灰度级别
    img.setHue(100)
    ### 设置亮度级别
    img.setLightness(100)
    ### 设置对比度级别
    img.setContrast(1)
    ### 设置锐化级别
    img.setSharpness(10)
    ### 设置色彩饱和度级别
    img.setSaturation(10)
    ### 设置图片格式
    img.setTranscoding('gif')
    ### 设置图片压缩质量
    img.setQuality(20)
    ### 设置获取gif图片第一帧
    img.setGetGifFirstFrame()
    ### 设置自动校准
    img.setAutoRotate()
    ### 执行图片处理
    ret = img.process()
    ### 返回图片base64 encoded binary data
    body = ret['response_params']['image_data']
    import base64
    return base64.b64decode(body) # 这里返回的就是图片的内容，保存下来就是图片，如果需要保存到BCS 中，结合上面的处理方式就可以保存了。
参考文档：[http://pythondoc.duapp.com/image.html](http://pythondoc.duapp.com/image.html)
部署了自己的一个小应用上去，还没完善，仍然在开发中: yihaomen.duapp.com
![](http://www.yihaomen.com/attachments/month_1303/020133131034.jpg)
除非申明,文章均为**[一号门](http://www.yihaomen.com)**原创,转载请注明本文地址,谢谢!
[本日志由 轻舞肥羊 于 2013-03-13 10:07 AM 编辑]
![](http://www.yihaomen.com/images/From.gif)**文章来自:**[本站原创](http://www.yihaomen.com/)
![](http://www.yihaomen.com/images/icon_trackback.gif)**引用通告:**[查看所有引用](http://www.yihaomen.com/trackback.asp?tbID=368&action=view) |
我要引用此文章
![](http://www.yihaomen.com/images/tag.gif)**Tags:**[bae](http://www.yihaomen.com/default.asp?tag=bae)[python](http://www.yihaomen.com/default.asp?tag=python)[django](http://www.yihaomen.com/default.asp?tag=django)
![](http://www.yihaomen.com/images/tag.gif)**相关日志:**
[百度BAE想说爱你并不容易[172]](http://www.yihaomen.com/article/diary/458.htm)[再次在BAE上用DJANGO搭建了一个网站[356]](http://www.yihaomen.com/article/python/432.htm)[如何用 python,Djano 生成 SEO 友好的 URL, 包含中文处理[36]](http://www.yihaomen.com/article/python/522.htm)[一个比较好用的python反编译工具[114]](http://www.yihaomen.com/article/python/515.htm)[用python 实现 java.util.Properties 类的一些基本操作[63]](http://www.yihaomen.com/article/python/488.htm)[python 序列化成json 乱码问题的解决[135]](http://www.yihaomen.com/article/python/487.htm)[是否该祭奠死去的ZOPE, 曾经浪费了我一年的青春.[147]](http://www.yihaomen.com/article/python/467.htm)[python得到得到当前登录用户信息[116]](http://www.yihaomen.com/article/python/419.htm)[python解析xml的简单例子[219]](http://www.yihaomen.com/article/python/365.htm)[django admin 根据用户显示不同的列表以及编辑界面等[330]](http://www.yihaomen.com/article/python/362.htm)
**分页: [1]**[2][3][4][5]
**模式:**全部显示[共114个相关文章]
评论: 9 | [引用: 0](http://www.yihaomen.com/trackback.asp?tbID=368&action=view) | 查看次数: 3846
[](http://www.yihaomen.com/#comm_top)
![](http://www.yihaomen.com/images/icon_quote.gif)[**爱简单吧**](http://www.yihaomen.com/member.asp?action=view&memName=%E7%88%B1%E7%AE%80%E5%8D%95%E5%90%A7)[2013-08-23
 08:06 PM]
Django有点笨重，BAE 却重点支持这个框架。我用tornado 作了一个小博客 bae4py.duapp.com 多多交流。
![](http://www.yihaomen.com/images/icon_quote.gif)[**Dexter**](http://www.yihaomen.com/member.asp?action=view&memName=Dexter)[2013-08-16
 11:26 PM]
hey, 不知道你有否做过在BAE上web界面上做上传，用户是通过选中本地文件上传的，在本地的时候，我测试是正常。放在BAE上就会说没有权限access 到temp目录。不知道您是怎么解决的。qq:310963321
![回复来自 轻舞肥羊 的评论](http://www.yihaomen.com/images/icon_reply.gif) 轻舞肥羊 于2013-08-17 04:18 PM 回复
当然可以，前提是你要申请 百度 云存储，然后调用相应的API 就可以了，我的上传图片都是这样解决的。
![](http://www.yihaomen.com/images/icon_quote.gif)[**zl**](http://www.yihaomen.com/member.asp?action=view&memName=zl)[2013-07-03
 09:44 AM]
请问一下，bae里面如何获取get、post中的信息啊？这些知识从哪里看呢？百度的python开发指南总共也没几个字
![回复来自 轻舞肥羊 的评论](http://www.yihaomen.com/images/icon_reply.gif) 轻舞肥羊 于2013-07-04 08:39 PM 回复
如果你用django的话，里面有request.POST,request.GET可以得到很多信息，这个与百度bae无关。好好看看django就知道了。
![](http://www.yihaomen.com/images/icon_quote.gif)[**zl**](http://www.yihaomen.com/member.asp?action=view&memName=zl)[2013-07-03
 09:40 AM]
请问一下，bae怎么获取网页的get、post中的参数，这些知识该往哪里看啊？python开发指南总共都没几个字。。。
![](http://www.yihaomen.com/images/icon_quote.gif)[**zsc1528**](http://www.yihaomen.com/member.asp?action=view&memName=zsc1528)[2013-06-28
 09:57 AM]
使用BAE django，我把本地数据库上传到云数据库文件的时候，我用的是从云存储bucket导入的，但是一直提示导入失败。 请大牛指点一下上传的注意事项，多谢。
![](http://www.yihaomen.com/images/icon_quote.gif)[**billychou**](http://www.yihaomen.com/member.asp?action=view&memName=billychou)[2013-06-26
 11:29 PM]
小白弱弱的问一句，我在本地开发的时候，用的是Django的ORM，直接创建数据库，但是在BAE环境下面，数据库都要自己创建吗？ 多谢。
![回复来自 轻舞肥羊 的评论](http://www.yihaomen.com/images/icon_reply.gif) 轻舞肥羊 于2013-06-27 08:13 PM 回复
是的，我的是自己创建的。在本地导出SQL 结构数据，在BAE 上导入的。
![回复来自 轻舞肥羊 的评论](http://www.yihaomen.com/images/icon_reply.gif) 轻舞肥羊 于2013-06-27 08:13 PM 回复
BAE 支持在线修改，其实还是很方便的。
![](http://www.yihaomen.com/images/icon_quote.gif)[**applepi**](http://www.yihaomen.com/member.asp?action=view&memName=applepi)[2013-03-28
 11:41 AM]
问一下大拿:
我没有用bae这个python sdk, 用的是pybcs这个sdk, 估计是bae sdk 的一个子集, 
想从django admin 中(web环境)直接上传本地文件到服务器,却总是不成功, 比如上传图片的时候要怎么把图片二进制信息添加到object里. 官方文档写的也不太清楚...有没有办法解决? QQ:35943132
![](http://www.yihaomen.com/images/icon_quote.gif)[**大叔**](http://www.yihaomen.com/member.asp?action=view&memName=%E5%A4%A7%E5%8F%94)[2013-03-15
 01:30 PM]
这是打算把博客放bae上？
博客的图片打算怎么处理？
![回复来自 轻舞肥羊 的评论](http://www.yihaomen.com/images/icon_reply.gif) 轻舞肥羊 于2013-03-16 07:33 PM 回复
图片放到BAIDU的云存储中，可以直接调取，很方便的。
![](http://www.yihaomen.com/images/icon_quote.gif)[**dyfire**](http://www.yihaomen.com/member.asp?action=view&memName=dyfire)[2013-03-15
 01:26 PM]
put_file 方法一直 internal server error 不知道是何故
![回复来自 轻舞肥羊 的评论](http://www.yihaomen.com/images/icon_reply.gif) 轻舞肥羊 于2013-03-16 07:34 PM 回复
我保存文件，图片到云存储都很正常，注意bucket,以及你的appkey 等。
