# 把只包含因子2、3和5的数称作丑数（Ugly Number）。例如6、8都是丑数，但14不是，因为它包含因子7。 习惯上我们把1当做是第一个丑数。求按从小到大的顺序的第N个丑数。 - A784586的专栏：任何事都存在 Plan B ！ - CSDN博客





2017年02月28日 13:20:42[QuJack](https://me.csdn.net/A784586)阅读数：4029








题目：

把只包含因子2、3和5的数称作丑数（Ugly Number）。例如6、8都是丑数，但14不是，因为它包含因子7。 习惯上我们把1当做是第一个丑数。

求按从小到大的顺序的第N个丑数。

思路：某个丑数肯定是前面丑数的2,3,5倍数。只需要从前往后生成即可。1,2,3,4,5,6,8,9,10,12,15，。。。。。。。




代码：



```java
public class Solution {
    public int min(int a,int b,int c){
        int min=(a<b)?a:b;
        return (min<c)?min:c;
    }
    public int GetUglyNumber_Solution(int index) {
        //1,2,3,4,5....
        if(index<=0)
            return 0;
        int[] a=new int[index];
        a[0]=1;
        int multi1=0;
        int multi2=0;
        int multi3=0;
        for(int i=1;i<a.length;i++){
            int min=min(a[multi1]*2,a[multi2]*3,a[multi3]*5);
            a[i]=min;
            while(a[multi1]*2<=min)
                multi1++;
            while(a[multi2]*3<=min)
                multi2++;
            while(a[multi3]*5<=min)
                multi3++;
                
        }
        
        return a[a.length-1];
    }
}
```







