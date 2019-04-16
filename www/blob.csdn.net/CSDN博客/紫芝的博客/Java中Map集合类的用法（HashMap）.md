# Java中Map集合类的用法（HashMap） - 紫芝的博客 - CSDN博客





2018年09月25日 19:39:18[紫芝](https://me.csdn.net/qq_40507857)阅读数：211








### **Map键值对，键（key）一般存储索引，值（value）一般存储对象。**

**Map的特性即「键-值」（Key-Value）匹配**

**java.util.HashMap**

**HashMap在内部使用哈希（Hash），很短时间内可以寻得「键-值」匹配.**

**HashMap中常用方法：**

1.建立一个新的映射

```java
put(object key,object value);
```

2.根据key值找出对应的value值

```java
get(object key);
```

3.判断键是否存在
`containsKey(object key);`
4.判断值是否存在
`containsValue(object value);`
### 实例

```java
package work2;

import java.util.Collection;

import java.util.Iterator;
import java.util.List;

import java.util.ArrayList;

import java.util.Scanner;

import java.util.Map;

import java.util.HashMap;
public class Main {
	public	static void main(String[] args) {
		Scanner cin=new Scanner(System.in);
		Map<String,String> map=new HashMap<String,String>();
		String key1="caterpillar";
		map.put(key1,"caterpillar message");
		System.out.println(map.get(key1));
		
		 map.put("justin", "justin的讯息");

		 map.put("momor", "momor的讯息");

		 map.put("caterpillar", "caterpillar的讯息");

		 Collection collection = map.values();//集合

		 Iterator iterator = collection.iterator();

		  while(iterator.hasNext()) {

		 System.out.println(iterator.next());

		 }
		  System.out.println();
		  
		  for(String value:map.values()) {
			  System.out.println(value);
		  }
		
		cin.close();
	}
}
```





