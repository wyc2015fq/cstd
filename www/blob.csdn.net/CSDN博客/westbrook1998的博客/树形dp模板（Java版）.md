# 树形dp模板（Java版） - westbrook1998的博客 - CSDN博客





2018年10月27日 21:33:10[westbrook1998](https://me.csdn.net/westbrook1998)阅读数：85








最简单的树形dp，树上最大点权独立集

```java
import java.util.Scanner;

public class Main{
    static class Edge{
        int v,next;
        Edge(int v,int next){
            this.v=v;
            this.next=next;
        }
    }

    static final int N=(int)6e3+50;
    static final int M=(int)2e4+50;
    static int n,u,v,cnt;
    static int[] head=new int[N];
    static int[][] dp=new int[N][2];
    static boolean[] ind=new boolean[N];
    static Edge[] edges=new Edge[M];

    static void init(){
        cnt=0;
        for(int i=1;i<=n;i++){
            head[i]=-1;
        }
    }
    static void addEdge(int u,int v){
        edges[cnt]=new Edge(v,head[u]);
        head[u]=cnt++;
    }
    static void dfs(int u){
        for(int i=head[u];i!=-1;i=edges[i].next){
            int v=edges[i].v;
            dfs(v);
            dp[u][1]+=dp[v][0];
            dp[u][0]+=Math.max(dp[v][0],dp[v][1]);
        }
    }
    public static void main(String[] args) {
        Scanner cin=new Scanner(System.in);
        while(cin.hasNext()){
            n=cin.nextInt();
            init();
            for(int i=1;i<=n;i++){
                ind[i]=false;
                dp[i][0]=dp[i][1]=0;
            }
            for(int i=1;i<=n;i++){
                dp[i][1]=cin.nextInt();
            }
            while(cin.hasNext()){
                v=cin.nextInt();
                u=cin.nextInt();
                if(v+v==0){
                    break;
                }
                addEdge(u,v);
                ind[v]=true;
            }
            int root=-1;
            for(int i=1;i<=n;i++){
                if(!ind[i]){
                    root=i;
                    break;
                }
            }
            dfs(root);
            System.out.println(Math.max(dp[root][0],dp[root][1]));
        }
    }
}
```



