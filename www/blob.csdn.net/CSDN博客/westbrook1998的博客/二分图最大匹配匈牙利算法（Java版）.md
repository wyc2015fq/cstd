# 二分图最大匹配匈牙利算法（Java版） - westbrook1998的博客 - CSDN博客





2018年11月03日 23:05:47[westbrook1998](https://me.csdn.net/westbrook1998)阅读数：56








```java
import java.util.Arrays;
import java.util.Scanner;

public class Main{
    static final int N=(int)2e3+50;
    static final int M=(int)1e4+50;
    static int n,m,u,v;
    static int cnt;
    static int[] head=new int[N],link=new int[N];
    static boolean[] vis=new boolean[N];
    static Edge[] edges=new Edge[2*M];
    static class Edge{
        int v,next;

        public Edge(int v,int next) {
            this.v = v;
            this.next = next;
        }
    }

    static void init(){
        cnt=0;
        Arrays.fill(head,-1);
    }

    static void addEdge(int u,int v){
        edges[cnt]=new Edge(v,head[u]);
        head[u]=cnt++;
        edges[cnt]=new Edge(u,head[v]);
        head[v]=cnt++;
    }

    static boolean dfs(int u){
        for(int i=head[u];i!=-1;i=edges[i].next) {
            int v = edges[i].v;
            if (!vis[v]) {
                vis[v] = true;
                if(link[v]==-1 || dfs(link[v])){
                    link[v]=u;
                    return true;
                }
            }
        }
        return false;
    }

    static int hungary(){
        int res=0;
        Arrays.fill(link,-1);
        for(int i=1;i<=n;i++){
            Arrays.fill(vis,false);
            if(dfs(i)){
                res++;
            }
        }
        return res/2;
    }

    public static void main(String[] args) {
        Scanner cin=new Scanner(System.in);
        n=cin.nextInt();
        m=cin.nextInt();
        init();
        while(m-->0){
            u=cin.nextInt();
            v=cin.nextInt();
            addEdge(u,v);
        }
        int ans=hungary();
        System.out.println(ans);
    }
}
```



