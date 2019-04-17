# nginx目录列表和目录访问权限设置 - DoubleLi - 博客园






**1.目录列表(directory listing)**

nginx让目录中的文件以列表的形式展现只需要一条指令

autoindex on;

autoindex可以放在location中，只对当前location的目录起作用。你也可以将它放在server指令块则对整个站点都起作用。或者放到http指令块，则对所有站点都生效。

下面是一个简单的例子:




|123456789101112|server{listen80;server_name  domain.com www.domain.com;access_log/var/...........................;root/path/to/root;location/{index  index.php index.html index.htm;}location/somedir{autoindex on;}}|
|----|----|



**2.nginx禁止访问某个目录**

跟Apache的Deny from all类似，nginx有deny all指令来实现。

禁止对叫dirdeny目录的访问并返回403 Forbidden，可以使用下面的配置:




|1234|location/dirdeny{deny all;return403;}|
|----|----|











