# java的内部类 - littlely_ll的博客 - CSDN博客





2017年08月23日 17:49:01[littlely_ll](https://me.csdn.net/littlely_ll)阅读数：186标签：[java																[内部类](https://so.csdn.net/so/search/s.do?q=内部类&t=blog)](https://so.csdn.net/so/search/s.do?q=java&t=blog)
个人分类：[java](https://blog.csdn.net/littlely_ll/article/category/7122947)







- 在使用内部类时，如果想从外部类除了静态方法之外的任意位置创建内部类对象，那么必须具体指明这个对象的类型：OuterClassName.InnerClassName。
- 内部类可以访问外部类的方法和字段。
- 要想直接创建内部类对象，必须使用外部类的对象来创建内部类对象。

```java
interface Dest{
    String readLine();//自动转换为public
}
interface Cont{
    public int value();
}
class P{
    private class PCont implements Cont{
        private int i = 11;
        public int value(){return i;}//重写接口中的声明
    }
    public class PDest implements Dest{
        private String label;
        private PDest(String whereTo){label = whereTo;}
        public String readLine(){return label;}//重写接口中的声明
    }
    public Dest dest(String s){
        return new PDest(s);//内部类向上转型为一个接口；
    }
    public Cont cont(){
        return new PCont();//上同；
    }
    public static void main(String[] args){
        P p = new P();
        Cont c = p.cont();
        Dest d = p.dest("Tasmania");
        d.readLine();
        P.PCont pc = p.new PCont();//使用外部类对象创建内部类对象，注意.new的使用；
        P.PDest pd = p.new PDest("tamade");//上同；
        pd.readLine();
    }
}
```

实现了某个接口的对象，得到对此接口的引用与向上转型为这个对象的基类效果一样。




