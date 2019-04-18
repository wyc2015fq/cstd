# Protostuff序列化工具类 - z69183787的专栏 - CSDN博客
2016年09月26日 17:36:04[OkidoGreen](https://me.csdn.net/z69183787)阅读数：2768
介绍一个工具类，绕过构造函数创建Java实例：
如果一个类没有参数为空的构造方法时候，那么你直接调用newInstance方法试图得到一个实例对象的时候是会抛出异常的。能不能有 办法绕过构造方法来实例化一个对象呢？
Objenesis 为其提供了在四个不同的jvm上的解决方案。
- 
 Sun Hotspot VM, versions 1.3, 1.4, 1.5 and 1.6
- 
 GCJ version 3.4.4 (tested on Windows/Cygwin)
- 
 BEA JRockit versions 7.0 (1.3.1), 1.4.2 and 1.5
- 
 Aonix PERC (no serialization support), tested on version  5.0.0667
Protobuf 反序列化时可用 Objenesis 构造实例（version 2.1）
```
<dependency>
   <groupId>org.objenesis</groupId>
   <artifactId>objenesis</artifactId>
</dependency>
```
源代码
```java
```java
package org.wit.ff.util;
import java.io.ByteArrayInputStream;
import java.io.ByteArrayOutputStream;
import java.io.IOException;
import java.util.List;
import com.dyuproject.protostuff.LinkedBuffer;
import com.dyuproject.protostuff.ProtostuffIOUtil;
import com.dyuproject.protostuff.Schema;
import com.dyuproject.protostuff.runtime.RuntimeSchema;
/**
 * 
 * <pre>
 * 序列号工具
 * </pre>
 *
 * @author F.Fang
 */
public class ProtoStuffSerializerUtil {
    public static <T> byte[] serialize(T obj) {
        if (obj == null) {
            throw new RuntimeException("序列化对象(" + obj + ")!");
        }
        @SuppressWarnings("unchecked")
        Schema<T> schema = (Schema<T>) RuntimeSchema.getSchema(obj.getClass());
        LinkedBuffer buffer = LinkedBuffer.allocate(1024 * 1024);
        byte[] protostuff = null;
        try {
            protostuff = ProtostuffIOUtil.toByteArray(obj, schema, buffer);
        } catch (Exception e) {
            throw new RuntimeException("序列化(" + obj.getClass() + ")对象(" + obj + ")发生异常!", e);
        } finally {
            buffer.clear();
        }
        return protostuff;
    }
    public static <T> T deserialize(byte[] paramArrayOfByte, Class<T> targetClass) {
        if (paramArrayOfByte == null || paramArrayOfByte.length == 0) {
            throw new RuntimeException("反序列化对象发生异常,byte序列为空!");
        }
        T instance = null;
        try {
            //** T message = objenesis.newInstance(cls);**
            instance = targetClass.newInstance();
        } catch (InstantiationException | IllegalAccessException e) {
            throw new RuntimeException("反序列化过程中依据类型创建对象失败!", e);
        }
        Schema<T> schema = RuntimeSchema.getSchema(targetClass);
        ProtostuffIOUtil.mergeFrom(paramArrayOfByte, instance, schema);
        return instance;
    }
    
    public static <T> byte[] serializeList(List<T> objList) {
        if (objList == null || objList.isEmpty()) {
            throw new RuntimeException("序列化对象列表(" + objList + ")参数异常!");
        }
        @SuppressWarnings("unchecked")
        Schema<T> schema = (Schema<T>) RuntimeSchema.getSchema(objList.get(0).getClass());
        LinkedBuffer buffer = LinkedBuffer.allocate(1024 * 1024);
        byte[] protostuff = null;
        ByteArrayOutputStream bos = null;
        try {
            bos = new ByteArrayOutputStream();
            ProtostuffIOUtil.writeListTo(bos, objList, schema, buffer);
            protostuff = bos.toByteArray();
        } catch (Exception e) {
            throw new RuntimeException("序列化对象列表(" + objList + ")发生异常!", e);
        } finally {
            buffer.clear();
            try {
                if(bos!=null){
                    bos.close();
                }
            } catch (IOException e) {
                e.printStackTrace();
            }
        }
        
        return protostuff;
    }
    
    public static <T> List<T> deserializeList(byte[] paramArrayOfByte, Class<T> targetClass) {
        if (paramArrayOfByte == null || paramArrayOfByte.length == 0) {
            throw new RuntimeException("反序列化对象发生异常,byte序列为空!");
        }
        
        Schema<T> schema = RuntimeSchema.getSchema(targetClass);
        List<T> result = null;
        try {
            result = ProtostuffIOUtil.parseListFrom(new ByteArrayInputStream(paramArrayOfByte), schema);
        } catch (IOException e) {
            throw new RuntimeException("反序列化对象列表发生异常!",e);
        }
        return result;
    }
}
```
```
测试代码
```java
```java
public class ProtoStuffSerializerUtilTest {
    
    public static class Person{
        int id;
        String name;
        
        public Person(){
            
        }
        
        public Person(int id, String name){
            this.id = id;
            this.name = name;
        }
        
        public int getId() {
            return id;
        }
        public String getName() {
            return name;
        }
        
    }
    
    @Test
    public void demo(){
        Person p = new Person(1,"ff");
        byte[] arr = ProtoStuffSerializerUtil.serialize(p);
        Person result = ProtoStuffSerializerUtil.deserialize(arr, Person.class);
        assertEquals(p.getId(), result.getId());
        assertEquals(p.getName(), result.getName());
    }
}
```
```
