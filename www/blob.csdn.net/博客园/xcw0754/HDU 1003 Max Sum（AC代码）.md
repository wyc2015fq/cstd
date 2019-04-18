# HDU 1003 Max Sum（AC代码） - xcw0754 - 博客园
# [HDU 1003 Max Sum（AC代码）](https://www.cnblogs.com/xcw0754/p/4051925.html)
```
1 #include <stdio.h>
 2 int main(){
 3         int i,t,j,n,x;
 4         int start,end,temp,max,sum;
 5         scanf("%d",&t);
 6         for(i=0;i<t;i++){
 7             temp=1;
 8             max=-100000;
 9             sum=0;
10             scanf("%d",&n);
11             for(j=0;j<n;j++){
12                 scanf("%d",&x);
13                 sum+=x;
14                 if(sum>=max){
15                     max=sum;
16                     start=temp;
17                     end=j+1;
18                 }
19                 if(sum<0){
20                     sum=0;
21                     temp=j+2;
22                 }                
23             }
24             printf("Case %d:\n",i+1);
25             printf("%d %d %d\n",max,start,end);
26             if(i!=t-1){
27                 printf("\n");
28             }
29             start=0;
30             end=0;
31         }
32     return 0;
33 }
```
格式看代码就行啦。
情况分析：
1、序列全为负数，则找出最大的那个即可。
2、子序列前面（多个）为负数，后面为正数，则start一定在负数之后。
3、子序列后面（多个）为负数，前面为正数，则end一定在负数之前。
4、即所要求的子序列应是：第一个必须0或正数，最后一个必须为0或正数
解题分析：
1、start和end分别记下开始和结束。
2、sum计算自序列的总和，若总和大于最大值，则修改最大值
3、若出现过max，且sum一直在减少，则当sum小于0时，sum清零（此时前面出现过最大的序列，开始和结束已记好），sum是从temp处开始加起的，所以temp要保持和sum一致，即sum小于零时就要修改temp。

