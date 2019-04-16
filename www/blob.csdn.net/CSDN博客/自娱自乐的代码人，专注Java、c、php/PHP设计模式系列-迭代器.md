# PHP设计模式系列 - 迭代器 - 自娱自乐的代码人，专注Java、c、php - CSDN博客





2012年06月26日 16:55:18[initphp](https://me.csdn.net/initphp)阅读数：2324标签：[设计模式																[php																[function																[iterator																[class																[c](https://so.csdn.net/so/search/s.do?q=c&t=blog)
个人分类：[PHP设计模式系列](https://blog.csdn.net/initphp/article/category/1171958)

所属专栏：[PHP设计模式](https://blog.csdn.net/column/details/phpshejimoshi.html)](https://so.csdn.net/so/search/s.do?q=class&t=blog)



- PHP迭代器：

> 
可帮助构造特定的对象，那些对象能够提供单一标准接口循环或迭代任何类型的可计数数据。（不是特别常用，在PHP中）

- 使用场景：

> 


1.访问一个聚合对象的内容而无需暴露它的内部表示。


2.支持对聚合对象的多种遍历。


3.为遍历不同的聚合结构提供一个统一的接口(即，多态迭代)。



- PHP代码实现：

> 

```php
<?php  
//迭代器：可帮助构造特定的对象，那些对象能够提供单一标准接口循环或迭代任何类型的可计数数据
class MyIterator implements Iterator {   
	
	private $var = array();
    
	public function __construct($array) {    
		$this->var = $array;
    }
    
	public function rewind() {     
        reset($this->var);
	}
    
	public function current() {   
		$var = current($this->var);
         return $var;
    }
	 
	public function valid() {    
		$var = $this->current() !== false;
        return $var;
    }
	
	public function next() {    
		$var = next($this->var);
         return $var;
    }
    
	public function key() {    
		$var = key($this->var);
         return $var;
    }
}
$values = array('a', 'b', 'c');
$it = new MyIterator($values);
foreach ($it as $a => $b) { 
	print "$a: $b<br>";  
}
?>
```

 参考：《PHP设计模式》Aaron Saray著](https://so.csdn.net/so/search/s.do?q=iterator&t=blog)](https://so.csdn.net/so/search/s.do?q=function&t=blog)](https://so.csdn.net/so/search/s.do?q=php&t=blog)](https://so.csdn.net/so/search/s.do?q=设计模式&t=blog)




