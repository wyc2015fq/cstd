# 树状数组模板（Java版） - westbrook1998的博客 - CSDN博客





2018年10月17日 21:01:31[westbrook1998](https://me.csdn.net/westbrook1998)阅读数：79








hiho1524

树状数组求逆序对

```java
import java.util.Scanner;

public class Main {
    public static final int N=100050;
    public static int n,x;
    public static long ans;
    public static int[] c=new int[N];

    public static void main(String[] args) {
        Scanner cin=new Scanner(System.in);
        n=cin.nextInt();
        for(int i=1;i<=n;i++){
            x=cin.nextInt();
            add(x,1);
            ans+=(i-sum(x));
        }
        System.out.println(ans);
    }

    public static int lowbit(int x){
        return x&(-x);
    }
    public static void add(int x,int d){
        while(x<=n){
            c[x]+=d;
            x+=lowbit(x);
        }
    }
    public static int sum(int x){
        int res=0;
        while(x>0){
            res+=c[x];
            x-=lowbit(x);
        }
        return res;
    }
}
```



