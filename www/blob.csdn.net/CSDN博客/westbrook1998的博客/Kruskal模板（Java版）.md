# Kruskal模板（Java版） - westbrook1998的博客 - CSDN博客





2018年10月28日 23:23:27[westbrook1998](https://me.csdn.net/westbrook1998)阅读数：37








gym100712F

```java
import java.util.ArrayList;
import java.util.Arrays;
import java.util.Collections;
import java.util.Scanner;

public class Main{
    static class Edge implements Comparable<Edge>{
        int u,v,w;
        Edge(int u,int v,int w){
            this.u=u;
            this.v=v;
            this.w=w;
        }


        @Override
        public int compareTo(Edge o) {
            return w-o.w;
        }
    }
    static final int N=(int)1e6+50;
    static final int M=(int)2e6+50;
    static int T,n,m,u,v,w,cnt;
    static int[] p=new int[N];
    static Edge[] edges=new Edge[M];

    static void addEdge(int u,int v,int w){
        edges[cnt++]=new Edge(u,v,w);
    }
    static int find(int x){
        return p[x]==x?x:(p[x]=find(p[x]));
    }
    static int Kruskal(){
        int ans=0;
        int c=0;
        for(int i=0;i<=n;i++){
            p[i]=i;
        }
        Arrays.sort(edges,0,cnt);
        for(int i=0;i<cnt;i++){
            int u=edges[i].u;
            int v=edges[i].v;
            int w=edges[i].w;
            int fa=find(u);
            int fb=find(v);
            if(fa!=fb){
                //求权值和
                //ans+=w;
                //求最长边
                ans=w;
                p[fa]=fb;
                c++;
            }
            if(c==n-1){
                break;
            }
        }
        if(c<n-1){
            //未连通
            return -1;
        }
        return ans;
    }
    public static void main(String[] args) {
        Scanner cin=new Scanner(System.in);
        T=cin.nextInt();
        while(T-->0){
            cnt=0;
            n=cin.nextInt();
            m=cin.nextInt();
            while(m-->0){
                u=cin.nextInt();
                v=cin.nextInt();
                w=cin.nextInt();
                addEdge(u,v,w);
            }
            int ans=Kruskal();
            System.out.println(ans);
        }
    }
}
```



