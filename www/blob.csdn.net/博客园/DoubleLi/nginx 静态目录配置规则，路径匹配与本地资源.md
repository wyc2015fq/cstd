# nginx 静态目录配置规则，路径匹配与本地资源 - DoubleLi - 博客园






经常配了nginx静态目录,死活访问不了,每次访问404.查看文档后,发现nginx配置静态目录使 
用以下规则 

假如nginx是在本机,静态目录也是在本机, 

1、子目录匹配 
如下配置 



Java代码  ![收藏代码](http://powertech.iteye.com/images/icon_star.png)

- location / {  
-     root /data/www;  
- }  




访问http://127.0.0.1/时,配匹配/data/www 
访问http://127.0.0.1/images时,配匹配/data/www/images 
访问http://127.0.0.1/images/1.jpg时,配匹配/data/www/images/1.jpg 
也就是说,地址栏里"/"后的路径是直接匹配目录data/www/下的路径 

而对于配置 



Java代码  ![收藏代码](http://powertech.iteye.com/images/icon_star.png)

- location /images/ {  
-     root /data/www;  
- }  



访问http://127.0.0.1/images时,配匹配/data/www/images 
也就是说,地址栏里/images,直接匹配了/data/www的子目录. 
**经常出问题的是,location里的url随意配了一个名字,如/xxx，但是对应的/data/www目录 下并没有该/data/www/xxx子目录，一访问就404**

2、重复路径匹配规则 

对于如下配置： 



Java代码  ![收藏代码](http://powertech.iteye.com/images/icon_star.png)

- server {  
-     location / {  
-         root /data/www;  
-     }  
- 
-     location /images/ {  
-         root /data;  
-     }  
- }  




访问URL http://localhost/images/example.png，将会匹配第二个/images/规则， 
虽然也可以匹配location /规则，但**nginx默认会选择最长前缀去匹配当前URL**，也就是 
第二个配置会生效，访问/data/images/目录，而不是/data/www/images/目录 


[Serving Static Content](http://nginx.org/en/docs/beginners_guide.html)




引用


Serving Static Content 

An important web server task is serving out files (such as images or static HTML pages). You will implement an example where, depending on the request, files will be served from different local directories: /data/www (which may contain HTML files) and /data/images (containing images). This will require editing of the configuration file and setting up of a server block inside the http block with two location blocks. 

First, create the /data/www directory and put an index.html file with any text content into it and create the /data/images directory and place some images in it. 

Next, open the configuration file. The default configuration file already includes several examples of the server block, mostly commented out. For now comment out all such blocks and start a new server block: 

    http { 
        server { 
        } 
    } 

Generally, the configuration file may include several server blocks distinguished by ports on which they listen to and by server names. Once nginx decides which server processes a request, it tests the URI specified in the request’s header against the parameters of the location directives defined inside the server block. 

Add the following location block to the server block: 

    location / { 
        root /data/www; 
    } 

This location block specifies the “/” prefix compared with the URI from the request. For matching requests, the URI will be added to the path specified in the root directive, that is, to /data/www, to form the path to the requested file on the local file system. If there are several matching location blocks nginx selects the one with the longest prefix. The location block above provides the shortest prefix, of length one, and so only if all other location blocks fail to provide a match, this block will be used. 

Next, add the second location block: 

    location /images/ { 
        root /data; 
    } 

It will be a match for requests starting with /images/ (location / also matches such requests, but has shorter prefix). 

The resulting configuration of the server block should look like this: 

    server { 
        location / { 
            root /data/www; 
        } 

        location /images/ { 
            root /data; 
        } 
    } 

This is already a working configuration of a server that listens on the standard port 80 and is accessible on the local machine at http://localhost/. In response to requests with URIs starting with /images/, the server will send files from the /data/images directory. For example, in response to the http://localhost/images/example.png request nginx will send the /data/images/example.png file. If such file does not exist, nginx will send a response indicating the 404 error. Requests with URIs not starting with /images/ will be mapped onto the /data/www directory. For example, in response to the http://localhost/some/example.html request nginx will send the /data/www/some/example.html file. 

To apply the new configuration, start nginx if it is not yet started or send the reload signal to the nginx’s master process, by executing: 

    nginx -s reload 

    In case something does not work as expected, you may try to find out the reason in access.log and error.log files in the directory /usr/local/nginx/logs or /var/log/nginx. 









