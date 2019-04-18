# php操作memcached - weixin_33985507的博客 - CSDN博客
2017年01月17日 20:04:00[weixin_33985507](https://me.csdn.net/weixin_33985507)阅读数：3

```php
1、实例化memcache的类
$mem = new Memcache();
2、连接memcahced服务器
$mem->connect('127.0.0.1', '11211');
3、memcache设置
$mem->set(key, value, COMPRESSED, expire)
$mem->add()
$mem->replace()
$mem->increment()
$mem->decrement()
4、获取
$mem->get(key)
5、删除
$mem->delete(key)
$mem->flush()
6、关闭memcahce连接
$mem->close();
```
