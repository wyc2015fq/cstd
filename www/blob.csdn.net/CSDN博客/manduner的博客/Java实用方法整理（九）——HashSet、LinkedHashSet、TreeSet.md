
# Java实用方法整理（九）——HashSet、LinkedHashSet、TreeSet - manduner的博客 - CSDN博客


2018年06月06日 23:15:04[Manduner_TJU](https://me.csdn.net/manduner)阅读数：151


# 一、HashSet
## 1，描述
（1）元素唯一
（2）元素无序（存储和取出不一致）
## 2，如何实现元素的唯一
（1）自定义元素唯一性的规则
（2）在元素所属对象类中重写hashCode()，equals()方法
## 3，应用案例
**需求：**产生10个[1-20]之间的随机数，要求不能重复
```python
import java.util.HashSet;
import java.util.Random;
/**
* @author Manduner_TJU
* @version 创建时间：2018年6月7日下午10:34:02
*/
public class HashSetDemo {
	public static void main(String[] args) {
		//创建Random对象
		Random r = new Random();
		
		//创建HashSet
		HashSet<Integer> hs = new HashSet<Integer>();
		
		//创建随机数并存入HashSet集合
		while(hs.size() < 10) {
			Integer i = r.nextInt(20) + 1;
			hs.add(i);
		}
		
		//遍历
		for(Integer i : hs) {
			System.out.println(i);
		}
	}
}
```
# 二、LinkedHashSet
## 1，描述
（1）底层 数据结构由哈希表和链表组成
（2）哈希表保证元素的唯一性
（3）链表保证元素有序（存储和取出一致）
## 2，应用案例
```python
import java.util.LinkedHashSet;
public class LinkedHashSetDemo {
	public static void main(String[] args) {
		LinkedHashSet<String> hs = new LinkedHashSet<String>();
		hs.add("my");
		hs.add("name");
		hs.add("manduener");
		hs.add("manduener");
		hs.add("name");
		// 遍历
		for (String s : hs) {
			System.out.println(s);
		}
	}
}
```

# 三、TreeSet
## 1，描述
（1）元素唯一
（2）能够对元素按照某种规则进行排序
1）自然排序
i、使用TreeSet的无参构造方法
ii、让元素所属的类实现自然排序接口   Comparable
2）比较器排序
i、使用TreeSet的带参构造方法
ii、让集合的构造方法接收一个比较器接口的子类对象 Comparator
（3）底层结构是红黑树
1）有自平衡的特点
2）遍历采用中序遍历的方法，即左、根、右，以达到集合元素按序取出

## 2，应用案例一：自然排序
**需求：**使用TreeSet存储自定义的Student对象，并按照Student对象中学生姓名的长度排序。Student类的定义以及TreeSet的实现如下。
**Student类**
```python
/**
* @author Manduner_TJU
* @version 创建时间：2018年6月6日下午9:37:34
*/
//Student类继承了Comparable<Student>后，TreeSet集合在添加元素的时候会根据重写的compareTo方法，对所添加元素进行去重，
//并会以自定义的排序要求，将元素以红黑数的形式存放起来
public class Student implements Comparable<Student> {
	private String name;
	private int age;
	
	public Student(String name, int age) {
		super();
		this.name = name;
		this.age = age;
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
	
	
	@Override
	public int compareTo(Student s) {
		// TODO Auto-generated method stub
		//return 0;
	
		//根据需求，计算待添加元素和TreeSet中已有元素的长度差值
		int num = this.getName().length() - s.getName().length();
		
		if(num == 0) { //如果长度一样，需要进一步作出如下判断
			if(this.getName().equals(s.getName())) { //如果名字一样，并且是同一个人，需要再判断年龄是否是一样的
				num = this.getAge() - s.getAge();
			} else { //如果名字不一样，按字典顺序比较两个字符串
				num = this.getName().compareTo(s.getName());
			}
		}
		
		//如果名字长度不一样，则将新来元素直接添加到TreeSet中
		return num;
	}
	
}
```

**实现类**
```python
import java.util.TreeSet;
import user.Student;
/**
* @author Manduner_TJU
* @version 创建时间：2018年6月6日下午9:43:47
*/
public class TreeSetDemo {
	public static void main(String[] args) {
		TreeSet<Student> ts = new TreeSet<Student>();
		
		Student s1 = new Student("baiqi",38);
		Student s2 = new Student("yingji",30);
		Student s3 = new Student("baiqi",38);
		Student s4 = new Student("qinhuiwenwang",60);
		Student s5 = new Student("minyue",25);
		Student s6 = new Student("baiqi",26);
		Student s7 = new Student("laiyy",26);
		
		ts.add(s1);
		ts.add(s2);
		ts.add(s3);
		ts.add(s4);
		ts.add(s5);
		ts.add(s6);
		ts.add(s7);
		
		
		//增强for遍历集合
		//当没有指定TreeSet集合按照什么规则去重和排序时，会有如下错误：
		//java.lang.ClassCastException: user.Student cannot be cast to java.lang.Comparable
		//错误的意思表示：需要在Student类中实现Comparable，并重写compareTo方法
		
		
		for(Student s : ts) {
			System.out.println(s.getName()+"  "+s.getAge());
		}
		
	}
}
```

## 3，应用案例二：比较器排序
**需求：**使用TreeSet存储自定义的Student对象，并按照Student对象中学生姓名的长度排序。Student类的定义以及TreeSet的实现如下。
**Student类**
```python
public class Student {
	private String name;
	private int age;
	public Student() {
		super();
	}
	public Student(String name, int age) {
		super();
		this.name = name;
		this.age = age;
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
}
```

**实现类（****注：Java8中已经没有匿名内部类和局部内部类了，所以下面这段代码不能在java8中运行****）**
```python
public class TreeSetDemo {
	public static void main(String[] args) {
		// 如果一个方法的参数是接口，那么真正要的是接口的实现类的对象
		// 而匿名内部类就可以实现这个东西
		TreeSet<Student> ts = new TreeSet<Student>(new Comparator<Student>() {
			@Override
			public int compare(Student s1, Student s2) {
				// 姓名长度
				int num = s1.getName().length() - s2.getName().length();
				// 姓名内容
				int num2 = num == 0 ? s1.getName().compareTo(s2.getName())
						: num;
				// 年龄
				int num3 = num2 == 0 ? s1.getAge() - s2.getAge() : num2;
				return num3;
			}
		});
		Student s1 = new Student("baiqi",38);
		Student s2 = new Student("yingji",30);
		Student s3 = new Student("baiqi",38);
		Student s4 = new Student("qinhuiwenwang",60);
		Student s5 = new Student("minyue",25);
		Student s6 = new Student("baiqi",26);
		Student s7 = new Student("laiyy",26);
		
		ts.add(s1);
		ts.add(s2);
		ts.add(s3);
		ts.add(s4);
		ts.add(s5);
		ts.add(s6);
		ts.add(s7);

		// 遍历
		for (Student s : ts) {
			System.out.println(s.getName() + "  " + s.getAge());
		}
	}
}
```


