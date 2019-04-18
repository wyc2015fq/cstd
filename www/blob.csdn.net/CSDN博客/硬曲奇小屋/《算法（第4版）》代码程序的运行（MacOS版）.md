# 《算法（第4版）》代码程序的运行（MacOS版） - 硬曲奇小屋 - CSDN博客





2018年01月23日 22:52:44[HardCookies](https://me.csdn.net/james_616)阅读数：145
个人分类：[算法																[java](https://blog.csdn.net/james_616/article/category/7420154)](https://blog.csdn.net/james_616/article/category/7420152)








《算法》上的Java运行环境的搭建按照本书网站上的[教程](https://algs4.cs.princeton.edu/mac/)就应该能完成，下面讲一下如何编译运行书中的代码。有两种方式，以书中第37页代码为例，我们分别介绍这两种方式。

```java
public class RandomSeq { 
    public static void main(String[] args) {
        int n = Integer.parseInt(args[0]);
        double lo = Double.parseDouble(args[1]);
        double hi = Double.parseDouble(args[2]);
        for (int i = 0; i < n; i++) {
            double x = StdRandom.uniform(lo, hi);
            StdOut.printf("%.2f\n", x);
        }
    }
}
```

## 1 导入本书标准库Std*

在配置好环境后，如果要直接调用库中的方法比如此处的StdRandom和StdOut，需要在代码前添加如下代码

```java
import edu.princeton.cs.algs4.StdRandom;
import edu.princeton.cs.algs4.StdOut;
```

之后需要用`javac-algs4`来编译RandomSeq.java，并用`java-algs4`来运行程序。结果如下图所示： 
![这里写图片描述](https://img-blog.csdn.net/20180123224605271?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvamFtZXNfNjE2/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)
## 2 存放静态库在相同路径

在文件夹RandomSeq中，存放RandomSeq.java, StdRandom.java和StdOut.java。 

之后可以直接用`javac`编译RandomSeq.java，用`java`运行RandomSeq程序。





