# java web中entity的实现规范 - xqhrs232的专栏 - CSDN博客
2018年10月30日 11:21:14[xqhrs232](https://me.csdn.net/xqhrs232)阅读数：61
原文地址::[https://blog.csdn.net/kevin_love_it/article/details/58090495](https://blog.csdn.net/kevin_love_it/article/details/58090495)
相关文章
1、[javaWeb开发中entityBean的习惯用法](https://www.cnblogs.com/susuguo/p/5078273.html)----[https://www.cnblogs.com/susuguo/p/5078273.html](https://www.cnblogs.com/susuguo/p/5078273.html)
在日常的Java项目开发中，entity（实体类）是必不可少的，它们一般都有很多的属性，并有相应的setter和getter方法。entity（实体类）的作用一般是和数据表做映射。所以快速写出规范的entity（实体类）是java开发中一项必不可少的技能。
在项目中写实体类一般遵循下面的规范：
1、根据你的设计，定义一组你需要的私有属性。
2、根据这些属性，创建它们的setter和getter方法。（eclipse等集成开发软件可以自动生成。具体怎么生成？请自行百度。）
3、提供带参数的构造器和无参数的构造器。
4、重写父类中的eauals()方法和hashcode()方法。（如果需要涉及到两个对象之间的比较，这两个功能很重要。）
5、实现序列化并赋予其一个版本号。
下面是我写的一个实体类（entity）例子：具体的细节都用注释标注了。
- 
`1 class Student implements Serializable{`
- 
`2 /**`
- 
`3 * 版本号`
- 
`4 */`
- 
`5 private static final long serialVersionUID = 1L;`
- 
`6 //定义的私有属性`
- 
`7 private int id;`
- 
`8 private String name;`
- 
`9 private int age;`
- 
`10 private double score;`
- 
`11 //无参数的构造器`
- 
`12 public Student(){`
- 
`13 `
- 
`14 }`
- 
`15 //有参数的构造器`
- 
`16 public Student(int id,String name,int age, double score){`
- 
`17 this.id = id;`
- 
`18 this.name = name;`
- 
`19 this.age = age;`
- 
`20 this.score = score;`
- 
`21 }`
- 
`22 //创建的setter和getter方法`
- 
`23 public int getId() {`
- 
`24 return id;`
- 
`25 }`
- 
`26 public void setId(int id) {`
- 
`27 this.id = id;`
- 
`28 }`
- 
`29 public String getName() {`
- 
`30 return name;`
- 
`31 }`
- 
`32 public void setName(String name) {`
- 
`33 this.name = name;`
- 
`34 }`
- 
`35 public int getAge() {`
- 
`36 return age;`
- 
`37 }`
- 
`38 public void setAge(int age) {`
- 
`39 this.age = age;`
- 
`40 }`
- 
`41 public double getScore() {`
- 
`42 return score;`
- 
`43 }`
- 
`44 public void setScore(double score) {`
- 
`45 this.score = score;`
- 
`46 }`
- 
`47 //由于id对于学生这个类是唯一可以标识的，所以重写了父类中的id的hashCode()和equals()方法。`
- 
`48 @Override`
- 
`49 public int hashCode() {`
- 
`50 final int prime = 31;`
- 
`51 int result = 1;`
- 
`52 result = prime * result + id;`
- 
`53 return result;`
- 
`54 }`
- 
`55 @Override`
- 
`56 public boolean equals(Object obj) {`
- 
`57 if (this == obj)`
- 
`58 return true;`
- 
`59 if (obj == null)`
- 
`60 return false;`
- 
`61 if (getClass() != obj.getClass())`
- 
`62 return false;`
- 
`63 Student other = (Student) obj;`
- 
`64 if (id != other.id)`
- 
`65 return false;`
- 
`66 return true;`
- 
`67 }`
- 
`68 `
- 
`69 }`
一个学生的Java实体类就基本完成了。
