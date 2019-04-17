# Java设计模式（适配器模式） - DEVELOPER - CSDN博客





2017年04月14日 10:58:18[学术袁](https://me.csdn.net/u012827205)阅读数：298标签：[JAVA设计模式](https://so.csdn.net/so/search/s.do?q=JAVA设计模式&t=blog)
个人分类：[java设计模式](https://blog.csdn.net/u012827205/article/category/2345605)

所属专栏：[JAVA设计模式](https://blog.csdn.net/column/details/15234.html)









想了解一下Java设计模式的适配器模式，然后在网上翻了翻。唉~痛心！就看到一篇让我满意的，然后就发给李达康书记看了看。达康书记说：“确实不错！”；我认真研究了一下，根据自己的理解做了一下记录！

什么是适配器模式（adapter）？？将一个类的接口转换成客户希望的另外一个接口。适配器模式使得原本由于接口不兼容而不能一起工作的那些类可以一起工作。

优点：将目标类和适配者类解耦，通过引入一个适配器类(adapter)来重用现有的适配者类(adaptee)，而无须修改原有代码,而且提高了适配者的复用性。

怎么理解“适配器这个词儿”呢？举个简单的栗子：就说我们使用的手机，苹果手机和安卓手机。他们为各自进行电池充电的时候，使用的充电接口类型是不一样的。苹果手机的是apple lighting,安卓手机的是Type-C。如果你分别有一个这样的手机，平时没电的时候使用各自的充电数据线插一下就行了。但是，如果突然发现苹果的数据线没了怎么办？简单，买一个唄！请不要这样，人家第一次遇到这种突发事件。接着说，如果你有一个转换头，这样所有事情不久暂时解决了吗！

而这个“转换头”就是这样的：[好博文地址](http://www.hollischuang.com/archives/1524)

![](https://img-blog.csdn.net/20170414143058405)

在我们的代码中我们可以把它当作“适配器Adapter”。

对于普通的充电方式，比如安卓手机（代码实现）：

```java
//接口：安卓充电接口
public interface IBatteryChargingForAndroid {
    //type-c 充电接口
    void batteryChargingWithTypeC();
}
```


```java
//安卓充电器
public class AndroidBatteryCharger implements IBatteryChargingForAndroid{
    @Override
    public void batteryChargingWithTypeC() {
        System.out.println("安卓手机正在充电中... \n请稍后骚扰我~");
    }
}
```


```java
//安卓手机(独立完成各自的电充)
public class AndroidPhone {
    private IBatteryChargingForAndroid iAndroid;
    public AndroidPhone(IBatteryChargingForAndroid IAndroid) {
        this.iAndroid = iAndroid;
    }

    //给安卓手机充电噻~
    public void batteryCharging(){
        iAndroid.batteryChargingWithTypeC();
    }
}
```




对于普通的充电方式，比如苹果手机（代码实现）：

```java
//接口：苹果充电接口
public interface IBatteryChargingForIOS {
    //苹果专用充电接口
    void batteryChargingWithIOS();
}
```

```java
//苹果充电器
public class IOSBatteryCharger implements IBatteryChargingForIOS {
    @Override
    public void batteryChargingWithIOS() {
        System.out.println("IPhone7 正在充电中... \n请稍后骚扰我~");
    }
}
```

```java
//苹果手机(独立完成各自的电充)
public class IOSPhone {
    private IBatteryChargingForIOS iosPhone;

    public IOSPhone(IBatteryChargingForIOS iosPhone) {
        this.iosPhone = iosPhone;
    }

    //给IPhone7 手机充电噻~(普通充电)
    public void batteryCharging(){
        iosPhone.batteryChargingWithIOS();
    }

}
```






用户拿起手机进行充电：
```java
public class MainCharger {
    public static void main(String[] args){
        //安卓手机的普通充电
        IBatteryChargingForAndroid iAndroid = new AndroidBatteryCharger();
        AndroidPhone androidPhone = new AndroidPhone(iAndroid);
        androidPhone.batteryCharging();

        //苹果手机的普通充电
        IBatteryChargingForIOS iIOS = new IOSBatteryCharger();
        IOSPhone iosPhone = new IOSPhone(iIOS);
        iosPhone.batteryCharging();

    }
}
```


然后再看一下突发情况处理：如果没有苹果的数据线怎么弄？这个时候需要使用一个适配器来完成苹果手机的充电功能。比如；
```java
//充电器 适配器（通过充电器的转换插头，能实现对IPhone的充电功能）
public class BatteryChargerAdapter implements IBatteryChargingForIOS{

    /**
     * 从当前类完成适配器的功能；
     * 目标（target）：IBatteryChargingForIOS
     * 适配者（adaptee）：IBatteryChargingForAndroid
     * 适配器（adapter）：BatteryChargerAdapter
     */
    private IBatteryChargingForAndroid iAndroid;
    public BatteryChargerAdapter(IBatteryChargingForAndroid iAndroid){
        this.iAndroid = iAndroid;
    }


    @Override
    public void batteryChargingWithIOS() {
        //通过转换插头，将安卓的type-c接入，完成苹果充电功能
        iAndroid.batteryChargingWithTypeC();
    }
}
```
苹果的充电也要做出简单的调整：
```java
//苹果手机(独立完成各自的电充)
public class IOSPhone {
    private IBatteryChargingForIOS iosPhone;

    public IOSPhone(){}

    public IOSPhone(IBatteryChargingForIOS iosPhone) {
        this.iosPhone = iosPhone;
    }

    //给IPhone7 手机充电噻~(普通充电)
    public void batteryCharging(){
        iosPhone.batteryChargingWithIOS();
    }

    //使用适配器进行转换-为转换器充电做准备
    public void setAdapter(IBatteryChargingForIOS iosPhone){
        this.iosPhone = iosPhone;
    }
}
```


最后再次拿起手机充电：
```java
public class MainCharger {
    public static void main(String[] args){
        //安卓手机的普通充电
        IBatteryChargingForAndroid iAndroid = new AndroidBatteryCharger();
        AndroidPhone androidPhone = new AndroidPhone(iAndroid);
        androidPhone.batteryCharging();

        //苹果手机的普通充电
        IBatteryChargingForIOS iIOS = new IOSBatteryCharger();
        IOSPhone iosPhone = new IOSPhone(iIOS);
        iosPhone.batteryCharging();

        //使用转化器 之后对苹果手机进行充电
        //其实使用转化器充电的核心就是：可以在 BatteryChargerAdapter$batteryChargingWithIOS 方法
        //中看到，其实是变相的重写了苹果充电器接口 进行充电的 方法。
        BatteryChargerAdapter adapter = new BatteryChargerAdapter(iAndroid);
        iosPhone.setAdapter(adapter);
        iosPhone.batteryCharging();


    }
}
```









