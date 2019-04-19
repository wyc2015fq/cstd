# 小话设计模式原则之（2）：单一职责原则SRP - 文章 - 伯乐在线
原文出处： [懒得安分](http://www.cnblogs.com/landeanfen/p/5247366.html)
前言：上篇[小话设计模式原则之（1）：依赖倒置原则DIP](http://blog.jobbole.com/99563)简单介绍了下依赖倒置的由来以及使用，中间插了两篇WebApi的文章，这篇还是回归正题，继续来写写设计模式另一个重要的原则：单一职责原则。
# 一、原理介绍
## 1、官方定义
单一职责原则，英文缩写**SRP**，全称Single Responsibility Principle。
原始定义：There should never be more than one reason for a class to change。
官方翻译：应该有且仅有一个原因引起类的变更。简单点说，**一个类，最好只负责一件事，只有一个引起它变化的原因。**
## 2、自己理解
### 2.1、原理解释
上面的定义不难理解，**引起类变化的原因不能多于一个。也就是说每一个类只负责自己的事情，此所谓单一职责**。
我们知道，在OOP里面，高内聚、低耦合是软件设计追求的目标，而单一职责原则可以看做是高内聚、低耦合的引申，将职责定义为引起变化的原因，以提高内聚性，以此来减少引起变化的原因。职责过多，可能引起变化的原因就越多，这将是导致职责依赖，相互之间就产生影响，从而极大的损伤其内聚性和耦合度。单一职责通常意味着单一的功能，因此不要为类实现过多的功能点，以保证实体只有一个引起它变化的原因。
不管是从官方定义，还是对“单一职责”名称的解释，都能很好的理解单一职责原则的意义。**其实在软件设计中，要真正用好单一职责原则并不简单，因为遵循这一原则最关键的地方在于职责的划分，博主的理解是职责的划分是根据需求定的，同一个类（接口）的设计，在不同的需求里面，可能职责的划分并不一样，为什么这么说呢？我们来看下面的例子。**
# 二、场景示例
关于单一职责原则的原理，我们就不做过多的解释了。重点是职责的划分！重点是职责的划分！重点是职责的划分！重要的事情说三遍。下面根据一个示例场景来看看如何划分职责。
假定现在有如下场景：国际手机运营商那里定义了生产手机必须要实现的接口，接口里面定义了一些手机的属性和行为，手机生产商如果要生成手机，必须要实现这些接口。
## 1、初始设计——初稿
我们首先以手机作为单一职责去设计接口，方案如下。

C#
```
/// 
    /// 充电电源
    /// 
    public class ElectricSource
    {    }
　　public interface IMobilePhone
    {
        //运行内存
        string RAM { get; set; }
        //手机存储内存
        string ROM { get; set; }
        //CPU主频
        string CPU { get; set; }
        //屏幕大小
        int Size { get; set; }
        //手机充电接口
        void Charging(ElectricSource oElectricsource);
        //打电话
        void RingUp();
        //接电话
        void ReceiveUp();
        //上网
        void SurfInternet();
    }
```
然后我们的手机生产商去实现这些接口

C#
```
//具体的手机示例
    public class MobilePhone:IMobilePhone
    {
        public string RAM
        {
            get {throw new NotImplementedException();}
            set{ throw new NotImplementedException();}
        }
        public string ROM
        {
            get{throw new NotImplementedException();}
            set{ throw new NotImplementedException();}
        }
        public string CPU
        {
            get{ throw new NotImplementedException();}
            set{ throw new NotImplementedException();}
        }
        public int Size
        {
            get{throw new NotImplementedException();}
            set{throw new NotImplementedException();}
        }
        public void Charging(ElectricSource oElectricsource)
       {
            throw new NotImplementedException();
        }
        public void RingUp()
        {
            throw new NotImplementedException();
        }
        public void ReceiveUp()
        {
            throw new NotImplementedException();
        }
        public void SurfInternet()
        {
            throw new NotImplementedException();
        }
    }
```
这种设计有没有问题呢？这是一个很有争议的话题。单一职责原则要求一个接口或类只有一个原因引起变化，也就是一个接口或类只有一个职责，它就负责一件事情，**原则上来说，我们以手机作为单一职责去设计，也是有一定的道理的，因为我们接口里面都是定义的手机相关属性和行为，引起接口变化的原因只可能是手机的属性或者行为发生变化，从这方面考虑，这种设计是有它的合理性的，如果你能保证需求不会变化或者变化的可能性比较小，那么这种设计就是合理的。**但实际情况我们知道，现代科技日新月异，科技的进步促使着人们不断在手机原有基础上增加新的属性和功能。比如有一天，我们给手机增加了摄像头，那么需要新增一个像素的属性，我们的接口和实现就得改吧，又有一天，我们增加移动办公的功能，那么我们的接口实现是不是也得改。由于上面的设计没有细化到一定的粒度，导致任何一个细小的改动都会引起从上到下的变化，有一种“牵一发而动全身”的感觉。所以需要细化粒度，下面来看看我们如何变更设计。
## 2、二次设计——变更
我们将接口细化

C#
```
//手机属性接口
    public interface IMobilePhoneProperty
    {
        //运行内存
        string RAM { get; set; }
        //手机存储内存
        string ROM { get; set; }
        //CPU主频
        string CPU { get; set; }
        //屏幕大小
        int Size { get; set; }
        //摄像头像素
        string Pixel { get; set; }
    }
    //手机功能接口
    public interface IMobilePhoneFunction
    {
        //手机充电接口
        void Charging(ElectricSource oElectricsource);
        //打电话
        void RingUp();
        //接电话
        void ReceiveUp();
        //上网
        void SurfInternet();
        //移动办公
        void MobileOA();
    }
```
实现类

C#
```
//手机属性实现类
    public class MobileProperty:IMobilePhoneProperty
    {
        public string RAM
        {
            get{ throw new NotImplementedException();}
            set{ throw new NotImplementedException();}
        }
        public string ROM
        {
            get{ throw new NotImplementedException();}
            set{ throw new NotImplementedException();}
        }
        public string CPU
        {
            get{ throw new NotImplementedException();}
            set{throw new NotImplementedException();}
        }
        public int Size
        {
            get{throw new NotImplementedException();}
            set{throw new NotImplementedException();}
        }
        public string Pixel
        {
            get{throw new NotImplementedException();}
            set{throw new NotImplementedException();}
        }
    }
    //手机功能实现类
    public class MobileFunction:IMobilePhoneFunction
    {
        public void Charging(ElectricSource oElectricsource)
        {
            throw new NotImplementedException();
        }
        public void RingUp()
        {
            throw new NotImplementedException();
        }
        public void ReceiveUp()
        {
            throw new NotImplementedException();
        }
        public void SurfInternet()
        {
            throw new NotImplementedException();
        }
        public void MobileOA()
        {
            throw new NotImplementedException();
        }
    }
    //具体的手机实例
    public class HuaweiMobile
    {
        private IMobilePhoneProperty m_Property;
        private IMobilePhoneFunction m_Func;
        public HuaweiMobile(IMobilePhoneProperty oProperty, IMobilePhoneFunction oFunc)
        {
            m_Property = oProperty;
            m_Func = oFunc;
        }
    }
```
对于上面题的问题，这种设计能够比较方便的解决，如果是增加属性，只需要修改IMobilePhoneProperty和MobileProperty即可；如果是增加功能，只需要修改IMobilePhoneFunction和MobileFunction即可。貌似完胜第一种解决方案。那么是否这种解决方案就完美了呢？答案还是看情况。**原则上，我们将手机的属性和功能分开了，使得职责更加明确，所有的属性都由IMobilePhoneProperty接口负责，所有的功能都由IMobilePhoneFunction接口负责，如果是需求的粒度仅仅到了属性和功能这一级，这种设计确实是比较好的。**反之，如果粒度再细小一些呢，那我们这种职责划分是否完美呢？比如我们普通的老人机只需要一些最基础的功能，比如它只需要充电、打电话、接电话的功能，但是按照上面的设计，它也要实现IMobilePhoneFunction接口，某一天，我们增加了一个新的功能玩游戏，那么我们就需要在接口上面增加一个方法PlayGame()。可是我们老人机根本用不着实现这个功能，可是由于它实现了该接口，它的内部实现也得重新去写。从这点来说，以上的设计还是存在它的问题。那么，我们如何继续细化接口粒度呢？
## 3、最终设计——成型
接口细化粒度设计如下

C#
```
//手机基础属性接口
    public interface IMobilePhoneBaseProperty
    {
        //运行内存
        string RAM { get; set; }
        //手机存储内存
        string ROM { get; set; }
        //CPU主频
        string CPU { get; set; }
        //屏幕大小
        int Size { get; set; }
    }
    //手机扩展属性接口
    public interface IMobilePhoneExtentionProperty
    {
        //摄像头像素
        string Pixel { get; set; }
    }
    //手机基础功能接口
    public interface IMobilePhoneBaseFunc
    {
        //手机充电接口
        void Charging(ElectricSource oElectricsource);
        //打电话
        void RingUp();
        //接电话
        void ReceiveUp();
    }
    //手机扩展功能接口
    public interface IMobilePhoneExtentionFunc
    {
        //上网
        void SurfInternet();
        //移动办公
        void MobileOA();
        //玩游戏
        void PlayGame();
    }
```
实现类和上面类似

C#
```
//手机基础属性实现
    public class MobilePhoneBaseProperty : IMobilePhoneBaseProperty
    {
        public string RAM
        {
            get{throw new NotImplementedException();}
            set{throw new NotImplementedException();}
        }
        public string ROM
        {
            get{throw new NotImplementedException();}
            set {throw new NotImplementedException();}
        }
        public string CPU
        {
            get{throw new NotImplementedException();}
            set{ throw new NotImplementedException();}
        }
        public int Size
        {
            get{ throw new NotImplementedException();}
            set{ throw new NotImplementedException();}
        }
    }
    //手机扩展属性实现
    public class MobilePhoneExtentionProperty : IMobilePhoneExtentionProperty
    {
        public string Pixel
        {
            get{ throw new NotImplementedException();}
            set{ throw new NotImplementedException();}
        }
    }
    //手机基础功能实现
    public class MobilePhoneBaseFunc : IMobilePhoneBaseFunc
    {
        public void Charging(ElectricSource oElectricsource)
        {
            throw new NotImplementedException();
        }
        public void RingUp()
        {
            throw new NotImplementedException();
        }
        public void ReceiveUp()
        {
            throw new NotImplementedException();
        }
    }
    //手机扩展功能实现
    public class MobilePhoneExtentionFunc : IMobilePhoneExtentionFunc
    {
        public void SurfInternet()
        {
            throw new NotImplementedException();
        }
        public void MobileOA()
        {
            throw new NotImplementedException();
        }
        public void PlayGame()
        {
            throw new NotImplementedException();
        }
    }
```
此种设计能解决上述问题，细分到此粒度，这种方案基本算比较完善了。能不能算完美？这个得另说。接口的粒度要设计到哪一步，取决于需求的变更程度，或者说取决于需求的复杂度。
# 三、总结
以上通过一个应用场景简单介绍了下单一职责原则的使用，上面三种设计，没有最合理，只有最合适。理解单一职责原则，最重要的就是理解职责的划分，职责划分的粒度取决于需求的粒度，最后又回到了那句话：没有最好的设计，只有最适合的设计。
软件设计原则系列文章
- [《小话设计模式原则之（1）：依赖倒置原则DIP》](http://blog.jobbole.com/99563/)
- [《小话设计模式原则之（3）：接口隔离原则ISP》](http://blog.jobbole.com/99616/)
- [《小话设计模式原则之（4）：开闭原则OCP》](http://blog.jobbole.com/99617/)
