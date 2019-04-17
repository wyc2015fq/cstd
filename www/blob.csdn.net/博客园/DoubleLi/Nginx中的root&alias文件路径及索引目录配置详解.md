# Nginx中的root&alias文件路径及索引目录配置详解 - DoubleLi - 博客园







这篇文章主要介绍了Nginx中的root&alias文件路径及索引目录配置,顺带讲解了root和alias命令的用法,需要的朋友可以参考下











**root&alias文件路径配置**nginx指定文件路径有两种方式root和alias，这两者的用法区别，使用方法总结了下，方便大家在应用过程中，快速响应。root与alias主要区别在于nginx如何解释location后面的uri，这会使两者分别以不同的方式将请求映射到服务器文件上。
[root]
语法：root path
默认值：root html
配置段：http、server、location、if
 [alias]
语法：alias path
配置段：location
实例：

location ~ ^/weblogs/ {
 root /data/weblogs/www.jb51.net;
 autoindex on;
 auth_basic      "Restricted";
 auth_basic_user_file passwd/weblogs;
}


如果一个请求的URI是/weblogs/httplogs/www.jb51.net-access.log时，web服务器将会返回服务器上的/data/weblogs/www.jb51.net/weblogs/httplogs/www.jb51.net-access.log的文件。
root会根据完整的URI请求来映射，也就是/path/uri。[
因此，前面的请求映射为path/weblogs/httplogs/www.jb51.net-access.log。

location ^~ /binapp/ { 
 limit_conn limit 4;
 limit_rate 200k;
 internal; 
 alias /data/statics/bin/apps/;
}


alias会把location后面配置的路径丢弃掉，把当前匹配到的目录指向到指定的目录。如果一个请求的URI是/binapp/a.jb51.net/favicon时，web服务器将会返回服务器上的/data/statics/bin/apps/a.jb51.net/favicon.jgp的文件。
1. 使用alias时，目录名后面一定要加"/"。
2. alias可以指定任何名称。
3. alias在使用正则匹配时，必须捕捉要匹配的内容并在指定的内容处使用。
4. alias只能位于location块中。

**索引目录配置**为了简单共享文件，有些人使用svn，有些人使用ftp，但是更多得人使用索引(index)功能。apache得索引功能强大，并且也是最常见得，nginx得auto_index实现得目录索引偏少,而且功能非常简单。先来看看我们得效果图。

![2016127174602759.jpg (874×251)](http://files.jb51.net/file_images/article/201601/2016127174602759.jpg?2016027174612)

nginx配置

    location ~ ^/2589(/.*)
    {
        autoindex on; //开启
        autoindex_localtime on;//开启显示功能
    }











