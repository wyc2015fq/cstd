# PHP相关系列 - Linux服务器-Memcache常用安装和使用 - 自娱自乐的代码人，专注Java、c、php - CSDN博客





2012年10月04日 18:01:42[initphp](https://me.csdn.net/initphp)阅读数：1436标签：[服务器																[linux																[function																[string																[缓存服务器](https://so.csdn.net/so/search/s.do?q=缓存服务器&t=blog)
个人分类：[PHP相关系列](https://blog.csdn.net/initphp/article/category/2609095)





## memcache安装前准备：

memcache安装前需要准备以下安装包：

1.memcache包：memcached-1.4.0.tar.gz

2.php扩展包：memcache-2.2.5.tgz



## 安装memcache：

将memcache安装到/usr/local/lib目录下面：

```
sudo  tar zxvf memcached-1.4.0.tar.gz 

cd memcached-1.4.0

sudo  ./configure --prefix=/usr/local/web/lib/memcache  --with-libevent 

sudo  make

sudo  make install
```




## 安装memcache的PHP扩展：

l安装扩展：

```
sudo  tar xzvf memcache-2.2.5.tgz

cd memcache-2.2.5 

sudo  /usr/local/web/php.5.3.3/bin/phpize 

sudo ./configure --enable-memcache --with-php-config=/usr/local/web/php.5.3.3/bin/php-config  --with-zlib-dir 

sudo  make

sudo  make install
```


修改php.ini配置文件，重启php-fpm：

```
sudo  vim /usr/local/web/php.5.3.3/lib/php.ini 

#加上：extension=memcache.so

sudo  kill  -USR2 `cat /usr/local/web/ php.5.3.3/var/run/php-fpm.pid` #重启
```




## 管理memcache：

l启动memcache

```
# export方式设置libevent库到路劲变量，memcache依赖libevent库

export LD_LIBRARY_PATH=$LD_LIBRARY_PATH:/usr/local/web/lib/libevent/lib/

cd  /usr/local/web/lib/memcache/bin/

memcached -d -m 1024 -u root -l 127.0.0.1 -p 11211 -c 128 -P /usr/local/web/lib/memcache/memcached.pid
```


l查看memcache进程：
`ps  -ef  |  grep  memcached`

停止memcache进程：
`sudo  kill   21039 `



## memcache具体使用：

```php
class memcachedInit {

       

       private $memcache;

              

       /**

        * Memcache缓存-设置缓存

        * 设置缓存key，value和缓存时间

        * @param  string $key   KEY值

        * @param  string $value 值

        * @param  string $time  缓存时间

        */

       public function set_cache($key, $value, $time = 0) { 

              return $this->memcache->set($key, $value, false, $time);

       }

       

       /**

        * Memcache缓存-获取缓存

        * 通过KEY获取缓存数据

        * @param  string $key   KEY值

        */

       public function get_cache($key) {

              return $this->memcache->get($key);

       }

       

       /**

        * Memcache缓存-清除一个缓存

        * 从memcache中删除一条缓存

        * @param  string $key   KEY值

        */

       public function clear($key) {

              return $this->memcache->delete($key);

       }

       

       /**

        * Memcache缓存-清空所有缓存

        * 不建议使用该功能

        * @return

        */

       public function clear_all() {

              return $this->memcache->flush();

       }

       

       /**

        * 字段自增-用于记数

        * @param string $key  KEY值

        * @param int    $step 新增的step值

        */

       public function  increment($key, $step = 1) {

              return $this->memcache->increment($key, (int) $step);

       }

       

       /**

        * 字段自减-用于记数

        * @param string $key  KEY值

        * @param int    $step 新增的step值

        */

       public function decrement($key, $step = 1) {

              return $this->memcache->decrement($key, (int) $step);

       }

       

       /**

        * 关闭Memcache链接

        */

       public function close() {

              return $this->memcache->close();

       }

       

       /**

        * 替换数据

        * @param string $key 期望被替换的数据

        * @param string $value 替换后的值

        * @param int    $time  时间值

        * @param bool   $flag  是否进行压缩

        */

       public function replace($key, $value, $time = 0, $flag = false) {

              return $this->memcache->replace($key, $value, false, $time);

       }

       

       /**

        * 获取Memcache的版本号

        */

       public function getVersion() {

              return $this->memcache->getVersion();

       }

       

       /**

        * 获取Memcache的状态数据

        */

       public function getStats() {

              return $this->memcache->getStats();

       }

       

       /**

        * Memcache缓存-设置链接服务器

        * 支持多MEMCACHE服务器

        * 配置文件中配置Memcache缓存服务器：

        * $InitPHP_conf['memcache'][0]   = array('127.0.0.1', '11211');  

        * @param  array $servers 服务器数组-array(array('127.0.0.1', '11211'))

        */

       public function add_server($servers) {

              $this->memcache = new Memcache;

              if (!is_array($servers) || empty($servers)) exit('memcache server is null!');

              foreach ($servers as $val) {

                     $this->memcache->addServer($val[0], $val[1]);

              }      

       }

}
```](https://so.csdn.net/so/search/s.do?q=string&t=blog)](https://so.csdn.net/so/search/s.do?q=function&t=blog)](https://so.csdn.net/so/search/s.do?q=linux&t=blog)](https://so.csdn.net/so/search/s.do?q=服务器&t=blog)




