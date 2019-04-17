# Python接口自动化测试之文件上传 - zhusongziye的博客 - CSDN博客





2018年10月30日 20:03:16[zhusongziye](https://me.csdn.net/zhusongziye)阅读数：695








 在接口测试中，经常会涉及到文件上传，文件上传一般包含的文件是图片，视频以及如csv,excel,记事本等文件，它的请求头中Content-Type对应的value值是multipart/form-data，这里依据实际的案例来说明文件上传的具体案例。这里以互联网产品人人网为案例，人人网上传成功后，在个人的主页，上传图片，见上传图片的请求参数如下图所示：

![](https://img-blog.csdnimg.cn/20181030200005276.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3podXNvbmd6aXll,size_16,color_FFFFFF,t_70)

见图片上传请求头的信息如下图所示：

![](https://img-blog.csdnimg.cn/20181030200109536.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3podXNvbmd6aXll,size_16,color_FFFFFF,t_70)

在上传文件的请求参数中，其它参数都比较好处理，文件的这部分，必须要指定文件的路径，文件的Content-Type类型，其实可以在Raw中可以看到，见如下图所示：

![](https://img-blog.csdnimg.cn/20181030200155206.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3podXNvbmd6aXll,size_16,color_FFFFFF,t_70)

那么文件部分的请求参数具体就为：

files = {"file":("wx.jpg", open("c:/wx.jpg", "rb"), "image/jpeg",{})}
这地方特别需要强调的是file其实就是请求参数中的file，必须与请求参数中的参数名称一一对应，在请求头中cookie比较很好处理，直接在登录成功后，获取到r.cookies并且把该内容返回。那么文件上传的完整业务流是先登录系统，登录成功后再进行上传文件，见实现的全部源码：

```python
#!/use/bin/env python
#coding:utf-8 

#Author:WuYa

import  requests

def loginData():
   data = {
      'email': '13484545195',
      'icode': '',
      'origURL': 'http://www.renren.com/home',
      'domain': 'renren.com',
      'key_id': 1,
      'captcha_type': 'web_login',
      'password': '8d9a71152919613bbe3df9bfa0e1b390eb2b13dd1bdde270c2816cf04dd1b7c5',
      'rkey': 'b4cdc6acc1d36171e3de73dd4676208e',
      'f': 'http%3A%2F%2Fname.renren.com%2F'}
   return data


def login():
   r = requests.post(
      url='http://www.renren.com/ajaxLogin/login?1=1&uniqueTimestamp=201894216799',
      data=loginData(),
      headers={'Content-Type': 'application/x-www-form-urlencoded'})
   return r.cookies


def uploadData():
   data = {
      "upload": "提交",
      "__channel": "renren",
      "privacyParams": '{"sourceControl": 99}',
      'hostid': '967004081',
      'requestToken': '-1124080368',
      '_rtk': '88c0e36a'}
   return data


def upload():
   r = requests.post(
      url='http://upload.renren.com/upload.fcgi?pagetype=addpublishersingle&hostid=967004081&'
          'callback=window.parent.handlePhotoData&uploadid=profile_publisher_photo_1540215890321',
      data=uploadData(),
      headers={'Conteny-Type': 'multipart/form-data'},
      files={"file": ("wx.jpg", open("c:/wx.jpg", "rb"), "image/jpeg", {})},
      cookies=login())
   print(r.status_code)
   print(r.text)


if __name__ == '__main__':
   upload()
```





