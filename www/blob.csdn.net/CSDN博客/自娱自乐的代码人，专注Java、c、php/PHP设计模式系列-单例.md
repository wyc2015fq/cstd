# PHP设计模式系列 - 单例 - 自娱自乐的代码人，专注Java、c、php - CSDN博客





2012年07月17日 16:05:38[initphp](https://me.csdn.net/initphp)阅读数：3707标签：[设计模式																[php																[class																[path																[框架](https://so.csdn.net/so/search/s.do?q=框架&t=blog)
个人分类：[PHP设计模式系列](https://blog.csdn.net/initphp/article/category/1171958)

所属专栏：[PHP设计模式](https://blog.csdn.net/column/details/phpshejimoshi.html)](https://so.csdn.net/so/search/s.do?q=path&t=blog)



- 单例模式

> 
通过提供自身共享实例的访问，单例设计模式用于限制特定对象只能被创建一次。



- 
使用场景

- 例如数据库实例，一般都会走单例模式。
- 单例模式可以减少类的实例化
- 
代码：来源InitPHP框架，先检测类有没被实例化，实例化了就使用已经存放在静态变量中的对象实例，没有则实例化并将对象保存起来。


```php
/**
	 * 框架核心加载-框架的所有类都需要通过该函数出去
	 * 1. 单例模式
	 * 2. 可以加载-Controller，Service，View，Dao，Util，Library中的类文件
	 * 3. 框架加载核心函数
	 * 使用方法：$this->load($class_name, $type)
	 * @param string $class_name 类名称
	 * @param string $type 类别
	 */
	public function load($class_name, $type) {
		$class_path = $this->get_class_path($class_name, $type);
		$class_name = $this->get_class_name($class_name);
		if (!file_exists($class_path)) InitPHP::initError('file '. $class_name . '.php is not exist!');
		if (!isset(self::$instance['initphp'][$class_name])) {
			require_once($class_path);
			if (!class_exists($class_name)) InitPHP::initError('class' . $class_name . ' is not exist!');
			$init_class = new $class_name;
			self::$instance['initphp'][$class_name] = $init_class;
		}
		return self::$instance['initphp'][$class_name];
	}
```](https://so.csdn.net/so/search/s.do?q=class&t=blog)](https://so.csdn.net/so/search/s.do?q=php&t=blog)](https://so.csdn.net/so/search/s.do?q=设计模式&t=blog)




