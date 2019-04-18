# Guice--Java依赖注入框架 - Orisun - 博客园







# [Guice--Java依赖注入框架](https://www.cnblogs.com/zhangchaoyang/articles/4232257.html)





### 面向接口编程

没有面向接口编程就没有依赖注入(Dependency Injection)，所以讲依赖注入之前先重温一下面向接口编程。

ps:[依赖注入(DI,Dependency Injection)和控制反转(IoC,Inversion of Control)的关系](http://www.cnblogs.com/niuniu1985/archive/2010/01/13/1646375.html)

```
public interface Vehicle {
    public String run();
}
```

```
public class Horse implements Vehicle{
    @Override
    public String run() {
        return "Horse Run.";
    }
}
```

```
public class Soldier implements Hero {
    Vehicle vehicle;
    public Soldier(Vehicle vehicle) {
        this.vehicle = vehicle;
    }
    @Override
    public String fight() {
        return vehicle.run() + " charge";
    }
}
```
- 上面就是一种依赖注入式的编程，即Soldier的fight方法依赖一个Vehicle，我们直接把一个Vehicle注入到Soldier类的内部，而不是在fight方法内临时创建一个Vehicle。
- 上面是一种面向接口的编程方式。假设Vehicle是我们依赖的一个外部的服务，在测试时我们可以自己实现一个简单的Vehicle，这样对Soldier类fight方法的测试就不再依赖于那个真实的外部服务了。当然如果不使用面向接口的编程方式，而是使用mock也可以实现同样的功能，但是个人感觉使用mock晦涩一些，容易出错。



下面正式进入Guice的使用介绍。

先贴一张UML图，展示我的demo程序的类结构。

![](https://images0.cnblogs.com/blog/103496/201501/182001165585730.jpg)

### 用Module绑定接口和实现，从Injector中获取实现类

```
public interface Energy {
    public String getName();
}
```

```
public class Fuel implements Energy{
    @Override
    public String getName() {
        return "Fule";
    }
}
```

```
@ImplementedBy(Horse.class)
public interface Vehicle {
    public String run();
}
```

```
@Singleton
public class Horse implements Vehicle{
    
    public static int instanceNum=0;
    @Inject
    Energy energy;
    
    public Horse(){
        instanceNum++;
    }

    @Override
    public String run() {
        return "Horse Run with " + energy.getName();
    }

}
```

```
public interface Hero {
    public String fight();
}
```

```
public class Soldier implements Hero {
    Vehicle vehicle;
　　
    public Soldier(Vehicle vehicle) {
        this.vehicle = vehicle;
    }
    @Override
    public String fight() {
        return vehicle.run() + " charge";
    }
}
```

```
1 public class MyModule implements Module {
 2     @Override
 3     public void configure(Binder binder) {
 4         binder.bind(Vehicle.class).annotatedWith(Fast.class).to(Copter.class).in(Scopes.SINGLETON);
 5         binder.bind(Vehicle.class).annotatedWith(Names.named("Speed")).to(AirShip.class);
 6         binder.requestStaticInjection(AirShip.class); 
 7         
 8         binder.bind(Energy.class).to(Fuel.class);
 9         binder.bind(Energy.class).annotatedWith(LightEnergy.class).to(Gas.class);
10         binder.bind(Energy.class).annotatedWith(Names.named("CleanEnergy")).to(Solar.class);
11         
12         binder.bind(Hero.class).toProvider(HeroProvider.class);
13         
14         binder.bind(String.class).annotatedWith(Names.named("point1")).toInstance("0.4");
15         InputStream stream = MyModule.class.getResourceAsStream("app.properties");
16         Properties appProperties = new Properties();
17         try {
18             appProperties.load(stream);
19             Names.bindProperties(binder, appProperties);
20         } catch (IOException e) {
21             binder.addError(e);
22         }
23     }
24 }
```

```
public class TestSoldier {

    @Test
    public void test() {
        Injector injector = Guice.createInjector(new MyModule());
        Vehicle vehicle = injector.getInstance(Vehicle.class);

        for (int i = 0; i < 3; i++) {
            Soldier hero = new Soldier(vehicle);
            Assert.assertEquals("Horse Run with Fule charge", hero.fight());
            Assert.assertEquals("inject.Soldier",hero.getClass().getCanonicalName());
        }

        Assert.assertTrue(1 == Horse.instanceNum);
    }
}
```

贴了一堆代码，现在开始解释。

在Module中指定了接口和实现的绑定方式，在创建Injector时需要把一个Module传进去。当我们想获取一个Vehicle的实例时，直接从Injector中取就可以了。在接口Vehicle上有一个注解@ImplementedBy(Horse.class)，它告诉Injector:Vehicle的默认实现是Horse。而Horse又依赖Energy，应该使用Energy的哪种实现呢？我们注意到Horse的energy成员上有一个@Inject注解，这种情况下Energy的实现类从Injector中获取，Injector会去询问Module，在MyModule的第8行指明了Energy的默认实现类是Fuel。

这里展示了绑定接口和实现的两种方法：在接口上使用@ImplementedBy；在Module中写binder.bind(interface).to(implementation)。一个接口的默认实现不能有两个，即如果在代码中出现：

```
binder.bind(Energy.class).to(Fuel.class);
binder.bind(Energy.class).to(Gas.class);
```

运行时会抛异常。

Horse类上面有个@Singleton注解，这告诉Injector只创建Horse的一个实例，任何人从Injector是获得的Horse都是同一个实体，即它们的hashCode都是相同的。我们故意设置了一个静态变量instanceNum来记录Horse的默认构造函数被调用的次数，试验证明该构造函数只被调用了1次。@Singleton注解这种方式跟在bind中使用in(Scopes.SINGLETON)效果是一样的，参见MyModule的第4行。

从Injector中创建的实例是正常的实例，没有做代理和转换。如果是代理的话输出实例的CanonicalName时将是：com.sun.proxy.$Proxy0

MyModule中还有好多语法下文会逐一解释。

```
public class Solar implements Energy {
    @Override
    public String getName() {
        return "Solar";
    }
}
```

```
public class AirShip implements Vehicle{
    @Inject
    @Named("CleanEnergy")
    static Energy energy;

    @Override
    public String run() {
        return "AirShip Fly with " + energy.getName();
    }
}
```

```
public class SuperHero implements Hero {
    @Inject
    @Named("Speed")
    Vehicle vehicle;

    @Override
    public String fight() {
        return vehicle.run() + " boom";
    }
}
```

```
public class TestSuperHero {
    @Test
    public void test(){
        Injector injector = Guice.createInjector(new MyModule());
        SuperHero hero = injector.getInstance(SuperHero.class);
        Assert.assertEquals("AirShip Fly with Solar boom", hero.fight());
    }
}
```

Injector.getInstance()方法的参数不仅可以是接口，也可以是一个具体的实现类(此时不需要在Module中指明绑定)。从Injector中获取SuperHero的实例时，SuperHero中的@Inject属性也会要求从Injector中获得实例。那么SuperHero使用的是哪一个Vehicle呢？是Horse吗？(因为Horse是Vehicle的默认实现)这里使用了@Named注解，参考MyModule的第5行我们知道SuperHero使用的Vehicle实际上是AirShip。同理类推，AirShip中使用的Energy是Solar，参见MyModule的第10行。

注意AirShip的energy属性是静态变量，所以在MyModule中必须写明binder.requestStaticInjection(AirShip.class); 那从AirShip到Energy的依赖链才可以传递下去。

Module中不能自己绑定到自己，但可以绑定到子类。

binder.bind(Implementation).to(Implementation)　　错误

binder.bind(Implementation).to(SubImplementation)　　正确

```
public class Gas implements Energy {
    @Override
    public String getName() {
        return "Gas";
    }
}
```

```
public class Copter implements Vehicle {
    
    public static int instanceNum = 0;
    Energy energy;

    public Copter() {
        energy = new Gas();
    }

    @Inject
    private void init(@LightEnergy Energy energy) {
        this.energy = energy;
        instanceNum++;
    }

    @Override
    public String run() {
        return "Copter Fly with " + energy.getName();
    }

}
```

```
public class WeaselGirl implements Hero {
    @Inject
    @Fast
    Vehicle vehicle;

    @Override
    public String fight() {
        return vehicle.run() + " shoot";
    }
}
```

```
public class TestWeaselGirl {

    @Test
    public void test() {
        Injector injector = Guice.createInjector(new MyModule());

        for (int i = 0; i < 3; i++) {
            WeaselGirl hero = injector.getInstance(WeaselGirl.class);
            Assert.assertEquals("Copter Fly with Gas shoot", hero.fight());
        }

        Assert.assertTrue(1 == Copter.instanceNum);
    }
}
```

WeaselGirl中依赖的Vehicle带@Fast注解，MyModule的第4行告诉我们此时Vehicle的实现类应该取Copter。Injectot在创建Copter实例时会优先去调用Copter的带@Inject注解的非私有构造函数，如果没有这种构造函数则去调用空构造函数，且调用完构造函数后Injector会立即去执行所有@Inject方法。Copter中有个@Inject方法叫init，该方法中的参数带@LightEnergy注解，MyModule的第9行告诉我们应该创建一个Gas实例传给init()方法。由于MyModule的第4行告诉 Injector:Copter是单例的，所以在test中尽管我们从Injector中获取了3次WeaselGirl(每次创建WeaselGirl都会去请求一个Copter)，但实际上Injetor只创建了一个Copter实例。

 至此我们已经学习了绑定接口和实现的3种方式：@ImplementedBy默认绑定；binder时使用annotatedWith(Annotation)实现条件绑定；binder时使用annotatedWith(Names.named("str"))实现条件绑定。其实我们也可以自己实现一个Provider，当一个类中有依赖项时（依赖项是一个接口），由Provider来提供具体的实现类。但是Injector的那些特性（比如@Singleton等）Provider都不具备，Provider只是一个简单的工厂模式。

```
public class Mobile implements Vehicle {
    Energy energy;

    public Mobile() {
        energy = new Gas();
    }

    @Inject
    public Mobile(Energy energy) {
        this.energy = energy;
        
    }

    @Override
    public String run() {
        return "Mobile Run with " + energy.getName();
    }
}
```

```
public class VehicleProvider implements Provider<Vehicle> {
    double point1;
    double point2;

    @Inject
    public VehicleProvider(@Named("point1") String p1, @Named("point2") String p2) {
        this.point1 = Double.parseDouble(p1);
        this.point2 = Double.parseDouble(p2);
    }

    @Override
    public Vehicle get() {
        Injector injector = Guice.createInjector(new MyModule());
        double rnd = Math.random();
        if (rnd < point1) {
            return injector.getInstance(Mobile.class);
        } else if (rnd < point2) {
            return injector.getInstance(AirShip.class);
        } else {
            return injector.getInstance(Mobile.class);
        }
    }
}
```

```
public class FrogMan implements Hero {
    Vehicle vehicle;

    public FrogMan(Provider<Vehicle> provider) {
        this.vehicle = provider.get();
    }

    @Override
    public String fight() {
        return vehicle.run()+" hack";
    }

}
```

```
public class TestFrogMan {
    @Test
    public void test(){
        Injector injector = Guice.createInjector(new MyModule());
        Provider<Vehicle> provider = injector.getInstance(VehicleProvider.class);
        for (int i = 0; i < 20; i++) {
            FrogMan hero = new FrogMan(provider);
            System.out.println(hero.fight());
        }
        System.out.println(Copter.instanceNum);
    }
}
```

FrogMan的构造函数中需要一个Provider，我们传进去的是一个VehicleProvider。VehicleProvider的get方法中随机返回Mobile和AirShip两种实体，这里的随机算法又依赖两个参数point1和point2。由于VehicleProvider是从Injector中获取的，所以Injector在创建VehicleProvider实例时会去调用VehicleProvider的@Inject构造函数。@Inject构造函数中用到的参数也全部由Injector来提供。MyModule的第14行告诉我们point1等于0.4，这是一个将常量绑定到PrimitiveType的例子。其实还可以借助于外部的配置文件将常量绑定到一个String变量，比如MyModule的第15行到第22行就是从一个peoperties文件中读取配置将常量值绑定到String变量，我们的point2就是通过这种方式赋值的。

```
public class HeroProvider implements Provider<Hero> {

    @Override
    public Hero get() {
        Injector injector = Guice.createInjector(new MyModule());
        Provider<Vehicle> provider = injector.getInstance(VehicleProvider.class);
        Hero hero = null;
        double d = Math.random();
        if (d < 0.3) {
            hero = new FrogMan(provider);
        } else {
            hero = injector.getInstance(SuperHero.class);
        }
        return hero;
    }
}
```

```
public class TestHeroProvider {

    @Test
    public void test(){
        Injector injector = Guice.createInjector(new MyModule());

        for (int i = 0; i < 20; i++) {
            Hero hero = injector.getInstance(Hero.class);
            System.out.println(hero.fight());
        }

        System.out.println(Copter.instanceNum);
    }
}
```

MyModule的第12行指明了当向Injector请求Hero时，由HeroProvider决定产生哪个具体的Hero。

[下载本文的所有代码](http://files.cnblogs.com/zhangchaoyang/inject.zip)

maven项目中引入依赖：

 <dependency>
			　　<groupId>com.google.inject</groupId>
			　　<artifactId>guice</artifactId>
			　　<version>3.0</version>
		</dependency>












