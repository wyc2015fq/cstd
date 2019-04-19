# 阿里云 OSS+CDN - starRTC免费im直播会议一对一视频 - CSDN博客
2016年08月31日 13:25:48[starRTC免费IM直播会议一对一视频](https://me.csdn.net/elesos)阅读数：3660
[https://promotion.aliyun.com/ntms/ossedu2.html](https://promotion.aliyun.com/ntms/ossedu2.html)
[https://www.aliyun.com/act/aliyun/ossdoc.html](https://www.aliyun.com/act/aliyun/ossdoc.html)
对象存储（Object Storage Service，简称OSS）
RESTFul API

#### 可以理解为一个无限大空间的存储集群。
OSS作为源站，搭配CDN进行加速分发，稳定、无回源带宽限制、性价比高，一键配置。

[http://star1.oss-cn-shanghai.aliyuncs.com/Koala.jpg](http://star1.oss-cn-shanghai.aliyuncs.com/Koala.jpg)
Bucket存储空间，主目录
GetService获取主目录列表
GetBucket获取文件列表
PutObject写文件
GetObject 读文件
Bucket命名规范：
存储空间命名规范
- 只能包括小写字母，数字和短横线（-）
- 必须以小写字母或者数字开头
- 长度必须在3-63字节之间
存储空间的名字全局唯一
## 创建虚拟目录
OSS是没有文件夹这个概念的，所有元素都是以Object来存储。创建文件夹本质上来说是创建了一个size为0的Object。对于这个Object可以上传下载，只是控制台会对以”/“结尾的Object以文件夹的方式展示。
其实目录不用创建，直接在文件中体现即可：
$ossClient->putObject($bucket, "2016/07/28/f.file", "hi, oss");
Common::println("file is created");
上面会自动创建目录
图片需要设置上传的元信息
IMG处理的图片是OSS用户上传到Bucket里的Object
先开启图片处理服务，在Object管理的下面 
[https://help.aliyun.com/document_detail/31917.html?spm=5176.doc32214.6.128.Wiwb6f](https://help.aliyun.com/document_detail/31917.html?spm=5176.doc32214.6.128.Wiwb6f)
[http://star1.oss-cn-shanghai.aliyuncs.com/Koala.jpg](http://star1.oss-cn-shanghai.aliyuncs.com/Koala.jpg)
star1.img-cn-shanghai.aliyuncs.com
[http://star1.img-cn-shanghai.aliyuncs.com/Koala.jpg@50h_50w_0e](http://star1.img-cn-shanghai.aliyuncs.com/Koala.jpg@50h_50w_0e)
下面的url访问不了
[http://image.yhyfarm.com/headlogo.jpg@50h_50w_0e](http://image.yhyfarm.com/headlogo.jpg@50h_50w_0e)
缩放参数
## 设置文件元信息(Object Meta)
[https://help.aliyun.com/document_detail/32103.html?spm=5176.doc32105.6.377.NaGb15](https://help.aliyun.com/document_detail/32103.html?spm=5176.doc32105.6.377.NaGb15)
 带身份验证的请求表示的是如下两种情况：
- 请求头部中带Authorization，格式为OSS + AccessKeyId + 签名字符串。
- 请求的URL中带OSS AccessKeyId和Signature字段。
OSS根据URL解析出Bucket和Object。
php-sdk
[https://help.aliyun.com/document_detail/32099.html](https://help.aliyun.com/document_detail/32099.html)
要接入阿里云OSS，您需要拥有一对有效的 AccessKey(包括AccessKeyId和AccessKeySecret)用来进行签名认证。
RAM (Resource Access Management) 是阿里云为客户提供的用户身份管理与访问控制服务。按需为用户分配最小权限。
OSS作为CDN的源站
OSS的存储费用仅为ECS磁盘费用的50%
相比直接通过OSS访问，除极少额外增加的回源流量外，主要流量使用CDN流量，单价最低只需0.26GB，远远低于OSS直接访问的外网流量单价
总结：
1，开通oss，通过Access
 Key ID 操作文件，然后在cdn上添加oss域名，最后在自己的域名处加上cname指向cdn域名
======================================
oss的cdn配置
直接在cdn页面进行配置即可
CDN相关
#### CloudDeliveryNetwork    从Content到Cloud，重新定义CDN
[https://help.aliyun.com/document_detail/29908.html?spm=5176.7933691.203612.3.SFKTCq](https://help.aliyun.com/document_detail/29908.html?spm=5176.7933691.203612.3.SFKTCq)
启用CDN加速服务，需要将您的域名指向加速域名
[https://promotion.aliyun.com/ntms/ossedu2.html](https://promotion.aliyun.com/ntms/ossedu2.html)
[https://www.aliyun.com/act/aliyun/ossdoc.html](https://www.aliyun.com/act/aliyun/ossdoc.html)
对象存储（Object Storage Service，简称OSS）
RESTFul API

#### 可以理解为一个无限大空间的存储集群。
OSS作为源站，搭配CDN进行加速分发，稳定、无回源带宽限制、性价比高，一键配置。

[http://star1.oss-cn-shanghai.aliyuncs.com/Koala.jpg](http://star1.oss-cn-shanghai.aliyuncs.com/Koala.jpg)
Bucket存储空间，主目录
GetService获取主目录列表
GetBucket获取文件列表
PutObject写文件
GetObject 读文件
Bucket命名规范：
存储空间命名规范
- 只能包括小写字母，数字和短横线（-）
- 必须以小写字母或者数字开头
- 长度必须在3-63字节之间
存储空间的名字全局唯一
## 创建虚拟目录
OSS是没有文件夹这个概念的，所有元素都是以Object来存储。创建文件夹本质上来说是创建了一个size为0的Object。对于这个Object可以上传下载，只是控制台会对以”/“结尾的Object以文件夹的方式展示。
其实目录不用创建，直接在文件中体现即可：
$ossClient->putObject($bucket, "2016/07/28/f.file", "hi, oss");
Common::println("file is created");
上面会自动创建目录
图片需要设置上传的元信息
IMG处理的图片是OSS用户上传到Bucket里的Object
先开启图片处理服务，在Object管理的下面 
[https://help.aliyun.com/document_detail/31917.html?spm=5176.doc32214.6.128.Wiwb6f](https://help.aliyun.com/document_detail/31917.html?spm=5176.doc32214.6.128.Wiwb6f)
[http://star1.oss-cn-shanghai.aliyuncs.com/Koala.jpg](http://star1.oss-cn-shanghai.aliyuncs.com/Koala.jpg)
star1.img-cn-shanghai.aliyuncs.com
[http://star1.img-cn-shanghai.aliyuncs.com/Koala.jpg@50h_50w_0e](http://star1.img-cn-shanghai.aliyuncs.com/Koala.jpg@50h_50w_0e)
下面的url访问不了
[http://image.yhyfarm.com/headlogo.jpg@50h_50w_0e](http://image.yhyfarm.com/headlogo.jpg@50h_50w_0e)
缩放参数
## 设置文件元信息(Object Meta)
[https://help.aliyun.com/document_detail/32103.html?spm=5176.doc32105.6.377.NaGb15](https://help.aliyun.com/document_detail/32103.html?spm=5176.doc32105.6.377.NaGb15)
 带身份验证的请求表示的是如下两种情况：
- 请求头部中带Authorization，格式为OSS + AccessKeyId + 签名字符串。
- 请求的URL中带OSS AccessKeyId和Signature字段。
OSS根据URL解析出Bucket和Object。
php-sdk
[https://help.aliyun.com/document_detail/32099.html](https://help.aliyun.com/document_detail/32099.html)
要接入阿里云OSS，您需要拥有一对有效的 AccessKey(包括AccessKeyId和AccessKeySecret)用来进行签名认证。
RAM (Resource Access Management) 是阿里云为客户提供的用户身份管理与访问控制服务。按需为用户分配最小权限。
OSS作为CDN的源站
OSS的存储费用仅为ECS磁盘费用的50%
相比直接通过OSS访问，除极少额外增加的回源流量外，主要流量使用CDN流量，单价最低只需0.26GB，远远低于OSS直接访问的外网流量单价
总结：
1，开通oss，通过Access
 Key ID 操作文件，然后在cdn上添加oss域名，最后在自己的域名处加上cname指向cdn域名
======================================
oss的cdn配置
直接在cdn页面进行配置即可
CDN相关
#### CloudDeliveryNetwork    从Content到Cloud，重新定义CDN
[https://help.aliyun.com/document_detail/29908.html?spm=5176.7933691.203612.3.SFKTCq](https://help.aliyun.com/document_detail/29908.html?spm=5176.7933691.203612.3.SFKTCq)
启用CDN加速服务，需要将您的域名指向加速域名
