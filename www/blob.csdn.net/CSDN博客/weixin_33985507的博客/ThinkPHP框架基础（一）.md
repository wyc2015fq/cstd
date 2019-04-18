# ThinkPHP框架基础（一） - weixin_33985507的博客 - CSDN博客
2017年03月20日 15:50:00[weixin_33985507](https://me.csdn.net/weixin_33985507)阅读数：12
文章目录
一、 简介
二、下载地址
三、Comoser安装更新 / Git下载更新
四、LAMP环境搭建
五、目录结构
六、入口文件
七、调试模式
## 一、简介
ThinkPHP 是一个快速、简单的基于 MVC 和面向对象的轻量级 PHP 开发框架，遵循 Apache2 开源协议发布，一直秉承简洁实用的设计原则，在保持出色的性能和至简的代码的同时，尤其注重开发体验和易用性，为 WEB 应用和 API 开发提供了强有力的支持。
> 
ThinkPHP5.0（以下简称TP5） 版本是一个颠覆和重构版本，也是ThinkPHP 十周年献礼版本，基于 PHP5.4 设计（完美支持 PHP7 ），采用全新的架构思想，引入了很多的 PHP 新特性，优化了核心，减少了依赖，支持Composer ，实现了真正的惰性加载，并且为 API 开发做了深入的支持，在功能、性能以及灵活性方面都较为突出
## 二、下载地址
官方下载地址：[http://www.thinkphp.cn/down.html]()
【Github】
应用框架：[https://github.com/top-think/think]()
核心框架：[https://github.com/top-think/framework]()
【码云】
应用框架：[https://git.oschina.net/liu21st/thinkphp5.git]()
核心框架：[https://git.oschina.net/liu21st/framework.git]()
【Coding】
应用框架：[https://git.coding.net/liu21st/thinkphp5.git]()
核心框架：[https://git.coding.net/liu21st/framework.git]()
## 三、Comoser安装更新 / Git下载更新
TP的稳定版可以在官网下载，官方网站的下载版不是实时更新的，如果需要实时更新就需要使用Composer或Git安装。
### Composer安装更新：
- 
**Windows**平台直接下载安装：[Composer-Setup.exe](https://link.jianshu.com?t=https://getcomposer.org/Composer-Setup.exe)
- 
**Linux/Mac**可以运行以下命令：
```
curl -sS https://getcomposer.org/installer | php 
mv composer.phar /usr/local/bin/composer
```
或者
```
apt install composer
```
- 安装完成后切换到你的Web根目录下（默认是/var/www/）执行以下命令，创建TP5项目：
```
composer create-project topthink/think tp5 --prefer-dist
```
- 更新框架到最新版本：
```
composer update
```
**tips:**
Composer安装出现问题可参考：[Composer中文文档](https://link.jianshu.com?t=http://www.kancloud.cn/thinkphp/composer)~~文档有些错误~~
composer下载速度慢使用命令切换国内镜像：`composer config -g repo.packagist composer https://packagist.phpcomposer.com`
apt 下载速度慢请切换国内镜像。
git clone到本地速度慢可以添加host映射，或者使用VPN。
### Git下载更新
还没有安装Git可以参考我写的[Git基础](https://www.jianshu.com/p/17b70b76586b)
安装完成后，先将项目克隆到本地：
```
git clone https://github.com/top-think/think tp5
```
然后切换到tp5目录下克隆核心框架仓库：
```
git clone https://github.com/top-think/framework thinkphp
```
需要更新核心框架时，切换到thinkphp核心目录下，执行以下命令即可：
```
git pull https://github.com/top-think/framework
```
![2118914-ac3480486a4c14e3.jpg](https://upload-images.jianshu.io/upload_images/2118914-ac3480486a4c14e3.jpg)
操作如图
#### 四、LAMP运行环境搭建
基于Ubuntu的LAMP环境搭建。
**mysql安装：**
```
sudo apt-get install mysql-server   然后输入管理员密码 选择Y
```
![2118914-fd4935b4f6a6d920.jpg](https://upload-images.jianshu.io/upload_images/2118914-fd4935b4f6a6d920.jpg)
安装过程中会出现提示设置mysql管理员密码：
![2118914-22d27e00fce21636.jpg](https://upload-images.jianshu.io/upload_images/2118914-22d27e00fce21636.jpg)
安装完成后输入命令查看，显示箭头的端口即表示安装成：
```
sudo netstat -tap | grep mysql
```
![2118914-0cbd0baba39280f3.jpg](https://upload-images.jianshu.io/upload_images/2118914-0cbd0baba39280f3.jpg)
mysql安装成功
**Apache安装：**
```
sudo apt-get install apache2   输入管理员密码 并选择Y
```
在浏览器中输入`127.0.0.1`检测Apache是否安装成功：
![2118914-b0c6bc6724d34bf8.jpg](https://upload-images.jianshu.io/upload_images/2118914-b0c6bc6724d34bf8.jpg)
Apache安装成功
php7安装：
```
sudo apt-get install php7.0 libapache2-mod-php7.0
```
使用命令`php7.0 -v`查看php版本：
![2118914-2a65ffd085425b1f.jpg](https://upload-images.jianshu.io/upload_images/2118914-2a65ffd085425b1f.jpg)
查看php版本
到此LAMP环境搭建完成，让我们写个脚本来测试apache是否能解析php文件：
```
sudo gedit /var/www/html/phpinfo.php
```
输入代码，保存退出：
```
<?php
        phpinfo();
?>
```
使用浏览器访问`http://127.0.0.1/phpinfo.php`:
![2118914-32d69a7b6c57e009.jpg](https://upload-images.jianshu.io/upload_images/2118914-32d69a7b6c57e009.jpg)
解析成功
为了方便管理mysql还可以安装phpmyadmin：
```
sudo apt-get install phpmyadmin
```
在安装过程中会要求选择Web server：apache2或lighttpd，使用空格键选定apache2，按tab键然后确定。然后会要求输入设置的Mysql数据库密码连接密码。
然后让phpmyadmin与apache2建立连接，以我的为例：www目录在/var/www，phpmyadmin在/etc/phpmyadmin目录，所以就用命令：
```
sudo ln -s /etc/phpmyadmin /var/www
```
## 五、目录结构
安装完后目录结构如图所示，Linux环境下目录需要755权限`sudo chmod 755 -R /var/www/`。
![2118914-7605a23a90490029.jpg](https://upload-images.jianshu.io/upload_images/2118914-7605a23a90490029.jpg)
TP5目录结构
**目录说明**
几个关键的路径：
> 
tp5 　　　　　　　项目根目录　　 　　　ROOT_PATH
tp5/application　　 应用目录　　　　　　  APP_PATH
tp5/thinkphp　　　 框架核心目录　　　　  THINK_PATH
tp5/extend　　　　应用扩展目录　　　　  EXTEND_PATH
tp5/vendor　　　　Composer扩展目录　　VENDOR_PATH
thinkphp5框架目录：
```
tp5
├─application                //应用目录
├─extend                     //扩展类库目录（可定义）
├─public                     //网站对外访问目录
├─runtime                    //运行时目录（可定义）
├─vendor                     //第三方类库目录（Composer）
├─thinkphp                   //框架核心目录
├─build.php                  //自动生成定义文件（参考）
├─composer.json              //Composer定义文件
├─LICENSE.txt                //授权说明文件
├─README.md                  //README 文件
├─think                      //命令行工具入口
```
核心框架目录结构：
```
├─thinkphp                   //框架系统目录
│ ├─lang                     //语言包目录
│ ├─library                  //框架核心类库目录
│ │ ├─think think            //类库包目录
│ │ └─traits                 //系统 traits 目录
│ ├─tpl                      //系统模板目录
│ ├─.htaccess                //用于 apache 的重写
│ ├─.travis.yml              //CI 定义文件
│ ├─base.php                 //框架基础文件
│ ├─composer.json            //composer 定义文件
│ ├─console.php              //控制台入口文件
│ ├─convention.php           //惯例配置文件
│ ├─helper.php               //助手函数文件（可选）
│ ├─LICENSE.txt              //授权说明文件
│ ├─phpunit.xml              //单元测试配置文件
│ ├─README.md                //README 文件
│ └─start.php                //框架引导文件
```
## 六、入口文件
TP5默认的入口文件位于public/index.php（实际部署时public为应用对外访问目录），入口文件包含以下几行代码：
```
// 定义应用目录
define('APP_PATH', __DIR__ . '/../application/');
// 加载框架引导文件
require __DIR__ . '/../thinkphp/start.php';
```
这段代码用于定义应用目录`APP_PATH`和`ThinkPHP`框架入口。
通过浏览器访问入口文件：
```
http://localhost/tp5/public/
```
我们会进入欢迎界面：
![2118914-f37aad4b2c959573.jpg](https://upload-images.jianshu.io/upload_images/2118914-f37aad4b2c959573.jpg)
欢迎界面
## 七、调试模式
ThinkPHP调试模式是默认开启的，对性能有一定影响，在实际部署项目时，可以修改配置文件（配置文件目录：application/config.php）中的app_debug配置参数：
```
//关闭调试模式
'app_debug' => false,
```
2017-4-11
