# 国产InitPHP框架系列 - InitPHP框架搭建高可用WEB应用03：模板View使用 - 自娱自乐的代码人，专注Java、c、php - CSDN博客





2013年12月29日 14:25:24[initphp](https://me.csdn.net/initphp)阅读数：6371
所属专栏：[InitPHP框架](https://blog.csdn.net/column/details/initphp.html)









**InitPHP框架是一款轻量级PHP开源框架，框架文档和下载地址：[http://initphp.com](http://write.blog.csdn.net/postedit/17334455)**




## 模板基本使用

> 
### 1. 模板配置

我们继续之前几篇文章的代码设计。这一节主要讲如何来使用InitPHP框架的模板。

先看一下配置，配置文件从initphp/initphp.conf.php中的模板配置拷贝到conf/comm.conf.php文件中。模板的配置看如下：

```php
/*********************************View配置*****************************************/
/**
 * 模板配置
 * 1. 可以自定义模板的文件夹，编译模板路径，模板文件后缀名称，编译模板后缀名称
 * 是否编译，模板的驱动和模板的主题
 * 2. 一般情况下，默认配置是最优的配置方案，你可以不选择修改模板文件参数
 */
$InitPHP_conf['template']['template_path']      = 'web/template'; //模板路径
$InitPHP_conf['template']['template_c_path']    = 'data/template_c'; //模板编译路径 
$InitPHP_conf['template']['template_type']      = 'htm'; //模板文件类型  
$InitPHP_conf['template']['template_c_type']    = 'tpl.php';//模板编译文件类型 
$InitPHP_conf['template']['template_tag_left']  = '<!--{';//模板左标签
$InitPHP_conf['template']['template_tag_right'] = '}-->';//模板右标签
$InitPHP_conf['template']['is_compile']         = true;//模板每次编译-系统上线后可以关闭此功能
$InitPHP_conf['template']['driver']             = 'simple'; //不同的模板驱动编译
$InitPHP_conf['template']['theme']              = ''; //模板主题
```


配置文件中已经有了详细的注释，这边要注意一下几个配置细节：

is_compile参数。一般情况下，模板都会写在template_path文件夹下，然后会生成新的编译文件到template_c_path文件夹下。在调试环境下打开这个参数，上线之后请关闭这个参数，这样每次请求都不会生成新的文件。

driver 参数。这个参数是模板驱动，默认有两种，第一种是default 第二种是simple，默认是simple





### 2. 使用模板

在web/目录下创建 template文件夹，主要用来存放模板，模板后缀名.htm

![](https://img-blog.csdn.net/20131229143130296?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvaW5pdHBocA==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)

在app/目录下创建data/template_c文件夹，data文件夹主要用来存放生成的模板文件，这个文件需要有可写777权限。

![](https://img-blog.csdn.net/20131229143317265?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvaW5pdHBocA==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)

在template下面创建一个hello.htm的模板文件

![](https://img-blog.csdn.net/20131229143628625?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvaW5pdHBocA==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)

然后在indexController中调用模板

```php
<?php
/**
 * 入口Controller
 * @author zhuli
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
		$this->view->display("hello"); //调用hello.htm模板，后缀名.htm不需要填写
	}
	
	/**
	 * get方法
	 */
	public function get() {
		echo "THIS IS GET<br/>";
	}
}
```


浏览器中访问http://127.0.0.1/test/www.initphp.com/ 后可以看到模板调用过，并且模板编译文件夹下多了一个新的文件hello.tpl.php

![](https://img-blog.csdn.net/20131229143959125?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvaW5pdHBocA==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)




### 3. View API使用

View Api 一共只有5个API接口。接口文档：http://www.initphp.com/6_4.htm

模板API使用：

```php
//模板会按照顺序设置，但是set_tpl函数中的第二个参数F和L分别让模板最先和最后显示
		$this->view->set_tpl('test'); //设置一个template/test.htm的模板页面
		$this->view->set_tpl('user/userinfo'); //设置template/user/userinfo.htm
		$this->view->set_tpl('header', 'F'); //设置为头部,最先显示
		$this->view->set_tpl('footer', 'L'); //最后显示
		$this->view->remove_tpl('test'); //可以移除上面已经设置的test.htm模板
		$this->view->get_tpl(); //可以得到已经设置的模板数组
		$this->view->display(); //模板显示
```


### 4. default的模板标签

default模板标签其实就是最原始的PHP使用方法，简单暴力好用。


```php
/* 以下是default的模板标签使用方法（具体使用和PHP中一样）：*/
/* 配置文件修改：*/
$InitPHP_conf['template']['driver']             = 'default';

/* 一个简单的模板*/
<!--{echo $uid;}--> //这个输出一个变量
<!--{if (true == true) { }-->
<!--{}}-->
<!--{layout:user}-->
```


### 5. simple模板标签

```php
/* 以下是simple的模板标签使用方法：*/

/* 配置文件修改： */
$InitPHP_conf['template']['driver']             = 'simple';

/* 标签使用方法： */

/* 普通使用方法：*/
<!--{echo $uid;}-->

/* if语句使用：*/
<!--{if ($a == 'yes')}-->
<!--{elseif ($a == 'no')}-->
<!--{else}-->
<!--{/if}--> 

/* foreach语句使用:*/
<!--{foreach ($a as $k => $V)}-->
<!--{/foreach}-->

/* for语句使用：*/
<!--{for ($i=0; $i<100; $i++)}-->
<!--{/for}--> 

/* 输出变量：*/
<!--{$username}-->

/* 输出常量：*/
<!--{APP_PATH}-->

<!--{layout:user}-->
```






## 模板高级使用

> 
### 1. 自定义标签
- 自定义的模板标签放置在： initphp/core/view/driver/文件夹下，该文件夹下已经有  default.init.php  和   simple.init.php，分别为default和simple两个模板标签驱动。
- 文件的名称为：标签驱动名称+'.init.php'，例如：simple.init.php
- 类名称为：标签驱动名称+'Init'，例如：simpleInit
- 需要定义一个init($str, $left, $right)的公有函数，$str是需要替换的HTML代码，$left是模板标签的左标记（默认：'<!--{'），$right是模板标签的右标记（默认：'}-->'）
- 需要使用自定义模板标签的时候，别忘记修改配置文件中的：$InitPHP_conf['template']['driver']


    具体参看下面的simple模板标签驱动：



```php
if (!defined('IS_INITPHP')) exit('Access Denied!');
/*********************************************************************************
 * InitPHP 2.1 国产PHP开发框架  View-simple 简单模板驱动规则模型
 *-------------------------------------------------------------------------------
 * 版权所有: CopyRight By initphp.com
 * 您可以自由使用该源码，但是在使用过程中，请保留作者信息。尊重他人劳动成果就是尊重自己
 *-------------------------------------------------------------------------------
 * $Author:zhuli
 * $Dtime:2011-10-09 
***********************************************************************************/
class simpleInit {

	/**
	 * 模板驱动-简单的驱动
	 * @param  string $str 模板文件数据
	 * @return string
	 */
	 public function init($str, $left, $right) {
	 	//if操作
	 	$str = preg_replace( "/".$left."if\s+(.+?)".$right."/", "", $str );
		$str = preg_replace( "/".$left."else".$right."/", "", $str );
		$str = preg_replace( "/".$left."elseif\s+(.+?)".$right."/", "", $str );
		//for操作
		$str = preg_replace("/".$left."for\s+(.+?)".$right."/","",$str);
		$str = preg_replace("/".$left."\/for".$right."/","",$str);
		//foreach操作
		$str = preg_replace("/".$left."foreach\s+(.+?)".$right."/","",$str);
		$str = preg_replace("/".$left."\/foreach".$right."/","",$str);
		//输出变量
		$str = preg_replace( "/".$left."(\\$[a-zA-Z_\x7f-\xff][a-zA-Z0-9_\x7f-\xff]*)".$right."/", "", $str );
		//常量输出
		$str = preg_replace( "/".$left."([A-Z_\x7f-\xff][A-Z0-9_\x7f-\xff]*)".$right."/s", "", $str );
		//标签解析
		$str = preg_replace ( "/".$left."\/if".$right."/", "", $str );
	 	$pattern = array('/'.$left.'/', '/'.$right.'/');
		$replacement = array('');
		return preg_replace($pattern, $replacement, $str);
	 }
}
```




### 2. layout布局


- 模板HTML页面中使用Layout：<!--{layout:version}-->。规则：左标签+'layout:'+'模板名称（不需要带.htm，有目录则user/version，和set_tpl()一样使用）'+右标签
- Layout是为了解决用户在HTML页面中来实现模板布局，html中使用了<!--{layout:version}-->，则会显示version.htm静态页面



```php
/* 模板布局，在所有模板引擎中该方法都通用 */
/* 模板布局主要是为了解决模板切割成多个小模板，模板中能直接调用使用*/
/* 标签使用：layout:模板名称（不需要带模板名称后缀，例如user.htm模板，则直接user。如果模板有多个文件夹，则test/user 代表test/user.htm模板）*/
<!--{layout:user}-->
```








### 3. 模板主题


- 一般情况下，一个网站会有多个模板主题，InitPHP提供了简单的模板主题配置方法
- 用户只需要配置$InitPHP_conf['template']['theme']这个变量，如果是red的主题，则值为'red'
- 模板主题都是放在模板默认的目录下面的，例如'red'主题，则模板文件夹目录为： template/red/文件夹下
- 实际上只是在模板默认的文件目录下再添加了一层目录，这样可以方便多主题的应用










### 4. 编译机制

- InitPHP的模板机制都会将HTML页面编译成.php文件，默认为（template_c/编译文件夹和.tpl.php后缀名称）
- $InitPHP_conf['template']['is_compile']是否开启编译机制，建议开发的时候开启，开发完成上线后，关闭该参数











