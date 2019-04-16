# PHP相关系列 - 绝招，php在zend编辑器下开发，代码提示 - 自娱自乐的代码人，专注Java、c、php - CSDN博客





2012年10月09日 17:50:20[initphp](https://me.csdn.net/initphp)阅读数：1041标签：[zend																[php																[service																[function																[string](https://so.csdn.net/so/search/s.do?q=string&t=blog)
个人分类：[PHP相关系列](https://blog.csdn.net/initphp/article/category/2609095)





单个变量，如果是一个对象，要代码提示的话，需要加上注释：

@var  daoInit

daoInit是类的名称

```php
/**
	 * @var serviceInit
	 */
	protected $service;
```



如果是函数，返回的是一个对象，要代码提示的话，主要加上注释：

@return daoInit

daoInit是类的名称

```php
/**
	 * 分库初始化DB
	 * 如果有多数据库链接的情况下，会调用该函数来自动切换DB link
	 * @param string $db
	 * @return dbInit
	 */
	public function init_db($db = 'default') {
		$this->dao->db->init_db($db);
		return $this->dao->db;
	}
```](https://so.csdn.net/so/search/s.do?q=function&t=blog)](https://so.csdn.net/so/search/s.do?q=service&t=blog)](https://so.csdn.net/so/search/s.do?q=php&t=blog)](https://so.csdn.net/so/search/s.do?q=zend&t=blog)




