# Java类和成员上的一些方法 - Orisun - 博客园







# [Java类和成员上的一些方法](https://www.cnblogs.com/zhangchaoyang/articles/5094957.html)





## isInstance和isAssignableFrom

obj instanceof Class

判断obj是不是Class或者Class的子类的实例

clazz.isInstance(obj)

判断obj能不能强制转换成clazz类型，亦即obj是不是clazz或者clazz的子类的实例

clazz1.isAssignableFrom(clazz2)

如果clazz2和clazz1相同，或者clazz1是clazz2的父类则返回True，否则返回Flase

```
static class Parent{}
static class Son extends Parent{}

public static void main(String[] args) {
    Parent parent=new Parent();
    Son son=new Son();
    Assert.assertTrue(son instanceof Son);
    Assert.assertTrue(son instanceof Parent);
    Assert.assertFalse(parent instanceof Son);
    
    Assert.assertTrue(Son.class.isInstance(son));
    Assert.assertFalse(Son.class.isInstance(parent));
    Assert.assertTrue(Parent.class.isInstance(son));
    
    Assert.assertTrue(Son.class.isAssignableFrom(Son.class));
    Assert.assertFalse(Son.class.isAssignableFrom(Parent.class));
    Assert.assertTrue(Parent.class.isAssignableFrom(Son.class));
}
```

## **Modifier.isTransient(field.getModifiers())**

在使用Java自带的方式对对象进行序列化时，transient成员变量不会被序列化，比如银行密码这样的敏感信息不允许序列化到磁盘或者在网络上传输。

```
import java.io.ByteArrayInputStream;
import java.io.ByteArrayOutputStream;
import java.io.IOException;
import java.io.ObjectInputStream;
import java.io.ObjectOutputStream;
import java.io.Serializable;
  
public class Account implements Serializable{

    private static final long serialVersionUID = 2103161633120805900L;

    private String name;
    private transient String password;
    
    public Account(String n,String p){
        this.name=n;
        this.password=p;
    }
    
    @Override
    public String toString(){
        return "["+this.name+"]\t["+this.password+"]";
    }
    
    //序列化
    public static byte[] serialize(Object object) {
        ObjectOutputStream oos = null;
        ByteArrayOutputStream baos = null;
        try {
            baos = new ByteArrayOutputStream();
            oos = new ObjectOutputStream(baos);
            oos.writeObject(object);
            oos.close();
            byte[] bytes = baos.toByteArray();
            return bytes;
        } catch (Exception e) {
            e.printStackTrace();
        }
        return null;
    }

    // 反序列化
    public static Object deserialize(byte[] bytes) {
        ByteArrayInputStream bais = null;
        try {
            bais = new ByteArrayInputStream(bytes);
            ObjectInputStream ois = new ObjectInputStream(bais);
            Object rect=ois.readObject();
            ois.close();
            return rect;
        } catch (Exception e) {
            e.printStackTrace();
        }
        return null;
    }
    
    public static void main(String[] args) throws IOException {
        Account inst=new Account("orisun","123456");
        System.out.println("序列化前"+inst);

        byte[] datas=serialize(inst);
        Account inst2=(Account)deserialize(datas);
        System.out.println("序列化后"+inst2);
    }
}
```


















