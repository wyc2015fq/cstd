# 五、世界上最好的网站后端语言php - jiangjingxuan的博客 - CSDN博客





2017年01月25日 11:13:09[jiangjingxuan](https://me.csdn.net/jiangjingxuan)阅读数：1413












![](http://www.shareditor.com/uploads/media/default/0001/01/thumb_35_default_big.png)



php是一种实现动态网页的服务器端语言，具有简单、轻量、上手快的优点，而且可以和其他很多语言结合使用。有人说php是世界上最好的语言，虽然说得不对，但从php工程师的角度来讲确实是这样的，那么就让我们一起领略一下php的神奇之处。

请尊重原创，转载请注明来源网站[www.shareditor.com](http://www.shareditor.com)以及原始链接地址

## 环境准备工作



打开我们的centos 7虚拟机，确保nginx已经安装好并启动了服务（在前面的教程里有安装和启动方法）。下面我们来安装php-fpm（php和nginx之间的一个桥梁），执行：
`[root@centos7vm ~]# yum install php55w-fpm`
执行
`[root@centos7vm ~]# service php-fpm start`
启动php-fpm服务

修改nginx配置来让nginx把php代码转发给php-fpm服务解析，修改文件/etc/nginx/nginx.conf里的server组后如下：



```
location ~ \.php$ {
            root           /usr/share/nginx/php/;
            fastcgi_pass   127.0.0.1:9000;
            fastcgi_index  index.php;
            fastcgi_param  SCRIPT_FILENAME  $document_root$fastcgi_script_name;
            include        fastcgi_params;
        }
```

执行
`[root@centos7vm ~]# service nginx reload`
重新加载配置文件

顺便我们看一眼这个配置文件的其他部分来做一些解释：

```
error_log  /var/log/nginx/error.log
……
access_log  /var/log/nginx/access.log  main;
```

这些表示http访问日志的存放的地方
`include /etc/nginx/conf.d/*.conf;`
这一句表示你可以在conf.d目录下创建更多的网站配置文件（虚拟主机），nginx会自动加载进来

listen       80;表示监听http默认的80端口

root         /usr/share/nginx/html表示你所有网页文件存放的地方，nginx只会解析你放在这里的网页

location /.....表示当你访问http://www.shareditor.com/时服务器要怎样处理

location /path.....表示当你访问http://www.shareditor.com/path时服务器要怎样处理

```
location ~ \.php$ {
            root           /usr/share/nginx/php/;
            fastcgi_pass   127.0.0.1:9000;
            fastcgi_index  index.php;
            fastcgi_param  SCRIPT_FILENAME  $document_root$fastcgi_script_name;
            include        fastcgi_params;
        }
```

这几行表示当你访问http://www.shareditor.com/*.php时，服务器会把对应找到的php文件转发给本机的9000端口（就是php-fpm监听的端口）来解析，并把解析好的内容发回给你

OK，大功告成，现在可以开始php之旅了



## 一句代码看清楚一切



在/usr/share/nginx/php/目录下创建一个php文件：index.php，内容如下：

```php
<?php
phpinfo()
?>
```

这时打开浏览器访问127.0.0.1/index.php结果如何？

![](http://www.shareditor.com/uploads/media/my-context/0001/01/1508f9a47325c20bac3b564dc7a80726143d5d19.jpeg)

展示了php的信息

事实上这都是phpinfo这一句代码在起作用



ps：通过我本机的浏览器看虚拟机的网站为什么访问不了？

请尊重原创，转载请注明来源网站[www.shareditor.com](http://www.shareditor.com)以及原始链接地址

因为默认情况下centos 7会开启防火墙，导致外部ip无法访问，关闭防火墙方法如下：

```
[root@centos7vm php]# systemctl stop firewalld.service
[root@centos7vm php]# systemctl disable firewalld.service
```



假设你虚拟机ip地址是172.16.142.130那么在你主机浏览器里访问[http://172.16.142.130](http://172.16.142.130/)就可以啦



看到这我还是不会写php啊！！



下面让我们来看看php的语法有多么的简单，一分钟你就学会了。我们来用php开发这样一个网页，网页显示一组倒数的数字，如果是偶数就把样式做成h1标题，如果是奇数就做成h3，如下：

```php
<?php
$i=10;
for ($i=10; $i > 0; $i=$i-1)
{
    if ($i % 2 == 0)
    {
        echo "<h1>" . $i . "<h1>";
    }
    else
    {
        echo "<h3>" . $i . "<h3>";
    }
}
?>
```

整个效果是这样子的

![](http://www.shareditor.com/uploads/media/my-context/0001/01/6a3aad97846ba62448f87286b2c99d47fb3d6583.jpeg)



现在是不是明白了php的工作原理了呢？php其实就是按照普通编程语言的逻辑来动态输出html标签，让他看起来像个静态html文件



## php支持函数、类吗？



支持的，php函数像这样：

```php
<?php
function writeMsg() {
  echo "Hello world!";
}

writeMsg(); // 调用函数
?>
```



php的类像这样：



```php
class test
{ 
    var $b;
    function test() { $this->b=5; }
    function addab($c) { return $this->b+$c; }
}

$a = new test(); echo $a->addab(4); // 返回 9
```

有关php语言的更高级功能可以系统学习php教程，但个人觉得以上内容就足够了，遇到不会的直接百度




