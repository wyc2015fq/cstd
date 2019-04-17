# floyd模板（Java版） - westbrook1998的博客 - CSDN博客





2018年10月25日 21:54:27[westbrook1998](https://me.csdn.net/westbrook1998)阅读数：80








多源最短路floyd

```java
import java.util.Scanner;

public class Main{
    static int m,n,u,v,w;
    static final int N=150;
    static final int INF=0x3f3f3f3f;
    static long[][] g=new long[N][N];
    static void floyd(){
        for(int k=1;k<=n;k++){
            for(int i=1;i<=n;i++){
                for(int j=1;j<=n;j++){
                    if(g[i][j]>g[i][k]+g[k][j]){
                        g[i][j]=g[i][k]+g[k][j];
                    }
                }
            }
        }
    }
    public static void main(String[] args) {
        Scanner cin=new Scanner(System.in);
        n=cin.nextInt();
        m=cin.nextInt();
        for(int i=1;i<=n;i++){
            for(int j=1;j<=n;j++){
                g[i][j]=i==j?0:INF;
            }
        }
        while(m-->0){
            u=cin.nextInt();
            v=cin.nextInt();
            w=cin.nextInt();
            if(w<g[u][v]){
                g[u][v]=g[v][u]=w;
            }
        }
        floyd();
        for(int i=1;i<=n;i++){
            for(int j=1;j<=n;j++){
                if(j!=1){
                    System.out.print(" ");
                }
                System.out.print(g[i][j]);
            }
            System.out.println();
        }
    }
}
```



