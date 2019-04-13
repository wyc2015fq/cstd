
# Go语言下载、安装、配置、使用 - 阳光岛主 - CSDN博客

2017年09月25日 22:15:54[阳光岛主](https://me.csdn.net/sunboy_2050)阅读数：1610个人分类：[Script																](https://blog.csdn.net/sunboy_2050/article/category/694817)



**Go语言**
Go语言（Golang，The Go Programming Language），是谷歌2009发布的第二款开源编程语言。2009年7月份，谷歌曾发布了[Simple语言](https://baike.baidu.com/item/Simple%E8%AF%AD%E8%A8%80)，它是用来开发Android应用。
Go语言是谷歌推出的一种全新的编程语言，可以在不损失应用程序性能的情况下降低代码的复杂性。
谷歌首席软件工程师罗布派克(Rob Pike)说：我们之所以开发Go，是因为过去10多年间软件开发的难度令人沮丧。
Go语言专门针对多处理器系统应用程序的编程进行了优化，使用Go编译的程序可以媲美C或C++代码的速度，而且更加安全、支持并行进程。
**Go语言官网**：[https://golang.org](https://golang.org/)（需要VPN访问）
**1. 下载 Go**
Go语言是跨平台，支持Windows、Linux、Mac OS X等系统，还提供有源码，可编译安装
Windows ：[go1.8.3.windows-amd64.msi (78MB)](https://storage.googleapis.com/golang/go1.8.3.windows-amd64.msi)
Linux ：[go1.8.3.linux-amd64.tar.gz (86MB)](https://storage.googleapis.com/golang/go1.8.3.linux-amd64.tar.gz)
Mac ：[go1.8.3.darwin-amd64.tar.gz (85MB)](https://storage.googleapis.com/golang/go1.8.3.darwin-amd64.tar.gz)（推荐）
Source：[go1.8.3.src.tar.gz (15MB)](https://storage.googleapis.com/golang/go1.8.3.src.tar.gz)
![go-yu-yan-xia-zai-an-zhuang-pei-zhi-shi-yong-01](http://blog.mimvp.com/wp-content/uploads/2017/09/go-yu-yan-xia-zai-an-zhuang-pei-zhi-shi-yong-01.png)
**2. 安装 Go**
sudo tar -C /usr/local/ -zxvf go1.8.3.darwin-amd64.tar.gz
注意：必须安装在 /usr/local/ 目录下，因为GOROOT必须为/usr/local/go
安装目录说明：
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
|$ ll|/usr/local/go/
|total 232
|-rw-r--r--    1 homer  staff  33243 May 25 04:14 AUTHORS
|-rw-r--r--    1 homer  staff   1366 May 25 04:14 CONTRIBUTING.md
|-rw-r--r--    1 homer  staff  45710 May 25 04:14 CONTRIBUTORS
|-rw-r--r--    1 homer  staff   1479 May 25 04:14 LICENSE
|-rw-r--r--    1 homer  staff   1303 May 25 04:14 PATENTS
|-rw-r--r--    1 homer  staff   1399 May 25 04:14 README.md
|-rw-r--r--    1 homer  staff      7 May 25 04:14 VERSION
|drwxr-xr-x   14 homer  staff    476 May 25 04:14 api
|drwxr-xr-x    5 homer  staff    170 May 25 04:16 bin
|drwxr-xr-x    4 homer  staff    136 May 25 04:16 blog
|drwxr-xr-x   44 homer  staff   1496 May 25 04:14 doc
|-rw-r--r--    1 homer  staff   5686 May 25 04:14 favicon.ico
|drwxr-xr-x    3 homer  staff    102 May 25 04:14 lib
|drwxr-xr-x   16 homer  staff    544 May 25 04:16 misc
|drwxr-xr-x    7 homer  staff    238 May 25 04:16 pkg
|-rw-r--r--    1 homer  staff     26 May 25 04:14 robots.txt
|drwxr-xr-x   68 homer  staff   2312 May 25 04:14 src
|drwxr-xr-x  269 homer  staff   9146 Jul 25 01:29|test
|

目录文件解释：
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
|api   — 目录，包含所有API列表，方便IDE使用
|bin   — 目录，存放编译后的可执行文件
|blog  — 目录，
|doc   — 目录，帮助文档
|lib   — 目录，
|misc  — 目录，
|pkg   — 目录，存放编译后的包文件。pkg中的文件是Go编译生成的
|src   — 目录，存放项目源文件
|test|- 目录，示例代码
|注：一般，bin和pkg目录可以不创建，go命令会自动创建（如 go|install|），只需要创建src目录即可。
|AUTHORS         — 文件，作者列表，用记事本打开
|CONTRIBUTING.md — 文件，
|CONTRIBUTORS    — 文件，
|LICENSE         — 文件，license，用记事本打开
|PATENTS         — 文件，
|README.md       — 文件，
|VERSION         — 文件，版本信息，用记事本打开
|favicon.ico     — 文件，
|robots.txt      — 文件，使用robots.txt阻止对网址的访问，
|详情查看https:|//support|.google.com|/webmasters/answer/6062608|?hl=zh-Hans
|

Eclipse 安装Go插件，在市场里搜索 "go"，如下图，安装 GoClipse
![go-yu-yan-xia-zai-an-zhuang-pei-zhi-shi-yong-02](http://blog.mimvp.com/wp-content/uploads/2017/09/go-yu-yan-xia-zai-an-zhuang-pei-zhi-shi-yong-02.png)
**注意：**GoClipse插件开发的程序，需要重启电脑后才会生效，因为 /etc/profile 配置的环境变量重启电脑后才生效
**3. 配置 Go**
vim /etc/profile
添加配置：
GOROOT=/usr/local/go
export PATH=$GOROOT/bin:$PATH
使配置生效：
source /etc/profile
验证安装成功：
$ go version
go version go1.8.3 darwin/amd64
**4. 使用 Go**
**基本语法**
Go 语言中 = 和 := 有什么区别
= 是赋值， := 是声明变量并赋值。
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
|// = 使用必须使用先var声明例如：
|var|a
|a=100
|// 或
|var|b = 100
|// 或
|var|c int = 100
|// := 是声明并赋值，并且系统自动推断类型，不需要var关键字
|d := 100
|

**1） hello 示例**
vim hello.go
|1
|2
|3
|4
|5
|6
|7
|package|main
|import|"fmt"
|func main() {
|fmt.Printf(|"hello mimvp.com"|)
|}
|

编译执行：
go build
./hello
运行结果：
$./hello
hello mimvp.com
**2）爬取网页示例**
vim spider_web.go
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
|/**
|* mimvp.com
|* 2017.4.12
|*
|* spider web of mimvp.com
|*/
|package main
|import (
|"fmt"
|"io/ioutil"
|"net/http"
|)
|func main() {
|response, _ := http.Get(|"http://mimvp.com"|)
|defer response.Body.Close()
|body, _ := ioutil.ReadAll(response.Body)
|fmt.Println(string(body))
|fmt.Printf(|"len : %d\n"|, len(string(body)));
|}
|

编译执行：
$ go build spider_web.go
$ ./spider_web
<!DOCTYPE html>
<html lang="zh-CN">
<head>
<meta charset="utf-8">
...
</body>
</html>
len : 15661
**5. 代理示例**
Go语言支持的代理为 http 和 socks5，不支持 https 和 socks4
其中，Go使用socks5代理时，需要安装引用 "golang.org/x/net/proxy"，安装配置步骤如下：
1. download[https://github.com/golang/net](https://github.com/golang/net)
2. unzip and move to /usr/local/go/src/golang.org/x/net/
3. import ( "golang.org/x/net/proxy" )
Go 语言代理示例如下（全部都经过测试，成功）：
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
|50
|51
|52
|53
|54
|55
|56
|57
|58
|59
|60
|61
|62
|63
|64
|65
|66
|67
|68
|69
|70
|71
|72
|73
|74
|75
|76
|77
|78
|79
|80
|81
|82
|83
|84
|85
|86
|87
|88
|89
|90
|91
|92
|93
|94
|95
|96
|97
|98
|99
|100
|101
|102
|103
|104
|105
|106
|107
|108
|109
|110
|111
|112
|113
|114
|115
|116
|117
|118
|119
|120
|121
|122
|123
|124
|125
|126
|127
|128
|129
|130
|131
|132
|133
|134
|135
|136
|137
|138
|139
|140
|141
|142
|143
|144
|145
|146
|147
|148
|149
|150
|151
|152
|153
|154
|155
|156
|157
|158
|159
|160
|161
|162
|163
|/**
|* mimvp.com
|* 2017.7.20
|* golang use http, socks5 proxy
|*/
|package main
|import (
|"fmt"
|"io/ioutil"
|"log"
|"net/http"
|"net/url"
|"os"
|"golang.org/x/net/proxy"
|)
|// success http
|// proxy_uri := "http://125.77.25.124:80"
|// mimvp_url := "http://mimvp.com"
|func main_1(proxy_uri string, mimvp_url string) {
|proxy := func(_ *http.Request) (*url.URL, error) {
|return|url.Parse(proxy_uri)
|}
|transport := &http.Transport{Proxy: proxy}
|client := &http.Client{Transport: transport}
|resp, err := client.Get(mimvp_url)
|if|err != nil {
|fmt.Println(|"error : "|, err)
|return
|}|else|{
|defer resp.Body.Close()
|body, _ := ioutil.ReadAll(resp.Body)
|fmt.Printf(|"%s\n"|, body)
|}
|}
|// success http
|// proxy_uri := "http://125.77.25.124:80"
|// mimvp_url := "http://mimvp.com"
|func main_2(proxy_uri string, mimvp_url string) {
|url_i := url.URL{}
|url_proxy, _ := url_i.Parse(proxy_uri)
|transport := &http.Transport{Proxy: http.ProxyURL(url_proxy)}
|client := http.Client{Transport: transport}
|resp, err := client.Get(mimvp_url)
|if|err != nil {
|log.Fatalln(err)
|}|else|{
|defer resp.Body.Close()
|body, _ := ioutil.ReadAll(resp.Body)
|fmt.Printf(|"%s\n"|, body)
|}
|}
|// config environment varable
|func main_22(proxy_uri string, mimvp_url string) {
|// url_i := url.URL{}
|// url_proxy, _ := url_i.Parse("https://127.0.0.1:9743")
|os.Setenv(|"HTTP_PROXY"|,|"http://125.77.25.124:80"|)
|os.Setenv(|"HTTPS_PROXY"|,|"https://210.209.89.100:8081"|)
|c := http.Client{|/* Transport: &http.Transport{ // Proxy: http.ProxyURL(url_proxy)} */|}
|resp, err := c.Get(|"http://mimvp.com"|)
|if|err != nil {
|log.Fatalln(err)
|}|else|{
|defer resp.Body.Close()
|body, _ := ioutil.ReadAll(resp.Body)
|fmt.Printf(|"%s\n"|, body)
|}
|}
|// 指定代理ip
|func getTransportFieldURL(proxy_addr *string) (transport *http.Transport) {
|url_i := url.URL{}
|url_proxy, _ := url_i.Parse(*proxy_addr)
|transport = &http.Transport{Proxy: http.ProxyURL(url_proxy)}
|return
|}
|// 从环境变量$http_proxy或$HTTP_PROXY中获取HTTP代理地址
|// Linux 设置代理环境变量命令：
|// http_proxy=http://125.77.25.124:80
|// https_proxy=https://210.209.89.100:8081
|func getTransportFromEnvironment() (transport *http.Transport) {
|transport = &http.Transport{Proxy: http.ProxyFromEnvironment}
|return
|}
|func fetch(mimvp_url, proxy_uri *string) (html string) {
|transport := getTransportFieldURL(proxy_uri)
|client := &http.Client{Transport: transport}
|req, err := http.NewRequest(|"GET"|, *mimvp_url, nil)
|if|err != nil {
|log.Fatal(err.Error())
|}
|resp, err := client.Do(req)
|if|err != nil {
|log.Fatal(err.Error())
|}
|if|resp.StatusCode == 200 {
|robots, err := ioutil.ReadAll(resp.Body)
|resp.Body.Close()
|if|err != nil {
|log.Fatal(err.Error())
|}
|html = string(robots)
|}|else|{
|html =|""
|}
|return
|}
|// success http
|// proxy_uri := "http://125.77.25.124:80"
|// mimvp_url := "http://mimvp.com"
|func main_3(proxy_uri string, mimvp_url string) {
|html := fetch(&mimvp_url, &proxy_uri)
|fmt.Println(html)
|}
|// success socks5
|// 1. download https://github.com/golang/net
|// 2. unzip and move to /usr/local/go/src/golang.org/x/net/
|// 3. import ( "golang.org/x/net/proxy" )
|func main_socks5(proxy_socks string, mimvp_url string) {
|// create a socks5 dialer
|dialer, err := proxy.SOCKS5(|"tcp"|, proxy_socks, nil, proxy.Direct)
|if|err != nil {
|fmt.Println(os.Stderr,|"can't connect to socks5 proxy:"|, err)
|os.Exit(1)
|}
|// setup a http client
|//    transport := &http.Transport{}
|//    transport.Dial = dialer.Dial
|transport := &http.Transport{Dial: dialer.Dial}
|client := &http.Client{Transport: transport}
|resp, err := client.Get(mimvp_url)
|if|err != nil {
|log.Fatalln(err)
|}|else|{
|defer resp.Body.Close()
|body, _ := ioutil.ReadAll(resp.Body)
|fmt.Printf(|"%s\n"|, body)
|}
|}
|func main() {
|// http proxy
|proxy_uri :=|"http://125.77.25.124:80"
|mimvp_url :=|"http://mimvp.com"
|main_1(proxy_uri, mimvp_url)
|main_2(proxy_uri, mimvp_url)
|main_3(proxy_uri, mimvp_url)
|// socks5 proxy
|proxy_socks :=|"175.138.65.244:1080"
|main_socks5(proxy_socks, mimvp_url)
|}
|

本示例中的代理，全部来自米扑代理
米扑代理，提供专业高品质代理，支持http、https、socks4、socks5等协议，
覆盖全球120多个国家，中国34个省市，代理质量和可用性非常高
米扑代理：[http://proxy.mimvp.com](http://proxy.mimvp.com/)


