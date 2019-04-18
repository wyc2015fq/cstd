# Java实现约瑟夫环问题 - z69183787的专栏 - CSDN博客
2018年03月30日 17:41:03[OkidoGreen](https://me.csdn.net/z69183787)阅读数：107
[https://blog.csdn.net/qq_21150865/article/details/60467166](https://blog.csdn.net/qq_21150865/article/details/60467166)
约瑟夫环（约瑟夫问题）是一个数学的应用问题：已知n个人（以编号1，2，3…n分别表示）围坐在一张圆桌周围。从编号为k的人开始报数，数到m的那个人出列；他的下一个人又从1开始报数，数到m的那个人又出列；依此规律重复下去，直到圆桌周围的人全部出列。
```java
/**
 * @author 16026
 *
 */
import java.util.ArrayList;
import java.util.List;
import java.util.Scanner;
public class Yue {
    public static void main(String[] args) {
        Scanner scanner = new Scanner(System.in);
        System.out.print("请输入总人数：");
        int totalNum = scanner.nextInt();
        System.out.print("请输入报数的大小：");
        int cycleNum = scanner.nextInt();
        yuesefu(totalNum, cycleNum);
        scanner.close();
    }
    public static void yuesefu(int totalNum, int countNum) {
        // 初始化人数
        List<Integer> start = new ArrayList<Integer>();
        for (int i = 1; i <= totalNum; i++) {
            start.add(i);
        }
        // 从第K个开始计数
        int k = 0;
        while (start.size() > 0) {
            k = k + countNum;
            // 第m人的索引位置
            k = k % (start.size()) - 1;
            // 判断是否到队尾
            if (k < 0) {
                System.out.println(start.get(start.size() - 1));
                start.remove(start.size() - 1);
                k = 0;
            } else {
                System.out.println(start.get(k));
                start.remove(k);
            }
        }
    }
}
```
- 1
- 2
- 3
- 4
- 5
- 6
- 7
- 8
- 9
- 10
- 11
- 12
- 13
- 14
- 15
- 16
- 17
- 18
- 19
- 20
- 21
- 22
- 23
- 24
- 25
- 26
- 27
- 28
- 29
- 30
- 31
- 32
- 33
- 34
- 35
- 36
- 37
- 38
- 39
- 40
- 41
- 42
- 43
- 44
![这里写图片描述](https://img-blog.csdn.net/20170305144815828?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvcXFfMjExNTA4NjU=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)
