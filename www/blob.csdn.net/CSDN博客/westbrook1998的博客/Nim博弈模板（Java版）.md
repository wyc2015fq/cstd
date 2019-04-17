# Nim博弈模板（Java版） - westbrook1998的博客 - CSDN博客





2018年10月18日 21:42:48[westbrook1998](https://me.csdn.net/westbrook1998)阅读数：46








```java
import java.util.PriorityQueue;
import java.util.Scanner;

public class Main {
    static int n,x,a;
    public static void main(String[] args) {
        Scanner cin=new Scanner(System.in);
        n=cin.nextInt();
        a=0;
        for(int i=0;i<n;i++){
            x=cin.nextInt();
            a^=x;
        }
        if(a==0){
            System.out.println("Bob");
        }
        else{
            System.out.println("Alice");
        }
    }
}
```



