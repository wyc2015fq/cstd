# 单例模式：Singleton - A784586的专栏：任何事都存在 Plan B ！ - CSDN博客





2016年12月05日 11:02:40[QuJack](https://me.csdn.net/A784586)阅读数：262标签：[java](https://so.csdn.net/so/search/s.do?q=java&t=blog)
个人分类：[Java](https://blog.csdn.net/A784586/article/category/6556682)









java中的实现：

单例模式：Singleton

特殊的工厂方法模式，一个类只有一个实例：

                           1.拥有一个私有的静态实例；

                           2. 拥有私有的默认构造函数；

                           3.静态工厂方法，同步的，防止多线程同时操作；

                           4.重写cone()函数，返回当前实例对象，默认clone()创建新实例；

public class SingletonFactory{

           //1.私有的，防止外部变量引用；

private static SingletonFactory _instance=null;

            //2.私有的默认构造函数，防止使用构造函数实例化；

private
SingletonFactory(){ 

                     }

            //3.单例静态工厂方法，同步方式多线程同时执行；

synchronized
public static SingletonFactory getInstance(){

            if(_instance==null){

                    _instance=new SiongletonFactory();

                        }

                        retrun _instance;

            }

        //4.重写克隆函数

public SingletonFactory
clone(){

    return getInstance();

                        }

}

----------------------------------------------------------------------

一般有了上面的解法，算是一种通用的模板吧，再Java种完成如上四点就是能保证一个单利模式了，但是还是有优化的地方的。首先，加锁是浪费时间的，单例模式只是在还没有创建之前需要加锁操作，以保证只有一个线程创建实例，当实例已经存在了就不需要加锁了。因此可以这样优化，先判断 
 if(_instance==null)，等于空在加锁，不等于空直接返回。可以考虑用Java种的lock。







如果进一步优化，可以考虑C#中的静态构造函数，c#种静态构造函数保证只执行一次。还有一种利用私有嵌套内部类，的特性，实现按需创建。

————————————————————————————————————————————————————


-------------------------------------------------------------------------

Q：对比synchronized与java.util.concurrent.locks.Lock的异同。
A:主要相同点：Lock能完成synchronized所实现的所有功能
     主要不同点：Lock有比synchronized更精确的线程语义和更好的性能。synchronized会自动释放锁，而Lock一定要求程序员手工释放，并且必须在finally从句中释放。


---------------------------------------------------------------------------------














