
# Shell curl 和 wget 使用代理IP - 阳光岛主 - CSDN博客

2017年08月10日 21:43:14[阳光岛主](https://me.csdn.net/sunboy_2050)阅读数：3910


Linux Shell 提供两个非常实用的命令来爬取网页，它们分别是 curl 和 wget
**curl 和 wget 使用代理**
curl 支持 http、https、socks4、socks5
wget 支持 http、https
**代理示例：**
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
|\#!/bin/bash
|\#
|\# curl 支持 http、https、socks4、socks5
|\# wget 支持 http、https
|\#
|\# 米扑代理示例：
|\# http://proxy.mimvp.com/demo2.php
|\#
|\# 米扑代理购买：
|\# http://proxy.mimvp.com
|\#
|\# mimvp.com
|\# 2015-11-09

|\# http代理格式         http_proxy=http://IP:Port
|\# https代理格式        https_proxy=http://IP:Port
|{|'http'|:|'http://120.77.176.179:8888'|}
|curl -m 30 --retry 3 -x http:|//120|.77.176.179:8888 http:|//proxy|.mimvp.com|/exist|.php|\# http_proxy
|wget -T 30 --tries 3 -e|"http_proxy=http://120.77.176.179:8888"|http:|//proxy|.mimvp.com|/exist|.php|\# http_proxy
|{|'https'|:|'http://46.105.214.133:3128'|}
|curl -m 30 --retry 3 --proxy-insecure -x http:|//46|.105.214.133:3128 -k https:|//proxy|.mimvp.com|/exist|.php|\# https_proxy
|wget -T 30 --tries 3 --no-check-certificate -e|"https_proxy=http://46.105.214.133:3128"|https:|//proxy|.mimvp.com|/exist|.php|\# https_proxy

|\# curl  支持socks
|{|'socks4'|:|'101.255.17.145:1080'|}
|curl -m 30 --retry 3 --socks4 101.255.17.145:1080 http:|//proxy|.mimvp.com|/exist|.php
|{|'socks5'|:|'82.164.233.227:45454'|}
|curl -m 30 --retry 3 --socks5 82.164.233.227:45454 http:|//proxy|.mimvp.com|/exist|.php

|\# wget 不支持socks
|

**wget 配置文件设置代理**
|1
|2
|3
|4
|5
|6
|7
|8
|vim ~/.wgetrc
|http_proxy=http:|//120|.77.176.179:8888:8080
|https_proxy=http:|//12|.7.17.17:8888:8080
|use_proxy = on
|wait = 30
|wget -T 30 --tries 3 http:|//proxy|.mimvp.com
|

**Shell 设置临时局部代理**
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
|\# proxy no auth
|export|http_proxy=http:|//120|.77.176.179:8888:8080
|export|https_proxy=http:|//12|.7.17.17:8888:8080
|\# proxy auth
|export|http_proxy=http:|//username|:password@120.77.176.179:8888:8080
|export|https_proxy=http:|//username|:password@12.7.17.17:8888:8080

|\# 取消设置
|unset|http_proxy
|unset|https_proxy
|

**Shell 设置系统全局代理**
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
|\# 修改 /etc/profile，保存并重启服务器
|sudo|vim|/etc/profile|\# 所有人有效
|或
|sudo|vim ~/.bashrc|\# 所有人有效
|或
|vim ~/.bash_profile|\# 个人有效

|\# proxy no auth
|export|http_proxy=http:|//120|.77.176.179:8888:8080
|export|https_proxy=http:|//12|.7.17.17:8888:8080
|\# proxy auth
|export|http_proxy=http:|//username|:password@120.77.176.179:8888:8080
|export|https_proxy=http:|//username|:password@12.7.17.17:8888:8080
|source|/etc/profile
|或
|source|~/.bashrc
|或
|source|~/.bash_profile

|sudo|reboot
|

**米扑代理示例**
米扑代理示例，包含Python、Java、PHP、C\#、Go、Perl、Ruby、Shell、NodeJS、PhantomJS、Groovy、Delphi、易语言等十多种编程语言或脚本，通过大量的可运行实例，详细讲解了使用代理IP的正确方法，方便网页爬取、数据采集、自动化测试等领域。
![shell-curl-he-wget-shi-yong-dai-li-ip-01](http://blog.mimvp.com/wp-content/uploads/2017/08/shell-curl-he-wget-shi-yong-dai-li-ip-01.png)
米扑代理示例，测试使用的代理IP，全部来自于米扑代理，其覆盖120多个国家，中国34个省市，支持http、https、socks4、socks5等
米扑代理示例官网：
[http://proxy.mimvp.com/demo2.php](http://proxy.mimvp.com/demo2.php#demo-main-div)

**参考推荐**：
**米扑代理之使用示例**（推荐）
**[mimvp-proxy-demo](https://github.com/mimvp/mimvp-proxy-demo)**（GitHub）
[Linux 抓取网页实例](http://blog.mimvp.com/2012/06/linux-zhua-qu-wang-ye-shi-li-shell-awk/)（shell+awk）
[LinuxIP代理筛选系统](http://blog.mimvp.com/2012/06/linuxip-dai-li-shai-xuan-xi-tong-shell-proxy/)（shell+proxy）


