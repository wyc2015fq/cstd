# 实现power函数 - A784586的专栏：任何事都存在 Plan B ！ - CSDN博客





2017年02月20日 14:39:00[QuJack](https://me.csdn.net/A784586)阅读数：673










## 题目描述   实现函数库中的 power（a,b）


给定一个double类型的浮点数base和int类型的整数exponent。求base的exponent次方。




思路：



    考虑要全面；

底数和指数都是0，无意义。其中相等不能用==；

指数大于0，迭代乘；

小余0；




代码实现：



```java
package com.mytest.mymain;


public class MyPower001 {
	public static void main(String[] args) {
		MyPower001 myPower001=new MyPower001();
		System.out.println("Main:  "+myPower001.Power(2, -3));
	}
	
	
    private boolean myequal(double a , double b){
        if(a-b<0.0000001 && a-b>-0.0000001){
            return true;
        }else{
            return false;
        }
    }
    
    private double unsignpower(double base, int exponent){
        double result=1.0;
        for(int i=1;i<=exponent;i++){
            result=result*base;
        }
        return result;
    }
    
    public double Power(double base, int exponent) {
        double result=0.0;
        if(myequal(base,0.0) && exponent<0)
            return 0.0;
        if(exponent<0){
            result=1.0/unsignpower(base,-exponent);
        }else if(exponent>0){
           result=unsignpower(base,exponent); 
        }else if(exponent==0){
            return 1.0;
        }
            
        
        return result;
  }
    
    
}
```


  改进：



       求指数有个快速迭代的公式：a^n=a^(n/2)*a^(n/2)  偶数    a^n=a^[(n-1)/2]*a^[(n-1)/2]  奇数



