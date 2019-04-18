# java 四种内部类和内部接口 - weixin_33985507的博客 - CSDN博客
2016年11月27日 23:25:00[weixin_33985507](https://me.csdn.net/weixin_33985507)阅读数：5

```java
/**
 * 常规内部类：常规内部类没有用static修饰且定义在在外部类类体中。
 * 1.常规内部类中的方法可以直接使用外部类的实例变量和实例方法。
 * 2.在常规内部类中可以直接用内部类创建对象
 */
public class MyOuter {
	private int x = 100;
	
	class MyInner{
		private String y="Hello!";
		public void innerMethod(){
			System.out.println("内部类中 String ="+y);
			System.out.println("外部类中的x ="+x);
			outerMethod();
			System.out.println("x is "+MyOuter.this.x);
		}
	}
	
	public void outerMethod(){
		x++;
	}
	
	public void makeInner(){
		//在外部类方法中创建内部类实例
		MyInner in = new MyInner();
	}
	
	public static void main(String[] args){
		MyOuter mo = new MyOuter();
		//使用外部类构造方法创建 mo 常规内部类需要通过外部类实例来new
		MyOuter.MyInner inner = mo.new MyInner();
		inner.innerMethod();
	}
}
/**
 * 静态内部类：与类的其他成员相似，可以用static修饰内部类，
 * 这样的类称为静态内部类。静态内部类与静态内部方法相似，
 * 只能访问外部类的static成员，
 * 不能直接访问外部类的实例变量，与实例方法，只有通过对象引用才能访问。
 */
public class MyOuter2 {
	
	public static int x=100;
	
	public static class Myinner{
		private String y="Hello!";
		/**
		 * 由于static内部类不具有任何对外部类实例的引用，
		 * 因此static内部类中不能使用this关键字来访问外部类中的实例成员，
		 * 但是可以访问外部类中的static成员。这与一般类的static方法相通
		 */
		public void innerMethod(){
			System.out.println("x="+x);
			System.out.println("y="+y);
		}
	}
	
	public static void main(String[] args){
		MyOuter2.Myinner si = new MyOuter2.Myinner();
		si.innerMethod();
	}
	
	
}
/**
 * 局部内部类：在方法体或语句块（包括方法、构造方法、局部块或静态初始化块）内部定义的类成为局部内部类。
 * 局部内部类不能加任何访问修饰符，因为它只对局部块有效。
 * 1.局部内部类只在方法体中有效，就想定义的局部变量一样，在定义的方法体外不能创建局部内部类的对象
 * 2.在方法内部定义类时，应注意以下问题：
 *  1.方法定义局部内部类同方法定义局部变量一样，不能使用private、protected、public等访问修饰说明符修饰，
 *  也不能使用static修饰，但可以使用final和   abstract修饰
 *  2.方法中的内部类可以访问外部类成员。对于方法的参数和局部变量，必须有final修饰才可以访问。
 *  3.static方法中定义的内部类可以访问外部类定义的static成员
 *
 */
public class MyOuter3 {
	private int size=5,y=7;
	
	public Object makeInner(int localVar){
		final int finalLocalVar = localVar;
		
		//创建内部类，该类只在makeInner（）方法有效，就像局部变量一样。
		//在方法体外部不能创建MyInner类的对象
		class Myinner{
			int y=4;
			public String toString(){
				return "OuterSize:"+size+" localVar:"+finalLocalVar+"\nthis.y="+this.y;
			}
		}
		
		return new Myinner();
	}
	
	public static void main(String[] args){
		Object obj = new MyOuter3().makeInner(47);
		System.out.println(obj.toString());
	}
}
/*匿名内部类：定义类的最终目的是创建一个类的实例，但是如果某个类的实例只是用一次，则可以将类的定义与类的创建，放到与一起完成，或者说在定义类的同时就创建一个类
  以这种方法定义的没有名字的类成为匿名内部类。
   声明和构造匿名内部类的一般格式如下：
   new ClassOrInterfaceName(){
    类体 }
   1.匿名内部类可以继承一个类或实现一个接口，这里的ClassOrInterfaceName是匿名内部类所继承的类名或实现的接口名。但匿名内部类不能同时实现一个接口和继承一个类，
也不能实现多个接口。如果实现了一个接口，该类是Object类的直接子类，匿名类继承一个类或实现一个接口，不需要extends和implements关键字。
  
   2.由于匿名内部类没有名称，所以类体中不能定义构造方法，由于不知道类名也不能使用关键字来创建该类的实例。实际上匿名内部类的定义、构造、和第一次使用都发生在同样一个地方。
此外，上式是一个表达式，返回的是一个对象的引用，所以可以直接使用或将其复制给一个对象变量。例：
   TypeName obj=new Name（）{
   此处为类体
      }
  同样，也可以将构造的对象作为调用的参数。例：
   
   someMethod(new Name(){
   此处为类体  });*/
public class MyOuter4 {
	private int size=5;
	
	public Object makeInner(int localvar){
		final int finallocalvar = localvar;
		return new Object(){
			public String toString(){
				return "OuterSize="+size+"\nfinalLocalvar="+finallocalvar;
			}
		};
	}
	
	public static void main(String args[]){
		Object obj=new MyOuter4().makeInner(67);
		System.out.println(obj.toString());
	}
}
```
 5，**内部接口:**
为了弄清楚内部接口是如何工作的，我们可以拿它与内部类作比较。内部类可以被认为是一个外部类内部定义的一个常规方法。因为一个方法可以被声明为静态和非静态，类似的内部类也可以被声明为静态和非静态。静态类类似于静态方法，它只能访问外部类的静态成员属性。非静态方法可以访问外部类的所有成员属性。
- 一种对那些在同一个地方使用的接口进行逻辑上分组；
- 封装思想的体现；
- 
因为接口是不能实例化的，内部接口只有当它是静态的才有意义。因此，默认情况下，内部接口是静态的，不管你是否手动加了static关键字。
- 
```java
public class MyOuter4 {
	private int size=5;
	private int id;
	
	public interface OnClickListener{
		void onClick(int id);
	}
	
	public void onClick(OnClickListener obj){
		obj.onClick(this.id);
	}
	
	public Object makeInner(int localvar){
		final int finallocalvar = localvar;
		return new Object(){
			public String toString(){
				return "OuterSize="+size+"\nfinalLocalvar="+finallocalvar;
			}
		};
	}
	
	public static void main(String args[]){
		Object obj=new MyOuter4().makeInner(67);
		System.out.println(obj.toString());
		
		MyOuter4 m = new MyOuter4();
		m.onClick(new OnClickListener() {
			
			@Override
			public void onClick(int id) {
				System.out.println(id);
			}
		});
	}
}
```
