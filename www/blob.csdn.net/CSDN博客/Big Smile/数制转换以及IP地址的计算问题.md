# 数制转换以及IP地址的计算问题 - Big Smile - CSDN博客
2016年11月06日 23:29:00[王啸tr1912](https://me.csdn.net/tr1912)阅读数：635标签：[网络																[二进制																[IP](https://so.csdn.net/so/search/s.do?q=IP&t=blog)](https://so.csdn.net/so/search/s.do?q=二进制&t=blog)](https://so.csdn.net/so/search/s.do?q=网络&t=blog)
个人分类：[网络																[总结](https://blog.csdn.net/tr1912/article/category/6109422)](https://blog.csdn.net/tr1912/article/category/6315927)
        这几天还是有同学一直在问数制转换的问题，所以来这里简单描述一下具体的思路
# 一、数制转换与存储空间的运算（二进制有关）
        其实在很早的时候我们就有了对于二进制进行计算的概念，但是当时意识还是不够强烈，但是现在做题做的多了感触还是颇深的。
        首先我们来讲二进制和十进制的转换，其实有多种方法，这里我讲我最常用的一种，就是位权法。老师最开始讲的快速的方法也是这个。
        我们都知道，二进制就是满二进一，和十进制是类似的。什么是位权呢，位权就是这个里面美一位数字有了值能代表多大的数字，比如十进制，每个位上最大的数字是9，所以位权表示如下：
![](https://img-blog.csdn.net/20161106213848102?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQv/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)
        那么我们想要表示十进制的6542怎么办呢，当然我们知道可以直接向里面添加数字了，但是我们要是按照位权添加是不是就也是一样了呢？由于十进制每个位上都有多个数字值可以表示，所以不便于用位权来表示，但是二进制的数字就不一样了，因为每个位置上只有0和1这两个数字，所以表示的也就是位权的内容，让我们来看一下二进制的位权：
![](https://img-blog.csdn.net/20161106213856915?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQv/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)
    有了位权，我们标志数字在二进制上就容易多了，举个例子，例如表示二进制的22，我们用位权如下图所示
![](https://img-blog.csdn.net/20161106214548796?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQv/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)
        把数字按位权分解，总能得到一个完整的位权相加的结果，所以在用到的位权上用1来标志，用0在未使用的位权上边标识，最后就完成了。
        那么十六进制的数字表示也就不复杂了，由于四位二进制可以标志一位十六进制（4位二进制最大数字是15，符合十六进制标准）如十六进制7D3B2H可以表示为：
![](https://img-blog.csdn.net/20161106215413249?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQv/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)
        这样一看其实进制的转换其实也是很简单的，下面的应用会让这个方法更加的有效率。
# 二、IP地址的计算
        首先我们先要弄清楚IP地址的计算都有什么，首先IP地址有主机地址，子网掩码，网络号和IP地址，他们之间存在着千丝万缕的联系，他们之间只要知道两个是可以计算出其他两个的，这一点很是重要。
那么首先来说他们的关系是什么呢？如下图所示，这个就是他们之间的运算关系。
![](https://img-blog.csdn.net/20161106231956132?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQv/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)
其实很简单对不对，当你得到了IP地址和任何一个地址都可以进行计算，举个例子:  
IP：192.123.11.5      子网掩码：255.255.254.0          
进行运算，这里交给大家一个办法：
首先把他们都变为二进制地址：
IP：11000000.01111010.00001011.00000101      子网掩码：11111111.11111111.11111110.00000000
把子网掩码1所在的位置在IP中置零，即可得到主机地址：0.0.1.5          
把子网掩码中0所在的位置在IP中置零，得到网络地址：192.123.10.0        
这个里面的运算和与的运算是等价的，于是乎我们就少了取反的步骤。

