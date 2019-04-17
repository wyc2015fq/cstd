# Java实现算法导论中Miller-Rabin随机性素数测试 - 专注于数据挖掘算法研究和应用 - CSDN博客





2016年12月01日 09:26:40[fjssharpsword](https://me.csdn.net/fjssharpsword)阅读数：3161
所属专栏：[算法导论专栏](https://blog.csdn.net/column/details/16283.html)










Miller-Rabin测试：

费马小定理：对于素数p和任意整数a，有ap ≡ a(mod p)（同余）。反过来，满足ap ≡ a(mod p)，p也几乎一定是素数。

伪素数：如果n是一个正整数，如果存在和n互素的正整数a满足 an-1 ≡ 1(mod n)，我们说n是基于a的伪素数。如果一个数是伪素数，那么它几乎肯定是素数。

Miller-Rabin测试：不断选取不超过n-1的基b(s次)，计算是否每次都有bn-1 ≡ 1(mod n)，若每次都成立则n是素数，否则为合数。　


结合算法导论中说明的理解，参考代码如下：



```java
package cn.ansj;

import java.util.Random;

public class MillerRabin {
	private static final int ORDER = 10000;// 随机数的数量级	
	private static final int MIN = 1000; // 选择的随机数的最小值
	public static void main(String[] args){
		int x = getPrime();
		boolean flag = true;
		for (int i = 0; i < 10; i++){
			if (!isPrime(x)){
				flag = false;
				break;
			}
		}
		if (flag) System.out.println(x + ":是素数，通过测试");
	    else System.out.println(x + ":不是素数");  
	 }
	 /**
	  * 整数转为二进制
	  * 
	  * @param m整数m
	  * @return 字节数组
	  */
	 public static byte[] getByte(int m){
		 String sb = "";
		 while (m > 0){
			 sb = (m % 2) + sb;
			 m = m / 2;
		 }
		 return sb.getBytes();
	 }
	 /**
	  * 平方-乘法计算指数模运算 a^m % n
	  * 
	  * @param a底数
	  * @param m指数
	  * @param n被mod数
	  * @return
	  */
	 public static int Square_and_Mutiply(int a, int m, int n){
	  int d = 1;	  
	  byte[] bm = getByte(m);// 把m转化为二进制数
	  for (int i = 0; i < bm.length; i++){
		  d = (d * d) % n;	   
		  if (bm[i] == 49)// 二进制1等于asciI码的49
			  d = (d * a) % n;  
	  }
	  return d;
	 }
	 /**
	  * 随机选择一个奇数
	  */
	 public static int getRandom(){
		 int x = 3;
		 Random rd = new Random();
		 do
		 {
			 x = rd.nextInt(ORDER);
		 } while (x < MIN || x % 2 == 0);
		 return x;
	 }
	 /**
	  * 验证一个数是否为素数，将n-1改写为2^k * m的形式，其中m是奇数，在{2,...,n-1}中随机选取一个整数a;
	  * 
	  * @param n
	  * @return 如果是素数返回true,否则返回false
	  */
	 public static boolean isPrime(int n){  
		 int[] arr = intTOIndex(n - 1);// n-1 用2的幂表示
		 int k = arr[0];
		 int m = arr[1];	 
		 Random r = new Random();// 在{2,...,n-1}随机选择一个整数a
		 int a = 0;
		 do {
			 a = r.nextInt(n - 1);
		 } while (a < 2);
		 int b = Square_and_Mutiply(a, m, n);
		 if (b == 1)   return true;
		 for (int i = 0; i < k; i++)  {
			 if (b == (n - 1)) return true;
	         else b = (b * b) % n;   
		}
		 return false;
	 }
	 /**
	  * 将一个数改为2^k * m的形式，其中m是奇数
	  * 
	  * @param n
	  * @return arr[0]=k,arr[1]=m
	  */
	 public static int[] intTOIndex(int n){
		 int[] arr = new int[2];
		 int k = 0;
		 int x;
		 // 当n为奇数是停止循环
		 do {
			 k++;
			 n >>= 1;
		 x = n & 1;
		 } while (x == 0);
		 arr[0] = k;
		 arr[1] = n;
		 return arr;
	 }
	 /**
	  * 获取一个随机数为并且检查其为素数
	  * 
	  * @return
	  */
	public  static int getPrime(){
		int x = 0;
		while (x % 2 == 0 || !isPrime(x)){
			x = getRandom();
		}
		return x;
	 }
}
```


执行结果：



```
7207:是素数，通过测试
```








