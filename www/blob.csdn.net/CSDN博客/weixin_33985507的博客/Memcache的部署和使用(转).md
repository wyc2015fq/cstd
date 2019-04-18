# Memcache的部署和使用(转) - weixin_33985507的博客 - CSDN博客
2016年02月18日 16:14:00[weixin_33985507](https://me.csdn.net/weixin_33985507)阅读数：3
一、memcache简介
Memcache是danga.com的一个项目，最早是为 LiveJournal 服务的，目前全世界不少人使用这个缓存项目来构建自己大负载的网站，来分担数据库的压力。
Memcache官方网站：[http://memcached.org/](http://memcached.org/)
二、memcache的安装
1、  下载源文件（目前最新稳定版本是memcached-1.4.5.tar.gz）
wget [http://memcached.googlecode.com/files/memcached-1.4.5.tar.gz](http://memcached.googlecode.com/files/memcached-1.4.5.tar.gz)
由于Memcache用到了libevent这个库用于Socket的处理，所以还需要安装libevent,
Libevent的官网为[http://www.monkey.org/~provos/libevent/](http://www.monkey.org/~provos/libevent/)，最新稳定版本为：1.4.14b
wget [http://www.monkey.org/~provos/libevent-1.4.14b-stable.tar.gz](http://www.monkey.org/~provos/libevent-1.4.14b-stable.tar.gz)
2、  安装memcache
1）、安装libevent
# tar xzvf libevent-1.4.14b-stable.tar.gz
# cd libevent-1.4.14b-stable
# ./configure --prefix=/usr
# make && make install
# cd ..
# ls -al /usr/lib | grep libevent （查看是否安装成功）
2）、安装memcache
# tar xzvf memcached-1.4.5.tar.gz
# cd memcached-1.4.5
# ./configure --with-libevent=/usr
# make && make install
# ls -al /usr/local/bin/mem*   (查看是否生成memcached可执行程序文件)
#备注：如果是64位的系统可能会报错，因为默认的库文件只会安装到/usr/lib下面
3、  启动memcache
# 通过/usr/local/bin/memcached –h查看命令选项帮助
[root@Centos2 memcached-1.4.5]# /usr/local/bin/memcached -h
memcached 1.4.5
-p <num>      TCP port number to listen on (default: 11211)
-U <num>      UDP port number to listen on (default: 11211, 0 is off)
-s <file>     UNIX socket path to listen on (disables network support)
-a <mask>     access mask for UNIX socket, in octal (default: 0700)
-l <ip_addr>  interface to listen on (default: INADDR_ANY, all addresses)
-d            run as a daemon
-r            maximize core file limit
-u <username> assume identity of <username> (only when run as root)
-m <num>      max memory to use for items in megabytes (default: 64 MB)
-M            return error on memory exhausted (rather than removing items)
-c <num>      max simultaneous connections (default: 1024)
#启动命令如下
# /usr/local/bin/memcached -d -m 10  -u root -l 172.28.5.2 -p 12000 -c 1024 –P /tmp/memcached.pid
####相关选项说明
-d   表示启动一个守护进程
-m 是分配给memcached使用的内存
-u   运行memcached的用户
-l     是memcached监听的ip
-p    是memcached监听的端口
-c    memcache运行的最大并发连接数
-P    是设置memcache的pid文件
三、Php的memcache扩展的安装
1、  windows平台memcache扩展的安装
下载相应版本的php_memcache.dll文件，然后将该文件放到D:\AppServ\php5\ext目录下（路径根据php安装的位置不同请自行设置），然后更改相应的php.ini的设置，创建<?php phpinfo();?>的php文件，访问这个文件看memcache模块是否起作用。
2、  linux平台memcache扩展的安装
wget [http://blog.s135.com/soft/linux/nginx_php/memcache/memcache-2.2.5.tgz](http://blog.s135.com/soft/linux/nginx_php/memcache/memcache-2.2.5.tgz)
tar zxvf memcache-2.2.5.tgz
cd memcache-2.2.5/
/usr/local/php/bin/phpize
./configure --with-php-config=/usr/local/php/bin/php-config
make
make install
cd ../
修改php.ini文件
查找/usr/local/php/etc/php.ini中的 extension_dir = "./"修改为extension_dir = "/usr/local/php/lib/php/extensions/no-debug-non-zts-20060613/"
并在此行后增加一行，然后保存：
extension = "memcache.so"
创建php文件aaa.php,内容如下：
<?php
Phpinfo();
?>
保存，然后访问看是否能显示memcache模块的信息！
四、Memcache的使用
//使用部分是转载的，原文地址：[http://blog.csdn.net/heiyeshuwu](http://blog.csdn.net/heiyeshuwu)
**[ ****接口介绍**** ]**
服务器端和客户端都安装配置好了，现在我们就来测试以下我们的成果。Memcache客户端包含两组接口，一组是面向过程的接口，一组是面向对象的接口，具体可以参考PHP手册 “LXXV. Memcache Functions” 这章。我们为了简单方便，就使用面向对象的方式，也便于维护和编写代码。Memcache面向对象的常用接口包括：
Memcache::connect -- 打开一个到Memcache的连接
Memcache::pconnect -- 打开一个到Memcache的长连接
Memcache::close -- 关闭一个Memcache的连接
Memcache::set -- 保存数据到Memcache服务器上
Memcache::get -- 提取一个保存在Memcache服务器上的数据
Memcache::replace -- 替换一个已经存在Memcache服务器上的项目（功能类似Memcache::set）
Memcache::delete -- 从Memcache服务器上删除一个保存的项目
Memcache::flush -- 刷新所有Memcache服务器上保存的项目（类似于删除所有的保存的项目）
Memcache::getStats -- 获取当前Memcache服务器运行的状态
**[ ****测试代码**** ]**
现在我们开始一段测试代码：
<?php
//连接
$mem = new Memcache;
$mem->connect("192.168.0.200", 12000);
//保存数据
$mem->set('key1', 'This is first value', 0, 60);
$val = $mem->get('key1');
echo "Get key1 value: " . $val ."
";
//替换数据
$mem->replace('key1', 'This is replace value', 0, 60);
$val = $mem->get('key1');
echo "Get key1 value: " . $val . "
";
//保存数组
$arr = array('aaa', 'bbb', 'ccc', 'ddd');
$mem->set('key2', $arr, 0, 60);
$val2 = $mem->get('key2');
echo "Get key2 value: ";
print_r($val2);
echo "
";
//删除数据
$mem->delete('key1');
$val = $mem->get('key1');
echo "Get key1 value: " . $val . "
";
//清除所有数据
$mem->flush();
$val2 = $mem->get('key2');
echo "Get key2 value: ";
print_r($val2);
echo "
";
//关闭连接
$mem->close();
//添加多台memcached服务器
$b = new Memcache(); 
$b->addServer("10.55.38.18",11271); 
$b->addServer("10.55.38.18",11272); 
$b->addServer("10.55.38.18",11273);
$arrHosts = array('10.10.10.11'=>'11211','10.10.10.12'=>'11211'); 
$mc = new Memcache(); 
echo serialize($mc)."\n"; 
foreach($arrHosts as $host=>$port) { 
echo 'start '.$host."\n"; 
$mc->connect($host,$port);
echo serialize($mc)."\n"; 
$stats = $mc->getStats(); 
echo $stats['pid']."\n";
}
?>
如果正常的话，浏览器将输出：
Get key1 value: This is first value
Get key1 value: This is replace value
Get key2 value: Array ( [0] => aaa [1] => bbb [2] => ccc [3] => ddd ) 
Get key1 value: 
Get key2 value:
基本说明我们的Memcache安装成功，我们再来分析以下上面的这段程序。
**[ ****程序分析**** ]**
初始化一个Memcache的对象：
$mem = new Memcache;
连接到我们的Memcache服务器端，第一个参数是服务器的IP地址，也可以是主机名，第二个参数是Memcache的开放的端口：
$mem->connect("192.168.0.200", 12000);
保存一个数据到Memcache服务器上，第一个参数是数据的key，用来定位一个数据，第二个参数是需要保存的数据内容，这里是一个字符串，第三个参数是一个标记，一般设置为0或者MEMCACHE_COMPRESSED就行了，第四个参数是数据的有效期，就是说数据在这个时间内是有效的，如果过去这个时间，那么会被Memcache服务器端清除掉这个数据，单位是秒，如果设置为0，则是永远有效，我们这里设置了60，就是一分钟有效时间：
$mem->set('key1', 'This is first value', 0, 60);
从Memcache服务器端获取一条数据，它只有一个参数，就是需要获取数据的key，我们这里是上一步设置的key1，现在获取这个数据后输出输出：
$val = $mem->get('key1');
echo "Get key1 value: " . $val;
现在是使用replace方法来替换掉上面key1的值，replace方法的参数跟set是一样的，不过第一个参数key1是必须是要替换数据内容的key，最后输出了：
$mem->replace('key1', 'This is replace value', 0, 60);
$val = $mem->get('key1');
echo "Get key1 value: " . $val;
同样的，Memcache也是可以保存数组的，下面是在Memcache上面保存了一个数组，然后获取回来并输出
$arr = array('aaa', 'bbb', 'ccc', 'ddd');
$mem->set('key2', $arr, 0, 60);
$val2 = $mem->get('key2');
print_r($val2);
现在删除一个数据，使用delte接口，参数就是一个key，然后就能够把Memcache服务器这个key的数据删除，最后输出的时候没有结果
$mem->delete('key1');
$val = $mem->get('key1');
echo "Get key1 value: " . $val . "
";
最后我们把所有的保存在Memcache服务器上的数据都清除，会发现数据都没有了，最后输出key2的数据为空，最后关闭连接
$mem->flush();
$val2 = $mem->get('key2');
echo "Get key2 value: ";
print_r($val2);
echo "
";
