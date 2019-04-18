# Spring学习-- SpEL表达式 - z69183787的专栏 - CSDN博客
2019年03月25日 17:16:17[OkidoGreen](https://me.csdn.net/z69183787)阅读数：31
[https://www.cnblogs.com/goodcheap/p/6490896.html](https://www.cnblogs.com/goodcheap/p/6490896.html)
> 
**目录**
**Spring 表达式语言(简称SpEL)：**是一个支持运行时查询和操作对象图的强大的表达式语言。
**语法类似于 EL：**SpEL 使用 #{...} 作为定界符 , 所有在大括号中的字符都将被认为是 SpEL , SpEL 为 bean 的属性进行动态赋值提供了便利。
**通过 SpEL 可以实现：**
- 通过 bean 的 id 对 bean 进行引用。
- 调用方式以及引用对象中的属性。
- 计算表达式的值
- 正则表达式的匹配。
```
<?xml version="1.0" encoding="UTF-8"?>
<beans xmlns="http://www.springframework.org/schema/beans"
       xmlns:xsi="http://www.w3.org/2001/XMLSchema-instance"
       xsi:schemaLocation="http://www.springframework.org/schema/beans http://www.springframework.org/schema/beans/spring-beans.xsd">
    <bean id="car" class="com.itdjx.spring.spel.Car">
        <property name="brand" value="#{'玛莎拉蒂'}"></property>
        <property name="price" value="#{32000.78}"></property>
        <property name="perimeter" value="#{T(java.lang.Math).PI * 75.8f}"></property>
    </bean>
    <bean id="person" class="com.itdjx.spring.spel.Person">
        <property name='name' value='#{"华崽儿"}'></property>
        <property name="age" value="#{25}"></property>
        <property name="marriage" value="#{car.price > 400000 and age > 30}"></property>
        <property name="car" value="#{car}"></property>
        <property name="socialStatus" value="#{car.price > 30000 ? '金领' : '白领'}"></property>
        <property name="address" value="#{address.province + '省' + address.city + '市' + address.area + '区'}"/>
    </bean>
    <bean id="address" class="com.itdjx.spring.spel.Address">
        <property name="province" value="#{'辽宁'}"/>
        <property name="city" value="#{'大连'}"/>
        <property name="area" value="#{'沙河口'}"/>
    </bean>
</beans>
```
```java
package com.itdjx.spring.spel;
/**
 * @author Wáng Chéng Dá
 * @create 2017-03-02 13:26
 */
public class Car {
    private String brand;
    private double price;
    private String perimeter;
    public String getBrand() {
        return brand;
    }
    public void setBrand(String brand) {
        this.brand = brand;
    }
    public double getPrice() {
        return price;
    }
    public void setPrice(double price) {
        this.price = price;
    }
    public String getPerimeter() {
        return perimeter;
    }
    public void setPerimeter(String perimeter) {
        this.perimeter = perimeter;
    }
    @Override
    public String toString() {
        return "Car{" +
                "brand='" + brand + '\'' +
                ", price=" + price +
                ", perimeter='" + perimeter + '\'' +
                '}';
    }
}
```
```java
package com.itdjx.spring.spel;
/**
 * @author Wáng Chéng Dá
 * @create 2017-03-02 13:48
 */
public class Address {
    private String province;
    private String city;
    private String area;
    public String getProvince() {
        return province;
    }
    public void setProvince(String province) {
        this.province = province;
    }
    public String getCity() {
        return city;
    }
    public void setCity(String city) {
        this.city = city;
    }
    public String getArea() {
        return area;
    }
    public void setArea(String area) {
        this.area = area;
    }
    @Override
    public String toString() {
        return "Address{" +
                "province='" + province + '\'' +
                ", city='" + city + '\'' +
                ", area='" + area + '\'' +
                '}';
    }
}
```
```java
package com.itdjx.spring.spel;
/**
 * @author Wáng Chéng Dá
 * @create 2017-03-02 13:27
 */
public class Person {
    private String name;
    private int age;
    private boolean marriage;
    private Car car;
    private String socialStatus;
    private String address;
    public String getName() {
        return name;
    }
    public void setName(String name) {
        this.name = name;
    }
    public int getAge() {
        return age;
    }
    public void setAge(int age) {
        this.age = age;
    }
    public boolean isMarriage() {
        return marriage;
    }
    public void setMarriage(boolean marriage) {
        this.marriage = marriage;
    }
    public Car getCar() {
        return car;
    }
    public void setCar(Car car) {
        this.car = car;
    }
    public String getSocialStatus() {
        return socialStatus;
    }
    public void setSocialStatus(String socialStatus) {
        this.socialStatus = socialStatus;
    }
    public String getAddress() {
        return address;
    }
    public void setAddress(String address) {
        this.address = address;
    }
    @Override
    public String toString() {
        return "Person{" +
                "name='" + name + '\'' +
                ", age=" + age +
                ", marriage=" + marriage +
                ", car=" + car +
                ", socialStatus='" + socialStatus + '\'' +
                ", address='" + address + '\'' +
                '}';
    }
}
```
```java
package com.itdjx.spring.spel;
import org.springframework.context.ApplicationContext;
import org.springframework.context.support.ClassPathXmlApplicationContext;
/**
 * @author Wáng Chéng Dá
 * @create 2017-03-02 13:42
 */
public class Main {
    public static void main(String[] args) {
        ApplicationContext app = new ClassPathXmlApplicationContext("spel-beans.xml");
        Person person = (Person) app.getBean("person");
        System.out.println(person);
    }
}
```
**.控制台输出：**
Person{name='华崽儿', age=25, marriage=false, car=Car{brand='玛莎拉蒂', price=32000.78, perimeter='238.13273272948624'}, socialStatus='金领', address='辽宁省大连市沙河口区'}
** SpEL 字面量：**
- 整数：#{8}
- 小数：#{8.8}
- 科学计数法：#{1e4}
- String：可以使用单引号或者双引号作为字符串的定界符号。
- Boolean：#{true}
**SpEL引用bean , 属性和方法：**
- 引用其他对象:#{car}
- 引用其他对象的属性：#{car.brand}
- 调用其它方法 , 还可以链式操作：#{car.toString()}
- 调用静态方法静态属性：#{T(java.lang.Math).PI}
** SpEL支持的运算符号：**
- 算术运算符：+，-，*，/，%，^(加号还可以用作字符串连接)
- 比较运算符：< , > , == , >= , <= , lt , gt , eg , le , ge
- 逻辑运算符：and , or , not , |
- if-else 运算符(类似三目运算符)：？:(temary), ?:(Elvis)
- 正则表达式：#{admin.email matches '[a-zA-Z0-9._%+-]+@[a-zA-Z0-9.-]+\\.[a-zA-Z]{2,4}'}
