# 国产InitPHP框架系列 - InitPHP框架搭建高可用WEB应用02：深入控制器应用 - 自娱自乐的代码人，专注Java、c、php - CSDN博客





2013年12月15日 17:23:18[initphp](https://me.csdn.net/initphp)阅读数：5652
所属专栏：[InitPHP框架](https://blog.csdn.net/column/details/initphp.html)









**InitPHP框架是一款轻量级PHP开源框架，框架文档和下载地址：[http://initphp.com](http://initphp.com)**





## 控制器Controller基本使用

> 
### 1. 配置参数

我们在上一节的时候，已经看到过配置信息。控制器的配置参数非常简单，看下面代码（注释中应该讲解的很详细了）：

```php
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


### 2. 创建一个控制器

我们创建一个indexController.php，indexController.php会放在web/controller。注意类名和文件名需要保持一致。

控制器必须继承框架内置的控制器基类Controller。


```php
<?php
/**
 * 入口Controller
 * @author zhuli
 *
 */
class indexController extends Controller {
	
	public $initphp_list = array("get"); //Action白名单
	
	/**
	 * 入口
	 */
	public function run() {
		echo "HELLO WORLD";
	}
	
	/**
	 * get方法
	 */
	public function get() {
		echo "THIS IS GET";
	}
}
```


### 3. URL中访问

上面的indexController中，我们可以通过URL访问：

http://127.0.0.1/test/www/index.php?c=index&a=run 会调用run方法。

http://127.0.0.1/test/www/index.php?c=index&a=get 会调用get方法。

因为index和run我们分别设置成了默认的default_controller和default_action，所以http://127.0.0.1/test/www/index.php也会调用run方法。


![](https://img-blog.csdn.net/20131215195857218?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvaW5pdHBocA==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)





### 4. Action白名单

我们可以看到上面的indeController类中有一个$initphp_list变量，这个变量是一个数组，参数就是对外可以访问的URL中的a参数的值。

**Controller中需要对外访问的Action都需要加到白名单中，默认的Action run方法除外。**


```php
public $initphp_list = array("get"); //Action白名单
```


### 5. 新增一个Controller

我们新增一个testController.php

![](https://img-blog.csdn.net/20131215200456343?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvaW5pdHBocA==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)


```php
<?php
/**
 * testController
 * @author zhuli.zhul
 *
 */
class testController extends Controller {
	
	public $initphp_list = array("get"); //Action白名单
	
	/**
	 * 入口
	 */
	public function run() {
		echo "TEST RUN";
	}
	
	/**
	 * get方法
	 */
	public function get() {
		echo "TEST GET";
	}
}
```

对外访问：

http://127.0.0.1/test/www/index.php?c=test&a=run

http://127.0.0.1/test/www/index.php?c=test&a=get

**从上面的访问可以看出，URL的c参数对应Controller的名称（除去后缀名称）;URL中的a参数对应Controller中可以访问的函数名称。**



### 6. Before和After方法

配置中$InitPHP_conf['controller']['default_before_action']和$InitPHP_conf['controller']['default_after_action']主要设置前置和后置Action。

看一个例子：

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
		//返回JSON格式，一般做API使用
		return $this->controller->ajax_return(200, "SUCCESS", array("uid" => 10), 'json');
	}
	
	/**
	 * get方法
	 */
	public function get() {
		echo "THIS IS GET";
	}
}
```

**before方法会在run或者get方法调用之前调用。一般可以做权限操作。after方法会在run或者get方法调用之后调用。一般可以做日志等后置的操作。**
调用：http://127.0.0.1/test/www/index.php?c=index&a=get返回结果：

![](https://img-blog.csdn.net/20131215212638421?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvaW5pdHBocA==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)


### 7. 使用InitPHP中自带的Controller API

因为我们的控制器都会继承InitPHP框架中的Controller基类，所以可以调用框架中的API。如果你没有继承基类，则无法调用。

接口文档可以参考：http://initphp.com中的

![](https://img-blog.csdn.net/20131215201922734?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvaW5pdHBocA==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)

控制器中的API，统一使用**$this->controller-> **方式调用。

列几个常用的用法：

1. 获取GET|POST常用的参数

```php
/**
	 * 入口
	 */
	public function run() {
		$uid = (int) $this->controller->get_gp("uid"); //获取GET|POST参数
		echo "HELLO WORLD UID:" + $uid;
	}
```


2. 跳转页面

```php
/**
	 * 入口
	 */
	public function run() {
                //10秒后跳转
		$this->controller->redirect("http://www.initphp.com", 10);
	}
```


3. 返回JSON结构数据

```php
/**
	 * 入口
	 */
	public function run() {
		//返回JSON格式，一般做API使用
		return $this->controller->ajax_return(200, "SUCCESS", array("uid" => 10), 'json');
	}
```

![](https://img-blog.csdn.net/20131215202944609?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvaW5pdHBocA==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)



## 控制器分组



> 
### 1. 为什么要分组？



我们的控制器在URL中访问现在是这样的层次结构index.php?c=xxx&a=xxx。我们只有两层的层次。加入业务比较庞大或者项目比较大，所有的控制器文件都放置在web/controller/文件夹中，会变得非常臃肿。下面是项目结构图：

![](https://img-blog.csdn.net/20131215210300093?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvaW5pdHBocA==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)

那么面对项目比较大的情况我们怎么办呢？一个很简单的办法，就是开启控制器分组。

**开启控制器分组功能后，URL中访问的方式是：http://127.0.0.1/index.php?m=user&c=index&a=run**





### 2. 配置设置

先看conf.php的配置设置

```php
/**
 * Controller控制器配置参数
 * 1. 你可以配置控制器默认的文件夹，默认的后缀，Action默认后缀，默认执行的Action和Controller
 * 2. 一般情况下，你可以不需要修改该配置参数
 * 3. $InitPHP_conf['ismodule']参数，当你的项目比较大的时候，可以选用module方式，
 * 开启module后，你的URL种需要带m的参数，原始：index.php?c=index&a=run, 加module：
 * index.php?m=user&c=index&a=run , module就是$InitPHP_conf['controller']['path']目录下的
 * 一个文件夹名称，请用小写文件夹名称
 */
$InitPHP_conf['ismodule'] = true; //开启module方式
$InitPHP_conf['controller']['path']                  = 'web/controller/'; //控制器存放的文件夹目录
$InitPHP_conf['controller']['controller_postfix']    = 'Controller'; //控制器文件后缀名
$InitPHP_conf['controller']['action_postfix']        = ''; //Action函数名称后缀
$InitPHP_conf['controller']['default_controller']    = 'index'; //默认执行的控制器名称
$InitPHP_conf['controller']['default_action']        = 'run'; //默认执行的Action函数
$InitPHP_conf['controller']['module_list']           = array('user', 'api'); //module白名单
$InitPHP_conf['controller']['default_module']        = 'api'; //默认执行module
$InitPHP_conf['controller']['default_before_action'] = 'before'; //默认前置的ACTION名称
$InitPHP_conf['controller']['default_after_action']  = 'after'; //默认后置ACTION名称
```


开启分组，首先我们要将$InitPHP_conf['ismodule']设置为true

为了安全，我们需要对分组的文件夹名称进行白名单维护，所以要配置$InitPHP_conf['controller']['module_list']

同时也要设置默认的分组名称，$InitPHP_conf['controller']['default_module']

### 3. 创建一个分组示例

我们需要创建user和api两个模块，则在web/controller/文件夹下创建user和api文件夹。

![](https://img-blog.csdn.net/20131215211341312?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvaW5pdHBocA==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)

访问API模块：http://127.0.0.1/test/www/?m=api&c=index&a=get


访问USER模块：http://127.0.0.1/test/www/?m=user&c=test&a=get
**所以开启控制器分组后，m相当于模块文件夹名称，c相当于控制器名称，a相当于控制器中的Action名称**




## 控制器Controller API实现详解

> 


### 1. 控制器基类Controller



我们上面讲到了控制器API的调用。这里我们会详细介绍控制器API的实现。

首先我们打开library/initphp/initphp.php文件中的Controller基类。



> 
```php
/**
 * 控制器Controller基类
 * 1. 每个控制器都需要继承这个基类
 * 2. 通过继承这个基类，就可以直接调用框架中的方法
 * 3. 控制器中可以直接调用$this->contorller 和 $this->view
 * @author zhuli
 */
class Controller extends coreInit {

	/**
	 * @var controllerInit
	 */
	protected $controller;

	/**
	 * @var viewInit
	 */
	protected $view;

	/**
	 * 初始化 
	 */
	public function __construct() {
		parent::__construct();
		$InitPHP_conf = InitPHP::getConfig();
		$this->controller = $this->load('controller', 'c'); //导入Controller
		$this->view       = $this->load('view', 'v'); //导入View
		$this->view->set_template_config($InitPHP_conf['template']); //设置模板
		$this->view->assign('init_token', $this->controller->get_token()); //全局输出init_token标记
		//注册全局变量，这样在Service和Dao中通过$this->common也能调用Controller中的类
		$this->register_global('common', $this->controller); 
	}
}
```


我们从上面的类中，可以看到这个基类中，实际上实现了$controller和$view这两个变量。这两个变量主要通过$this->load这个方法实现controller.init.php对象单例。$view是模板引擎的对象，会在下一节讲。

$this->load是一个工厂函数，在coreInit.php。InitPHP框架中大部分类都通过这个方法进行实例化。


![](https://img-blog.csdn.net/20131215204440296?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvaW5pdHBocA==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)






> 













