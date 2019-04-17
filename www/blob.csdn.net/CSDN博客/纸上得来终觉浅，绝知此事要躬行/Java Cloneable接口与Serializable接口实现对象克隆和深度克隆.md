# Java Cloneable接口与Serializable接口实现对象克隆和深度克隆 - 纸上得来终觉浅，绝知此事要躬行 - CSDN博客





2017年04月27日 15:42:36[boonya](https://me.csdn.net/boonya)阅读数：973标签：[java																[api																[Cloneable																[Serilizable](https://so.csdn.net/so/search/s.do?q=Serilizable&t=blog)
个人分类：[Java Practice](https://blog.csdn.net/boonya/article/category/3089055)





 Cloneable接口与Serializable接口都是定义接口而没有任何的方法。Cloneable可以实现对象的克隆复制，Serializable主要是对象序列化的接口定义。很多时候我们涉及到对象的复制，我们不可能都去使用setter去实现，这样编写代码的效率太低。JDK提供的Cloneable接口正是为了解决对象复制的问题而存在。Cloneable结合Serializable接口可以实现JVM对象的深度复制。


## Java JDK定义的接口

本文涉及到的两个接口都是功能定义接口没有实际的方法声明：


### **java.lang.Cloneable**



```java
/*
 * Copyright (c) 1995, 2004, Oracle and/or its affiliates. All rights reserved.
 * ORACLE PROPRIETARY/CONFIDENTIAL. Use is subject to license terms.
 *
 */

package java.lang;

/**
 * A class implements the <code>Cloneable</code> interface to
 * indicate to the {@link java.lang.Object#clone()} method that it
 * is legal for that method to make a
 * field-for-field copy of instances of that class.
 * <p>
 * Invoking Object's clone method on an instance that does not implement the
 * <code>Cloneable</code> interface results in the exception
 * <code>CloneNotSupportedException</code> being thrown.
 * <p>
 * By convention, classes that implement this interface should override
 * <tt>Object.clone</tt> (which is protected) with a public method.
 * See {@link java.lang.Object#clone()} for details on overriding this
 * method.
 * <p>
 * Note that this interface does <i>not</i> contain the <tt>clone</tt> method.
 * Therefore, it is not possible to clone an object merely by virtue of the
 * fact that it implements this interface.  Even if the clone method is invoked
 * reflectively, there is no guarantee that it will succeed.
 *
 * @author  unascribed
 * @see     java.lang.CloneNotSupportedException
 * @see     java.lang.Object#clone()
 * @since   JDK1.0
 */
public interface Cloneable {
}
```



Cloneable接口是一个空接口，仅用于标记对象，Cloneable接口里面是没有clone()方法，的clone()方法是Object类里面的方法！默认实现是一个Native方法



```java
protected native Object clone() throws CloneNotSupportedException;
```

如果对象implement Cloneable接口的话，需要覆盖clone方法（因为Object类的clone方法是protected，需要覆盖为public）




```java
public Object clone() throws CloneNotSupportedException{
    return super.clone();
}
```




Object类里的clone()方法仅仅用于浅拷贝（拷贝基本成员属性，对于引用类型仅返回指向改地址的引用）

### **java.io.Serializable**



```java
/*
 * Copyright (c) 1996, 2013, Oracle and/or its affiliates. All rights reserved.
 * ORACLE PROPRIETARY/CONFIDENTIAL. Use is subject to license terms.
 *
 */

package java.io;

/**
 * Serializability of a class is enabled by the class implementing the
 * java.io.Serializable interface. Classes that do not implement this
 * interface will not have any of their state serialized or
 * deserialized.  All subtypes of a serializable class are themselves
 * serializable.  The serialization interface has no methods or fields
 * and serves only to identify the semantics of being serializable. <p>
 *
 * To allow subtypes of non-serializable classes to be serialized, the
 * subtype may assume responsibility for saving and restoring the
 * state of the supertype's public, protected, and (if accessible)
 * package fields.  The subtype may assume this responsibility only if
 * the class it extends has an accessible no-arg constructor to
 * initialize the class's state.  It is an error to declare a class
 * Serializable if this is not the case.  The error will be detected at
 * runtime. <p>
 *
 * During deserialization, the fields of non-serializable classes will
 * be initialized using the public or protected no-arg constructor of
 * the class.  A no-arg constructor must be accessible to the subclass
 * that is serializable.  The fields of serializable subclasses will
 * be restored from the stream. <p>
 *
 * When traversing a graph, an object may be encountered that does not
 * support the Serializable interface. In this case the
 * NotSerializableException will be thrown and will identify the class
 * of the non-serializable object. <p>
 *
 * Classes that require special handling during the serialization and
 * deserialization process must implement special methods with these exact
 * signatures:
 *
 * <PRE>
 * private void writeObject(java.io.ObjectOutputStream out)
 *     throws IOException
 * private void readObject(java.io.ObjectInputStream in)
 *     throws IOException, ClassNotFoundException;
 * private void readObjectNoData()
 *     throws ObjectStreamException;
 * </PRE>
 *
 * <p>The writeObject method is responsible for writing the state of the
 * object for its particular class so that the corresponding
 * readObject method can restore it.  The default mechanism for saving
 * the Object's fields can be invoked by calling
 * out.defaultWriteObject. The method does not need to concern
 * itself with the state belonging to its superclasses or subclasses.
 * State is saved by writing the individual fields to the
 * ObjectOutputStream using the writeObject method or by using the
 * methods for primitive data types supported by DataOutput.
 *
 * <p>The readObject method is responsible for reading from the stream and
 * restoring the classes fields. It may call in.defaultReadObject to invoke
 * the default mechanism for restoring the object's non-static and
 * non-transient fields.  The defaultReadObject method uses information in
 * the stream to assign the fields of the object saved in the stream with the
 * correspondingly named fields in the current object.  This handles the case
 * when the class has evolved to add new fields. The method does not need to
 * concern itself with the state belonging to its superclasses or subclasses.
 * State is saved by writing the individual fields to the
 * ObjectOutputStream using the writeObject method or by using the
 * methods for primitive data types supported by DataOutput.
 *
 * <p>The readObjectNoData method is responsible for initializing the state of
 * the object for its particular class in the event that the serialization
 * stream does not list the given class as a superclass of the object being
 * deserialized.  This may occur in cases where the receiving party uses a
 * different version of the deserialized instance's class than the sending
 * party, and the receiver's version extends classes that are not extended by
 * the sender's version.  This may also occur if the serialization stream has
 * been tampered; hence, readObjectNoData is useful for initializing
 * deserialized objects properly despite a "hostile" or incomplete source
 * stream.
 *
 * <p>Serializable classes that need to designate an alternative object to be
 * used when writing an object to the stream should implement this
 * special method with the exact signature:
 *
 * <PRE>
 * ANY-ACCESS-MODIFIER Object writeReplace() throws ObjectStreamException;
 * </PRE><p>
 *
 * This writeReplace method is invoked by serialization if the method
 * exists and it would be accessible from a method defined within the
 * class of the object being serialized. Thus, the method can have private,
 * protected and package-private access. Subclass access to this method
 * follows java accessibility rules. <p>
 *
 * Classes that need to designate a replacement when an instance of it
 * is read from the stream should implement this special method with the
 * exact signature.
 *
 * <PRE>
 * ANY-ACCESS-MODIFIER Object readResolve() throws ObjectStreamException;
 * </PRE><p>
 *
 * This readResolve method follows the same invocation rules and
 * accessibility rules as writeReplace.<p>
 *
 * The serialization runtime associates with each serializable class a version
 * number, called a serialVersionUID, which is used during deserialization to
 * verify that the sender and receiver of a serialized object have loaded
 * classes for that object that are compatible with respect to serialization.
 * If the receiver has loaded a class for the object that has a different
 * serialVersionUID than that of the corresponding sender's class, then
 * deserialization will result in an {@link InvalidClassException}.  A
 * serializable class can declare its own serialVersionUID explicitly by
 * declaring a field named <code>"serialVersionUID"</code> that must be static,
 * final, and of type <code>long</code>:
 *
 * <PRE>
 * ANY-ACCESS-MODIFIER static final long serialVersionUID = 42L;
 * </PRE>
 *
 * If a serializable class does not explicitly declare a serialVersionUID, then
 * the serialization runtime will calculate a default serialVersionUID value
 * for that class based on various aspects of the class, as described in the
 * Java(TM) Object Serialization Specification.  However, it is <em>strongly
 * recommended</em> that all serializable classes explicitly declare
 * serialVersionUID values, since the default serialVersionUID computation is
 * highly sensitive to class details that may vary depending on compiler
 * implementations, and can thus result in unexpected
 * <code>InvalidClassException</code>s during deserialization.  Therefore, to
 * guarantee a consistent serialVersionUID value across different java compiler
 * implementations, a serializable class must declare an explicit
 * serialVersionUID value.  It is also strongly advised that explicit
 * serialVersionUID declarations use the <code>private</code> modifier where
 * possible, since such declarations apply only to the immediately declaring
 * class--serialVersionUID fields are not useful as inherited members. Array
 * classes cannot declare an explicit serialVersionUID, so they always have
 * the default computed value, but the requirement for matching
 * serialVersionUID values is waived for array classes.
 *
 * @author  unascribed
 * @see java.io.ObjectOutputStream
 * @see java.io.ObjectInputStream
 * @see java.io.ObjectOutput
 * @see java.io.ObjectInput
 * @see java.io.Externalizable
 * @since   JDK1.1
 */
public interface Serializable {
}
```


## 使用Cloneable接口对Java对象进行克隆

实现克隆的步骤：

1.让该类实现java.lang.Cloneable接口；

2. 重写（override）Object类的clone()方法。


普通对象的克隆是克隆属性的值，而不是引用。



```java
public class Person implements Cloneable {

	private int id;

	private String name;

	public int getId() {
		return id;
	}

	public void setId(int id) {
		this.id = id;
	}

	public String getName() {
		return name;
	}

	public void setName(String name) {
		this.name = name;
	}

	@Override
	protected Object clone() throws CloneNotSupportedException {
		// TODO Auto-generated method stub
		return super.clone();
	}

	/*
	 * (non-Javadoc)
	 * 
	 * @see java.lang.Object#toString()
	 */
	@Override
	public String toString() {
		return "Person [id=" + id + ", name=" + name + "]";
	}

	public static void main(String[] args) throws CloneNotSupportedException {
		Person person = new Person();
		person.setId(1);
		person.setName("boonya");

		System.out.println(person);

		Person clonePerson = (Person) person.clone();

		System.out.println("==" + (person == clonePerson));

		person.setId(2);
		System.out.println(clonePerson);

	}

}
```

运行时输出：


```
Person [id=1, name=boonya]
==false
Person [id=1, name=boonya]
```
同样的，我们在测试一个：


```java
import java.util.Date;


public class User implements Cloneable {
	
	private String username;

	private String password;

	private Date birthdate;
	
	public User(String username, String password, Date birthdate) {
		super();
		this.username = username;
		this.password = password;
		this.birthdate = birthdate;
	}

	/**
	 * @return the username
	 */
	public String getUsername() {
		return username;
	}

	/**
	 * @param username the username to set
	 */
	public void setUsername(String username) {
		this.username = username;
	}

	/**
	 * @return the password
	 */
	public String getPassword() {
		return password;
	}

	/**
	 * @param password the password to set
	 */
	public void setPassword(String password) {
		this.password = password;
	}

	/**
	 * @return the birthdate
	 */
	public Date getBirthdate() {
		return birthdate;
	}

	/**
	 * @param birthdate the birthdate to set
	 */
	public void setBirthdate(Date birthdate) {
		this.birthdate = birthdate;
	}

	/* (non-Javadoc)
	 * @see java.lang.Object#clone()
	 */
	@Override
	protected Object clone() throws CloneNotSupportedException {
		// TODO Auto-generated method stub
		return super.clone();
	}
	
	
	/* (non-Javadoc)
	 * @see java.lang.Object#equals(java.lang.Object)
	 */
	public boolean equals(User user) {
		return toString().equals(user.toString());
	}

	/* (non-Javadoc)
	 * @see java.lang.Object#toString()
	 */
	@Override
	public String toString() {
		return "User [username=" + username + ", password=" + password
				+ ", birthdate=" + birthdate + "]";
	}

	public static void main(String[] args) throws CloneNotSupportedException {
		User u1 = new User("Kent", "123456", new Date());

		User u2 = u1; 

		User u3 = (User) u1.clone();

		System.out.println(u1 == u2);		// true

		System.out.println(u1.equals(u2));	// true

		System.out.println(u1 == u3);		// false

		System.out.println(u1.equals(u3));	// true

	}

}
```

u1和u2指向同一个引用，而u1和u3只是值相同。注意重写Object的clone()方法时需要实现接口，否则报错，说明必须实现java.lang.Cloneable接口：



```java
Exception in thread "main" java.lang.CloneNotSupportedException: PUser
	at java.lang.Object.clone(Native Method)
	at PUser.clone(PUser.java:53)
	at PUser.main(PUser.java:61)
```



## 对象实现Cloneable和Serializable接口实现对象深度克隆

先来看下如何简单地实现Serializable接口：



```java
import java.io.Serializable;


public class Student implements Serializable {
	
	// getter
	
	// setter

	/**
	 * 
	 */
	private static final long serialVersionUID = -2467119047141875151L;

}
```

一般地对象克隆可能存在问题，来看下如下代码：


```java
import java.util.Date;

public class PUser implements Cloneable {

	private User user;

	private Boolean editable;

	public PUser(User user, Boolean editable) {

		this.user = user;

		this.editable = editable;

	}

	/**
	 * @return the user
	 */
	public User getUser() {
		return user;
	}

	/**
	 * @param user
	 *            the user to set
	 */
	public void setUser(User user) {
		this.user = user;
	}

	/**
	 * @return the editable
	 */
	public Boolean getEditable() {
		return editable;
	}

	/**
	 * @param editable
	 *            the editable to set
	 */
	public void setEditable(Boolean editable) {
		this.editable = editable;
	}

	/*
	 * (non-Javadoc)
	 * 
	 * @see java.lang.Object#clone()
	 */
	@Override
	protected Object clone() throws CloneNotSupportedException {
		// TODO Auto-generated method stub
		return super.clone();
	}

	/*
	 * (non-Javadoc)
	 * 
	 * @see java.lang.Object#equals(java.lang.Object)
	 */
	public boolean equals(PUser pUser) {
		// TODO Auto-generated method stub
		return toString().equals(pUser.toString());
	}

	/*
	 * (non-Javadoc)
	 * 
	 * @see java.lang.Object#toString()
	 */
	@Override
	public String toString() {
		return "PUser [user=" + user + ", editable=" + editable + "]";
	}

	public static void main(String[] args) throws CloneNotSupportedException {
		PUser a1 = new PUser(new User("Kent", "123456", new Date()), true);

		PUser a2 = a1;

		PUser a3 = (PUser) a1.clone();

		System.out.println(a1 == a2); // true

		System.out.println(a1.equals(a2)); // true

		System.out.println(a1 == a3); // false

		System.out.println(a1.equals(a3)); // true

		System.out.println(a1.getUser() == a3.getUser()); // true ! It's not our expected!!!!!

		System.out.println(a1.getUser().equals(a3.getUser())); // true

	}

}
```

a1和a3获取用户的比较应该是false才符合预期，这就是对象浅层复制的问题。实现对象的深度克隆需要借助java.io.Serializable接口。
下面来看一个序列化的测试：



```java
import java.io.FileInputStream;
import java.io.FileNotFoundException;
import java.io.FileOutputStream;
import java.io.IOException;
import java.io.ObjectInputStream;
import java.io.ObjectOutputStream;
import java.io.Serializable;

public class DeepClone implements Serializable {
	
	    // 序列化 ID 在 Eclipse 下提供了两种生成策略
		// 一个是固定的 1L，一个是随机生成一个不重复的 long 类型数据（实际上是使用 JDK 工具生成）
		// 如果没有特殊需求，就是用默认的 1L 就可以
		static final long serialVersionUID = 1L;
		
		String name;
		
		int num;
		
		static int staticNum;
		// transient关键字是不能被虚拟机默认序列化的，如果想序列化需要重写private void
		// writeObject(ObjectOutputStream s)和private void
		// readObject(ObjectInputStream s)
		transient String pwd;
		
		transient int num0;

		DeepClone(String name, int num, int staticNum, String pwd, int num0) {
			this.name = name;
			this.num = num;
			this.staticNum = staticNum;
			this.pwd = pwd;
			this.num0 = num0;
		}

		public String toString() {
			return "name=" + name + ",num=" + num + ",staticNum=" + staticNum
					+ ",pwd=" + pwd + ",num0=" + num0;
		}

	
	
	public static void main(String[] args) {
		
		ObjectOutputStream objectos = null;
		
		DeepClone old = new DeepClone("str", 12, 1, "123456", 8);
        
        try {
            objectos = new ObjectOutputStream(new FileOutputStream("test.dat"));
            objectos.writeObject(old);
            objectos.flush();
            objectos.close();
        } catch (FileNotFoundException e) {
            // TODO Auto-generated catch block
            e.printStackTrace();
        } catch (IOException e) {
            // TODO Auto-generated catch block
            e.printStackTrace();
        }
 
        ObjectInputStream objectin;
        
        DeepClone mts = null;
        
        try {
            objectin = new ObjectInputStream(new FileInputStream("test.dat"));
            mts = (DeepClone) objectin.readObject();
        } catch (FileNotFoundException e) {
            // TODO Auto-generated catch block
            e.printStackTrace();
        } catch (IOException e) {
            // TODO Auto-generated catch block
            e.printStackTrace();
        } catch (ClassNotFoundException e) {
            // TODO Auto-generated catch block
            e.printStackTrace();
        }
 
        System.out.println(old);
        System.out.println(mts);
        System.out.println(old==mts);
        // 注意，没有覆盖equals()方法，Object类的equals()默认是地址的比较
        System.out.println(mts.equals(old));
	}
}
```

输出：


```java
name=str,num=12,staticNum=1,pwd=123456,num0=8
name=str,num=12,staticNum=1,pwd=null,num0=0
false
false
```

上面基于对象流的操作很好地实现了对象的序列化处理。将基于序列化的流操作提出为一个工具类：


```java
import java.io.ByteArrayInputStream;
import java.io.ByteArrayOutputStream;
import java.io.IOException;
import java.io.ObjectInputStream;
import java.io.ObjectOutputStream;

public class CloneUtil {

	@SuppressWarnings("unchecked")
	public static <T> T cloneTo(T src) throws RuntimeException {

		ByteArrayOutputStream memoryBuffer = new ByteArrayOutputStream();
		ObjectOutputStream out = null;
		ObjectInputStream in = null;
		T dist = null;

		try {
			out = new ObjectOutputStream(memoryBuffer);
			out.writeObject(src);
			out.flush();

			in = new ObjectInputStream(new ByteArrayInputStream(
					memoryBuffer.toByteArray()));
			dist = (T) in.readObject();
		} catch (Exception e) {

			throw new RuntimeException(e);

		} finally {

			if (out != null)

				try {
					out.close();
					out = null;
				} catch (IOException e) {
					throw new RuntimeException(e);
				}

			if (in != null)
				try {
					in.close();
					in = null;
				} catch (IOException e) {
					throw new RuntimeException(e);
				}
		}

		return dist;

	}

}
```

此时我们在通过工具类去复制对象--User、PUser都需要实现Serilizable接口：


```java
import java.io.Serializable;
import java.util.Date;


public class User implements Cloneable,Serializable{
	
	/**
	 * 
	 */
	private static final long serialVersionUID = -7040740302952460639L;

	private String username;

	private String password;

	private Date birthdate;
	
	public User(String username, String password, Date birthdate) {
		super();
		this.username = username;
		this.password = password;
		this.birthdate = birthdate;
	}

	/**
	 * @return the username
	 */
	public String getUsername() {
		return username;
	}

	/**
	 * @param username the username to set
	 */
	public void setUsername(String username) {
		this.username = username;
	}

	/**
	 * @return the password
	 */
	public String getPassword() {
		return password;
	}

	/**
	 * @param password the password to set
	 */
	public void setPassword(String password) {
		this.password = password;
	}

	/**
	 * @return the birthdate
	 */
	public Date getBirthdate() {
		return birthdate;
	}

	/**
	 * @param birthdate the birthdate to set
	 */
	public void setBirthdate(Date birthdate) {
		this.birthdate = birthdate;
	}

	/* (non-Javadoc)
	 * @see java.lang.Object#clone()
	 */
	@Override
	protected Object clone() throws CloneNotSupportedException {
		// TODO Auto-generated method stub
		return super.clone();
	}
	
	
	/* (non-Javadoc)
	 * @see java.lang.Object#equals(java.lang.Object)
	 */
	public boolean equals(User user) {
		return toString().equals(user.toString());
	}

	/* (non-Javadoc)
	 * @see java.lang.Object#toString()
	 */
	@Override
	public String toString() {
		return "User [username=" + username + ", password=" + password
				+ ", birthdate=" + birthdate + "]";
	}

	public static void main(String[] args) throws CloneNotSupportedException {
		User u1 = new User("Kent", "123456", new Date());

		User u2 = u1; 

		User u3 = (User) u1.clone();

		System.out.println(u1 == u2);		// true

		System.out.println(u1.equals(u2));	// true

		System.out.println(u1 == u3);		// false It's our expected!!!

		System.out.println(u1.equals(u3));	// true

	}

}
```

输出：


```java
true
true
false
true
false
true
```

这样就符合我们的预期了。




## 参考资料



[Java对象克隆（Clone）及Cloneable接口、Serializable接口的深入探讨](http://kentkwan.iteye.com/blog/739514)

[Java中的Serializable接口transient关键字，及字节、字符、对象IO](http://www.cnblogs.com/tonyluis/p/5779187.html)](https://so.csdn.net/so/search/s.do?q=Cloneable&t=blog)](https://so.csdn.net/so/search/s.do?q=api&t=blog)](https://so.csdn.net/so/search/s.do?q=java&t=blog)




