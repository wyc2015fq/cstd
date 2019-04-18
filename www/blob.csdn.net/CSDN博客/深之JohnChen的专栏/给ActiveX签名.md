# 给ActiveX签名 - 深之JohnChen的专栏 - CSDN博客

2013年02月22日 16:20:48[byxdaz](https://me.csdn.net/byxdaz)阅读数：2116
个人分类：[VC](https://blog.csdn.net/byxdaz/article/category/7211574)



给AcitveX签名有很多种方式，现介绍我所了解的几种： 

以下用到的工具请在http://ftp.intron.ac/pub/security/下载authenticode.zip文件 

一，使用微软的工具不采用私钥文件 

1.制作根证书 

makecert -sk "myPK" -ss mySSName -n "CN=公司名称" -r myroot.cer 

sk-表示主题的密钥容器位置，ss-主题的证书存储名称， n-证书颁发对象，r-证书存储位置； 

2.制作子证书 

makecert -sk "myPK" -is mySSName -n "CN=公司名称" -$ commercial -ic myroot.cer test.cer

sk-表示主题的密钥容器位置，is-颁发者的证书存储名称， n-证书颁发对象，ic-颁发者的证书存储位置，-$-授权范围（用于代码签名）； 

3.使用Cert2Spc生成spc发行者证书 

cert2spc test.cer test.spc 

4.使用signcode为你的程序，库或cab包签名：

双击signcode，或在控制台键入signcode，不带参数会启动签名向导。在第三步选择“自定义选项”，第四步选择“从文件选择”选择test.spc或test.cer，第五步选择“CSP中的私钥”，在密钥容器中选择我们定义的myPK,其他步骤默认即可，如果想添加时间戳，请在时间戳服务器地址上键入：（免费时间戳认证）
[http://timestamp.wosign.com/timestamp](http://timestamp.wosign.com/timestamp)

完成后，观察你所签名的文件属性，应该已经添加数字签名项。 

5.将myroot.cer导入“受信任的根证书颁发机构”，使用chktrust测试刚才的文件是否签名成功 

二，使用微软的工具采用私钥文件 

1.制作根证书 

makecert -sv "myroot.pvk" -ss mySSName -n "CN=公司名称" -r myroot.cer 

sv-私钥文件名，ss-主题的证书存储名称， n-证书颁发对象，r-证书存储位置； 

2.制作子证书 

makecert -sv "test.pvk" -iv myroot.pvk -n "CN=公司名称" -$ commercial -ic myroot.cer test.cer

sv-私钥文件名，iv-根证书的私钥文件， n-证书颁发对象，ic-颁发者的证书存储位置，-$-授权范围（用于代码签名）； 

3.使用Cert2Spc生成spc发行者证书 

cert2spc test.cer test.spc 

4.使用signcode为你的程序，库或cab包签名：

双击signcode，或在控制台键入signcode，不带参数会启动签名向导。在第三步选择“自定义选项”，

第四步选择“从文件选择”选择test.spc或test.cer，

第五步选择“文件中的私钥”选择test.pvk,其他步骤默认即可，如果想添加时间戳，请在时间戳服务器地址上键入：（免费时间戳认证）
[http://timestamp.wosign.com/timestamp](http://timestamp.wosign.com/timestamp)

完成后，观察你所签名的文件属性，应该已经添加数字签名项。 

用命令方式：signcode -spc test.spc -v test.pvk -n test的软件 test.cab 

注意：用signcode.exe签署自己的软件。假如是.cab文件，需要在用cabarc.exe制作的时候

用-s参数留出签名的空间(一般6144字节即可)。 

5.将myroot.cer导入“受信任的根证书颁发机构”，使用chktrust测试刚才的文件是否签名成功 

三，使用openssl产生根证书 

1.用openssl创建CA证书的RSA密钥(PEM格式)：

openssl genrsa -des3 -out ca.key 1024 

2.用openssl创建CA证书(PEM格式,假如有效期为一年)：

openssl req -new -x509 -days 365 -key ca.key -out ca.crt -config openssl.cnf 

openssl是可以生成DER格式的CA证书的，很奇怪Windows却说那证书是“无效的”，

无奈，只好用IE将PEM格式的CA证书转换成DER格式的CA证书。 

3.将ca.crt导入至IE中。

导入时注意一定要将证书存储至“本地计算机”。

具体步骤如下：

1)在“我的电脑”或“资源管理器”里双击该文件图标。

2)在“常规”卡片上选择“安装证书”。

3)点“下一步”至“证书导入向导”，选择“将所有的证书放入下列存储区”，

点下面的“浏览”。勾上“显示物理存储区”。选择“受信任的根目录...”下一级的

“本地计算机”。点“确定”，再点“下一步”。

4)点“完成”。 

可以检查一下导入是否完全成功：

在IE的Internet选项中的“证书”中“受信任根证书颁发机构”中应该可以

看见上述的根证书。 

4.IE的Internet选项中的“证书”中“受信任根证书颁发机构”中将刚才

导入的证书导出。格式为“DER编码的二进制X.509(.CER)”。

假设导出的文件名为ca.cer 

5.将PEM格式的ca.key转换为Microsoft可以识别的pvk格式。

  pvk -in ca.key -out ca.pvk -nocrypt -topvk 

6.步骤接第二种方式的第3步

