# hdu 3723  Delta Wave - HJ - CSDN博客
2016年07月18日 01:28:35[FZH_SYU](https://me.csdn.net/feizaoSYUACM)阅读数：612
Delta Wave  hdu 3723
Problem Description 
A delta wave is a high amplitude brain wave in humans with a frequency of 1 – 4 hertz which can be recorded with an electroencephalogram (EEG) and is usually associated with slow-wave sleep (SWS). 
– from Wikipedia
The researchers have discovered a new kind of species called “otaku”, whose brain waves are rather strange. The delta wave of an otaku’s brain can be approximated by a polygonal line in the 2D coordinate system. The line is a route from point (0, 0) to (N, 0), and it is allowed to move only to the right (up, down or straight) at every step. And during the whole moving, it is not allowed to dip below the y = 0 axis.
For example, there are the 9 kinds of delta waves for N = 4:
Given N, you are requested to find out how many kinds of different delta waves of otaku.
Input 
There are no more than 20 test cases. There is only one line for each case, containing an integer N (2 < N <= 10000)
Output 
Output one line for each test case. For the answer may be quite huge, you need only output the answer module 10100.
Sample Input 
3 
4
Sample Output 
4 
9
解析：首先可以观察到上升、下降位置的个数是一样的。假设有k对上升、下降的位置，那么这k对位置的合法组合数为Cat[k]。Cat为卡特兰数，这个组合问题等价于卡特兰数的经典应用括号化问题。设T[k]为n个位置中有k对上升、下降位置的组合数，这k对位置从n个位置中选出。所以T[k] = C(n,2*k)*Cat[k] = C(n,2*k)*C(2*k,k)/(k+1)。这样只需要枚举k即可，sum = T[0]+T[1]+…+T[m] (2*m<=n)。然而n<=10000，C(n,m)无法打表，也不好求。进一步可以推出T[k]/T[k-1] = (n-2*k+1)*(n-2*k+2)/(k*(k+1))。显然T[0] = 1，于是逐个递推出来即可。
注意：由于给出数据和取模数据很大，建议用Java定义BigInteger
代码如下：
```
import java.math.BigInteger;  
import java.util.Scanner;  
public class Main {  
    public static void main(String[] args) {  
        Scanner sc = new Scanner(System.in);  
        BigInteger mod = BigInteger.TEN.pow(100);  
        BigInteger sum = new BigInteger("0");  
        BigInteger t = new BigInteger("0");  
        while (sc.hasNext()) {  
            int n = sc.nextInt();  
            sum = BigInteger.ONE;  
            t = BigInteger.ONE;  
            for(int k = 1; k + k <= n; k++) {  
                t = t.multiply(BigInteger.valueOf((n-2*k+1)*(n-2*k+2)))  
                     .divide(BigInteger.valueOf(k*(k+1)));  
                sum = sum.add(t);  
            }  
            System.out.println(sum.mod(mod));  
        }  
    }  
}
```
