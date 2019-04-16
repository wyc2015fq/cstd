# 国产InitPHP框架系列 - InitPHP框架搭建高可用WEB应用05：数据层Dao使用 - 自娱自乐的代码人，专注Java、c、php - CSDN博客





2014年11月29日 09:39:32[initphp](https://me.csdn.net/initphp)阅读数：5962
所属专栏：[InitPHP框架](https://blog.csdn.net/column/details/initphp.html)









**InitPHP框架是一款轻量级PHP开源框架，框架文档和下载地址：[http://initphp.com](http://initphp.com)**



## Dao层说明



Dao层通俗的讲就是数据层。再简单的讲，Dao层主要是用于写sql语句的。可能没有搞过Java的同学会对DAO层比较陌生，甚至不能接受。

但是引入DAO层有非常大的好处：

1. 将业务和数据操作进行剥离。例如将原来的MVC中的module层分割成Service和Dao层。Service主要用来负责业务操作，而Dao主要用来负责数据的操作。

2. 原来的MVC模式，项目开发越久，时间越长，大部分SQL语句都会和业务合在一起，后期维护会变得异常困难，甚至修改一个表的字段需要寻找所有的SQL语句进行修改。引进了DAO 之后，SQL语句的修改只需要在DAO中直接修改就好了。笔者曾经参与过phpwind，因为历史原因，曾经也干过为了修改一个数据表字段而找遍所有文件的戳事。

3. 引进Dao层之后，大大方便了分表分库的操作。因为SQL语句都集中在一个文件中，直接修改就行了。

4. 引进DAO层之后，可以方便的迁移存储媒介，例如本来这个数据表存在mysql上的，现在可以方便的迁移到mongodb上面。

5. Dao层的 原则：一般情个况下一个Dao文件对应一个数据库表，但是也允许一个Dao中有多个表（尽量业务是相同的模块）。




## Dao配置

下面是我们InitPHP框架的数据库配置：



```php
/*********************************DAO数据库配置*****************************************/
/**
 * Dao配置参数
 * 1. 你可以配置Dao的路径和文件（类名称）的后缀名
 * 2. 一般情况下您不需要改动此配置
 */
$InitPHP_conf['dao']['dao_postfix']  = 'Dao'; //后缀
$InitPHP_conf['dao']['path']  = 'library/dao/'; //后缀
/**
 * 数据库配置
 * 1. 根据项目的数据库情况配置
 * 2. 支持单数据库服务器，读写分离，随机分布的方式
 * 3. 可以根据$InitPHP_conf['db']['default']['db_type'] 选择mysql mysqli（暂时支持这两种）
 * 4. 支持多库配置 $InitPHP_conf['db']['default']
 * 5. 详细见文档
 */
$InitPHP_conf['db']['driver']   = 'mysqli'; //选择不同的数据库DB 引擎，一般默认mysqli,或者mysqls
//default数据库配置 一般使用中 $this->init_db('default')-> 或者 $this->init_db()-> 为默认的模型
$InitPHP_conf['db']['default']['db_type']                   = 0; //0-单个服务器，1-读写分离，2-随机
$InitPHP_conf['db']['default'][0]['host']                   = '127.0.0.1'; //主机
$InitPHP_conf['db']['default'][0]['username']               = 'root'; //数据库用户名
$InitPHP_conf['db']['default'][0]['password']               = 'root'; //数据库密码
$InitPHP_conf['db']['default'][0]['database']               = 'test'; //数据库
$InitPHP_conf['db']['default'][0]['charset']                = 'utf8'; //数据库编码   
$InitPHP_conf['db']['default'][0]['pconnect']               = 0; //是否持久链接


//test数据库配置 使用：$this->init_db('test')->  支持读写分离，随机选择（有两个数据库）
$InitPHP_conf['db']['test']['db_type']                      = 2; //0-单个服务器，1-读写分离，2-随机
$InitPHP_conf['db']['test'][0]['host']                      = '127.0.0.1'; //主机
$InitPHP_conf['db']['test'][0]['username']                  = 'root'; //数据库用户名
$InitPHP_conf['db']['test'][0]['password']                  = ''; //数据库密码
$InitPHP_conf['db']['test'][0]['database']                  = 't1'; //数据库
$InitPHP_conf['db']['test'][0]['charset']                   = 'utf8'; //数据库编码   
$InitPHP_conf['db']['test'][0]['pconnect']                  = 0; //是否持久链接

$InitPHP_conf['db']['test'][1]['host']                      = '127.0.0.1'; //主机
$InitPHP_conf['db']['test'][1]['username']                  = 'root'; //数据库用户名
$InitPHP_conf['db']['test'][1]['password']                  = ''; //数据库密码
$InitPHP_conf['db']['test'][1]['database']                  = 't1'; //数据库
$InitPHP_conf['db']['test'][1]['charset']                   = 'utf8'; //数据库编码   
$InitPHP_conf['db']['test'][1]['pconnect']                  = 0; //是否持久链接
```


InitPHP支持多数据库连接，也支持读写分离。



我们这边主要只将单表的操作。




## 基本使用

### 1. 创建一个数据表

我们在test这个库中创建一个非常简单的user表：

![](https://img-blog.csdn.net/20141129094215267?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvaW5pdHBocA==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)

表结构：



```
CREATE TABLE `user` (
  `id` int(10) NOT NULL auto_increment,
  `username` varchar(255) NOT NULL,
  `password` varchar(255) NOT NULL,
  PRIMARY KEY  (`id`)
) ENGINE=MyISAM  DEFAULT CHARSET=utf8  ;
```





### 2. 创建一个Dao

配置文件中，我们配置了Dao的相关配置：



```php
$InitPHP_conf['dao']['dao_postfix']  = 'Dao'; //后缀
$InitPHP_conf['dao']['path']  = 'library/dao/'; //后缀
```


所以我们创建的Dao文件放在library/dao/文件夹中，后缀名称Dao



![](https://img-blog.csdn.net/20141129094810854?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvaW5pdHBocA==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)




userDao需要继承Dao的基类



```php
<?php 
class userDao extends Dao {
	
	public $table_name = 'user';
	private $fields = "username,password";
	
	/**
	 * 新增用户
	 * @param $user
	 */
	public function addUser($user) {
		$user = $this->init_db()->build_key($user, $this->fields);
		return $this->init_db()->insert($user, $this->table_name);
	}
	
	/**
	 * 通过ID获取一条数据
	 * @param unknown_type $id
	 */
	public function getUser($id) {
		return $this->init_db()->get_one($id, $this->table_name);
	}
}
```




### 3. 在Service中调用Dao

我们有一个userService：

![](https://img-blog.csdn.net/20141129095026046?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvaW5pdHBocA==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)




userService需要继承基类Service



```php
<?php
/**
 * DEMO的Service测试
 * @author zhuli
 */
class userService extends Service {

	/**
	 * @var userDao
	 */
	private $userDao;

	public function __construct() {
		parent::__construct();
		$this->userDao = InitPHP::getDao("user");
	}

	/**
	 * 获取一个用户
	 * @param int $id
	 */
	public function getUser($id) {
		if ($id < 1) {
			return array();
		}
		return $this->userDao->getUser($id);
	}

	/**
	 * 创建一个用户
	 */
	public function createUser($user) {
		return $this->userDao->addUser($user);
	}

}
```




### 4. 在Controller中调用

在Controller中调用Service，来新增和获取一个用户信息

![](https://img-blog.csdn.net/20141129095557638?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvaW5pdHBocA==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)

indexController需要继承基类Controller



```php
<?php
/**
 * InitPHP开源框架 - DEM
 * @author zhuli
 */
class indexController extends Controller {

	public $initphp_list = array(); //Action白名单
	
	//使用zend编辑器，变量上面加@var注释之后，编辑器中能提示userService中的方法
	/**
	 * @var userService
	 */
	private $userService;
	
	public function __construct() {
		parent::__construct();
		$this->userService = InitPHP::getService("user");
	}

	public function run() {
		$user = array("username" => "zhuli", "password" => "123456");
		$id = $this->userService->createUser($user);
		echo "UserId:" . $id . "<br/>";
		$userInfo = $this->userService->getUser($id);
		print_r($userInfo);
	}
}
```




### 5. 浏览器中结果

![](https://img-blog.csdn.net/20141129095703611?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvaW5pdHBocA==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)




以上就是Dao层最简单的使用方法。复杂的使用请见Intphp官方网站文档：[http://initphp.com/4_2.htm](http://initphp.com/4_2.htm)






