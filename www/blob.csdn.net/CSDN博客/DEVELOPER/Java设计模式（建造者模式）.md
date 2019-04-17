# Java设计模式（建造者模式） - DEVELOPER - CSDN博客





2017年04月17日 18:47:56[学术袁](https://me.csdn.net/u012827205)阅读数：269标签：[Java设计模式](https://so.csdn.net/so/search/s.do?q=Java设计模式&t=blog)
个人分类：[java设计模式](https://blog.csdn.net/u012827205/article/category/2345605)









由于移动端UI交互逻辑和设计上跟老板不同，最后怼起来了！所以现在心情很不好，那谁？！你就是公司老板是吧？”达康书记找你“。

现在咱就一起来了解一下Java设计模式的”建造者模式“；老规矩：什么是建造者模式？它有什么好处值得我们去使用？？

建造者模式是什麽？建造者模式是一种创建型设计模式，又名生成器模式。简单解释就是，将建造复杂对象的过程和组织对象部件的过程解耦。

建造者模式优点有多少？？（开闭原则：对扩展开放，对修改关闭）1，产品本身与产品的创建过程解耦，使得相同的创建过程可以创建不同的产品对象。2，系统扩展方便，符合“开闭原则”。每一个具体建造者都相对独立，而与其他的具体建造者无关，因此可以很方便地替换具体建造者或增加新的具体建造者，用户使用不同的具体建造者即可得到不同的产品对象。3，可以更加精细地控制产品的创建过程。将复杂产品的创建步骤分解在不同的方法中，使得创建过程更加清晰，也更方便使用程序来控制创建过程。

我们通过一件事情来解释一哈！假如，这时候公司需要做一款类似JD的购物软件。这个软件呢，它必须具备几个功能（1，有后台提供数据源展现再屏幕上；2，用户可以进行浏览商品；3，用户喜欢了那一款商品可以进行收藏；4，用户可以进行支付购买）；只有写出具备了这样一些功能的应用，那才算完，老板才不会狠狠的吊你！



```java
//购物软件特征(假如：满足以下条件才能形成一个购物软件)
public class JDEntity {
    String functionBrower;//浏览
    String functionCollect;//收藏
    String functionPayMoney;//支付
    String functionDataOffer;//后台提供数据源

    public String getFunctionBrower() {
        return functionBrower;
    }

    public void setFunctionBrower(String functionBrower) {
        this.functionBrower = functionBrower;
    }

    public String getFunctionCollect() {
        return functionCollect;
    }

    public void setFunctionCollect(String functionCollect) {
        this.functionCollect = functionCollect;
    }

    public String getFunctionPayMoney() {
        return functionPayMoney;
    }

    public void setFunctionPayMoney(String functionPayMoney) {
        this.functionPayMoney = functionPayMoney;
    }

    public String getFunctionDataOffer() {
        return functionDataOffer;
    }

    public void setFunctionDataOffer(String functionDataOffer) {
        this.functionDataOffer = functionDataOffer;
    }
}
```

```java
//购物项目建造者接口
public interface IProjectBuilder {
    /**
     * 组成当前应用所需要的行为
     */

    //UI体验、功能交互
    void browser();//浏览
    void collection();//收藏
    void payMoney();//购买支付
    void backendData();//后台数据接口提供数据

    //获得 (行为组装)应用对象
    JDEntity getProjectInstance();
}
```

```java
public class JDBuilder implements IProjectBuilder {
    private JDEntity jdEntity;
    public JDBuilder(JDEntity jdEntity){
        this.jdEntity = jdEntity;
    }
    @Override
    public void browser() {
        System.out.println("实现功能：浏览商品~");
        jdEntity.setFunctionBrower("实现功能：浏览商品~");
    }

    @Override
    public void collection() {
        System.out.println("实现功能：收藏商品~");
        jdEntity.setFunctionCollect("实现功能：收藏商品~");
    }

    @Override
    public void payMoney() {
        System.out.println("实现功能：支付商品~");
        jdEntity.setFunctionPayMoney("实现功能：支付商品~");
    }

    @Override
    public void backendData() {
        System.out.println("实现功能：后台提供商品数据源~");
        jdEntity.setFunctionDataOffer("实现功能：后台提供商品数据源~");
    }

    @Override
    public JDEntity getProjectInstance() {
        return jdEntity;
    }
}
```

```java
//组织JD对象部件，才能形成一个真是可用的app
public class JDDirector {
    private IProjectBuilder jdBuilder;
    public JDDirector(IProjectBuilder jdBuilder){
        jdBuilder.backendData();
        jdBuilder.browser();
        jdBuilder.collection();
        jdBuilder.payMoney();
    }
}
```

```java
public class JDMain {
    public static void main(String[] args){
        JDEntity jdEntity = new JDEntity();
        JDBuilder jdBuilder = new JDBuilder(jdEntity);
        JDDirector jdDirector = new JDDirector(jdBuilder);

        //最终获取到 JD 应用包含操作功能的app
        jdEntity = jdBuilder.getProjectInstance();

    }
}
```






