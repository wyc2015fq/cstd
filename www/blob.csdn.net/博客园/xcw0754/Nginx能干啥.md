# Nginx能干啥? - xcw0754 - 博客园
# [Nginx能干啥?](https://www.cnblogs.com/xcw0754/p/9845709.html)
Nginx能干的事很多，很强大。官方文档详见[点我](https://nginx.org/en/docs)。
### Nginx的命令行参数
```
-? | -h 打印帮助信息
-c file 读取指定配置文件，而不是默认的nginx.conf
-g directives 从命令行直接读取配置，取代nginx.conf
-p prefix 指定工作目录 (默认是/usr/local/nginx)
-q 配置测试期间，禁止输出非错误信息
-s signal 发送信号给worker进程，其中signal真实信号名，也可以是stop(强制退出)、quit(优雅退出)、reload(重新加载配置)、reopen(重新打开log文件)
-t 测试配置文件是否正确
-T 同上，但是会同时输出整个配置文件
-v 打印版本号
-V 打印更多配置/编译信息
```
Nginx进程pid默认在以下文件中:
`/usr/local/nginx/logs/nginx.pid`
发送信号给master进程以控制worker进程，详见[点我](https://nginx.org/en/docs/control.html)
### Nginx如何处理一个连接？
如果有如下配置，那连接被代理到这三个里边的哪一个处理？是根据HTTP头部的HOST字段来判断的，如果没有HOST字段，那就默认第一个配置来处理，除非指定了default_server关键字。如果想决绝掉所有不带HOST字段的请求，那就配置一个server_name为空字符串""，然后`return`一个444状态码。
```
server {
    listen      80;
    server_name example.org www.example.org;
    ...
}
server {
    listen      80;
    server_name example.net www.example.net;
    ...
}
server {
    listen      80;
    server_name example.com www.example.com;
    ...
}
```
如果有如下配置，首先检测的是ip，其次才是server_name。
```
server {
    listen      192.168.1.1:80;
    server_name example.org www.example.org;
    ...
}
server {
    listen      192.168.1.1:80;
    server_name example.net www.example.net;
    ...
}
server {
    listen      192.168.1.2:80;
    server_name example.com www.example.com;
    ...
}
```
如果有如下配置，nginx会优先选择最匹配的uri。`location /`就能匹配所有url，那么优先级就最低，如果访问/xyz.php，那就交给cgi处理，如果访问/xyz.png，那就到/data/www目录下找。如果访问的是`/`，那么在/data/www目录下找index.html文件，找不到则匹配index.php，也就是交给cgi处理，因为`location ~ \.php$`更匹配。
```
server {
    listen      80;
    server_name example.org www.example.org;
    root        /data/www;
    location / {
        index   index.html index.php;
    }
    location ~* \.(gif|jpg|png)$ {
        expires 30d;
    }
    location ~ \.php$ {
        fastcgi_pass  localhost:9000;
        fastcgi_param SCRIPT_FILENAME
                      $document_root$fastcgi_script_name;
        include       fastcgi_params;
    }
}
```
### server_name配置方法
如下配置，server_name 参数支持精确匹配、通配符、正则表达式。建议一行别写太多个参数，参数的个数是有限制的。
```
server {
    listen       80;
    server_name  example.org  www.example.org;
    ...
}
server {
    listen       80;
    server_name  *.example.org;
    ...
}
server {
    listen       80;
    server_name  mail.*;
    ...
}
server {
    listen       80;
    server_name  ~^(?<user>.+)\.example\.net$;
    ...
}
```
如果HTTP host匹配到多个server，则按如下顺序选择：
- 精确匹配
- 前置通配符，如*.example.com
- 后置通配符，如mai.*
- 首个正则表达式，根据在配置文件中出现的顺序
编写规则：
- 通配符出现的位置只允许在字符串的头部或尾部，不允许出现在中间，如`ww*.example.com`。一个比较特殊的字符串`.example.org`，可以匹配`example.org`和`*.example.org`。
- 正则表达式的开头必须是`~`，否则会被当做其他两种匹配方式。常用的有前置`^`和尾置`$`，`.`需要加转义字符`\`。
- 用到大括号`{}`时需要为字符串加上双引号，如`"~^(?<name>\w\d{1,3}+)\.example\.net$"`
如下配置，是server_name变量捕捉的例子。
```
server {
    server_name   ~^(www\.)?(?<domain>.+)$;
    location / {
        root   /sites/$domain;
    }
}
```
语法:
- `?<name>` Perl 5.10 compatible syntax, supported since PCRE-7.0
- `?'name'` Perl 5.10 compatible syntax, supported since PCRE-7.0
- `?P<name>` Python compatible syntax, supported since PCRE-4.0

