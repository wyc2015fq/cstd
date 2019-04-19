# nginx模块指令 - BlueSky - CSDN博客
2018年04月16日 15:13:55[ASIA_kobe](https://me.csdn.net/ASIA_kobe)阅读数：267
转载：非常感谢两位大神！ 
[http://jinnianshilongnian.iteye.com/blog/2186448](http://jinnianshilongnian.iteye.com/blog/2186448)
[https://blog.csdn.net/imlsz/article/details/42297383](https://blog.csdn.net/imlsz/article/details/42297383)
Nginx共11个处理阶段，而相应的处理阶段是可以做插入式处理，即可插拔式架构；另外指令可以在http、server、server if、location、location if几个范围进行配置：
![这里写图片描述](https://img-blog.csdn.net/20180329211901380?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQveGllanVubmE=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)
================================================================
```bash
lua_code_cache
语法：lua_code_cache on | off
默认： on
适用上下文：http、server、location、location if
这个指令是指定是否开启lua的代码编译缓存，开发时可以设置为off，以便lua文件实时生效，如果是生产线上，为了性能，建议开启。
lua_package_path
语法：lua_package_path <lua-style-path-str>
默认：由lua的环境变量决定
适用上下文：http
设置lua代码的寻找目录。
例如：lua_package_path "/opt/nginx/conf/www/?.lua;;";
具体的路径设置要参考lua的模块机制
init_by_lua（_file）
语法：init_by_lua <lua-script-str>
适用上下文：http
 init_by_lua 'cjson = require "cjson"';
    server {
        location = /api {
            content_by_lua '
                ngx.say(cjson.encode({dog = 5, cat = 6}))
            ';
        }
    }
从这段配置代码，我们可以看出，其实这个指令就是初始化一些lua的全局变量，以便后续的代码使用。
注：有（_file）的选项代表可以直接引用外部的lua源代码文件，效果与直接写配置文件一样，不过可维护性当然是分开好点。
init_worker_by_lua(_file)
类似于上面的，不过是作用在work进程的，先于work进程启动而调用。
set_by_lua(_file)
语法：set_by_lua $res <lua-script-str> [$arg1 $arg2 ...]
适用上下文：server、location、location if
 location /foo {
        set $diff ''; # we have to predefine the $diff variable here
        set_by_lua $sum '
            local a = 32
            local b = 56
            ngx.var.diff = a - b;  -- write to $diff directly
            return a + b;          -- return the $sum value normally
        ';
        echo "sum = $sum, diff = $diff";
    }
这个指令是为了能够让nginx的变量与lua的变量相互作用赋值。
content_by_lua(_file)
语法：content_by_lua <lua-script-str>
适用上下文：location、location if
        location /nginx_var {
            # MIME type determined by default_type:
            default_type 'text/plain';
            # try access /nginx_var?a=hello,world
            content_by_lua "ngx.print(ngx.var['arg_a'], '\\n')";
        }
通过这个指令，可以由lua直接确定nginx响应页面的正文。
rewrite_by_lua(_file)
语法：rewrite_by_lua <lua-script-str>
适用上下文：location、location if
这个指令更多的是为了替代HttpRewriteModule的rewrite指令来使用的，优先级低于rewrite指令
比如
 location /foo {
          set $a 12; # create and initialize $a
          set $b ''; # create and initialize $b
          rewrite_by_lua 'ngx.var.b = tonumber(ngx.var.a) + 1';
          if ($b = '13') {
             rewrite ^ /bar redirect;
             break;
          }
         echo "res = $b";
    }
这个并不会像预期的那样子，因为我猜测，rewrite_by_lua是开启一个协程去工作的，可是下面却继续执行下去了，所以得不到预期的结果。
此时如果由lua代码来控制rewrite，那就没有问题了。
    location /foo {
        set $a 12; # create and initialize $a
        set $b ''; # create and initialize $b
        rewrite_by_lua '
            ngx.var.b = tonumber(ngx.var.a) + 1
            if tonumber(ngx.var.b) == 13 then
                return ngx.redirect("/bar");
            end
        ';
        echo "res = $b";
    }
access_by_lua(_lua)
语法：access_by_lua <lua-script-str>
适用上下文：http, server, location, location if
 location / {
        deny    192.168.1.1;
        allow   192.168.1.0/24;
        allow   10.1.1.0/16;
        deny    all;
        access_by_lua '
            local res = ngx.location.capture("/mysql", { ... })
            ...
        ';
        # proxy_pass/fastcgi_pass/...
    }
顾名思义，这个指令用在验证通过或者需要验证的时候。
header_filter_by_lua(_file)
语法：header_filter_by_lua <lua-script-str>
适用上下文：http, server, location, location if
    location / {
        proxy_pass http://mybackend;
        header_filter_by_lua 'ngx.header.Foo = "blah"';
    }
用lua的代码去指定http响应的 header一些内容。
body_filter_by_lua(_file)
语法：body_filter_by_lua <lua-script-str>
适用上下文：http, server, location, location if
   location /t {
        echo hello world;
        echo hiya globe;
        body_filter_by_lua '
            local chunk = ngx.arg[1]
            if string.match(chunk, "hello") then
                ngx.arg[2] = true  -- new eof
                return
            end
            -- just throw away any remaining chunk data
            ngx.arg[1] = nil
        ';
    }
这个指令可以用来篡改http的响应正文的。
```
