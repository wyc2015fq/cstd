# 并查集模板（Java版） - westbrook1998的博客 - CSDN博客





2018年11月04日 13:33:40[westbrook1998](https://me.csdn.net/westbrook1998)阅读数：82








```java
import java.util.Scanner;

public class Main{
    static final int N=1050;
    static int n,m,u,v;
    static int[] p=new int[N];
    static void init(){
        for(int i=1;i<=n;i++){
            p[i]=i;
        }
    }
    static int find(int x){
        return p[x]==x?x:(p[x]=find(p[x]));
    }
    static void unit(int a,int b){
        p[find(a)]=find(b);
    }
    static boolean isUnit(int a,int b){
        return find(a)==find(b);
    }
    public static void main(String[] args) {
        Scanner cin=new Scanner(System.in);
        while(cin.hasNext()){
            n=cin.nextInt();
            if(n==0){
                break;
            }
            m=cin.nextInt();
            init();
            int res=n-1;
            while(m-->0){
                u=cin.nextInt();
                v=cin.nextInt();
                if(!isUnit(u,v)){
                    res--;
                    unit(u,v);
                }
            }
            System.out.println(res);
        }
    }
}
```



