# PHP设计模式系列 - 观察者模式 - 自娱自乐的代码人，专注Java、c、php - CSDN博客





2012年07月02日 11:43:59[initphp](https://me.csdn.net/initphp)阅读数：4515标签：[设计模式																[php																[function																[class																[email																[web](https://so.csdn.net/so/search/s.do?q=web&t=blog)
个人分类：[PHP设计模式系列](https://blog.csdn.net/initphp/article/category/1171958)

所属专栏：[PHP设计模式](https://blog.csdn.net/column/details/phpshejimoshi.html)](https://so.csdn.net/so/search/s.do?q=email&t=blog)



- 观察者模式

> 
观察者设计模式能够更便利创建和查看目标对象状态的对象，并且提供和核心对象非耦合的置顶功能性。观察者设计模式非常常用，在一般复杂的WEB系统中，观察者模式可以帮你减轻代码设计的压力，降低代码耦合。

- 
场景设计

- 
设计一个订单类

- 
订单创建完成后，会做各种动作，比如发送EMAIL，或者改变订单状态等等。

- 
原始的方法，是将这些操作都写在create函数里面

- 
但是随着订单创建类的越来越庞大，这样的操作已经无法满足需求和快速变动

- 
这个时候，观察者模式出现了。

- 
代码设计：


```php
<?php
//观察者设计模式能够更便利创建和查看目标对象状态的对象，并且提供和核心对象非耦合的置顶功能性。
//观察者设计模式非常常用，在一般复杂的WEB系统中，观察者模式可以帮你减轻代码设计的压力，降低代码耦合。
//以一个购物流程为例子
class order {

	protected $observers = array(); // 存放观察容器
	
	//观察者新增
	public function addObServer($type, $observer) {
		$this->observers[$type][] = $observer;
	}
	
	//运行观察者
	public function obServer($type) {
		if (isset($this->observers[$type])) {
			foreach ($this->observers[$type] as $obser) {
				$a = new $obser;
				$a->update($this); //公用方法
			}
		}
	}
	
	//下单购买流程
	public function create() {
		echo '购买成功';
		$this->obServer('buy'); // buy动作
	}
}
class orderEmail {
	public static function update($order) {
		echo '发送购买成功一个邮件';
	}
}
class orderStatus {
	public static function update($order) {
		echo '改变订单状态';
	}
}
$ob = new order;
$ob->addObServer('buy', 'orderEmail');
$ob->addObServer('buy', 'orderStatus');
$ob->create();
```






参考：《PHP设计模式》Aaron Saray著](https://so.csdn.net/so/search/s.do?q=class&t=blog)](https://so.csdn.net/so/search/s.do?q=function&t=blog)](https://so.csdn.net/so/search/s.do?q=php&t=blog)](https://so.csdn.net/so/search/s.do?q=设计模式&t=blog)




