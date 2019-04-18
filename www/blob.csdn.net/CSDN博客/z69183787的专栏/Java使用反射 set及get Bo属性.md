# Java使用反射 set及get Bo属性 - z69183787的专栏 - CSDN博客
2013年11月21日 16:48:10[OkidoGreen](https://me.csdn.net/z69183787)阅读数：1443
```java
/**
 * 
 */
package xml;
import java.beans.PropertyDescriptor;
import java.io.StringWriter;
import java.lang.reflect.Field;
import java.lang.reflect.Method;
import javax.xml.bind.JAXBContext;
import javax.xml.bind.JAXBException;
import javax.xml.bind.Marshaller;
import javax.xml.bind.annotation.XmlAccessorType;
import javax.xml.bind.annotation.XmlAccessType;
import javax.xml.bind.annotation.XmlRootElement;
/** 
 * @ClassName: DocSend 
 * @Description: TODO(这里用一句话描述这个类的作用) 
 * @author zhoushun 
 * @date 2013-8-22 下午3:13:40 
 *  
 */
@XmlRootElement(name="zs")
@XmlAccessorType(XmlAccessType.FIELD)
public class DocSend {
	private String userName;
	private String age ;
	  
	
	  
	public String getUserName() {
		return userName;
	}
	public void setUserName(String userName) {
		this.userName = userName;
	}
	public String getAge() {
		return age;
	}
	public void setAge(String age) {
		this.age = age;
	}
	public static void main(String[] args) throws JAXBException, Exception {
		Field f = DocSend.class.getDeclaredField("age");
		System.out.println(f.getGenericType().toString());
		JAXBContext context1 = JAXBContext.newInstance(new Class[] { DocSend.class });
	      Marshaller m;
		DocSend bo = new DocSend();
		bo.setAge("21321");//bo.setUserName("dsadsa");
			m = context1.createMarshaller();
			 m.setProperty("jaxb.formatted.output", Boolean.valueOf(true));
		      m.setProperty("jaxb.encoding", "UTF-8");
		      StringWriter sw = new StringWriter();
	          m.marshal(bo, sw);
	          System.out.println(sw.toString());
		
	          Field[] fields = bo.getClass().getDeclaredFields();
	          //写数据
//	          for(Field ff : fields) {
//	        	  ff.setAccessible(true);
//	           PropertyDescriptor pd = new PropertyDescriptor(ff.getName(), bo.getClass());
//	           Method rM = pd.getReadMethod();//获得读方法
//	           Object num = (Object) rM.invoke(bo);
//	           if(num == null) {
//		           Method wM = pd.getWriteMethod();//获得写方法
//		           wM.invoke(bo, "1");//因为知道是int类型的属性，所以传个int过去就是了。。实际情况中需要判断下他的参数类型
//	           }
//	          }
	          
	          
	          for(Field ff : fields) {
	        	  ff.setAccessible(true);
	        	  
	           Object num = (Object) ff.get(bo);
	           if(num == null) {
	        	   ff.set(bo,"222");
	           }
	          }
	          
	         
	          m.marshal(bo, sw);
	          System.out.println(sw.toString());
	     
	}
}
```
