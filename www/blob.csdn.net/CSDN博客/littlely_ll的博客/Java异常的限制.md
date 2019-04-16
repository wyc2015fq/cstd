# Java异常的限制 - littlely_ll的博客 - CSDN博客





2017年08月28日 10:01:38[littlely_ll](https://me.csdn.net/littlely_ll)阅读数：176








```java
class BaseballException extends Exception{}
class Foul extends BaseballException{}
class Strike extends BaseballException{}

abstract class Inning{
	public Inning() throws BaseballException{}
	public void event() throws BaseballException{}
	public abstract void atBat() throws Strike, Foul;
	public void walk(){}
}
class StormException extends Exception{}
class RainedOut extends StormException{}
class PopFoul extends Foul{}
interface Storm{
	public void event() throws RainedOut;
	public void rainHard() throws RainedOut;
}

public class StormyInning extends Inning implements Storm{
	public StormyInning() throws RainedOut, BaseballException{}
	public StormyInning(String s) throws Foul, BaseballException{}
	
	//! public void event() throws RainedOut{}//Interface CANNOT add exceptions to exsiting methods from the base calss
	public void rainHard() throws RainedOut{}//if the method does not already exist in the base class, the exception is OK
	public void event(){}//you can choose not to throw exceptions, even if the base version does
	public void atBat() throws PopFoul{}//overridden methods can throw inherited exception
	
	public static void main(String[] args){
		try{
			StormyInning si = new StormyInning();
			si.atBat();
		}catch(PopFoul e){
			System.out.println("Pop foul");
		}catch(RainedOut e){
			System.out.println("Rained out");
		}catch(BaseballException e){
			System.out.println("Generic baseball exception");
		}
		try{
			Inning i = new StormyInning();//upcast, what happens
			i.atBat();
			//you must catch the exceptions from the base-class version of the method:
		}catch(Strike e){
			System.out.println("Strike");
		}catch(Foul e){
			System.out.println("Foul");
		}catch(RainedOut e){
			System.out.println("Rained out");
		}catch(BaseballException e){
			System.out.println("Generic baseball exception");
		}
	}
}
```




1. 如果StormyInning类在扩展Inning类的同时又实现了Storm的接口，那么Storm里的event()方法就不能改变在Inning中的event()方法的异常接口。

2. 派生类构造器不能捕获基类构造器抛出的异常。

3. 异常说明本身不属于方法类型的一部分，方法类型是由方法的名字和参数的类型组成，因此，不能基于异常说明来重载方法。



