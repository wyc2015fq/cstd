# Spring @cacheable注解实现的缓存 - z69183787的专栏 - CSDN博客
2019年01月04日 19:17:42[OkidoGreen](https://me.csdn.net/z69183787)阅读数：93
[https://blog.csdn.net/u010251278/article/details/76229085](https://blog.csdn.net/u010251278/article/details/76229085)
在软件开发中使用缓存已经有一个非常久的历史了。缓存是一种很好的设计思想，一旦你用了他，你将会发现他确实很有用。Spring3.1版本的核心对缓存做了实现。在Java推出Annotation特性之前，实现缓存的一个难点在于它与业务逻辑代码的耦合性太强。然而，Spring中使用@Cacheable 和@CacheEvict实现缓存在某种程度上解决了这个问题，基本思想是在方法加上@Cacheable注解，这个方法的返回值将具有缓存特性。
@Cacheable注解可以用在方法或者类级别。当他应用于方法级别的时候，就是如上所说的缓存返回值了。当应用在类级别的时候，这个类的所有方法的返回值都将被缓存。
```java
@Cacheable(value = "employee")
public class EmployeeDAO {
 
  public Person findEmployee(String firstName, String surname, int age) {
 
    return new Person(firstName, surname, age);
  }
 
  public Person findAnotherEmployee(String firstName, String surname, int age) {
 
    return new Person(firstName, surname, age);
  }
}
```
@Cacheable注解有三个参数，value是必须的，还有key和condition。第一个参数，也就是value指明了缓存将被存到什么地方。
```java
@Cacheable(value = "employee")
 public Person findEmployee(String firstName, String surname, int age) {
 
   return new Person(firstName, surname, age);
 }
```
上面的代码保证findEmployee的返回值Person对象将被存储在"employee"中。
任何存储在缓存中的数据为了高速访问都需要一个key。Spring默认使用被@Cacheable注解的方法的签名来作为key，当然你可以重写key，自定义key可以使用SpEL表达式。
```java
@Cacheable(value = "employee", key = "#surname")   public Person findEmployeeBySurname(String firstName, String surname, int age) {
     
    return new Person(firstName, surname, age);
}
```
在findEmployeeBySurname()的注解中"#surname"是一个SpEL表达式，他将使用findEmployeeBySurname()方法中的surname参数作为key。
@Cacheable的最后一个参数是condition（可选），同样的，也是引用一个SpEL表达式。但是这个参数将指明方法的返回结果是否被缓存。
```java
@Cacheable(value = "employee", condition = "#age < 25")
 public Person findEmployeeByAge(String firstName, String surname, int age) {
 
   return new Person(firstName, surname, age);
 }
```
上面的例子中，只有年龄小于25的时候才被缓存。
在快速看完了如何使用缓存后，我们接下来看看缓存带来的效果。
```java
@Test
  public void testCache() {
 
    Person employee1 = instance.findEmployee("John", "Smith", 33);
    Person employee2 = instance.findEmployee("John", "Smith", 33);
 
    assertEquals(employee1, employee2);
  }
```
上面的例子很简单，第一次调用findEmployee，findEmployee方法将被执行，Spring将他的返回值一个person对象存入缓存。第二次调用findEmployee的时候findEmployee将不被执行，Spring直接将缓存中的数据作为返回值返回。所以employee1 和employee2引用了同样的对象。
而下面的例子中，我们将年龄小于25作为缓存条件，就将得到不同的结果。
```java
@Test
 public void testCacheWithAgeAsCondition() {
 
   Person employee1 = instance.findEmployeeByAge("John", "Smith", 33);
   Person employee2 = instance.findEmployeeByAge("John", "Smith", 33);
 
   assertEquals(employee1, employee2);
 }
```
下面的例子我们在findEmployeeBySurname的方法的注解中自定义了key，我们使用了自定义的key生成方式，以确保不同的surname将会指向不同的人。看下面的程序
```java
@Test
 public void testCacheOnSurnameAsKey() {
 
   Person employee1 = instance.findEmployeeBySurname("John", "Smith", 22);
   Person employee2 = instance.findEmployeeBySurname("Jack", "Smith", 55);
 
   assertEquals(employee1, employee2);
 }
```
我们想找到两个不同的人，但是两个人的surname是相同的，你将发现两次调用返回了相同的结果，这不是Spring的问题，而是我们的cache key的生成方式有问题。所以在我们定义key的时候要小心注意key的生成策略，避免造成这种问题。
最后总结一下流程，当执行到一个被@Cacheable注解的方法时，Spring首先检查condition条件是否满足，如果不满足，执行方法，返回；如果满足，在value所命名的缓存空间中查找使用key存储的对象，如果找到，将找到的结果返回，如果没有找到执行方法，将方法的返回值以key-对象的方式存入value缓存中，然后方法返回。这里只是介绍了简单的spring @cacheable注解的用法，更详细的使用可以参考下面的参考资料。
