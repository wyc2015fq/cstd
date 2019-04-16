# PHP设计模式系列 - 中介者模式 - 自娱自乐的代码人，专注Java、c、php - CSDN博客





2012年06月27日 13:16:43[initphp](https://me.csdn.net/initphp)阅读数：2944标签：[设计模式																[php																[containers																[function																[archive																[null](https://so.csdn.net/so/search/s.do?q=null&t=blog)
个人分类：[PHP设计模式系列](https://blog.csdn.net/initphp/article/category/1171958)

所属专栏：[PHP设计模式](https://blog.csdn.net/column/details/phpshejimoshi.html)](https://so.csdn.net/so/search/s.do?q=archive&t=blog)



- 中介者模式

> 
中介者模式用于开发一个对象，这个对象能够在类似对象相互之间不直接相互的情况下传送或者调解对这些对象的集合的修改。一般处理具有类似属性，需要保持同步的非耦合对象时，最佳的做法就是中介者模式。PHP中不是特别常用的设计模式。

- 
设计场景：

- - 我们有一个CD类和一个MP3类，两个类的结构相似。
- 我们需要在CD类更新的时候，同步更新MP3类。
- 传统的做法就是在CD类中实例化MP3类，然后去更新，但是这么做的话，代码就会很难维护，如果新增一个同样的MP4类，那么就没法处理了。
- 中介者模式很好的处理了这种情况，通过中介者类，CD类中只要调用中介者这个类，就能同步更新这些数据。
- 我们的phpwind论坛中，之前有用到过这个设计模式。

- 
代码：


```php
<?php
class CD {
	public $band  = '';           
	public $title = '';           
	protected $_mediator;  
	
	public function __construct(MusicContainerMediator $mediator = NULL) {               
		$this->_mediator = $mediator;           
	}
	
	public function save() {              
		//具体实现待定               
		var_dump($this);           
	}
	
	public function changeBandName($bandname) {               
		if ( ! is_null($this->_mediator)) {                   
			$this->_mediator->change($this, array("band" => $bandname));               
		}               
		$this->band = $bandname;               
		$this->save();           
	}         
}
//MP3Archive类       
class MP3Archive {                      
	protected $_mediator;                      
	
	public function __construct(MusicContainerMediator $mediator = NULL) {               
		$this->_mediator = $mediator;           
	}                      
	
	public function save() {               
		//具体实现待定             
		var_dump($this);           
	}                      
	
	public function changeBandName($bandname) {               
		if ( ! is_null($this->_mediator)) {                   
			$this->_mediator->change($this, array("band" => $bandname));               
		}               
		$this->band = $bandname;               
		$this->save();           
	}       
	
}   

 //中介者类       
class MusicContainerMediator {                      
	protected $_containers = array();                      
	
	public function __construct() {               
		$this->_containers[] = "CD";               
		$this->_containers[] = "MP3Archive";           
	}                      
	
	public function change($originalObject, $newValue) {               
		$title = $originalObject->title;               
		$band  = $originalObject->band;                              
		foreach ($this->_containers as $container) {                   
			if ( ! ($originalObject instanceof $container)) {                      
				$object = new $container;                       
				$object->title = $title;                       
				$object->band  = $band;                                              
				foreach ($newValue as $key => $val) {                
					$object->$key = $val;                       
				}                                              
				$object->save();                   
			}               
		}           
	}       
}   

//测试实例       
$titleFromDB = "Waste of a Rib";       
$bandFromDB  = "Never Again";       
$mediator = new MusicContainerMediator();       
$cd = new CD($mediator);       
$cd->title = $titleFromDB;       
$cd->band  = $bandFromDB;       
$cd->changeBandName("Maybe Once More");
```



 参考：《PHP设计模式》Aaron Saray著](https://so.csdn.net/so/search/s.do?q=function&t=blog)](https://so.csdn.net/so/search/s.do?q=containers&t=blog)](https://so.csdn.net/so/search/s.do?q=php&t=blog)](https://so.csdn.net/so/search/s.do?q=设计模式&t=blog)




