# 最长上升子序列模板（Java版） - westbrook1998的博客 - CSDN博客





2018年11月02日 22:30:33[westbrook1998](https://me.csdn.net/westbrook1998)阅读数：170








hdu1257

d[i]就是保存到i的最大上升子序列长度

g[i]保存长度为i的最大上升子序列的的最小编号（因为编号越小越有机会有上升子序列的机会）
```java
import java.util.Scanner;

public class Main{
    static int n;
    static final int N=10050;
    static final int INF=0x3f3f3f3f;
    static int[] a=new int[N];
    static int[] g=new int[N];
    static int[] d=new int[N];
    public static void main(String[] args) throws Exception {
        Scanner cin=new Scanner(System.in);
        while(cin.hasNext()){
            n=cin.nextInt();
            for(int i=0;i<n;i++){
                a[i]=cin.nextInt();
            }
            /**
             * O(n^2)
             */
//            for(int i=1;i<n;i++){
//                for(int j=0;j<i;j++){
//                    if(a[j]<a[i]) {
//                        d[i] = Math.max(d[j], 0) + 1;
//                    }
//                }
//            }
            /**
             * O(nlogn)
             */
            for(int i=1;i<=n;i++){
                g[i]=INF;
            }
            int ans=0;
            for(int i=0;i<n;i++){
                int k=lowerBound(g,1,n,a[i]);
                g[k]=a[i];
                d[i]=k;
                ans=Math.max(ans,k);
            }
            System.out.println(ans);
        }
    }

    static int lowerBound(int[] nums,int l,int r,int v){
        while(l<r){
            int m=l+((r-l)>>1);
            if(nums[m]>=v){
                r=m;
            }
            else{
                l=m+1;
            }
        }
        return l;
    }
}
```




