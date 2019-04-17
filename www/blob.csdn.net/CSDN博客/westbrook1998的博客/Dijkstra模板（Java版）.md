# Dijkstra模板（Java版） - westbrook1998的博客 - CSDN博客





2018年10月16日 22:53:19[westbrook1998](https://me.csdn.net/westbrook1998)阅读数：59








为了蓝桥杯准备一下java的模板

hdu2455

```java
import java.util.Arrays;
import java.util.Scanner;

public class Main {
    public static final int N=1050;
    public static final int INF=0x3f3f3f3f;
    public static int[][] cost=new int[N][N];
    public static int[] dis=new int[N];
    public static boolean[] vis=new boolean[N];
    public static int n,m,s,t;
    public static int u,v,w;
    public static void main(String[] args) {
        Scanner scanner=new Scanner(System.in);
        while(scanner.hasNextInt()){
            n=scanner.nextInt();
            m=scanner.nextInt();
            if(n==0 && m==0){
                break;
            }
            for(int i=0;i<N;i++){
                for(int j=0;j<N;j++){
                    cost[i][j]=INF;
                }
            }
            for(int i=0;i<m;i++){
                u=scanner.nextInt();
                v=scanner.nextInt();
                w=scanner.nextInt();
                cost[u][v]=cost[v][u]=w;
            }
            s=1;
            t=n;
            Dijkstra();
            System.out.println(dis[t]);
        }
    }

    public static void Dijkstra(){
        for(int i=1;i<=n;i++){
            dis[i]=INF;
            vis[i]=false;
        }
        dis[s]=0;
        for(int i=1;i<n;i++){
            int k=-1;
            int Min=INF;
            for(int j=1;j<=n;j++){
                if(!vis[j] && dis[j]<Min){
                    Min=dis[j];
                    k=j;
                }
            }
            if(k==-1){
                break;
            }
            vis[k]=true;
            for(int j=1;j<=n;j++){
                if(!vis[j] && dis[k]+cost[k][j]<dis[j]){
                    dis[j]=dis[k]+cost[k][j];
                }
            }
        }
    }
}
```



