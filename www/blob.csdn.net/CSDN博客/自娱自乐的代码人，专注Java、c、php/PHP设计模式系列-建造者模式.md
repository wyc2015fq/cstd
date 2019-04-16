# PHP设计模式系列 - 建造者模式 - 自娱自乐的代码人，专注Java、c、php - CSDN博客





2012年06月19日 19:23:46[initphp](https://me.csdn.net/initphp)阅读数：2129
所属专栏：[PHP设计模式](https://blog.csdn.net/column/details/phpshejimoshi.html)








- 什么是建造者模式

> 
建造者模式主要是为了消除其它对象复杂的创建过程。

- 设计场景
- - 
有一个用户的UserInfo类，创建这个类，需要创建用户的姓名，年龄，金钱等信息，才能获得用户具体的信息结果。

- 
创建一个UserInfoBuilder 用户建造者类，这个类，将UserInfo复杂的创建姓名，年龄，金钱等操作封装起来，简化用户类的创建过程


- 代码：UserInfo类，创建UserInfo类是复杂的，痛苦的。

```php
//建造者模式，目的是消除其它对象复杂的创建过程

/* 描述一个用户的类，包含用户姓名，年龄，金钱 */
class UserInfo {

	protected $userName = '';  
	protected $userAge = '';
	protected $userMoney = '';

	public function setUserName($userName) {
		$this->userName = $userName;
	}	
	
	public function setUserAge($userAge) {
		$this->userAge = $userAge;
	}	
	
	public function setUserMoney($userMoney) {
		$this->userMoney = $userMoney;
	}
	
	public function getPeople() {
		echo "这个人的姓名是：" . $this->setUserName . ',年龄是：'  . $this->userAge . ', 金钱：' . $this->userMoney;
	}
}
/* 实例化，并且创建这个用户的时候，是很痛苦的，需要设置用户名，年龄和金钱*/
$peopleInfo = array(
	'userName' => 'initphp',
	'userAge' => 28,
	'userMoney' => '100元'
	);
$UserInfo = new UserInfo;
//下面需要一步步的设置用户信息，才能得到用户详细信息，过程纠结而痛苦
$UserInfo->setUserName($peopleInfo['userName']); 
$UserInfo->setUserAge($peopleInfo['userAge']);
$UserInfo->setUserMoney($peopleInfo['userMoney']);
$UserInfo->getPeople();
```
- 代码：UserInfoBuilder 用户信息建造者类，将UserInfo的创建过程封装掉，开发者使用起来心情舒畅

```php
<?php
//建造者模式，目的是消除其它对象复杂的创建过程
include("UserInfo.php");
class UserInfoBuilder {
	protected $obj;
	
	public function __construct() {
		$this->obj = new UserInfo;
	}
	
	public function buildPeople($peopleInfo) {
		$this->obj->setUserName($peopleInfo['userName']);
		$this->obj->setUserAge($peopleInfo['userAge']);
		$this->obj->setUserMoney($peopleInfo['userMoney']);
	} 
	
	public function getPeople() {
		$this->obj->getPeople();
	}
}

/* 创建过程被封装了，用户使用简单了 */
$peopleInfo = array(
	'userName' => 'initphp',
	'userAge' => 28,
	'userMoney' => '100元'
	);
$UserInfoBuilder = new UserInfoBuilder;
$UserInfoBuilder->buildPeople($peopleInfo); //直接一个build
$UserInfoBuilder->getPeople();
```




参考：《PHP设计模式》Aaron Saray著






