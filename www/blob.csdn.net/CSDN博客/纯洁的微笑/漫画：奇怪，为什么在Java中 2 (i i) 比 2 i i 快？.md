# 漫画：奇怪，为什么在Java中 2*(i*i) 比 2*i*i 快？ - 纯洁的微笑 - CSDN博客
2019年01月28日 08:36:00[微笑很纯洁](https://me.csdn.net/ityouknow)阅读数：1707
![640?wx_fmt=png](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_png/R5ic1icyNBNd5Dm9NfcHfNY0wh4eUjfH1WmApuHzZBzWWk1UEZjVt3TKWY0waEW0JqadQuCFaYhfib0OZTIAMgBicg/640?wx_fmt=png)
![640?wx_fmt=png](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_png/R5ic1icyNBNd5Dm9NfcHfNY0wh4eUjfH1W7mWiboWkK7C801sjraxR57QunPWHTcSOMiajEIjq6LK4DW8ZjGn8ZzNA/640?wx_fmt=png)
既然我设计的两只小萌宠出场了，也该它们的粑粑出场了，今天这篇文章，我们通过一个故事来深入聊聊 Java 编译背后的秘密。
![640?wx_fmt=png](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_png/R5ic1icyNBNd5Dm9NfcHfNY0wh4eUjfH1Wic9zVGAbXFFDbZqjY7ibPcMXYp28w9w2OITvkATYojgMl5WNeZwGmV6Q/640?wx_fmt=png)
![640?wx_fmt=png](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_png/R5ic1icyNBNd5Dm9NfcHfNY0wh4eUjfH1W4RicTdOaCqa1aicSr5LAQr3uw1gEgzMqSaTnGrzPZ0NmjfiaiaXcFedhXQ/640?wx_fmt=png)
![640?wx_fmt=png](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_png/R5ic1icyNBNd5Dm9NfcHfNY0wh4eUjfH1WctyyecOD47PxI52lzWibUickJg4zia3XDvBnzfEPP8iaAo4USQicSsRQYtQ/640?wx_fmt=png)
![640?wx_fmt=png](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_png/R5ic1icyNBNd5Dm9NfcHfNY0wh4eUjfH1WbHF6IdY0omv3RDyTOYXMUm1OicibWpyl7XPHnVViaN02owzA4ibK6Ahb8g/640?wx_fmt=png)
![640?wx_fmt=png](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_png/R5ic1icyNBNd5Dm9NfcHfNY0wh4eUjfH1WicYIEb9ia8fqKZC7eJweaAzc15epzY73OKK3YBPkQVEjicGWZ19HF58sg/640?wx_fmt=png)
东哥说这段代码来自于 Stackoverflow，真正去逛这个网站的同学还是很少），如下。
public static void main(String[] args) {
long startTime = System.nanoTime();
    int n = 0;
    for (int i = 0; i < 1000000000; i++) {
        n += 2 * i * i;
}
    System.out.println((double) (System.nanoTime() - startTime) / 1000000000 + " s");
System.out.println("n = " + n);
}
代码很简单吧，它的执行时间大概在 0.60s ~ 0.65s。但是如果你把 2*i*i 替换成 2*(i*i)，执行时间大概在 0.50s ~ 0.55s。
对这段程序的两个版本分别执行 15 次，得到的结果如下。
![640?wx_fmt=png](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_png/R5ic1icyNBNd5Dm9NfcHfNY0wh4eUjfH1WlficjYsfWh2EnT3BKE6CZlpFCT61GspN7NqEBC0ibg2CDxJ31Qn1SLQA/640?wx_fmt=png)
我们可以看出 2*(i*i) 比 2*i*i 快 。
![640?wx_fmt=png](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_png/R5ic1icyNBNd5Dm9NfcHfNY0wh4eUjfH1WaRWplXRrpyZI5KaVdA663BP31wbftVyoQGdZDWobQhUrN8dpvTCkvg/640?wx_fmt=png)
我们来分别查看它的字节码，这里东哥给我推荐了一款好用的 IDEA 插件，叫做 jclasslib bytecode viewer。
 2*i*i 的字节码如下。
![640?wx_fmt=png](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_png/R5ic1icyNBNd5Dm9NfcHfNY0wh4eUjfH1WQcOUW1RkxicodeRJ1KnK8EWicicdxSibI8vh5ricWNJiaEKnqBiaXWthv7ibfQ/640?wx_fmt=png)
2*(i*i) 的字节码如下。
![640?wx_fmt=png](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_png/R5ic1icyNBNd5Dm9NfcHfNY0wh4eUjfH1WVhrvDzCOr20nncDMWZ3N4L3CFiaTajqMicuOetnD0VeCy1NA2TnUP9OA/640?wx_fmt=png)
我们可以发现除了字节码顺序不同外，没有其它异常，下一步该怎么办呢？
![640?wx_fmt=png](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_png/R5ic1icyNBNd5Dm9NfcHfNY0wh4eUjfH1W2ouMREZY5ViblFibHXHHW9PxW9VrzZ4Ow9D0hcicEl9zicSF6ytUl1jZ4w/640?wx_fmt=png)
这里东哥给我推荐一款开源的工具，叫做 jitwatch，它可以查看查看分析HotSpot JIT compiler 生成的汇编代码，关于它的安装方法可以通过谷歌查阅。
查看是否安装成功？可以用下面的命令。
![640?wx_fmt=png](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_png/R5ic1icyNBNd5Dm9NfcHfNY0wh4eUjfH1WjgBc8AFVmL8xPaBUKxicsBH6fQO7HT1HPiaC3p5XaicXST7xu7tJdoZibg/640?wx_fmt=png)
如何查看汇编代码？
运行时，添加参数 -XX:+PrintAssembly。
好了，那我们来分别看看它们的汇编代码，由于汇编代码很多，我这里就不贴出来了，各位同学可以去运行分析一下。
我这里就说一下结论，通过对比分析，我们会发现，2*i*i 进行了大量的堆栈操作，因此，需要保存大量的中间结果；而 2*(i*i) 只有少量的堆栈操作。
显而易见，2*(i*i) 比 2*i*i 快是由于 JIT 优化的结果。
![640?wx_fmt=png](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_png/R5ic1icyNBNd5Dm9NfcHfNY0wh4eUjfH1W2YM6XxnXnDKArC3Jykdo3Veg3x4TZHCgiau9aibgHQhyibSTcPRiavTQ3A/640?wx_fmt=png)
-END-
**纯洁的微笑**
**一个有故事的程序员**
![640?wx_fmt=jpeg](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_jpg/PgqYrEEtEnqjV7GOKB2htgfZjgMjqxftxfmmdrLiaMKpyicTmLLX5fUjb6YxA6Z5Bhcozb3p0uMV7wqdKED89HZA/640?wx_fmt=jpeg)
