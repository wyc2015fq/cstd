# （Nginx） URL  REWRITE - 在思索中前行！ - CSDN博客





2014年09月25日 22:45:34[_Tham](https://me.csdn.net/txl16211)阅读数：4656








## URL重写的基础介绍

把URI地址用作参数传递：URL REWRITE

最简单的是基于各种WEB服务器中的URL重写转向（Rewrite）模块的URL转换：

这样几乎可以不修改程序的实现将 news.asp?id=234 这样的链接映射成 news/234.html，从外面看上去和静态链接一样。Apache服务器上有一个模块（非缺省）：mod_rewrite：URL REWRITE功能之强大足够写上一本书。

当我需要将将news.asp?id=234的映射成news/234.html时，只需设置：



RewriteRule /news/(/d+)/.html /news/.asp/?id=$1 [N,I]

这样就把 /news/234.html 这样的请求映射成了 /news.asp?id=234

当有对/news/234.html的请求时：web服务器会把实际请求转发给/news.asp?id=234

而在IIS也有相应的REWRITE模块：比如ISAPI REWRITE和IIS REWRITE，语法都是基于正则表达式，因此配置几乎和apache的mod_rewrite是相同的：



比对于某一个简单应用可以是：

RewriteRule /news/(/d+)/.html /news/news/.php/?id=$1 [N,I]

这样就把 http://www.chedong.com/news/234.html 映射到了 http://www.chedong.com/news/news.php?id=234

一个更通用的能够将所有的动态页面进行参数映射的表达式是：

把 http://www.myhost.com/foo.php?a=A&b=B&c=C 

表现成 http://www.myhost.com/foo.php/a/A/b/B/c/C。

RewriteRule (.*?/.php)(/?[^/]*)?/([^/]*)/([^/]*)(.+?)?$1(?2$2&:/?)$3=$4?5$5: [N,I]


以下是针对phpBB的一个Apache mod_rewrite配置样例：

       RewriteEngine On

       RewriteRule /forum/topic_(.+)/.html$     /forum/viewtopic.php?t=$1 [L]

       RewriteRule /forum/forum_(.+)/.html$ /forum/viewforum.php?f=$1 [L]

       RewriteRule /forum/user_(.+)/.html$     /forum/profile.php?mode=viewprofile&u=$1     [L]

这样设置后就可以通过topic_1234.html forum_2.html user_34.html这样的链接访问原来的动态页面了。

通过URL REWRITE还有一些好处：

mod_rewrite和isapirewrite基本兼容，但是还是有些不同，比如：isapirewrite中"?"需要转义成"/?"，mod_rewrite不用，isapirewrite支持 "/d+" （全部数字），mod_rewrite不支持


? 隐藏后台实现：这在后台应用平台的迁移时非常有用：当从asp迁移到java平台时，对于前台用户来说，根本感受不到后台应用的变化； 

? 简化数据校验：因为像(/d+)这样的参数，可以有效的控制数字的格式甚至位数； 

比如我们需要将应用从news.asp?id=234迁移成news.php?query=234时，前台的表现可以一直保持为 news/234.html。从实现应用和前台表现的分离：保持了URL的稳定性，而使用mod_rewrite甚至可以把请求转发到其他后台服务器上。

基于PATH_INFO的URL美化

Url美化的另外一个方式就是基于PATH_INFO：

PATH_INFO是一个CGI 1.1的标准，经常发现很多跟在CGI后面的"/value_1/value_2"就是PATH_INFO参数：

比如：http://phpunixman.sourceforge.net/index.php/man/ls 中：$PATH_INFO = "/man/ls"

PATH_INFO是CGI标准，因此PHP Servlet等都有的支持。比如Servlet中就有request.getPathInfo()方法。

注意：/myapp/servlet/Hello/foo的 getPathInfo()返回的是/foo，而/myapp/dir/hello.jsp/foo的getPathInfo()将返回的 /hello.jsp，从这里你也可以知道jsp其实就是一个Servlet的PATH_INFO参数。ASP不支持PATH_INFO


PHP中基于PATH_INFO的参数解析的例子如下：

//注意：参数按"/"分割，第一个参数是空的：从/param1/param2中解析出$param1 $param2这2个参数

if ( isset($_SERVER["PATH_INFO"]) ) {

       list($nothing, $param1, $param2) = explode('/', $_SERVER["PATH_INFO"]);

}

如何隐蔽应用：例如 .php，的扩展名：

在APACHE中这样配置：

<FilesMatch "^app_name$">

       ForceType application/x-httpd-php

</FilesMatch>

如何更像静态页面：app_name/my/app.html

解析的PATH_INFO参数的时候，把最后一个参数的最后5个字符“.html”截断即可。

注意：APACHE2中缺省是不允许PATH_INFO的，需要设置 AcceptPathInfo on

特别是针对使用虚拟主机用户，无权安装和配置mod_rewrite的时候，PATH_INFO往往就成了唯一的选择。

OK，这样以后看见类似于http://www.example.com/article/234这样的网页你就知道可能是 article/show.php?id=234这个php程序生成的动态网页，很多站点表面看上去可能有很多静态目录，其实很有可能都是使用1，2个程序实现的内容发布。比如很多WIKIWIKI系统都使用了这个机制：整个系统就一个简单的wiki程序，而看上去的目录其实都是这个应用拿后面的地址作为参数的查询结果。

利用基于MOD_REWRITE/PATH_INFO ＋ CACHE服务器的解决方案对原有的动态发布系统进行改造，也可以大大降低旧有系统升级到新的内容管理系统的成本。并且方便了搜索引擎收录入索引。



## [nginx 重写 rewrite 基础及实例](http://www.cnblogs.com/buffer/p/3204718.html)

nginx rewrite 正则表达式匹配

大小写匹配

~ 为区分大小写匹配 

~* 为不区分大小写匹配 

!~和!~*分别为区分大小写不匹配及不区分大小写不匹配 

文件及目录匹配

-f和!-f用来判断是否存在文件 

-d和!-d用来判断是否存在目录 

-e和!-e用来判断是否存在文件或目录 

-x和!-x用来判断文件是否可执行 

flag标记

last 相当于Apache里的[L]标记，表示完成rewrite

break 终止匹配, 不再匹配后面的规则。

redirect 返回302临时重定向 地址栏会显示跳转后的地址。

permanent 返回301永久重定向 地址栏会显示跳转后的地址。

logcation的几个使用实例：

   1）location  / { }：匹配任何查询，因为所有请求都以 / 开头。但是正则表达式规则将被优先和查询匹配。

   2）location =/ {}：仅仅匹配/

   3）location ~* \.(gif|jpg|jpeg)$

     ｛

        rewrite \.(gif|jpg)$ /logo.png;

     ｝：location不区分大小写，匹配任何以gif，jpg，jpeg结尾的文件。




### 几个实例：

### 多目录转成参数 

要求：abc.domian.com/sort/2 => abc.domian.com/index.php?act=sort&name=abc&id=2 

规则配置：

if ($host ~* (.*)\.domain\.com) { 

    set $sub_name $1;

    rewrite ^/sort\/(\d+)\/?$ /index.php?act=sort&cid=$sub_name&id=$1 last; 

} 

### 目录对换

要求：/123456/xxxx -> /xxxx?id=123456 

规则配置：

rewrite ^/(\d+)/(.+)/ /$2?id=$1 last; 



再来一个针对浏览器优化的自动rewrite，这里rewrite后的目录可以是存在的；

例如设定nginx在用户使用ie的使用重定向到/nginx-ie目录

规则如下：

 if ($http_user_agent ~ MSIE) {

     rewrite ^(.*)$ /nginx-ie/$1 break; 

} 

目录自动加“/” ，这个功能一般浏览器自动完成

if (-d $request_filename){ 

rewrite ^/(.*)([^/])$ http://$host/$1$2/ permanent; 

} 

以下这些可能就跟广义的rewrite重写无关了

禁止htaccess

location ~/\.ht { 

    deny all; 

} 

### 禁止多个目录 

location ~ ^/(cron|templates)/ { 

    deny all; break; 

} 

禁止以/data开头的文件，可以禁止/data/下多级目录下.log.txt等请求

location ~ ^/data { 

    deny all; 

} 

### 禁止单个文件 

location ~ /data/sql/data.sql { 

    deny all; 

} 

给favicon.ico和robots.txt设置过期时间; 这里为favicon.ico为99天,robots.txt为7天并不记录404错误日志 

location ~(favicon.ico) { 

    log_not_found off; 

    expires 99d; 

    break; 

} 

location ~(robots.txt) { 

    log_not_found off; 

    expires 7d; 

    break; 

} 

设定某个文件的浏览器缓存过期时间;这里为600秒，并不记录访问日志 

location ^~ /html/scripts/loadhead_1.js { 

    access_log off; 

    expires 600; 

    break; 

} 

Nginx还可以自定义某一类型的文件的保质期时间，具体写法看下文的代码：

location ~* \.(js|css|jpg|jpeg|gif|png|swf)$ {

if (-f $request_filename) {

   expires    1h;

   break;

  }

  }

//上段代码就将js|css|jpg|jpeg|gif|png|swf这类文件的保质期设置为一小时。

### 防盗链的设置：

防盗链：如果你的网站是个下载网站，下载步骤应该是先经过你的主页找到下载地址，才能下载，为了防止某些网友直接访问下载地址完全不通过主页下载，我们就可以使用防盗链的方式，具体代码如下：

location ~* \.(gif|jpg|swf)$ {

  valid_referers none blocked start.igrow.cn sta.igrow.cn;

  if ($invalid_referer) {

  rewrite ^/ http://$host/logo.png;

  }

}

文件反盗链并设置过期时间--<盗链多次请求也会打开你的站点的图片啊，所以设置下缓存时间，不会每次盗链都请求并下载这张图片>

location ~* ^.+\.(jpg|jpeg|gif|png|swf|rar|zip|css|js)$ { 

    valid_referers none blocked *.jjonline.cn *.jjonline.com.cn *.lanwei.org *.jjonline.org localhost  42.121.107.189; 

    if ($invalid_referer) { 

        rewrite ^/ http://img.jjonline.cn/forbid.gif; 

        return 417; 

        break; 

    } 

    access_log off; 

    break; 

} 

说明：

这里的return 417 为自定义的http状态码，默认为403，方便通过nginx的log文件找出正确的盗链的请求地址 

“rewrite ^/ http://img.jjonline.cn/forbid.gif;”显示一张防盗链图片

 “access_log off;”不记录访问日志，减轻压力 

“expires 3d”所有文件3天的浏览器缓存 

### 只充许固定ip访问网站，并加上密码；这个对有权限认证的应用比较在行

location \ { 

    allow 22.27.164.25; #允许的ipd

    deny all; 

    auth_basic “KEY”; #认证的一些设置

    auth_basic_user_file htpasswd; 

}

说明：location的应用也有各种变化，这里的写法就针对了根目录了。

文件和目录不存在的时重定向

if (!-e $request_filename) { 

    #proxy_pass http://127.0.0.1; #这里是跳转到代理ip，这个代理ip上有一个监听的web服务器

    rewrite ^/ http://www.jjonline.cn/none.html;  #跳转到这个网页去

    #return 404; #直接返回404码，然后会寻找root指定的404.html文件

} 

### 域名跳转 

server { 

    listen 80; 

    server_name jump.jjonline.cn ;#需要跳转的多级域名

    index index.html index.htm index.php; #入口索引文件的名字

    root /var/www/public_html/; #这个站点的根目录

    rewrite ^/ http://www.jjonline.cn/; 

    #rewrite到这个地址，功能表现：在浏览器上输入jump.jjonline.cn并回车，不会有任何提示直接变成www.jjonline.cn

    access_log off; 

} 

### 多域名转向

server { 

    listen 80; 



    server_name www.jjonline.cn www.jjonline.org;

    index index.html index.htm index.php; 

    root /var/www/public_html/; 

    if ($host ~ “jjonline\.org”) { 

        rewrite ^(.*) http://www.jjonline.cn$1 permanent; 

    } 

}

### 三级域名跳转 

if ($http_host ~* “^(.*)\.i\.jjonline\.cn$”) { 

    rewrite ^(.*) http://demo.jjonline.cn$1; 

    break; 

} 

域名镜向

server { 

    listen 80; 

    server_name mirror.jjonline.cn; 

    index index.html index.htm index.php; 

    root /var/www/public_html; 

    rewrite ^/(.*) http://www.jjonline.cn/$1 last; 

    access_log off; 

} 

某个子目录作镜向,这里的示例是demo子目录

location ^~ /demo { 

    rewrite ^.+ http://demo.jjonline.cn/ last; 

    break; 

}

以下在附带本博客的rewrite写法，emlog系统的rewrite



location ~ {

    if (!-e $request_filename) {

           rewrite ^/(.+)$ /index.php last;

    }

}

# [使用PHP模拟 URL Rewrite](http://blog.csdn.net/zuiaituantuan/article/details/5907154)



项目中有许多的服务器不支持Rewrite，或者配置起来太麻烦，看到许多网站也采用了下面方式的Rewrite方法，于是想到可以用PHP模拟一个这样URL。


*复制内容到剪贴板*
##### 代码:
`?a/haha/b/hehe/d/hoho.html`

通过实例化类后，也可以通过$_GET及$_SERVER["QUERY_STRING"]正常获取到页面GET参数的传递，并附加了$_SERVER["R_QUERY_STRING"]预定义变量。这样许多的项目就可以通过不用太大的改变就可以实现Rewrite了。


这个类同时支持以下几种模拟的URL，同时支持原始形态的URL地址，并自动删除值为空的参数和值： 

*复制内容到剪贴板*
##### 代码:

```
?a/haha/b/hehe/d/hoho.html

?a/haha/b/hehe/d/hoho/

?a/haha/b/hehe/d/hoho
```


代码刚写完，来不及做更多测试，希望大家多多指出代码中的错误和问题。

下面是这个类的代码 (url_rewrite.class.php)： 

*复制内容到剪贴板*
##### 代码:

```
<?

class URLRewrite {

    var $_query_string;

    /**

     * 构造函数

     *

     */

    function URLRewrite() {

        $this->_query_string = str_replace('.html', '', $_SERVER["QUERY_STRING"]);

    }

    /**

     * 返回模拟 Rewirte 形态页面传递字符串

     *

     */

    function GetRewriteString() {

        if (!strpos($this->_query_string, '&')) return $this->_query_string;

        $URLArray = explode('&', $this->_query_string);

        for ($i=0; $i<count($URLArray); $i++) {

            if (strpos($URLArray[$i], '=')+1 != strlen($URLArray[$i]))

                $ParaArray[] = str_replace('=', '/', $URLArray[$i]);

        }

        return implode('/', $ParaArray);

    }

    /**

     * 返回模拟 Rewirte 形态的 URL 地址

     *

     */

    function GetRewriteUrl() {

        return $_SERVER['PHP_SELF'] . '?' . $this->GetRewriteString() . '.html';

    }

    /**

     * 返回原始形态页面传递字符串

     *

     */

    function GetOriginalString() {

        if (!strpos($this->_query_string, '/')) return $this->_query_string;

        $URLArray = explode('/', $this->GetRewriteString($this->_query_string));

        for ($i=0; $i<count($URLArray); $i=$i+2) {

            if ($URLArray[$i] && $URLArray[$i+1])

                $ParaArray[] = $URLArray[$i] . '=' . $URLArray[$i+1];

        }

        return implode('&', $ParaArray);

    }

    /**

     * 返回原始形态的 URL 地址

     *

     */

    function GetOriginalUrl() {

        return $_SERVER['PHP_SELF'] . '?' . $this->GetOriginalString();

    }

    /**

     * 将解析的参数写入到 $_GET 预定义变量

     *

     */

    function ParseUrl() {

        $URLArray = explode('/', $this->GetRewriteString());

        $_GET = '';

        for ($i=0; $i<count($URLArray); $i=$i+2) {

            $_GET[$URLArray[$i]] = $URLArray[$i+1];

        }

        $_SERVER["QUERY_STRING"] = $this->GetOriginalString();

        $_SERVER["R_QUERY_STRING"] = $this->GetRewriteString();

    }

}
```


测试一下使用后的效果： 

*复制内容到剪贴板*
##### 代码:

```
include_once ('url_rewrite.class.php');

        $ReURL = new URLRewrite();

        $ReURL->ParseUrl();

        echo '$_SERVER["QUERY_STRING"]：' . $_SERVER["QUERY_STRING"] . '<br>';

        echo '$_SERVER["R_QUERY_STRING"]：' . $_SERVER["R_QUERY_STRING"] . '<br>';

        echo '原始形态的URL地址：' . $ReURL->GetOriginalUrl() . '<br>';

        echo '模拟形态的URL地址：' . $ReURL->GetRewriteUrl() . '<br>';

        echo '--------------------------------------------------------------------------------';

        echo '$_GET[/'a/'] = ' . $_GET['a'] . '<br>';

        echo '$_GET[/'b/'] = ' . $_GET['b'] . '<br>';

        echo '$_GET[/'c/'] = ' . $_GET['c'] . '<br>';

        echo '$_GET[/'d/'] = ' . $_GET['d'] . '<br>';
```


URL地址为test.php?a=haha&b=hehe&c=&d=hoho时的显示结果:



##### 引用:

> $_SERVER["QUERY_STRING"]：a=haha&b=hehe&d=hoho

$_SERVER["R_QUERY_STRING"]：a/haha/b/hehe/d/hoho

原始形态的URL地址：test.php?a=haha&b=hehe&d=hoho

模拟形态的URL地址：test.php?a/haha/b/hehe/d/hoho.html

--------------------------------------------------------------------------------

$_GET['a'] = haha

$_GET['b'] = hehe

$_GET['c'] = 

$_GET['d'] = hoho


URL地址为test.php?a/haha/b/hehe/d/hoho时的显示结果:



##### 引用:

> $_SERVER["QUERY_STRING"]：a=haha&b=hehe&d=hoho

$_SERVER["R_QUERY_STRING"]：a/haha/b/hehe/d/hoho

原始形态的URL地址：test.php?a=haha&b=hehe&d=hoho

模拟形态的URL地址：test.php?a/haha/b/hehe/d/hoho.html

--------------------------------------------------------------------------------

$_GET['a'] = haha

$_GET['b'] = hehe

$_GET['c'] = 

$_GET['d'] = hoho



# 超详细的 NGINX URL重写实例讲解

[http://www.jbxue.com/article/2187.html](http://www.jbxue.com/article/2187.html)



# nginx配置url重写及自定义404错误页面等

[http://www.jbxue.com/article/3837.html](http://www.jbxue.com/article/3837.html)












