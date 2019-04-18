# Java中处理异常中return关键字 - z69183787的专栏 - CSDN博客
2017年03月13日 17:54:42[OkidoGreen](https://me.csdn.net/z69183787)阅读数：1489
Java中，执行try-catch-finally语句需要注意：
第一：return语句并不是函数的最终出口，如果有finally语句，这在return之后还会执行finally（return的值会暂存在栈里面，等待finally执行后再返回）
第二：finally里面不建议放return语句，根据需要，return语句可以放在try和catch里面和函数的最后。可行的做法有四种：
　　1）return语句只在方法最后出现一次。
　　2）return语句仅在try和catch里面都出现。
　　3）return语句仅在try和方法最后都出现。
　　4）return语句仅在catch和方法的最后都出现。
注意，除此之外的其他做法都是不可行的，编译器会报错。
（1）如果程序运行到try成功时可以返回结果，则采用方法2。
（2）如果程序运行到catch时（即中途出错时）无需再继续执行后面的代码了，则采取方法4。
（3）如果程序运行到try或catch时还需要继续执行后面的代码，则采取方法1。
1.
```
![复制代码](http://common.cnblogs.com/images/copycode.gif)
package cn.day006;
public class Test {
     public static void main(String[] args) {
            System.out.println(getResult());
        }
        @SuppressWarnings("finally")
        public static int getResult(){
            int a =100;
            
            try{
                return a+1; //注意，java的基础数据类型是值传递，这里的返回值已经和上面的a没有关系了
            }catch(Exception e){
                e.printStackTrace();  
            }finally{
                return a;    //最后再把值重定向到a(相当于将try中的返回值覆盖掉)，所以输出还是100
            }
        }
}
![复制代码](http://common.cnblogs.com/images/copycode.gif)
```
结果：100
2.
```
![复制代码](http://common.cnblogs.com/images/copycode.gif)
package cn.day006;
public class Test {
     public static void main(String[] args) {
            System.out.println(getResult());
        }
        @SuppressWarnings("finally")
        public static int getResult(){
            int a =100;
            
            try{                
                return a++;   //切记 a++   ----> a=a+1 此时a的值做出了改变
            }catch(Exception e){
                e.printStackTrace();  
            }finally{
                return a;   
            }
        }
}
![复制代码](http://common.cnblogs.com/images/copycode.gif)
```
结果：101
3.
```
![复制代码](http://common.cnblogs.com/images/copycode.gif)
package cn.day006;
public class Test {
     public static void main(String[] args) {
            System.out.println(getResult());
        }
        public static int getResult(){
            int a =100;            
            try{
                  a++;
                return a;              
            }finally{
                a++;   
            }
        }
}
![复制代码](http://common.cnblogs.com/images/copycode.gif)
```
结果是101。
分析：
在try语句中，在执行return语句时，要返回的结果已经准备好了，就在此时，程序转到finally执行了。
在转去之前，try中先把要返回的结果存放到不同于x的局部变量中去，执行完finally之后，在从中取出返回结果，
因此，即使finally中对变量x进行了改变，但是不会影响返回结果。
它应该使用栈保存返回值。
4.
```
![复制代码](http://common.cnblogs.com/images/copycode.gif)
package cn.day006;
public class Test2 {
     public static void main(String[] args) {
            try{
            System.out.println(getResult());
            }catch(Exception e){
                e.printStackTrace();
                System.out.println("截获异常catch");
            }finally{
                System.out.println("异常处理finally");
            }
        }
        @SuppressWarnings("finally")
        public static int getResult() throws Exception{
            int a =100;            
            try{                
                a=a+10; 
                throw new RuntimeException();
            }catch(Exception e){
                System.out.println("截获异常并重新抛出异常");
                throw new Exception();            
            }finally{
                return a;
            }
        }
}
![复制代码](http://common.cnblogs.com/images/copycode.gif)
```
结果如下：
截获异常并重新抛出异常
110
异常处理finally
关键的“截获异常catch”却没有执行！！！
原因是在getResult()的finally中return一个值，等同于告诉编译器该方法没有异常,但实际上异常是有的，这样的结果是该方法的调用者却捕获不到异常，相对于异常被无端的被吃掉了，隐藏杀机啊！！
参考牛人博客：http://www.cnblogs.com/Fskjb/archive/2012/02/15/2353256.html
