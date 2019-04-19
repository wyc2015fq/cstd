# FZU  a^b（结论题） - HJ - CSDN博客
2017年03月12日 11:50:42[FZH_SYU](https://me.csdn.net/feizaoSYUACM)阅读数：593
对于任意两个正整数a,b（0<=a,b<10000）计算a b各位数字的和的各位数字的和的各位数字的和的各位数字的和。 
Input 
    输入有多组数据，每组只有一行，包含两个正整数a,b。最后一组a=0,b=0表示输入结束，不需要处理。 
Output 
    对于每组输入数据，输出ab各位数字的和的各位数字的和的各位数字的和的各位数字的和。 
Sample Input
```
2 3
5 7
0 0
```
Sample Output
```
8
5
```
利用结论： 
**任何数除以9的余数等于各位数的和除以9的余数**
证明如下：
```
s=a1a2...an=a1*10^(n-1)+a2*10^(n-1)+...+an
           =a1*(999..9+1)+a2*(999..9+1)+...+an
           =(a1*999..9+a2*999..9+...+a(n-1)*9)+(a1+a2+...+an)
```
因为 
```
(a1*999..9+a2*999..9+...+a(n-1)*9)%9=0;
```
则
```
s%9=(a1+a2+...+an)%9
```
得证！
又：
```
(a*b)%n = ((a%n)*(b%n)) % n;
```
```
import java.util.Scanner;
public class Main {
    public static void main(String args[]){
        Scanner in=new Scanner(System.in);
        int a,b,sum;
        while(in.hasNextInt()){
            a=in.nextInt();
            b=in.nextInt();
            if(a==0&&b==0){
                break;
            }                        //end if
            if(b==0){                //b=0时任何数的b次方都为1 
                System.out.println("1");
                break;
            }
            int num=1;              //store remainder
            for(int i=0;i<b;i++){
                num=(num*a)%9;  
            }
            if(num==0){
                System.out.println("9");
            }else{
                System.out.println(num);
            }
        }                         //end while
    }
}
```
