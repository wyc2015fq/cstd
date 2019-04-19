# PHP中phar包的使用 - walkabc - 博客园
# [PHP中phar包的使用](https://www.cnblogs.com/hitfire/articles/6367908.html)
PHP5.3之后支持了类似Java的jar包，名为phar。用来将多个PHP文件打包为一个文件。
首先需要修改php.ini配置将phar的readonly关闭，默认是不能写phar包的，include是默认开启的。
phar.readonly => On
## 创建一个phar压缩包
```php;gutter
<?php
$phar = new Phar('swoole.phar');
$phar->buildFromDirectory(__DIR__.'/../', '/\.php$/');
$phar->compressFiles(Phar::GZ);
$phar->stopBuffering();
$phar->setStub($phar->createDefaultStub('lib_config.php'));
```
new Phar的参数是压缩包的名称。buildFromDirectory指定压缩的目录，第二个参数可通过正则来制定压缩文件的扩展名。
Phar::GZ表示使用gzip来压缩此文件。也支持bz2压缩。参数修改为 PHAR::BZ2即可。
setSub用来设置启动加载的文件。默认会自动加载并执行 lib_config.php。
执行此代码后，即生成一个swoole.phar文件。
## 使用phar压缩包
```php;gutter
<?php
include 'swoole.phar';
include 'swoole.phar/code/page.php';
```
使用phar可以很方便的打包你的代码，集成部署到线上机器。

