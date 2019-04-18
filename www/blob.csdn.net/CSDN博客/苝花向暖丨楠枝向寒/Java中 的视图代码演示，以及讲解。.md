# Java中 的视图代码演示，以及讲解。 - 苝花向暖丨楠枝向寒 - CSDN博客

2018年06月13日 22:48:58[苝花向暖丨楠枝向寒](https://me.csdn.net/weixin_40247263)阅读数：85


明人不说暗话，直接上代码，代码可以直接copy，自己根据想法测试。

```java
package View;

import java.util.ArrayList;
import java.util.Arrays;
import java.util.Collection;
import java.util.Collections;
import java.util.Date;
import java.util.HashMap;
import java.util.Iterator;
import java.util.List;
import java.util.Map;
import java.util.NavigableSet;
import java.util.Set;
import java.util.SortedSet;
import java.util.TreeSet;

import jdk.internal.util.xml.impl.Pair;

public class ViewTest {

	
	
	public static void main(String[] args) {
		
		
		
		
		System.out.println("--------------------------------------轻量级集合包装器--------------------------------------");			
		
		/**
		 * 轻量级集合包装器
		 */
		
		
		//Card[] cardDeck = new Card[52];
		//List<Card> cardList1 = Arrays.asList(cardDeck);

		//返回的是一个视图、实际底层存储的数据结构还是数组，可以更改里面的值例如set方法，但不可以改结构，例如remove方法
		List<String> cardList = Arrays.asList("1","2","3");
		
		Iterator<String> itr = cardList.iterator();
		
		cardList.set(1, "4");
		//cardList.add("5");   java.lang.UnsupportedOperationException
		while(itr.hasNext()) {
			
			System.out.println(itr.next());  //1 4 3
			//itr.remove();   java.lang.UnsupportedOperationException
		}
		
		
		
		
		//返回一个视图，数据是存在CopiesList中的，不支持set方法，也不允许修改结构，只能获取值
		List<String> strList = Collections.nCopies(10, "DEFAULT");
		
		//  strList.set(1, "1");   java.lang.UnsupportedOperationException
		
		Iterator<String> itr1 = strList.iterator();
		
		while(itr1.hasNext()) {
			
			System.out.println(itr1.next());    //10个DEFAULT
	 
		}
		
		
		//返回一个视图对象，里面只有一个元素，并且只能看
		Set<Card> singleton = Collections.singleton(new Card());
		
		Iterator<Card> iterator = singleton.iterator();

		while(iterator.hasNext()) {
			System.out.println(iterator.next());  //Card [name=zzh, pass=123456]
			//iterator.remove();    java.lang.UnsupportedOperationException
		}
		
		
		//返回一个视图对象，里面只有一个元素，并且只能看
		Map<String, String> singletonMap = Collections.singletonMap("1", "a");

		//singletonMap.put("2", "b");   java.lang.UnsupportedOperationException
		
		System.out.println(singletonMap);   //{1=a}
		
		//返回一个空集合
		Set<Object> emptySet = Collections.emptySet();
		System.out.println(emptySet);    //[]
	
		
		System.out.println("--------------------------------------list子视图--------------------------------------");	
		
		/**
		 * list 子视图
		 */
		
		
		
		//往集合里放入 1-20 个 数
		List<Integer> list = new ArrayList<>();
		
		for(int i=1;i<=20;i++) {
			
			list.add(i);
		}
			
		System.out.println(list);   //[1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16, 17, 18, 19, 20]

		
		/*
		 * sublist返回一个子视图，并且可以对子视图进行操作，如增加删除元素,修改元素，并且对子视图所做
			的任何操作，将影响原集合
		*/
		List<Integer> subList = list.subList(10, 20);
		
		System.out.println(subList);    //[11, 12, 13, 14, 15, 16, 17, 18, 19, 20]
		
		subList.add(1);
		
		subList.set(9, 0);
		
		System.out.println(subList);   //[11, 12, 13, 14, 15, 16, 17, 18, 19, 0, 1]

		System.out.println(list);   //[1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16, 17, 18, 19, 0, 1]
		//清空子视图，原list集合中对应的数据也会被清空
		subList.clear();
		
		System.out.println(subList); //[]
		
		System.out.println(list);  //[1, 2, 3, 4, 5, 6, 7, 8, 9, 10]
		

		
		
		System.out.println("--------------------------------------set子视图--------------------------------------");	
		
		
		
		/**
		 * set子视图  --SortedSet
		 * 下面 符合 左包右不包的规则
		 */
		//LinkedHashSet 和 HashSet 没有实现SortedSet，
		//TreeSet implements NavigableSet extends SortedSet   -- 如果泛型为String，调用compareto比较的时候会报错
		SortedSet<Integer> sortset = new TreeSet<>();
		
		for(int i=1;i<=20;i=i+2) {
			
			sortset.add(i++);
		}
			
		//返回大于等于4且小于7的元素
		SortedSet<Integer> subSet = sortset.subSet(4, 13);
		//返回小于13的
		SortedSet<Integer> headSet = sortset.headSet(13);
		//返回大于等于4的
		SortedSet<Integer> tailSet = sortset.tailSet(4);
		
		System.err.println(sortset);  //[1, 4, 7, 10, 13, 16, 19]
		System.err.println(subSet);	  //[4, 7, 10]
		System.err.println(headSet);  //[1, 4, 7, 10]
		System.err.println(tailSet);  //[4, 7, 10, 13, 16, 19]
		
		//同样的操作子视图会对原集合产生影响
		subSet.clear();	
		System.out.println(sortset);  //[1, 13, 16, 19]
		headSet.clear();
		System.out.println(sortset);  //[13, 16, 19]
		
		
		/**
		 * Set子视图 --NavigableSet
		 * 在截取 子视图时，可以选择是否包含边界 。  SortedSet默认是前面的边界包含，后面的不包含，即左包右不包
		 * 在下面的示例中，我将其设置为右包左不包
		 */
		NavigableSet<Integer> navigableSet = new TreeSet<>();
		
		for(int i=1;i<=20;i=i+2) {
			
			navigableSet.add(i++);
		}
		//大于4小于等于13
		NavigableSet<Integer> subSet2 = navigableSet.subSet(4, false, 13, true);
		//小于等于13
		NavigableSet<Integer> headSet2 = navigableSet.headSet(13, true);
		//大于4
		NavigableSet<Integer> tailSet2 = navigableSet.tailSet(4, false);
		
		System.out.println(navigableSet);   //[1, 4, 7, 10, 13, 16, 19]
		System.out.println(subSet2);		//[7, 10, 13]
		System.out.println(headSet2);		//[1, 4, 7, 10, 13]
		System.out.println(tailSet2);		//[7, 10, 13, 16, 19]
		
		//同样对子视图的操作会对原集合产生影响
		subSet2.clear();
		System.out.println(navigableSet);   //[1, 4, 16, 19]
		
		
		
		System.out.println("--------------------------------------不可修改的视图--------------------------------------");	
		
		/**
		Collections.unmodifiableCollection(c)  
		Collections.unmodifiableList(list)
		Collections.unmodifiableMap(m)
		Collections.unmodifiableNavigableMap(m)
		Collections.unmodifiableNavigableSet(s)
		Collections.unmodifiableSet(s)
		Collections.unmodifiableSortedMap(m)
		Collections.unmodifiableSortedSet(s)
		
		通过上述几种方法可以获得不可修改视图，即 不允许对视图中 值 做修改(set) 以及 结构做修改 (add,remove)
		
		 */
		

		
		List<Integer> arrayList = new ArrayList<>();
		
		for(int i=1;i<=20;i=i+2) {
			
			arrayList.add(i++);
		}
		
		List<Integer> unmodifiableList = Collections.unmodifiableList(arrayList);

		System.out.println(unmodifiableList);  //[1, 4, 7, 10, 13, 16, 19]
		System.out.println(unmodifiableList.get(2)); //7
		Iterator<Integer> iterator2 = unmodifiableList.iterator();
		while(iterator2.hasNext()) {
			System.out.println(iterator2.next()); //[1, 4, 7, 10, 13, 16, 19]
			//iterator2.remove();  java.lang.UnsupportedOperationException
 		}
		//unmodifiableList.set(5, 1); java.lang.UnsupportedOperationException
		//unmodifiableList.add(21);   java.lang.UnsupportedOperationException
		
		
		
		List<Integer> arrayList1 = new ArrayList<>();
		
		for(int i=1;i<=20;i=i+2) {
			
			arrayList1.add(i++);
		}
		
		List<Integer> unmodifiableList1 = Collections.unmodifiableList(arrayList1);
		
		//unmodifiableList和unmodifiableSet返回的对象，调用的equals方法是集合底层的equals，可以判断两个对象内容是否相等
		//unmodifiableSet在这里不做测试
		System.out.println(unmodifiableList1.equals(unmodifiableList));   //true

		
		
		
		List<Integer> arrayList2 = new ArrayList<>();
		
		for(int i=1;i<=20;i=i+2) {
			
			arrayList2.add(i++);
		}
		
		Collection<Integer> unmodifiableCollection = Collections.unmodifiableCollection(arrayList2);
		
		
		List<Integer> arrayList3 = new ArrayList<>();
		
		for(int i=1;i<=20;i=i+2) {
			
			arrayList3.add(i++);
		}
		
		Collection<Integer> unmodifiableCollection1 = Collections.unmodifiableCollection(arrayList3);
	
		//unmodifiableCollection返回的对象，调用的equals方法是Object类的equals，判断两个对象是否是同一个对象
		System.out.println(unmodifiableCollection1.equals(unmodifiableCollection));   //false
		

		
		System.out.println("--------------------------------------同步视图--------------------------------------");	
		
		/**
		 *  为了防止多线程 操作同一个集 且不会被破坏，可以通过视图来实现同步访问
		 *  get 和 put 是同步的。
		 *  
		 *  Collections.synchronizedMap(new HashMap<>());
		 *  Collections.synchronizedList(list)
		 *  Collections.synchronizedCollection(c)
		 *  Collections.synchronizedNavigableMap(m)
		 *  Collections.synchronizedNavigableSet(s)
		 *  Collections.synchronizedSet(s)
		 *  Collections.synchronizedSortedMap(m)
		 *  Collections.synchronizedSortedSet(s)
		 *  
		 */
		Map<Object, Object> synchronizedMap = Collections.synchronizedMap(new HashMap<>());
		
		
		
		System.out.println("--------------------------------------受查视图--------------------------------------");	
		
		/**
		 *  在开发中有可能会出现如下情况 
		 */
		ArrayList<String> strings = new ArrayList<>();
		strings.add("测试数据1");
		strings.add("测试数据2");
		ArrayList rawList = strings;
		rawList.add(new Date());  //add的时候不会报错
	
		//直接输出没问题
		 System.out.println(strings);  //[测试数据1, 测试数据2, Wed Jun 13 22:11:30 CST 2018]
		 System.out.println(rawList);  //[测试数据1, 测试数据2, Wed Jun 13 22:13:35 CST 2018]
		 
		 /**
		  *  遍历单个元素 就会出问题
		  */
		//System.out.println(strings.get(2));    java.util.Date cannot be cast to java.lang.String
		 
/*		 Iterator<String> iterator3 = strings.iterator();
		 while(iterator3.hasNext()) {
			 System.out.println(iterator3.next());            java.util.Date cannot be cast to java.lang.String
		 }
*/
		 
		 
		 //遍历 rawList没问题， 因为 rawList 没加泛型。
		 //所以说 泛型是针对于 引用的，而不是针对于真正存数据的集合的。
		System.out.println(rawList.get(2));      //Wed Jun 13 22:15:16 CST 2018
		
		Iterator iterator3 = rawList.iterator();
		 while(iterator3.hasNext()) {
			 System.out.println(iterator3.next());     // 测试数据1  测试数据2 Wed Jun 13 22:16:33 CST 2018
		 }
		 
		 
		 
		/**
		 *  可通过受查视图  避免上述错误的发生   
		 */
		 
		ArrayList<String> string = new ArrayList<>();
		List<String> safeString = Collections.checkedList(string, String.class);
		List newStrings = safeString;
		newStrings.add("测试数据1");
		newStrings.add("测试数据2");
		System.out.println(newStrings); //[测试数据1, 测试数据2]
		//用了受查视图以后，我们可以在准确的位置，即当我们add错误类型的元素时就会报错，而不是像上面一样，当调用的时候才报错，给我们找bug带来了方便
		//newStrings.add(new Date());java.lang.ClassCastException: Attempt to insert class java.util.Date element into collection with element type class java.lang.String
		
		
/*		
 * 		集合里插入的元素类型主要受限与	checkedList() 方法的第二个参数	
 * 
		ArrayList<List<String>> listString = new ArrayList<>();
		List<String> unSafeString = Collections.checkedList(string, String.class);
		List unSafeNewStrings = unSafeString;
		unSafeNewStrings.add("测试数据1");
		unSafeNewStrings.add("测试数据2");
		unSafeNewStrings.add(new ArrayList<>().add("测试数据3"));
		Attempt to insert class java.lang.Boolean element into collection with element type class java.lang.String
		
		*
		*
		*/
		
		
	}
}
```

