# 数位dp模板（Java版） - westbrook1998的博客 - CSDN博客





2018年10月27日 20:08:43[westbrook1998](https://me.csdn.net/westbrook1998)阅读数：59








hdu的不要62那道题

```java
import java.util.Scanner;

public class Main {
    static int[] a = new int[20];
    //dp[pos][sta]表示当前第pos位，前一位是否为6的状态
    static int[][] dp = new int[20][2];

    //当前位数 上一个数 当前状态(上一位是否为6) 是否有枚举上界
    static int dfs(int pos, int pre, int sta, boolean limit) {
        //枚举结束
        if (pos == -1) {
            return 1;
        }
        //记忆化
        if (!limit && dp[pos][sta] != -1) {
            return dp[pos][sta];
        }
        //当前位枚举上界
        int up = limit ? a[pos] : 9;
        int tmp = 0;
        for (int i = 0; i <= up; i++) {
            if (pre == 6 && i == 2) {
                continue;
            }
            if (i == 4) {
                continue;
            }
            if (i == 6) {
                tmp += dfs(pos - 1, i, 1, limit && i == a[pos]);
            } else {
                tmp += dfs(pos - 1, i, 0, limit && i == a[pos]);
            }
        }
        if (!limit) {
            dp[pos][sta] = tmp;
        }
        return tmp;
    }

    //分解数位
    static int solve(int x) {
        int pos = 0;
        while (x > 0) {
            a[pos++] = x % 10;
            x /= 10;
        }
        //从高位枚举
        return dfs(pos - 1, -1, 0, true);
    }

    public static void main(String[] args) {
        int l, r;
        Scanner cin = new Scanner(System.in);
        while (cin.hasNext()) {
            l = cin.nextInt();
            r = cin.nextInt();
            if(l+r==0){
                break;
            }
            for (int i = 0; i < 20; i++) {
                dp[i][0] = dp[i][1] = -1;
            }
            System.out.println(solve(r) - solve(l - 1));
        }
    }
}
```



