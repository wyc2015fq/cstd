# SPFA模板（Java版） - westbrook1998的博客 - CSDN博客





2018年10月25日 19:31:43[westbrook1998](https://me.csdn.net/westbrook1998)阅读数：91








判负环就是看入队次数>n

```java
import java.util.Queue;
import java.util.Scanner;
import java.util.concurrent.LinkedBlockingDeque;

public class Main {
    static class Edge{
        int v,w,next;
        Edge(int v,int w,int next){
            this.v=v;
            this.w=w;
            this.next=next;
        }
    }
    static final int N= 100500;
    static final int M=1000050;
    static final int INF=0x3f3f3f3f;
    static int n,m,s,t,u,v,w,cnt;
    static int[] head=new int[N];
    static Edge[] edges=new Edge[M];
    static int[] dis=new int[N];
    static boolean[] vis=new boolean[N];
    static int[] inq=new int[N];
    static void init(){
        cnt=0;
        for(int i=1;i<=n;i++){
            head[i]=-1;
        }
    }
    static void addEdge(int u,int v,int w){
        edges[cnt]=new Edge(v,w,head[u]);
        head[u]=cnt++;
        edges[cnt]=new Edge(u,w,head[v]);
        head[v]=cnt++;
    }
    static boolean spfa(){
        for(int i=1;i<=n;i++){
            vis[i]=false;
            inq[i]=0;
            dis[i]=INF;
        }
        dis[s]=0;
        vis[s]=true;
        Queue<Integer> q=new LinkedBlockingDeque<>();
        q.add(s);
        inq[s]=1;
        while(!q.isEmpty()){
            int u=q.poll();
            vis[u]=false;
            for(int i=head[u];i!=-1;i=edges[i].next){
                int v=edges[i].v;
                int w=edges[i].w;
                if(dis[v]>dis[u]+w){
                    dis[v]=dis[u]+w;
                    if(!vis[v]){
                        vis[v]=true;
                        q.add(v);
                        if(++inq[v]>n){
                            return false;
                        }
                    }
                }
            }
        }
        return true;
    }
    public static void main(String[] args) {
        Scanner cin=new Scanner(System.in);
        n=cin.nextInt();
        m=cin.nextInt();
        s=cin.nextInt();
        t=cin.nextInt();
        init();
        while(m-->0){
            u=cin.nextInt();
            v=cin.nextInt();
            w=cin.nextInt();
            addEdge(u,v,w);
        }
        spfa();
        System.out.println(dis[t]);
    }
}
```



