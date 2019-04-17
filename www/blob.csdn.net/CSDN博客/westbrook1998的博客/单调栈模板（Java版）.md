# 单调栈模板（Java版） - westbrook1998的博客 - CSDN博客





2018年11月03日 11:29:43[westbrook1998](https://me.csdn.net/westbrook1998)阅读数：91








单调栈是一个很神奇的东西…

比如可以用来算出数组中每个数作为最大/最小值能向左向右延伸到那里，从而可以来算出区间的一些问题

单调栈的思想感觉有点像尺取法，就是在一个新的数要入栈的时候就判断然后不断地出栈（如果需要的话），然后通过出栈完的状态（空栈或者还有栈顶元素）来记录当前数的状态。

hdu1506 求最大的矩形面积，其实也就相当于求每个高度作为区间的最小值能延伸到哪里，那对应的矩形面积就是这个高度乘以这个延伸到的区间长度

以这题来看，具体就是当一个数要入栈时，先判断栈是否为空，如果非空而且当前高度小于等于栈顶高度，那这个栈顶元素就要出栈，只要栈顶元素大于要加入的元素，即保持这个栈的单调性（栈底最小，栈顶最大），出栈完之后就要记录要加入这个数的状态了，因为栈中存的就是数组下标，所以这个要加入的数作为最小值就能延伸到栈顶元素的下标+1了，而如果是空栈，那就说明可以延伸到第一个元素，然后就将这个元素加入栈中，进行下一个元素的操作

```java
import java.util.Scanner;
import java.util.Stack;

public class Main {
    static int n;
    static final int N=(int)1e5+50;
    static long[] a=new long[N];
    static int[] ll=new int[N],rr=new int[N];
    static long solve(){
        Stack<Integer> s=new Stack<>();
        for(int i=1;i<=n;i++){
            while(s.size()>0 && a[i]<=a[s.peek()]){
                s.pop();
            }
            if(s.size()==0){
                ll[i]=1;
            }
            else{
                ll[i]=s.peek()+1;
            }
            s.push(i);
        }
        s=new Stack<>();
        for(int i=n;i>=1;i--){
            while(s.size()>0 && a[i]<=a[s.peek()]){
                s.pop();
            }
            if(s.size()==0){
                rr[i]=n;
            }
            else{
                rr[i]=s.peek()-1;
            }
            s.push(i);
        }
        for(int i=1;i<=n;i++){
            System.out.print(ll[i]+" ");
        }
        System.out.println();
        for(int i=1;i<=n;i++){
            System.out.print(rr[i]+" ");
        }
        System.out.println();
        long ans=0;
        for(int i=1;i<=n;i++){
            ans=Math.max(ans,a[i]*(rr[i]-ll[i]+1));
        }
        return ans;
    }
    public static void main(String[] args) {
        Scanner cin=new Scanner(System.in);
        while(cin.hasNext()){
            n=cin.nextInt();
            if(n==0){
                break;
            }
            for(int i=1;i<=n;i++){
                a[i]=cin.nextLong();
            }
            long ans=solve();
            System.out.println(ans);
        }
    }
}
```



