# hdu6433——Problem H. Pow - westbrook1998的博客 - CSDN博客





2018年09月22日 23:25:28[westbrook1998](https://me.csdn.net/westbrook1998)阅读数：105








> 
There are n numbers 3^0, 3^1, . . . , 3^n-1. Each time you can choose a subset of them (may be empty), and then add them up.

Count how many numbers can be represented in this way.

Input

The first line of the input contains an integer T , denoting the number of test cases. In each test case, there is a single integers n in one line, as described above.

1 ≤ T ≤ 20, 0 ≤ n ≤ 1000

Output

For each test case, output one line contains a single integer, denoting the answer.

Sample Input

4

9

7

8

233

Sample Output

512

128

256

13803492693581127574869511724554050904902217944340773110325048447598592
这个一开始觉得很难，仔细一想，这不就是一个数按带权的每一位的拆分吗，比如19=1*10^1 +9*10^0 这样n位有2^n种选择

然后其实我也不知道为什么答案就是2^n 反正其实直接看样例，然后java大数写个快速幂

代码：

```
import java.math.BigInteger;
import java.util.Scanner;

public class Main {

    public static BigInteger pow(BigInteger n){
        BigInteger ans=BigInteger.ONE;
        BigInteger base=BigInteger.valueOf(2);
        while(n.compareTo(BigInteger.ZERO)>0){
            if(n.mod(BigInteger.valueOf(2)).compareTo(BigInteger.ONE)==0){
                ans=ans.multiply(base);
            }
            base=base.multiply(base);
            n=n.divide(BigInteger.valueOf(2));
            //System.out.println(ans+" "+n);
        }
        return ans;
    }
    public static void main(String[] args) {
        Scanner scanner=new Scanner(System.in);
        int t=scanner.nextInt();
        while(t-->0){
            BigInteger n=scanner.nextBigInteger();
            System.out.println(pow(n));
        }
    }
}
```





