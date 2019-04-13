
# Java实用方法整理（五）——集合类常用方法 - manduner的博客 - CSDN博客


2018年04月17日 23:23:07[Manduner_TJU](https://me.csdn.net/manduner)阅读数：4665标签：[Java																](https://so.csdn.net/so/search/s.do?q=Java&t=blog)[集合																](https://so.csdn.net/so/search/s.do?q=集合&t=blog)[Collection																](https://so.csdn.net/so/search/s.do?q=Collection&t=blog)[
							](https://so.csdn.net/so/search/s.do?q=集合&t=blog)[
																					](https://so.csdn.net/so/search/s.do?q=Java&t=blog)个人分类：[Java学习笔记																](https://blog.csdn.net/manduner/article/category/7486695)
[
																								](https://so.csdn.net/so/search/s.do?q=Java&t=blog)


# 1，集合概述
## （1）集合的由来
Java是面向对象的语言，而面向对象语言对事物的描述是通过对象体现的，为了方便对多个对象进行操作，我们必须把多个对象进行存储。已有的容器类型有：数组和StringBuffer。但是，StringBuffer的结果是一个字符串，不一定满足我们的要求，所以我们只能选择数组，这就是对象数组。而对象数组又不能适应变化的需求，因为数组的长度是固定的，此时，为了适应变化的需求，Java就提供了集合类。
## （2）数组和集合区别
### 1）长度区别
数组的长度固定；集合长度可变
### 2）内容不同
数组存储的是同一种类型的元素；集合可以存储不同类型的元素
### 3）元素的数据类型问题
数组可以存储基本数据类型，也可以存储引用数据类型；集合只能存储引用类型。
## （3）集合的分类
存储多个元素会有不同的需求，针对这些不同的需求，Java就提供了不同的集合类。下边列出了常用的集合类别：
![](https://img-blog.csdn.net/2018041723223797)
Collection概述：它是Collection层次结构中的根接口。Collection表示一组对象，这些对象也称为collection的元素。一些collection允许有重复的元素，而另一些则不允许。一些collection是有序的，而另一些则是无序的。
List概述：它的类型也是接口，称为有序的collection（也称为序列）（注意：这里所说的有序不是说List集合中元素从小到大或者从大到小排列，而是说，元素“出”集合的顺序和“进”集合的顺序是一样的）。使用此接口的用户可以对列表中每个元素的插入位置进行精确地控制。用户可以根据元素的整数索引（在列表中的位置）访问元素，并搜索列表中的元素；与set不同，列表List通常允许重复的元素。
# 2，集合常用的方法
## （1）Collection常用的方法
add(E e) ，添加元素
clear() ，暴力清除集合中所有元素
contains(Object o)， 返回值类型：boolean。判断集合是否包含某个元素
isEmpty() ，返回值类型：boolean。如果此集合不包含元素，则返回true。
iterator() 迭代器。返回值类型：Iterator
size() 返回值类型：int。返回集合中的元素数
## （2）List集合（列表）特有且常用的方法
添加功能
void add(int index,Object element)：在指定位置添加元素
```python
List list = new ArrayList();
list.add("我");
list.add("爱");
list.add("你");
list.add(1,"很");//没有问题
//list.add(3."很")//没有问题
//list.add(10,"很")//有问题，提示“越界添加”
System.out.println(list);//输出：[我,很,爱,你]//注意：在索引位置1处添加元素，并不是覆盖掉原来位置的元素
```
获取功能
Object get(int index)：获取指定位置的元素
```python
List list = new ArrayList();
list.add("我");
list.add("爱");
list.add("你");
//下面这段代码展示List中特有的遍历方法：应用size()和get()方法
for(int i = 0;i<list.size();i++){
   String s = (String)list.get(i);
   System.out.println(s);
}
```
列表迭代器
ListIterator listIterator()：List集合特有的迭代器。该迭代器继承了Iterator迭代器，所以，就可以直接使用hasNext()和next()方法。
特有功能：Object previous()：获取上一个元素；boolean hasPrevious()：判断是否有元素。
注意：ListIterator可以实现逆向遍历，但是必须先正向遍历，才能逆向遍历，所以一般无意义，不使用该方法进行集合的迭代。
**下面这段代码展示了ListIterator的一个应用,同时该段代码也是一个关于ListIterator的一个面试题，值得学习**
```python
/*需求：有如下集合，请判断该集合里面是否包含“java”这个元素，如果有，就添加一个“love”元素
*
*/
List list = new ArrayList();
list.add("hello");
list.add("java");
list.add("world");
/* 1，展示一个会出错的代码
* 出错提示：ConcurrentModificationException：当方法检测到对象的并发修改，但不允许这种修改时，抛出此异常。
* 也就是说：迭代器遍历元素的时候，通过集合是不能修改元素的
*/
Iterator it = list.iterator();
while(it.hasNext()){
	String s = (String)it.next();
	if(s.equals("java")){
		list.add("love");
	}
}
System.out.println(list);
/* 2，当遇到以上错误时，给出两种解决办法
* 首先展示第一种解决办法：使用ListIterator的add()方法
*/
ListIterator lit = list.listItertor();
while(lit.hasNext()){
	String s = (String)lit.next();
	if(s.equals("java")){
		lit.add("love");//注意：此处是利用迭代器进行添加元素，刚添加的元素处于刚才迭代的元素的后面。
	}
}
/* 3，给出另一种解决办法：
* 使用普通循环方法，即使用get()和size()的方法
*/
for(int i = 0;i<list.size();i++){
	String s = (String)list.get(i);
	if(s.equals("java")){
		list.add("love");//注意：此处是将新的元素添加到了集合的最后
	}
}
```
删除功能
Object remove(int index)：根据索引删除元素，返回被删除的元素
修改功能
Object set(int index,Object element)：根据索引修改元素，返回被修改的元素
## （3）List子类特点（面试题）
ArrayList：底层数据结构是数组，查询快，增删慢；线程不安全，效率高。
Vector：底层数据结构是数组，查询快，增删慢；线程安全，效率低。现在已不常用
LinkedList：底层数据结构是链表，查询慢，增删快。线程不安全，效率高。
## （4）LinkedList的特有功能：
添加功能
public void addFirst(Object e)
public void addLast(Object e) //和add()功能一样，所以不常用此方法
获取功能
puclic Object getFirst()
public Object getLast()
删除功能
public Object removeFirst()
public Object removeLast()
# 3，集合应用案例
案例一：获取10个1-30之间的随机数，要求不能重复
```python
import java.util.ArrayList;
import java.util.Random;
/**
* @author Manduner_TJU
* @version 创建时间：2018年5月27日下午4:21:19
*/
/*
 * 需求：获取10个1-30之间的随机数，要求不能重复
 * 分析：
 * 		（1）创建随机数对象
 * 		（2）创建存放随机数的集合
 * 		（3）设定统计量
 * 		（4）产生随机数，并判断集合中是否包含该随机数
 * 				是：不放入集合
 * 				否：放入集合，同时统计量++
 * 		（5）遍历集合
 * 
 */
public class RandomDemo {
	public static void main(String[] args) {
		//创建随机数对象、创建存放随机数的集合
		Random r = new Random();
		ArrayList<Integer> array = new ArrayList<Integer>();
		
		//设定统计量
		int count = 0;
		
		//产生随机数，并判断集合中是否包含该随机数
		while(count < 10) {
			int temp = r.nextInt(30) + 1;
			if(!array.contains(temp)) {
				array.add(temp);
				count++;
			}
		}
		
		//遍历集合
		for(Integer x : array) {
			System.out.println(x);
		}
	}
	
}
```
案例二：键盘录入多个数据，以0结束，要求在控制台输出所录入数据的最大值
```python
import java.util.ArrayList;
import java.util.Arrays;
import java.util.Scanner;
/**
* @author Manduner_TJU
* @version 创建时间：2018年5月27日下午5:06:25
*/
/*
 *需求：键盘录入多个数据，以0结束，要求在控制台输出所录入数据的最大值
 *分析：
 *		（1）创建键盘录入对象
 *		（2） 创建集合
 *		（3）录入数据，并存放入集合中
 *		（4）将集合转换为数组
 *		（5）将数组进行排序
 *		（6）输出数组的最大值
 * 
 */
public class ArrayListDemo {
	public static void main(String[] args) {
		//创建键盘录入对象、创建集合
		Scanner sc = new Scanner(System.in);
		ArrayList<Integer> array = new ArrayList<Integer>();
		
		//录入数据，并存放入集合中
		while(true) {
			System.out.println("请输入数据：");
			Integer i = sc.nextInt();
			if(i != 0) {
				array.add(i);
			}else {
				break;
			}
				
		}
		
		//将集合转换为数组
		Integer[] ii = new Integer[array.size()];
		array.toArray(ii);
		
		//将数组进行排序
		Arrays.sort(ii);
		
		//输出数组的最大值
		System.out.println("数组的最大值是 " + ii[ii.length - 1]);
	}
}
```



