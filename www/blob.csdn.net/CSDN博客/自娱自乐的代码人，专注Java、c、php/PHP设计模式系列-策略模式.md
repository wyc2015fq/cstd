# PHP设计模式系列 - 策略模式 - 自娱自乐的代码人，专注Java、c、php - CSDN博客





2012年07月18日 18:34:55[initphp](https://me.csdn.net/initphp)阅读数：4179标签：[设计模式																[php																[xml																[function																[json																[class](https://so.csdn.net/so/search/s.do?q=class&t=blog)
个人分类：[PHP设计模式系列](https://blog.csdn.net/initphp/article/category/1171958)

所属专栏：[PHP设计模式](https://blog.csdn.net/column/details/phpshejimoshi.html)](https://so.csdn.net/so/search/s.do?q=json&t=blog)



- 策略模式：

> 
策略模式设计帮助构建的对象不必自身包含逻辑，而是能够根据需要利用其他对象中的算法。



- 
使用场景：

- - 例如有一个CD类，我们类存储了CD的信息。
- 原先的时候，我们在CD类中直接调用getCD方法给出XML的结果
- 随着业务扩展，需求方提出需要JSON数据格式输出
- 这个时候我们引进了策略模式，可以让使用方根据需求自由选择是输出XML还是JSON

- 
代码实例：


```php
<?php
//策略模式
//cd类
class cd {
	protected $cdArr;
	
	public function __construct($title, $info) { 
		$this->cdArr['title'] = $title;
		$this->cdArr['info']  = $info;
	}
	
	public function getCd($typeObj) {
		return $typeObj->get($this->cdArr);
	} 
}

class json {
	public function get($return_data) {
		return json_encode($return_data);
	}
}

class xml {
	public function get($return_data) {
			$xml = '<?xml version="1.0" encoding="utf-8"?>';
			$xml .= '<return>';
				$xml .= '<data>' .serialize($return_data). '</data>';
			$xml .= '</return>';
			return $xml;
	}
}

$cd = new cd('cd_1', 'cd_1');
echo $cd->getCd(new json);
echo $cd->getCd(new xml);
```



 参考：《PHP设计模式》Aaron Saray著](https://so.csdn.net/so/search/s.do?q=function&t=blog)](https://so.csdn.net/so/search/s.do?q=xml&t=blog)](https://so.csdn.net/so/search/s.do?q=php&t=blog)](https://so.csdn.net/so/search/s.do?q=设计模式&t=blog)




