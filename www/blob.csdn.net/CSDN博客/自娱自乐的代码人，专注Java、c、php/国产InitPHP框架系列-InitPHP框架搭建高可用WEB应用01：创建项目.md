# 国产InitPHP框架系列 - InitPHP框架搭建高可用WEB应用01：创建项目 - 自娱自乐的代码人，专注Java、c、php - CSDN博客





2013年12月15日 15:23:30[initphp](https://me.csdn.net/initphp)阅读数：9824
所属专栏：[InitPHP框架](https://blog.csdn.net/column/details/initphp.html)









**InitPHP框架是一款轻量级PHP开源框架，框架文档和下载地址：[http://initphp.com](http://initphp.com)**

**[](http://write.blog.csdn.net/postedit/17334455)**

## 创建Hello World项目

> 
### 1. 下载框架

选择最新的版本，进行下载。框架下载地址：[http://initphp](http://initphp.com)




### 2. 创建项目目录

创建项目目录，目录示意图：

![](https://img-blog.csdn.net/20131215154425859?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvaW5pdHBocA==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)

1. 文件夹initphp是最新下载的initphp框架文件夹

2. conf/comm.conf.php 放置项目配置文件

3. web/controller/ 文件夹放置控制器文件

4. index.php是项目入口文件

说明：使用initphp开发的项目是单入口，所有请求都会通过index.php进行分发。





### 3. index.php入口文件

index.php代码示例

```php
<?php
define('APP_PATH', dirname(__FILE__)); //这个参数必须配置，是项目文件目录
header("Content-Type:text/html; charset=utf-8"); //编码
require_once(APP_PATH . '/initphp/initphp.php'); //导入框架
require_once(APP_PATH . '/conf/comm.conf.php'); //公用配置
InitPHP::init(); //框架初始化
```

入口文件非常简单，定义一个APP_PATH变量，然后导入框架initphp.php文件，最后调用框架中的InitPHP::init()就能运行框架了。




### 4. 配置文件comm.conf.php

comm.conf.php配置文件已经在index.php入口文件中导入。如果我们没有自己配置conf.php文件，则框架会自动使用框架中自带的一个默认的配置文件initphp.conf.php

![](https://img-blog.csdn.net/20131215155451968?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvaW5pdHBocA==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)

我们自定义的配置可以拷贝initphp.conf.php里面的配置信息。里面的配置信息非常多，可以根据需要进行筛选。

由于我们第一个教程只需要使用Hello World，所以配置如下：

```php
<?php
/*********************************************************************************
 * InitPHP 3.2.2 国产PHP开发框架  
 *-------------------------------------------------------------------------------
 * 版权所有: CopyRight By initphp.com
 * 您可以自由使用该源码，但是在使用过程中，请保留作者信息。尊重他人劳动成果就是尊重自己
 *-------------------------------------------------------------------------------
 * $Author:zhuli
 * $Dtime:2012-11-27
***********************************************************************************/
/* 框架全局配置常量 */ 
define('INITPHP_PATH', dirname(__FILE__)); 
define('IS_INITPHP', 1);
error_reporting(E_ERROR | E_PARSE); 
/* 框架全局配置变量 */
$InitPHP_conf = array();
/*********************************基础配置*****************************************/
/**
 * 站点URL配置
 * 必选参数
 */
$InitPHP_conf['url'] = 'http://127.0.0.1/test/';  
/**
 * 是否开启调试
 */
$InitPHP_conf['is_debug'] = true; //开启-正式上线请关闭
/**
 * 路由访问方式
 * 1. 如果为true 则开启path访问方式，否则关闭
 * 2. default：index.php?m=user&c=index&a=run
 * 3. rewrite：/user/index/run/?id=100
 * 4. path: /user/index/run/id/100
 * 5. html: user-index-run.htm?uid=100
 * 6. 开启PATH需要开启APACHE的rewrite模块，详细使用会在文档中体现
 */
$InitPHP_conf['isuri'] = 'default'; 
/*********************************Controller配置*****************************************/
/**
 * Controller控制器配置参数
 * 1. 你可以配置控制器默认的文件夹，默认的后缀，Action默认后缀，默认执行的Action和Controller
 * 2. 一般情况下，你可以不需要修改该配置参数
 * 3. $InitPHP_conf['ismodule']参数，当你的项目比较大的时候，可以选用module方式，
 * 开启module后，你的URL种需要带m的参数，原始：index.php?c=index&a=run, 加module：
 * index.php?m=user&c=index&a=run , module就是$InitPHP_conf['controller']['path']目录下的
 * 一个文件夹名称，请用小写文件夹名称
 */
$InitPHP_conf['ismodule'] = false; //开启module方式
$InitPHP_conf['controller']['path']                  = 'web/controller/'; //控制器存放的文件夹目录
$InitPHP_conf['controller']['controller_postfix']    = 'Controller'; //控制器文件后缀名
$InitPHP_conf['controller']['action_postfix']        = ''; //Action函数名称后缀
$InitPHP_conf['controller']['default_controller']    = 'index'; //默认执行的控制器名称
$InitPHP_conf['controller']['default_action']        = 'run'; //默认执行的Action函数
$InitPHP_conf['controller']['module_list']           = array('test', 'index'); //module白名单
$InitPHP_conf['controller']['default_module']        = 'index'; //默认执行module
$InitPHP_conf['controller']['default_before_action'] = 'before'; //默认前置的ACTION名称
$InitPHP_conf['controller']['default_after_action']  = 'after'; //默认后置ACTION名称
```


我们主要配置$InitPHP_conf['url']和controller相关的配置参数。




### 5. indexController.php 控制器文件

```php
<?php
/**
 * 入口Controller
 * @author zhuli
 *
 */
class indexController extends Controller {
	
	/**
	 * 入口
	 */
	public function run() {
		echo "HELLO WORLD";
	}
}
```


index.php入口文件需要继承Controller基类，Controller是InitPHP中定义的一个类。

需要定义一个run方法，这个run方法是一个默认的Action方法。这个方法名称可以在配置中定义名称。




### 6. 完成项目搭建，运行

这个时候我们访问http://127.0.0.1/test/，浏览器中就会输出HelloWorld

我们也可以通过参数的方式访问：http://127.0.0.1/test/index.php?c=index&a=run访问。**参数c代表对应的Controller名称，run代表Controller中的可访问的Action方法名称。**


![](https://img-blog.csdn.net/20131215160617781?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvaW5pdHBocA==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)







## 入口文件和APP分离项目

> 
### 1. 项目目录结构

![](https://img-blog.csdn.net/20131215171149609?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvaW5pdHBocA==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)






### 2. 为何这样做？



1. 入口文件和app项目文件分离，首先可以更加安全。在配置apache或Nginx虚拟机的时候，只要www目录指到test/www目录下面就可以了，对外层来说，用户无法通过URL访问到app文件夹下的文件。

2. 可以实现多入口。通过多入口，可以方便实现不同入口进入不同的项目模块。

3. 让二级域名实现更加简单。






### 3. 这种部署和上面的部署有啥区别？



入口文件分离部署的方式只需修改入口文件index.php即可。下面是index.php



```php
<?php
define('APP_PATH', "../app/"); //这个参数必须配置，是项目文件目录
header("Content-Type:text/html; charset=utf-8"); //编码
require_once(APP_PATH . '/library/initphp/initphp.php'); //导入框架
require_once(APP_PATH . '/conf/comm.conf.php'); //公用配置
InitPHP::init(); //框架初始化
```



后面的介绍，都会使用这个项目的部署方式。













