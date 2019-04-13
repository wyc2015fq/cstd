
# Apache 使用ssl模块配置HTTPS - 阳光岛主 - CSDN博客

2016年02月06日 09:05:52[阳光岛主](https://me.csdn.net/sunboy_2050)阅读数：58043


Web服务器在默认情况下使用HTTP，这是一个纯文本的协议。正如其名称所暗示的，纯文本协议不会对传输中的数据进行任何形式的加密。而基于HTTP的Web服务器是非常容易配置，它在安全方面有重大缺陷。任何”中间人”，通过精心防止的数据包嗅探器，是能够看到任何经过的数据包内容。更进一步，恶意用户甚至可以在传输路径设置一个假冒的WEB服务器冒名顶替实际的目标Web服务器。在这种情况下，最终用户可能实际上与假冒者服务器，而不是真正的目的服务器进行通信。这样，恶意用户可以通过精心设计的表单欺骗终端用户获取到敏感信息，如用户名密码。
为了处理这些类型的漏洞，大多数供应商往往在他们的web服务器应用HTTPS协议。对于只读类型的网站，用户只能读取内容，并没有实际提交任何信息，HTTP仍然是一个可行的选择。但是，对于保存敏感信息的网站，比如：用户需要登录来获得网站的服务，那么HTTPS是必须的。 HTTPS能够为一个网站提供以下能力。
确保所有经过服务器传输的数据包都是经过加密的。
建立了一个官方数字证书证书，使得假冒服务器无法冒充真正的服务器。
需要建立支持HTTPS的WEB服务器所要做的第一件事就是获得数字证书。数字证书可以在以下任一方法来获得。
自签名证书被推荐用于测试目的和个人项目。自签名证书，也可以用于服务提供商，不过一般适用于用户互相信任的情形。另外，自签名证书不用花钱购买。
证书可以由社区为基础的认证供应商如StartSSL和CACERT办法。这些证书也不需要花钱，但建议为个人项目。
对于全球性商业网站，建议从值得信赖的知名证书颁发机构购买证书。这些证书需要花钱，但他们增加了网络服务提供商的信誉。

**1. 准备**
在这篇文档中，我们将使用自签名证书。假设CentOS已经安装了Apache Web服务器。我们需要使用OpenSSL生成自签名证书。如果尚未安装OpenSSL，它可以使用yum来安装。
\# yum install mod_ssl openssl
安装完毕后，会自动生成**/etc/httpd/conf.d/ssl.conf**文件，下文配置会用到！
**2. 生成一个自签名证书**
下面的命令可以被用来产生一个自签名的证书。
**首先**，生成2048位的加密私钥
\# openssl genrsa -out server.key 2048
**然后**，生成[证书签名请求](http://en.wikipedia.org/wiki/Certificate_signing_request)（CSR），这里需要填写许多信息，如国家，省市，公司等
\# openssl req -new -key server.key -out server.csr
**最后**，生成类型为X509的自签名证书。有效期设置3650天，即有效期为10年
\# openssl x509 -req -days 3650 -in server.csr -signkey server.key -out server.crt
**生成密钥流程如下图，供参考**
![use-ssl-httpd-module-configuration-https](http://cdn-blog.mimvp.com/wp-content/uploads/2015/08/use-ssl-httpd-module-configuration-https.png)
创建证书后，将文件复制到对应的目录。
\# cp server.crt /etc/pki/tls/certs/
\# cp server.key /etc/pki/tls/private/
\# cp server.csr /etc/pki/tls/private/
**3. 配置Apache Web服务器**
**首先**，修改下面的配置文件。仅需配置红色部分**SSLCertificateFile**和**SSLCertificateKeyFile**
\# vim /etc/httpd/conf.d/ssl.conf
\#\#\# overwrite the following parameters \#\#\#
SSLCertificateFile /etc/pki/tls/certs/server.crt
SSLCertificateKeyFile /etc/pki/tls/private/server.key
\#\#\# The following parameter does not need to be modified in case of a self-signed certificate. \#\#\#
\#\#\# If you are using a real certificate, you may receive a certificate bundle. The bundle is added using the following parameters \#\#\#
SSLCertificateChainFile /etc/pki/tls/certs/example.com.ca-bundle
**然后**，重新启动httpd服务使更改生效
\# service httpd restart// 或者  /etc/init.d/httpd restart
Web服务器现在可以使用HTTPS
**4. 调整虚拟主机**
Apache Web服务器可以配置为多个Web站点。这些站点在httpd的配置文件中以虚拟主机的形式定义。例如，让我们假设我们的Apache Web服务器托管站点为proxy.mimvp.com，网站所有的文件都保存在/var/www/html/virtual-web目录。
对于虚拟主机，典型的HTTP配置是这样的。
\# vim /etc/httpd/conf/httpd.conf
NameVirtualHost *:80
<VirtualHost *:80>
ServerAdmin email@example.com
DocumentRoot /var/www/html/virtual-web
ServerName proxy.mimvp.com
</VirtualHost>
我们可以参考上面的配置创建HTTPS虚拟主机。
\# vim /etc/httpd/conf/httpd.conf
NameVirtualHost *:443
<VirtualHost *:443>
SSLEngine on
SSLCertificateFile /etc/pki/tls/certs/server.crt
SSLCertificateKeyFile /etc/pki/tls/private/server.key
<Directory /var/www/html/virtual-web>
AllowOverride All
</Directory>
ServerAdmin email@example.com
DocumentRoot /var/www/html/virtual-web
ServerName proxy.mimvp.com
</VirtualHost>
需要按照上面的配置，定义每个虚拟主机。添加虚拟主机后，重新启动Web服务。
\# service httpd restart
现在的虚拟主机就可以使用HTTPS
**实例见米扑代理**：[https://proxy.mimvp.com](https://proxy.mimvp.com/)
**5. 强制Apache Web服务器始终使用https**
如果由于某种原因，你需要站点的Web服务器都只使用HTTPS，此时就需要将所有HTTP请求(端口80)重定向到HTTPS(端口443)。 Apache Web服务器可以容易地做到这一点。
1，强制主站所有Web使用（全局站点）
如果要强制主站使用HTTPS，我们可以这样修改httpd配置文件：
\# vim /etc/httpd/conf/httpd.conf
ServerName www.example.com:80
Redirect permanent / https://www.example.com
重启Apache服务器，使配置生效：
\# service httpd restart
2，强制虚拟主机（单个站点）
如果要强制单个站点在虚拟主机上使用HTTPS，对于HTTP可以按照下面进行配置：
\# vim /etc/httpd/conf/httpd.conf
<VirtualHost *:80>
ServerName proxy.mimvp.com
Redirect permanent / https://proxy.mimvp.com/
</VirtualHost>
重启Apache服务器，使配置生效：
\# service httpd restart
单个站点全部使用HTTPS，则[http://proxy.mimvp.com](http://proxy.mimvp.com/)会强制重定向跳转到https://proxy.mimvp.com
一般情况下，由于浏览器会自动拦截https未被认证的网址，因此建议同时保留[http://proxy.mimvp.com](http://proxy.mimvp.com/)和https://proxy.mimvp.com，或者购买权威的认证服务，让用户浏览器信任https浏览访问。
**米扑博客实例：**
[http://proxy.mimvp.com](http://proxy.mimvp.com/)
https://proxy.mimvp.com
综上所述，当存在如用户登录，付款交易等交互时推荐HTTPS方式，可以提高服务器的安全性。
证书可以通过各种方式获得，如：自签名、社区提供的甚至商业机构够哦买的。管理员应当谨慎选择将要使用的证书的类型。


