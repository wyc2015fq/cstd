# Java设计模式（观察者模式） - DEVELOPER - CSDN博客





2017年04月12日 15:41:25[学术袁](https://me.csdn.net/u012827205)阅读数：329标签：[Java设计模式](https://so.csdn.net/so/search/s.do?q=Java设计模式&t=blog)
个人分类：[java设计模式](https://blog.csdn.net/u012827205/article/category/2345605)

所属专栏：[JAVA设计模式](https://blog.csdn.net/column/details/15234.html)









hello world! 大家好！

说到观察者模式，什么是观察者模式？一种一对多的依赖关系,让多个观察者对象同时监听某一主题对象,在它的状态发生变化时,会通知所有的观察者.

那观察者模式的优点优势是什麽？？观察者与被观察者之间是属于轻度的关联关系，并且是抽象耦合的，这样，对于两者来说都比较容易进行扩展。

观察者模式是一种常用的触发机制，它形成一条触发链，依次对各个观察者的方法进行处理。

然后，我就讲一下我的观察者模式实现：

首先是这样一个故事，一个偷鸡贼（哦，不！不能这么称呼人家）正在偷别人家的鸡或者狗。这时候正是被警察A和警察B看到，并在最后的角逐中小偷不行被逮！

其中，小偷（被观察者）、警察AB（观察者）；当小偷（被观察者）做出某些侮辱妇女行为，错了，是偷！～的行为，触发了警察AB（观察者）的底线并对小偷进行红色全球通缉。

接口 小偷（被观察者）：

```java
public interface IThief {
    //注册观察者（类似注册监听）
    void registerPolice(IPolice police);
    //移去观察者（类似移走监听）
    void removePolice(WeakReference<IPolice> police);
    //notify通知报警方法
    void notifyPolice();

    //偷鸡
    void stealChicken();

    //摸狗
    void stealDog();
}
```
实现类 小偷（被观察者）：
```java
public class DogThief implements IThief{
    public List<WeakReference<IPolice>> polices = new ArrayList<>();
    @Override
    public void registerPolice(IPolice police) {
        //第一步：必须先要进行监听的注册
        polices.add(new WeakReference<IPolice>(police));
    }

    @Override
    public void removePolice(WeakReference<IPolice> police) {
        int index = polices.indexOf(police);
        if(index >= 0){
            polices.remove(index);
        }
    }

    @Override
    public void notifyPolice() {
        for (int i = 0; i < polices.size(); i++){
            WeakReference<IPolice> weakPolice = polices.get(i);
            IPolice police = weakPolice.get();
            police.runAfterThief();//警察听到警报，追赶小偷
        }
    }


    @Override
    public void stealChicken() {
        System.out.println("警报：有小偷偷鸡了！！！");
        notifyPolice();
    }

    @Override
    public void stealDog() {
        System.out.println("警报：有小偷，摸狗了！！！");
        notifyPolice();
    }
}
```




接口 警察（观察者）：

```java
//接口 警察
public interface IPolice {
    //追赶小偷
    void runAfterThief();

    //擒住小偷
    void catchThief();
}
```




对于小偷的行为进行处理的接口类：



```java
public interface IJudgement {
    //对小偷的罪行进行判决
    void deadBShoot();//枪毙
}
```


```java
public interface ILower {
    //作为律师对偷鸡摸狗贼 辩护
    void lowerPlead();
}
```


实现类 警察A（观察者）：
```java
public class PoliceA implements IPolice, IJudgement {
    @Override
    public void runAfterThief() {
        System.out.println("呼哧呼哧。。。追赶小偷！！");

        catchThief();
    }

    @Override
    public void catchThief() {
        System.out.println("尼玛。。。逮着你了吧？！！");

        deadBShoot();//判决，拉出去枪毙
    }

    @Override
    public void deadBShoot() {
        //警察对该贼进行监视，最终对小偷的偷窃行为做出来自己的合理操作A
        System.out.println("哦！死了。。。");
    }
}
```
实现类 警察B（观察者）：
```java
public class PoliceB implements IPolice, ILower{
    @Override
    public void lowerPlead() {
        //警察B对该贼进行监视，最终对小偷的偷窃行为做出来自己的合理操作
        System.out.println("我家猫贼，没偷鸡！！");
    }

    @Override
    public void runAfterThief() {
        System.out.println("呼哧呼哧。。。追赶小偷！！");
        catchThief();
    }

    @Override
    public void catchThief() {
        System.out.println("尼玛。。。逮着你了吧？！！");
        lowerPlead();
    }
}
```


小贼偷鸡摸狗，被警察发现并逮捕。然后俩警察分别对小贼做了相关处理！
```java
public class MainText {
    public static void main(String[] args){
        PoliceA policeA = new PoliceA();
        PoliceB policeB = new PoliceB();

        DogThief dogThief = new DogThief();
        dogThief.registerPolice(policeA);
        dogThief.registerPolice(policeB);

        dogThief.stealChicken();//偷鸡
        dogThief.stealDog();//摸狗
    }
}
```




还有一种写法是使用java自带的接口和抽象类：抽象类（被观察者：Observable）、观察者（接口：Observer）

```java
public class MainText {
    public static void main(String[] args){
        PoliceA policeA = new PoliceA();
        PoliceB policeB = new PoliceB();

        DogThief dogThief = new DogThief();
        dogThief.register(policeA);//注册A
        dogThief.register(policeB);//注册B
        dogThief.stealChickerDogs();// 开始偷鸡摸狗，触发警察AB的行为
    }
}
```

```java
//被观察者（贼）
public class DogThief extends Observable {

    public void register(Observer observer){
        this.addObserver(observer);
    }
    public void stealChickerDogs(){
        System.out.println("偷鸡摸狗");
        this.notifyObservers();
    }
}
```

```java
//观察者（警察A）
public class PoliceA implements Observer {
    @Override
    public void update(Observable o, Object arg) {
        System.out.println("直接追上去逮住小偷");
    }
}
```

```java
//观察者（警察B）
public class PoliceB  implements Observer {
    @Override
    public void update(Observable o, Object arg) {
        System.out.println("打电话请求支援！");
    }
}
```


先将 Observer 注册到 Observable ，那么当 Observable 状态改变时会通知它持有的所有 Observer ,对了，最好 Observable 中的 mList 的泛型是 WeakReference ,防止内存泄漏；看图再理解一下：![](https://img-blog.csdn.net/20170412154024218)







