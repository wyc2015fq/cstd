# JavaBean之Builder模式 - z69183787的专栏 - CSDN博客
2019年01月07日 16:08:31[OkidoGreen](https://me.csdn.net/z69183787)阅读数：56
个人分类：[工具-Lombok																[设计模式](https://blog.csdn.net/z69183787/article/category/2176787)](https://blog.csdn.net/z69183787/article/category/5834225)
[https://blog.csdn.net/kangkanglou/article/details/78724240](https://blog.csdn.net/kangkanglou/article/details/78724240)
静态工厂和构造器
重载构造函数，分别包含多个不同的参数
无法很好地扩展到大量的可选参数
JavaBean方式
调用一个无参构造函数来创建对象，然后通过setter方法来设置必要参数
对象构造过程被分割到了几个调用，构造过程中可能导致JavaBean处于不一致的状态，类无法通过检验构造器参数的有效性来保证一致性
Builder模式
不直接生成想要的对象，通过调用构造器（或静态工厂），得到builder对象，客户端在builder对象上调用类似setter方法，来设置每个相关的可选参数，最后，再调用build方法来生成不可变对象
```java
public class Person {
    @Override
    public String toString() {
        return "Person{" +
                "name='" + name + '\'' +
                ", address='" + address + '\'' +
                ", firstName='" + firstName + '\'' +
                ", lastName='" + lastName + '\'' +
                '}';
    }
    private String name;
    private String address;
    private String firstName;
    private String lastName;
    private Person(String name, String address, String firstName, String lastName) {
        this.address = address;
        this.name = name;
        this.lastName = lastName;
        this.firstName = firstName;
    }
    public static Builder builder() {
        return new Builder();
    }
    public static class Builder {
        private String name;
        private String address;
        private String firstName;
        private String lastName;
        public Builder name(final String name) {
            this.name = name;
            return this;
        }
        public Builder address(final String address) {
            this.address = address;
            return this;
        }
        public Builder lastName(final String lastName) {
            this.lastName = lastName;
            return this;
        }
        public Builder firstName(final String firstName) {
            this.firstName = firstName;
            return this;
        }
        public Person build() {
            return new Person(name, address, firstName, lastName);
        }
    }
}
```
客户端调用
```java
Person p = new Person.Builder().address("上海").firstName("三").lastName("张").name("欧阳").build();
        System.out.println(p);
        Person person = Person.builder().address("上海").firstName("三").lastName("张").name("欧阳").build();
        System.out.println(person);
```
如果我们使用IDE，我们可以直接使用Lombok插件的@Builder注解，仅需声明类属性
```java
@Builder
@ToString
public class LombokPerson {
    private String name;
    private String address;
    private String firstName;
    private String lastName;
}
```
查看生成的class源码
```java
public class LombokPerson {
    private String name;
    private String address;
    private String firstName;
    private String lastName;
    @ConstructorProperties({"name", "address", "firstName", "lastName"})
    LombokPerson(String name, String address, String firstName, String lastName) {
        this.name = name;
        this.address = address;
        this.firstName = firstName;
        this.lastName = lastName;
    }
    public static LombokPerson.LombokPersonBuilder builder() {
        return new LombokPerson.LombokPersonBuilder();
    }
    public String toString() {
        return "LombokPerson(name=" + this.name + ", address=" + this.address + ", firstName=" + this.firstName + ", lastName=" + this.lastName + ")";
    }
    public static class LombokPersonBuilder {
        private String name;
        private String address;
        private String firstName;
        private String lastName;
        LombokPersonBuilder() {
        }
        public LombokPerson.LombokPersonBuilder name(String name) {
            this.name = name;
            return this;
        }
        public LombokPerson.LombokPersonBuilder address(String address) {
            this.address = address;
            return this;
        }
        public LombokPerson.LombokPersonBuilder firstName(String firstName) {
            this.firstName = firstName;
            return this;
        }
        public LombokPerson.LombokPersonBuilder lastName(String lastName) {
            this.lastName = lastName;
            return this;
        }
        public LombokPerson build() {
            return new LombokPerson(this.name, this.address, this.firstName, this.lastName);
        }
        public String toString() {
            return "LombokPerson.LombokPersonBuilder(name=" + this.name + ", address=" + this.address + ", firstName=" + this.firstName + ", lastName=" + this.lastName + ")";
        }
    }
}
```
客户端调用：
```java
LombokPerson lombokPerson = LombokPerson.builder().address("上海").firstName("三").lastName("张").name("欧阳").build();
        System.out.println(lombokPerson);
```
