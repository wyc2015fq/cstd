# PHP设计模式系列 - 外观模式 - 自娱自乐的代码人，专注Java、c、php - CSDN博客





2012年06月23日 10:23:11[initphp](https://me.csdn.net/initphp)阅读数：2440标签：[设计模式																[php																[function																[user																[class																[扩展](https://so.csdn.net/so/search/s.do?q=扩展&t=blog)
个人分类：[PHP设计模式系列](https://blog.csdn.net/initphp/article/category/1171958)

所属专栏：[PHP设计模式](https://blog.csdn.net/column/details/phpshejimoshi.html)](https://so.csdn.net/so/search/s.do?q=class&t=blog)



- 外观模式

> 
通过在必需的逻辑和方法的集合前创建简单的外观接口，外观设计模式隐藏了调用对象的复杂性。

外观设计模式和建造者模式非常相似，建造者模式一般是简化对象的调用的复杂性，外观模式一般是简化含有很多逻辑步骤和方法调用的复杂性。

- 应用场景
- 设计一个User类，里面有getUser获取用户信息接口
- 在使用getUser这个接口的时候，需要设置用户的用户名和用户年龄
- 所以在正常情况下，调用getUser接口，需要先实例化User类，然后设置用户信息，最后才调用getUser方法，这个过程是复杂的，如果用户信息非常多的话，或者不断变化的话，调用用户信息类将是维护成本很大的事情，比如，随着业务扩展又添加了用户手机，住址，体重，婚否等信息。
- 设计了一个UserFacade，里面有一个静态方法getUserCall，这个方法可以直接调用getUser函数。
- 代码：getUser类



```php
<?php
//外观模式，通过在必须的逻辑和方法的集合前创建简单的外观接口，外观设计模式隐藏了来自调用对象的复杂性
class User {
	
	protected $userName;
	protected $userAge;
	
	public function setUserName($userName) {
		return $this->userName = $userName;
	}
	
	public function setUserAge($userAge) {
		return $this->userAge = $userAge;
	}
	
	public function getUser() {
		echo '用户姓名：' . $this->userName . '； 用户年龄：' . $this->userAge;
	}
	
}
```

- 代码：UserFacade 用户类外观接口，一个getUserCall接口



```php
//创建一个User 类调用接口，简化获取用户getUser方法的调用
class UserFacade {
	public static function getUserCall($userInfo) {
		$User = new User;
		$User->setUserName($userInfo['username']);
		$User->setUserAge($userInfo['userAge']);
		return $User->getUser();
	}
}
$userInfo = array('username' => 'initphp', 'userAge' => 12);
UserFacade::getUserCall($userInfo); //只要一个函数就能将调用类简化
```](https://so.csdn.net/so/search/s.do?q=user&t=blog)](https://so.csdn.net/so/search/s.do?q=function&t=blog)](https://so.csdn.net/so/search/s.do?q=php&t=blog)](https://so.csdn.net/so/search/s.do?q=设计模式&t=blog)




