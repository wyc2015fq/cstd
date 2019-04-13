
# Python + Selenium + Chrome 使用代理 auth 的用户名密码授权 - 阳光岛主 - CSDN博客

2018年07月18日 05:52:28[阳光岛主](https://me.csdn.net/sunboy_2050)阅读数：3120


米扑代理，全球领导的代理品牌，专注代理行业近十年，提供开放、私密、独享代理，并可免费试用
米扑代理官网：[https://proxy.mimvp.com](https://proxy.mimvp.com/)
本文示例，是结合米扑代理的私密、独享、开放代理，专门研发的示例，
支持 http、https的无密码、白名单ip、密码授权三种类型
示例中，用的插件 xpi 请到[米扑代理官网](https://proxy.mimvp.com/demo2.php)，或米扑官方[github](https://github.com/mimvp/mimvp-proxy-demo)下载
本文，直接给出完整的代码，都经过严格验证通过，具体请见注释
**本文示例的运行环境：**
MacBook Pro  MacOS High Sierra Version 10.13.4
Google Chrome  Version 63.0.3239.84 (Official Build) (64-bit)
Python 2.7.13 (v2.7.13:a06454b1afa1, Dec 17 2016, 12:39:47)
$ pip list | grep selenium
selenium (3.4.2)
chromedriver 下载地址：[http://chromedriver.storage.googleapis.com/index.html](http://chromedriver.storage.googleapis.com/index.html)
**Python + Selenium + Chrome**
出错提示：WebDriverException: 'chromedriver' executable needs to be in PATH
解决方法：
a. 下载[ChromeDriver](https://sites.google.com/a/chromium.org/chromedriver/)，其它浏览器参见[官网说明](http://www.seleniumhq.org/about/platforms.jsp#browsers)
b. 复制 chromedrive 文件到 Google Chrome 程序目录下，或复制到环境变量下
cp chromedrive /usr/local/bin/
各操作系统里的位置路径可以参考[官方Wiki](https://github.com/SeleniumHQ/selenium/wiki/ChromeDriver)
Python 代码里创建 webdriver 对象时传递 chromedrive 路径
**示例1：MacOS + chrome 环境**
|1
|2
|3
|4
|5
|chromedriver|=|"/Applications/Google Chrome.app/Contents/MacOS/chromedriver"
|browser|=|webdriver.Chrome(executable_path|=|chromedriver)|\# 打开 Chrome 浏览器
|browser.get(url)
|content|=|browser.page_source
|print|(|"content: "|+|str|(content))
|
**示例2：MacOS + 环境变量**
|1
|2
|3
|4
|5
|6
|7
|8
|9
|10
|11
|12
|13
|14
|def|spider_url_chrome(url):
|browser|=|None
|display|=|None
|try|:
|display|=|Display(visible|=|0|, size|=|(|800|,|600|))
|display.start()
|chromedriver|=|'/usr/local/bin/chromedriver'
|browser|=|webdriver.Chrome(executable_path|=|chromedriver)|\# 打开 Chrome 浏览器
|browser.get(url)
|content|=|browser.page_source
|print|(|"content: "|+|str|(content))
|finally|:
|if|browser: browser.quit()
|if|display: display.stop()
|
**Selenium + chromedriver 代理使用，无密码或已设置白名单ip**
|1
|2
|3
|4
|5
|6
|7
|8
|9
|10
|11
|12
|13
|14
|15
|16
|17
|18
|19
|20
|21
|22
|23
|24
|25
|26
|27
|28
|29
|30
|31
|32
|33
|34
|\#\# webdriver + chrome + proxy + whiteip （无密码，或白名单ip授权）
|\#\# 米扑代理：https://proxy.mimvp.com
|def|spider_url_chrome_by_whiteip(url):
|browser|=|None
|display|=|None
|\#\# 白名单ip，请见米扑代理会员中心： https://proxy.mimvp.com/usercenter/userinfo.php?p=whiteip
|mimvp_proxy|=|{
|'ip'|:|'140.143.62.84'|,|\# ip
|'port_https'|:|62288|,|\# http, https
|'port_socks'|:|62287|,|\# socks5
|'username'|:|'mimvp-user'|,
|'password'|:|'mimvp-pass'
|}
|try|:
|display|=|Display(visible|=|0|, size|=|(|800|,|600|))
|display.start()
|chrome_options|=|Options()|\# ok
|chrome_options|=|webdriver.ChromeOptions()|\# ok
|proxy_https_argument|=|'--proxy-server=http://{ip}:{port}'|.|format|(ip|=|mimvp_proxy[|'ip'|], port|=|mimvp_proxy[|'port_https'|])|\# http, https (无密码，或白名单ip授权，成功)
|chrome_options.add_argument(proxy_https_argument)
|\#         proxy_socks_argument = '--proxy-server=socks5://{ip}:{port}'.format(ip=mimvp_proxy['ip'], port=mimvp_proxy['port_socks'])   \# socks5 (无密码，或白名单ip授权，失败)
|\#         chrome_options.add_argument(proxy_socks_argument)
|chromedriver|=|'/usr/local/bin/chromedriver'
|browser|=|webdriver.Chrome(executable_path|=|chromedriver, chrome_options|=|chrome_options)|\# 打开 Chrome 浏览器
|browser.get(url)
|content|=|browser.page_source
|print|(|"content: "|+|str|(content))
|finally|:
|if|browser: browser.quit()
|if|display: display.stop()
|

**Selenium + chromedriver 代理使用，支持http、https账号密码**
本示例，采用了米扑代理的用户名密码授权
获取户名密码授权，请到米扑代理 - 会员中心 -[白名单ip](https://proxy.mimvp.com/usercenter/userinfo.php?p=whiteip)
**1、创建一个zip包，包含以下两个文件 background.js 和 manifest.json，打包成 proxy.zip**
**1）background.js**
|1
|2
|3
|4
|5
|6
|7
|8
|9
|10
|11
|12
|13
|14
|15
|16
|17
|18
|19
|20
|21
|22
|23
|24
|25
|26
|27
|28
|var|config = {
|mode:|"fixed_servers"|,
|rules: {
|singleProxy: {
|scheme:|"http"|,
|host:|"140.143.62.84"|,
|port: 19480
|},
|bypassList: [|"mimvp.com"|]
|}
|};
|chrome.proxy.settings.set({value: config, scope:|"regular"|},|function|() {});
|function|callbackFn(details) {
|return|{
|authCredentials: {
|username:|"mimvp-user"|,
|password:|"mimvp-pass"
|}
|};
|}
|chrome.webRequest.onAuthRequired.addListener(
|callbackFn,
|{urls: [|"<all_urls>"|]},
|[|'blocking'|]
|);</all_urls>
|
注意：上面配置中，需要把代理ip、port、username、password 替换成米扑代理的ip:port、授权用户名和密码
**2）manifest.json**
|1
|2
|3
|4
|5
|6
|7
|8
|9
|10
|11
|12
|13
|14
|15
|16
|17
|18
|{
|"version"|:|"1.0.0"|,
|"manifest_version"|: 2,
|"name"|:|"Chrome Proxy"|,
|"permissions"|: [
|"proxy"|,
|"tabs"|,
|"unlimitedStorage"|,
|"storage"|,
|"<all_urls>"|,
|"webRequest"|,
|"webRequestBlocking"
|],
|"background"|: {
|"scripts"|: [|"background.js"|]
|},
|"minimum_chrome_version"|:|"22.0.0"
|}
|
说明：上面配置，不需要改动，直接拷贝使用即可
**2、添加 proxy.zip 到 chrome 中作为插件**
|1
|2
|3
|4
|5
|6
|7
|8
|9
|10
|11
|12
|13
|14
|15
|16
|17
|18
|19
|20
|21
|22
|23
|24
|25
|26
|27
|28
|29
|30
|31
|32
|33
|34
|35
|36
|37
|\#!/usr/bin/env python
|\# -*- coding:utf-8 -*-
|from|selenium|import|webdriver
|from|selenium.webdriver.common.proxy|import|*
|from|selenium.webdriver.chrome.options|import|Options
|from|pyvirtualdisplay|import|Display
|\# from xvfbwrapper import Xvfb

|def|spider_url_chrome_by_https(url):
|browser|=|None
|display|=|None
|try|:
|display|=|Display(visible|=|0|, size|=|(|800|,|600|))
|display.start()
|chrome_options|=|Options()
|chrome_options.add_extension(|"proxy.zip"|)
|chromedriver|=|'/usr/local/bin/chromedriver'
|browser|=|webdriver.Chrome(executable_path|=|chromedriver, chrome_options|=|chrome_options)|\# 打开 Chrome 浏览器
|browser.get(url)
|content|=|browser.page_source
|print|(|"content: "|+|str|(content))
|finally|:
|if|browser: browser.quit()
|if|display: display.stop()

|if|__name__|=|=|'__main__'|:
|url|=|'https://ip.cn'
|url|=|'https://mimvp.com/'
|url|=|'https://proxy.mimvp.com/ip.php'
|\# http, https 密码授权，成功
|spider_url_chrome_by_https(url)
|
**3、运行效果，验证成功**
content: <html xmlns="http://www.w3.org/1999/xhtml"><head></head><body>140.143.62.84</body></html>

**Selenium + Chrome Diver使用用户名密码认证的HTTP代理的方法 （****升级版****）**
默认情况下，Chrome的--proxy-server="http://ip:port"参数不支持设置用户名和密码认证。
这样就使得"Selenium + Chrome Driver"无法使用HTTP Basic Authentication的HTTP代理。
一种变通的方式就是采用IP地址认证，[米扑代理](https://proxy.mimvp.com/)提供白名单ip授权，即属于IP地址认证，详见[米扑代理](https://proxy.mimvp.com/)-[会员中心](http://proxy.mimvp.com/usercenter/userinfo.php)-[白名单ip](https://proxy.mimvp.com/usercenter/userinfo.php?p=whiteip)
但在国内网络环境下，大多数用户都采用ADSL形式网络接入，IP是变化的（ISP动态切换），因此无法采用IP地址绑定认证。
因此，迫切需要找到一种让Chrome自动实现HTTP代理用户名密码认证的方案。
Stackoverflow上有人分享了一种利用 Chrome插件 实现自动代理用户密码认证的方案非常不错，
详细地址：[how-to-override-basic-authentication-in-selenium2-with-java-using-chrome-driver](https://stackoverflow.com/questions/9888323/how-to-override-basic-authentication-in-selenium2-with-java-using-chrome-driver)
[米扑代理](https://proxy.mimvp.com/)的研发工程师，在该思路的基础上用Python实现了自动化的Chrome插件创建过程，
即根据指定的代理“username:password@ip:port”实现了自动创建一个Chrome代理插件，
然后就可以在"Selenium + Chrome Driver"中通过安装该插件实现代理配置功能，
具体代码如下:
**1、创建模板文件夹 Chrome-proxy-helper**
![](https://blog.mimvp.com/wp-content/uploads/2018/07/python-selenium-chrome-shi-yong-dai-li-auth-de-yong-hu-ming-mi-ma-shou-quan-01.png)
如上图结构，依次创建：
1）创建模板文件夹
**Chrome-proxy-helper**
2）创建**background.js**
vim Chrome-proxy-helper/background.js
|1
|2
|3
|4
|5
|6
|7
|8
|9
|10
|11
|12
|13
|14
|15
|16
|17
|18
|19
|20
|21
|22
|23
|24
|25
|26
|27
|28
|var|config = {
|mode:|"fixed_servers"|,
|rules: {
|singleProxy: {
|scheme:|"http"|,
|host:|"mimvp_proxy_host"|,
|port: parseInt(mimvp_proxy_port)
|},
|bypassList: [|"mimvp.com"|]
|}
|};
|chrome.proxy.settings.set({value: config, scope:|"regular"|},|function|() {});
|function|callbackFn(details) {
|return|{
|authCredentials: {
|username:|"mimvp_username"|,
|password:|"mimvp_password"
|}
|};
|}
|chrome.webRequest.onAuthRequired.addListener(
|callbackFn,
|{urls: [|"<all_urls>"|]},
|[|'blocking'|]
|);</all_urls>
|
3）创建**manifest.json**
vim Chrome-proxy-helper/manifest.json
|1
|2
|3
|4
|5
|6
|7
|8
|9
|10
|11
|12
|13
|14
|15
|16
|17
|18
|{
|"version"|:|"1.0.0"|,
|"manifest_version"|: 2,
|"name"|:|"Chrome Proxy"|,
|"permissions"|: [
|"proxy"|,
|"tabs"|,
|"unlimitedStorage"|,
|"storage"|,
|"<all_urls>"|,
|"webRequest"|,
|"webRequestBlocking"
|],
|"background"|: {
|"scripts"|: [|"background.js"|]
|},
|"minimum_chrome_version"|:|"22.0.0"
|}
|
**2、创建zip打包的函数**
在 python 脚本里，创建zip打包的函数
|1
|2
|3
|4
|5
|6
|7
|8
|9
|10
|11
|12
|13
|14
|15
|16
|17
|18
|19
|20
|21
|22
|23
|24
|25
|26
|27
|28
|29
|30
|31
|32
|33
|34
|35
|36
|37
|38
|39
|40
|41
|42
|import|os, re, time, zipfile
|from|selenium|import|webdriver
|def|get_chrome_proxy_extension(proxy):
|"""获取一个Chrome代理扩展,里面配置有指定的代理(带用户名密码认证)
|proxy - 指定的代理,格式: username:password@ip:port
|"""
|\# Chrome代理插件的参考模板 https://github.com/RobinDev/Selenium-Chrome-HTTP-Private-Proxy
|CHROME_PROXY_HELPER_DIR|=|'Chrome-proxy-helper'|\# 自定义目录名，放在代理项目的当前同一级目录
|\# 存储自定义Chrome代理扩展文件的目录，一般为当前同一级目录
|\# 生成的zip路径为：chrome-proxy-extensions/mimvp-user_mimvp-pass@140.143.62.84_19480.zip
|CUSTOM_CHROME_PROXY_EXTENSIONS_DIR|=|'chrome-proxy-extensions'
|m|=|re.|compile|(|'([^:]+):([^\@]+)\@([\d\.]+):(\d+)'|).search(proxy)
|if|m:
|\# 提取代理的各项参数
|username|=|m.groups()[|0|]
|password|=|m.groups()[|1|]
|ip|=|m.groups()[|2|]
|port|=|m.groups()[|3|]
|\# 创建一个定制Chrome代理扩展(zip文件)
|if|not|os.path.exists(CUSTOM_CHROME_PROXY_EXTENSIONS_DIR):
|os.mkdir(CUSTOM_CHROME_PROXY_EXTENSIONS_DIR)
|extension_file_path|=|os.path.join(CUSTOM_CHROME_PROXY_EXTENSIONS_DIR,|'{}.zip'|.|format|(proxy.replace(|':'|,|'_'|)))
|\# 扩展文件不存在，创建
|if|not|os.path.exists(extension_file_path):
|zf|=|zipfile.ZipFile(extension_file_path, mode|=|'w'|)
|zf.write(os.path.join(CHROME_PROXY_HELPER_DIR,|'manifest.json'|),|'manifest.json'|)
|\# 替换模板中的代理参数
|background_content|=|open|(os.path.join(CHROME_PROXY_HELPER_DIR,|'background.js'|)).read()
|background_content|=|background_content.replace(|'mimvp_proxy_host'|, ip)
|background_content|=|background_content.replace(|'mimvp_proxy_port'|, port)
|background_content|=|background_content.replace(|'mimvp_username'|, username)
|background_content|=|background_content.replace(|'mimvp_password'|, password)
|zf.writestr(|'background.js'|, background_content)
|zf.close()
|return|extension_file_path
|else|:
|raise|Exception(|'Invalid proxy format. Should be username:password@ip:port'|)
|
**3、编写 Python 脚本的使用代理函数**
|1
|2
|3
|4
|5
|6
|7
|8
|9
|10
|11
|12
|13
|14
|15
|16
|17
|18
|19
|20
|21
|22
|23
|24
|25
|26
|27
|28
|29
|30
|31
|32
|\#\# webdriver + chrome + proxy + https （https密码授权，自动打包zip）
|\#\# 米扑代理：https://proxy.mimvp.com
|def|spider_url_chrome_by_https2(url):
|browser|=|None
|display|=|None
|try|:
|display|=|Display(visible|=|0|, size|=|(|800|,|600|))
|display.start()
|proxy|=|'mimvp-guest:welcome2mimvp@140.143.62.84:19480'
|chrome_options|=|Options()
|chrome_options|=|webdriver.ChromeOptions()
|chrome_options.add_extension(get_chrome_proxy_extension(proxy))
|chromedriver|=|'/usr/local/bin/chromedriver'
|browser|=|webdriver.Chrome(executable_path|=|chromedriver, chrome_options|=|chrome_options)|\# 打开 Chrome 浏览器
|browser.get(url)
|content|=|browser.page_source
|print|(|"content: "|+|str|(content))
|finally|:
|if|browser: browser.quit()
|if|display: display.stop()

|if|__name__|=|=|'__main__'|:
|url|=|'https://ip.cn'
|url|=|'https://mimvp.com/'
|url|=|'https://proxy.mimvp.com/ip.php'
|\# http, https 密码授权，成功
|spider_url_chrome_by_https2(url)
|
**4、运行结果，验证成功**
content: <html xmlns="http://www.w3.org/1999/xhtml"><head></head><body>140.143.62.84</body></html>
**5、小结**
通过模板，使用脚本自动创建zip文件，实现了自动动态调用代理，可以充分灵活运用米扑代理了

**Selenium + chromedriver 代理使用，不支持 socks5，米扑实测不成功**
|1
|2
|3
|4
|5
|6
|7
|8
|9
|10
|11
|12
|13
|14
|15
|16
|17
|18
|19
|20
|21
|22
|23
|24
|25
|26
|27
|28
|29
|30
|31
|32
|33
|34
|35
|36
|37
|38
|39
|40
|41
|42
|\#\# webdriver + chrome + proxy + socks （socks密码授权）
|\#\# 米扑代理：https://proxy.mimvp.com
|def|spider_url_chrome_by_socks(url):
|browser|=|None
|display|=|None
|\#\# 白名单ip，请见米扑代理会员中心： https://proxy.mimvp.com/usercenter/userinfo.php?p=whiteip
|mimvp_proxy|=|{
|'ip'|:|'140.143.62.84'|,|\# ip
|'port_https'|:|62288|,|\# http, https
|'port_socks'|:|62289|,|\# socks5
|'username'|:|'mimvp-user'|,
|'password'|:|'mimvp-pass'
|}
|try|:
|display|=|Display(visible|=|0|, size|=|(|800|,|600|))
|display.start()
|capabilities|=|dict|(DesiredCapabilities.CHROME)
|capabilities[|'proxy'|]|=|{
|'proxyType'|:|'MANUAL'|,
|\#                                     'httpProxy'    : mimvp_proxy['ip'] + ":" + str(mimvp_proxy['port_https']),
|\#                                     'sslProxy'     : mimvp_proxy['ip'] + ":" + str(mimvp_proxy['port_https']),
|'socksProxy'|: mimvp_proxy[|'ip'|]|+|":"|+|str|(mimvp_proxy[|'port_socks'|]),
|'ftpProxy'|: mimvp_proxy[|'ip'|]|+|":"|+|str|(mimvp_proxy[|'port_https'|]),
|'noProxy'|:|'localhost,127.0.0.1'|,
|'class'|:|"org.openqa.selenium.Proxy"|,
|'autodetect'|:|False
|}
|capabilities[|'proxy'|][|'socksUsername'|]|=|mimvp_proxy[|'username'|]
|capabilities[|'proxy'|][|'socksPassword'|]|=|mimvp_proxy[|'password'|]
|chromedriver|=|'/usr/local/bin/chromedriver'
|browser|=|webdriver.Chrome(chromedriver, desired_capabilities|=|capabilities)
|browser.get(url)
|content|=|browser.page_source
|print|(|"content: "|+|str|(content))
|finally|:
|if|browser: browser.quit()
|if|display: display.stop()
|

完整的代理示例，请见米扑代理的使用示例：
[https://proxy.mimvp.com/demo2.php](https://proxy.mimvp.com/demo2.php)（Selenium Python）
更多的代理示例，请见米扑代理的官方github：
[https://github.com/mimvp/mimvp-proxy-demo](https://github.com/mimvp/mimvp-proxy-demo)
本文中，测试的代理ip，全部来自米扑代理：
[https://proxy.mimvp.com](https://proxy.mimvp.com/)

**附加说明：**
**Chrome-proxy-helper 有官方版：**
**[https://github.com/sunboy-2050/Chrome-proxy-helper](https://github.com/sunboy-2050/Chrome-proxy-helper)**
**Introduction**
By default, Chrome use the system proxy setting (IE proxy settings on Windows platform ), but sometime we want to set proxy*ONLY*for chrome, not the whole system.
Chrome proxy helper extension use Chrome native proxy API to set proxy, support socks5, socks4, http and https protocol and pac script, Fast And Simple.
**Features**
support socks4, socks5, http, https proxy settings
support pac proxy settings
support bypass list
support online pac script
support customer proxy rules
support proxy authentication
support extension settings synchronize


