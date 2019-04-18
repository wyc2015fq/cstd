# JAVA常见算法题(八) - weixin_33985507的博客 - CSDN博客
2017年06月05日 21:19:00[weixin_33985507](https://me.csdn.net/weixin_33985507)阅读数：4
```
package com.xiaowu.demo;
/**
 * 求s=a+aa+aaa+aaaa+aa...a的值，其中a是一个数字。例如2+22+222+2222+22222(此时共有5个数相加)，
 * 几个数相加由键盘控制。
 * 
 * @author WQ
 *
 */
public class Demo8 {
    public static void main(String[] args) {
        sum(2, 5);
    }
    public static void sum(int a, int n) {
        int b = a;
        long sum = 0;
        for (int i = 0; i < n; i++) {
            sum += a;
            a = a * 10 + b;
        }
        System.out.println("和为：" + sum);
    }
}
```
