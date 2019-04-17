# Java —— 枚举类（enum 类） - Alex_McAvoy的博客 - CSDN博客





2018年10月23日 14:27:44[Alex_McAvoy](https://me.csdn.net/u011815404)阅读数：206
个人分类：[Java学习笔记](https://blog.csdn.net/u011815404/article/category/8863016)

所属专栏：[Java 学习笔记](https://blog.csdn.net/column/details/34317.html)









有时一个类的对象是有限且固定的，在这种情况下，为了方便，可以使用枚举类。

# 【自定义枚举类】

自定义枚举类的步骤分以下四步：
- 提供类的属性，声明为：private、final
- 将属性在构造器内初始化
- 通过公共方法调用属性
- 在枚举类内部创建枚举对象

```java
import java.util.*;
public class Main {
	public static void main(String[] args) {
		Season spring=Season.SPRING;
		System.out.println(spring.getSeasonName());
		System.out.println(spring.getSeasonNumber());
	}
}
class Season{
	//提供类的属性
	private final String name;
	private final int number;

	//构造器内初始化
	private Season(String name,int number) {
		this.name = name;
		this.number = number;
	}

	//通过公共方法调用属性
	public String getSeasonName() {
		return name;
	}
	public int getSeasonNumber() {
		return number;
	}
        @Override
	public String toString() {
		return "Season [name=" + name + ", number=" + number + "]";
	}	
	
	//创建枚举类对象
	public static final Season SPRING = new Season("spring",1);
	public static final Season SUMMER = new Season("summer",2);
	public static final Season AUTUMN = new Season("autumn",3);
	public static final Season WINTER = new Season("winter",4);
	
}
```

# 【使用 enum 定义枚举类】

使用 enum 定义枚举类与自定义枚举类方法大致相同，不同的是要将创建枚举类对象放至类的最前，且初始化格式如下

```cpp
enum Season{
	//创建枚举类对象
	SPRING("spring",1),
	SUMMER("summer",2),
	AUTUMN("autumn",3),
	WINTER("winter",4);
	
	//提供类的属性
	private final String name;
	private final int number;

	//构造器内初始化
	private Season(String name,int number) {
		this.name = name;
		this.number = number;
	}

	//通过公共方法调用属性
	public String getSeasonName() {
		return name;
	}
	public int getSeasonNumber() {
		return number;
	}
        @Override
	public String toString() {
		return "Season [name=" + name + ", number=" + number + "]";
	}	
}
```

枚举类的最大特点是增加了一些内置方法，如：values() 方法可以将枚举对象转成数组，valueOf(String str) 方法可以将写入字符串转回枚举对象

```java
public static void main(String[] args) {
	Season[] seasons=Season.values();
	for(Season i:seasons)
		System.out.println(i);
    /* 运行结果
     * Season [name=spring, number=1]
     * Season [name=summer, number=2]
     * Season [name=autumn, number=3]
     * Season [name=winter, number=4]
    */
	
    String str="SPRING";
	Season summer=Season.valueOf(str);
	System.out.println(spring);
    //运行结果：Season [name=spring, number=1]

}
```

关于枚举类的更多用法：[点击这里](https://blog.csdn.net/qq_27093465/article/details/52180865)



