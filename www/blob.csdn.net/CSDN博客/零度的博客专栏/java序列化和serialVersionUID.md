# java序列化和serialVersionUID - 零度的博客专栏 - CSDN博客
2016年10月13日 17:47:46[零度anngle](https://me.csdn.net/zmx729618)阅读数：392
  1、序列化：
Java代码  ![收藏代码](http://luan.iteye.com/images/icon_star.png)
- 序列化可以将一个java对象以二进制流的方式在网络中传输并且可以被持久化到数据库、文件系统中，反序列化则是可以把之前持久化在数据库或文件系统中的二进制数据以流的方式读取出来重新构造成一个和之前相同内容的java对象。  
![](http://static.blog.csdn.net/images/save_snippets.png)
 2、序列化的作用：
Java代码  ![收藏代码](http://luan.iteye.com/images/icon_star.png)
- 第一种：用于将java对象状态储存起来，通常放到一个文件中，使下次需要用到的时候再读取到它之前的状态信息。  
- 第二种：可以让java对象在网络中传输。  
![](http://static.blog.csdn.net/images/save_snippets.png)
 3、序列化的实现：
Java代码  ![收藏代码](http://luan.iteye.com/images/icon_star.png)
- 1)、需要序列化的类需要实现Serializable接口，该接口没有任何方法，只是标示该类对象可被序列化。  
- 2）、序列化过程：使用一个输出流(如：FileOutputStream)来构造一个ObjectOutputStream(对象流)对象，接着，使用ObjectOutputStream对象的writeObject(Object obj)方法就可以将参数为obj的对象写出(即保存其状态)  
- 3)、反序列化过程：使用一个输入流(如：FileInputStream)来构造一个ObjectInputStream(对象流)对象，接着，使用ObjectInputStream对象的readObject(Object obj)方法就可以将参数为obj的对象读出(即获取其状态)  
![](http://static.blog.csdn.net/images/save_snippets.png)
 4、静态long类型常量serialVersionUID的作用：
Java代码  ![收藏代码](http://luan.iteye.com/images/icon_star.png)
- 如果没有设置这个值，你在序列化一个对象之后，改动了该类的字段或者方法名之类的，那如果你再反序列化想取出之前的那个对象时就可能会抛出异常，因为你改动了类中间的信息，serialVersionUID是根据类名、接口名、成员方法及属性等来生成一个64位的哈希字段,当修改后的类去反序列化的时候发现该类的serialVersionUID值和之前保存在问价中的serialVersionUID值不一致，所以就会抛出异常。  
- 
- 
- 而显示的设置serialVersionUID值就可以保证版本的兼容性，如果你在类中写上了这个值，就算类变动了，它反序列化的时候也能和文件中的原值匹配上。而新增的值则会设置成null，删除的值则不会显示。  
![](http://static.blog.csdn.net/images/save_snippets.png)
 5、例子：
Java代码  ![收藏代码](http://luan.iteye.com/images/icon_star.png)
- package com.sxit;  
- 
- import java.io.File;  
- import java.io.FileInputStream;  
- import java.io.FileNotFoundException;  
- import java.io.FileOutputStream;  
- import java.io.IOException;  
- import java.io.ObjectInputStream;  
- import java.io.ObjectOutputStream;  
- import java.io.Serializable;  
- 
- publicclass Serializable01 extends Object implements Serializable{  
- 
- privatestaticfinallong serialVersionUID = -1466479389299512377L;  
- 
- publicbyte b = 12;  
- 
- publicbyte getB() {  
- return b;  
-     }  
- 
- publicvoid setB(byte b) {  
- this.b = b;  
-     }  
- 
- publicstaticvoid main(String[] args) {  
- 
- try {  
-             FileOutputStream fos = new FileOutputStream(new File("E:\\tmp\\demo2.txt"));  
-             ObjectOutputStream os = new ObjectOutputStream(fos);  
-             Serializable01 s = new Serializable01();  
- 
-             os.writeObject(s);  
-             os.flush();  
-             os.close();  
- 
-             FileInputStream fis = new FileInputStream(new File("E:\\tmp\\demo2.txt"));  
-             ObjectInputStream ois = new ObjectInputStream(fis);  
- 
-             Serializable01 s2 = (Serializable01)ois.readObject();  
-             System.out.println(s2.getB());  
-             ois.close();  
- 
-         } catch (FileNotFoundException e) {  
-             e.printStackTrace();  
-         } catch (IOException e) {  
-             e.printStackTrace();  
-         } catch (ClassNotFoundException e) {  
-             e.printStackTrace();  
-         }  
-     }  
- }  
![](http://static.blog.csdn.net/images/save_snippets.png)
 6、序列化特点：
Java代码  ![收藏代码](http://luan.iteye.com/images/icon_star.png)
- 1）、如果一个类可被序列化，其子类也可以，如果该类有父类，则根据父类是否实现Serializable接口，实现了则父类对象字段可以序列化，没实现，则父类对象字段不能被序列化。  
- 2）、声明为transient类型的成员数据不能被序列化。transient代表对象的临时数据；  
- 3）、当一个对象的实例变量引用其他对象，序列化该对象时也把引用对象进行序列化；  
