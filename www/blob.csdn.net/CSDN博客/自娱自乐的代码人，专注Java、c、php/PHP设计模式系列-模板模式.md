# PHP设计模式系列 - 模板模式 - 自娱自乐的代码人，专注Java、c、php - CSDN博客





2012年07月21日 10:20:35[initphp](https://me.csdn.net/initphp)阅读数：674标签：[设计模式																[php																[function																[数据库																[string																[database](https://so.csdn.net/so/search/s.do?q=database&t=blog)



- 模板模式

> 
模板设计模式创建了一个实施一组方法的抽象对象，子类通常将这个对象作为模板用于自己的设计。



- 
设计场景

- 
一般会用于数据库抽象类。

- 
代码设计：


```php
<?php
if (!defined('IS_INITPHP')) exit('Access Denied!');
/*********************************************************************************
 * InitPHP 2.0 国产PHP开发框架  Dao-dbbase Driver DB基类
 *-------------------------------------------------------------------------------
 * 版权所有: CopyRight By initphp.com
 * 您可以自由使用该源码，但是在使用过程中，请保留作者信息。尊重他人劳动成果就是尊重自己
 *-------------------------------------------------------------------------------
 * $Author:zhuli
 * $Dtime:2011-10-09 
***********************************************************************************/
abstract class dbbaseInit{

	/**
	 * 抽象数据库链接
	 * @param  string $host sql服务器
	 * @param  string $user 数据库用户名
	 * @param  string $password 数据库登录密码
	 * @param  string $database 数据库
	 * @param  string $charset 编码
	 * @param  string $pconnect 是否持久链接
	 */
	abstract protected function connect($host, $user, $password, $database, $charset = 'utf8', $pconnect = 0);
	
	/**
	 * 抽象数据库执行语句
	 * @param  string $sql SQL语句
	 * @return obj
	 */
	abstract protected function query($sql);
	
	/**
	 * 抽象数据库-结果集中的行数
	 * @param $result 结果集
	 * @return array
	 */
	abstract protected function result($result, $num=1);
	
	/**
	 * 抽象数据库-从结果集中取得一行作为关联数组
	 * @param $result 结果集
	 * @return array
	 */
	abstract protected function fetch_assoc($result);
	
	/**
	 * 抽象数据库-从结果集中取得列信息并作为对象返回
	 * @param  $result 结果集
	 * @return array
	 */
	abstract protected function fetch_fields($result);
	
	/**
	 * 抽象数据库-前一次操作影响的记录数
	 * @return int
	 */
	abstract protected function affected_rows();
	
	/**
	 * 抽象数据库-结果集中的行数
	 * @param $result 结果集
	 * @return int
	 */
	abstract protected function num_rows($result);
	
	/**
	 * 抽象数据库-结果集中的字段数量
	 * @param $result 结果集
	 * @return int
	 */
	abstract protected function num_fields($result);
	
	/**
	 * 抽象数据库-获取上一INSERT的ID值
	 * @return Int
	 */
	abstract protected function insert_id();
	
	/**
	 * 抽象数据库-释放结果内存
	 * @param obj $result 需要释放的对象
	 */
	abstract protected function free_result($result);
	
	/**
	 * 抽象数据库链接关闭
	 * @param  string $sql SQL语句
	 * @return obj
	 */
	abstract protected function close();
	
	/**
	 * 错误信息
	 * @return string
	 */
	abstract protected function error();
}


<?php
if (!defined('IS_INITPHP')) exit('Access Denied!');
/*********************************************************************************
 * InitPHP 2.0 国产PHP开发框架  Dao-mysqli 基类
 *-------------------------------------------------------------------------------
 * 版权所有: CopyRight By initphp.com
 * 您可以自由使用该源码，但是在使用过程中，请保留作者信息。尊重他人劳动成果就是尊重自己
 *-------------------------------------------------------------------------------
 * $Author:zhuli
 * $Dtime:2011-10-09 
***********************************************************************************/
class mysqliInit extends dbbaseInit{
	
	public $link_id; //链接对象 
	 
	/**
	 * MYSQL连接器
	 * @param  string $host sql服务器
	 * @param  string $user 数据库用户名
	 * @param  string $password 数据库登录密码
	 * @param  string $database 数据库
	 * @param  string $charset 编码
	 * @param  string $pconnect 是否持久链接
	 * @return obj
	 */
	public function connect($host, $user, $password, $database, $charset = 'utf8', $pconnect = 0) {
		$link_id = ($pconnect == 0) ? mysqli_connect($host, $user, $password) : mysqli_pconnect($host, $user, $password);
		if (!$link_id) InitPHP::initError('mysql connect error!');
		mysqli_query($link_id, 'SET NAMES ' . $charset);
		if (!mysqli_select_db($link_id, $database)) InitPHP::initError('database is not exist!');
		return $link_id;
	}
	
	/**
	 * SQL执行器
	 * @param  string $sql SQL语句
	 * @return obj
	 */
	public function query($sql) {
		return mysqli_query($this->link_id, $sql);
	}
	
	/**
	 * 结果集中的行数
	 * @param $result 结果集
	 * @return array
	 */
	public function result($result, $num=1) {
		return mysqli_result($result, $num);
	}
		
	/**
	 * 从结果集中取得一行作为关联数组
	 * @param $result 结果集
	 * @return array
	 */
	public function fetch_assoc($result) {
		return mysqli_fetch_assoc($result);
	}
	
	/**
	 * 从结果集中取得列信息并作为对象返回
	 * @param  $result 结果集
	 * @return array
	 */
	public function fetch_fields($result) {
		return mysqli_fetch_field($result);
	}
	
	/**
	 * 结果集中的行数
	 * @param $result 结果集
	 * @return int
	 */
	public function num_rows($result) {
		return mysqli_num_rows($result);
	}
	
	/**
	 * 结果集中的字段数量
	 * @param $result 结果集
	 * @return int
	 */
	public function num_fields($result) {
		return mysqli_num_fields($result);
	}
	
	/**
	 * 释放结果内存
	 * @param obj $result 需要释放的对象
	 */
	public function free_result($result) {
		return mysqli_free_result($result);
	}
	
	/**
	 * 获取上一INSERT的ID值
	 * @return Int
	 */
	public function insert_id() {
		return mysqli_insert_id($this->link_id);
	}
	
	/**
	 * 前一次操作影响的记录数
	 * @return int
	 */
	public function affected_rows() {
		return mysqli_affected_rows($this->link_id);
	}
	
	/**
	 * 关闭连接
	 * @return bool
	 */
	public function close() {
		if ($this->link_id !== NULL) @mysqli_close($this->link_id);
		$this->link_id = NULL;
		return true;
	}
	
	/**
	 * 错误信息
	 * @return string
	 */
	public function error() {
		return mysqli_error($this->link_id);
	}
	
}
```](https://so.csdn.net/so/search/s.do?q=string&t=blog)](https://so.csdn.net/so/search/s.do?q=数据库&t=blog)](https://so.csdn.net/so/search/s.do?q=function&t=blog)](https://so.csdn.net/so/search/s.do?q=php&t=blog)](https://so.csdn.net/so/search/s.do?q=设计模式&t=blog)




