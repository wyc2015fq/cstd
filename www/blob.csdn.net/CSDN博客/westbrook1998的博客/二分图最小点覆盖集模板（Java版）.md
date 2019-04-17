# 二分图最小点覆盖集模板（Java版） - westbrook1998的博客 - CSDN博客





2018年11月04日 10:43:21[westbrook1998](https://me.csdn.net/westbrook1998)阅读数：34标签：[二分图最小点集覆盖](https://so.csdn.net/so/search/s.do?q=二分图最小点集覆盖&t=blog)
个人分类：[Java算法模板](https://blog.csdn.net/westbrook1998/article/category/8228296)









```java
import java.util.ArrayList;
import java.util.Arrays;
import java.util.Scanner;

public class Main{
    static final int N=(int)1e5+50;
    static final int M=(int)1e5+50;
    static int n,m;
    static String s;
    static int cnt;
    static int[] head=new int[N],Left=new int[N],Right=new int[N];
    static boolean[] S=new boolean[N],T=new boolean[N];
    static Edge[] edges=new Edge[M];
    static ArrayList<Integer> X=new ArrayList<>(),Y=new ArrayList<>();
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
    }

    static boolean dfs(int u){
        S[u]=true;
        for(int i=head[u];i!=-1;i=edges[i].next) {
            int v = edges[i].v;
            if (!T[v]) {
                T[v] = true;
                if(Left[v]==-1 || dfs(Left[v])){
                    Left[v]=u;
                    Right[u]=v;
                    return true;
                }
            }
        }
        return false;
    }

    static int hungary(){
        int res=0;
        Arrays.fill(Left,-1);
        Arrays.fill(Right,-1);
        for(int i=1;i<=n;i++){
            Arrays.fill(T,false);
            if(dfs(i)){
                res++;
            }
        }
        return res;
    }

    static void solve(){
        Arrays.fill(S,false);
        Arrays.fill(T,false);
        for(int i=1;i<=n;i++){
            if(Right[i]==-1){
                dfs(i);
            }
        }
        for(int u=1;u<=n;u++){
            if(!S[u]){
                X.add(u);
            }
        }
        for(int v=1;v<=m;v++){
            if(T[v]){
                Y.add(v);
            }
        }
    }
    public static void main(String[] args) {
        Scanner cin=new Scanner(System.in);
        n=cin.nextInt();
        m=cin.nextInt();
        init();
        for(int i=1;i<=n;i++){
            s=cin.next();
            for(int j=0;j<m;j++){
                if(s.charAt(j)=='*'){
                    addEdge(i,j+1);
                }
            }
        }
        int ans=hungary();
        solve();
        System.out.println(ans);
        System.out.print(X.size());
        for(int i=0;i<X.size();i++){
            System.out.print(" "+X.get(i));
        }
        System.out.println();
        System.out.print(Y.size());
        for(int i=0;i<Y.size();i++){
            System.out.print(" "+Y.get(i));
        }
        System.out.println();
    }
}
```



