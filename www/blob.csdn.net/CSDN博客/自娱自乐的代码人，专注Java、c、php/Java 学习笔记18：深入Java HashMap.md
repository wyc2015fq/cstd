# Java 学习笔记18：深入Java HashMap - 自娱自乐的代码人，专注Java、c、php - CSDN博客





2012年12月06日 14:16:55[initphp](https://me.csdn.net/initphp)阅读数：1387
所属专栏：[Java学习笔记](https://blog.csdn.net/column/details/javago.html)








- HashMap是什么？

> 
HashMap是基于哈希表的Map接口的非同步实现。此实现提供所有可选的映射操作，并允许使用null值和null键。此类不保证映射的顺序，特别是它不保证该顺序恒久不变。


- Java的HashMap

> 
Java的HashMap主要由两种数据结构组成：一个是数组，一个是链表。
![](https://img-my.csdn.net/uploads/201212/06/1354774346_6486.jpg)

新建HashMap的时候，会新建一个数组结构，但是数组结构的长度有限，例如是1024的长度，那么我们只能放置1024个数据么？不是的，1024的长度对于传递进入hashmap的值来说，只是一个hash的基本数据值，无论这个长度多少，总会有数据存进来会和之前的数据重叠在同一个数组key上，那么用什么方法来解决hash冲突呢？

第一种就是开放地址法。开放地址法的方法是如果冲突发生就会根据一定的值逐个单元去寻找空的单元数据格，如果整个数据链都寻找一遍都被占位了怎么办？这个时候开放地方法会在原先额外开辟出的缓冲区域存放数据。

第二种就是链表法。链表的好处就是前一个数据会记录下面一个数据的内存地址，这样有冲突之后，就可以再这个链表上添加一条数据，查询的时候也只要遍历链表直到查询找到位置。

所以，数组的key值越大，hash冲突的概率越小。

之前转过一篇：http://blog.csdn.net/initphp/article/details/8078432 解决hash冲突的方法。新浪微博的转发计数就是用了开放地址法，memcache用了的链表法。


我之前用nodejs写过一个hashmap的实例：https://github.com/zhuli/hash_map


Java中两种hashMap:HashMap和TreeMap。两者的区别是：HashMap通过hashcode对其内容进行快速查找，而TreeMap中所有的元素都保持着某种固定的顺序，如果你需要得到一个有序的结果你就应该使用TreeMap（HashMap中元素的排列顺序是不固定的）。





- Java的 hashmap使用方法：


```java
@RequestMapping(value="/test")
	@ResponseBody
	public String test() {
		Map map = new HashMap(); //实例化一个hashmap
		map.put("username", "initphp"); //新增一个数据
		map.put("password", "test");
		map.put("age", "100");
		map.put("age1", "age1");
		map.put("age2", "age2");
		if (map.containsKey("username") == true) { //判断key是否存在
			System.out.println("key 存在");
		}
		System.out.println("用户名:" + map.get("username")); //获取一个数据
		System.out.println("密码:" + map.get("password"));
		map.remove("age"); //删除一个age数据
		System.out.println("年龄:" + map.get("age"));
		System.out.println("长度：" + map.size()); //整个hashmap数据的长度
		
		//循环输出：
		Iterator iterator_1 = map.keySet().iterator();
		System.out.println("列表："); //整个hashmap数据的长度
		while (iterator_1.hasNext()) {
			Object key = iterator_1.next();
			System.out.println("tab.get(key) is :"+map.get(key));
		} 
		map.clear();//清除整个hash map
		return "test";
	}
```


结果：

![](https://img-my.csdn.net/uploads/201212/06/1354776045_1644.jpg)

- hashmap和list看起来用法上有相似的地方，但是有很大的区别，一个是hash的模式，一个是列表的模式。
- list适合一些列表，有序数据。hashmap更适合k => v的数据结构。
- hashmap可以发现是无序的



- Java的TreeMap使用方法


```java
@RequestMapping(value="/test")
	@ResponseBody
	public String test() {
		TreeMap map = new TreeMap(); //实例化一个hashmap
		map.put("username", "initphp"); //新增一个数据
		map.put("password", "test");
		map.put("age", "100");
		map.put("age1", "age1");
		map.put("age2", "age2");
		if (map.containsKey("username") == true) { //判断key是否存在
			System.out.println("key 存在");
		}
		System.out.println("用户名:" + map.get("username")); //获取一个数据
		System.out.println("密码:" + map.get("password"));
		map.remove("age"); //删除一个age数据
		System.out.println("年龄:" + map.get("age"));
		System.out.println("长度：" + map.size()); //整个hashmap数据的长度
		
		//循环输出：
		Iterator iterator_1 = map.keySet().iterator();
		System.out.println("列表："); //整个hashmap数据的长度
		while (iterator_1.hasNext()) {
			Object key = iterator_1.next();
			System.out.println("tab.get(key) is :"+map.get(key));
		} 
		map.clear();//清除整个hash map
		return "test";
	}
```


结果：

![](https://img-my.csdn.net/uploads/201212/06/1354776320_5678.jpg)

- 结果除了列表之外，其余和HashMap结果一样
- TreeMap是有序的列表，根据key的a,b,c的字母排序








