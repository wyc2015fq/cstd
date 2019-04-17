# CodeIgniter框架中关于URL重写（index.php）的二三事 - 在思索中前行！ - CSDN博客





2014年11月16日 15:55:37[_Tham](https://me.csdn.net/txl16211)阅读数：2419









      最近，在做自己的个人网站时，采用了轻量级的php框架CodeIgniter。乍一看上去，代码清晰简洁，MVC模型非常容易维护。开发时我采用的工具是Netbeans IDE 8.0，当然，本文的内容和开发工具是没有关系的，和我们最后网站采用的服务器有很大的关系。目前最为常用的两款免费web服务器是Apache和Nginx（这两款服务器的比较，可以参考一篇网上的经典文章：http://zyan.cc/nginx_php_v6/）。在我网站开发与上线的过程中，刚好两个服务器都用到了，他们配置CodeIgniter时稍有区别，接下来分别阐述一下，也为了让其他开发者在使用CodeIgniter框架时少踩坑。



# **（1）关于CodeIgniter**


CodeIgniter是一款开源的超轻量级MVC框架，常用于快速地Web开发中，如果你愿意，甚至可以随意地更改其中的源代码，以适应自己的需求。这个框架本身不想多做介绍，需要熟悉的朋友可以去CodeIgniter官方网站下载，并且官网也提供了非常详细的中文帮助手册，可以很好的帮助新人学习。帮助手册链接： http://codeigniter.org.cn/user_guide/toc.html 。帮助手册中从0开始教你搭建一个简单的网站。



# **（2）CodeIgniter架设的网站URL**


最开始CodeIgniter搭建的网站URL是这样子的：


http://[网站网址]/index.php/[controller class name]/[class function name]/[function parameter]


举个例子说明：http://127.0.0.1/index.php/welcome/hello/zhangsan。这个例子中，假设了网站的网址是127.0.0.1，也就是我们常用的本机地址，采用controller文件夹中的welcome.php这个用php编写的类处理这个url请求，具体处理方法是调用这个welcome类里面的hello函数，这个函数是需要一个参数的，我们传进去的参数是字符串zhangsan。不过有一点比较碍眼，就是url里面包含了一个固定的字段index.php，看着很不爽。我想要的结果是这样的：http://127.0.0.1/welcome/hello/zhangsan。那么如何把index.php去掉呢？这就是我想主要分享的地方。




在讲如何去掉index.php之前，我们先弄清楚它为什么会出现在这里。对于任意一个url请求，CodeIgniter都是先由位于网站跟目录下的index.php文件处理的，这个文件再根据你提供的url中index.php后面的部分来确定把这个请求重新交给哪一个类的哪一个函数处理。因此，url中必须包含index.php这个字段，显式的告诉服务器，这个url你先让index.php去重新定向到我后面指定的类去处理。如果不经过任何配置就直接把index.php去掉，你的网页是显示不出来的。那么我们如果要去掉它，就是希望经过一些配置选项，让服务器见到一个url默认地就用index.php去处理就好了，不用再把index.php显示在url里面。



# **（3）Apache服务器下去掉index.php**


我在自己电脑上开发时使用的是Apache服务器，因此不可避免地要先在Apache服务器下解决这个问题。其实，文章开始时提到的CodeIgniter官方帮助手册里面已经给了Apache下的解决办法，却没有给出Nginx下的解决办法。没办法，Apache服务器据说市场占有率已经超过60%，作为如此主流的服务器，官方手册还是有必要说明它的配置方法的。为了更加清晰一些，我再更加详细地说明一下。




在网站根目录下（也就是与前文提到的index.php同一个目录下），新建一个文件，名为.htaccess。注意不要忘了htaccess前面还有个点。用记事本打开这个文件，写入如下命令：

```
![复制代码](http://common.cnblogs.com/images/copycode.gif)

RewriteEngine on

RewriteCond %{REQUEST_FILENAME} !-f
RewriteCond %{REQUEST_FILENAME} !-d

RewriteCond $1 !^(index\.php|images|js|css|robots\.txt)
RewriteRule ^(.*)$ /index.php/$1 [L]

![复制代码](http://common.cnblogs.com/images/copycode.gif)
```


看一下英文单词Rewrite也猜到了，其实这个文件控制的就是url的重写规则。关于.htaccess文件的具体重写规则和全部种类的语法又是一门大学问，在这里就不详述了。我们只关心上面写的这几句话是啥意思。


第一句 RewriteEngine on：翻译过来就是“重写引擎 开启”，相当于启动url重写机制。


第二句 RewriteCond %{REQUEST_FILENAME} !-f：翻译过来就是“重写条件 请求文件名不是个文件”。最后一个字母f理解为file，!-f的感叹号表示否定，也就表示不是个file。


第三句 RewriteCond %{REQUEST_FILENAME} !-d：翻译过来就是“重写条件 请求文件名不是个目录”。最后一个字母d理解为directory。


第二句与第三句的用途是说，只有在你的URL请求不是文件的名字或者文件夹名字的时候，才启用重写规则。举例说明，比如http://127.0.0.1/test.html。这个请求其实就是在你的网站根目录下寻找test.html，如果找到了，就直接返回这个文件即可，不用重写；只有在根目录找不到test.html时，才重写这个url，这是REQUEST_FILENAME是个文件的例子。再比如http://127.0.0.1/nihao，这里nihao很有可能是根目录下的一个文件夹（当然也可以是一个没后缀的文件），这种情况下先寻找根目录有没有nihao这个文件夹，没有的话再启用url重写，这是REQUEST_FILENAME是个目录的例子。


第四句 RewriteCond $1 !^(index\.php|images|js|css|robots\.txt)：翻译过来就是“重写条件 url网址后的第一个参数不可以是index.php，images, css, js, robots.txt任何一个”。比如 http://127.0.0.1/images/girl.png，这个url第一个参数是images，这种情况下不要重写，只有不是以上列出来的那些时才重写。这句话目的是排除一些url请求的重写，因为我们经常会把网站的css文件，javascript文件，图片文件放在根目录下的css，js，images文件夹下，然后在网页中通过url引用这些资源，如果请求这些资源的url也被重写了，网页中就引用不到了。你可以根据自己的实际需求，新添加一些需要排除在外的重写url的情况。


第五句 RewriteRule ^(.*)$ /index.php/$1 [L]：翻译过来就是“重写规则 把url网址后第一个参数前面加上index.php”。[L]表示这是最后一条重写规则，后面没有了。


这样，你如果在浏览器输入http://127.0.0.1/hello/zhangsan，其实相当于是http://127.0.0.1/index.php/hello/zhangsan。


最后还有一个小坑，就是在使用工具开发网站的时候，往往我们网站代码并不在Apache服务器的根目录下，比如我们在Apache服务器的根目录下又建立一个文件夹xxx，并且把网站整个放在这个文件夹里面，那么我们的主页地址就是http://127.0.0.1/xxx/index.php。这时必须把上面配置文件第五句话改为RewriteRule ^(.*) /xxx/index.php/$1 [L]，还有一种改法是直接去掉第五句话中index.php前面的斜杠（即RewriteRule ^(.*)$ index.php/$1
 [L]），这一点千万注意！




完成了上面的.htaccess文件后，还有两件事情要做。


第一，找到CodeIgniter中的application/config/config.php文件，把index_page设为空值，即$config['index_page'] = '"";还有base_url设为网站根目录（index.php所在目录），$config['base_url']="http://127.0.0.1/xxx/"。部署到真正服务器上使网站上线前，不要忘了把127.0.0.1改成你网站的网址，如果index.php放在服务器根目录，也记得把base_url里这个xxx去掉。


第二，找到Apache的配置文件，也就是conf/httpd.conf这个文件，确保LoadModule rewrite_module modules/mod_rewrite.so前面的井号#已经去掉了。然后关键字搜索htaccess，找到配置.htaccess的部分，其设置应该改为AllowOverride All。其实如果你没有用很旧版本的Apache，井号和AllowOverride All默认就应该是设置好了的。这一步只是确认一下，不是这么配的要改成这样。




至此，Apache服务器下CodeIgniter的URL配置大功告成。现在index.php已经不需要出现在URL中了，系统会默认让index.php先去处理URL。



# **（4）Nginx服务器下去掉index.php**


上文中提到的Apache去掉index.php在官方帮助文档也有简要的说明，但是Nginx服务器就没有这么幸运了。我在网站开发时本地是Apache，但是网站上线时的服务器却是Nginx的，因此不得已又去网上搜索Nginx服务器下的配置，折腾了很久，试错试了很多次，总算把一个正确的版本试出来了，现在可以提供给大家参考。因为自己对于Nginx的配置没有深入研究过，所以先说明一下自己线上服务器用的环境，然后再展示Nginx配置的修改内容。以我目前的配置，亲测可以很好地工作，大家碰到了类似的问题可以按照我的配置尝试一下，不过我也不敢保证在您的系统上一定会奏效……我的线上服务器是买的XX云（避免广告~）服务器（自己从零开始搞一个服务器实在太麻烦了），系统配置好之后默认就是Nginx。操作系统采用的是Ubuntu
 12.04，Nginx版本是nginx/1.1.19。




看网上好多人的Nginx服务器默认配置文件是/etc/nginx/nginx.conf，我的也不例外。不过有个注意事项，有时候nginx.conf中会有一句include ***（另外一个文件），也就是引用外边某个文件的内容作为配置文件，这时候，如果你没有在nginx.conf中找到服务器server相关配置，不妨去它include的另外一个文件中找一下，我的就是这种情况。在我的配置文件中，和服务器有关的配置应该改成如下：

```
![复制代码](http://common.cnblogs.com/images/copycode.gif)

server {
        listen   80;

        root /usr/share/nginx/www;
        index index.php index.html index.htm;

        # Make site accessible from http://localhost/
        server_name localhost;

        location / {
                index index.php index.html index.htm;
                
                # Uncomment to enable naxsi on this location
                # include /etc/nginx/naxsi.rules

                # 请留意下面这条重写规则，和前面的Apache有些类似
                if (!-e $request_filename) { ##如果没有找到目标文件
                        rewrite ^/(.*)$ /index.php/$1 last;
                        break;
                }
　　　　　　　　　　
　　　　　　　    
　　　　　　　　　 # 上面的重写规则也可以改成下面这种形式，亲测两者都可行
　　　　　　　　　 # if ($request_filename !~ (js|styles|images|robots\.txt|index\.php.*)){ ##如果不是请求js，styles等文件
　　　　　　　　　 #        rewrite ^/(.*)$ /index.php/$1 last;
　　　　　　　　　 #        break;
　　　　　　　　　 # }
        }

        location /doc/ {
                alias /usr/share/doc/;
                autoindex on;
                allow 127.0.0.1;
                deny all;
        }
 
        location ~ \.php($|/) {
                fastcgi_split_path_info ^(.+\.php)(.*)$;
 
                fastcgi_pass 127.0.0.1:9000;
      
                fastcgi_index index.php;
                fastcgi_param PATH_INFO $fastcgi_path_info;
                fastcgi_param SCRIPT_FILENAME $document_root$fastcgi_script_name;
                include fastcgi_params;
        }

        # deny access to .htaccess files, if Apache's document root
        # concurs with nginx's one
        #
        location ~ /\.ht {
                deny all;
        }
}

![复制代码](http://common.cnblogs.com/images/copycode.gif)
```


具体的改动已经标注在上面的注释中了，很简单的一句重写规则，我却折腾了蛮久的时间。希望分享出来，帮助大家少踩坑。就写到这里吧！



