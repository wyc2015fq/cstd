
# CSDN博客搬家到WordPress - 阳光岛主 - CSDN博客

2015年12月06日 21:33:40[阳光岛主](https://me.csdn.net/sunboy_2050)阅读数：4699


一直想把CSDN上写的博客，搬家移植到自己的WordPress博客，网上介绍了一些方法，例如先把CSDN博客导入博客园，然后从博客园导出xml文件，最后用WordPress导入工具导入进WordPress博客，操作步骤太繁杂。
于是周末在家自己动手用Python写了爬虫脚本，利用WordPress自定义的投稿功能（wp_insert_post），导入进WordPress博客，步骤只有简单的两步：
步骤1：Python脚本爬取CSDN博客，包括博客**标题，正文，发布日期，浏览数，标签**
步骤2：利用[WordPress投稿功能](http://blog.mimvp.com/2015/11/add-posting-functionality-to-wordpress/)插入到WordPress，把所有CSDN博客导入WordPress
**Python完整代码：**
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
|164
|165
|166
|167
|168
|169
|170
|171
|172
|173
|174
|175
|176
|177
|178
|179
|180
|181
|182
|183
|184
|185
|186
|187
|188
|189
|190
|191
|192
|193
|194
|195
|196
|197
|198
|199
|200
|201
|202
|203
|204
|205
|206
|207
|208
|209
|210
|211
|212
|213
|214
|215
|216
|217
|218
|219
|220
|221
|222
|223
|224
|225
|226
|227
|228
|229
|230
|231
|232
|233
|234
|235
|236
|237
|238
|239
|240
|241
|242
|243
|244
|245
|246
|247
|248
|249
|250
|251
|252
|253
|254
|255
|256
|257
|258
|259
|260
|261
|262
|263
|264
|265
|266
|267
|268
|269
|270
|271
|272
|273
|274
|275
|276
|277
|278
|279
|280
|281
|282
|283
|284
|285
|286
|287
|288
|289
|290
|291
|292
|293
|294
|295
|296
|297
|298
|299
|300
|301
|302
|303
|304
|305
|306
|307
|308
|309
|310
|311
|312
|313
|314
|315
|316
|317
|318
|319
|320
|321
|322
|323
|324
|325
|326
|327
|328
|329
|330
|331
|332
|333
|334
|335
|336
|337
|338
|339
|340
|341
|342
|343
|344
|345
|346
|347
|348
|349
|350
|351
|352
|353
|354
|355
|356
|357
|358
|359
|360
|361
|362
|363
|364
|365
|366
|367
|368
|369
|370
|371
|372
|373
|374
|375
|376
|\#!/usr/bin/env python
|\# -*- coding:utf8 -*-
|'''
|author  :    yanggang@mimvp.com
|date    :    2015-12-01
|blog    :    http://blog.mimvp.com
|demo    :    http://blog.mimvp.com/category/csdn_blog/
|安装库文件：
|1. MySQLdb，连接MySQL数据库，修改浏览次数
|2. bs4，    格式化HTML，抓取过滤CSDN网页内容

|WordPress博客POST文章接口API
|http://blog.mimvp.com/tougao/
|查询自动POST文章ID：
|select id from wp_posts where post_title like '【米扑代理】%'
|修改POST文章浏览数：
|update wp_postmeta set meta_value=meta_value+100 where meta_key='views' and post_id in (select id from wp_posts where post_title like '【米扑代理】%');
|修改浏览次数小于100
|select post_id, meta_value from wp_postmeta where  meta_key='views' and meta_value < 100 and post_id in (select id  from wp_posts where post_status = 'publish');
|'''
|import|time, datetime
|import|os, random
|import|urllib, urllib2
|import|base64
|import|bs4
|import|MySQLdb
|import|sys
|reload|(sys)
|sys.setdefaultencoding(|'utf-8'|)
|\# 排除要抓取的CSDN博客链接
|EXCLUDE_BLOG_URL|=|[
|'http://blog.csdn.net/sunboy_2050/article/details/45868613'|,
|'http://blog.csdn.net/sunboy_2050/article/details/49449319'|,
|'http://blog.csdn.net/sunboy_2050/article/details/48622023'|,
|'http://blog.csdn.net/sunboy_2050/article/details/47858847'|,
|]
|\# 根据CSDN博客标题打标签tags
|CATEGORY_DICT|=|{
|'Python'|:|1|,
|'C/C++'|:|2|,
|'网络常识'|:|3|,
|'Algrithm'|:|11|,
|'Clojure'|:|4|,
|'CSDN'|:|5|,
|'Git/SVN'|:|6|,
|'Go'|:|7|,
|'HTML/CSS/JS'|:|8|,
|'iOS/Android'|:|9|,
|'Java/JSP'|:|113|,
|'Linux/Unix'|:|95|,
|'MacBook'|:|111|,
|'Nginx/Apache'|:|112|,
|'PHP'|:|777|,
|'SQL/NoSQL'|:|114|,
|'Storm/Hadoop'|:|115|,
|'WP技巧'|:|12|,
|'产品经理'|:|116|,
|'创业邦'|:|3|,
|'理财'|:|117|,
|'生活小札'|:|4|,
|'科技资讯'|:|10|,
|'米扑代理'|:|118|,
|'系统架构'|:|97|,
|'软件测试'|:|119|,
|'leetcode'|:|11|,
|'链表'|:|11|,
|'算法'|:|11|,
|'leetcode'|:|11|,
|'django'|:|1|,
|'tornado'|:|1|,
|'c语言'|:|2|,
|'c++'|:|2|,
|'c\#'|:|2|,
|'vc'|:|2|,
|'qt'|:|2|,
|'网络'|:|3|,
|'算法'|:|11|,
|'Git'|:|6|,
|'SVN'|:|6|,
|'版本控制'|:|6|,
|'HTML'|:|8|,
|'CSS'|:|8|,
|'JS'|:|8|,
|'javascript'|:|8|,
|'iOS'|:|9|,
|'Android'|:|9|,
|'Java'|:|113|,
|'JSP'|:|113|,
|'JVM'|:|113|,
|'Spring'|:|113|,
|'Eclipse'|:|113|,
|'Linux'|:|95|,
|'Unix'|:|95|,
|'Ubuntu'|:|95|,
|'CentOS'|:|95|,
|'Shell'|:|95|,
|'AWK'|:|95|,
|'vim'|:|95|,
|'Nginx'|:|112|,
|'Apache'|:|112|,
|'Tomcat'|:|112|,
|'数据库'|:|114|,
|'SQL'|:|114|,
|'NoSQL'|:|114|,
|'MySQL'|:|114|,
|'Redis'|:|114|,
|'Memcache'|:|114|,
|'mongo'|:|114|,
|'sqlite'|:|114|,
|'WP'|:|12|,
|'WordPress'|:|12|,
|'软件'|:|119|,
|'测试'|:|119|,
|'Storm'|:|115|,
|'Hadoop'|:|115|,
|}
|\# CSDN博客用urllib2 + header抓取不了，改用了curl爬取网页
|headers|=|{
|'Use-Agent'|:|'Mozilla/5.0 (X11; Linux x86_64) AppleWebKit/537.36 (KHTML, like Gecko) Chrome/41.0.2272.118 Safari/537.36'|,
|'Cookie'|:|'bdshare_firstime=1430381399166; uuid_tt_dd=-4349350129693538496_20150430; __gads=ID=b549fb1461110ca8:T=1430381399:S=ALNI_MbhiN2nMgWWiXPP25667Pq-BPZf5g; CloudGuest=dbieDkbnW7cz5P9qimYEjpecak8Udv0BPR8Iflg0PlBd3HR1Wj+RyQQksR2cDE9ab/hPXNGjFpuKsRe1dFjVJjY+mf3bfeWiP6kN0TKk1rY6g5SOuowPs/8F5FJJBdddW71JZ7rp4Q9b8DsLk2TASIPHLj3iL599bPUGKga0mRsaTJi0td73QBZaNlY7+VAl; __qca=P0-127195741-1432118980220; lzstat_uv=29735680241046032471|2955225@3582543@2675686@3411160; __utma=1722629.287164464.1430711804.1438215324.143920134.51; __utmz=1722629.14380001130.49.44.utmcsr=baidu|utmccn=(organic)|utmcmd=organic; UN=Sunboy_2050; UE="yanggang_2050@19.com"; __message_district_code=110000; uuid=92c68418-b2bd-41bc-846d-a1e4e9cf094; _ga=GA1.2.287164464.1430711804; scvh=2011-09-09+17%3a11%3a33+003; FullCookie=1; ViewMode=list; avh=8471508%2c298041%2c45868613%2c2659431%2c17398807; __message_sys_msg_id=0; __message_gu_msg_id=0; __message_cnel_msg_id=0; __message_in_school=0; lzstat_ss=3875140043_20_1448553416_2955225; dc_tos=nyewqw; dc_session_id=1448524616939'|,
|'Host'|:|'blog.csdn.net'|,
|'DNT'|:|1|,
|'Cache-Control'|:|'max-age=0'|,
|'Connection'|:|'keep-alive'|,
|'Accept'|:|'text/html,application/xhtml+xml,application/xml;q=0.9,image/webp,*/*;q=0.8'|,
|'Accept-Encoding'|:|'gzip, deflate, sdch'|,
|'Accept-Language'|:|'en-US,en;q=0.8,zh-CN;q=0.6,zh;q=0.4'|,
|'DNT'|:|1|,
|}
|\# hangzhou of 115.29.237.28
|PROXY_MYSQL_SERVER|=|{
|"host"|:|"localhost"|,
|"port"|:|3306|,
|"user"|:|"root"|,
|"passwd"|:|"123456"|,
|"dbname"|:|"wp_blog"|,
|}
|\# 获取当前的实时日期
|def|get_now_datetime():
|return|datetime.datetime.now().strftime(|'%Y-%m-%d %H:%M:%S'|)

|def|split_category(blog_title|=|''):
|cat_keys|=|CATEGORY_DICT.keys()
|blog_title|=|blog_title.lower()
|print|blog_title
|cat_list|=|[|'5'|]
|for|cat|in|cat_keys:
|cat_lower|=|cat.lower()
|if|blog_title.find(cat_lower) >|=|0|:
|cat_value|=|str|(CATEGORY_DICT.get(cat))
|cat_list.append(cat_value)
|cat_join|=|","|.join(cat_list)
|return|cat_join

|\# 爬取博客链接，先爬取翻页数 ——》 拼接翻页链接 ——》 爬取翻页正文 ——》 获取博客链接
|def|spider_blog_url_list(blog_url|=|'http://blog.csdn.net/sunboy_2050/'|):
|blog_url_list|=|[]
|blog_url|=|blog_url
|print|(|"blog_url: "|+|blog_url)

|\#\#\#\#\#\#\#\#\#\#\#\#\#\#\#\#\#\#\#\#\#\#\#\#\# 爬取博客翻页链接 \#\#\#\#\#\#\#\#\#\#\#\#\#\#\#\#\#\#\#\#\#\#\#\#
|try|:
|\#         req = urllib2.Request(blog_url, headers=headers)
|\#         content = urllib2.urlopen(req).read()
|content|=|os.popen(|'curl '|+|blog_url).read()
|content|=|bs4.BeautifulSoup(content)
|content|=|content.prettify()
|content|=|bs4.BeautifulSoup(content, from_encoding|=|'GB18030'|)
|table_soup|=|content.find(|'div'|, {|"id"|:|"papelist"|}).find_all(|'a'|)
|last_a|=|table_soup[|len|(table_soup)|-|1|]
|last_a_href|=|last_a[|'href'|]
|list_a_prefix, list_num|=|os.path.split(last_a_href)
|list_num|=|int|(list_num)
|blog_url_page_list|=|[]
|for|i|in|range|(list_num):
|i|+|=|1
|page_url|=|'http://blog.csdn.net'|+|list_a_prefix|+|'/'|+|str|(i)
|print|page_url
|blog_url_page_list.append(page_url)
|except|Exception as ex:
|print|(|"spider_url() - error_msg: "|+|str|(ex))

|\#\#\#\#\#\#\#\#\#\#\#\#\#\#\#\#\#\#\#\#\#\#\#\#\# 爬取每页的博客链接 \#\#\#\#\#\#\#\#\#\#\#\#\#\#\#\#\#\#\#\#\#\#\#\#
|blog_url_set|=|set|()
|list_len|=|len|(blog_url_page_list)
|index|=|0
|for|page_url|in|blog_url_page_list:
|index|+|=|1
|print|(|"++++ "|, index,|"/"|, list_len,|"page_url: "|+|page_url)
|try|:
|content|=|os.popen(|'curl '|+|page_url).read()
|content|=|bs4.BeautifulSoup(content)
|content|=|content.prettify()
|content|=|bs4.BeautifulSoup(content)
|span_list|=|content.find_all(|'span'|, {|"class"|:|"link_title"|})
|for|span|in|span_list:
|a_list|=|span.find_all(|'a'|)
|for|a|in|a_list:
|href|=|a[|'href'|]
|blog_url|=|'http://blog.csdn.net'|+|href
|print|(|"blog_url: "|+|blog_url)
|blog_url_set.add(blog_url)
|except|Exception as ex:
|print|(|"spider_url() - error_msg: "|+|str|(ex))
|blog_url_list|=|blog_url_set
|return|blog_url_list

|\# 爬取博客正文
|def|spider_blog_url(blog_url|=|'http://blog.csdn.net/sunboy_2050/article/details/45868613'|):
|print|blog_url
|if|blog_url|in|EXCLUDE_BLOG_URL:
|print|(|"blog_url is IN EXCLUDE_BLOG_URL, blog_url: "|+|blog_url)
|return
|blog_title|=|''
|blog_content|=|''
|blog_tags|=|''
|blog_cat|=|'5'
|blog_postdate|=|''
|blog_viewsCount|=|0
|try|:
|content|=|os.popen(|'curl '|+|blog_url).read()
|content|=|bs4.BeautifulSoup(content)
|content|=|content.prettify()
|content|=|bs4.BeautifulSoup(content)
|blog_title|=|content.find(|'span'|, {|"class"|:|"link_title"|}).text.strip()
|blog_title|=|blog_title.replace(|"\n"|, "|").replace("|[置顶]|", "|").strip()
|blog_cat|=|split_category(blog_title)
|tags_set|=|set|()
|tags_link|=|content.find(|'span'|, {|"class"|:|"link_categories"|})
|if|tags_link:
|a_list|=|tags_link.find_all(|'a'|)
|for|a|in|a_list:
|tags_set.add(a.text.strip())
|blog_tags|=|","|.join(tags_set)
|blog_postdate|=|content.find(|'span'|, {|"class"|:|"link_postdate"|}).text.strip()
|blog_postdate|=|blog_postdate|+|":"|+|str|(random.randint(|10|,|60|))
|blog_viewsCount|=|content.find(|'span'|, {|"class"|:|"link_view"|}).text.strip()
|blog_viewsCount|=|blog_viewsCount.replace(|"人阅读"|, "")
|blog_content|=|content.find(|'div'|, {|"class"|:|"article_content"|})
|\# 语法高亮，例如： <pre class="python" xxx    ===> <pre class="brush:python" xxxx
|blog_content|=|str|(blog_content).replace(|'<pre class="'|,|'<pre class="brush:'|)

|except|Exception as ex:
|print|(|"spider_blog_url() - error_msg: "|+|str|(ex))

|print|blog_title
|print|blog_tags
|print|blog_postdate
|print|blog_viewsCount
|print|blog_cat
|\#     print blog_content
|post_blog(blog_title, blog_tags, blog_content, blog_postdate, blog_viewsCount, blog_cat, blog_url)

|\# 爬取博客正文
|def|spider_blog(blog_root|=|'http://blog.csdn.net/sunboy_2050/'|):
|blog_url_list|=|spider_blog_url_list(blog_root)
|list_len|=|len|(blog_url_list)
|index|=|0
|for|blog_url|in|blog_url_list:
|index|+|=|1
|print|"++++++++++++++ "|, index,|"/"|, list_len,|"++++++++++++++ "|, blog_url
|spider_blog_url(blog_url)

|\# 写入博客， cat=5 ： CSDN分类
|def|post_blog(title|=|'test_title'|, tags|=|'test_tag'|, content|=|'test_content'|, postdate|=|'|', viewsCount=0, cat='|5|', blog_url='|'):
|if|postdate|=|=|'':
|postdate|=|datetime.datetime.now().strftime(|'%Y-%m-%d %H:%M:%S'|)
|post_data|=|{
|'tougao_form'|:|'blog_mimvp'|,
|'tougao_authorname'|:|'admin'|,
|'tougao_authoremail'|:|'yanggang@mimvp.com'|,
|'tougao_authorblog'|:|'blog.mimvp.com'|,
|'tougao_title'|:   title,
|'tougao_tags'|:   tags,
|'tougao_cat'|:   cat,
|'tougao_content'|:   content,
|'tougao_date'|:   postdate,
|}
|content_head|=|"<div style='font-size: 16px;'>"
|content_foot|=|"</div><div style='margin: 50px auto 50px;'><h3><font color='red'>原文：</font> <a target='_blank' href='{blog_url}'>{blog_title}</a></h3></div>"|.|format|(blog_url|=|blog_url, blog_title|=|title)
|tougao_content|=|content_head|+|str|(content)|+|str|(content_foot)
|try|:
|POST_URL|=|'http://blog.mimvp.com/tougao/'
|post_data[|'tougao_content'|]|=|tougao_content
|post_data|=|urllib.urlencode(post_data)
|req|=|urllib2.Request(POST_URL, data|=|post_data)
|res|=|urllib2.urlopen(req).read()
|except|Exception as ex:
|print|(|"error_msg: "|+|str|(ex))
|\# 延时一分钟后，等文章发布后，修改浏览次数
|print|(|"sleep 3, then modify post_views..."|)
|time.sleep(|3|)
|modify_post_views(title, viewsCount)

|\# 文章发布后，修改浏览次数
|def|modify_post_views(tougao_title|=|'', viewsCount|=|100|):
|if|tougao_title|=|=|'':
|print|(|"error: no post title"|)
|return
|sql|=|"update wp_postmeta set meta_value=meta_value+{viewsCount} where meta_key='views' and post_id in (select id from wp_posts where post_title = '{tougao_title}');"|.|format|(viewsCount|=|viewsCount, tougao_title|=|tougao_title)
|print|sql
|try|:
|sql_conn|=|MySQLdb.connect(host|=|PROXY_MYSQL_SERVER[|'host'|],
|port|=|int|(PROXY_MYSQL_SERVER[|'port'|]),
|user|=|PROXY_MYSQL_SERVER[|'user'|],
|passwd|=|PROXY_MYSQL_SERVER[|'passwd'|],
|db|=|PROXY_MYSQL_SERVER[|'dbname'|],
|charset|=|'utf8'|)
|sql_cursor|=|sql_conn.cursor()
|sql_cursor.execute(sql)
|sql_cursor.close()
|sql_conn.close()
|except|Exception, ex:
|print|(|"check_except_province_little_proxy（） -- error_msg: %r"|%|ex)

|if|__name__|=|=|'__main__'|:
|spider_blog()
|

**博客搬家示例**
导出CSDN原博客  ：[http://blog.csdn.net/sunboy_2050](http://blog.csdn.net/sunboy_2050)
导入WordPress博客：[http://blog.mimvp.com/category/csdn_blog/](http://blog.mimvp.com/category/csdn_blog/)



