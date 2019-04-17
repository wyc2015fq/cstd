# NGINX conf 配置文件中的变量大全 可用变量列表及说明 - DoubleLi - 博客园





|$args #这个变量等于请求行中的参数。$content_length #请求头中的Content-length字段。$content_type #请求头中的Content-Type字段。$document_root #当前请求在root指令中指定的值。$host #请求主机头字段，否则为服务器名称。$http_user_agent #客户端agent信息$http_cookie #客户端cookie信息$limit_rate #这个变量可以限制连接速率。$request_body_file #客户端请求主体信息的临时文件名。$request_method #客户端请求的动作，通常为GET或POST。$remote_addr #客户端的IP地址。$remote_port #客户端的端口。$remote_user #已经经过Auth Basic Module验证的用户名。$request_filename #当前请求的文件路径，由root或alias指令与URI请求生成。$query_string #与$args相同。$scheme #HTTP方法（如http，https）。$server_protocol #请求使用的协议，通常是HTTP/1.0或HTTP/1.1。$server_addr #服务器地址，在完成一次系统调用后可以确定这个值。$server_name #服务器名称。$server_port #请求到达服务器的端口号。$request_uri #包含请求参数的原始URI，不包含主机名，如：”/foo/bar.php?arg=baz”。$uri #不带请求参数的当前URI，$uri不包含主机名，如”/foo/bar.html”。$document_uri #与$uri相同。|
|----|


类别： [无分类](http://www.lc365.net/blog/u/1/?dirid=0) |  [评论](http://www.lc365.net/blog/b/14455/#c)(4) |  [浏览](http://www.lc365.net/blog/b/14455/)(16431) |  收藏







##### 一共有 4 条评论
![](http://www.lc365.net/blog/image/upload/none.gif)

[Admin](http://www.lc365.net/blog/u/1/)2012年02月28日16时40分 16:40 评论:
在混合虚拟主机的记录Access.log中，定义日志格式时，需要使用 $http_host 来表示virtual server name （每个主机的域名）


![](http://www.lc365.net/blog/image/upload/none.gif)

[Admin](http://www.lc365.net/blog/u/1/)2011年12月06日21时38分 21:38 评论:
    map $args $foo {
        default    0;
        debug      1;
    }

这里首次用到了标准 ngx_map 模块的 map 配置指令，我们有必要在此介绍一下。map 在英文中除了“地图”之外，也有“映射”的意思。比方说，中学数学里讲的“函数”就是一种“映射”。而 Nginx 的这个 map 指令就可以用于定义两个 Nginx 变量之间的映射关系，或者说是函数关系。回到上面这个例子，我们用 map 指令定义了用户变量 $foo 与 $args 内建变量之间的映射关系。特别地，用数学上的函数记法 y = f(x) 来说，我们的 $args 就是“自变量” x，而 $foo 则是“因变量” y，即 $foo 的值是由 $args 的值来决定的，或者按照书写顺序可以说，我们将 $args 变量的值映射到了 $foo 变量上。

    现在我们再来看 map 指令定义的映射规则：

    map $args $foo {
        default    0;
        debug      1;
    } 

花括号中第一行的 default 是一个特殊的匹配条件，即当其他条件都不匹配的时候，这个条件才匹配。当这个默认条件匹配时，就把“因变量” $foo 映射到值 0. 而花括号中第二行的意思是说，如果“自变量” $args 精确匹配了 debug 这个字符串，则把“因变量” $foo 映射到值 1. 将这两行合起来，我们就得到如下完整的映射规则：当 $args 的值等于 debug 的时候，$foo 变量的值就是 1，否则 $foo 的值就为 0.


![](http://www.lc365.net/blog/image/upload/none.gif)

[Admin](http://www.lc365.net/blog/u/1/)2011年12月06日21时35分 21:35 评论:
也有一些内建变量是支持改写的，其中一个例子是 $args. 这个变量在读取时返回当前请求的 URL 参数串（即请求 URL 中问号后面的部分，如果有的话 ），而在赋值时可以直接修改参数串

$args 变量和 $arg_XXX 一样，也不再使用属于自己的存放值的容器。当我们读取 $args 时，Nginx 会执行一小段代码，从 Nginx 核心中专门存放当前 URL 参数串的位置去读取数据；而当我们改写 $args 时，Nginx 会执行另一小段代码，对相同位置进行改写。Nginx 的其他部分在需要当前 URL 参数串的时候，都会从那个位置去读数据，所以我们对 $args 的修改会影响到所有部分的功能。

对内建变量 $args 的修改会直接导致当前请求的 URL 参数串发生变化，因此内建变量 $arg_XXX 自然也会随之变化。

在读取变量时执行的这段特殊代码，在 Nginx 中被称为“取处理程序”（get handler）；而改写变量时执行的这段特殊代码，则被称为“存处理程序”（set handler）。不同的 Nginx 模块一般会为它们的变量准备不同的“存取处理程序”，从而让这些变量的行为充满魔法。


![](http://www.lc365.net/blog/image/upload/none.gif)

[Admin](http://www.lc365.net/blog/u/1/)2011年12月06日21时32分 21:32 评论:
由 ngx_http_core 模块提供的内建变量 $uri，可以用来获取当前请求的 URI（经过解码，并且不含请求参数），而 $request_uri 则用来获取请求最原始的 URI （未经解码，并且包含请求参数）。

另一个特别常用的内建变量其实并不是单独一个变量，而是有无限多变种的一群变量，即名字以 arg_ 开头的所有变量，我们估且称之为 $arg_XXX 变量群。一个例子是 $arg_name / $arg_id ... ，这个变量的值是当前请求名为 name 的 URI 参数的值，而且还是未解码的原始形式的值。

许多内建变量都是只读的，比如我们刚才介绍的 $uri 和 $request_uri. 对只读变量进行赋值是应当绝对避免的，因为会有意想不到的后果










