# fluent编程风格 - weixin_33985507的博客 - CSDN博客
2017年11月08日 14:54:00[weixin_33985507](https://me.csdn.net/weixin_33985507)阅读数：13
在看zookeeper过程中，看到Curator客户端工具，在创建Zookeeper client 的过程中，使用了fluent风格的编码。对我来说这个是新的概念，那么什么是fluent设计风格呢，让我们来看看！
创建client 示例1
![2306592-18d4665b44ddbfa9.png](https://upload-images.jianshu.io/upload_images/2306592-18d4665b44ddbfa9.png)
Curator创建示例.png
创建示例2
Google Collections中的MapMaker是个不错的示例：
```
ConcurrentMap graphs = new MapMaker()
.concurrencyLevel(32)
.softKeys()
.weakValues()
.expiration(30, TimeUnit.MINUTES)
.makeComputingMap(
    new Function() {
        public Graph apply(Key key) {
            return createExpensiveGraph(key);
        }
      });
```
由此可见，使用Fluent Interface：Fluent Interface可以使代码更短，更容易编写。
原先创建和赋值是这么写：
The most common way I see this kind of thing built up is like this:
```
private void makeNormal(Customer customer) {
    Order o1 = new Order();
    customer.addOrder(o1);
    OrderLine line1 = new OrderLine(6, Product.find("TAL"));
    o1.addLine(line1);
    OrderLine line2 = new OrderLine(5, Product.find("HPK"));
    o1.addLine(line2);
    OrderLine line3 = new OrderLine(3, Product.find("LGV"));
    o1.addLine(line3);
    line2.setSkippable(true);
    o1.setRush(true);
}
```
以后可以这么写：
```
private void makeFluent(Customer customer) {
    customer.newOrder()
            .with(6, "TAL")
            .with(5, "HPK").skippable()
            .with(3, "LGV")
            .priorityRush();
}
```
关于这种风格可能最重要的一点是，其目的是按照内部的域特定语言来做一些事情 。引自：[这](https://link.jianshu.com?t=https://martinfowler.com/bliki/FluentInterface.html)，有兴趣的可以看看。
