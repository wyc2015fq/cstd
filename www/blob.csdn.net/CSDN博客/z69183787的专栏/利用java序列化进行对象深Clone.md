# 利用java序列化进行对象深Clone - z69183787的专栏 - CSDN博客
2014年08月14日 20:17:09[OkidoGreen](https://me.csdn.net/z69183787)阅读数：1772
在java语言里深复制一个对象，通常可以使对象实现Serializable接口，然后把对象（实际上是对象的一个copy），写到一个流里面，便可重复建立对象。
这样做被复制的对象与被复制对象里面的引用都是可以被一并深复制的，不同于Object基类的Clone方法（浅复制：如果被复制的对象存在其他对象的引用，复制一个引用指向原对象的实例）。废话不多说Code如下：
**[java]**[view
 plain](http://blog.csdn.net/ningguixin/article/details/8169378#)[copy](http://blog.csdn.net/ningguixin/article/details/8169378#)
- import java.io.ByteArrayInputStream;  
- import java.io.ByteArrayOutputStream;  
- import java.io.IOException;  
- import java.io.ObjectInputStream;  
- import java.io.ObjectOutputStream;  
- import java.io.Serializable;  
- 
- 
- publicclass CloneTest {  
- publicstaticvoid main(String[] args) throws IOException, ClassNotFoundException {  
-         Teacher t = new Teacher();  
-         t.setName("Teacher xin");  
-         t.setAge(30);  
- 
-         Student s = new Student();  
-         s.setName("Student xiaoxin");  
-         s.setAge(20);  
-         s.setTeacher(t);  
- 
-         System.out.println("s name="+s.getName());  
-         System.out.println("s age="+s.getAge());  
-         System.out.println("s t name="+s.getTeacher().getName());  
-         System.out.println("s t age="+s.getTeacher().getAge());  
-         System.out.println("-------------------");  
- 
-         Student sCopy = (Student)s.deepCopy();  
-         sCopy.getTeacher().setName("Teacher mao");  
- //下面运行的结果看出对Student的引用Teacher类进行了深复制
-         System.out.println("sCopy t name="+sCopy.getTeacher().getName());  
-         System.out.println("-------------------");  
-         System.out.println("s t name="+s.getTeacher().getName());  
-     }  
- 
- }  
- 
- class Teacher implements Serializable{  
- 
- privatestaticfinallong serialVersionUID = -2380977769309339400L;  
- private String name;  
- privateint age;  
- publicint getAge() {  
- return age;  
-     }  
- publicvoid setAge(int age) {  
- this.age = age;  
-     }  
- public String getName() {  
- return name;  
-     }  
- publicvoid setName(String name) {  
- this.name = name;  
-     }  
- 
- }  
- 
- class Student implements Serializable{  
- 
- privatestaticfinallong serialVersionUID = 739748261443702736L;  
- private String name;  
- privateint age;  
- publicint getAge() {  
- return age;  
-     }  
- publicvoid setAge(int age) {  
- this.age = age;  
-     }  
- 
- private Teacher teacher;  
- public Teacher getTeacher() {  
- return teacher;  
-     }  
- publicvoid setTeacher(Teacher teacher) {  
- this.teacher = teacher;  
-     }  
- public String getName() {  
- return name;  
-     }  
- publicvoid setName(String name) {  
- this.name = name;  
-     }  
- /**
-      * 把对象读到字节流中在读出来
-      * @return
-      * @throws IOException
-      * @throws ClassNotFoundException
-      */
- public Object deepCopy() throws IOException, ClassNotFoundException{  
-         ByteArrayOutputStream bop = new ByteArrayOutputStream();  
- 
-         ObjectOutputStream oos = new ObjectOutputStream(bop);  
- 
-         oos.writeObject(this);  
- 
-         ByteArrayInputStream bis = new ByteArrayInputStream(bop.toByteArray());  
- 
-         ObjectInputStream ois = new ObjectInputStream(bis);  
- 
- return ois.readObject();  
- 
-     }  
- }  
运行结果：
s name=Student xiaoxin
s age=20
s t name=Teacher xin
s t age=30
-------------------
sCopy t name=Teacher mao
-------------------
s t name=Teacher xin
