# Django 安全漏洞 - 宇宙浪子的专栏 - CSDN博客
2014年09月21日 23:38:31[_宇宙浪子_](https://me.csdn.net/bluehawksky)阅读数：1931
本文整理自：[http://sebug.net/search?wd=Django](http://sebug.net/search?wd=Django)
[Django](http://sebug.net/appdir/Django)
- 2013-09-15[Django密码哈希拒绝服务漏洞(CVE-2013-1443)](http://sebug.net/vuldb/ssvid-61033)
- 2013-09-11[Django 目录遍历序列(CVE-2013-4315)](http://sebug.net/vuldb/ssvid-61010)
- 2013-09-11[Django "ssi"模板标签目录遍历漏洞(CVE-2013-4315)](http://sebug.net/vuldb/ssvid-61012)
- 2013-08-19[Django "is_safe_url()" 跨站脚本和 "URLField" 脚本插入漏洞](http://sebug.net/vuldb/ssvid-60965)
- 2013-02-19[Django 1.3/1.4 拒绝服务和信息泄露漏洞](http://sebug.net/vuldb/ssvid-60639)
- 2012-12-04[django CMS 'page_attribute' Template HTML 注入漏洞](http://sebug.net/vuldb/ssvid-60610)
- 2012-10-19[Django 1.3.1 'HttpRequest.get_host()'信息泄露漏洞](http://sebug.net/vuldb/ssvid-60442)
- 2012-07-31[Django跨站脚本执行和两个拒绝服务漏洞](http://sebug.net/vuldb/ssvid-60305)
- 2011-09-12[Django开发框架多个安全漏洞](http://sebug.net/vuldb/ssvid-20926)
- 2007-01-24[Django消息文件远程任意命令执行漏洞](http://sebug.net/vuldb/ssvid-1275)
- Django密码哈希拒绝服务漏洞(CVE-2013-1443)
SSV-ID: [61033](http://sebug.net/vuldb/ssvid-61033)
[排行](http://sebug.net/branking)[Wiki](http://ssv.sebug.net/N_list)[捐款](http://sebug.net/donate)[漏洞扫描](http://sebug.net/lto?url=http://www.scanv.com)[云存储](http://sebug.net/lto?url=http://www.upyun.com/)[VPS](http://sebug.net/lto?url=http://www.linode.com/?r=c4c36f1a2d3b525013a28fca90433f1c47ae0739)[Mac](http://mac.gd/)
SSV-AppDir: [Django漏洞](http://sebug.net/appdir/Django)
发布时间: 2013-09-15 (GMT+0800)
#### 受影响产品:
Django <= 1.5.4
Django <= 1.4.8
#### 漏洞描述:
BUGTRAQ  ID: 62409
CVE ID: CVE-2013-1443
Django是Python编程语言驱动的一个开源Web应用程序框架
Django 1.4.8 and 1.5.4之前版本在计算已提交密码的哈希时，django.contrib.auth中存在错误，通过发送较大的密码，攻击者可利用此漏洞造成受影响系统崩溃，导致拒绝服务
<* 参考
> [http://secunia.com/advisories/54815/](http://sebug.net/lto?url=http://secunia.com/advisories/54815/)
[http://sebug.net/appdir/Django](http://sebug.net/appdir/Django)
*>
#### Sebug安全建议:
厂商补丁：
Django
------
目前厂商已经发布了升级补丁以修复这个安全问题，请到厂商的主页下载：
http://www.djangoproject.com/
https://www.djangoproject.com/weblog/2013/sep/15/security/
@Sebug.net [ 2013-09-18 ]
- Django 目录遍历序列(CVE-2013-4315)
SSV-ID: [61010](http://sebug.net/vuldb/ssvid-61010)
[排行](http://sebug.net/branking)[Wiki](http://ssv.sebug.net/N_list)[捐款](http://sebug.net/donate)[漏洞扫描](http://sebug.net/lto?url=http://www.scanv.com)[云存储](http://sebug.net/lto?url=http://www.upyun.com/)[VPS](http://sebug.net/lto?url=http://www.linode.com/?r=c4c36f1a2d3b525013a28fca90433f1c47ae0739)[Mac](http://mac.gd/)
SSV-AppDir: [Django漏洞](http://sebug.net/appdir/Django)
发布时间: 2013-09-11 (GMT+0800)
#### 受影响产品:
Django 1.4.x
#### 漏洞描述:
BUGTRAQ  ID: 62332
CVE(CAN) ID: CVE-2013-4315
Django是Python编程语言驱动的一个开源Web应用程序框架。
Django在ssi模板标签的实现上存在目录遍历漏洞，攻击者可利用此漏洞获取敏感信息。
<* 参考
> [https://bugzilla.redhat.com/show_bug.cgi?id=1004969](http://sebug.net/lto?url=https://bugzilla.redhat.com/show_bug.cgi?id=1004969)
[http://sebug.net/appdir/Django](http://sebug.net/appdir/Django)
*>
#### Sebug安全建议:
厂商补丁：
Django
------
目前厂商已经发布了升级补丁以修复这个安全问题，请到厂商的主页下载：
http://www.djangoproject.com/
@Sebug.net [ 2013-09-14 ]
- Django "ssi"模板标签目录遍历漏洞(CVE-2013-4315)
SSV-ID: [61012](http://sebug.net/vuldb/ssvid-61012)
[排行](http://sebug.net/branking)[Wiki](http://ssv.sebug.net/N_list)[捐款](http://sebug.net/donate)[漏洞扫描](http://sebug.net/lto?url=http://www.scanv.com)[云存储](http://sebug.net/lto?url=http://www.upyun.com/)[VPS](http://sebug.net/lto?url=http://www.linode.com/?r=c4c36f1a2d3b525013a28fca90433f1c47ae0739)[Mac](http://mac.gd/)
SSV-AppDir: [Django漏洞](http://sebug.net/appdir/Django)
发布时间: 2013-09-11 (GMT+0800)
#### 受影响产品:
Django 1.5.x
Django 1.4.x
#### 漏洞描述:
BUGTRAQ  ID: 62332
CVE(CAN) ID: CVE-2013-4315
Django是Python编程语言驱动的一个开源Web应用程序框架。
Django 1.4.7、1.5.3之前版本在处理"ssi"标签时没有在template/defaulttags.py内正确验证ALLOWED_INCLUDE_ROOTS设置，即用于读取文件，远程攻击者通过目录遍历序列利用此漏洞可获取敏感信息。
<* 参考
> [http://secunia.com/advisories/54772/](http://sebug.net/lto?url=http://secunia.com/advisories/54772/)
[http://sebug.net/appdir/Django](http://sebug.net/appdir/Django)
*>
#### Sebug安全建议:
厂商补丁：
Django
------
目前厂商已经发布了升级补丁以修复这个安全问题，请到厂商的主页下载：
http://www.djangoproject.com/
https://www.djangoproject.com/weblog/2013/sep/10/security-releases-issued/
@Sebug.net [ 2013-09-14 ]
- Django "is_safe_url()" 跨站脚本和 "URLField" 脚本插入漏洞
SSV-ID: [60965](http://sebug.net/vuldb/ssvid-60965)
[排行](http://sebug.net/branking)[Wiki](http://ssv.sebug.net/N_list)[捐款](http://sebug.net/donate)[漏洞扫描](http://sebug.net/lto?url=http://www.scanv.com)[云存储](http://sebug.net/lto?url=http://www.upyun.com/)[VPS](http://sebug.net/lto?url=http://www.linode.com/?r=c4c36f1a2d3b525013a28fca90433f1c47ae0739)[Mac](http://mac.gd/)
SSV-AppDir: [Django漏洞](http://sebug.net/appdir/Django)
发布时间: 2013-08-19 (GMT+0800)
#### 受影响产品:
Django 1.4.x
#### 漏洞描述:
CVE(CAN) ID: CVE-2013-4249
Django是Python编程语言驱动的一个开源Web应用程序框架。
Django 1.4、1.5没有正确过滤django.contrib.admin的"URLField"字段、"django.utils.http.is_safe_url()"函数(utils/http.py) 的URL重定向相关输入没有被正确过滤，可被利用插入任意HTML和脚本代码，导致这些恶意代码被查看时，会在受影响站点上下文的浏览器会话中执行。
<* 参考
> [http://secunia.com/advisories/54476/](http://sebug.net/lto?url=http://secunia.com/advisories/54476/)
[http://sebug.net/appdir/Django](http://sebug.net/appdir/Django)
*>
#### Sebug安全建议:
厂商补丁：
Django
------
目前厂商已经发布了升级补丁以修复这个安全问题，请到厂商的主页下载：
http://www.djangoproject.com/
https://www.djangoproject.com/weblog/2013/aug/13/security-releases-issued/
@Sebug.net [ 2013-08-27 ]
- Django 1.3/1.4 拒绝服务和信息泄露漏洞
SSV-ID: [60639](http://sebug.net/vuldb/ssvid-60639)
[排行](http://sebug.net/branking)[Wiki](http://ssv.sebug.net/N_list)[捐款](http://sebug.net/donate)[漏洞扫描](http://sebug.net/lto?url=http://www.scanv.com)[云存储](http://sebug.net/lto?url=http://www.upyun.com/)[VPS](http://sebug.net/lto?url=http://www.linode.com/?r=c4c36f1a2d3b525013a28fca90433f1c47ae0739)[Mac](http://mac.gd/)
SSV-AppDir: [Django漏洞](http://sebug.net/appdir/Django)
发布时间: 2013-02-19 (GMT+0800)
#### 受影响产品:
Django 1.4.x
Django 1.3.x
#### 漏洞描述:
BUGTRAQ  ID: 58061
CVE(CAN) ID: CVE-2013-0305,CVE-2013-0306
Django是Python编程语言驱动的一个开源Web应用程序框架。
Django 1.3.6、1.4.4之前版本在实现上存在多个漏洞，可被恶意用户利用绕过某些安全限制并造成拒绝服务。
1、扩展XML实体时存在错误，通过包含恶意属性的特制XML，可消耗大量内存。
2、处理某些XML数据时存在错误，通过包含外部实体引用的特制XML数据，可泄露敏感信息。
3、在访问历史视图时，管理员界面没有正确验证访问权限，可被利用查看管理界面内的对象访问历史。
4、在处理表单提交时表单集中存在错误，通过提交特制的表单，可导致大量内存消耗并显示应用不可用。
<* 参考
> [https://www.djangoproject.com/weblog/2013/feb/19/security/](http://sebug.net/lto?url=https://www.djangoproject.com/weblog/2013/feb/19/security/)
[http://secunia.com/advisories/52243/](http://sebug.net/lto?url=http://secunia.com/advisories/52243/)
[http://sebug.net/appdir/Django](http://sebug.net/appdir/Django)
*>
#### Sebug安全建议:
厂商补丁：
Django
------
目前厂商已经发布了升级补丁以修复这个安全问题，请到厂商的主页下载：
https://www.djangoproject.com/m/releases/1.3/Django-1.3.6.tar.gz
@Sebug.net [ 2013-02-22 ]
- django CMS 'page_attribute' Template HTML 注入漏洞
SSV-ID: [60610](http://sebug.net/vuldb/ssvid-60610)
[排行](http://sebug.net/branking)[Wiki](http://ssv.sebug.net/N_list)[捐款](http://sebug.net/donate)[漏洞扫描](http://sebug.net/lto?url=http://www.scanv.com)[云存储](http://sebug.net/lto?url=http://www.upyun.com/)[VPS](http://sebug.net/lto?url=http://www.linode.com/?r=c4c36f1a2d3b525013a28fca90433f1c47ae0739)[Mac](http://mac.gd/)
发布时间: 2012-12-04 (GMT+0800)
#### 受影响产品:
django CMS 2.x
#### 漏洞描述:
BUGTRAQ  ID: 57559
django CMS是用pony作为技术支持的CMS，可以前端编辑。
django CMS 2.3.5 之前版本的page_attribute模板标签没有正确过滤，在实现上存在HTML注入漏洞，成功利用后可允许在受影响浏览器中执行HTML和脚本代码。
<* 参考
> [http://secunia.com/advisories/51953/](http://sebug.net/lto?url=http://secunia.com/advisories/51953/)
*>
#### Sebug安全建议:
厂商补丁：
django CMS
----------
目前厂商已经发布了升级补丁以修复这个安全问题，请到厂商的主页下载2.3.5版本：
https://www.django-cms.org/en/
@Sebug.net [ 2013-01-30 ]
- Django 1.3.1 'HttpRequest.get_host()'信息泄露漏洞
SSV-ID: [60442](http://sebug.net/vuldb/ssvid-60442)
[排行](http://sebug.net/branking)[Wiki](http://ssv.sebug.net/N_list)[捐款](http://sebug.net/donate)[漏洞扫描](http://sebug.net/lto?url=http://www.scanv.com)[云存储](http://sebug.net/lto?url=http://www.upyun.com/)[VPS](http://sebug.net/lto?url=http://www.linode.com/?r=c4c36f1a2d3b525013a28fca90433f1c47ae0739)[Mac](http://mac.gd/)
SSV-AppDir: [Django漏洞](http://sebug.net/appdir/Django)
发布时间: 2012-10-19 (GMT+0800)
#### 受影响产品:
Django 1.3.1
#### 漏洞描述:
Bugtraq ID:56146
CVE ID:CVE-2012-4520
Django是一款开放源代码的Web应用框架，由Python写成。
当处理HTTP "Host"头数据时"HttpRequest.get_host()"方法存在漏洞，可被利用构建特制的头字段泄露任意URLS。
<* 参考
> [http://secunia.com/advisories/51033/](http://sebug.net/lto?url=http://secunia.com/advisories/51033/)
[http://sebug.net/appdir/Django](http://sebug.net/appdir/Django)
*>
#### Sebug安全建议:
厂商解决方案
Django 1.3.4或1.4.2已经修复此漏洞，建议用户下载使用：
https://www.djangoproject.com/weblog/2012/oct/17/security/
@Sebug.net [ 2012-10-23 ]
## Django跨站脚本执行和两个拒绝服务漏洞
SSV-ID: [60305](http://sebug.net/vuldb/ssvid-60305)
[排行](http://sebug.net/branking)[Wiki](http://ssv.sebug.net/N_list)[捐款](http://sebug.net/donate)[漏洞扫描](http://sebug.net/lto?url=http://www.scanv.com)[云存储](http://sebug.net/lto?url=http://www.upyun.com/)[VPS](http://sebug.net/lto?url=http://www.linode.com/?r=c4c36f1a2d3b525013a28fca90433f1c47ae0739)[Mac](http://mac.gd/)
SSV-AppDir: [Django漏洞](http://sebug.net/appdir/Django)
发布时间: 2012-07-31 (GMT+0800)
#### 受影响产品:
Django 1.4.x
Django 1.3.x
#### 漏洞描述:
BUGTRAQ  ID: 54742
CVE ID: CVE-2012-3442,CVE-2012-3443,CVE-2012-3444
Django是Python编程语言驱动的一个开源Web应用程序框架。
Django 1.3和1.4及其他版本在实现上存在两个安全漏洞，可被恶意用户利用执行跨站脚本攻击和拒绝服务。
1）验证框架中的login()或logout()试图的重定向功能内传递的输入，在重定向到"data:" scheme URL后没有正确过滤就返回给用户。
2）解压图形时，ImageField类中的图形验证中存在错误，可被利用消耗大量内存资源。
3）在ImageFile类中的图形大小计算存在错误，可被利用消耗服务器资源并造成应用停止响应。
<* 参考
> [https://www.djangoproject.com/weblog/2012/](http://sebug.net/lto?url=https://www.djangoproject.com/weblog/2012/)
[http://sebug.net/appdir/Django](http://sebug.net/appdir/Django)
*>
#### Sebug安全建议:
厂商补丁：
Django
------
Django已经为此发布了一个安全公告（jul）以及相应补丁:
jul：Cross-site scripting in authentication views
链接：https://www.djangoproject.com/weblog/2012/
@Sebug.net [ 2012-08-03 ]
- Django开发框架多个安全漏洞
SSV-ID: [20926](http://sebug.net/vuldb/ssvid-20926)
[排行](http://sebug.net/branking)[Wiki](http://ssv.sebug.net/N_list)[捐款](http://sebug.net/donate)[漏洞扫描](http://sebug.net/lto?url=http://www.scanv.com)[云存储](http://sebug.net/lto?url=http://www.upyun.com/)[VPS](http://sebug.net/lto?url=http://www.linode.com/?r=c4c36f1a2d3b525013a28fca90433f1c47ae0739)[Mac](http://mac.gd/)
SSV-AppDir: [Django漏洞](http://sebug.net/appdir/Django)
发布时间: 2011-09-12 (GMT+0800)
#### 受影响产品:
Django 1.2.5
Django 1.3 beta 1
Django 1.2.4
Django 1.2.2
Django 1.2
#### 漏洞描述:
Bugtraq ID: 49573
Django是一款开放源代码的Web应用框架，由Python写成。
Django存在多个安全漏洞，允许攻击者获得敏感信息，操作数据，进行缓存毒药攻击或进行拒绝服务攻击。
1)当使用缓存后端时django.contrib.sessions中处理会话存在错误，可被利用操作会话信息。要成功个利用漏洞需要已知会话KEY和应用程序允许攻击者使用合法会话KEY储存字典类对象到缓冲中。
2)Django模型系统包括一个字段类型-- URLField --，用于校验提供的值是否为合法URL，如果布尔关键字参数verify_exists为真，会尝试校验提供的URL并解析。默认情况下，底层套接字没有超时设置，攻击者可以利用此漏洞发送特制URL消耗所有服务器内存，造成拒绝服务攻击。
3)当校验提供给"URLField"字段类型的URLs处理重定向应答存在错误，攻击者可以利用此漏洞把重定向应答返回给"file://" URL，可判断服务器上的本地文件是否存在。
4)当生成重定向应答的全路径URL时处理"X-Forwarded-Host" HTTP头存在错误，攻击者可以利用此漏洞进行缓存毒药攻击。
<* 参考
> [https://www.djangoproject.com/weblog/2011/sep/09/security-releases-issued/](https://www.djangoproject.com/weblog/2011/sep/09/security-releases-issued/)
[http://secunia.com/advisories/45939/](http://secunia.com/advisories/45939/)
[http://sebug.net/appdir/Django](http://sebug.net/appdir/Django)
*>
#### Sebug安全建议:
厂商解决方案
Django 1.3.1和1.2.7已经修复此漏洞，建议用户下载使用：
http://www.djangoproject.com/
@Sebug.net [ 2011-09-13 ]
- Django消息文件远程任意命令执行漏洞
SSV-ID: [1275](http://sebug.net/vuldb/ssvid-1275)
[排行](http://sebug.net/branking)[Wiki](http://ssv.sebug.net/N_list)[捐款](http://sebug.net/donate)[漏洞扫描](http://sebug.net/lto?url=http://www.scanv.com)[云存储](http://sebug.net/lto?url=http://www.upyun.com/)[VPS](http://sebug.net/lto?url=http://www.linode.com/?r=c4c36f1a2d3b525013a28fca90433f1c47ae0739)[Mac](http://mac.gd/)
SSV-AppDir: [Django漏洞](http://sebug.net/appdir/Django)
发布时间: 2007-01-24 (GMT+0800)
#### 受影响产品:
Django 0.95
#### 漏洞描述:
Django是Python编程语言驱动的一个开源模型-视图-控制器（MVC）风格的Web应用程序框架。
Django处理消息文件存在问题，远程攻击者可以利用漏洞以应用程序进程权限执行任意命令。
问题是处理文件时不正确过滤用户提交的输入就直接使用Python 'os.system()'函数调用，构建恶意.po文件，诱使用户访问处理，可导致以应用程序进程权限执行任意命令。
<* 参考
> [porkythepig@anspi.pl](mailto:porkythepig@anspi.pl)
[http://bugs.debian.org/cgi-bin/bugreport.cgi?bug=407519](http://bugs.debian.org/cgi-bin/bugreport.cgi?bug=407519)
[http://sebug.net/appdir/Django](http://sebug.net/appdir/Django)
*>
#### Sebug安全建议:
可参考如下的安全补丁程序：
[http://bugs.debian.org/cgi-bin/bugreport.cgi?bug=407519](http://bugs.debian.org/cgi-bin/bugreport.cgi?bug=407519)
@Sebug.net [ 2007-01-24 ]
