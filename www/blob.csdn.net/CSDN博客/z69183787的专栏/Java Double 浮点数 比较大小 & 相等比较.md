# Java Double 浮点数 比较大小 & 相等比较 - z69183787的专栏 - CSDN博客
2018年07月31日 18:42:06[OkidoGreen](https://me.csdn.net/z69183787)阅读数：476
[https://blog.csdn.net/z69183787/article/details/53286492](https://blog.csdn.net/z69183787/article/details/53286492)
```java
public class DoubleComparer {
    private static final double DEFAULT_DELTA = 0.000001; //默认比较精度
    //比较2个double值是否相等（默认精度）
    public static boolean considerEqual(double v1, double v2) {
        return considerEqual(v1, v2, DEFAULT_DELTA);
    }
    //比较2个double值是否相等（指定精度）
    public static boolean considerEqual(double v1, double v2, double delta) {
        return Double.compare(v1, v2) == 0 || considerZero(v1 - v2, delta);
    }
    //判断指定double是否为0（默认精度）
    public static boolean considerZero(double value) {
        return considerZero(value, DEFAULT_DELTA);
    }
    //判断指定double是否为0（指定精度）
    public static boolean considerZero(double value, double delta) {
        return Math.abs(value) <= delta;
    }
}
```
