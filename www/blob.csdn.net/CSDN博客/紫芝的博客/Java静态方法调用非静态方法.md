# Java静态方法调用非静态方法 - 紫芝的博客 - CSDN博客





2018年09月19日 20:23:42[紫芝](https://me.csdn.net/qq_40507857)阅读数：218








静态方法**(static mehod)**不能直接调用非静态方法**(non-static method),**可以通过一个对象的引用传入静态方法中，再去调用该对象的非静态方法。在主函数（static方法）中，经常创建某个类的实例，再利用其引用变量调用他的非静态方法。

```java
public class staticMethodTest {
	void nonStaticMethod() {
		System.out.println("This is a non static method");
	}
	static void staticMethod(staticMethodTest s) {
		System.out.println("This is a static method");
		s.nonStaticMethod();
	}
	public static void main(String[] args) {
		staticMethodTest obj=new staticMethodTest();
		staticMethod(obj);
	}
}
```

在类中创建一个非静态方法，该方法输出一个九九乘法表

```java
package work1;
import java.util.*;
public class mul {
	void mulTable()
	{
		int i=1;
		while(i<=9) {
			int j=1;
			while(j<=i) {
				System.out.print(j+"*"+i+"="+i*j+" ");
				j++;
			}
			System.out.println();
			i++;
		}
	}
	static void print(mul s) {
		s.mulTable();
	}
	public static void main(String[] args) {
		mul obj = new mul();
		print(obj);
	}
}
```





