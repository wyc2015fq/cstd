
# java实用方法整理（二十七）——PriorityQueue实现最小堆和最大堆 - manduner的博客 - CSDN博客


2019年04月10日 23:13:05[Manduner_TJU](https://me.csdn.net/manduner)阅读数：18标签：[java																](https://so.csdn.net/so/search/s.do?q=java&t=blog)[PriorityQueue																](https://so.csdn.net/so/search/s.do?q=PriorityQueue&t=blog)[最大堆																](https://so.csdn.net/so/search/s.do?q=最大堆&t=blog)[最小堆																](https://so.csdn.net/so/search/s.do?q=最小堆&t=blog)[
							](https://so.csdn.net/so/search/s.do?q=最大堆&t=blog)[
																					](https://so.csdn.net/so/search/s.do?q=PriorityQueue&t=blog)个人分类：[Java学习笔记																](https://blog.csdn.net/manduner/article/category/7486695)
[
																								](https://so.csdn.net/so/search/s.do?q=PriorityQueue&t=blog)
[
				](https://so.csdn.net/so/search/s.do?q=java&t=blog)
[
			](https://so.csdn.net/so/search/s.do?q=java&t=blog)

# 1，PriorityQueue默认实现的是最小堆
```python
import java.util.PriorityQueue;
/**
* @author Manduner_TJU
* @version 创建时间：2019年4月10日下午4:51:00
*/
public class Test9 {
	public static void main(String[] args) {
		int[] a = {45,36,18,53,72,30,48,93,15,35};
		//1，默认实现的是最小堆，元素按照natural ordering排序（自然排序，例如，数字的从小到大）
		PriorityQueue<Integer> minHeap = new PriorityQueue<Integer>();
		
		for(int i=0;i<a.length;i++) {
			minHeap.offer(a[i]);
		}
		
		while(!minHeap.isEmpty()) {
			System.out.print(minHeap.poll()+" ");
		}
		System.out.println();
		
        //输出（升序）：15 18 30 35 36 45 48 53 72 93 
	}
}
```
# 2，通过比较器实现最大堆
```python
import java.util.Comparator;
import java.util.PriorityQueue;
/**
* @author Manduner_TJU
* @version 创建时间：2019年4月10日下午4:51:00
*/
public class Test9 {
	public static void main(String[] args) {
		int[] a = {45,36,18,53,72,30,48,93,15,35};
		//2，通过比较器排序，实现最大堆
		PriorityQueue<Integer> maxHeap = new PriorityQueue<Integer>(new Comparator<Integer>() {
			@Override
			public int compare(Integer o1, Integer o2) {
				/**以下是对比较器升序、降序的理解.
				 *(1) 写成return o1.compareTo(o2) 或者 return o1-o2表示升序
				 *(2) 写成return o2.compareTo(o1) 或者return o2-o1表示降序
				 */
				return o2.compareTo(o1);
			}
			
		}) ;
		
		for(int i=0;i<a.length;i++) {
			maxHeap.offer(a[i]);
		}
		
		while(!maxHeap.isEmpty()) {
			System.out.print(maxHeap.poll()+" ");
		}
		System.out.println();
        //输出（降序）：93 72 53 48 45 36 35 30 18 15 
	}
}
```


