# Apache URLRewrite 原理及配置实现 - 在思索中前行！ - CSDN博客





2014年10月13日 22:39:43[_Tham](https://me.csdn.net/txl16211)阅读数：3731








       看一下网站上的一些 URL。您是否发现一些类似于 [http://yoursite.com/info/dispEmployeeInfo.
 ... 99&type=summary](http://yoursite.com/info/dispEmployeeInfo.aspx?EmpID=459-099&type=summary%C2%A0)的 URL？或者，您可能将一系列网页从一个目录或网站移动到另一个目录或网站，结果导致已将旧 URL 用作书签的访问者断开链接。在本文中，我们将了解如何通过将 [http://yoursite.com/info/dispEmployeeInfo.
 ... 99&type=summary](http://yoursite.com/info/dispEmployeeInfo.aspx?EmpID=459-099&type=summary%C2%A0)替换为类似于 [http://yoursite.com/people/sales/chuck.smith](http://yoursite.com/people/sales/chuck.smith%C2%A0)的网址，使用 URL 重写将那些冗长的 URL 缩写为富有意义且容易记忆的 URL。我们还将了解如何将 URL 重写用于创建智能 404 错误。
        URL 重写是截取传入 Web 请求并自动将请求重定向到其他资源的过程。执行 URL 重写时，通常会检查被请求的 URL，并基于 URL 的值将请求重定向到其他 URL。例如，在进行网站重组而将 /people/ 目录下的所有网页移动到 /info/employees/ 目录中时，您可能希望使用 URL 重写来检查 Web 请求是否指向了 /people/ 目录中的文件。如果请求指向 /people/ 目录中的文件，您可能希望自动将请求重定向到 /info/employees/ 目录中的同一文件。

## URLRewrite原理

mod_rewrite是Apache的一个非常强大的功能，它可以实现伪静态页面。下面我详细说说它的使用方法！对初学者很有用的哦！

### 1.检测Apache是否支持mod_rewrite

通过php提供的phpinfo()函数查看环境配置，通过Ctrl+F查找到“Loaded Modules”，其中列出了所有apache2handler已经开启的模块，如果里面包括“mod_rewrite”，则已经支持，不再需要继续设置。

如果没有开启“mod_rewrite”，则打开目录 您的apache安装目录“/apache/conf/” 下的 httpd.conf 文件，通过Ctrl+F查找到“LoadModule rewrite_module”，将前面的”#”号删除即可。

如果没有查找到，则到“LoadModule” 区域，在最后一行加入“LoadModule rewrite_module modules/mod_rewrite.so”（必选独占一行），然后重启apache服务器即可。

### 2.让apache服务器支持.htaccess

如 何让自己的本地APACHE服务器支持”.htaccess”呢?其实只要简单修改一下apache的httpd.conf设置就可以让 APACHE支 持.htaccess了。打开httpd.conf文件(在那里? APACHE目录的CONF目录里面),用文本编辑器打开后,查找

Options FollowSymLinks

AllowOverride None

改为

Options FollowSymLinks

AllowOverride All

就可以了。

### 3.建立.htaccess 文件

如 果是在windows平台下，刚开始还真不知道怎么建立”.htaccess”文件，因为这个文件实际上没有文件名，仅仅只有扩展名，通过普通方 式是无法建立这个文件的，别着急，马上告诉你三种方法：三种方法都是先建立一个htaccess.txt的文本文件（当然，这个文本文件的名字你可以随便 取），然后有三种方式给这个文件重命名：

（1）用记事本 打开，点击文件–另存为，在文件名窗口输入”.htaccess”，注意是整个绿色部分，也就是包含英文引号，然后点击保存就行了。

（2）进入cmd命令 窗口，通过cd切换当刚建立htaccess.txt文件的文件夹，然后输入命令：rename htaccess.txt .htaccess ，然后点击键盘Enter键即可。

（3）通过ftp连接htaccess.txt所在文件夹，通过ftp软件重命名。

### 4.rewrite规则学习

我们新建一个.htaccess文件之后，就在里面写入以下内容：

RewriteEngine on #rewriteengine为重写引擎开关on为开启off为关闭

RewriteRule ([0-9]{1,})$ index.php?id=$1

我讲解一下RewriteRule：RewriteRule是重写规则，支持正则表达式的，上面的([0-9]{1,})是指由数字组成的，$是结束标志，说明是以数字结束！

好吧，现在我们可以实现伪静态页面了，写下一下的规则：

<IfModule mod_rewrite.c>

RewriteEngine on

RewriteRule([a-zA-Z]{1,})-([0-9]{1,}).html$index.php?action=$1&id=$2

</IfModule>


([a-zA-Z]{1,})-([0-9]{1,}).html$是规则，index.php?action=$1&id=$2是要替换的格式，$1代表第一个括号匹配的值，$2代表第二个，如此类推！！

我们写一个处理的PHP脚本：

index.php


PHP代码
<?php
echo ‘你的Action是：’ . $_GET['action'];
echo ‘<br/>’;
echo ‘你的ID是：’ . $_GET['id'];
?>

好了，我们现在在浏览器中输入：

localhost/view-12.html

输出的是：

你的Action是：view

你的ID是：12

＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝一道华丽的分割线＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝

[13 个mod_rewrite 应用举例](http://www.cnblogs.com/analyzer/articles/1581929.html)

1．给子域名加www标记 
RewriteCond %{HTTP_HOST} ^([a-z.]+)?example\.com$ [NC] 

RewriteCond %{HTTP_HOST} !^www\. [NC] 

RewriteRule .? http://www.%1example.com%{REQUEST_URI} [R=301,L] 
这个规则抓取二级域名的%1变量，如果不是以www开始，那么就加www，以前的域名以及{REQUEST_URI}会跟在其后。

2．去掉域名中的www标记 
RewriteCond %{HTTP_HOST} !^example\.com$ [NC] 

RewriteRule .? [http://example.com](http://example.com/)%{REQUEST_URI} [R=301,L]

3．去掉www标记，但是保存子域名 
RewriteCond %{HTTP_HOST} ^www\.(([a-z0-9_]+\.)?example\.com)$ [NC] 

RewriteRule .? http://%1%{REQUEST_URI} [R=301,L]

这里，当匹配到1%变量以后，子域名才会在%2（内部原子）中抓取到，而我们需要的正是这个%1变量。 

4．防止图片盗链 
一些站长不择手段的将你的图片盗链在他们网站上，耗费你的带宽。你可以加一下代码阻止这种行为。 
RewriteCond %{HTTP_REFERER} !^$ 

RewriteCond %{HTTP_REFERER} !^http://(www\.)?example\.com/ [NC] 

RewriteRule \.(gif|jpg|png)$ - [F] 
如果{HTTP_REFERER}值不为空，或者不是来自你自己的域名，这个规则用[F]FLAG阻止以gif|jpg|png 结尾的URL 

如果对这种盗链你是坚决鄙视的，你还可以改变图片，让访问盗链网站的用户知道该网站正在盗用你的图片。 
RewriteCond %{HTTP_REFERER} !^$ 

RewriteCond %{HTTP_REFERER} !^http://(www\.)?example\.com/.*$ [NC] 

RewriteRule \.(gif|jpg|png)$ 你的图片地址 [R=301,L] 
除了阻止图片盗链链接，以上规则将其盗链的图片全部替换成了你设置的图片。 

你还可以阻止特定域名盗链你的图片： 
RewriteCond %{HTTP_REFERER} !^http://(www\.)?leech_site\.com/ [NC] 

RewriteRule \.(gif|jpg|png)$ - [F,L]

这个规则将阻止域名黑名单上所有的图片链接请求。 

当然以上这些规则都是以{HTTP_REFERER}获取域名为基础的，如果你想改用成IP地址，用{REMOTE_ADDR}就可以了。

5．如果文件不存在重定向到404页面 
如果你的主机没有提供404页面重定向服务，那么我们自己创建。 
RewriteCond %{REQUEST_FILENAME} !-f 

RewriteCond %{REQUEST_FILENAME} !-d 

RewriteRule .? /404.php [L] 
这里-f匹配的是存在的文件名，-d匹配的存在的路径名。这段代码在进行404重定向之前，会判断你的文件名以及路径名是否存在。你还可以在404页面上加一个?url=$1参数： 
RewriteRule ^/?(.*)$ /404.php?url=$1 [L]

这样，你的404页面就可以做一些其他的事情，例如默认信心，发一个邮件提醒，加一个搜索，等等。

6．重命名目录

如果你想在网站上重命名目录，试试这个： 
RewriteRule ^/?old_directory/([a-z/.]+)$ new_directory/$1 [R=301,L]

在规则里我添加了一个“.”（注意不是代表得所有字符，前面有转义符）来匹配文件的后缀名。 

7．将.html后缀名转换成.php

前提是.html文件能继续访问的情况下，更新你的网站链接。 
RewriteRule ^/?([a-z/]+)\.html$ $1.php [L]

这不是一个网页重定向，所以访问者是不可见的。让他作为一个永久重定向（可见的），将FLAG修改[R=301,L]。 

8．创建无文件后缀名链接

如果你想使你的PHP网站的链接更加简洁易记－或者隐藏文件的后缀名，试试这个: 
RewriteRule ^/?([a-z]+)$ $1.php [L]

如果网站混有PHP以及HTML文件，你可以用RewriteCond先判断该后缀的文件是否存在，然后进行替换： 
RewriteCond %{REQUEST_FILENAME}.php -f 

RewriteRule ^/?([a-zA-Z0-9]+)$ $1.php [L] 

RewriteCond %{REQUEST_FILENAME}.html -f 

RewriteRule ^/?([a-zA-Z0-9]+)$ $1.html [L]

如果文件是以.php为后缀，这条规则将被执行。

9．检查查询变量里的特定参数

如果在URL里面有一个特殊的参数，你可用RewriteCond鉴别其是否存在： 
RewriteCond %{QUERY_STRING} !uniquekey= 

RewriteRule ^/?script_that_requires_uniquekey\.php$ other_script.php [QSA,L]

以上规则将检查{QUERY_STRING}里面的uniquekey参数是否存在，如果{REQUEST_URI}值为script_that_requires_uniquekey，将会定向到新的URL。 

10．删除查询变量

Apache的mod_rewrite模块会自动辨识查询变量，除非你做了以下改动： 

a).分配一个新的查询参数（你可以用[QSA,L]FLAG保存最初的查询变量） 

b).在文件名后面加一个“?”（比如index.php?）。符号“?”不会在浏览器的地址栏里显示。

11．用新的格式展示当前URI 

如 果这就是我们当前正在运行的URLs：/index.php?id=nnnn。我们非常希望将其更改成/nnnn并且让搜索引擎以新格式展现。首先，我们 为了让搜索引擎更新成新的，得将旧的URLs重定向到新的格式，但是，我们还得保证以前的index.php照样能够运行。是不是被我搞迷糊了？ 

实现以上功能，诀窍就在于在查询变量中加了一个访问者看不到的标记符“marker”。我们只将查询变量中没有出现“marker”标记的链接进行重定向，然后将原有的链接替换成新的格式，并且通过[QSA]FLAG在已有的参数加一个“marker”标记。以下为实现的方式： 
RewriteCond %{QUERY_STRING} !marker 

RewriteCond %{QUERY_STRING} id=([-a-zA-Z0-9_+]+) 

RewriteRule ^/?index\.php$ %1? [R=301,L] 

RewriteRule ^/?([-a-zA-Z0-9_+]+)$ index.php?marker &id=$1 [L]

这里，原先的URL：[http://www.example.com/index.php?id=nnnn](http://www.example.com/index.php?id=nnnn),不包含marker，所以被第一个规则永久重定向到[http://www.example.com/nnnn](http://www.example.com/nnnn)，第二个规则将[http://www.example.com/nnnn](http://www.example.com/nnnn)反定向到[http://www.example.com/index.php?marker](http://www.example.com/index.php?marker)&id=nnnn，并且加了marker以及id=nnnn两个变量，最后mod_rewrite就开始进行处理过程。

第二次匹配，marker被匹配，所以忽略第一条规则，这里有一个“.”字符会出现在[http://www.example.com/index.php?marker](http://www.example.com/index.php?marker) &id=nnnn中，所以第二条规则也会被忽略，这样我们就完成了。

注意，这个解决方案要求Apache的一些扩展功能，所以如果你的网站放于在共享主机中会遇到很多障碍。

12．保证安全服务启用

Apache可以用两种方法辨别你是否开启了安全服务，分别引用{HTTPS}和{SERVER_PORT}变量： 
RewriteCond %{REQUEST_URI} ^secure_page\.php$ 

RewriteCond %{HTTPS} !on 

RewriteRule ^/?(secure_page\.php)$ [https://www.example.com/](https://www.example.com/)$1 [R=301,L]

以上规则测试{REQUEST_URI}值是否等于我们的安全页代码，并且{HTTPS}不等于on。如果这两个条件同时满足，请求将被重定向到安全服务URI.另外你可用{SERVER_PORT}做同样的测试，443是常用的安全服务端口 
RewriteCond %{REQUEST_URI} ^secure_page\.php$ 

RewriteCond %{SERVER_PORT} !^443$ 

RewriteRule ^/?(secure_page\.php)$ [https://www.example.com/](https://www.example.com/)$1 [R=301,L]

13．在特定的页面上强制执行安全服务 
遇到同一个服务器根目录下分别有一个安全服务域名和一个非安全服务域名，所以你就需要用RewriteCond 判断安全服务端口是否占用，并且只将以下列表的页面要求为安全服务： 
RewriteCond %{SERVER_PORT} !^443$ 

RewriteRule ^/?(page1|page2|page3|page4|page5)$ [https://www.example.com/%1](https://www.example.com/%251)[R=301,L] 
以下是怎样将没有设置成安全服务的页面返回到80端口： 
RewriteCond %{ SERVER_PORT } ^443$ 

RewriteRule !^/?(page6|page7|page8|page9)$[http://www.example.com](http://www.example.com/)%{REQUEST_URI} [R=301,L]




## 5、UrlRewrite配置步骤



1：开启apache的url_rewrite模块，也就是在httpd.conf中去掉这句话的注释LoadModule rewrite_module modules/mod_rewrite.so

2：找到AllowOverride，把AllowOverride None修改成AllowOverride all（这个至观重要，不做这个设置的话就算做了其它的操作，也是无效的）

3：在所需要进行rewrite的web的主目录下添加.htaccess文件，添加上一句话：RewriteEngine on

4：再.htaccess下补充个人的重写规则即可

PS:最初我是省略了步骤2，所以一直配置都是无效，但是把规则写到httpd.conf中就有效了。今天找了很多资料看了步骤2是很重要，这个配置的含义就是让apache去读取对应的配置文件，.htaccess属于配置文件之一，如果设置成为了None就不让读取了，肯定就不得行了




