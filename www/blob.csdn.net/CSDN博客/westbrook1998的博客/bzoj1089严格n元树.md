# bzoj1089严格n元树 - westbrook1998的博客 - CSDN博客





2018年11月25日 20:08:54[westbrook1998](https://me.csdn.net/westbrook1998)阅读数：36标签：[递推																[树上																[大数](https://so.csdn.net/so/search/s.do?q=大数&t=blog)
个人分类：[BZOJ刷题](https://blog.csdn.net/westbrook1998/article/category/8335089)





> 
Description

　　如果一棵树的所有非叶节点都恰好有n个儿子，那么我们称它为严格n元树。如果该树中最底层的节点深度为d

（根的深度为0），那么我们称它为一棵深度为d的严格n元树。例如，深度为２的严格２元树有三个，如下图：
![在这里插入图片描述](https://lydsy.com/JudgeOnline/images/1089/1.jpg)

　　给出n, d，编程数出深度为d的n元树数目。

Input

　　仅包含两个整数n, d( 0   <   n   <   =   32,   0  < =   d  < = 16)

Output

　　仅包含一个数，即深度为d的n元树的数目。

Sample Input

【样例输入1】

2 2

【样例输入2】

2 3

【样例输入3】

3 5

Sample Output

【样例输出1】

3

【样例输出2】

21

【样例输出2】

58871587162270592645034001
本想打表找规律但是d=3就数不清了

好不容易看懂题解,一个递推关系,n固定,以s[i]表示最大深度为i的严格n元树的方案数,然后从s[i-1]递推到s[i],就是就是在深度为i-1的树上再加一个根节点,然后这个根节点有n个子节点,每个点都可以连上s[i-1]那些方案的树,那这样就是s[i-1]^n,再加上1就是根不连任何子树的情况

然后最后答案就是s[d]-s[d-1]了,很强

java大数能过

代码:
```java
import java.math.BigInteger;
import java.util.Scanner;

public class Main{
    public static void main(String[] args) {
        int n,d;
        Scanner cin=new Scanner(System.in);
        n=cin.nextInt();
        d=cin.nextInt();
        BigInteger[] s=new BigInteger[50];
        if(d==0){
            System.out.println(1);
        }else{
            s[0]=BigInteger.valueOf(1);
            for(int i=1;i<=d;i++){
                s[i]=(s[i-1].pow(n)).add(BigInteger.valueOf(1));
            }
            System.out.println(s[d].subtract(s[d-1]));
        }
    }
}
```](https://so.csdn.net/so/search/s.do?q=树上&t=blog)](https://so.csdn.net/so/search/s.do?q=递推&t=blog)




