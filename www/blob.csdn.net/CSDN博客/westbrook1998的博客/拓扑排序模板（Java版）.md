# 拓扑排序模板（Java版） - westbrook1998的博客 - CSDN博客





2018年10月18日 21:32:52[westbrook1998](https://me.csdn.net/westbrook1998)阅读数：38








hiho的题目

java时间和内存真的炸…

20倍时间 10倍内存差不多
```java
import java.util.PriorityQueue;
import java.util.Scanner;

public class Main {
    static int t,n,m,u,v,cnt;
    static final int N=100050,M=500050;
    static int[] ind=new int[N],topo=new int[N],head=new int[N];
    static Edge[] edge=new Edge[M];

    public static void init(){
        cnt=0;
        for(int i=0;i<=n;i++){
            head[i]=-1;
            topo[i]=-1;
            ind[i]=0;
        }
    }
    public static void addEdge(int u,int v){
        edge[cnt]=new Edge(u,v,head[u]);
        head[u]=cnt++;
        ind[v]++;
    }

    public static void topoSort(){
        int k=1;
        PriorityQueue<Integer> q=new PriorityQueue<>();
        for(int i=1;i<=n;i++){
            if(ind[i]==0){
                q.add(i);
            }
        }
        while(!q.isEmpty()){
            int u=q.poll();
            ind[u]--;
            topo[k++]=u;
            for(int i=head[u];i!=-1;i=edge[i].next){
                int v=edge[i].v;
                ind[v]--;
                if(ind[v]==0){
                    q.add(v);
                }
            }
        }
    }
    public static void main(String[] args) {
        Scanner cin=new Scanner(System.in);
        t=cin.nextInt();
        while(t>0){
            t--;
            n=cin.nextInt();
            m=cin.nextInt();
            init();
            for(int i=0;i<m;i++) {
                u = cin.nextInt();
                v = cin.nextInt();
                addEdge(u, v);
            }
            topoSort();
            boolean flag=true;
            for(int i=1;i<=n;i++){
                if(topo[i]==-1){
                    flag=false;
                    break;
                }
            }
            if(flag){
                System.out.println("Correct");
            }
            else{
                System.out.println("Wrong");
            }
        }
    }
}

class Edge{
    int u,v,next;
    Edge(int u,int v,int next){
        this.u=u;
        this.v=v;
        this.next=next;
    }
}
```




