
# Java实用方法整理（十三）——异常处理方法之try...catch... - manduner的博客 - CSDN博客


2018年08月24日 11:27:50[Manduner_TJU](https://me.csdn.net/manduner)阅读数：50


# 一：处理一个异常
## 1，格式
try{
......
}catch(异常名 变量){
......
}
## 2，实例
```python
package exceptionUtils;
/**
* @author Manduner_TJU
* @version 创建时间：2018年8月24日上午9:27:44
*/
public class Test {
	public static void main(String[] args) {
		int a = 6;
		int b = 0;
		
		try {
			System.out.println(a/b);
		}catch(ArithmeticException e) {
			System.out.println("操作异常!!");
		}
		
		//出现异常并不影响程序向下执行
		System.out.println("程序运行结束!");
	}
}
```
# 二：处理多个异常
## 1，格式：
### （1）方式一：每个异常都写一个try...catch...
try{
//异常点...
......
}catch{
......
}
try{
//异常点...
......
}catch{
......
}

### （2）方式二：一个try，匹配多个catch
try{
//异常一
......
//异常二
......
//异常......
......
}catch(异常名 变量){
......
}catch(异常名 变量){
......
}catch(异常名 变量){
......
}
### （3）方式三：一个try...catch...就可以搞定
try{
......
}catch(异常名1 | 异常名 2 |  ... 变量){
......
}
## 2，实例
### （1）方式一：每个异常都写一个try...catch...
```python
package exceptionUtils;
/**
* @author Manduner_TJU
* @version 创建时间：2018年8月24日上午9:27:44
*/
public class TryCatchDemo {
	public static void main(String[] args) {
		int a = 6;
		int b = 0;
		int[] arr = {1, 2, 3};
		
		//第一个异常
		try {
			System.out.println(a/b); //ArithmeticException
			
		}catch(ArithmeticException e) {
			System.out.println("操作异常  ArithmeticException!!");
		}
		
		//第二个异常
		try {
			System.out.println(arr[3]); //ArrayIndexOutOfBoundsException
		}
		catch(ArrayIndexOutOfBoundsException e) {
			System.out.println("操作异常  ArrayIndexOutOfBoundsException!!");
		}
		
		System.out.println("程序运行结束!");
	}
}
		
		System.out.println("程序运行结束!");
	}
}
```
### （2）方式二：一个try，匹配多个catch
```python
package exceptionUtils;
/**
* @author Manduner_TJU
* @version 创建时间：2018年8月24日上午9:27:44
*/
public class TryCatchDemo {
	public static void main(String[] args) {
		int a = 6;
		int b = 0;
		int[] arr = {1, 2, 3};
		
		try {
			System.out.println(a/b); //ArithmeticException
			System.out.println(arr[3]);
		}catch(ArithmeticException e) {
			System.out.println("操作异常  ArithmeticException!!");
		}catch(ArrayIndexOutOfBoundsException e) {
			System.out.println("操作异常  ArrayIndexOutOfBoundsException!!");
		}
		
		System.out.println("程序运行结束!");
	}
}
```
### （3）方式三：一个try...catch...就可以搞定
```python
package exceptionUtils;
/**
* @author Manduner_TJU
* @version 创建时间：2018年8月24日上午9:27:44
*/
public class TryCatchDemo {
	public static void main(String[] args) {
		int a = 6;
		int b = 0;
		int[] arr = {1, 2, 3};
		
		try {
			System.out.println(a/b); //ArithmeticException
			System.out.println(arr[3]);
		}catch(ArithmeticException | ArrayIndexOutOfBoundsException e) {
			System.out.println("操作异常!!");
		}
		
		System.out.println("程序运行结束!");
	}
}
```

# 三：Exception 简单粗暴的方式
不管有一个异常还是多个异常，直接在catch中写一个Exception作为异常名，就可以搞定
```python
package exceptionUtils;
/**
* @author Manduner_TJU
* @version 创建时间：2018年8月24日上午9:27:44
*/
public class TryCatchDemo {
	public static void main(String[] args) {
		int a = 6;
		int b = 0;
		int[] arr = {1, 2, 3};
		
		try {
			System.out.println(a/b);
			System.out.println(arr[3]);
		}catch(Exception e) {
			System.out.println("操作异常!!");
		}
		
		System.out.println("程序运行结束!");
	}
}
```



