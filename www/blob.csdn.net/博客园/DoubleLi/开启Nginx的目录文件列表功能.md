# 开启Nginx的目录文件列表功能 - DoubleLi - 博客园






**ngx_http_autoindex_module  ****此模块用于自动生成目录列表，****ngx_http_autoindex_module****只在**** ngx_http_index_module****模块未找到索引文件时发出请求****.**

nginx默认是不允许列出整个目录的。

**开启目录列表：**
打开nginx.conf文件，在location server 或 http段中加入
 autoindex on;




http {    include       mime.types;    default_type  application/octet-stream;

    autoindex on;//自动显示目录    autoindex_exact_size off;//人性化方式显示文件大小否则以byte显示    autoindex_localtime on;//按服务器时间显示，否则以gmt时间显示

    server_names_hash_bucket_size 128;    client_header_buffer_size 32k;    large_client_header_buffers 4 32k;    client_max_body_size 8m;    limit_zone one $binary_remote_addr 32k;    sendfile        on;    tcp_nopush     on;    keepalive_timeout  60;    tcp_nodelay on;    gzip  on;    gzip_min_length  1k;    gzip_buffers     4 16k;    gzip_http_version 1.0;    gzip_comp_level 2;    gzip_types       text/plain application/x-javascript text/css application/xml;    gzip_vary on;    log_format  wwwlogs  '$remote_addr - $remote_user [$time_local] $request$status$body_bytes_sent$http_referer$http_user_agent$http_x_forwarded_for';    #include default.conf;    include vhost/*.conf;}





另外两个参数最好也加上去:
autoindex_exact_size off;
默认为on，显示出文件的确切大小，单位是bytes。
改为off后，显示出文件的大概大小，单位是kB或者MB或者GB
autoindex_localtime on;
默认为off，显示的文件时间为GMT时间。
 改为on后，显示的文件时间为文件的服务器时间

配置Nginx目录列表的方法详细参照:http://wiki.nginx.org/NginxChsHttpAutoindexModule

如果想希望目录列表支持header,footer则可以安装三方插件: [http://wiki.nginx.org/NginxNgxFancyIndex](http://wiki.nginx.org/NginxNgxFancyIndex)









