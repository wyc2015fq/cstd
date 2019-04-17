# Java —— 进制转换 - Alex_McAvoy的博客 - CSDN博客





2018年09月10日 10:58:34[Alex_McAvoy](https://me.csdn.net/u011815404)阅读数：70
个人分类：[Java学习笔记](https://blog.csdn.net/u011815404/article/category/8863016)

所属专栏：[Java 学习笔记](https://blog.csdn.net/column/details/34317.html)









# 【进制】
- 二进制：0、1，满 2 进 1，0b 或 0B 开头
- 八进制：0-7，满 8 进 1，0 开头
- 十进制：0-9，满 10 进 1
- 十六进制：0-9 及 A-F(不区分大小写)，满 16 进 1，0x 或 0X 开头

# 【原码、反码、补码】
- 所有数字在计算机底层均以二进制形式存储
- 进制的基本转换，是以二进制为桥梁的，任意两进制互转，都需先转为二进制
- 原码：直接将一根数值转换为二进制数
- 反码：除符号位外，对原码按位取反
- 对于正数，原码、反码、补码，三码合一
- 对于负数，其补码是反码加1

# 【任意进制互转框架】

```java
import java.util.*;
public class Main {
	public static void main(String args[]) {
		Scanner input = new Scanner(System.in);
		while(input.hasNext()) {
			int n=input.nextInt();
			System.out.println("转换前的进制："+n);
			int m=input.nextInt();
			System.out.println("转换后的进制："+m);
			String str=input.next();
			System.out.print("要转换的数："+str);
			
			int a=Integer.valueOf(str,n);
			String res=Integer.toString(a,m).toUpperCase();
			System.out.println("结果："+res);
		}
	}
}
```

# 【十进制转其他进制】

十进制转二、八、十六进制，可以通过包装类中的方法来转换

```java
int num=10,res;
res=Integer.toBinaryString(num);//转2进制
res=Integer.toOctalString(num);//转8进制
res=Integer.toHexString(num);//转16进制
```

# 【其他进制转十进制】

当需要其他进制转十进制时，常通过包装类中的 valueOf() 方法（返回值为对象）与 parseInt() 方法（返回值为整数）来进行转换

关于包装类：[点击这里](https://blog.csdn.net/u011815404/article/details/82931291)

```java
int a;
a=Integer.valueOf("FFFF",16).toString();
a=Integer.valueOf("17",8).toString();
a=Integer.valueOf("0101",2).toString();
a=Integer.parseInt("FFFF",16);
a=Integer.parseInt("17",8);
a=Integer.parseInt("0101",2);
```

**注：对于二进制、八进制和十六进制之间的转换，可先转换为十进制，在用十进制转多进制的相应方法进行转换**



