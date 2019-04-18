# iOS设计模式四部曲(一):创建型模式 内附Demo - weixin_33985507的博客 - CSDN博客
2017年08月22日 01:09:00[weixin_33985507](https://me.csdn.net/weixin_33985507)阅读数：10
> 
最近刚重温完经典书籍[《EffectiveObjective-C2.0编写高质量iOS与OSX代码的52个有效方法》](https://www.jianshu.com/p/862b064e82e0)，接下来准备把设计模式扫扫盲，强烈推荐图书《Head First设计模式》，如果看完了有空可以翻翻《大话设计模式》或者《设计模式之禅》，这几篇就是阅读学习设计模式的产物吧。因设计模式大致可分三种类型以及MVX架构系列，所以总共分了4篇来讲解：`创建型模式篇`，`结构性模式篇`，`行为型模式篇`，`架构型模式篇`，这是第一篇，由于个人能力有限，难免有一些遗漏或者错误，请各位看官不吝赐教！谢谢！本文所有Demo可以在我的Git上获取，[请点击这里](https://link.jianshu.com?t=https://github.com/maligh/DesignPatterns)
![1457495-b27a7821529beb5d.png](https://upload-images.jianshu.io/upload_images/1457495-b27a7821529beb5d.png)
设计模式.png
闲言碎语不多讲，上图是本系列的一个总体大纲。这篇文章是第一部：创建型模式。不过在开讲之前首先说一下设计模式的六大原则：
![1457495-4cac842b1e171838.png](https://upload-images.jianshu.io/upload_images/1457495-4cac842b1e171838.png)
关于这六大原则的解释，具体可以看我之前整理的一篇文章，[请点击这里前往](https://www.jianshu.com/p/807bc228dbc2)
创建型模式包括：`原型模式`，`单例模式`，`工厂方法模式`，`抽象工厂模式`，`建造者模式`，下面我们就通过[Demo](https://link.jianshu.com?t=https://github.com/maligh/DesignPatterns)来一个一个讲解。
## 原型模式（Prototype）：
**1. 定义：**原型模式是通过克隆已有的对象来创建新的对象，已有的对象称为原型。通俗来讲，原型模式就是允许你创建现有对象的副本并根据需要进行修改，而不是从头开始创建对象并进行设置。
**2. 使用场景：**通过初始化产生一个对象需要非常繁琐的准备步骤，也就是新生成一个对象的代价比较大，则可以考虑使用原型模式。
**3. 具体实现：**原型模式实现起来比较简单，iOS实现这个模式用的就是`copy`方法，如果是类使用`copy`，那这个类就要实现。`NSCopying`协议中的`copyWithZone`方法，告诉程序如何复制该对象。（当然也可以自己定义协议和自定义copy方法来克隆，但是不建议这样做)。具体代码可以看[Demo](https://link.jianshu.com?t=https://github.com/maligh/DesignPatterns/tree/master/MLDesignPatterns-OC/MLDesignPatterns-OC/Prototype)。另外在`Foudation`框架中你也会找到大量实现了`NSCopying`或者`NSMutableCopying`的类。
**4.优点：** 在某些场景可以避免内存开销大的创建初始化动作。
**5.缺点：** 基本没有缺点可言。
**6.注意事项:** 涉及到`copy`，注意下深复制和浅复制就好。
## 单例模式（Singleton）：
**1.定义:** 单例模式能够确保某个类在应用中只存在一个实例，创建之后会向整个系统共用这个实例。
**2. 使用场景：** 需要用来保存全局的状态，并且不和任何作用域绑定的时候可以考虑单例。
**3. 具体实现：**
最简单最常见的一种实现方式:
```
+ (instancetype)sharedInstance {
    static id shareInstance;
    static dispatch_once_t onceToken;
    dispatch_once(&onceToken, ^{
        shareInstance = [[self alloc] init];
    });
    return shareInstance;
}
```
**4.优点：**在某些场景节省了一些内存开销，尤其是对一个需要频繁创建销毁的对象。
**5.缺点：**单例对象一旦建立，对象指针是保存在静态区的，单例对象在堆中分配的内存空间，会在应用程序终止后才会被释放。�虽然也可以做到提前释放，但一般很少这样去做。另外单例类无法继承，扩展很困难。
**6.注意事项:** 单例模式比较常用，也可能是很多初级工程师唯一会使用的设计模式。这里还是要尽量避免滥用单例，大家可以查看[这篇文章](https://link.jianshu.com?t=https://www.objccn.io/issue-13-2/) 另外还要防止一下对单例对象的`copy`操作。
## 工厂方法模式（FactoryMethod）：
**1.定义:** 定义一个用于创建对象的接口，让子类决定实例化哪一个类。
**2. 使用场景：** 当存在多个类共同实现一个协议或者共同继承一个基类的时候，需要创建不同的对象，这个时候就可以考虑是否有必要使用工厂类进行管理。
**3. 具体实现：**  这里引用《设计模式之禅》一书中女娲造人的示例，[具体实现请看demo](https://link.jianshu.com?t=https://github.com/maligh/DesignPatterns/tree/master/MLDesignPatterns-OC/MLDesignPatterns-OC/FactoryMethod)，另外还有工厂方法模式还可以缩小成简单工厂模式，形如：
```
//CarFactory.m
+ (Car *)createCarWithType:(NSIntger)passengersCount {
    if (passengersCount < 2) {
        return [[SportCar alloc] init];
    } else if (passengersCount < 4) {
        return [[NormalCar alloc] init];
    } else {
        return [[SUV alloc] init];
    }
}
```
但简单工厂模式的耦合和扩展方面存在一些问题，基本所有使用简单工厂模式的地方都可以用`依赖注入`来解决掉这个问题。
**4.优点：** 1.在工厂方法中，用户只需要知道所要产品的具体工厂，不需要知道具体的创建过程，甚至不需要具体产品类。2.在系统增加新的产品时，我们只需要添加一个具体产品类和对应的实现工厂，无需对原工厂进行任何修改，很好地符合了“开闭原则”。
**5.缺点：** 每次增加一个产品时，都需要增加一个具体类和对象实现工厂，代码量会增加，也增加了系统的复杂度。
## 抽象工厂模式（AbstractFactory）：
**1.定义:** 抽象工厂模式是工厂方式模式的升级版本，抽象工厂模式允许调用组件在不了解创建对象所需类的情况下，创建一组相关或者互相依赖的对象。
**2. 使用场景：** 一个对象族有相同的约束时可以使用抽象工厂模式。
**3. 具体实现：** 这里引用了一个生产门的工厂，木门需要搭配木门安装工，铁门需要搭配铁门安装工，[具体实现请看demo](https://link.jianshu.com?t=https://github.com/maligh/DesignPatterns/tree/master/MLDesignPatterns-OC/MLDesignPatterns-OC/AbstractFactory)
**4.优点：** 良好的封装性：抽象工厂模式允许调用组件不必了解创建对象使用的类，也不必知道为什么选择这些类，因为我可以在不修改调用组件的情况下，对使用的类进行修改。
**5.缺点：**  扩展产品族困难，需要更改接口及其下所有子类(什么是产品族：在抽象工厂模式中，产品族是指由同一个工厂生产的。例如苹果手机，苹果平板，苹果电脑)。
## 建造者模式（Builder）：
**1.定义:** 将一个复杂的对象的构建与他的表示分离，使得同样的构建过程可以创建不同的表示。
**2. 使用场景：** 当创建多种风格的对象时或者创建对象时涉及很多步骤，可以使用建造者模式。
**3. 具体实现：** 这里举了一个手抓饼的例子，5元的不辣手抓饼需要添加（生菜 + 火腿肠 + 鸡蛋，味精 + 番茄酱），10元的变态辣手抓饼需要添加(生菜 + 热狗 + 肉松 + 里脊 + 芝士，辣椒 + 辣酱 + 麻酱 + 干辣椒 + 剁辣椒 + 老干妈 + 辣椒油)，[具体实现请看demo](https://link.jianshu.com?t=https://github.com/maligh/DesignPatterns/tree/master/MLDesignPatterns-OC/MLDesignPatterns-OC/Builder)
**4.优点：** 1.将产品的创建过程与产品本身分离开来，可以使用相同的创建过程来得到不同的产品。2.每一个具体建造者都相对独立，因此可以很方便地替换具体建造者或增加新的具体建造者。
**5.缺点：** 1.建造者模式所创建的产品一般具有较多的共同点，其组成部分相似，如果产品之间的差异性很大，则不适合使用建造者模式，因此其使用范围受到一定的限制。2.如果产品的内部变化复杂，可能会导致需要定义很多具体建造者类来实现这种变化，导致系统变得很庞大。
**EOF :** 这篇文章通过[Demo](https://link.jianshu.com?t=https://github.com/maligh/DesignPatterns/tree/master/MLDesignPatterns-OC/MLDesignPatterns-OC/)梳理了设计模式中的创建型模式，由于个人能力有限，难免有一些遗漏或者错误，还请各位看官不吝赐教！下周会给大家带来第二篇`结构性模式篇`，本文已同步到[个人博客](https://link.jianshu.com?t=http://www.superma.me/2017/08/22/iOS%E8%AE%BE%E8%AE%A1%E6%A8%A1%E5%BC%8F%E5%9B%9B%E9%83%A8%E6%9B%B2%E4%B8%80%E5%88%9B%E5%BB%BA%E5%9E%8B%E6%A8%A1%E5%BC%8F%E5%86%85%E9%99%84Demo/)，**欢迎关注，欢迎点赞，欢迎star，欢迎一起交流，一起进步！**🤓
