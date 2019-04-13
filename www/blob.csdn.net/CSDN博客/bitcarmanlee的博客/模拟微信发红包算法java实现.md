
# 模拟微信发红包算法java实现 - bitcarmanlee的博客 - CSDN博客


2019年04月03日 16:12:31[bitcarmanlee](https://me.csdn.net/bitcarmanlee)阅读数：41


发红包现在作为一种人民群众喜闻乐见的业余文化活动，尤其在过年过节的时候发挥了重要的作用。现在我们来模拟微信发红包写个简单的小算法，逻辑比较简单，具体的过程参考注释。
`public class HongBaoAlgorithm {
    // 每个红包最小金额
    private static final double MINMONEY = 0.01;
    public static void hongbaotest(double totalmoney, int count) {
        double[] moneys = new double[count];
        for(int i=1; i<count; i++) {
            // 当前第i个红包的最大金额与实际金额，实际金额保证不小于最小金额
            double maxmoney = (totalmoney - (count-i) * MINMONEY) / (count - i);
            double curmoney = Math.random() * (maxmoney - MINMONEY) + MINMONEY;
            totalmoney -= curmoney;
            moneys[i-1] = curmoney;
        }
        // 最后一个红包
        moneys[count-1] = totalmoney;
        printnums(moneys);
    }
    public static void printnums(double[] moneys) {
        double sum = 0.0;
        for(double each: moneys) {
            System.out.println(each);
            sum += each;
        }
        System.out.println("the sum is: " + sum);
    }
    public static void main(String[] args) {
        hongbaotest(200, 10);
    }
}`某次运行的输出结果
`5.134629101796313
9.691355220067367
15.481764414378727
25.07662708441383
27.70410154666285
13.53784589067451
1.3830881259396322
9.043098630641323
18.340855733059616
74.60663425236581
the sum is: 199.99999999999997`最后小数点后面的误差是由double类型带来的误差。当然实际中处理钱相关的数据可以用BigDecimal类保证精度。

