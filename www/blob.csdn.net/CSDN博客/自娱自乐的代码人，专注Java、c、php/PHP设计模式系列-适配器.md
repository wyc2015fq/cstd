# PHP设计模式系列 - 适配器 - 自娱自乐的代码人，专注Java、c、php - CSDN博客





2012年06月19日 12:30:05[initphp](https://me.csdn.net/initphp)阅读数：2700标签：[设计模式																[php																[function																[class																[include																[扩展](https://so.csdn.net/so/search/s.do?q=扩展&t=blog)
个人分类：[PHP设计模式系列](https://blog.csdn.net/initphp/article/category/1171958)

所属专栏：[PHP设计模式](https://blog.csdn.net/column/details/phpshejimoshi.html)](https://so.csdn.net/so/search/s.do?q=include&t=blog)



- 什么是适配器：

> 
适配器设计模式只是将某个对象的接口适配为另一个对象所期望的接口。



- 
设计情景：

- 假如我们原始的有一个UserInfo的类，提供用户信息的类，早起设计该类的时候，只实现了一个getUserName获取用户名的方法。
- 我们的MyOldObject类中，将从UserInfo这个类中获取用户信息，并且输出用户名
- 随着时间的推移，我们旧的UserInfo这个类只提供的获取用户名的方法，已经没法满足需求，我们同时需要获取用户的年龄等信息。
- 为了不改变原本UserInfo这个类，我们就继承UserInfo，建立一个UserInfoAdapter类，实现getAge获取年龄这样的方法。
- 在我们的MyNewObject新的类中，我们实例化UserInfoAdapter，打印出用户姓名和年龄。
- 这样，随着我们的扩展，我们没有改变原先UserInfo这个类和使用这个类的接口，我们通过适配的方法，将UserInfo类扩展出来
- 代码：UserInfo类，实现getUserName方法

```php
<?php 
//早期的一个用户类，只实现获取用户名的方法
class UserInfo {

	public function getUserName() {
		return 'initphp';
	}
}
```
- 代码：MyOldObject类，从UserInfo类中获取信息，输出用户名

```php
<?php
include_once("UserInfo.php");
class MyOldObject {
	public function write() {
		$UserInfo = new UserInfo;
		echo $UserInfo->getUserName();
	}
}
$a = new MyOldObject;
$a->write();
```
- 代码：UserInfoAdapter类，随着时间推移，项目需求在变化，UserInfo类无法满足需求，我们做了UserInfo类的适配器，满足新功能的需求

```php
<?php 
include_once("UserInfo.php"); 
class UserInfoAdapter extends UserInfo{

	public function getUserAge() {
		return 28;
	}
	
	public function getUser() { 
		return array(
			'username' => $this->getUserName(),
			'age' => $this->getUserAge()
		);
	}
}
```
- 代码：MyNewObject类，新功能的类，需要打印出用户年龄和姓名，UserInfo类无法满足需求，需要调用UserInfoAdapter适配器这个类

```php
<?php
include_once("UserInfoAdapter.php");
class MyNewObject { 
	public function write() {
		$UserInfoAdapter = new UserInfoAdapter;
		print_r($UserInfoAdapter->getUser()); 
	}
}
$a = new MyNewObject;
$a->write();
```





参考：《PHP设计模式》Aaron Saray著](https://so.csdn.net/so/search/s.do?q=class&t=blog)](https://so.csdn.net/so/search/s.do?q=function&t=blog)](https://so.csdn.net/so/search/s.do?q=php&t=blog)](https://so.csdn.net/so/search/s.do?q=设计模式&t=blog)




