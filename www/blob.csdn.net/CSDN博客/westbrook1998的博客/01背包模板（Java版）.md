# 01背包模板（Java版） - westbrook1998的博客 - CSDN博客





2018年10月18日 12:25:14[westbrook1998](https://me.csdn.net/westbrook1998)阅读数：32标签：[01背包](https://so.csdn.net/so/search/s.do?q=01背包&t=blog)
个人分类：[Java算法模板](https://blog.csdn.net/westbrook1998/article/category/8228296)









poj3628

```java
import java.util.Scanner;

public class Main {
    public static void main(String[] args) {
        int n,b;
        final int N=25,MAX=20000020,INF=0x3f3f3f3f;
        int[] h=new int[N];
        int[] dp=new int[MAX];
        int sum=0;
        int ans=INF;
        Scanner cin=new Scanner(System.in);
        n=cin.nextInt();
        b=cin.nextInt();
        for(int i=0;i<n;i++){
            h[i]=cin.nextInt();
            sum+=h[i];
        }
        for(int i=0;i<n;i++){
            for(int j=sum;j>=h[i];j--){
                dp[j]=Math.max(dp[j],dp[j-h[i]]+h[i]);
                if(dp[j]>=b){
                    ans=Math.min(ans,dp[j]);
                }
            }
        }
        System.out.println(ans-b);
    }
}
```



