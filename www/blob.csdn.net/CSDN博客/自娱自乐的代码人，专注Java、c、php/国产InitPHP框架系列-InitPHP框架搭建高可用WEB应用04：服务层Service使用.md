# 国产InitPHP框架系列 - InitPHP框架搭建高可用WEB应用04：服务层Service使用 - 自娱自乐的代码人，专注Java、c、php - CSDN博客





2014年01月23日 13:49:41[initphp](https://me.csdn.net/initphp)阅读数：5884
所属专栏：[InitPHP框架](https://blog.csdn.net/column/details/initphp.html)









**InitPHP框架是一款轻量级PHP开源框架，框架文档和下载地址：[http://initphp.com](http://initphp.com)**



## Service层说明

> 
### 1. Service来源

Service层的设计来源于Java。Java有很多优秀的设计思想，但是很多东西并不一定适合php，因为两种语言的性质不一样。

Service层是业务逻辑层，一般情况下你可以理解为MVC中的Model层，只是传统的Model层有数据库操作，而Service层只专注于业务处理。




### 2. 简单说明
-     如果你的项目要采用InitPHP的分层体系方式，那么Service这一业务逻辑层是必须的，而且是应用程序核心的部分。
- 如果采用MVC模式，那么可以放弃此层应用，直接在Controller调用Dao层就可以了。
-   Service层是介于Controller和Dao中间层，主要的职责是处理业务的逻辑，封装成Controller可以通用调用的服务接口。例如：用户注册接口，userRegister()
-  因为各种项目和开发的不同，Service层在Mvc的时候不需要存在，所以InitPHP框架内置的Service API只有两个接口，也是Service比较常用的两个接口。
-  所有的Service需要继承InitPHP框架提供的框架基类Service
- 建议使用Zend Studio工具开发，有代码提示功能，能将所有的API提示显示，方便您的开发速度




## Service层使用

> 
### 1. 应用配置

在配置文件中，添加Service的配置信息。Service的配置非常简单，就只要配置Service类名称和文件名称的后缀名，以及Service存放的路径就OK了。



```php
/**
 * Service配置参数
 * 1. 你可以配置service的路径和文件（类名称）的后缀名
 * 2. 一般情况下您不需要改动此配置
 */
$InitPHP_conf['service']['service_postfix']  = 'Service'; //后缀
$InitPHP_conf['service']['path'] = 'library/service/'; //service路径
```






### 2. 创建一个TestService.php



在library/service/文件夹下创建/user/TestService.php类。

![](https://img-blog.csdn.net/20140123140133562?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvaW5pdHBocA==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)

TestService：



```php
<?php
/**
 * 创建一个测试的Service
 * @author zhuli
 */
class TestService extends Service {
	
	public function getVal() {
		echo "<h1>This is TestService</h1>";
	}
}
```


### 3. 控制器中调用Service

在Controller中通过InitPHP::getService方法调用Service，该方法实例化了TestService类并且是单例。



```php
<?php
/**
 * 入口Controller
 * @author zhuli.zhul
 *
 */
class indexController extends Controller {
	
	public $initphp_list = array("get"); //Action白名单
	
	public function before() {
		echo "before<br/>";
	}
	
	public function after() {
		echo "after<br/>";
	}
	
	/**
	 * 入口
	 */
	public function run() {
		$this->getTestService()->getVal(); //调用TestService中的getVal方法
		$this->view->display("hello"); //调用hello.htm模板，后缀名.htm不需要填写
	}
	
	/**
	 * get方法
	 */
	public function get() {
		echo "THIS IS GET<br/>";
	}
	
	/**
	 * 调用TestService
	 * @return TestService
	 */
	public function getTestService() {
		return InitPHP::getService("Test", "user");
	}
}
```
- InitPHP框架提供了一个InitPHP::getService() 全局API函数，可以直接单例-实例化Service
- 接口有两个参数，第一个参数是Service的类名称，不需要带后缀'Service'；第二个参数是文件夹路径，默认Service都是放在![](http://initphp.com/static/folderopen.gif)    lib/service/文件夹下的，也会碰到项目比较庞大，将Service类进行分类，比如分成User的类，那么文件夹目录![](http://initphp.com/static/folderopen.gif)    lib/service/user/
- InitPHP::getService和InitPHP::getDao两个接口的用法类似。全局都可以使用。如果用户采用MVC模式，直接在Controller中直接导入Dao即可。








