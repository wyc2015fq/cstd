# （Nginx和PHP下）URL重写，TP实现URL重写 - 在思索中前行！ - CSDN博客





2014年09月25日 22:31:43[_Tham](https://me.csdn.net/txl16211)阅读数：10142








## ngnix中添加网站重定向

## 由于域名没备案所以不能使用隐性url直接域名转接到另一个网站，所以迫于无奈只能在Vhost上动手脚了，原理是先访问另一个网站，然后由这个网站做转发。

## 方法很简单，找到nginx的conf文件，目录一般在/usr/local/nginx/conf。如果你没有做域名识别，这个文件就在conf文件夹下的nginx.conf，如果做了多域名识别处理，conf文件就在vhost目录下，打开之后，添加一行

rewrite
 "^/(.*)$" 要跳转的域名/$1 break;


![](http://4gml.com.cn/wp-content/uploads/2017/05/1.jpg)


这样所有的访问都会转发到要跳转到的域名了。





## UrlRewrite就是我们通常说的地址重写，用户得到的全部都是经过处理后的URL地址。

## 优点

## 一：提高安全性，可以有效的避免一些参数名、ID等完全暴露在用户面前，如果用户随便乱输的话，不符合规则的话直接会返回个404或错误页面，这比直接返回500或一大堆服务器错误信息要好的多



二：美化URL，去除了那些比如*.do之类的后缀名、长长的参数串等，可以自己组织精简更能反映访问模块内容的URL

三：更有利于搜索引擎的收入，通过对URL的一些优化，可以使搜索引擎更好的识别与收录网站的信息

### 使用范围

**地址重写一般是用于将动态地址伪静态**。如果本身就是静态就没必要了。地址重写后网站制作者可以通过输入地址名直接访问。

## Thinkphp的.htaccess文件配置URL的一些重写规则


在thinkphp的案例中有一个.htaccess文件，里面配置了URL的一些重写规则，如：

<IfModule mod_rewrite.c>

  RewriteEngine on

  RewriteCond %{REQUEST_FILENAME} !-d

  RewriteCond %{REQUEST_FILENAME} !-f

  RewriteRule ^(.*)$ index.php/$1 [QSA,PT,L]

</IfModule>

它的作用就是设置URL重写以隐藏URL中含有的index.php。一般来说URL过长或者动态化的URL都不利于SEO因此隐藏的目的就是要达到更好的SEO效果。

明白了.htaccess文件的作用，接下来就是让它工作起来。

要使.htaccess文件起作用，通常需要服务器开启URL_REWRITE模块才能支持。

下面是Apache的配置过程：

**1、httpd.conf配置文件中加载了mod_rewrite.so模块**    在httpd.conf配置文件搜索 LoadModule rewrite_module modules/mod_rewrite.so (Apache2是这个)去掉前面的#

**2、AllowOverride None 将None改为 All**    在httpd.conf配置文件找到“AllowOverride None”将None改为 All。这点值得注意的是,“AllowOverride None”在文件中能找到几处，但要改的却只有一处。如图所示：

AllowOverride All

![](http://www.w3note.com/Public/ueditor/php/upload/18261351601841.gif)

**3、确保项目(一般是前台项目)配置文件的URL_MODEL设置为2**

**4、把.htaccess文件放到入口文件的同级目录下**



### [ThinkPHP 利用.htaccess文件的 Rewrite 规则隐藏URL中的 index.php](http://blog.csdn.net/aoyoo111/article/details/6686085)实现

### 去掉 URL 中的 index.php




ThinkPHP 作为 PHP 框架，是单一入口的，那么其原始的 URL 便不是那么友好。但 ThinkPHP 提供了各种机制来定制需要的 URL 格式，配合 Apache .htaccess 文件，更是可以定制出人性化的更利于 SEO 的 URL 地址来。


.htaccess文件是 Apache 服务器中的一个配置文件，它负责相关目录下的网页配置。我们可以利用 .htaccess 文件的 Rewrite 规则来隐藏掉 ThinkPHP URL 中的 index.php 文件（即入口文件），这也是 ThinkPHP URL 伪静态的第一步。


例如原来的 URL 为：
http://127.0.0.1/index.php/Index/insert


去掉 index.php 之后变为：
http://127.0.0.1/Index/insert


如此一来，就变成了 http://服务器地址/应用模块名称/操作名称[/变量参数] 的常见 URL 格式。

## 更改 Apache httpd.conf 配置文件


提示：如果在虚拟主机商配置，请直接配置第三、四步，因为支持 .htaccess 的空间已经配置好了前面两步。


用编辑器打开 Apache 配置文件 httpd.conf（该文件位于 Apache 安装目录Apache2conf），并按如下步骤修改，。

### 一、加载了 mod_rewrite.so


确认加载了 mod_rewrite.so 模块（将如下配置前的 # 号去掉）：
LoadModule rewrite_module modules/mod_rewrite.so

### 二、更改 AllowOverride 配置


更改需要读取 .htaccess 文件的目录，将原来的目录注释掉：
#<Directory "C:/Program Files/Apache Group/Apache2/htdocs">
<Directory E:/html/myapp>
更改 AllowOverride None 为 AllowOverride FileInfo Options ，更改后的配置如下所示：#<Directory "C:/Program Files/Apache Group/Apache2/htdocs">
<Directory E:/html/myapp>
    AllowOverride FileInfo Options
</Directory>


.htaccess 是基于目录来控制的，<Directory E:/html/myapp> 该句即表示需要读取 .htaccess 文件的目录，要根据实际具体 Apache 的解析目录来配置。虚拟主机如果提供 .htaccess 控制，一般都已经配置好了。

### 三、添加 .htaccess 文件 Rewrite 规则


在需要隐藏 index.php 的目录下（本教程中为 E:/html/myapp，也即入口文件所在目录）创建 .htaccess 文件，并写入如下规则代码：
<IfModule mod_rewrite.c>
RewriteEngine on

#不显示index.php

RewriteCond %{REQUEST_FILENAME} !-d
RewriteCond %{REQUEST_FILENAME} !-f
RewriteRule ^(.*)$ index.php/$1 [QSA,PT,L]

</IfModule>


如果网站已经有 .htaccess 文件，在里面添加该段配置规则即可。如果不能创建该文件（Windows 平台不能创建），可以从本站下载该文件，但该文件仅配置了隐藏 index.php 的规则，[点击此处下载](http://www.5idev.com/Public/download/server/htaccess.rar)。

### 四、更改项目配置文件


编辑项目配置文件 Conf/config.php ，将 URL 模式配置为 2（Rewrite模式）：
    'URL_MODEL'=>2,


至此，各个配置已经完成。保存各配置文件后，重启 Apache 服务器并删除 Runtime 目录下的项目缓存文件，在浏览器访问隐藏 index.php 后的地址测试是否成功：
http://127.0.0.1/html/myapp/Index/index


如果访问成功，那么利用 Apache .htaccess 文件的 Rewrite 规则隐藏 index.php 入口文件的配置就成功了。





## **thinkphp框架在nginx环境下去掉index.php路径显示**


将apache下的一个网站迁移到nginx环境中，结果发现网站用的ThinkPHP框架做的开发，默认用的pathinfo。

这是一个很头疼的问题，因为nginx不支持pathinfo，贸然一并打开也担心不安全。

于是查询资料后整理如下：

找到applications/Conf/的配置文件 config.php

return array(

'URL_MODEL'=>2,  //关于URL更多说明请参考Tinkphp/Common/convention.php

);

复制代码

配置 Nginx.conf

在你的虚拟主机下添加

location / {

      if (!-e $request_filename){

           rewrite ^/(.*)$ /index.php/$1 last;

      }

}
**复制代码如果你的项目入口文件在一个子目录内则  // 这一段重写规则相当重要，什么意思呢？**

**location /目录/ {      if (!-e $request_filename){           rewrite ^/目录/(.*)$ /目录/index.php/$1 last;      }}**


.abu.点评：

测试成功，注意Tinkphp/Common/convention.php

和网站下的applications/Conf/config.php

两个文件 都要修改。

主要就是改成模式2，rewrite方式就可以了。





