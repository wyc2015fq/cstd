# 使用 hibernate 存取大对象数据类型（clob和blob） - z69183787的专栏 - CSDN博客
2013年11月22日 13:19:22[OkidoGreen](https://me.csdn.net/z69183787)阅读数：21015
数据库表如下:
book表
![QQ截图20131015201126](http://images.cnitblog.com/blog/556868/201310/15212336-e62439ad73f24c379cb7ce5cb542089d.png)
|id|该表的主键。number类型。|
|----|----|
|photo|代表图书的图片，blob类型。|
|description|图书的描述，clob类型。|
## 使用 hibernate3 往 book 表插入Clob,Blob数据
省略hibernate配置文件，实体映射文件和实体类代码，直接帖实现代码：
```
![复制代码](http://common.cnblogs.com/images/copycode.gif)
package accp.hibernate;
import java.io.FileInputStream;
import java.io.IOException;
import java.io.InputStream;
import java.sql.Blob;
import java.sql.Clob;
import org.hibernate.Hibernate;
import org.hibernate.Session;
import org.hibernate.SessionFactory;
import org.hibernate.cfg.Configuration;
public class Client {
    public static void main(String[] args) throws IOException {
        //得到session
        Configuration cfg = new Configuration().configure();
        SessionFactory sf = cfg.buildSessionFactory();
        Session session = sf.openSession();
        //得到图片的blob
        InputStream in = new FileInputStream("F:\\4563123.jpg");
        Blob blob = Hibernate.createBlob(in);
        //得到简介的clob
        Clob clob = Hibernate.createClob("这是一本书和详细描述。#（*&#@￥%（*&@￥）（@#￥#￥");
        //创建图书对象
        Book book = new Book();
        book.setPhoto(blob);
        book.setDescription(clob);
        //存进数据库
        session.beginTransaction();
        session.save(book);
        session.getTransaction().commit();
        session.close();
    }
}
![复制代码](http://common.cnblogs.com/images/copycode.gif)
```
    使用hibernate的静态方法createBlob()得到Blob对象。createBlob方法需要一个InputStream对象作为参数。new一个FileInputStream作为它的参数，假设在F盘有一个图片文件名为4563123.jpg。
插入方法2：
bo：
```java
/**
 * 
 */
package test.archive.model;
import java.sql.Blob;
import javax.persistence.Basic;
import javax.persistence.Column;
import javax.persistence.Entity;
import javax.persistence.FetchType;
import javax.persistence.GeneratedValue;
import javax.persistence.Id;
import javax.persistence.Lob;
import javax.persistence.Table;
import org.hibernate.annotations.GenericGenerator;
/** 
 * @ClassName: BlobTest 
 * @Description: TODO(这里用一句话描述这个类的作用) 
 * @author zhoushun 
 * @date 2013-4-3 上午11:15:38 
 *  
 */
@Entity
@Table(name = "T_FUNC_XT_YWDXSM")
public class BlobTest implements java.io.Serializable {
	
	/**
	 * 
	 */
	private static final long serialVersionUID = 1L;
	private byte[] imgs;
	private String mc;
	private String id;
	
	
	
	@Id
	@GeneratedValue(generator = "system-uuid")
	@GenericGenerator(name = "system-uuid", strategy = "uuid")
	@Column(name = "ID")
	public String getId() {
		return id;
	}
	public void setId(String id) {
		this.id = id;
	}
	@Column(name = "MC_DX", nullable = true, length = 500)
	public String getMc() {
		return mc;
	}
	public void setMc(String mc) {
		this.mc = mc;
	}
	@Lob 
	@Basic(fetch=FetchType.LAZY) 
	@Column(name="IMAGE", columnDefinition="BLOB", nullable=true)
	public byte[] getImgs() {
		return imgs;
	}
	public void setImgs(byte[] imgs) {
		this.imgs = imgs;
	}
	
	
}
```
方法：
```java
@SuppressWarnings("unchecked")
	public void blobTest() {
		BlobTest b = new BlobTest();
		try {
			FileInputStream fis = new FileInputStream(
					"E:\\Users\\zhoushun\\Pictures\\0fb3c75c9037e36bfbf2c0b5.jpg");
			byte[] buffer = new byte[fis.available()];
			fis.read(buffer);
		fis.close();
			b.setImgs(buffer);
			b.setMc("zhoushun");
			this.h.save(b);
		} catch (FileNotFoundException e) {
			// TODO Auto-generated catch block
			e.printStackTrace();
		}// 定义文件读入流
		catch (IOException e) {
			// TODO Auto-generated catch block
			e.printStackTrace();
		}
	}
```
    使用hibernate的静态方法createClob()得到Clob对象。createClob方法直接传入字符串即可。
## 使用 hibernate3 从 book 表取出Clob,Blob数据
省略hibernate配置文件，实体映射文件和实体类代码，直接帖实现代码：
```
![复制代码](http://common.cnblogs.com/images/copycode.gif)
 1 package accp.hibernate;
 2 
 3 import java.io.FileOutputStream;
 4 import java.io.IOException;
 5 import java.io.InputStream;
 6 import java.io.OutputStream;
 7 import java.io.Reader;
 8 import java.sql.SQLException;
 9 import org.hibernate.Session;
10 import org.hibernate.SessionFactory;
11 import org.hibernate.cfg.Configuration;
12 
13 public class Client {
14 
15     public static void main(String[] args) throws IOException, SQLException {
16         //得到session
17         Configuration cfg = new Configuration().configure();
18         SessionFactory sf = cfg.buildSessionFactory();
19         Session session = sf.openSession();
20 
21         //从数据库里取值
22         session.beginTransaction();
23         Book book = (Book)session.get(Book.class,22);
24         session.getTransaction().commit();
25         session.close();
26         //把简历打印到控制台
27         Reader reader = book.getDescription().getCharacterStream();
28         int i = reader.read();
29         while(i!=-1){
30             System.out.print((char)i);
31             i=reader.read();
32         }
33         reader.close();
34         //把图片拷贝到D盘
35         InputStream in = book.getPhoto().getBinaryStream();
36         OutputStream out=new FileOutputStream("d:\\out.jpg");
37         byte[] b=new byte[1024];    
38         while((i=in.read(b))!=-1){
39             out.write(b);
40         }
41         out.close();
42         in.close();
43     }
44 
45 }
![复制代码](http://common.cnblogs.com/images/copycode.gif)
```
    21-25行代码把数据从数据库中取出，封装到book对象中。
    为了验证取值是否成功，第26-33行代码把Clob对象的内容打印到控制台，通过Clob对象的getCharacterStream()方法得到包含 `CLOB` 数据的 `java.io.Reader` 对象，然后通过操作Reader对象把内容输出。
    第34-42行代码把Blob对象包含的内容拷贝到d:\out.jpg文件中。通过Blob对象的getBinaryStream()方法得到包含 `BLOB` 数据的流，通过操作流把内容拷贝。
## 使用 hibernate4 往 book 表插入Clob,Blob数据
hibernate4相比hibernate3要复杂一些，如下：
```
![复制代码](http://common.cnblogs.com/images/copycode.gif)
//省略import...........
public class Client {
    public static void main(String[] args) throws IOException {
        //得到session
        Configuration cfg = new Configuration().configure();
        ServiceRegistry  sr = new ServiceRegistryBuilder().applySettings(cfg.getProperties()).buildServiceRegistry(); 
        SessionFactory sf = cfg.buildSessionFactory(sr);
        Session session = sf.openSession();
        //得到LobHelper
        LobHelper lobHelper = session.getLobHelper();
        //得到图片的blob
        InputStream in = new FileInputStream("F:\\4563123.jpg");
        Blob blob = lobHelper.createBlob(in, in.available());
        //得到简介的clob
        Clob clob = lobHelper.createClob("这是一本书。#（*&#@￥%（*&@￥）（@#￥#￥");
        //创建图书对象
        Book book = new Book();
        book.setPhoto(blob);
        book.setDescription(clob);
        //存进数据库
        session.beginTransaction();
        session.save(book);
        session.getTransaction().commit();
        session.close();
    }
}
![复制代码](http://common.cnblogs.com/images/copycode.gif)
```
      hibernate4中得到session对象的方法和hibernate3中略有不同，但用hibernate3的写法也是可以的。
      在hibernate4中 hibernate.createBlob() 方法和 hibernate.createClob() 已经过时，取而代之的是LobHelper.createBlob()和lobHelper.createClob(）。可以通过session.getLobHelper();得到LobHerlper对象。
取值 和hibernate3 基本一样，这里不再演示！
