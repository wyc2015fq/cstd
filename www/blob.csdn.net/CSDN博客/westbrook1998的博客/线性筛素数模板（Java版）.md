# 线性筛素数模板（Java版） - westbrook1998的博客 - CSDN博客





2018年11月12日 15:55:58[westbrook1998](https://me.csdn.net/westbrook1998)阅读数：57








```java
package OJ;

import java.util.Scanner;

public class Main {
    static final int N=(int)1e7+50;
    static int n,m,a;
    static int[] p=new int[N];
    static boolean[] check=new boolean[N];

    static void init(){
        int t;
        check[1]=true;
        for(int i=2;i<=n;i++){
            if(!check[i]){
                p[++p[0]]=i;
            }
            for(int j=1;j<=p[0];j++){
                t=i*p[j];
                if(t>n){
                    break;
                }
                check[t]=true;
                if(i%p[j]==0){
                    break;
                }
            }
        }
    }
}
```



