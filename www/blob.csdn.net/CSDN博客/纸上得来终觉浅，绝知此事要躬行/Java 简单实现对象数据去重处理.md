# Java 简单实现对象数据去重处理 - 纸上得来终觉浅，绝知此事要躬行 - CSDN博客





2017年08月09日 17:51:32[boonya](https://me.csdn.net/boonya)阅读数：3254
个人分类：[Java](https://blog.csdn.net/boonya/article/category/878447)









对于数据库去重相当简单，只需要在查询语句上添加DISTINCT关键字就可以搞定，但是针对Nosql数据库而言DISTINCT未必支持，这时候我们需要自己手动实现。这里有一个数据不重复的最简单的原理：字符串的hashcode比较，当然记录的唯一指标不是ID，而是数据本身，我们去掉ID再来比较是否重复就可以了。

**Java 模拟代码：**



```java
package com.boonya.mongo.client;

import java.util.ArrayList;
import java.util.HashMap;
import java.util.List;
import java.util.Map;

public class LoveTest {
	
	public static void main(String[] args) {
		List<Person> list=new ArrayList<Person>();
		
		Person p=new Person("1", "boonya", 20);
		Person p2=new Person("2", "boonya", 20);
		Person p3=new Person("3", "boonya", 20);
		Person p4=new Person("4", "boonya", 20);
		
		list.add(p);
		list.add(p2);
		list.add(p3);
		list.add(p4);
		
		Map<String,Object> map=new HashMap<String, Object>();
		for (Person person : list) {
			if(!map.containsKey(""+person.toString().hashCode())){
				map.put(person.toString().hashCode()+"", person);
			}
		}
		
		for (Object  person : map.values()) {
			Person uniquePerson=(Person) person;
			System.out.println(uniquePerson.getId()+" "+uniquePerson);
		}
	}
	

}
class Person{
	private String id;
	private String name;
	private int age;
	public String getId() {
		return id;
	}
	public void setId(String id) {
		this.id = id;
	}
	public String getName() {
		return name;
	}
	public void setName(String name) {
		this.name = name;
	}
	public int getAge() {
		return age;
	}
	public void setAge(int age) {
		this.age = age;
	}
	public Person(String id, String name, int age) {
		super();
		this.id = id;
		this.name = name;
		this.age = age;
	}
	
	@Override
	public String toString() {
		return "Person [" + (name != null ? "name=" + name + ", " : "")
				+ "age=" + age + "]";
	}
	
	
}
```
**测试结果 ：**




```java
1 Person [name=boonya, age=20]
```

hash碰撞的情况本文不考虑。




**Java8 去重**
```
：
```


```java
List<String> unique = list.stream().distinct().collect(Collectors.toList());
```







