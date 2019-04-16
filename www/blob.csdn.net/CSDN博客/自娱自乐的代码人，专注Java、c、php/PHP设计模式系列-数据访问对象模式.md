# PHP设计模式系列 - 数据访问对象模式 - 自娱自乐的代码人，专注Java、c、php - CSDN博客





2012年06月20日 10:13:31[initphp](https://me.csdn.net/initphp)阅读数：2544
所属专栏：[PHP设计模式](https://blog.csdn.net/column/details/phpshejimoshi.html)








- 数据访问对象模式

> 
数据访问对象模式描述了如何创建透明访问数据源的对象。

- 场景设计

> - 设计一个BaseDao基类，实现数据库操作基本的一些query,insert,update方法
- 在实际使用的过程中，继承BaseDao，就可以直接调用基类的数据库操作方法

- 代码：BaseDao 数据库操作基类



```php
<?php
//数据访问对象模式

//将数据库访问层脱离出来 作为公用的访问接口，方便用户开放，是php中常用的一种设计模式

class BaseDao {
	private $db;
	
	public function __construct($config) { 
		$this->db = mysql_connect($config['user'], $config['pass'], $config['host']);
		mysql_select_db($config['database'], $this->db);
	}
	
	public function query($sql) {
		return mysql_query($sql, $this->db);
	}
}
```



- 代码：UserDao 用户数据表的数据操作，继承BaseDao



```php
<?php
include("UserDao.php");
class UserDao extends BaseDao {
	public function addUser() {
		$sql = "INSERT INTO user (username) VALUES ('initphp')";
		return $this->query($sql);
	}
}

$UserDao = new UserDao;
$UserDao->addUser();
```



参考：《PHP设计模式》Aaron Saray著




