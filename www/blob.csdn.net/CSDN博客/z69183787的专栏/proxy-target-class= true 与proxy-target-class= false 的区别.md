# proxy-target-class="true" 与proxy-target-class="false"的区别 - z69183787的专栏 - CSDN博客
2013年12月06日 09:36:25[OkidoGreen](https://me.csdn.net/z69183787)阅读数：27490
<tx:annotation-driven transaction-manager="transactionManager" 
                                       proxy-target-class="true"/>
　　注意：proxy-target-class属性值决定是基于接口的还是基于类的代理被创建。如果proxy-target-class 属性值被设置为true，那么基于类的代理将起作用（这时需要cglib库）。如果proxy-target-class属值被设置为false或者这个属性被省略，那么标准的JDK 基于接口的代理将起作用。
即使你未声明 proxy-target-class="true" ，但运行类没有继承接口，spring也会自动使用CGLIB代理。
高版本spring自动根据运行类选择 JDK 或 CGLIB 代理
