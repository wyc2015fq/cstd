# Laravel-admin的安装 - weixin_33985507的博客 - CSDN博客
2018年07月27日 14:30:00[weixin_33985507](https://me.csdn.net/weixin_33985507)阅读数：35
首先是对应的版本
```
Laravel 5.1       composer require encore/laravel-admin "1.1.*"
Laravel 5.2       composer require encore/laravel-admin "1.2.*"
Laravel 5.3       composer require encore/laravel-admin "1.3.*"
Laravel 5.4       composer require encore/laravel-admin "1.4.x-dev"
Laravel 5.5       composer require encore/laravel-admin "1.5.x-dev"
```
我们选用`Laravel 5.5`
- 首先安装`laravel 5.5`
```
composer create-project --prefer-dist laravel/laravel='5.5.*' laravel-admin
```
2.然后进去`laravel-admin`文件中
```
cd larabel-admin
```
- 修改`.env`的数据库连接设置，然后进去`config/database`，修改`mysql`的`'strict' => true`，改为`false`(`mysql5.7`的需要改，`5.7`以下的不需要改)
- 运行这些命令来发布资产和配置
```
php artisan vendor:publish --provider="Encore\Admin\AdminServiceProvider"
```
- 最后运行以下命令来完成安装
```
php artisan admin:install
```
- 配置`laravel`的`nginx`重写规则，在`nginx`配置下加入`try_files $uri $uri/ /index.php?$query_string;`
```
location / {
            root   '/www/laravel-admin/public';
            # 网站默认首页
            index index.php index.html index.htm;
            try_files $uri $uri/ /index.php?$query_string;
        }
```
7.打开浏览器输入`http://localhost/admin/`，使用用户名`admin`和密码`admin`登录
