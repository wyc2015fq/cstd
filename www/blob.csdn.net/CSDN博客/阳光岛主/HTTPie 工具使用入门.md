
# HTTPie 工具使用入门 - 阳光岛主 - CSDN博客

2017年10月28日 00:48:28[阳光岛主](https://me.csdn.net/sunboy_2050)阅读数：1503个人分类：[Script																](https://blog.csdn.net/sunboy_2050/article/category/694817)



HTTPie 是一个 HTTP 的命令行客户端，目标是让 CLI 和 web 服务之间的交互尽可能的人性化。这个工具提供了简洁的 http 命令，允许通过自然的语法发送任意 HTTP 请求数据，展示色彩化的输出。HTTPie 可用于与 HTTP 服务器做测试、调试和常规交互。
HTTPie (pronounced*aitch-tee-tee-pie*) is a command line HTTP client. Its goal is to make CLI interaction with web services as human-friendly as possible. It provides a simple`http`command that allows for sending arbitrary HTTP requests using a simple and natural syntax, and displays colorized output. HTTPie can be used for testing, debugging, and generally interacting with HTTP servers.
博客原文，见[米扑博客](https://blog.mimvp.com/2017/10/httpie-gong-ju-shi-yong-ru-men/)，专注IT技术和量化金融
**主要特性：**
直观的语法
格式化和色彩化的终端输出
内置 JSON 支持
支持上传表单和文件
HTTPS、代理和认证
任意请求数据
自定义头部
持久性会话
类 Wget 下载
支持 Python 2.6, 2.7 和 3.x
支持 Linux, Mac OS X 和 Windows
插件
文档
测试覆盖率
HTTPie 是用 Python 编写，用到了[Requests](http://python-requests.org/)和[Pygments](http://pygments.org/)这些出色的库。
**Github**：[https://github.com/jakubroztocil/httpie](https://github.com/jakubroztocil/httpie)
**httpie 使用示例**
http -h mimvp.com
![httpie-gong-ju-shi-yong-ru-men-01](https://blog.mimvp.com/wp-content/uploads/2017/10/httpie-gong-ju-shi-yong-ru-men-01.png)
**1. 安装 httpie**
httpie 是跨平台命令，支持Mac OS X、 Linux、 Windows
1） Mac OS X
brew install httpie// brew 命令安装
port install httpie// ports 命令安装
2） Linux
easy_install httpie（CentOS 5.6/6.5/7.2 都成功）
\# Debian, Ubuntu, etc.
apt-get install httpie
\# Fedora, CentOS, RHEL, …
yum install httpie
\# Arch Linux
pacman -S httpie
3）Windows
\# Make sure we have an up-to-date version of pip and setuptools:
$ pip install --upgrade pip setuptools
$ pip install --upgrade httpie
**2. httpie 帮助**
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
|\# http
|usage: http [--json] [--form] [--pretty {all,colors,|format|,none}]
|[--style STYLE] [--print WHAT] [--headers] [--body] [--verbose]
|[--all] [--|history|-print WHAT] [--stream] [--output FILE]
|[--download] [--|continue|]
|[--session SESSION_NAME_OR_PATH | --session-|read|-only SESSION_NAME_OR_PATH]
|[--auth USER[:PASS]] [--auth-|type|{basic,digest}]
|[--proxy PROTOCOL:PROXY_URL] [--follow]
|[--max-redirects MAX_REDIRECTS] [--timeout SECONDS]
|[--check-status] [--verify VERIFY]
|[--ssl {ssl2.3,ssl3,tls1,tls1.1,tls1.2}] [--cert CERT]
|[--cert-key CERT_KEY] [--ignore-stdin] [--help] [--version]
|[--traceback] [--default-scheme DEFAULT_SCHEME] [--debug]
|[METHOD] URL [REQUEST_ITEM [REQUEST_ITEM ...]]
|http: error: the following arguments are required: URL
|

**3. httpie 命令**
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
|43
|44
|45
|46
|47
|48
|49
|\# 显示请求信息（包含返回头200）
|http mimvp.com
|\# 显示详细的请求（包含请求和返回头200）
|http|-|v mimvp.com
|\# 只显示Header
|http|-|h mimvp.com
|http|-|-|head mimvp.com
|http|-|-|header mimvp.com
|http|-|-|headers mimvp.com
|\# 只显示Body
|http|-|b mimvp.com
|http|-|-|body mimvp.com
|\# 下载文件
|http|-|d mimvp.com
|\# 模拟提交表单
|http|-|f POST mimvp.com username|=|'mimvp-user'
|\# 请求删除的方法
|http DELETE mimvp.com
|\# 传递JSON数据请求(默认就是JSON数据请求)
|http PUT mimvp.com username|=|'mimvp-user'|password|=|'mimvp-pwd'
|\# 如果JSON数据存在不是字符串则用:=分隔，例如
|http PUT mimvp.com username|=|'mimvp-user'|password|=|'mimvp-pwd'|age:|=|28|a:|=|true streets:|=|'["a", "b"]'
|\# 模拟Form的Post请求, Content-Type: application/x-www-form-urlencoded; charset=utf-8
|http|-|-|form POST mimvp.com username|=|'mimvp-user'
|\# 模拟Form的上传, Content-Type: multipart/form-data
|http|-|f POST example.com|/|jobs username|=|'mimvp-user'|file|@~|/|test.pdf
|\# 修改请求头, 使用:分隔
|http mimvp.com  User|-|Agent:mimvp|-|agent|/|1.0|'Cookie:a=b;b=c'|Referer:http:|/|/|mimvp.com|/
|\# 认证
|http|-|a username:password mimvp.com
|http|-|-|auth|-|type|=|digest|-|a username:password mimvp.com
|\# 使用http代理
|http|-|-|proxy|=|http:http:|/|/|217.107|.|197.174|:|8081|proxy.mimvp.com
|http|-|-|proxy|=|http:http:|/|/|user:|pass|@|217.107|.|197.174|:|8081|proxy.mimvp.com
|http|-|-|proxy|=|https:http:|/|/|112.114|.|96.34|:|8118|proxy.mimvp.com
|http|-|-|proxy|=|https:http:|/|/|user:|pass|@|112.114|.|96.34|:|8118|proxy.mimvp.com
|

**4. httpie 示例**
**1）请求 mimvp.com**（米扑科技）
请求：http mimvp.com
结果： 返回有状态头200和网页正文
![httpie-gong-ju-shi-yong-ru-men-02](https://blog.mimvp.com/wp-content/uploads/2017/10/httpie-gong-ju-shi-yong-ru-men-02.png)
**2）仅返回文件头header**
请求：http -h proxy.mimvp.com
结果：
![httpie-gong-ju-shi-yong-ru-men-03](https://blog.mimvp.com/wp-content/uploads/2017/10/httpie-gong-ju-shi-yong-ru-men-03.png)
**3）表单请求登录**
请求：http --form POST proxy.mimvp.com username='mimvp-user' password='mimvp-pwd'
**4）设置代理请求**
请求：
http --proxy=http:http://217.107.197.174:8081 proxy.mimvp.com
http --proxy=http:http://user:pass@217.107.197.174:8081 proxy.mimvp.com
http --proxy=https:http://112.114.96.34:8118 proxy.mimvp.com
http --proxy=https:http://user:pass@112.114.96.34:8118 proxy.mimvp.com
提取代理IP，请访问米扑代理：[http://proxy.mimvp.com](http://proxy.mimvp.com/)


