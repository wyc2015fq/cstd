
# Java实用方法整理（一） - manduner的博客 - CSDN博客


2018年03月09日 19:26:19[Manduner_TJU](https://me.csdn.net/manduner)阅读数：242


# 1，tocharArray()
将字符串转换成字符数组
```python
String s = "abhnmf";
char[] ch = s.toCharArray();
```
# 2，String类下的valueOf()
将任意类型的数据转换成字符串，且可直接将字符数组转换成字符串
```python
char[] chs = {'l','o','v','e','y','o','u'};
int x = 1314;
String s1 = String.valueOf(chs);
String s2 = String.valueOf(x);
System.out.println(s1+s2); #输出结果为：loveyou1314
```
# 3，Arrays类
针对数组进行操作的工具类，比如排序、查找（含二分查找等）、字符串形式表示数组内容等。方法的详细用法查看Java的API说明文档。说明这个类的原因是，以后涉及到排序或者查找，直接可以只用该类的相关方法，省时省力！
（1）字符串形式表示数组内容用法举例
```python
int[] arry = {23,12,56,41,32,78,10};
		
//字符串形式表示数组内容
String s = Arrays.toString(arry);
System.out.println(s);
```
（2）排序（底层用的快速排序）
```python
//排序
Arrays.sort(arry);
```
（3）二分查找
```python
Arrays.binarySearch(arry, 32);
```
# 4，基本类型包装类的引入
（1）为了对基本数据类型进行更多的操作，更方便的操作，Java就针对每一种基本数据类型提供了对应的类 类型，即，包装类类型。
|基本类型|包装类类型|
|byte|Byte|
|short|Short|
|int
|Integer
|
|long|Long|
|float|Float|
|double|Double|
|char|Character|
|boolean|Boolean|
（2）基本类型和String类型的相互转换
1）int类型（其他类型类似的操作）转换成String类型
```python
int num = 100;
String s = String.valueOf(num);
```
2）String类型转换成int类型（转换成其他类型有类似的操作）
```python
String s = "100";
int x = Integer.parseInt(x);
```
（3）常用的基本进制转换
1）十进制到二进制，八进制，十六进制
```python
//public static String toBinaryString(int i); //10进制到二进制
String s2 = Integer.toBinaryString(100);
//public static String toOctalString(int i); //10进制到8进制
String s8 = Integer.toOctalString(100);
//public static String toHexString(int i); //10进制到16进制
String s16 = Integer.toHexString(100);
```
2）十进制到所有进制（进制范围为（2,36））的通用方法
```python
//public static String toString(int i,int radix)
System.out.println(Integer.toString(100,10)); //默认10进制
System.out.println(Integer.toString(100,2)); //十进制到二进制
System.out.println(Integer.toString(100,8)); //十进制到八进制
System.out.println(Integer.toString(100,16)); //十进制到十六进制
System.out.println(Integer.toString(100,5)); //十进制到5进制
System.out.println(Integer.toString(100,19)); //十进制到19进制
```
3）其他所有进制到十进制
```python
System.out.println(Integer.parseInt("100",10));//将“100”视为10进制，直接按照10进制输出
System.out.println(Integer.parseInt("100",2));//将“100”视为2进制串（即，串中每一位代表1位二进制位），并将该串转换为10进制
System.out.println(Integer.parseInt("100",8));//将“100”视为8进制串（即，串中每一位代表3位二进制位），并将该串转换为10进制
System.out.println(Integer.parseInt("100",16));//将“100”视为16进制串（即，串中每一位代表4位二进制位），并将该串转换为10进制
```
# 5，Integer直接赋值问题
Integer的数据直接赋值，如果数据在-128到127之间，会直接从缓冲池里获取数据。如下案例：看程序写结果
```python
public class IntegerDemo {
	public static void main(String[] args) {
		Integer i1 = new Integer(127);
		Integer i2 = new Integer(127);
		System.out.println(i1 == i2); //false 因为对象间比的是地址值
		System.out.println(i1.equals(i2)); //true
		System.out.println("----------------");
		
		Integer i3 = new Integer(128);
		Integer i4 = new Integer(128);
		System.out.println(i3 == i4); //false
		System.out.println(i3.equals(i4)); //true
		System.out.println("----------------");
		
		Integer i5 = 127;
		Integer i6 = 127;
		System.out.println(i5 == i6); //true
		System.out.println(i5.equals(i6)); //true
		System.out.println("----------------");
		
		Integer i7 = 128;
		Integer i8 = 128;
		System.out.println(i7 == i8); //false  因为128超出了缓冲池的范围（-128,127）
		System.out.println(i7.equals(i8)); //true
	}
}
```

# 6，System类下常用的方法
## （1）public static void exit(int status)
1）终止当前正在运行的Java虚拟机。参数用作状态码；根据惯例，非0的状态码表示异常终止。
2）System.exit(0);  //该行代码 以下的代码段不再执行
## （2）public static long currentTimeMillis()
1)返回以毫秒为单位的当前时间
2）用途之一：测试某段代码的运行时间
```python
public class SystemDemo {
	public static void main(String[] args) {
		
		long start = System.currentTimeMillis();
		for (int x = 0; x < 100000; x++) {
			System.out.println("hello" + x);
		}
		long end = System.currentTimeMillis();
		System.out.println("共耗时：" + (end - start) + "毫秒");
	}
}
```
## （3）public static void arraycopy(Object src,int srcPos,Object dest,int destPos,int length)
```python
import java.util.Arrays;
public class SystemDemo {
	public static void main(String[] args) {
		// 定义数组
		int[] arr = { 11, 22, 33, 44, 55 };
		int[] arr2 = { 6, 7, 8, import java.util.Arrays;
public class SystemDemo {
	public static void main(String[] args) {
		// 定义数组
		int[] arr = { 11, 77, 65, 44, 55 };
		int[] arr2 = { 6, 7, 8, 9, 10 };
		System.arraycopy(arr, 1, arr2, 2, 2);
		System.out.println(Arrays.toString(arr)); //输出[11, 77, 65, 44, 55]
		System.out.println(Arrays.toString(arr2));//输出[6, 7, 77, 65, 10]
	}
}
```

