# java反射--如何改变private中的变量及方法 - z69183787的专栏 - CSDN博客
2014年04月09日 18:44:23[OkidoGreen](https://me.csdn.net/z69183787)阅读数：8608
class DemoTest{
   private String name="123";
   public getName(){
       system.out.println("public getName " + name);
       return name;
   }
   private getName2(){
       system.out.println("private getName2 " + name);
      return name;
   }
}
如这个，要改变name的值。如何改变。通过java反射如何改变
先来看看如何用反射来调用getName方法
class<DemoTest> calzz=DemoTest.class;
Constructor cons=clazz.getConstructor(new class[]{});//这个是得到class对象的构造方法。参数为构造方法的参数的类的对象。
如：DemoTest有一个构造方法为public DemoTest(String arg0,String arg1){......}
这时，Constructor cons=clazz.getConstructor(new class[]{String.class,String.class});两个String.calss分别对应arg0,arg1.
接下来是：
DemoTest test=(DemoTest)cons.newInstance(new Object[]{});//生成相应的对象。参数new Object[]{} 就是构造方法对应的参数的具体值。如刚才的：
DemoTest test=(DemoTest)cons.newInstance(new Object[]{“李车”,"张三"});
接下来：
Method method=clazz.getMethod("getName",new Class[]{String.class});
method.invoke(test,new Object[]{"王五"});
**这时就可以打印出王五了。**
**那要如何把name为private 的值进行改变呢。**
1。先得到field，
Field field=clazz.getDeclaredField("name");
2.将此对象的 accessible 标志设置为指示的布尔值。值为 true 则指示反射的对象在使用时应该取消 Java 语言访问检查。值为 false 则指示反射的对象应该实施 Java 语言访问检查。
field.**setAccessible(true);**
3.对变量值进行修改
field.set("name","王五");
OK；
要访问私有的方法也是一样。
Method method=clazz.getDeclaredMethod("getName2");
method.setAccessible(true);
method.invoke(test,new Object[]{"王五"});
**用getDeclaredFields就能得到private 和public protend protected 的字段 如果用c.getFields();只能得到public类型的属性 **
