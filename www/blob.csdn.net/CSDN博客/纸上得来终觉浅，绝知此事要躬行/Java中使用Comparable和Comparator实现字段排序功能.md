# Java中使用Comparable和Comparator实现字段排序功能 - 纸上得来终觉浅，绝知此事要躬行 - CSDN博客





2016年12月23日 11:15:59[boonya](https://me.csdn.net/boonya)阅读数：1241








**Comparable 简介**

Comparable 是排序接口。

若一个类实现了Comparable接口，就意味着“**该类支持排序**”。  即然实现Comparable接口的类支持排序，假设现在存在“实现Comparable接口的类的对象的List列表(或数组)”，则该List列表(或数组)可以通过 Collections.sort（或 Arrays.sort）进行排序。

此外，“实现Comparable接口的类的对象”可以用作“有序映射(如TreeMap)”中的键或“有序集合(TreeSet)”中的元素，而不需要指定比较器。

**Comparable 定义**

Comparable 接口仅仅只包括一个函数，它的定义如下：



```java
package java.lang;
import java.util.*;

public interface Comparable<T> {
    public int compareTo(T o);
}
```



说明：
假设我们通过 x.compareTo(y) 来“比较x和y的大小”。若返回“负数”，意味着“x比y小”；返回“零”，意味着“x等于y”；返回“正数”，意味着“x大于y”。





**Comparator 简介**

Comparator 是比较器接口。

我们若需要控制某个类的次序，而该类本身不支持排序(即没有实现Comparable接口)；那么，我们可以建立一个“该类的比较器”来进行排序。这个“比较器”只需要实现Comparator接口即可。也就是说，我们可以通过“**实现Comparator类来新建一个比较器**”，然后通过该比较器对类进行排序。



**Comparator 定义**

Comparator 接口仅仅只包括两个函数，它的定义如下：



```java
package java.util;

public interface Comparator<T> {

    int compare(T o1, T o2);

    boolean equals(Object obj);
}
```



说明：
(01) 若一个类要实现Comparator接口：它一定要实现compareTo(T o1, T o2) 函数，但可以不实现 equals(Object obj) 函数。

        为什么可以不实现 equals(Object obj) 函数呢？ 因为任何类，默认都是已经实现了equals(Object obj)的。 Java中的一切类都是继承于java.lang.Object，在Object.java中实现了equals(Object obj)函数；所以，其它所有的类也相当于都实现了该函数。

(02) int compare(T o1, T o2) 是“比较o1和o2的大小”。返回“负数”，意味着“o1比o2小”；返回“零”，意味着“o1等于o2”；返回“正数”，意味着“o1大于o2”。








**Comparator 和 Comparable 比较**

Comparable是排序接口；若一个类实现了Comparable接口，就意味着“该类支持排序”。

而Comparator是比较器；我们若需要控制某个类的次序，可以建立一个“该类的比较器”来进行排序。我们不难发现：Comparable相当于“内部比较器”，而Comparator相当于“外部比较器”。







**Comparator 和 Comparable 实例讲解**



首先我们定义一个实现Comparable的接口的实体类，Person：



```java
package com.boonya.program.compare;

/**
 * 可比较排序Person
 * 
 * @package com.boonya.program.compare.ComparablePerson
 * @date 2016年12月23日 下午10:36:10
 * @author pengjunlin
 * @comment
 * @update
 */
public class Person implements Comparable<Person> {

	private long id;
	private String name;
	private int age;
	private String email;
	private String address;

	public Person(long id, String name, int age, String email,
			String address) {
		super();
		this.id = id;
		this.name = name;
		this.age = age;
		this.email = email;
		this.address = address;
	}

	public Person() {
		super();
	}

	public long getId() {
		return id;
	}

	public void setId(long id) {
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

	public String getEmail() {
		return email;
	}

	public void setEmail(String email) {
		this.email = email;
	}

	public String getAddress() {
		return address;
	}

	public void setAddress(String address) {
		this.address = address;
	}

	@Override
	public int compareTo(Person o) {
		if (this == o) {
			return 0;
		} else if (o != null && o instanceof Person) {
			Person u = (Person) o;
			if (id < u.id) {
				return -1;
			}else if(id == u.id){
				return 0;
			} else {
				return 1;
			}
		} else {
			return -1;
		}
	}

	@Override
	public String toString() {
		return "Person [id=" + id + ", name=" + name + ", age=" + age
				+ ", email=" + email + ", address=" + address + "]";
	}
	
	

}
```
然后定义一个排序枚举类，SortEnum：



```java
package com.boonya.program.compare;
/**
 * 排序类型枚举
 * 
 * @package com.boonya.program.compare.SortEnum
 * @date   2016年12月23日  上午10:58:44
 * @author pengjunlin
 * @comment   
 * @update
 */
public enum SortEnum {
	
	/**
	 * 升序
	 */
	ASC(0),
	/**
	 * 降序
	 */
	DESC(1);
	
	private int value=0;

	public int getValue() {
		return value;
	}

	public void setValue(int value) {
		this.value = value;
	}

	private SortEnum(int value) {
		this.value = value;
	}
	
	

}
```


定义Person的比较器，PersonComparator：



```java
package com.boonya.program.compare;

import java.util.Comparator;
/**
 * 比较器PersonComparator
 * 
 * @package com.boonya.program.compare.PersonComparator
 * @date   2016年12月23日  上午10:56:53
 * @author pengjunlin
 * @comment   
 * @update
 */
public class PersonComparator implements Comparator<Person> {

	private String sortFieldName;

	private SortEnum sortEnum;

	public String getSortFieldName() {
		return sortFieldName;
	}

	public void setSortFieldName(String sortFieldName) {
		this.sortFieldName = sortFieldName;
	}

	public SortEnum getSortEnum() {
		return sortEnum;
	}

	public void setSortEnum(SortEnum sortEnum) {
		this.sortEnum = sortEnum;
	}

	public PersonComparator() {
		super();
	}

	public PersonComparator(SortEnum sortEnum) {
		super();
		this.sortEnum = sortEnum;
	}

	public PersonComparator(String sortFieldName, SortEnum sortEnum) {
		super();
		this.sortFieldName = sortFieldName;
		this.sortEnum = sortEnum;
	}

	@Override
	public int compare(Person o1, Person o2) {
		if (sortFieldName == null) {
			if (sortEnum == SortEnum.DESC) {
				return o2.compareTo(o1);
			} else {
				return o1.compareTo(o2);
			}
		} else {
			if (sortEnum == SortEnum.DESC) {
				if (sortFieldName.equals("id")) {
					long val = o2.getId() - o1.getId();
					if (val < 0)
						return 1;
					else if (val > 0)
						return -1;
					else
						return 0;
				} else if (sortFieldName.equals("name")) {
					return o2.getName().compareTo(o1.getName());
				} else if (sortFieldName.equals("age")) {
					int val = o2.getAge() - o1.getAge();
					if (val < 0)
						return 1;
					else if (val > 0)
						return -1;
					else
						return 0;
				} else if (sortFieldName.equals("email")) {
					return o2.getEmail().compareTo(o1.getEmail());
				} else if (sortFieldName.equals("address")) {
					return o2.getAddress().compareTo(o1.getAddress());
				}
				return o2.compareTo(o1);
			} else {
				if (sortFieldName.equals("id")) {
					long val = o1.getId() - o2.getId();
					if (val > 0)
						return 1;
					else if (val < 0)
						return -1;
					else
						return 0;
				} else if (sortFieldName.equals("name")) {
					return o1.getName().compareTo(o2.getName());
				} else if (sortFieldName.equals("age")) {
					int val = o1.getAge() - o2.getAge();
					if (val > 0)
						return 1;
					else if (val < 0)
						return -1;
					else
						return 0;
				} else if (sortFieldName.equals("email")) {
					return o1.getEmail().compareTo(o2.getEmail());
				} else if (sortFieldName.equals("address")) {
					return o1.getAddress().compareTo(o2.getAddress());
				}
				return o1.compareTo(o2);
			}

		}

	}

}
```


最后测试一下我们的排序，PersonComparatorTest：



```java
package com.boonya.program.compare;

import java.util.ArrayList;
import java.util.Collections;
import org.junit.Test;
/**
 * 比较器测试
 * 
 * @package com.boonya.program.compare.PersonComparatorTest
 * @date   2016年12月23日  上午10:58:06
 * @author pengjunlin
 * @comment   
 * @update
 */
public class PersonComparatorTest {

	@Test
	public void sortAscTest() {
		ArrayList<Person> persons = new ArrayList<Person>();
		Person p1 = new Person(1, "Allen", 12, "Allen@support.com", "New York");
		Person p2 = new Person(5, "Bob", 9, "Bob@support.com", "Atlantic");
		Person p3 = new Person(2, "Author", 20, "Author@support.com", "Washington");
		Person p4 = new Person(3, "Peter", 30, "Peter@support.com", "Los Angilis");
		Person p5 = new Person(4, "Mercy", 21, "Mercy@support.com", "San Fransisco");
		persons.add(p1);
		persons.add(p2);
		persons.add(p3);
		persons.add(p4);
		persons.add(p5);
		
		System.out.println(">>>>========sort by id asc===========<<<<"); 
		PersonComparator comparator=new PersonComparator(SortEnum.ASC);

		Collections.sort(persons, comparator);// 用我们写好的Comparator对persons进行排序
		for (int i = 0; i < persons.size(); i++) {
			System.out.println(persons.get(i).toString());
		}
	}
	
	@Test
	public void sortDescTest() {
		ArrayList<Person> persons = new ArrayList<Person>();
		Person p1 = new Person(1, "Allen", 12, "Allen@support.com", "New York");
		Person p2 = new Person(5, "Bob", 9, "Bob@support.com", "Atlantic");
		Person p3 = new Person(2, "Author", 20, "Author@support.com", "Washington");
		Person p4 = new Person(3, "Peter", 30, "Peter@support.com", "Los Angilis");
		Person p5 = new Person(4, "Mercy", 21, "Mercy@support.com", "San Fransisco");
		persons.add(p1);
		persons.add(p2);
		persons.add(p3);
		persons.add(p4);
		persons.add(p5);
		
		System.out.println(">>>>========sort by id desc===========<<<<"); 
		PersonComparator comparator=new PersonComparator(SortEnum.DESC);

		Collections.sort(persons, comparator);// 用我们写好的Comparator对persons进行排序
		for (int i = 0; i < persons.size(); i++) {
			System.out.println(persons.get(i).toString());
		}
	}
	
	@Test
	public void sortByFieldAscTest() {
		ArrayList<Person> persons = new ArrayList<Person>();
		Person p1 = new Person(1, "Allen", 12, "Allen@support.com", "New York");
		Person p2 = new Person(5, "Bob", 9, "Bob@support.com", "Atlantic");
		Person p3 = new Person(2, "Author", 20, "Author@support.com", "Washington");
		Person p4 = new Person(3, "Peter", 30, "Peter@support.com", "Los Angilis");
		Person p5 = new Person(4, "Mercy", 21, "Mercy@support.com", "San Fransisco");
		persons.add(p1);
		persons.add(p2);
		persons.add(p3);
		persons.add(p4);
		persons.add(p5);
		
		System.out.println(">>>>========sort by name asc===========<<<<"); 
		PersonComparator comparator=new PersonComparator("name",SortEnum.ASC);

		Collections.sort(persons, comparator);// 用我们写好的Comparator对persons进行排序
		for (int i = 0; i < persons.size(); i++) {
			System.out.println(persons.get(i).toString());
		}
		
		System.out.println(">>>>========sort by age asc===========<<<<"); 
		PersonComparator comparator2=new PersonComparator("age",SortEnum.ASC);

		Collections.sort(persons, comparator2);// 用我们写好的Comparator对persons进行排序
		for (int i = 0; i < persons.size(); i++) {
			System.out.println(persons.get(i).toString());
		}
		
		System.out.println(">>>>========sort by email asc===========<<<<"); 
		PersonComparator comparator3=new PersonComparator("email",SortEnum.ASC);

		Collections.sort(persons, comparator3);// 用我们写好的Comparator对persons进行排序
		for (int i = 0; i < persons.size(); i++) {
			System.out.println(persons.get(i).toString());
		}
		
		System.out.println(">>>>========sort by address asc===========<<<<"); 
		PersonComparator comparator4=new PersonComparator("address",SortEnum.ASC);

		Collections.sort(persons, comparator4);// 用我们写好的Comparator对persons进行排序
		for (int i = 0; i < persons.size(); i++) {
			System.out.println(persons.get(i).toString());
		}
	}

}
```


输出结果：



```java
>>>>========sort by id asc===========<<<<
Person [id=1, name=Allen, age=12, email=Allen@support.com, address=New York]
Person [id=2, name=Author, age=20, email=Author@support.com, address=Washington]
Person [id=3, name=Peter, age=30, email=Peter@support.com, address=Los Angilis]
Person [id=4, name=Mercy, age=21, email=Mercy@support.com, address=San Fransisco]
Person [id=5, name=Bob, age=9, email=Bob@support.com, address=Atlantic]
>>>>========sort by id desc===========<<<<
Person [id=5, name=Bob, age=9, email=Bob@support.com, address=Atlantic]
Person [id=4, name=Mercy, age=21, email=Mercy@support.com, address=San Fransisco]
Person [id=3, name=Peter, age=30, email=Peter@support.com, address=Los Angilis]
Person [id=2, name=Author, age=20, email=Author@support.com, address=Washington]
Person [id=1, name=Allen, age=12, email=Allen@support.com, address=New York]
>>>>========sort by name asc===========<<<<
Person [id=1, name=Allen, age=12, email=Allen@support.com, address=New York]
Person [id=2, name=Author, age=20, email=Author@support.com, address=Washington]
Person [id=5, name=Bob, age=9, email=Bob@support.com, address=Atlantic]
Person [id=4, name=Mercy, age=21, email=Mercy@support.com, address=San Fransisco]
Person [id=3, name=Peter, age=30, email=Peter@support.com, address=Los Angilis]
>>>>========sort by age asc===========<<<<
Person [id=5, name=Bob, age=9, email=Bob@support.com, address=Atlantic]
Person [id=1, name=Allen, age=12, email=Allen@support.com, address=New York]
Person [id=2, name=Author, age=20, email=Author@support.com, address=Washington]
Person [id=4, name=Mercy, age=21, email=Mercy@support.com, address=San Fransisco]
Person [id=3, name=Peter, age=30, email=Peter@support.com, address=Los Angilis]
>>>>========sort by email asc===========<<<<
Person [id=1, name=Allen, age=12, email=Allen@support.com, address=New York]
Person [id=2, name=Author, age=20, email=Author@support.com, address=Washington]
Person [id=5, name=Bob, age=9, email=Bob@support.com, address=Atlantic]
Person [id=4, name=Mercy, age=21, email=Mercy@support.com, address=San Fransisco]
Person [id=3, name=Peter, age=30, email=Peter@support.com, address=Los Angilis]
>>>>========sort by address asc===========<<<<
Person [id=5, name=Bob, age=9, email=Bob@support.com, address=Atlantic]
Person [id=3, name=Peter, age=30, email=Peter@support.com, address=Los Angilis]
Person [id=1, name=Allen, age=12, email=Allen@support.com, address=New York]
Person [id=4, name=Mercy, age=21, email=Mercy@support.com, address=San Fransisco]
Person [id=2, name=Author, age=20, email=Author@support.com, address=Washington]
```

## 使用反射的方式实现List集合对象任意字段排序

参考：[https://github.com/Tony36051/sortByField/blob/master/src/main/java/SortByField.java](https://github.com/Tony36051/sortByField/blob/master/src/main/java/SortByField.java)




```java
package main.java;

import java.lang.invoke.MethodHandle;
import java.lang.invoke.MethodHandles;
import java.lang.invoke.MethodType;
import java.lang.reflect.Field;
import java.lang.reflect.InvocationTargetException;
import java.lang.reflect.Method;
import java.util.Collections;
import java.util.Comparator;
import java.util.List;

public class SortByField {

	static class Cmp<E> implements Comparator<E> {
		Method getMethod = null;
		Field fieldToGet = null;
		MethodHandle cmpMethodHandle;
		// Method cmpMethod ;

		Cmp(MethodHandle cmpMethodHandle, /* Method cmpMethod, */Method getMethod) {
			this.getMethod = getMethod;
			this.cmpMethodHandle = cmpMethodHandle;
			// this.cmpMethod = cmpMethod;
		}

		Cmp(MethodHandle cmpMethodHandle, Field fieldToGet) {
			this.cmpMethodHandle = cmpMethodHandle;
			this.fieldToGet = fieldToGet;
		}

		@Override
		public int compare(E o1, E o2) {
			if (o2 == null) {
				return -1;
			} else if (o1 == null) {
				return 1;
			}
			try {
				if (getMethod != null) {
					return (int) cmpMethodHandle.invokeExact((Comparable<?>) getMethod.invoke(o1),
							getMethod.invoke(o2));
					// return (int)cmpMethod.invoke(getMethod.invoke(o1),
				}
				if (fieldToGet != null) {
					return (int) cmpMethodHandle.invokeExact((Comparable<?>) fieldToGet.get(o1), fieldToGet.get(o2));
				}
			} catch (IllegalAccessException | IllegalArgumentException | InvocationTargetException e) {
				System.out.println("If sortByField() isn't modifie, it won't print errorStackTrace. Default return 0");
				e.printStackTrace();
			} catch (Throwable e) {
				e.printStackTrace();
			}
			return 0;
		}
	}

	/**
	 * sort a list containing JavaBean according specific key( field ).  
	 * Mostly, sortByField take ~1.5 times as much as Traditional implementation when list.size() > 100K
	 * 
	 * @param list:
	 *            list to be sorted
	 * @param fieldName:
	 *            sort list according this field
	 * @param order:
	 *            asc(default) or desc
	 * @author Tony
	 * @email 360517703@163.com
	 * @Time 2015-08-14 11:12
	 */
	public static <E> void sortByField(List<E> list, String fieldName, String order) {
		if (list == null || list.size() < 2) { // no need to sort
			return;
		}
		if (fieldName == null || fieldName.trim().equals(""))
			return; // won't sort if fieldName is null or ""
		// get actual class of generic E
		Class<?> eClazz = null; // use reflect to get the actual class
		boolean isAllNull = true; // default all elements are null
		for (E e : list) {
			if (e != null) {
				isAllNull = false;
				eClazz = e.getClass();
				break;
			}
		}
		if (isAllNull)
			return; // no need to sort, because all elements are null
		// check fieldName in Class E
		Field keyField = null; // the <fieldName> Field as sort key
		try {
			keyField = eClazz.getDeclaredField(fieldName);
		} catch (NoSuchFieldException e1) {
			e1.printStackTrace();
			System.out.println("The List<E> doesn't contain fieldName. That is "
					+ String.format("%s has no Field %s.", eClazz, fieldName));
		} catch (SecurityException e1) {
			e1.printStackTrace();
			System.out.println("deny access to  class or field.");
		}
		// check field is either Comparable
		Class<?> fieldClazz = keyField.getType();
		boolean isComparable = Comparable.class.isAssignableFrom(fieldClazz);
		if (isComparable == false)
			return; // if the class of fieldName is not comparable, don't sort

		// try to use getter method to get field first. Because a little faster
		// than Field.get(Object)
		StringBuilder getterName; // adapt to JavaBean getter method
		if (fieldClazz.getSimpleName().equals("Boolean")) {
			getterName = new StringBuilder("is");
		} else {
			getterName = new StringBuilder("get");
		}
		char[] cs = fieldName.toCharArray();
		if (cs[0] >= 'a' && cs[0] <= 'z')
			cs[0] -= 32; // change the first char to lowerCase
		getterName.append(cs);
		Method getterMethod = null;
		try {
			getterMethod = eClazz.getDeclaredMethod(getterName.toString());
		} catch (NoSuchMethodException | SecurityException e1) {
			// System.out.println("Field " + fieldName + " has no " + getterName
			// + "() . ");
			// e1.printStackTrace();
		}
		/*
		 * // get compare method for specified field. //Abandoned. Because
		 * MethodHandle.invokeExact() is a little faster than Method.invoke()
		 * Method cmpMethod = null; try { cmpMethod =
		 * fieldClazz.getDeclaredMethod("compareTo", fieldClazz); } catch
		 * (NoSuchMethodException | SecurityException e1) { System.out.println(
		 * "deny access to class or method(comparaTo).\nImpossible to show errorStrackTrace Because of Comparable check"
		 * ); e1.printStackTrace(); cmpMethod.setAccessible(true); }
		 */
		Cmp<E> cmp;
		MethodHandles.Lookup lookup = MethodHandles.lookup();
		MethodType type = MethodType.methodType(int.class, Object.class);
		MethodHandle mh = null;
		try {
			mh = lookup.findVirtual(Comparable.class, "compareTo", type);
		} catch (NoSuchMethodException | IllegalAccessException e1) {
			// TODO Auto-generated catch block
			e1.printStackTrace();
		}
		if (getterMethod != null) {
			// cmpMethod.setAccessible(true);
			getterMethod.setAccessible(true);
			cmp = new Cmp<E>(mh, getterMethod);
		} else { // if cannot find getter method, use reflect to get specified
					// field
			keyField.setAccessible(true);
			cmp = new Cmp<E>(mh, keyField);
		}

		if (order.equalsIgnoreCase("desc")) {
			Collections.sort(list, Collections.reverseOrder(cmp));
			return;
		}
		Collections.sort(list, cmp);
	}

	/**
	 * sort a list containing JavaBean according specific key( field ) order by
	 * ascend.  
	 * 
	 *  Mostly, sortByField take ~1.5 times as much as Traditional implementation when list.size() > 100K
	 * @param list
	 *            list to be sort
	 * @param fieldName
	 *            sort list according this field
	 * @author Tony
	 * @email 360517703@163.com
	 * @Time 2015-08-14 11:12
	 */
	public static <E> void sortByField(List<E> list, String fieldName) {
		sortByField(list, fieldName, "asc");
	}

}
```










