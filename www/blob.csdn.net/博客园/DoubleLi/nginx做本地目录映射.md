# nginx做本地目录映射 - DoubleLi - 博客园






有时候需要访问服务器上的一些静态资源，比如挂载其他设备上的图片到本地的目录，而本地的目录不在nginx根目录下，这个时候就需要简单的做一下目录映射来解决，比如想通过浏览器http://ip/image/2016/04/29/10/abc.jpg访问到系统目录/image_data/2016/04/29/10/abc.jpg需要在nginx.conf中添加location规则，配置如下：

```
location /image/ {
            root /;
            rewrite ^/image/(.*)$ /image_data/$1 break;
        }
```

　　location /image/设置服务器拦截含有/image/的请求，实际上范围根据实际情况确定精确度

　　root /;设定作用的根目录，rewrite就是真正的跳转规则，设置以image为开头的请求跳转到/image_data开头的根目录，后面的参数原封不动的添加过去，这样就实现了目录的映射，

　　使用上面的规则可以很方便的映射到本地指定的多个用于静态访问的目录

　　保存之后使用 `./nginx -s reload 重新载入配置文件，访问指定的url就访问到了`

　　另外还可以灵活配置精确代理，比如代理某一个访问路径，比如我们访问http://ip/abc/bcd/123.ico这时候我们映射到/web/host1/123.ico那么可以配置如下规则：

```
location /abc/bcd/123.ico {
        root /;
        rewrite ^/abc/bcd/123.ico$ /web/host1/123.ico break;
}
```

　　以上就实现了精确地路径代理，注意后面的/web/host1属于文件系统路径，而不是web主机的虚拟目录，因为上面root设置的是根目录，如果没有指定root那么以前面的server块中指定的全局root值为准，每个location块中属性的作用域只是这一个块中；另外可以代理多个路径可以通过web真实路径访问也可以使用代理路径访问，从而根据不同的业务需求设置代理，那么nginx本身就可以实现很多实用并且好用的功能









