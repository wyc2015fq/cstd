# 二分图判定模板（Java版） - westbrook1998的博客 - CSDN博客





2018年11月03日 22:24:48[westbrook1998](https://me.csdn.net/westbrook1998)阅读数：70








邻接表数组记得要开两倍边啊

```java
import java.util.Arrays;
import java.util.Scanner;

public class Main{
    static final int N=(int)1e4+50;
    static final int M=(int)4e4+50;
    static int t,n,m,u,v;
    static int cnt;
    static int[] head=new int[N],color=new int[N];
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

    static boolean dfs(int u,int c){
        color[u]=c;
        for(int i=head[u];i!=-1;i=edges[i].next){
            int v=edges[i].v;
            if(color[v]==c){
                return false;
            }
            if(color[v]==0 && !dfs(v,-c)){
                return false;
            }
        }
        return true;
    }

    static void solve(){
        Arrays.fill(color,0);
        for(int i=1;i<=n;i++){
            if(color[i]==0){
                if(!dfs(i,1)){
                    System.out.println("Wrong");
                    return;
                }
            }
        }
        System.out.println("Correct");
    }

    public static void main(String[] args) {
        Scanner cin=new Scanner(System.in);
        t=cin.nextInt();
        while(t-->0){
            init();
            n=cin.nextInt();
            m=cin.nextInt();
            while(m-->0){
                u=cin.nextInt();
                v=cin.nextInt();
                addEdge(u,v);
            }
            solve();
        }
    }
}
```



