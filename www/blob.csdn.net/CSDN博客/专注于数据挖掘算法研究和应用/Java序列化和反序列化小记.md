# Java序列化和反序列化小记 - 专注于数据挖掘算法研究和应用 - CSDN博客





2016年08月16日 14:58:31[fjssharpsword](https://me.csdn.net/fjssharpsword)阅读数：2097








                
序列化：将java对象转换为字节序列的过程叫做序列化

反序列化：将字节对象转换为java对象的过程叫做反序列化


通常情况下，序列化有两种用途：


1) 把对象的字节序列永久的保存在硬盘中


2）在网络上传输对象的字节序列


相应的ＡＰＩ

1)java.io.ObjectOutputStream.writeObject(Object obj)

2)java.io.ObjectInputStream.readObject()


只有实现了Serializable或者Externalizable接口的类的对象才能够被序列化。




1、序列化类



```java
package ct.data;

import java.io.Serializable;

/*
 * 把对象转换为字节序列的过程称为对象的序列化。把字节序列恢复为对象的过程称为对象的反序列化。
 * 对象的序列化主要有两种用途：
 * 1） 把对象的字节序列永久地保存到硬盘上，通常存放在一个文件中，用于保存到硬盘；
 * 2） 在网络上传送对象的字节序列，应用于两个进程在进行远程通信，类似C++结构体作用；
 */
public class TupleInfo implements Serializable{
	private String UserNo;
	private String Lac;
	private String Ci;
	private String StartTime;
	private String LongStay;
	
	public void setUserNo(String strUserNo){
		this.UserNo=strUserNo;
	}
	public String getUserNo(){
		return UserNo;
	}
	
	public void setLac(String strLac){
		this.Lac=strLac;
	}
	public String getLac(){
		return Lac;
	}
	
	public void setCi(String strCi){
		this.Ci=strCi;
	}
	public String getCi(){
		return Ci;
	}
	
	public void setStartTime(String strStartTime){
		this.StartTime=strStartTime;
	}
	public String getStartTime(){
		return StartTime;
	}
	
	public void setLongStay(String strLongStay){
		this.LongStay=strLongStay;
	}
	public String getLongStay(){
		return LongStay;
	}
}
```



2、测试



```java
package ct.data;

import java.io.File;
import java.io.FileInputStream;
import java.io.FileNotFoundException;
import java.io.FileOutputStream;
import java.io.IOException;
import java.io.ObjectInputStream;
import java.io.ObjectOutputStream;
import java.text.MessageFormat;

public class TupleInfoTest {
	
	public static void main(String[] args) throws Exception {
        SerializeTupleInfo();//序列化TupleInfo对象
        TupleInfo tuple = DeserializeTupleInfo();//反序列TupleInfo对象
        System.out.println(MessageFormat.format("userno={0},lac={1},ci={2},starttime={3},longstay={4}",tuple.getUserNo(), tuple.getLac(), tuple.getCi(),tuple.getStartTime(),tuple.getLongStay()));
    }
    
    /**
     * MethodName: SerializePerson 
     * Description: 序列化TupleInfo对象
     */
    private static void SerializeTupleInfo() throws FileNotFoundException,IOException {
    	TupleInfo tuple = new TupleInfo();
    	tuple.setUserNo("123");
    	tuple.setLac("DE");
    	tuple.setCi("FG");
    	tuple.setStartTime("20160808080808");
    	tuple.setLongStay("300");//秒
        // ObjectOutputStream 对象输出流，将TupleInfo对象存储到D盘tmp目录下的tuple.txt文件中，完成对TupleInfo对象的序列化操作
        ObjectOutputStream oo = new ObjectOutputStream(new FileOutputStream(new File("D:/tmp/tuple.obj")));
        oo.writeObject(tuple);
        System.out.println("TupleInfo对象序列化成功！");
        oo.close();
    }

    /**
     * MethodName: DeserializePerson 
     * Description: 反序列TupleInfo对象
     */
    private static TupleInfo DeserializeTupleInfo() throws Exception, IOException {
        ObjectInputStream ois = new ObjectInputStream(new FileInputStream(new File("D:/tmp/tuple.obj")));
        TupleInfo tuple = (TupleInfo) ois.readObject();
        System.out.println("TupleInfo对象反序列化成功！");
        return tuple;
    }
}
```





