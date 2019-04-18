# PHP开发笔记 - zwvista - 博客园

## [PHP开发笔记](https://www.cnblogs.com/zwvista/p/10326326.html)

### 安装XAMPP（PHP开发环境）

XAMPP Apache + MariaDB + PHP + Perl
[XAMPP Download](https://www.apachefriends.org/download.html)

### 安装 Composer

[Composer Download](https://getcomposer.org/download/)

```
$ php -v
PHP 7.1.19 (cli) (built: Aug 17 2018 20:10:18) ( NTS )
$ php -r "copy('https://getcomposer.org/installer', 'composer-setup.php');"
$ php -r "if (hash_file('sha384', 'composer-setup.php') === '93b54496392c062774670ac18b134c3b3a95e5a5e5c8f1a9f115f203b75bf9a129d5daa8ba6a13e2cc8a1da0806388a8') { echo 'Installer verified'; } else { echo 'Installer corrupt'; unlink('composer-setup.php'); } echo PHP_EOL;"
$ php composer-setup.php
$ php -r "unlink('composer-setup.php');"
$ mv composer.phar /usr/local/bin/composer
$ composer -V
Composer version 1.8.0 2018-12-03 10:31:16
```

### 安装 PHP 和 Intl 插件（Mac OS）

[Php-intl installation on XAMPP](https://stackoverflow.com/questions/27886117/php-intl-installation-on-xampp)
[PHP 5.3 to 7.3 for OS X / macOS 10.6 to 10.13 as binary package](https://php-osx.liip.ch/)

```
$ curl -s https://php-osx.liip.ch/install.sh | bash -s 7.3
$ export PATH=/usr/local/php5/bin:$PATH
```

### 安装使用 CakePHP

```
# 创建新的应用程序
$ composer self-update && composer create-project --prefer-dist cakephp/app my_app_name
$ cd my_app_name
# 启动服务器
$ bin/cake server
# http://localhost:8765/
```

### 安装使用 Laravel

```
$ composer global require laravel/installer
$ export PATH=$PATH:~/.composer/vendor/bin
# 创建新的应用程序
$ laravel new blog
# 或者
$ composer create-project --prefer-dist laravel/laravel blog
$ cd blog
$ php artisan serve
# http://localhost:8000
```


