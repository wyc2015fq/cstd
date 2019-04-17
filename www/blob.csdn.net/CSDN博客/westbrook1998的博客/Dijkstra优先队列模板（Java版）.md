# Dijkstra优先队列模板（Java版） - westbrook1998的博客 - CSDN博客





2018年10月24日 13:31:56[westbrook1998](https://me.csdn.net/westbrook1998)阅读数：136








再加一个数组模拟邻接表

仍然是c++的二三十倍的时间

```java
import java.util.Arrays;
import java.util.PriorityQueue;
import java.util.Scanner;

public class Main {
    static class Edge{
        public int v,w,next;

        public Edge(int v, int w, int next) {
            this.v = v;
            this.w = w;
            this.next = next;
        }
    }

    static class node implements Comparable<node>{
        public int v,c;

        public node(int v, int c) {
            this.v = v;
            this.c=c;
        }

        @Override
        public int compareTo(node o) {
            if(c>o.c){
                return 1;
            }else if(c==o.c){
                return 0;
            }else{
                return -1;
            }
        }
    }
    static final int N=1050;
    static final int M=200050;
    static final int INF=0x3f3f3f3f;
    static int[] dis=new int[N];
    static Edge[] edges=new Edge[M];
    static int cnt=0;
    static int[] head=new int[N];
    static boolean[] vis=new boolean[N];
    static int n,m,s,t;
    static int u,v,w;
    public static void main(String[] args) {
        Scanner scanner=new Scanner(System.in);
        while(scanner.hasNextInt()){
            n=scanner.nextInt();
            m=scanner.nextInt();
            init();
            if(n==0 && m==0) {
                break;
            }
            for(int i=0;i<m;i++){
                u=scanner.nextInt();
                v=scanner.nextInt();
                w=scanner.nextInt();
                addEdge(u,v,w);
            }
            s=1;
            t=n;
            Dijkstra();
            System.out.println(dis[t]);
        }
    }

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

    public static void Dijkstra(){
        for(int i=1;i<=n;i++){
            vis[i]=false;
            dis[i]=INF;
        }
        PriorityQueue<node> q=new PriorityQueue<>();
        dis[s]=0;
        q.add(new node(s,0));
        node tmp;
        while(!q.isEmpty()){
            tmp=q.poll();
            int u=tmp.v;
            if(vis[u]){
                continue;
            }
            vis[u]=true;
            for(int i=head[u];i!=-1;i=edges[i].next){
                int v=edges[i].v;
                int w=edges[i].w;
                if(!vis[v] && dis[v]>dis[u]+w){
                    dis[v]=dis[u]+w;
                    q.add(new node(v,dis[v]));
                }
            }
        }
    }
}
```



