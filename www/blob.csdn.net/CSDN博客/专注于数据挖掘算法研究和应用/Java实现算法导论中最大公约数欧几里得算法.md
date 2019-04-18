# Java实现算法导论中最大公约数欧几里得算法 - 专注于数据挖掘算法研究和应用 - CSDN博客





2016年11月25日 11:57:06[fjssharpsword](https://me.csdn.net/fjssharpsword)阅读数：2169
所属专栏：[算法导论专栏](https://blog.csdn.net/column/details/16283.html)









最大公约数的欧几里得算法，代码如下：



```java
package cn.ansj;

public class GCD {
	public static void main(String args[]) {  
		GCD g = new GCD();  
		//d=gcd(a,b)=gcd(b,a mod b)
        int igcd = g.getGcd(42823, 6409);  
        System.out.println("Iterator Algorithm get gcd: " + igcd);  
        int rgcd=g.Euclid(42823, 6409);
        System.out.println("recursion Algorithm get gcd: " + rgcd); 
    }  
	
	/** 
     * author  欧几里得算法递归实现
     */  
	public int Euclid(int a,int b){
	    if (b==0) return a;
	    else return Euclid(b,a%b);
	}

    /** 
     * author  欧几里得算法迭代实现
     */  
    public int getGcd(int i, int j) {  
        int k;   
        while ((k=i%j) != 0) {            
            i = j;  
            j = k;  
        }  
        return j;  
    }  
}
```


执行结果：



```
Iterator Algorithm get gcd: 17
recursion Algorithm get gcd: 17
```







