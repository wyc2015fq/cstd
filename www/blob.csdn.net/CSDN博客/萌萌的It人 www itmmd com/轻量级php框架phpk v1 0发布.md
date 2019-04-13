
# 轻量级php框架phpk v1.0发布 - 萌萌的It人 www.itmmd.com - CSDN博客


2013年08月07日 18:53:53[Jlins](https://me.csdn.net/dyllove98)阅读数：1792


# phpk框架简介
PHPK是一个简单易用，易于扩展的轻量级PHP框架。phpk不仅仅是一个php框架，也是一个js框架，内置一套全新的js内库，完全摒弃了庞大的jquery，所有的前端都是一个全新的微世界。运行高效而不失功能强大，操作灵活而又能扩展自如。作为PHP框架里的“后起之秀”，PHPK秉承了那些优秀的PHP框架所共有的，并进行了很多微创新。
# 功能说明
OOP编写风格，完全依据面向对象的思想编写
多项目管理，共享数据缓存层
自动压缩js和css文件
内嵌一个js微型内核，易于扩展
URL路由，支持3种路由模式
hook钩子操作，支持系统内核外扩展
助手函数，支持外围函数扩展
模版视图，模版引擎支持视图，支持本地缓存
wedgit，支持单独的区块加载
插件机制，可以很方面的引用第三方类库
多语言，支持多语言扩展
多种加密方式
多缓存机制
多种数据库引擎
支持分布式
多权限认证机制
支持自动侦测是wap还是web浏览
支持自动侦测客户端语言

# 使用说明
## 一、如何调用？
把phpk文件夹下载到本地，然后在phpk文件夹上层建立index.php，里面代码如下：
```python
<?php
include './phpk/base.php';
?>
```
然后访问index.php，然后你就会看到“欢迎使用phpk框架！”
## 二、如何配置？
如果第一步访问成功，你就会在项目的根目录发现多了几个文件夹，分别是plugins、data、projects、projects/www。当然还有根目录下的config.inc.php，这个文件是项目总配置文件，代码如下：
```python
<?php
//全局配置声明在这个文件中.
$CONFIG['system']['db']['driver']                 = 'mysql';//数据库类型
$CONFIG['system']['db']['charset']                = 'utf8';//数据库编码
$CONFIG['system']['db']['prefix']                 = 'ig_';//表名前缀
$CONFIG['system']['db']['master']['host']         = 'localhost';//主库地址
$CONFIG['system']['db']['master']['database']     = 'itest';//数据库名称
$CONFIG['system']['db']['master']['port']         = '3306';//默认端口
$CONFIG['system']['db']['master']['user']         = 'root';//数据库帐号
$CONFIG['system']['db']['master']['password']     = '123456';//数据库密码
$CONFIG['system']['app']['allow_dir']             = array("www","wap","admin","tools");//系统允许的应用
$CONFIG['system']['app']['auto_detect']           = 1; //是否自动侦测是wap还是web
$CONFIG['system']['app']['defalut_web_path']      = 'www';//默认的应用目录
$CONFIG['system']['app']['defalut_wap_path']      = 'wap';//默认的手机应用目录
$CONFIG['system']['security']                     = '_phpk_@#!%DFDS_';   // 系统的加密字符串
$CONFIG['language']['switch_on']                  = 1; //是否打开语言包
$CONFIG['language']['default_lang']               = 'zh-cn';//默认语言
$CONFIG['language']['auto_detect']                = 1; //是否自动侦测语言
$CONFIG['language']['get_var']                    = 'lang'; //前端get获取语言包的字符
$CONFIG['language']['lang_list']                  = 'zh-cn';//语言包列表，用逗号隔开
```
在projects/www文件夹下面也会多出一个项目配置文件config.inc.php，代码如下：
```python
<?php
//项目配置声明在这个文件中.
$CONFIG['debug']                        = 1;            // 项目的调试模式，1为打开，0为关闭
$CONFIG['route']['url_type']            = 2;            // url模式，1为字符串模式，2为pathinfo模式
$CONFIG['route']['default_controller']  = 'index';      // 默认控制器文件
$CONFIG['route']['default_action']      = 'index';      // 默认action接口
$CONFIG['output']['charset'] 			= 'utf-8';	    // 页面字符集
$CONFIG['output']['errorlog'] 			= 0;	        // 是否记录错误日志
$CONFIG['output']['forceheader']		= 0;		    // 强制输出页面字符集，用于避免某些环境乱码,0为关闭，1为开启
$CONFIG['output']['gzip'] 				= 0;		    // 是否采用 Gzip 压缩输出
$CONFIG['output']['tmpl_cache'] 		= 0;		    // 是否开启模版缓存，0为内存输出做调试模式用，1为检测缓存输出做开发模式用，2为调用缓存输出，3做正式环境用
$CONFIG['output']['layout_on'] 		    = 1;		    // 是否开启布局，0为关闭，1为开启
$CONFIG['output']['layout_name'] 		= 'layout';     // 布局的文件
$CONFIG['output']['layout_item'] 		= '{_content}'; // 布局的内部替换的标签名称
$CONFIG['output']['tmpl_suffix'] 		= '.php';		// 模版的后缀
$CONFIG['output']['default_tmpl'] 		= 'default';    // 默认模版目录
$CONFIG['output']['csszip'] 		    = 1;		    // 是否压缩css文件，1为压缩，0为不压缩
$CONFIG['output']['jszip'] 		        = 1;		    // 是否压缩js文件，1为压缩，0为不压缩
$CONFIG['loader']['libs']    		    = array();		// 是否加载外围的类
$CONFIG['loader']['helpers']     	    = array();		// 是否加载外围函数
$CONFIG['loader']['hooks']     		    = array();		// 是否加载钩子
$CONFIG['session']['prefix']            = 'pk_';        // session的前缀
$CONFIG['session']['id']                = 'phpk';       // session的id
$CONFIG['session']['expire']            = 1200;         // session的超时时间
$CONFIG['session']['auto_start']        = 1;            // 是否自启动session，1为开启，0为关闭
$CONFIG['cookie']['prefix']             = 'pk_';        // cookie的前缀
$CONFIG['cookie']['path']               = '/';          // cookie的有效路径
$CONFIG['cookie']['expire']             = 1200;         // cookie的超时时间
$CONFIG['cookie']['domain']             = '';           // cookie的作用域
$CONFIG['cache']['type']                = 'file';       // 缓存类型
$CONFIG['cache']['table']               = 'tb_cache';   // 缓存表的名称
$CONFIG['cache']['prefix']              = 'cache_';     // 缓存前缀
$CONFIG['cache']['time']                = 3600;         // 缓存过期时间
$CONFIG['cache']['check']               = true;         // 是否进行缓存的数据校验
$CONFIG['cache']['compress']            = true;         // 是否缓存压缩
$CONFIG['auth']['auth_on']              = 1;            //是否启动权限认证
$CONFIG['auth']['auth_type']            = 'phpk';       //权限认证方式
```
项目配置文件继承了项目总配置文件，也可以对项目总配置文件重写。这样做的好处是，增加了项目的灵活性。
## 三、如何使用模版？
首先要写controller文件，在projects/www/controllers/indexController.class.php文件夹里，这样写：
```python
<?php
class IndexController extends Controller
{
    function index ()
    {
        $welcome = "欢迎使用phpk框架！";
        $this->assign('welcome', $welcome);
        $this->display('welcome');
    }
}
```
模版文件支持layout，可以在projects/www/views/layouts/layout.php这样写，（这个文件的名字可以在项目里配置，也可以单独指定）
```python
<!DOCTYPE html PUBLIC "-//W3C//DTD HTML 4.01 Transitional//EN" "http://www.w3.org/TR/html4/loose.dtd">
<html>
<head>
<meta http-equiv="Content-Type" content="text/html; charset=UTF-8">
<title><?php echo $welcome;?></title>
</head>
<body>
{_content}
</body>
</html>
```

然后在projects/www/views/default/index.php模版文件里这样写：
```python
<?php echo $welcome;?>
```
phpk模版支持js和css简写调用：
```python
{css cssfile1,cssfile2}
{js jsfile1,jsfile2}
```
也支持一些静态调用方法，比如：{img}{js}{css}{pro}{url}{public}等，尽量做到简单、精简、规范。
phpk的内置模版引擎可以不用本地缓存，直接内存输出，也可以在项目完成后直接调用缓存文件。
## 四、如何数据查询？
phpk框架摒弃了一般框架的orm（关系数据库映射），让用户更加容易上手。下面是一个模型用例（projects/www/model/indexModel.class.php）：
```python
<?php
class IndexModel extends Model{
    /**
     * 检测用户名
     * @param string $username
     * @param string $password
     * @return multitype:
     */
    function checkUser($username,$password){
        $sql = "select uid,username,group_id from tb_user where username='$username' and password='$password'";
        return $this->select($sql,1);
    }
    /**
     * 更新用户登录信息
     * @param int $uid
     */
    function updateUser($uid,$arr = array()){
        $data = array('last_login_time'=>time(),'last_login_ip'=>getIp());
        $result = array_merge($data, $arr);
        $this->update("tb_user", $result,"uid='$uid'");
    }
    
    
}
```

## 五、如何使用钩子、助手函数和外围库？
phpk框架沿用了codeigniter的钩子思想，借鉴了thinkphp的行为体系，坚持易用简单的原则。配置钩子，首先需要在项目配置文件里加上：
```python
$CONFIG['loader']['hooks']    = array(
            'pre_controller' => 'auth'
        );		// 是否加载钩子
```
key值参考下面的说明：
```python
* pre_system
     *     系统执行的早期调用.仅仅在系统类和配置项加载完毕的时候. 没有执行路由或者其它的过程.
     * pre_controller
     *     在调用你的任何控制器之前调用.此时所用的基础类,路由选择和安全性检查都已完成.
     * post_controller_constructor
     *     在你的控制器实例化之后，任何方法调用之前调用，传入的参数为实例化的控制器。
     * post_controller
     *     在你的控制器完全运行之后调用，传入的参数为实例化的控制器。
     * display_override
     *     output.tmpl_cache=0时截取调试模式下的模版, 用来在系统执行末尾向web浏览器发送最终页面,传入参数为解析后的模版内容。调用后记得exit。
     * cache_override
     *     截取cache模式下的模版,这可以让你使用自己的方法来显示缓存文件，传入参数为解析后的缓存文件。调用后记得exit。
     * post_system
     *     在最终着色页面发送到浏览器之后,浏览器接收完最终数据的系统执行末尾调用
```
value值为需要加载的文件名，hook的文件放在projects/controllers/hooks/authHook.class.php
助手文件和外围库的加载更加简单，只需要这样写配置文件即可：
```python
$CONFIG['loader']['libs']    		    = array('html','csv');		// 是否加载外围的类
$CONFIG['loader']['helpers']     	    = array('dir','str');		// 是否加载外围函数
```

当然你也可以扩展自己的助手文件和类库，把他们分别放在extensions/functions/和extensions/classes/里面，系统会先判断项目加载文件是否存在，然后再去判断系统内核文件。

# 发布地址
下载地址：[http://code.taobao.org/p/phpk/file/2590/phpk.rar](http://code.taobao.org/p/phpk/file/2590/phpk.rar)
SVN地址：[http://code.taobao.org/p/phpk/src/](http://code.taobao.org/p/phpk/src/)
项目交流群：61591845



