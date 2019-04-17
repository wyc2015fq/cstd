# UVa10183——How Many Fibs? - westbrook1998的博客 - CSDN博客





2018年06月26日 13:59:03[westbrook1998](https://me.csdn.net/westbrook1998)阅读数：80








> 
Recall the definition of the Fibonacci numbers: 

  f1 := 1 

  f2 := 2 

  fn := fn−1 + fn−2 (n ≥ 3) 

  Given two numbers a and b, calculate how many Fibonacci numbers are in the range [a, b]. 

  Input 

  The input contains several test cases. Each test case consists of two non-negative integer numbers a 

  and b. Input is terminated by a = b = 0. Otherwise, a ≤ b ≤ 10100. The numbers a and b are given 

  with no superfluous leading zeros. 

  Output 

  For each test case output on a single line the number of Fibonacci numbers fi with a ≤ fi ≤ b. 

  Sample Input 

  10 100 

  1234567890 9876543210 

  0 0 

  Sample Output 

  5 

  4
大数 java暴力过  求区间内的斐波纳契数个数  可以直到斐波纳契在后面其实是很稀疏的 所以用一个1000的数字打表存起来就够了

代码：

```
ggpackage com.zxc;

import java.math.BigInteger;
import java.util.Scanner;

public class Main {
    public static void main(String[] args) {
        Scanner scanner=new Scanner(System.in);
        BigInteger a,b;
        BigInteger[] fibs=new BigInteger[1000];
        fibs[0]=new BigInteger("1");
        fibs[1]=new BigInteger("2");
        for(int i=2;i<1000;i++){
            fibs[i]=fibs[i-2].add(fibs[i-1]);
        }
        while(scanner.hasNext()){
            a=scanner.nextBigInteger();
            b=scanner.nextBigInteger();
            if(a.compareTo(BigInteger.ZERO)==0 && b.compareTo(BigInteger.ZERO)==0){
                break;
            }
            int ans=0;
            for(int i=0;i<1000;i++){
                if(fibs[i].compareTo(a)!=-1 && fibs[i].compareTo(b)!=1){
                    ans++;
                }
            }
            System.out.println(ans);
        }
    }
}
```





