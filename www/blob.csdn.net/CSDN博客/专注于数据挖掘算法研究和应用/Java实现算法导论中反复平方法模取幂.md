# Java实现算法导论中反复平方法模取幂 - 专注于数据挖掘算法研究和应用 - CSDN博客





2016年11月29日 10:31:19[fjssharpsword](https://me.csdn.net/fjssharpsword)阅读数：2062
个人分类：[Algorithm																[Java](https://blog.csdn.net/fjssharpsword/article/category/6480440)](https://blog.csdn.net/fjssharpsword/article/category/6309933)

所属专栏：[算法导论专栏](https://blog.csdn.net/column/details/16283.html)









在众多的加密[算法](http://lib.csdn.net/base/datastructure)中都需要进行幂的取模运算，比如在RSA算法中需要计算d=ne mod N，我们称之为幂模算法，其中：
- N=p*q（p，q为大素数）
- n为加密数据，n<N
- e为公钥，d为私钥，满足关系ed≡1 (mod (p-1)*(q-1))


其中n,e都是非常大的数，ne mod N用算法导论中的反复平方法，具体代码如下：



```java
package cn.ansj;

public class ModulaExponentiation {
	public static void main(String args[]) {  
		int a=7;
		int b=560;
		int n=561;
		int c=0;
		int d=1;
		//b表示成位二进制
		String bb=Integer.toBinaryString(b);
		System.out.println("b表示成二进制："+bb);	
		//for (int i=bb.length()-1;i>=0;i--){
		for (int i=0;i<bb.length();i++){
			c=2*c;
			d=(d*d)%n;
			if (bb.charAt(i)=='1'){
				c=c+1;
				d=(d*a)%n;
			}
			System.out.println("b"+(bb.length()-i-1)+"="+bb.charAt(i)+";"+"c="+c+";"+"d="+d+";");//打印迭代结果
		}	
		/*
		char[] cb=bb.toCharArray();
		for (int i=cb.length-1;i>=0;i--){
			c=2*c;
			d=(d*d)%n;
			if (cb[i]=='1'){
				c=c+1;
				d=(d*a)%n;
			}
			System.out.println("b"+i+"="+cb[i]+";"+"c="+c+";"+"d="+d+";");//打印迭代结果
		}	
		*/	
	}
}
```
执行结果：



```
b表示成二进制：1000110000
b9=1;c=1;d=7;
b8=0;c=2;d=49;
b7=0;c=4;d=157;
b6=0;c=8;d=526;
b5=1;c=17;d=160;
b4=1;c=35;d=241;
b3=0;c=70;d=298;
b2=0;c=140;d=166;
b1=0;c=280;d=67;
b0=0;c=560;d=1;
```









