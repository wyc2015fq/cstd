# Java大数总结 - 紫芝的博客 - CSDN博客





2018年09月26日 14:50:47[紫芝](https://me.csdn.net/qq_40507857)阅读数：51
个人分类：[Java 基础](https://blog.csdn.net/qq_40507857/article/category/8070824)









### HDU1753

话说，经过了漫长的一个多月，小明已经成长了许多，所以他改了一个名字叫“大明”。 

这时他已经不是那个只会做100以内加法的那个“小明”了，现在他甚至会任意长度的正小数的加法。 


现在，给你两个正的小数A和B，你的任务是代表大明计算出A+B的值。 

**Input**

本题目包含多组测试数据，请处理到文件结束。 

每一组测试数据在一行里面包含两个长度不大于400的正小数A和B。

**Output**

请在一行里面输出输出A+B的值，请输出最简形式。详细要求请见Sample Output。 

Sample Input

```
1.1 2.9
1.1111111111 2.3444323343
1 1.1
```

Sample Output

```
4
3.4555434454
2.1
```

```java
import java.util.*;
import java.math.*;
public class Main {
	static int maxn=105;
	static int mod=(int)(1e9+7);
	
	public static void main(String[] args) {
		Scanner cin=new Scanner(System.in);
		int ca=0;
		while(cin.hasNext()) {
			ca++;
			BigDecimal a=cin.nextBigDecimal();
			BigDecimal b=cin.nextBigDecimal();
			System.out.println(a.add(b).stripTrailingZeros().toPlainString());
            //先去掉末尾的后导0，然后转化为字符串
		}
		cin.close();
	}
}
```

### 51Nod1030

给出一个36进制的大数(0-9,A-Z)，将其转为10进制并输出。

**Input**

输入：36进制的大数，每一位用0-9,A-Z来表示，A表示10，Z表示35。(A的长度 <= 100000)

**Output**

输出：该数的10进制表示

Sample Input
`1AZ`
Sample Output
`1691`
```cpp
import java.math.*;
import java.util.*;
public class Main {
//num需要转换的数字，from原数的进制，to要转换的进制
    private static String change(String num,int from,int to) {
    	return new java.math.BigInteger(num,from).toString(to);
    }
    public static void main (String[] args) {
    	  Scanner cin = new Scanner(System.in);
         while(cin.hasNext()) {
      	  String num=cin.next();
      	  System.out.println(change(num,36,10));
         }
         cin.close();
      }
    }
```

### 超时代码

```cpp
import java.math.*;
 
import java.util.*;
public class Main {
    void solve () {
        Scanner cin = new Scanner(System.in);
        int maxn=(int)(1e5+6);  
       //int T=cin.nextInt();
        int ca=0;
       while(cin.hasNext()) {
    	    ca++;    
    	  String num=cin.next();
    	  BigInteger ans=BigInteger.ZERO;
    	  int len=num.length();
    	  int k=num.length()-1;
    	  for(int i=0;i<len;i++) {
    		  char ch=num.charAt(i);
    		  int m=0;
    		  
    		  if(ch>='0'&&ch<='9')
    			  m=ch-'0';
    		  else if(ch>='A'&&ch<='Z')
    			  m=ch-'A'+10;
    		  BigInteger p=BigInteger.valueOf(36);
    				  p=p.pow(k);
    		
    		  ans=ans.add(p.multiply(BigInteger.valueOf(m)));
    		  k--;
    	  }
    	  System.out.println(ans);
       }
       cin.close();
    }
    public static void main (String[] args) {
        Main work = new Main();
        work.solve ();
    }
}
```

### 51Nod 1166

给出一个大整数N，求不大于N的平方根的最大整数。例如：N = 8，2 * 2 < 8，3 * 3 > 8，所以输出2。

**Input**

给出一个大数N（N的长度 <= 100000)。

**Output**

输出不大于Sqrt(n)的最大整数。

Sample Input
`9`
Sample Output
`3`
```python
from math import *
from decimal import *
getcontext().prec=10**5
print(floor(Decimal((input())).sqrt()))
```

```java
import java.math.*;
import java.util.*;
public class Main {
	//牛顿迭代法
    void solve () {
        Scanner cin = new Scanner(System.in);
        while(cin.hasNext()) {
        	String s=cin.next();
            int len=s.length();
            if(len<9) {
         	   System.out.println((long)Math.sqrt(Double.valueOf(s)));
         	   continue;
            }
            BigInteger num=new BigInteger(s);
            if(len%2==0) {
            	s=s.substring(0, len/2+1);
            }else {
            	s=s.substring(0,(len+1)/2);
            }
            BigInteger x=new BigInteger(s);
            if(s.equals("1"))
            	System.out.println("1");
            else {
            	while(num.compareTo(x.multiply(x))<0) {
            		x=x.add(num.divide(x)).divide(new BigInteger("2"));
            	}
            }
            System.out.println(x);
        }
       cin.close();
    }
  //num需要转换的数字，from原数的进制，to要转换的进制
    private static String change(String num,int from,int to) {
    	return new java.math.BigInteger(num,from).toString(to);
    }
    
    public static void main (String[] args) {
        Main work = new Main();
        work.solve ();
    }
}
```

### HDU1865

You will be given a string which only contains ‘1’; You can merge two adjacent ‘1’ to be ‘2’, or leave the ‘1’ there. Surly, you may get many different results. For example, given 1111 , you can get 1111, 121, 112,211,22. Now, your work is to find the total number of result you can get. 

**Input**

The first line is a number n refers to the number of test cases. Then n lines follows, each line has a string made up of ‘1’ . The maximum length of the sequence is 200. 

**Output**

The output contain n lines, each line output the number of result you can get . 

Sample Input

```
3
1
11
11111
```

Sample Output

```
1
2
8
```

```java
import java.math.*;
import java.util.*;
public class Main {
	
    void solve () {
        Scanner cin = new Scanner(System.in);
        int maxn=207;
        BigInteger[] f=new BigInteger[maxn];
        f[1]=BigInteger.valueOf(1);
        f[2]=BigInteger.valueOf(2);
 
        for(int i=3;i<maxn;i++) {
        	f[i]=f[i-1].add(f[i-2]);
        }
        int T=cin.nextInt();
        while((T--)!=0) {
        	String n=cin.next();
        	int len=n.length();
        	System.out.println(f[len].toString());
        }
       cin.close();
    }
 
    public static void main (String[] args) {
        Main work = new Main();
        work.solve ();
    }
}
```





