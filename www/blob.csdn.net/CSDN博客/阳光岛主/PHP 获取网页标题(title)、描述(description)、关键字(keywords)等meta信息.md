
# PHP 获取网页标题(title)、描述(description)、关键字(keywords)等meta信息 - 阳光岛主 - CSDN博客

2018年06月22日 00:55:10[阳光岛主](https://me.csdn.net/sunboy_2050)阅读数：5052


[米扑导航](https://site.mimvp.com)在收集网站、验证网站时，需要采集网页标题(title)、描述(description)、关键字(keywords)等meta信息
本文将其整理出来，总结的经验分享给大家，欢迎交流、斧正、改进。
本文原文转自米扑博客：[PHP 获取网页标题(title)、描述(description)、关键字(keywords)等meta信息](https://blog.mimvp.com/article/23089.html)
**1. PHP 内置函数 get_meta_tags**
|1
|2
|$meta_array|= get_meta_tags(|'https://mimvp.com'|);
|var_dump(|$meta_array|);
|

**运行结果：**
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
|array (size=21)
|'description'|=> string|'米扑科技，小而美、简而信，工匠艺术的互联网服务。'|(length=72)
|'keywords'|=> string|'米扑科技,米扑,mimvp.com,mimvp,米扑代理,米扑域名,米扑财富,米扑支付,米扑活动,米扑学堂,米扑博客,米扑论坛,小而美,简而信,简单可信赖'|(length=175)
|'author'|=> string|'米扑科技'|(length=12)
|'version'|=> string|'mimvp-home-1.2'|(length=14)
|'copyright'|=> string|'2009-2017 by mimvp.com'|(length=22)
|'mimvp-site-verification'|=> string|'I-love-mimvp.com-from-20160912'|(length=30)
|'baidu-site-verification'|=> string|'pzH9C12mmf'|(length=10)
|'sogou_site_verification'|=> string|'QCi6brPm84'|(length=10)
|'360-site-verification'|=> string|'d42818ef57d4f110b6c1fdf268c8cb07'|(length=32)
|'shenma-site-verification'|=> string|'f85fa0493059ca7e6b73ad5ae44751ec_1498128383'|(length=43)
|'google-site-verification'|=> string|'DSE-4k0kg0zlz8aGyKmZImOoTkpiIreULTsgMwNqJYE'|(length=43)
|'baiduspider'|=> string|'index,follow'|(length=12)
|'googlebot'|=> string|'index,follow'|(length=12)
|'bingbot'|=> string|'index,follow'|(length=12)
|'robots'|=> string|'index,follow'|(length=12)
|

get_meta_tags 默认是 utf-8 编码，如果遇到 gbk、gb2312编码，则会出现乱码，
例如，爬取www.qq.com首页，其编码为 gb2312 则出现了乱码：
|1
|2
|$meta_array|= get_meta_tags(|'http://www.qq.com'|);
|var_dump(|$meta_array|);
|

**运行结果：**
|1
|2
|3
|4
|array (size=3)
|'baidu-site-verification'|=> string|'cNitg6enc2'|(length=10)
|'keywords'|=> string|'��Ѷ,����,�ƾ�,����,��Ƶ,NBA,�Ƽ�,��Ѷ��,��Ѷ,QQ,Tencent'|(length=56)
|'description'|=> string|'��Ѷ����2003�괴�������Ѿ���Ϊ��������Ϣ������ֱ���������ữý����Ѷ�Ͳ�ƷΪһ��Ļ�����ý��ƽ̨����Ѷ���������š��Ƽ����ƾ������֡�������������ʱ�еȶ��Ƶ������������û��Բ�ͬ������Ѷ������ͬʱרע��ͬ�������ݣ����쾫Ʒ��Ŀ����˳Ӧ������չ���ƣ��Ƴ�����ֱ���ȴ�����ʽ���ı����û���ȡ��Ѷ�ķ�ʽ��ϰ�ߡ�'|(length=314)
|

解决乱码，方法有二种（其实，方法有很多，这里先介绍两种）
**方法1**
需加上 header 头编码，正确代码格式如下：
|1
|2
|3
|header(|"content-type:text/html; charset=gb2312"|);
|$meta_array|= get_meta_tags(|'http://www.qq.com'|);
|var_dump(|$meta_array|);
|

**方法2**
通用的编码解决方案：
|1
|2
|3
|header(|"content-type:text/html; charset=xxx"|);
|$meta_array|= get_meta_tags(|'http://www.qq.com'|);
|var_dump(|$meta_array|);
|

注意：charset=xxx 表示让浏览器自动适配正确的编码，不会造成乱码错误，header函数仅在PHP中使用
**小结：**
PHP 内置函数get_meta_tags 虽然可以获取meta信息，但是也有其缺点
1）可能会出现编码错误、中文乱码，如上 https://mimvp.com（UTF-8） 和 http://www.qq.com（GB2312）
2） meta 获取的信息不全面，例如米扑科技首页（https://mimvp.com）还有 <meta charset="utf-8"> 等meta标签没有包含显示出来

**2. 爬取网页，正则匹配**
爬取网页内容，然后进行正则匹配
**1）爬取网页内容**
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
|function|get_siteurl_curlinfo(|$url|=|'https://mimvp.com'|,|$timeout|=5,|$conntimeout|=3) {
|$ch|= curl_init();
|$url_host|=|explode|(|"/"|,|$url|)[2];
|$header|=|array|();
|array_push|(|$header|,|'User-Agent: Mozilla/5.0 (Macintosh; Intel Mac OS X 10_12_6) AppleWebKit/537.36 (KHTML, like Gecko) Chrome/62.0.3202.94 Safari/537.36'|);
|array_push|(|$header|,|'Referer:'|.|$url|);
|array_push|(|$header|,|'host:'|.|$url_host|);
|array_push|(|$header|,|'accept:  text/html,application/xhtml+xml,application/xml;q=0.9,image/webp,image/apng,*/*;q=0.8'|);
|array_push|(|$header|,|'upgrade-insecure-requests:1'|);
|curl_setopt(|$ch|, CURLOPT_HTTPHEADER,|$header|);
|curl_setopt(|$ch|, CURLOPT_AUTOREFERER, 1);
|curl_setopt(|$ch|, CURLOPT_FOLLOWLOCATION, 1);|// HTTP 头中的 "Location: "重定向
|curl_setopt(|$ch|, CURLOPT_RETURNTRANSFER, 1);|// 字符串返回
|curl_setopt(|$ch|, CURLOPT_SSL_VERIFYPEER, FALSE);|// https请求 不验证证书和hosts
|curl_setopt(|$ch|, CURLOPT_SSL_VERIFYHOST, FALSE);
|curl_setopt(|$ch|, CURLOPT_HEADER, 1);|// 0表示不输出Header，1表示输出
|curl_setopt(|$ch|, CURLOPT_NOBODY, 0);|// 0表示不输出Body，1表示输出
|curl_setopt(|$ch|, CURLOPT_CONNECTTIMEOUT,|$conntimeout|);|// 尝试连接时等待的秒数。设置为0，则无限等待
|curl_setopt(|$ch|, CURLOPT_TIMEOUT,|$timeout|+5);|// 允许 cURL 函数执行的最长秒数
|curl_setopt(|$ch|, CURLOPT_URL,|$url|);
|$output|= curl_exec(|$ch|);
|$curl_info|= curl_getinfo(|$ch|);
|curl_close(|$ch|);
|$curl_info|[|'page_info'|] = get_page_info(|$output|,|$curl_info|);
|return|$curl_info|;
|}
|

**2）正则匹配**
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
|function|get_page_info(|$output|) {
|$page_info|=|array|();
|$page_info|[|'site_title'|] =|''|;
|$page_info|[|'site_description'|] =|''|;
|$page_info|[|'site_keywords'|] =|''|;
|$page_info|[|'friend_link_status'|] = 0;
|$page_info|[|'site_claim_status'|] = 0;
|$page_info|[|'site_home_size'|] = 0;
|if|(|empty|(|$output|))|return|$page_info|;
|\# Keywords
|preg_match(|'/<META\s+name=["\']keywords[\'"]\s+content=["\']?([\w\W]*?)["\']/si'|,|$output|,|$matches|);
|if|(|empty|(|$matches|[1])) {
|preg_match(|'/<META\s+content=["\']([\w\W]*?)["\']\s+name=["\']keywords["\']/si'|,|$output|,|$matches|);
|}
|if|(|empty|(|$matches|[1])) {
|preg_match(|'/<META\s+http-equiv=["\']keywords["\']\s+content=["\']([\w\W]*?)["\']/si'|,|$output|,|$matches|);
|}
|if|(|empty|(|$matches|[1])) {
|preg_match(|'/<META\s+content=["\']keywords["\']\s+http-equiv=["\']([\w\W]*?)["\']/si'|,|$output|,|$matches|);
|}
|if|(!|empty|(|$matches|[1])) {
|$page_info|[|'site_keywords'|] =|$matches|[1];
|}
|\# Description
|preg_match(|'/<META\s+name=["\']description["\']\s+content=["\']([\w\W]*?)["\']/si'|,|$output|,|$matches|);
|if|(|empty|(|$matches|[1])) {
|preg_match(|'/<META\s+content=["\']([\w\W]*?)["\']\s+name=["\']description["\']/si'|,|$output|,|$matches|);
|}
|if|(|empty|(|$matches|[1])) {
|preg_match(|'/<META\s+http-equiv=["\']description["\']\s+content=["\']([\w\W]*?)["\']/si'|,|$output|,|$matches|);
|}
|if|(|empty|(|$matches|[1])) {
|preg_match(|'/<META\s+content=["\']description["\']\s+http-equiv=["\']([\w\W]*?)["\']/si'|,|$output|,|$matches|);
|}
|if|(!|empty|(|$matches|[1])) {
|$page_info|[|'site_description'|] =|$matches|[1];
|}
|\# Title
|preg_match(|'/<TITLE>([\w\W]*?)<\/TITLE>/si'|,|$output|,|$matches|);
|if|(!|empty|(|$matches|[1])) {
|$page_info|[|'site_title'|] =|$matches|[1];
|}
|\# mimvp-site-verification
|preg_match(|'/<META\s+name=["\']mimvp-site-verification["\']\s+content=["\']([\w\W]*?)["\']/si'|,|$output|,|$matches|);
|preg_match(|'/<META\s+name="mimvp-site-verification"\s+content="([\w\W]*?)"/si'|,|$output|,|$matches|);
|if|(|empty|(|$matches|[1])) {
|preg_match(|'/<META\s+content=["\']([\w\W]*?)["\']\s+name=["\']mimvp-site-verification["\']/si'|,|$output|,|$matches|);
|}
|if|(!|empty|(|$matches|[1])) {
|$page_info|[|'site_claim_status'|] =|$matches|[1];
|}
|\# mimvp-friend-link
|if|(|strstr|(|$output|,|'https://proxy.mimvp.com'|) !=|""|) {
|$page_info|[|'friend_link_status'|] = 1;
|}
|$page_info|[|'site_home_size'|] =|strlen|(|$output|);
|return|$page_info|;
|}
|

这里，正则匹配的代码在网上也可以找到，但米扑科技对其进行了优化
**优化1）**兼容单引号、双引号，兼容content前后顺序，兼容 name 和 http-equiv
**优化2）**不仅匹配keywords和description，还匹配了标题（title）、自定义网站归属认证（mimvp-site-verification）、友情链接检测（friend_link_status）
**优化3）**统计了正文网页大小，网页打开时间（上面代码没给出，可自己去实现，给出关键代码）
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
|//  获取毫秒的时间戳
|function|getMillisecond() {
|//  $microtime = microtime();
|//  $time = explode(" ", $microtime);           // 毫秒 + 秒   0.51026400 1436683760
|//  $time = $time[1] . ($time[0] * 1000);       // 拼接成毫秒，   1436683760510.264
|//  $time2 = explode(".", $time)[0];            // 取毫秒整数        1436683760510
|//  $time3 = $time2 / 1000;                     // 毫秒转成秒        1436683760.51
|//  $res = sprintf('%01.3f', $time3);           // 格式小数秒        1436683760.510
|$time|=|explode|(|" "|, microtime());
|return|$time|[1] +|$time|[0];
|}
|// 计算提取结果的耗时
|$cost_time_start|= getMillisecond();
|echo|"stat cost time"|;
|$cost_time|= getMillisecond() -|$cost_time_start|;
|$res|[|"cost_time"|] = sprintf(|'%01.6f'|,|$cost_time|);
|

**运行结果：**
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
|array (size=27)
|'url'|=> string|'https://mimvp.com/'|(length=18)
|'content_type'|=> string|'text/html; charset=UTF-8'|(length=24)
|'http_code'|=> int 200
|'header_size'|=> int 254
|'request_size'|=> int 320
|'filetime'|=> int -1
|'ssl_verify_result'|=> int 0
|'redirect_count'|=> int 0
|'total_time'|=> float 0.072495
|'namelookup_time'|=> float 0.015999
|'connect_time'|=> float 0.023155
|'pretransfer_time'|=> float 0.051711
|'size_upload'|=> float 0
|'size_download'|=> float 20824
|'speed_download'|=> float 287247
|'speed_upload'|=> float 0
|'download_content_length'|=> float -1
|'upload_content_length'|=> float -1
|'starttransfer_time'|=> float 0.072301
|'redirect_time'|=> float 0
|'redirect_url'|=> string|''|(length=0)
|'primary_ip'|=> string|'47.95.6.112'|(length=11)
|'certinfo'|=>
|array (size=0)
|empty
|'primary_port'|=> int 443
|'local_ip'|=> string|'192.168.31.191'|(length=14)
|'local_port'|=> int 62474
|'page_info'|=>
|array (size=6)
|'site_title'|=> string|'米扑科技 - 简单可信赖'|(length=30)
|'site_description'|=> string|'米扑科技，小而美、简而信，工匠艺术的互联网服务。'|(length=72)
|'site_keywords'|=> string|'米扑科技,米扑,mimvp.com,mimvp,米扑代理,米扑域名,米扑财富,米扑支付,米扑活动,米扑学堂,米扑博客,米扑论坛,小而美,简而信,简单可信赖'|(length=175)
|'friend_link_status'|=> int 0
|'site_claim_status'|=> string|'I-love-mimvp.com-from-20160912'|(length=30)
|'site_home_size'|=> int 21078
|

正则匹配，表面看似乎已经满足了需求，不仅匹配了keywords和description，还获得了title、自定义meta、特定友情链接判断等
实际上，这里的正则匹配仍然无法用于项目，因为其内藏的问题还有很多，例如：
|1
|2
|3
|4
|5
|6
|<?php
|header(|"content-type:text/html; charset=xxx"|);
|$url|=|"https://mimvp.com"|;     // 默认 utf-8
|$url|=|"http://www.qq.com"|;     // 默认 gb2312，需添加 header，否则乱码
|$url|=|"https://www.dajie.com"|; // content在前，name在后，匹配错误（从第一个content开始，从最后一个name结束）
|

网页编码、乱码问题，似乎可以用 header 函数来解决，但是 content在前，name在后，匹配错误的问题，却没法解决！
例如，匹配大街网的结果如下（部分）：
|1
|2
|3
|4
|5
|6
|7
|8
|9
|'page_info'|=>
|array (size=6)
|'site_title'|=> string|'求职找工作 - 人才招聘 - 大街网'|(length=42)
|'site_description'|=> string|'大街网是中国职业社交网站的缔造者，帮你找到更好的工作并工作的更好。24000000职业人士在这里创建立体化职业档案，打造职业形象，维系和拓展职业人脉，获取职业洞察。'|(length=230)
|'site_keywords'|=> string '大街网是中国职业社交网站的缔造者，帮你找到更好的工作并工作的更好。24000000职业人士在这里创建立体化职业档案，打造职业形象，维系和拓展职业人脉，获取职业洞察。|" name="|description"/>
|<meta content="大街网,找工作,招聘,求职,校园招聘,应届生求职,职业档案,找实习,职业形象,职业 档案,中国职业社交网站缔造者。' (length=422)
|'friend_link_status'|=> int 0
|'site_claim_status'|=> int 0
|'site_home_size'|=> int 171181
|

可见，site_keywords 中包含了 <meta content=" 内容，原因看大街网的网页源码
|1
|2
|3
|4
|<title>求职找工作 - 人才招聘 - 大街网<|/title|>
|<link rel=|"shortcut icon"|href=|"https://assets.dajieimg.com/images/favicon.ico"|/>
|<meta content=|"大街网是中国职业社交网站的缔造者，帮你找到更好的工作并工作的更好。24000000职业人士在这里创建立体化职业档案，打造职业形象，维系和拓展职业人脉，获取职业洞察。"|name=|"description"|/>
|<meta content=|"大街网,找工作,招聘,求职,校园招聘,应届生求职,职业档案,找实习,职业形象,职业 档案,中国职业社交网站缔造者。"|name=|"keywords"|/>
|

匹配出错的原因，即上文我已经在注释里说明的：content在前，name在后，匹配错误（从第一个content开始，从最后一个name结束）
这里，使用get_meta_tags('https://www.dajie.com');则不会出现上面的问题，但其无法获取标题title等meta信息
有没有更好的方法呢？既能获得正确的meta，又能获得title、自定义meta、特定字符串匹配（如友情链接）等
答案，当然是有的，但是需要自己写代码实现，且比正则匹配、get_meta_tags 的功能更加强大！

**3. 改进网页正则匹配**
前面的两个方法，并不能完全且完美的满足我们米扑科技的最基本的需求：
需求1） 正确的获取 keywords、description
需求2） 正确的获取 title、自定义meta、检测特定字符串（如友情链接）等
于是，自己动手写了
**原理思路：**
1）首先，正则匹配，获取全部的meta信息
2）然后，保存每一个meta的key和value到数组（实现了一个自己的get_meta_tags函数）
3）最后，提取 keywords和description
**优化的正则匹配代码**
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
|function|get_page_info(|$output|,|$curl_info|=|array|()) {
|$page_info|=|array|();
|$page_info|[|'site_title'|] =|''|;
|$page_info|[|'site_description'|] =|''|;
|$page_info|[|'site_keywords'|] =|''|;
|$page_info|[|'friend_link_status'|] = 0;
|$page_info|[|'site_claim_status'|] = 0;
|$page_info|[|'site_home_size'|] = 0;
|if|(|empty|(|$output|))|return|$page_info|;
|// 获取网页编码，把非utf-8网页编码转成utf-8，防止网页出现乱码
|$meta_content_type|=|''|;
|if|(isset(|$curl_info|[|'content_type'|]) &&|strstr|(|$curl_info|[|'content_type'|],|"charset="|) !=|""|) {
|$meta_content_type|=|explode|(|"charset="|,|$curl_info|[|'content_type'|])[1];
|}
|if|(|$meta_content_type|==|''|) {
|preg_match(|'/<META\s+http-equiv="Content-Type"\s+content="([\w\W]*?)"/si'|,|$output|,|$matches|);|// 中文编码，如 http://www.qq.com
|if|(|empty|(|$matches|[1])) {
|preg_match(|'/<META\s+content="([\w\W]*?)"\s+http-equiv="Content-Type"/si'|,|$output|,|$matches|);
|}
|if|(|empty|(|$matches|[1])) {
|preg_match(|'/<META\s+charset="([\w\W]*?)"/si'|,|$output|,|$matches|);|// 特殊字符编码，如 http://www.500.com
|}
|if|(!|empty|(|$matches|[1]) &&|strstr|(|$matches|[1],|"charset="|) !=|""|) {
|$meta_content_type|=|explode|(|"charset="|,|$matches|[1])[1];
|}
|}
|if|(!in_array(|strtolower|(|$meta_content_type|),|array|(|''|,|'utf-8'|,|'utf8'|))) {
|$output|= mb_convert_encoding(|$output|,|"utf-8"|,|$meta_content_type|);|// gbk, gb2312
|}
|// 若网页仍然有乱码，有乱码则gbk转utf-8
|if|(json_encode(|$output|) ==|''||| json_encode(|$output|) == null) {
|$output|= mb_convert_encoding(|$output|,|"utf-8"|,|'gbk'|);
|}
|$page_info|[|'site_home_size'|] =|strlen|(|$output|);
|\# Title
|preg_match(|'/<TITLE>([\w\W]*?)<\/TITLE>/si'|,|$output|,|$matches|);
|if|(!|empty|(|$matches|[1])) {
|$page_info|[|'site_title'|] =|$matches|[1];
|}
|// 正则匹配，获取全部的meta元数据
|preg_match_all(|'/<META(.*?)>/si'|,|$output|,|$matches|);
|$meta_str_array|=|$matches|[0];
|$meta_array|=|array|();
|$meta_array|[|'description'|] =|''|;
|$meta_array|[|'keywords'|] =|''|;
|foreach|(|$meta_str_array|as|$meta_str|) {
|preg_match(|'/<META\s+name="([\w\W]*?)"\s+content="([\w\W]*?)"/si'|,|$meta_str|,|$res|);
|if|(!|empty|(|$res|))|$meta_array|[|strtolower|(|$res|[1])] =|$res|[2];
|preg_match(|'/<META\s+content="([\w\W]*?)"\s+name="([\w\W]*?)"/si'|,|$meta_str|,|$res|);
|if|(!|empty|(|$res|))|$meta_array|[|strtolower|(|$res|[2])] =|$res|[1];
|preg_match(|'/<META\s+http-equiv="([\w\W]*?)"\s+content="([\w\W]*?)"/si'|,|$meta_str|,|$res|);
|if|(!|empty|(|$res|))|$meta_array|[|strtolower|(|$res|[1])] =|$res|[2];
|preg_match(|'/<META\s+content="([\w\W]*?)"\s+http-equiv="([\w\W]*?)"/si'|,|$meta_str|,|$res|);
|if|(!|empty|(|$res|))|$meta_array|[|strtolower|(|$res|[2])] =|$res|[1];
|preg_match(|'/<META\s+scheme="([\w\W]*?)"\s+content="([\w\W]*?)"/si'|,|$meta_str|,|$res|);
|if|(!|empty|(|$res|))|$meta_array|[|strtolower|(|$res|[1])] =|$res|[2];
|preg_match(|'/<META\s+content="([\w\W]*?)"\s+scheme="([\w\W]*?)"/si'|,|$meta_str|,|$res|);
|if|(!|empty|(|$res|))|$meta_array|[|strtolower|(|$res|[2])] =|$res|[1];
|}
|$page_info|[|'site_keywords'|] =|$meta_array|[|'keywords'|];
|$page_info|[|'site_description'|] =|$meta_array|[|'description'|];
|$page_info|[|'meta_array'|] =|$meta_array|;
|\# mimvp-site-verification
|preg_match(|'/<META\s+name="mimvp-site-verification"\s+content="([\w\W]*?)"/si'|,|$output|,|$matches|);
|if|(|empty|(|$matches|[1])) {
|preg_match(|'/<META\s+content="([\w\W]*?)"\s+name="mimvp-site-verification"/si'|,|$output|,|$matches|);
|}
|if|(!|empty|(|$matches|[1])) {
|$page_info|[|'site_claim_status'|] = 1;
|}
|\# mimvp-site-verification
|if|(|strstr|(|$output|,|'https://proxy.mimvp.com'|) !=|""|) {
|$page_info|[|'friend_link_status'|] = 1;
|}
|return|$page_info|;
|}
|

运行结果，解决方法1 大街网的问题：
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
|'page_info'|=>
|array (size=7)
|'site_title'|=> string|'求职找工作 - 人才招聘 - 大街网'|(length=42)
|'site_description'|=> string|'大街网是中国职业社交网站的缔造者，帮你找到更好的工作并工作的更好。24000000职业人士在这里创建立体化职业档案，打造职业形象，维系和拓展职业人脉，获取职业洞察。'|(length=230)
|'site_keywords'|=> string|'大街网,找工作,招聘,求职,校园招聘,应届生求职,职业档案,找实习,职业形象,职业 档案,中国职业社交网站缔造者。'|(length=149)
|'friend_link_status'|=> int 0
|'site_claim_status'|=> int 0
|'site_home_size'|=> int 171177
|'meta_array'|=>
|array (size=6)
|'description'|=> string|'大街网是中国职业社交网站的缔造者，帮你找到更好的工作并工作的更好。24000000职业人士在这里创建立体化职业档案，打造职业形象，维系和拓展职业人脉，获取职业洞察。'|(length=230)
|'keywords'|=> string|'大街网,找工作,招聘,求职,校园招聘,应届生求职,职业档案,找实习,职业形象,职业 档案,中国职业社交网站缔造者。'|(length=149)
|'referrer'|=> string|'always'|(length=6)
|'content-type'|=> string|'text/html; charset=UTF-8'|(length=24)
|'x-ua-compatible'|=> string|'ie=8'|(length=4)
|'saoatech-site-verification'|=> string|'N2Y5MzU2M2YyNmVhOTllMGQwYmI4NzAwMTIxYmE2ZDU='|(length=44)
|

一切都在代码里，核心有两个：
1）匹配了所有meta信息，实现了自定义的get_meta_tags
2）解决网页乱码的问题，否则获取的数据是乱码或为空
正则匹配，基本可以解决一切问题了
但是用正则解决的问题，一般还会考虑有HTMP Parse解析来提取元素 element

**4. HTML Dom 解析网页，提取元素内容**
方法3 正则解析网页，是一个非常好的方法，这里再附加介绍一种 HTML Dom 解析，替换方法3的正则匹配即可
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
|$doc|=|new|DOMDocument();
|@|$doc|->loadHTML(|$output|);
|// title
|$nodes|=|$doc|->getElementsByTagName(|'title'|);
|$title|=|$nodes|->item(0)->nodeValue;
|// meta
|$meta_array|=|array|();
|$metas|=|$doc|->getElementsByTagName(|'meta'|);
|for|(|$i|= 0;|$i|<|$metas|->length;|$i|++) {
|$meta|=|$metas|->item(|$i|);
|if|(|$meta|->getAttribute(|'name'|)) {
|$meta_array|[|strtolower|(|$meta|->getAttribute(|'name'|))] =|$meta|->getAttribute(|'content'|);
|}
|if|(|$meta|->getAttribute(|'http-equiv'|)) {
|$meta_array|[|strtolower|(|$meta|->getAttribute(|'http-equiv'|))] =|$meta|->getAttribute(|'content'|);
|}
|if|(|$meta|->getAttribute(|'scheme'|)) {
|$meta_array|[|strtolower|(|$meta|->getAttribute(|'scheme'|))] =|$meta|->getAttribute(|'content'|);
|}
|}
|$page_info|[|'site_title'|] =|$title|;
|$page_info|[|'site_keywords'|] =|$meta_array|[|'keywords'|];
|$page_info|[|'site_description'|] =|$meta_array|[|'description'|];
|$page_info|[|'meta_array'|] =|$meta_array|;
|

遇到问题，一般采用方法3 和 方法4，基本可以解决80%的网页抓取问题
此方法4和方法3，能够正常解析网页的前提是网页内容不能有乱码，
解决乱码详见方法1和方法3，深究代码，注释已给出了四种方法
下面给出测试case，供你验证乱码问题：
|1
|2
|3
|4
|5
|6
|7
|<?php
|header(|"content-type:text/html; charset=xxx"|);
|$url|=|"https://mimvp.com"|;     // 默认 utf-8
|$url|=|"http://www.qq.com"|;     // 默认 gb2312，需添加 header，否则乱码
|$url|=|"https://www.dajie.com"|; // content在前，name在后，匹配错误（从第一个content开始，从最后一个name结束）
|$url|=|"http://dytt8.net"|;      // 无法获得网页编码，则用 json_encode(|$output|) ==|''|检测网页乱码
|


**附上****[HTML <meta> 标签](http://www.w3school.com.cn/tags/tag_meta.asp)**
**定义和用法**
<meta> 元素可提供有关页面的元信息（meta-information），比如针对搜索引擎和更新频度的描述和关键词。
<meta> 标签位于文档的头部，不包含任何内容。<meta> 标签的属性定义了与文档相关联的名称/值对。
**HTML 与 XHTML 之间的差异**
在 HTML 中，<meta> 标签没有结束标签。
在 XHTML 中，<meta> 标签必须被正确地关闭。
**提示和注释**
注释：<meta> 标签永远位于 head 元素内部。
注释：元数据总是以名称/值的形式被成对传递的。
必需的属性
|属性|值|描述|
|content|some_text|定义与 http-equiv 或 name 属性相关的元信息|
可选的属性
|属性|值|描述|
|http-equiv|content-type
|expires
|refresh
|set-cookie
|把 content 属性关联到 HTTP 头部。|
|name|author
|description
|keywords
|generator
|revised
|others
|把 content 属性关联到一个名称。|
|scheme|some_text|定义用于翻译 content 属性值的格式。|

**总结**
至此，本文比较详细、全面的总结了PHP获取网页标题(title)、描述(description)、关键字(keywords)等meta信息的多种方法
方法3，米扑科技自己写代码，改进了网页匹配，实现了获取全部meta信息，比PHP内置的get_meta_tags函数功能更加强大，也实现了一些自定义需求，基本满足了米扑科技的项目要求。其中，还有一些技术细节处理，需根据项目改进优化，这里就不给出了。
本文分析了抓取网页遇到的多种问题，并给出了一个比较完善的匹配解决方案，
本文基本可解决90%的网页匹配问题，余下的10%问题，在项目中遇到了就去逐步解决
例如，有些网站禁止了curl、wget爬虫，也有的强制需要cookie、referer、host等header参数
解决方案就是 PHP + Selenium + Firefox，通过虚拟浏览器爬取展现网页内容，并抓取其内容再解析，且听下文分解
**应用实例**
本文PHP匹配抓取网页，应用于米扑科技的项目有[米扑导航](https://site.mimvp.com/)、[米扑代理](https://proxy.mimvp.com/)、[米扑博客](https://blog.mimvp.com/)、[米扑域名](https://domain.mimvp.com/)等
为方便客户，米扑科技提供了对外的API接口：[https://site.mimvp.com/api/header.php](https://site.mimvp.com/api/header.php)


