
# 数字证书认证(CA)中心 - 尹成的技术博客 - CSDN博客

2018年11月14日 14:40:46[尹成](https://me.csdn.net/yincheng01)阅读数：143


CA(Certification Authorith)：证书颁发机构.
证书引入：要开车得先考驾照，驾照上面有本人得照片，姓名，出生日期等个人信息，以及有效期，准驾车辆得类型等信息，并由公安局在上面盖章，我们只要看到驾照，就可以知道公安局认定此人具有驾驶车辆的资格。
证书：也称之为公钥证书，和驾照很相似，里面有姓名，组织，邮箱，地址等个人信息，以及属于此人的公钥，并由认证机构施加数字签名所形成的一段信息。
证书的应用场景：
![这里写图片描述](https://img-blog.csdn.net/20180905223201432?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3dlaXhpbl80MjI4NjAyMw==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)
通过openssl生成CA并签发证书：
[](https://img-blog.csdn.net/20180905223201432?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3dlaXhpbl80MjI4NjAyMw==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)修改配置文件：D:\OpenSSL-Win64\bin\cnf\openssl.cnf
![这里写图片描述](https://img-blog.csdn.net/20180905223316753?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3dlaXhpbl80MjI4NjAyMw==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)
使用rsa算法生成私钥：
![这里写图片描述](https://img-blog.csdn.net/20180905223354692?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3dlaXhpbl80MjI4NjAyMw==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)
根据私钥生成自签证书：
![这里写图片描述](https://img-blog.csdn.net/20180905223447571?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3dlaXhpbl80MjI4NjAyMw==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)
在demoCA目录下创建certs,newcerts,crl文件夹，创建index.txt和serial文件，并向serial写入01，作为第一张证书的序号。
![这里写图片描述](https://img-blog.csdn.net/20180905223525948?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3dlaXhpbl80MjI4NjAyMw==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)
![这里写图片描述](https://img-blog.csdn.net/2018090522360718?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3dlaXhpbl80MjI4NjAyMw==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)
下面准备为别人签署证书，首先在PEM目录下创建httpd(名称任意),然后在httpd下创建ssl(名称任意)，创建私钥：
![这里写图片描述](https://img-blog.csdn.net/20180905223636268?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3dlaXhpbl80MjI4NjAyMw==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)
填写相应信息，准备申请证书：
![这里写图片描述](https://img-blog.csdn.net/20180905223704353?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3dlaXhpbl80MjI4NjAyMw==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)
签署证书：
![这里写图片描述](https://img-blog.csdn.net/20180905223738724?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3dlaXhpbl80MjI4NjAyMw==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)
效果：serial中的01变成02，index.txt已经写入一条证书
![这里写图片描述](https://img-blog.csdn.net/20180905223938477?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3dlaXhpbl80MjI4NjAyMw==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)
学院Go语言视频主页
[https://edu.csdn.net/lecturer/1928](https://edu.csdn.net/lecturer/1928)
[清华团队带你实战区块链开发](https://ke.qq.com/course/344443?tuin=3d17195d)
扫码获取海量视频及源码   QQ群：721929980
![在这里插入图片描述](https://img-blog.csdnimg.cn/20181114143613461.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3lpbmNoZW5nMDE=,size_16,color_FFFFFF,t_70)

