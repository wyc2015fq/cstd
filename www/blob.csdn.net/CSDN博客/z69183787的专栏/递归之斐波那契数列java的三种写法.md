# 递归之斐波那契数列java的三种写法 - z69183787的专栏 - CSDN博客
2018年03月30日 16:16:44[OkidoGreen](https://me.csdn.net/z69183787)阅读数：179
[https://blog.csdn.net/bruce_suxin/article/details/52937720](https://blog.csdn.net/bruce_suxin/article/details/52937720)
第一种普通写法
[java][view
 plain](https://blog.csdn.net/bruce_suxin/article/details/52937720#)[copy](https://blog.csdn.net/bruce_suxin/article/details/52937720#)
- publicclass Demo {  
- 
- publicstaticvoid main(String[] args) {  
- int num1 = 1;  
- int num2 = 1;  
- int num3 = 0;  
-         System.out.println(num1);  
-         System.out.println(num2);  
- for (int i = 1; i < 10; i++) {    
-             num3 = num1 + num2;  
-             num1 = num2;                                                                                                                    
-             num2 = num3;  
-             System.out.println(num3);  
-         }  
-     }                       
- }  
第二种数组形式递归的写法
[java][view
 plain](https://blog.csdn.net/bruce_suxin/article/details/52937720#)[copy](https://blog.csdn.net/bruce_suxin/article/details/52937720#)
- publicclass DIGUI1 {  
- 
- publicstaticvoid main(String[] args) {  
- int  []arr=newint[20];  
-          arr[1]=1;  
-          arr[2]=1;  
-          System.out.print(" "+arr[1]);  
-           System.out.print(" "+arr[2]);  
- for(int i=3;i<20;i++){  
-              arr[i]=arr[i-1]+arr[i-2];  
-             System.out.print("   "+arr[i]);  
-         }  
-     }  
-  }  
第三种 递归形式的写法
[java][view
 plain](https://blog.csdn.net/bruce_suxin/article/details/52937720#)[copy](https://blog.csdn.net/bruce_suxin/article/details/52937720#)
- 
- 
- publicclass Demo {  
- 
- 
- publicstaticint f(int n) throws Exception {  
- if(n==0){  
- thrownew Exception("参数错误！");  
-         }  
- if (n == 1 || n == 2) {  
- return1;  
-         } else {  
- return f(n-1)+f(n-2);//自己调用自己
-         }  
-  }  
- 
- 
- publicstaticvoid main(String[] args) throws Exception {  
- for (int i = 1; i <=10; i++) {  
-             System.out.print(f(i)+" ");  
-         }  
-     }    
- }  
用递归最大的问题就是效率问题了，但是有的程序必须用递归写才可以写出来。例如著名的汉若塔问题，如果有谁可以用其他方式写出来我服。
