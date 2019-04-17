# 程序设计之Java设计模式(抽象工厂模式) - 纸上得来终觉浅，绝知此事要躬行 - CSDN博客





2015年03月01日 10:08:27[boonya](https://me.csdn.net/boonya)阅读数：603标签：[java																[AbstractFactory																[工厂模式](https://so.csdn.net/so/search/s.do?q=工厂模式&t=blog)
个人分类：[设计模式																[Java](https://blog.csdn.net/boonya/article/category/878447)](https://blog.csdn.net/boonya/article/category/1628871)





原文地址：[http://zhangwei-david.iteye.com/blog/2187627](http://zhangwei-david.iteye.com/blog/2187627)，该文通俗易懂，实为经典！



 在设计模式一书中是这样描述抽象工厂的意图的： 提供一个创建一系列相关或相互依赖对象的接口，而无需指定他们具体的实现类。

    使用场景：一个系统要独立于它的产品创建、组合和表示时，且一个该系统提供多个产品系列，系统每次仅消费其中一个系类时可以使用抽象工厂模式。

    假设现在手机厂商在生产手机（phone）的同时也生产平板（Pad）（这两个产品是一个系列，都是移动端产品）,简单的定义两个产品接口



Java代码  ![收藏代码](http://zhangwei-david.iteye.com/images/icon_star.png)

- /**
-  *
-  * @author zhangwei_david
-  * @version $Id: Pad.java, v 0.1 2015年2月28日 下午4:54:33 zhangwei_david Exp $
-  */
- publicinterface Pad {  
- public String getBrand();  
- }  




Java代码  ![收藏代码](http://zhangwei-david.iteye.com/images/icon_star.png)

- /**
-  *
-  * @author zhangwei_david
-  * @version $Id: Phone.java, v 0.1 2015年2月28日 下午4:28:34 zhangwei_david Exp $
-  */
- publicinterface Phone {  
- 
- public String getBrand();  
- 
- }  


 不同厂商生产的产品的品牌一定是不同的，定义两组具体实现类



Java代码  ![收藏代码](http://zhangwei-david.iteye.com/images/icon_star.png)

- /**
-  *
-  * @author zhangwei_david
-  * @version $Id: Ipad.java, v 0.1 2015年2月28日 下午4:55:06 zhangwei_david Exp $
-  */
- publicclass Ipad implements Pad {  
- 
- /**
-      * @see com.pattern.create.abstractFactory.product.Pad#getBrand()
-      */
- @Override
- public String getBrand() {  
- return"APPLE";  
-     }  
- 
- }  






Java代码  ![收藏代码](http://zhangwei-david.iteye.com/images/icon_star.png)

- /**
-  *
-  * @author zhangwei_david
-  * @version $Id: Iphone4.java, v 0.1 2015年2月28日 下午4:35:50 zhangwei_david Exp $
-  */
- publicclass Iphone implements Phone {  
- 
- privatestaticfinal String BRAND = "APPLE";  
- 
- /**
-      * @see com.pattern.create.abstractFactory.product.Phone#getBrand()
-      */
- @Override
- public String getBrand() {  
- return BRAND;  
-     }  
- 
- }  


 第二组产品：



Java代码  ![收藏代码](http://zhangwei-david.iteye.com/images/icon_star.png)

- /**
-  *
-  * @author zhangwei_david
-  * @version $Id: MiPad.java, v 0.1 2015年2月28日 下午4:55:53 zhangwei_david Exp $
-  */
- publicclass MiPad implements Pad {  
- 
- /**
-      * @see com.pattern.create.abstractFactory.product.Pad#getBrand()
-      */
- @Override
- public String getBrand() {  
- return"MI";  
-     }  
- 
- }  




Java代码  ![收藏代码](http://zhangwei-david.iteye.com/images/icon_star.png)

- /**
-  *
-  * @author zhangwei_david
-  * @version $Id: MiPhone.java, v 0.1 2015年2月28日 下午4:36:50 zhangwei_david Exp $
-  */
- publicclass MiPhone implements Phone {  
- 
- privatestaticfinal String BRAND = "MI";  
- 
- /**
-      * @see com.pattern.create.abstractFactory.product.Phone#getBrand()
-      */
- @Override
- public String getBrand() {  
- return BRAND;  
-     }  
- 
- }  


 通过上面两组产品可以发现，每组产品的品牌都是一致的，也就是在工厂生产的时候不可能一条生产线上生成两种不同的品牌产品（山寨除外）



定义一个抽象工厂的接口，该接口提供生产一组产品的操作



Java代码  ![收藏代码](http://zhangwei-david.iteye.com/images/icon_star.png)

- /**
-  *
-  * @author zhangwei_david
-  * @version $Id: AbstractFactory.java, v 0.1 2015年2月28日 下午4:27:57 zhangwei_david Exp $
-  */
- publicinterface AbstractFactory {  
- 
- public Phone createPhone();  
- 
- public Pad createPad();  
- }  


 有两个具体实现类：



Java代码  ![收藏代码](http://zhangwei-david.iteye.com/images/icon_star.png)

- publicclass AppleFactory implements AbstractFactory {  
- 
- /**
-      * @see com.pattern.create.abstractFactory.AbstractFactory#createPhone()
-      */
- @Override
- public Phone createPhone() {  
- returnnew Iphone();  
-     }  
- 
- /**
-      * @see com.pattern.create.abstractFactory.AbstractFactory#createPad()
-      */
- @Override
- public Pad createPad() {  
- returnnew Ipad();  
-     }  
- 
- }  




Java代码  ![收藏代码](http://zhangwei-david.iteye.com/images/icon_star.png)

- publicclass MiFactory implements AbstractFactory {  
- 
- /**
-      * @see com.pattern.create.abstractFactory.AbstractFactory#createPhone()
-      */
- @Override
- public Phone createPhone() {  
- returnnew MiPhone();  
-     }  
- 
- /**
-      * @see com.pattern.create.abstractFactory.AbstractFactory#createPad()
-      */
- @Override
- public Pad createPad() {  
- returnnew MiPad();  
-     }  
- 
- }  




Java代码  ![收藏代码](http://zhangwei-david.iteye.com/images/icon_star.png)

- publicclass Client {  
- 
- /**
-      *
-      * @param args
-      */
- publicstaticvoid main(String[] args) {  
-         AbstractFactory appleFactory = new AppleFactory();  
-         System.out.println(appleFactory.createPad().getBrand() + " PAD");  
-         System.out.println(appleFactory.createPhone().getBrand() + " PHONE");  
- 
-         AbstractFactory miFactory = new MiFactory();  
-         System.out.println(miFactory.createPad().getBrand() + " PAD");  
-         System.out.println(miFactory.createPhone().getBrand() + " PHONE");  
-     }  
- 
- }  


 结果：



Java代码  ![收藏代码](http://zhangwei-david.iteye.com/images/icon_star.png)

- APPLE PAD  
- APPLE PHONE  
- MI PAD  
- MI PHONE  


**优点：**
- 分离了具体的类
- 易于更换产品系列
- 有利于维护产品一致性

**缺点：**
- 产品簇中难以增加新的产品

抽象工厂和工厂方法的比较：

     抽象工厂和工厂方法都分离了工厂接口和具体实现，不同点是工厂方法中仅抽象一个产品，有多个产品的实现，在抽象工厂中有多个抽象产品，有多组产品实现，但是具体工厂只能提供其中一组产品！](https://so.csdn.net/so/search/s.do?q=AbstractFactory&t=blog)](https://so.csdn.net/so/search/s.do?q=java&t=blog)




