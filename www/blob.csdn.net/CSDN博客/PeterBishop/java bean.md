# java bean - PeterBishop - CSDN博客





2018年09月29日 14:40:23[PeterBishop0](https://me.csdn.net/qq_40061421)阅读数：20








### JavaBean是一种Java语言写成的可重用组件。

### 所谓javaBean，是指符合如下标准的Java类：

### 类是公共的

### 有一个无参的公共的构造器

### 有属性，且有对应的getter、setter方法

### 用户可以使用JavaBean将功能、处理、值、数据库访问和其他任何可以用java代码创建的对象进行打包，并且其他的开发者可以通过内部的JSP页面、Servlet、其他JavaBean、applet程序或者应用来使用这些对象。用户可以认为JavaBean提供了一种随时随地的复制和粘贴的功能，而不用关心任何改变。

### getter、setter方法:

###        idea和eclispe可以自动生成getter和setter方法

### setter方法:

###        public void setXxx(Type xxx){

###        this.xxx = xxx;

### }

###        getter方法:

###               public Type getXxx(){

###                      return this.xxx;

### }

### 一个java bean的小Demo

### public class User {    private int id;    private int age;    public User(){}    public User(int id, int age) {        this.id = id;        this.age = age;    }    public int getId() {        return id;    }    public void setId(int id) {        this.id = id;    }    public int getAge() {        return age;    }    public void setAge(int age) {        this.age = age;    }}

### 注意:

###        除非是继承或其他需要，Java类里的属性尽量用private修饰，然后用getter和setter方法来进行读写操作，这是一种良好的编码规范



