
# Java实用方法整理（六）——泛型 - manduner的博客 - CSDN博客


2018年05月24日 22:52:17[Manduner_TJU](https://me.csdn.net/manduner)阅读数：94


# 1，描述
泛型：是一种吧类型明确的工作推迟到创建对象或者调用方法的时候才去明确的特殊类型。参数化类型，即吧类型当作参数一样的传递
# 2，格式
<数据类型>
注：此处的数据类型只能是引用类型
# 3，好处
（1）把运行期间的问题提前到了编译期间
（2）避免了强制类型转换
（3）优化了程序设计
# 4，实例
```python
import java.util.ArrayList;
import java.util.Iterator;
/**
* @author Manduner_TJU
* @version 创建时间：2018年5月23日下午9:30:17
*/
public class ArryListDemo {
	public static void main(String[] args) {
		ArrayList<String> arr = new ArrayList<String>();
		ArrayList<String> arr_new = new ArrayList<String>();
		
		arr.add("hello");
		arr.add("world");
		arr.add("java");
		arr.add("world");
		arr.add("hello");
		
	 Iterator<String> it = arr.iterator();
		
		while(it.hasNext()) {
			//String temp = (String)it.next(); //因为在创建迭代器的时候已经限定了对象的类型，所以在此处避免了强制类型转换
			String temp = it.next();
			if(!arr_new.contains(temp)) {
				arr_new.add(temp);
			}
		}
		 
		System.out.println(arr_new);
	}
}
```
# 5，泛型通配符
## （1）泛型如果在前后都明确写出了，那么前后必须一致，如下：第一个是错误的写法，第二是正确的写法
```python
Collection<Object> c1 = new ArrayList<Student>();//错误的写法
Collection<Student> c2 = new ArrayList<Student>();//正确的写法
```
## （2）通配符之“？”用法
```python
Collection<?> c1 = new ArrayList<Student>();
Collection<?> c2 = new ArrayList<Dog>();
```
## （3）通配符之“？ extends E”用法
### 1）描述
向下限定，“？”包含E及其子类
### 2）实例（以下例子中，Dog、Cat继承了Animal这个类）
```python
Collection<? extends Animal> c1 = new ArrayList<Object>();//错误的写法
Collection<? extends Animal> c2 = new ArrayList<Animal>();//正确的写法
Collection<? extends Animal> c3 = new ArrayList<Cat>();//正确的写法
Collection<? extends Animal> c4 = new ArrayList<Dog>();//正确的写法
```
## （4）通配符之“？supper E”用法
用法和第（3）相似，只是supper指的是向上限定

