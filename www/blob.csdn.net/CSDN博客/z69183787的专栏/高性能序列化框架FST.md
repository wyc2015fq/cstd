# 高性能序列化框架FST - z69183787的专栏 - CSDN博客
2016年11月02日 11:03:13[OkidoGreen](https://me.csdn.net/z69183787)阅读数：5740
fst是完全兼容JDK序列化协议的系列化框架，序列化速度大概是JDK的4-10倍，大小是JDK大小的1/3左右。
首先引入pom<dependency><groupId>de.ruedigermoeller</groupId><artifactId>fst</artifactId><version>2.04</version></dependency> 测试代码package zookeeper.seria;
import java.io.Serializable;
publicclass FSTSeriazle {
	publicstaticvoid main(String[] args) {
		User bean = new User();
		bean.setUsername("xxxxx");
		bean.setPassword("123456");
		bean.setAge(1000000);
		System.out.println("序列化 ， 反序列化 对比测试：");
		longsize = 0;
		long time1 = System.currentTimeMillis();
		for (int i = 0; i < 10000; i++) {
			byte[] jdkserialize = JRedisSerializationUtils.jdkserialize(bean);
			size += jdkserialize.length;
			JRedisSerializationUtils.jdkdeserialize(jdkserialize);
		}
		System.out.println("原生序列化方案[序列化10000次]耗时："
				+ (System.currentTimeMillis() - time1) + "ms size:=" + size);
		size = 0;
		long time2 = System.currentTimeMillis();
		for (int i = 0; i < 10000; i++) {
			byte[] serialize = JRedisSerializationUtils.serialize(bean);
			size += serialize.length;
			User u = (User) JRedisSerializationUtils.unserialize(serialize);
		}
		System.out.println("fst序列化方案[序列化10000次]耗时："
				+ (System.currentTimeMillis() - time2) + "ms size:=" + size);
		size = 0;
		long time3 = System.currentTimeMillis();
		for (int i = 0; i < 10000; i++) {
			byte[] serialize = JRedisSerializationUtils.kryoSerizlize(bean);
			size += serialize.length;
			User u = (User) JRedisSerializationUtils.kryoUnSerizlize(serialize);
		}
		System.out.println("kryo序列化方案[序列化10000次]耗时："
				+ (System.currentTimeMillis() - time3) + "ms size:=" + size);
	}
}
class User implements Serializable{
	private String username;
	privateint age;
	private String password;
	public String getUsername() {
		return username;
	}
	publicvoid setUsername(String username) {
		this.username = username;
	}
	publicint getAge() {
		return age;
	}
	publicvoid setAge(int age) {
		this.age = age;
	}
	public String getPassword() {
		return password;
	}
	publicvoid setPassword(String password) {
		this.password = password;
	}
} 结果序列化 ， 反序列化 对比测试：
原生序列化方案[序列化10000次]耗时：458ms size:=1160000
fst序列化方案[序列化10000次]耗时：184ms size:=550000
kryo序列化方案[序列化10000次]耗时：462ms size:=390000 工具类
```java
package zookeeper.seria;
import java.io.ByteArrayInputStream;
import java.io.ByteArrayOutputStream;
import java.io.IOException;
import java.io.ObjectInputStream;
import java.io.ObjectOutputStream;
import org.nustaq.serialization.FSTConfiguration;
import com.esotericsoftware.kryo.Kryo;
import com.esotericsoftware.kryo.io.Input;
import com.esotericsoftware.kryo.io.Output;
public class JRedisSerializationUtils {
	public JRedisSerializationUtils() {
	}
	static FSTConfiguration configuration = FSTConfiguration
	// .createDefaultConfiguration();
			.createStructConfiguration();
	public static byte[] serialize(Object obj) {
		return configuration.asByteArray(obj);
	}
	public static Object unserialize(byte[] sec) {
		return configuration.asObject(sec);
	}
	public static byte[] kryoSerizlize(Object obj) {
		Kryo kryo = new Kryo();
		byte[] buffer = new byte[2048];
		try(
				Output output = new Output(buffer);
				) {
			
			kryo.writeClassAndObject(output, obj);
			return output.toBytes();
		} catch (Exception e) {
		}
		return buffer;
	}
	static Kryo kryo = new Kryo();
	public static Object kryoUnSerizlize(byte[] src) {
		try(
		Input input = new Input(src);
				){
			return kryo.readClassAndObject(input);
		}catch (Exception e) {
		}
		return kryo;
	}
	// jdk原生序列换方案
	public static byte[] jdkserialize(Object obj) {
		try (ByteArrayOutputStream baos = new ByteArrayOutputStream();
				ObjectOutputStream oos = new ObjectOutputStream(baos);) {
			oos.writeObject(obj);
			return baos.toByteArray();
		} catch (IOException e) {
			throw new RuntimeException(e);
		}
	}
	public static Object jdkdeserialize(byte[] bits) {
		try (ByteArrayInputStream bais = new ByteArrayInputStream(bits);
				ObjectInputStream ois = new ObjectInputStream(bais);
		) {
			return ois.readObject();
		} catch (Exception e) {
			throw new RuntimeException(e);
		}
	}
}
```
