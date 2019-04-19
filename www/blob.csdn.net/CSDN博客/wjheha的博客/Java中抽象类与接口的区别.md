# Java中抽象类与接口的区别 - wjheha的博客 - CSDN博客
2017年10月13日 19:29:27[wjheha](https://me.csdn.net/wjheha)阅读数：67标签：[java																[abstract](https://so.csdn.net/so/search/s.do?q=abstract&t=blog)](https://so.csdn.net/so/search/s.do?q=java&t=blog)
个人分类：[Java/Java Web](https://blog.csdn.net/wjheha/article/category/6775424)
在Java中，抽象类与接口很相似，它们之间的区别是啥呢？下面是一个表格记录
|序号|区别|抽象类|接口|
|----|----|----|----|
|1|关键字|abstract class|interface|
|2|组成|构造方法、普通方法、抽象方法、static方法、常量、变量|抽象方法、全局常量|
|3|子类使用|class 子类 extends 抽象类|class 子类 implemnents 接口1,接口2|
|4|关系|抽象类可以实现多个接口|接口可以继承多个父接口，但是不能继承抽象类|
|5|权限|可以使用各种权限|只能使用public权限|
|6|限制|单继承权限|无单继承权限|
|7|子类|抽象类和接口都必须有子类，子类必须要覆写全部抽象方法，依靠子类对象的向上转型进行对象的实例化|抽象类和接口都必须有子类，子类必须要覆写全部抽象方法，依靠子类对象的向上转型进行对象的实例化|
由上述比较发现：抽象类支持的功能绝对比接口更多，但是有一点不好，就是单继承权限，这一缺点掩盖了所有抽象类的优点，即：当抽象类和接口都可以使用时，优先考虑接口。
