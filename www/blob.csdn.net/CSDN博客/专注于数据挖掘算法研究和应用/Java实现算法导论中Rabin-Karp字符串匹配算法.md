# Java实现算法导论中Rabin-Karp字符串匹配算法 - 专注于数据挖掘算法研究和应用 - CSDN博客





2016年12月05日 09:46:49[fjssharpsword](https://me.csdn.net/fjssharpsword)阅读数：3068
所属专栏：[算法导论专栏](https://blog.csdn.net/column/details/16283.html)










Rabin-Karp算法的思想：
- 假设子串的长度为M,目标字符串的长度为N
- 计算子串的hash值
- 计算目标字符串中每个长度为M的子串的hash值（共需要计算N-M+1次）
- 比较hash值
- 如果hash值不同，字符串必然不匹配，如果hash值相同，还需要使用朴素算法再次判断

在算法导论中，hash是通过基数来实现，将每个字符串映射成d进制的数字，再利用模来实现，具体可结合导论中来理解，参考代码如下：


```java
package cn.ansj;

public class RabinKarp {
	public static void RabinKarpAlogrithm(char[] T,char[] P, int d,int q){
		int n=T.length;
	    int m=P.length;
	    if( n < m) return ;
	    int h = 1;
	    for(int i=1; i<=m-1; i++)   // 计算高度 h 
	        h = h*d%q;//m-1个d相乘然后模q
        //预处理，计算p， t0 
	    int p=0, t=0;
	    for(int i=0; i<m; i++) {
	        p = (( d*p + P[i]) % q); 
	        t = (( d*t + T[i]) % q);
	    }
        //开始匹配
	    for(int s=0; s <n-m+1; s++) {  
	    	if( p == t ){
	    		int i=0;
	            for(i=0; i<m; i++)// 进一步验证
	                if(P[i]!=T[s+i])
	                    break;
	            if(i==m) System.out.println("Pattern ocurs with shift:"+s);
	        }
	        if( s < n-m )
	            t= (d* (t - T[s]*h%q) + T[s+m])  % q;  // 计算ts+1
	    }
	    System.out.println("string matching ends");
	}
	public static void main(String[] args){
		String strT="2359023141526739921";
		String strP="31415";
		char[] T=strT.toCharArray();
		char[] P=strP.toCharArray();
		int d=10;
		int q=13;
		RabinKarp.RabinKarpAlogrithm(T,P,d,q);   
	}
}
```
执行结果：


```
Pattern ocurs with shift:6
string matching ends
```







