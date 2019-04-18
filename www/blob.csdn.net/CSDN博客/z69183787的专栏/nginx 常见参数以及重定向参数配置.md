# nginx 常见参数以及重定向参数配置 - z69183787的专栏 - CSDN博客
2017年09月19日 18:52:49[OkidoGreen](https://me.csdn.net/z69183787)阅读数：5346
**nginx 各参数翻译，作用**
**$arg_PARAMETER #这个变量包含GET请求中，如果有变量PARAMETER时的值。**
**$args #这个变量等于请求行中(GET请求)的参数，例如foo=123&bar=blahblah;**
**$binary_remote_addr #二进制的客户地址。**
**$body_bytes_sent #响应时送出的body字节数数量。即使连接中断，这个数据也是精确的。**
**$content_length #请求头中的Content-length字段。**
**$content_type #请求头中的Content-Type字段。**
**$cookie_COOKIE #cookie COOKIE变量的值**
**$document_root #当前请求在root指令中指定的值。**
**$document_uri #与$uri相同。**
**$host #请求主机头字段，否则为服务器名称。**
**$hostname #Set to the machine’s hostname as returned by gethostname**
**$http_HEADER**
**$is_args #如果有$args参数，这个变量等于”?”，否则等于”"，空值。**
**$http_user_agent #客户端agent信息**
**$http_cookie #客户端cookie信息**
**$limit_rate #这个变量可以限制连接速率。**
**$query_string #与$args相同。**
**$request_body_file #客户端请求主体信息的临时文件名。**
**$request_method #客户端请求的动作，通常为GET或POST。**
**$remote_addr #客户端的IP地址。**
**$remote_port #客户端的端口。**
**$remote_user #已经经过Auth Basic Module验证的用户名。**
**$request_completion #如果请求结束，设置为OK. 当请求未结束或如果该请求不是请求链串的最后一个时，为空(Empty)。**
**$request_method #GET或POST**
**$request_filename #当前请求的文件路径，由root或alias指令与URI请求生成。**
**$request_uri #包含请求参数的原始URI，不包含主机名，如：”/foo/bar.php?arg=baz”。不能修改。**
**$scheme #HTTP方法（如http，https）。**
**$server_protocol #请求使用的协议，通常是HTTP/1.0或HTTP/1.1。**
**$server_addr #服务器地址，在完成一次系统调用后可以确定这个值。**
**$server_name #服务器名称。**
**$server_port #请求到达服务器的端口号。**
**$uri #不带请求参数的当前URI，$uri不包含主机名，如”/foo/bar.html”。该值有可能和$request_uri 不一致。$request_uri是浏览器发过来的值。该值是rewrite后的值。例如做了internal redirects后。**
**今天在给某网站写rewrite重定向规则时，碰到了这个关于重定向的参数处理问题。默认的情况下，Nginx在进行rewrite后都会自动添加上旧地址中的参数部分，而这对于重定向到的新地址来说可能是多余。虽然这也不会对重定向的结果造成多少影响，但当你注意到新地址中包含有多余的“?xxx=xxx”时，心里总还是会觉得不爽。那么该如何来处理这部分的内容呢？看了下面两个简单的例子你就会明白了。**
例如：
把http://example.com/test.php?para=xxx 重定向到 http://example.com/new
若按照默认的写法：rewrite ^/test.php(.*) /new permanent;
重定向后的结果是：http://example.com/new?para=xxx
如果改写成：rewrite ^/test.php(.*) /new? permanent;
那结果就是：http://example.com/new
**所以，关键点就在于“？”这个尾缀。假如又想保留某个特定的参数，那又该如何呢？可以利用Nginx本身就带有的$arg_PARAMETER参数来实现。**
例如：
把http://example.com/test.php?para=xxx&p=xx重写向到 http://example.com/new?p=xx
可以写成：rewrite  ^/test.php   /new?p=$arg_p?  permanent;
**只求结果的朋友可以直接忽略前面的内容，看这里：**
> 
**rewrite  ^/test.php  /new  permanent;       //重写向带参数的地址**
**rewrite  ^/test.php  /new?  permanent;      //重定向后不带参数**
**rewrite  ^/test.php   /new?id=$arg_id?  permanent;    //重定向后带指定的参数**
**permanent是永久重定向参数，根据需要去掉也可以，不过最好是带有。**
**参考301重定向与302重定向的区别**
