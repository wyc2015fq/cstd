# 拓展欧几里德求逆元模板（Java版） - westbrook1998的博客 - CSDN博客





2018年11月08日 23:18:10[westbrook1998](https://me.csdn.net/westbrook1998)阅读数：32标签：[拓展欧几里德																[模n逆元](https://so.csdn.net/so/search/s.do?q=模n逆元&t=blog)](https://so.csdn.net/so/search/s.do?q=拓展欧几里德&t=blog)
个人分类：[Java算法模板](https://blog.csdn.net/westbrook1998/article/category/8228296)








Java没有传引用这种东西，long的包装类又是final的，所以只能用数组来代替一下

拓展欧几里德这东西真实难懂，逆元这东西也是难懂

```java
import java.io.*;
import java.util.Scanner;
import java.util.StringTokenizer;

public class Main {

    static long n,b;
    static final int MOD=9973;

    static long exgcd(long a,long b,long[] x,long[] y){
        if(b==0){
            x[0]= 1L;
            y[0]= 0L;
            return a;
        }
        long d=exgcd(b,a%b,x,y);
        long tmp=x[0];
        x[0]=y[0];
        y[0]=tmp-a/b*y[0];
        return d;
    }

    static long inv(long a,long m){
        long[] x={0};
        long[] y={0};
        long d=exgcd(a,m,x,y);
        if(d==1){
            return (x[0]%m+m)%m;
        }
        return -1;
    }
    public static void main(String[] args) {
        Scanner cin=new Scanner(System.in);
        int t=cin.nextInt();
        while(t-->0){
            n=cin.nextLong();
            b=cin.nextLong();
            long c=inv(b,MOD);
            System.out.println(n*c%MOD);
        }
    }
}
```




