# 证书创建工具 (Makecert.exe) - xqhrs232的专栏 - CSDN博客
2013年10月23日 16:20:53[xqhrs232](https://me.csdn.net/xqhrs232)阅读数：1863
原文地址::[http://www.cnblogs.com/yjmyzz/archive/2008/08/20/1272098.html](http://www.cnblogs.com/yjmyzz/archive/2008/08/20/1272098.html)
相关文章
1、**Cert2spc.exe（软件发行者证书测试工具）----[http://msdn.microsoft.com/zh-cn/f657tk8f(VS.80).aspx](http://msdn.microsoft.com/zh-cn/f657tk8f(VS.80).aspx)**
**2、****Pvk2Pfx----[http://technet.microsoft.com/zh-cn/subscriptions/ff550672](http://technet.microsoft.com/zh-cn/subscriptions/ff550672)**
**3、**如何：使用
 SignTool.exe 对安装文件进行签名 (Windows Installer)----[http://msdn.microsoft.com/zh-cn/subscriptions/ee290833(v=vs.100).aspx](http://msdn.microsoft.com/zh-cn/subscriptions/ee290833(v=vs.100).aspx)
证书创建工具生成仅用于测试目的的 X.509 证书。它创建用于数字签名的公钥和私钥对，并将其存储在证书文件中。此工具还将密钥对与指定发行者的名称相关联，并创建一个 X.509 证书，该证书将用户指定的名称绑定到密钥对的公共部分。
Makecert.exe 包含基本选项和扩展选项。基本选项是最常用于创建证书的选项。扩展选项提供更多的灵活性。
一定不要将此工具生成的证书私钥存储在 .snk 文件中。如果需要存储私钥，则应使用密钥容器。有关如何在密钥容器中存储私钥的更多信息，请参见[如何：将非对称密钥存储在密钥容器中](http://msdn.microsoft.com/zh-cn/tswxhw92(VS.80).aspx)。
应使用证书存储区来安全地存储证书。此工具使用的 .snk 文件以不受保护的方式存储私钥。创建或导入 .snk 文件时，在使用期间应注意保证其安全，并在使用后将其移除。
makecert [options] outputCertificateFile
|参数|说明|
|----|----|
|outputCertificateFile|测试 X.509 证书要写入的 .cer 文件的名称。|
![](http://i.msdn.microsoft.com/Platform/Controls/CollapsibleArea/resources/minus.gif) 基本选项
|选项|说明|
|----|----|
|-nx509name|指定主题的证书名称。此名称必须符合 X.500 标准。最简单的方法是在双引号中指定此名称，并加上前缀 CN=；例如，"CN=myName"。|
|-pe|将所生成的私钥标记为可导出。这样可将私钥包括在证书中。|
|-skkeyname|指定主题的密钥容器位置，该位置包含私钥。如果密钥容器不存在，系统将创建一个。|
|-srlocation|指定主题的证书存储位置。Location 可以是 currentuser（默认值）或 localmachine。|
|-ssstore|指定主题的证书存储名称，输出证书即存储在那里。|
|-#number|指定一个介于 1 和 2,147,483,647 之间的序列号。默认值是由 Makecert.exe 生成的唯一值。|
|-$authority|指定证书的签名权限，必须设置为 commercial（对于商业软件发行者使用的证书）或 individual（对于个人软件发行者使用的证书）。|
|-?|显示此工具的命令语法和基本选项列表。|
|-!|显示此工具的命令语法和扩展选项列表。|
![](http://i.msdn.microsoft.com/Platform/Controls/CollapsibleArea/resources/minus.gif) 扩展选项
|选项|说明|
|----|----|
|-aalgorithm|指定签名算法。必须是 md5（默认值）或 sha1。|
|-bmm/dd/yyyy|指定有效期的开始时间。默认为证书的创建日期。|
|-cycertType|指定证书类型。有效值是 end（对于最终实体）和 authority（对于证书颁发机构）。|
|-dname|显示主题的名称。|
|-emm/dd/yyyy|指定有效期的结束时间。默认为 12/31/2039 11:59:59 GMT。|
|-ekuoid[,oid]|将用逗号分隔的增强型密钥用法对象标识符 (OID) 列表插入到证书中。|
|-hnumber|指定此证书下面的树的最大高度。|
|-icfile|指定颁发者的证书文件。|
|-ikkeyName|指定颁发者的密钥容器名称。|
|-ikykeytype|指定颁发者的密钥类型，必须是 signature、exchange 或一个表示提供程序类型的整数。默认情况下，可传入 1 表示交换密钥，传入 2 表示签名密钥。|
|-in name|指定颁发者的证书公用名称。|
|-ipprovider|指定颁发者的 CryptoAPI 提供程序名称。|
|-irlocation|指定颁发者的证书存储位置。Location 可以是 currentuser（默认值）或 localmachine。|
|-isstore|指定颁发者的证书存储名称。|
|-ivpvkFile|指定颁发者的 .pvk 私钥文件。|
|-iypvkFile|指定颁发者的 CryptoAPI 提供程序类型。|
|-llink|到策略信息的链接（例如，一个 URL）。|
|-mnumber|以月为单位指定证书有效期的持续时间。|
|-nscp|包括 Netscape 客户端身份验证扩展。|
|-r|创建自签署证书。|
|-scfile|指定主题的证书文件。|
|-skykeytype|指定主题的密钥类型，必须是 signature、exchange 或一个表示提供程序类型的整数。默认情况下，可传入 1 表示交换密钥，传入 2 表示签名密钥。|
|-spprovider|指定主题的 CryptoAPI 提供程序名称。|
|-svpvkFile|指定主题的 .pvk 私钥文件。如果该文件不存在，系统将创建一个。|
|-sytype|指定主题的 CryptoAPI 提供程序类型。|

作者：[菩提树下的杨过](http://yjmyzz.cnblogs.com/)
出处：[http://yjmyzz.cnblogs.com](http://yjmyzz.cnblogs.com/)
本文版权归作者和博客园共有，欢迎转载，但未经作者同意必须保留此段声明，且在文章页面明显位置给出原文连接，否则保留追究法律责任的权利。
