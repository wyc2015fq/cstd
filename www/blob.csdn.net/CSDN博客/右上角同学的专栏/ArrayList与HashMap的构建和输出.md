# ArrayList与HashMap的构建和输出 - 右上角同学的专栏 - CSDN博客
2016年08月22日 16:58:33[右上角徐](https://me.csdn.net/u011032983)阅读数：344
本博客仅为小编练习所用。其中涉及
ArrayList的构造方法之一ArrayList(Collection<? extends E> c)的使用；
HashMap及ArrayList的capacity与size区别（Capactiy是底层数组的长度即可用容量；size指实际存储对象个数，扩容是对Capactiy扩容）；
HashMap及ArrayList输出所有对象的方法。
```java
import java.util.ArrayList;
import java.util.HashMap;
import java.util.Iterator;
import java.util.Map;
import java.util.Map.Entry;
import java.util.Set;
/**
 * @author xhl
 * 
 */
public class arraylist {
	/**
	 * @param args
	 */
	public static void main(String[] args) {
		HashMap<Integer, String> map = new HashMap<Integer, String>(20);
		for (int i = 0; i < 11; i++) {
			map.put(i, "NO." + i);
		}
		System.out.println("HashMap长度：" + map.size());
		// 在构造方法中就加入hashmap元素，使用的构造方法ArrayList(Collection<? extends E> c)
		ArrayList<HashMap.Entry> list = new ArrayList<HashMap.Entry>(
				map.entrySet());
		
		// 注意：HashMap不支持Iterator所以要通过其他的方式迭代Map中的key和value
		// 获取map返回的Set视图
		// 非常注意：返回此映射中包含的映射关系的 Set视图,对此Set视图的操作会反映到原来的HashMap
		Set s = map.entrySet();
		// 对Set视图进行迭代，返回的Entry包含key和value ，格式为：key=value
		Iterator i = s.iterator();
		while (i.hasNext()) {
			Map.Entry o = (Map.Entry) i.next();
			list.add(o);
			System.out.println("hashmap输出方法1:key" + o);
		}
		Set s2 = map.keySet();
		Iterator i2 = s2.iterator();
		while (i2.hasNext()) {
			Object o2 = i2.next();
			// Map.Entry是接口，不能实例化
			System.out.println("hashmap输出方法2:key" + o2 + " value:"
					+ map.get(o2));
		}
		System.out.println("arraylist长度：" + list.size());// 因为把map加入到list两次，所以长度是2*11
		/* ArrayList输出方法1：利用迭代器iterator() */
		Iterator it = list.iterator();
		while (it.hasNext()) {
			System.out.println("ArrayList输出方法1:" + it.next());
		}
		// ArrayList输出方法2：凡是支持迭代器的，就支持增强型for循环
		for (Object tmp : list) {
			System.out.println("ArrayList输出方法2:" + tmp);
		}
		// ArrayList输出方法3：先得到size
		for (int j = 0; j < list.size(); j++) {
			System.out.println("ArrayList输出方法3:" + list.get(j));
		}
	}
}
```
