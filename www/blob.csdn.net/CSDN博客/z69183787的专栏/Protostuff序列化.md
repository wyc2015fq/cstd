# Protostuff序列化 - z69183787的专栏 - CSDN博客
2016年09月26日 17:36:40[OkidoGreen](https://me.csdn.net/z69183787)阅读数：1365
前言：
       Java序列化是Java技术体系当中的一个重要议题，序列化的意义在于信息的交换和存储，通常会和io、持久化、rmi技术有关(eg:一些orm框架会要求持久化的对象类型实现Serializable接口)。
       本文将提供Java自带序列化机制和ProtoStuff的序列化(仅仅当作一种数据格式)的比较，从序列化的内容和特点来对二者进行比较。
       结论：1，Java序列化对象时不需要通过属性的get set方法或其它无关序列化内部定义的方法(比如readObject,writeObject是内置的序列化方法)，序列化也不需要get set方法支持，反序列化是构造对象的一种手段。
               2，Java序列化时类型必须完全匹配(全路径类名+序列化id)。
               3，Protostuff反序列化时并不要求类型匹配，比如包名、类名甚至是字段名，它仅仅需要序列化类型A 和反序列化类型B 的字段类型可转换(比如int可以转换为long)即可。
java.io.Serializable
       标识一个对象需要系列化，该对象类型需要实现 Serializable 接口。关于序列化的认识，可以参考IBM社区的文章《[Java序列化的高级认识](http://www.ibm.com/developerworks/cn/java/j-lo-serial/)》，本文直接拿该文档的结论。
       1，序列化的类型和反序列化的类型的序列化ID必须一致(远程信息交换时)。
       2，静态数据不会被序列化，Transient关键字修饰的字段不会被序列化。
       3，对象序列化存储时，两次存储相同值对象会有优化(第二次对象写入会只存储引用)。
ProtostuffUtil
```
![复制代码](http://common.cnblogs.com/images/copycode.gif)
import java.util.Map;
import java.util.concurrent.ConcurrentHashMap;
import com.dyuproject.protostuff.LinkedBuffer;
import com.dyuproject.protostuff.ProtostuffIOUtil;
import com.dyuproject.protostuff.Schema;
import com.dyuproject.protostuff.runtime.RuntimeSchema;
public class ProtostuffUtil {
    private static Map<Class<?>, Schema<?>> cachedSchema = new ConcurrentHashMap<Class<?>, Schema<?>>();
    private static <T> Schema<T> getSchema(Class<T> clazz) {
        @SuppressWarnings("unchecked")
        Schema<T> schema = (Schema<T>) cachedSchema.get(clazz);
        if (schema == null) {
            schema = RuntimeSchema.getSchema(clazz);
            if (schema != null) {
                cachedSchema.put(clazz, schema);
            }
        }
        return schema;
    }
    /**
     * 序列化
     *
     * @param obj
     * @return
     */
    public static <T> byte[] serializer(T obj) {
        @SuppressWarnings("unchecked")
        Class<T> clazz = (Class<T>) obj.getClass();
        LinkedBuffer buffer = LinkedBuffer.allocate(LinkedBuffer.DEFAULT_BUFFER_SIZE);
        try {
            Schema<T> schema = getSchema(clazz);
            return ProtostuffIOUtil.toByteArray(obj, schema, buffer);
        } catch (Exception e) {
            throw new IllegalStateException(e.getMessage(), e);
        } finally {
            buffer.clear();
        }
    }
    /**
     * 反序列化
     *
     * @param data
     * @param clazz
     * @return
     */
    public static <T> T deserializer(byte[] data, Class<T> clazz) {
        try {
            T obj = clazz.newInstance();
            Schema<T> schema = getSchema(clazz);
            ProtostuffIOUtil.mergeFrom(data, obj, schema);
            return obj;
        } catch (Exception e) {
            throw new IllegalStateException(e.getMessage(), e);
        }
    }
}
![复制代码](http://common.cnblogs.com/images/copycode.gif)
```
ProtostuffTest
```
![复制代码](http://common.cnblogs.com/images/copycode.gif)
import java.io.ByteArrayInputStream;
import java.io.ByteArrayOutputStream;
import java.io.IOException;
import java.io.ObjectInputStream;
import java.io.ObjectOutputStream;
import java.util.Random;
import org.apache.commons.lang.StringUtils;
public class ProtostuffTest {
    /** 产生一个随机的字符串*/
    public static String randomString(int length) {
        String str = "abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ0123456789";
        int strlen = str.length();
        Random random = new Random();
        StringBuffer buf = new StringBuffer();
        for (int i = 0; i < length; i++) {
            int num = random.nextInt(strlen);
            buf.append(str.charAt(num));
        }
        return buf.toString();
    }
    private static ResourceObj getObj(String name, String path, int contentSize) {
        ResourceObj obj = new ResourceObj(name, path, "");
        obj.setContent(randomString(contentSize));
        return obj;
    }
    private static long speedTest(int contentSize, int times) {
        ResourceObj obj = getObj("lb.conf", "/home/admin/conf/lb", contentSize);
        long start = System.currentTimeMillis();
        for (int i = 0; i < times; i++) {
            byte[] bytes = ProtostuffUtil.serializer(obj);
            ProtostuffUtil.deserializer(bytes, ResourceObj.class);
        }
        long end = System.currentTimeMillis();
        return end - start;
    }
    private static long speedTestOrg(int contentSize, int times) throws IOException,
                                                                ClassNotFoundException {
        ResourceObj obj = getObj("lb.conf", "/home/admin/conf/lb", contentSize);
        long start = System.currentTimeMillis();
        for (int i = 0; i < times; i++) {
            ByteArrayOutputStream baos = new ByteArrayOutputStream();
            ObjectOutputStream oos = new ObjectOutputStream(baos);
            oos.writeObject(obj);
            byte[] bytes = baos.toByteArray();
            ByteArrayInputStream bais = new ByteArrayInputStream(bytes);
            ObjectInputStream ois = new ObjectInputStream(bais);
            ois.readObject();
        }
        long end = System.currentTimeMillis();
        return end - start;
    }
    public static void main(String[] args) throws IOException, ClassNotFoundException {
        System.out.println(speedTestOrg(9999999, 1));
        System.out.println(speedTest(9999999, 1));
    }
    private static void test() {
        ResourceObj obj = getObj("lb.conf", "/home/admin/conf/lb", 88888);
        byte[] bytes = ProtostuffUtil.serializer(obj);
        ResourceObj obj2 = ProtostuffUtil.deserializer(bytes, ResourceObj.class);
        System.out.println(obj2.getFilename());
        System.out.println(obj2.getPath());
        System.out.println(StringUtils.equals(obj.getContent(), obj2.getContent()));
    }
}
![复制代码](http://common.cnblogs.com/images/copycode.gif)
```
结果
```
![复制代码](http://common.cnblogs.com/images/copycode.gif)
size:10000000
java-build-in:189ms
protostaff:237ms
size:100000000
java-build-in:1661ms
protostaff:792ms
![复制代码](http://common.cnblogs.com/images/copycode.gif)
```
自己测试的结果：
1、在对象较小的时候，还是java自带的序列化比较快。10M之下的时候。（没有考虑对象的复杂度）
2、文件较大时，protostuff比较快。（protostuff-runtime方式，用protostuff可能会更快，只是没这么方便）
Google 的protobuf是一个优秀的序列化工具，跨语言、快速、序列化后体积小。
protobuf的一个缺点是需要数据结构的预编译过程，首先要编写.proto格式的配置文件，再通过protobuf提供的工具生成各种语言响应的代码。由于java具有反射和动态代码生成的能力，这个预编译过程不是必须的，可以在代码执行时来实现。有个protostuff(http://code.google.com/p/protostuff/)已经实现了这个功能。
protostuff基于Google protobuf，但是提供了更多的功能和更简易的用法。其中，protostuff-runtime实现了无需预编译对java bean进行protobuf序列化/反序列化的能力。
protostuff-runtime的局限是序列化前需预先传入schema，反序列化不负责对象的创建只负责复制，因而必须提供默认构造函数。
此外，protostuff还可以按照protobuf的配置序列化成json/yaml/xml等格式。
