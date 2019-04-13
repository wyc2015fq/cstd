
# 十大免费SSL证书：网站免费添加HTTPS加密 - 阳光岛主 - CSDN博客

2017年09月24日 09:15:54[阳光岛主](https://me.csdn.net/sunboy_2050)阅读数：35987个人分类：[NetWork																](https://blog.csdn.net/sunboy_2050/article/category/698436)



SSL证书，用于加密HTTP协议，也就是HTTPS。随着淘宝、百度等网站纷纷实现全站Https加密访问，搜索引擎对于Https更加友好，加上互联网上越来越多的人重视隐私安全，站长们给网站添加SSL证书似乎成为了一种趋势。
给自己的网站添加SSL证书其实并不复杂，但是关键一点就是首先要拥有一个SSL证书。由于SSL证书价格不菲，很多个人站长会选择放弃使用Https。但是，自从开源、免费的Let's Encrypt证书出现后，我觉得SSL也是我们草根站长可以玩的了。
**SSL 证书级别**
分为三种类型，域名型SSL证书（DV SSL）、企业型SSL证书（OVSSL）、增强型SSL证书（EVSSL）
![https-ssl-mian-fei-zheng-shu-fu-wu-shen-qing-01](http://blog.mimvp.com/wp-content/uploads/2017/12/https-ssl-mian-fei-zheng-shu-fu-wu-shen-qing-01.png)
**1. 域名型 SSL 证书（DV SSL -**Domain Validation SSL**）**
即证书颁布机构只对域名的所有者进行在线检查，通常是验证域名下某个指定文件的内容，或者验证与域名相关的某条 TXT 记录；
比如访问 [http|https]://www.mimvp.com/.../test.txt，文件内容： 2016082xxxxxmimvpcom2016
或添加一条 TXT 记录：www.mimvp.com –> TXT –> 20170xxxxxmimvpcom2066
**2. 企业型 SSL 证书（OV SSL -**Organization Validation SSL**）**
是要购买者提交组织机构资料和单位授权信等在官方注册的凭证，
证书颁发机构在签发 SSL 证书前，不仅仅要检验域名所有权，
还必须对这些资料的真实合法性进行多方查验，只有通过验证的才能颁发 SSL 证书。
**3. 增强型 SSL 证书（EV SSL -**Extended Validation SSL**）**
与其他 SSL 证书一样，都是基于 SSL/TLS 安全协议，
但是验证流程更加具体详细，验证步骤更多，
这样一来证书所绑定的网站就更加的可靠、可信。
它跟普通 SSL 证书的区别也是明显的，安全浏览器的地址栏变绿，
如果是不受信的 SSL 证书则拒绝显示，如果是钓鱼网站，地址栏则会变成红色，以警示用户。

当前可供大家免费使用的SSL证书：
**一、Let's Encrypt**（推荐）
官网：[https://letsencrypt.org](https://letsencrypt.org/)
1、Let's Encrypt是国外一个公共的免费SSL项目，由 Linux 基金会托管，它的来头不小，由Mozilla、思科、Akamai、IdenTrust和EFF等组织发起，目的就是向网站自动签发和管理免费证书，以便加速互联网由HTTP过渡到HTTPS。
2、Let's Encrypt安装部署简单、方便，目前Cpanel、Oneinstack等面板都已经集成了Let's Encrypt一键申请安装，网上也有不少的利用Let's Encrypt开源的特性制作的在线免费SSL证书申请网站，总之Let's Encrypt得到大家的认可。
3、最后选择Let's Encrypt，一方面是Let’s Encrypt SSL已经被Firefox、Chrome、IE等浏览器所支持，IE 6 用户估计也不怎么看我的博客，所以可以忽略。另一方面，Let’s Encrypt SSL证书下载和安装已经是傻瓜式的简单了。
**二、StartSSL**
官网：[https://www.startcomca.com](https://www.startcomca.com/)
1、StartSSL是StartCom公司旗下的SSL证书，应该算是免费SSL证书中的“鼻祖”，最早提供完全免费的SSL证书并且被各大浏览器所支持的恐怕就只有StartSSL证书了。任何个人都可以从StartSSL中申请到免费一年的SSL证书。
2、首次申请StartSSL免费SSL证书是免费一年，但是你可以在第二年继续续期。之前StartSSL管理SSL证书只有本地浏览器安装数字证书一种，所以一旦本地的数字证书丢失的话就无法获取到自己之前申请的证书了，不过现在已经增加了邮箱登录了。
3、如果你有看新闻，也许已经知道了“Mozilla正式提议将停止信任 WoSign 和 StartCom 签发的新证书”，使用有风险，不推荐。
**三、COMODO PositiveSSL**
官网：[https://www.positivessl.com](https://www.positivessl.com/)
1、COMODO官网只有免费90天的SSL证书试用申请，这个COMODO PositiveSSL证书来自UK2公司，VPS.net等就是UK2公司旗下的产品。目前获取UK2提供的免费COMODO PositiveSSL不需要额外的操作，只需要你将域名的IP地址解析到指定的IP即可。
2、先把域名解析到UK2公司的服务器上，然后在网页上获取SSL证书并下载，最后你就可以解除域名解析，同时将下载的域名证书文件上传到服务器配置SSL即可。不过由于是UK2提供的COMODO PositiveSSL免费证书，如果你没有用他们的主机总归不知道哪一天会出问题的。
**四、CloudFlare SSL**
官网：[https://www.cloudflare.com/ssl/](https://www.cloudflare.com/ssl/)
1、CloudFlare提供的免费SSL证书是UniversalSSL，即通用SSL，用户无需向证书发放机构申请和配置证书就可以使用的SSL证书，CloudFlare向所有用户(包括免费用户)提供SSL加密功能。
2、不过Universal SSL的服务对免费用户有限制，CloudFlare只支持扩展支持Server Name Indication(SNI)协议的现代浏览器，这意味着它不支持IE6及之前版本、运行Android 2.2或更旧版本的Android浏览器。
**五、Wosign沃通SSL**
官网：[https://www.wosign.com](https://www.wosign.com/)
1、Wosign沃通是国内一家提供SSL证书服务的网站，其免费的SSL证书申请比较简单，在线开通，一个SSL证书只能对应一个域名，支持证书状态在线查询协议(OCSP)。
2、由于Wosign沃通SSL是一家国内的SSL服务商，所以SSL证书申请和管理都比较简单，并且网站使用的是中文有问题还可以联系客服。不过，受“Mozilla正式提议将停止信任 WoSign 和 StartCom 签发的新证书”的影响，使用有风险，不推荐。
3、Wosign 目前已经停止了免费证书申请
**六、腾讯云DV SSL 证书**（推荐）
官网：[https://cloud.tencent.com/product/ssl](https://cloud.tencent.com/product/ssl)
腾讯云DV SSL 域名型证书由赛门铁克提供自动审核认证，快速签发，
支持自动 CSR 生成、域名身份 DNS 自动验证，一步提交申请，审核签发流程全自动。
可以一键部署到腾讯云资源，轻松获得数据安全。
可以下载证书，用于其它云商，例如用于阿里云
可以多次申请，每次一个二级域名，如有多个域名可申请多次
**七、阿里云DV SSL证书**（推荐）
官网：[https://common-buy.aliyun.com/?commodityCode=cas](https://common-buy.aliyun.com/?commodityCode=cas#/buy)
赛门铁克是 SSL/TLS 证书的领先提供商，为全球一百多万台网络服务器提供安全防护。
选择赛门铁克后，证书颁发机构 (CA) 将妥善保护您的网站和信誉，让您安枕无忧。
免费数字证书，最多保护一个明细子域名，不支持通配符，一个阿云帐户最多签发20张免费证书
保护一个明细域名，例如:[mimvp.com](http://mimvp.com/)，[proxy.mimvp.com](http://proxy.mimvp.com/)，[blog.mimvp.com](http://blog.mimvp.com/)，各个明细子域名都算一个域名，
如果每一个明细域名，都需要配置SSL，则需要分别申请多个免费的SSL证书
**八、360网站卫士、百度云加速免费SSL、又拍云、七牛云**
百度云：[https://console.bce.baidu.com/cas/](https://console.bce.baidu.com/cas/#/cas/apply/create)（有免费，不支持导出，只可用于百度云产品）
七牛云：[SSL证书服务](https://portal.qiniu.com/certificate/apply)（有免费，需登录查看）
又拍云：[SSL证书申购](https://console.upyun.com/toolbox/createCertificate/)（有免费，需登录查看）
360网站卫士、百度云加速与Symantec等合作推出了免费的SSL证书，其实类似于上面的腾讯云DV SSL 证书，
只不过360网站卫士如果要使用SSL证书必须得实名认证，而且还得使用他们家的CDN。
而百度云加速，只能使用百度云服务器才可以申请免费SSL证书。
**九、AlphaSSL**
官网：[https://www.alphassl.com](https://www.alphassl.com/)
AlphaSSL 是国外一个网站，背景不详，只是从今年四月份开始陆续有朋友告诉我这个网站提供了免费AlphaSSL证书申请，部落自己试了一下发现申请容易，但是成功率并不是100%。
**十、AWS 免费 SSL证书**
官网：[AWS Certificate Manager](https://aws.amazon.com/cn/certificate-manager/?sc_channel=PS&sc_campaign=acquisition_CN&sc_publisher=baidu&sc_category=pc&sc_medium=certificate_manager_nb&sc_content=ssl_certificate_p&sc_detail=%E5%85%8D%E8%B4%B9ssl&sc_segment=100019559&sc_matchtype=exact&sc_country=CN&s_kwcid=AL!4422!88!14673029713!!57573035173&ef_id=WcW4CgAABMy4aSG0:20170924001218:s)
通过 AWS Certificate Manager 预置 SSL/TLS 证书不收取任何额外费用，您只需为创建用于运行应用程序的 AWS 资源付费，比如 Elastic Load Balancer、Amazon CloudFront 分配或 Amazon API Gateway 上的 API。
AWS Certificate Manager 是一项服务，可帮助您轻松地预置、管理和部署用于 AWS 服务的安全套接字层/传输层安全性 (SSL/TLS) 证书。SSL/TLS 证书用于保护网络通信的安全并确立网站在 Internet 上的身份。使用 AWS Certificate Manager，您无需再为购买、上传和更新 SSL/TLS 证书而经历耗时的手动流程。利用 AWS Certificate Manager，您可以快速申请证书，在 AWS 资源 (如 Elastic Load Balancer、Amazon CloudFront 分配和 API Gateway 上的 API) 上部署该证书，并让 AWS Certificate Manager 处理证书更新事宜。
**便宜SSL证书的**有很多家：
Namecheap：[Cheap SSL Certificates from $7.95/yr • Namecheap.com](https://link.zhihu.com/?target=https%3A//www.namecheap.com/security/ssl-certificates.aspx)
ssls：[SSL Certificates. Buy Cheap SSL Certs from $4.99/yr](https://link.zhihu.com/?target=https%3A//www.ssls.com/)
cheapssl：[Cheap SSL Certificates. Buy or Renew Cheapest SSL at $4.80](https://cheapsslsecurity.com/)
Gogetssl：[https://www.gogetssl.com/domain-validation/comodo-positive-ssl/](https://www.gogetssl.com/domain-validation/comodo-positive-ssl/)
Starfieldtech：[https://www.starfieldtech.com/](https://www.starfieldtech.com/)

**小结**
1、几年前使用SSL证书的网站，仅限于一些电子商务类的网站，但是现在各大搜索引擎、各类行业网站都纷纷上马了Https，而SSL证书价格也是越来越低，免费的SSL证书也越来越多了，可供大家的选择也是越来越多了，全站和全网使用SSL HTTPS已经是大势所趋了。
2、上面介绍的十大免费SSL证书，要说最让人放心的当属Let's Encrypt、腾讯云和阿里云的DV SSL证书，效果可以参考米扑科技的多个产品网站，覆盖了全部使用场景。其它免费SSL证书，建议大家谨慎使用，对于一些重要的网站还是建议你直接购买SSL证书。
3、 米扑科技，作为一家技术驱动型的初创公司，充分调研、实践了免费和收费SSL证书，可供参考。
米扑科技：[ https://mimvp.com](http://https//mimvp.com)（全站使用HTTPS）
米扑代理：[https://proxy.mimvp.com](https://proxy.mimvp.com/)（第一家全站使用HTTPS的代理服务商）
米扑域名：[https://domain.mimvp.com](https://domain.mimvp.com/)
米扑支付：[https://pay.mimvp.com](https://pay.mimvp.com/)
米扑财富：[https://money.mimvp.com](https://money.mimvp.com/)
米扑博客：[https://blog.mimvp.com](https://blog.mimvp.com/)（全站使用HTTPS的个人博客）




