# Java中 CopyOnWriteArrayList 的使用 - 零度的博客专栏 - CSDN博客
2016年11月22日 17:31:44[零度anngle](https://me.csdn.net/zmx729618)阅读数：278标签：[并发集合](https://so.csdn.net/so/search/s.do?q=并发集合&t=blog)
个人分类：[Java并发编程](https://blog.csdn.net/zmx729618/article/category/6181602)
java中，List在遍历的时候，如果被修改了会抛出java.util.ConcurrentModificationException错误。
看如下代码：
**[java]**[view plain](http://blog.csdn.net/imzoer/article/details/9751591#)[copy](http://blog.csdn.net/imzoer/article/details/9751591#)[print](http://blog.csdn.net/imzoer/article/details/9751591#)[?](http://blog.csdn.net/imzoer/article/details/9751591#)
- import java.util.ArrayList;  
- import java.util.List;  
- 
- publicclass Resource3 {  
- 
- publicstaticvoid main(String[] args) throws InterruptedException {  
-         List<String> a = new ArrayList<String>();  
-         a.add("a");  
-         a.add("b");  
-         a.add("c");  
- final ArrayList<String> list = new ArrayList<String>(  
-                 a);  
-         Thread t = new Thread(new Runnable() {  
- int count = -1;  
- 
- @Override
- publicvoid run() {  
- while (true) {  
-                     list.add(count++ + "");  
-                 }  
-             }  
-         });  
-         t.setDaemon(true);  
-         t.start();  
-         Thread.currentThread().sleep(3);  
- for (String s : list) {  
-             System.out.println(s);  
-         }  
-     }  
- }  
![](http://static.blog.csdn.net/images/save_snippets.png)
```java
import java.util.ArrayList;
import java.util.List;
public class Resource3 {
	public static void main(String[] args) throws InterruptedException {
		List<String> a = new ArrayList<String>();
		a.add("a");
		a.add("b");
		a.add("c");
		final ArrayList<String> list = new ArrayList<String>(
				a);
		Thread t = new Thread(new Runnable() {
			int count = -1;
			@Override
			public void run() {
				while (true) {
					list.add(count++ + "");
				}
			}
		});
		t.setDaemon(true);
		t.start();
		Thread.currentThread().sleep(3);
		for (String s : list) {
			System.out.println(s);
		}
	}
}
```
这段代码运行的时候就会抛出java.util.ConcurrentModificationException错误。这是因为主线程在遍历list的时候，子线程在向list中添加元素。
那么有没有办法在遍历一个list的时候，还向list中添加元素呢？办法是有的。就是java concurrent包中的CopyOnWriteArrayList。
先解释下CopyOnWriteArrayList类。
CopyOnWriteArrayList类最大的特点就是，在对其实例进行修改操作（add/remove等）会新建一个数据并修改，修改完毕之后，再将原来的引用指向新的数组。这样，修改过程没有修改原来的数组。也就没有了ConcurrentModificationException错误。
看下面的代码：
**[java]**[view plain](http://blog.csdn.net/imzoer/article/details/9751591#)[copy](http://blog.csdn.net/imzoer/article/details/9751591#)[print](http://blog.csdn.net/imzoer/article/details/9751591#)[?](http://blog.csdn.net/imzoer/article/details/9751591#)
- import java.util.ArrayList;  
- import java.util.List;  
- import java.util.concurrent.CopyOnWriteArrayList;  
- 
- publicclass Resource3 {  
- 
- publicstaticvoid main(String[] args) throws InterruptedException {  
-         List<String> a = new ArrayList<String>();  
-         a.add("a");  
-         a.add("b");  
-         a.add("c");  
- final CopyOnWriteArrayList<String> list = new CopyOnWriteArrayList<String>(a);  
-         Thread t = new Thread(new Runnable() {  
- int count = -1;  
- 
- @Override
- publicvoid run() {  
- while (true) {  
-                     list.add(count++ + "");  
-                 }  
-             }  
-         });  
-         t.setDaemon(true);  
-         t.start();  
-         Thread.currentThread().sleep(3);  
- for (String s : list) {  
-             System.out.println(list.hashCode());  
-             System.out.println(s);  
-         }  
-     }  
- }  
![](http://static.blog.csdn.net/images/save_snippets.png)
```java
import java.util.ArrayList;
import java.util.List;
import java.util.concurrent.CopyOnWriteArrayList;
public class Resource3 {
	public static void main(String[] args) throws InterruptedException {
		List<String> a = new ArrayList<String>();
		a.add("a");
		a.add("b");
		a.add("c");
		final CopyOnWriteArrayList<String> list = new CopyOnWriteArrayList<String>(a);
		Thread t = new Thread(new Runnable() {
			int count = -1;
			@Override
			public void run() {
				while (true) {
					list.add(count++ + "");
				}
			}
		});
		t.setDaemon(true);
		t.start();
		Thread.currentThread().sleep(3);
		for (String s : list) {
			System.out.println(list.hashCode());
			System.out.println(s);
		}
	}
}
```
这段代码在for循环中遍历list的时候，同时会输出list的hashcode来看看list是不是同一个list了。
部分输出结果如下：
**[plain]**[view plain](http://blog.csdn.net/imzoer/article/details/9751591#)[copy](http://blog.csdn.net/imzoer/article/details/9751591#)[print](http://blog.csdn.net/imzoer/article/details/9751591#)[?](http://blog.csdn.net/imzoer/article/details/9751591#)
- 669661746  
- a  
- 2119153548  
- b  
- 471684173  
- c  
- 550648901  
- -1  
- -76447331  
- 0  
- 1638154873  
- 1  
- 921225916  
- 2  
- 1618672031  
- 3  
- 1404182932  
- 4  
- 950140076  
- 5  
- -610377050  
- 6  
- -610377050  
- 7  
- -610377050  
- 8  
- -610377050  
- 9  
- -610377050  
- 10  
- -610377050  
- 11  
- -610377050  
- 12  
![](http://static.blog.csdn.net/images/save_snippets.png)
```
669661746
a
2119153548
b
471684173
c
550648901
-1
-76447331
0
1638154873
1
921225916
2
1618672031
3
1404182932
4
950140076
5
-610377050
6
-610377050
7
-610377050
8
-610377050
9
-610377050
10
-610377050
11
-610377050
12
```
从上面的结果很容易就看出来，hashcode变化了多次，说明了list已经不是原来的list对象了。这说明了CopyOnWriteArrayList类的add函数在执行的时候确实是修改了list的数组对象。
看add函数的代码：
**[java]**[view plain](http://blog.csdn.net/imzoer/article/details/9751591#)[copy](http://blog.csdn.net/imzoer/article/details/9751591#)[print](http://blog.csdn.net/imzoer/article/details/9751591#)[?](http://blog.csdn.net/imzoer/article/details/9751591#)
- /**
-      * Appends the specified element to the end of this list.
-      *
-      * @param e element to be appended to this list
-      * @return <tt>true</tt> (as specified by {@link Collection#add})
-      */
- publicboolean add(E e) {  
- final ReentrantLock lock = this.lock;  
-     lock.lock();  
- try {  
-         Object[] elements = getArray();  
- int len = elements.length;  
-         Object[] newElements = Arrays.copyOf(elements, len + 1);  
-         newElements[len] = e;  
-         setArray(newElements);  
- returntrue;  
-     } finally {  
-         lock.unlock();  
-     }  
-     }  
![](http://static.blog.csdn.net/images/save_snippets.png)
```java
/**
     * Appends the specified element to the end of this list.
     *
     * @param e element to be appended to this list
     * @return <tt>true</tt> (as specified by {@link Collection#add})
     */
    public boolean add(E e) {
	final ReentrantLock lock = this.lock;
	lock.lock();
	try {
	    Object[] elements = getArray();
	    int len = elements.length;
	    Object[] newElements = Arrays.copyOf(elements, len + 1);
	    newElements[len] = e;
	    setArray(newElements);
	    return true;
	} finally {
	    lock.unlock();
	}
    }
```
add函数中拷贝了原来的数组并在最后加上了新元素。然后调用setArray函数将引用链接到新数组：
**[java]**[view plain](http://blog.csdn.net/imzoer/article/details/9751591#)[copy](http://blog.csdn.net/imzoer/article/details/9751591#)[print](http://blog.csdn.net/imzoer/article/details/9751591#)[?](http://blog.csdn.net/imzoer/article/details/9751591#)
- /**
-     * Sets the array.
-     */
- finalvoid setArray(Object[] a) {  
-        array = a;  
-    }  
![](http://static.blog.csdn.net/images/save_snippets.png)
```java
/**
     * Sets the array.
     */
    final void setArray(Object[] a) {
        array = a;
    }
```
[](http://blog.csdn.net/imzoer/article/details/9751591#)[](http://blog.csdn.net/imzoer/article/details/9751591#)[](http://blog.csdn.net/imzoer/article/details/9751591#)[](http://blog.csdn.net/imzoer/article/details/9751591#)[](http://blog.csdn.net/imzoer/article/details/9751591#)[](http://blog.csdn.net/imzoer/article/details/9751591#)
