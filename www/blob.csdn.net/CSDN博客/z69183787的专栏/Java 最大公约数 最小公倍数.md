# Java 最大公约数 最小公倍数 - z69183787的专栏 - CSDN博客
2012年11月18日 13:33:08[OkidoGreen](https://me.csdn.net/z69183787)阅读数：707
最小公倍数=两数的乘积/最大公约（因）数,
```java
class Max_Min1 {
 int MaxComMul(int a,int b){ //求最大公约数（辗转相除法）
  if(a<=0 || b<=0)
   return 0; //说明不存在公约数
  if(a<b){
   int c;
   c=a;
   a=b;
   b=c;
  }
  
  int r=a%b;
  while(r!=0){
   a=b;
   b=r;
   r=a%b;
  }
  return b;
 }
 
 int MinComDiv(int a,int b){ //利用最大公约数求最小公倍数
  int max;
  max=MaxComMul(a,b);
  if(max==0)
   return 0; //说明不存在公倍数
  return a*b/max;
  
 }
}
```
```java
方法二：
public class Max_Min2 {
 int MinComDiv(int a,int b){ //最小公倍数位于a（假设a>b）到a*b之间，且是a的倍数
  if(a<=0 || b<=0)
   return 0; //说明不存在公倍数
  if(a<b){
   int c;
   c=a;
   a=b;
   b=c;
  }
  
  int i;
  for(i=a;i<=a*b;i=i+a)
   if(i%b==0)
    break;
  return i;
 }
 
 int MaxComMul(int a,int b){ //利用最小公倍数求最大公约数
  int min;
  min=MinComDiv(a,b);
  if(min==0)
   return 0; //说明不存在公约数
  return a*b/min;
 }
}
```

```java
//输入两个正整数m和n，求其最大公约数和最小公倍数
//辗除法：公约数指的是能够整除两个数字的一个数字。
//那么公约数一定能被这两个数字的差整除。既然能被差整除，
//那就能被这个差和最小的那个数字整除，就这么反复的应用。
public class GongShu {
public static void main(String[] args) {
   // TODO: Add your code here
   int m = 50;
   int n = 60;
   int temp;
   int max = (m>n)?m:n;//两个数中间的那个大的
   int min = (m<n)?m:n;//两个数中间的那个小的
   //利用辗除法求最大公约数
   while(max!=min)
   {
    temp = max-min;
    max=(temp>min)?temp:min;
    min=(temp<min)?temp:min;
   }
   System.out.println(max);
   //求最小公倍数
   int beishu=(m*n)/max;
   System.out.println(beishu);
} 
}
```
