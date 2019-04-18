# Java实现数据序列化工具Avro的例子 - 专注于数据挖掘算法研究和应用 - CSDN博客





2016年12月27日 10:14:14[fjssharpsword](https://me.csdn.net/fjssharpsword)阅读数：3824








                
1、Avro简介

Avro是一个数据序列化的系统。

它可以提供：

1)丰富的数据结构类型

2)快速可压缩的二进制数据形式

3)存储持久数据的文件容器

4)远程过程调用RPC

5)简单的动态语言结合功能，Avro和动态语言结合后，读写数据文件和使用RPC协议都不需要生成代码，而代码生成作为一种可选的优化只值得在静态类型语言中实现。



Avro依赖于模式(Schema)。Avro数据的读写操作是很频繁的，而这些操作都需要使用模式，这样就减少写入每个数据资料的开销，使得序列化快速而又轻巧。这种数据及其模式的自我描述方便于动态脚本语言的使用。

当Avro数据存储到文件中时，它的模式也随之存储，这样任何程序都可以对文件进行处理。如果需要以不同的模式读取数据，这也很容易解决，因为两个模式都是已知的。

当在RPC中使用Avro时，服务器和客户端可以在握手连接时交换模式。服务器和客户端有着彼此全部的模式，因此相同命名字段、缺失字段和多余字段等信息之间通信中需要解决的一致性问题就可以容易解决

还有，Avro模式是用JSON（一种轻量级的数据交换模式）定义的，这样对于已经拥有JSON库的语言可以容易实现。

Avro提供着与诸如Thrift和Protocol Buffers等系统相似的功能，但是在一些基础方面还是有区别的，主要是：

1)动态类型：Avro并不需要生成代码，模式和数据存放在一起，而模式使得整个数据的处理过程并不生成代码、静态数据类型等等。这方便了数据处理系统和语言的构造。

2)未标记的数据：由于读取数据的时候模式是已知的，那么需要和数据一起编码的类型信息就很少了，这样序列化的规模也就小了。

3)不需要用户指定字段号：即使模式改变，处理数据时新旧模式都是已知的，所以通过使用字段名称可以解决差异问题。





2、Java例子

1）下载avro jar包：

   下载地址 http://www.trieuvan.com/apache/avro/avro-1.8.1/java/

   下载avro-1.8.1.jar and avro-tools-1.8.1.jar两个jar包，引入工程；

2）代码：创建模式-输入记录-根据模式获取记录



```java
package sk.ann;

import java.io.File;
import java.io.IOException;

import org.apache.avro.Schema;
import org.apache.avro.file.DataFileReader;
import org.apache.avro.file.DataFileWriter;
import org.apache.avro.generic.GenericData;
import org.apache.avro.generic.GenericData.Record;
import org.apache.avro.generic.GenericDatumReader;
import org.apache.avro.generic.GenericDatumWriter;
public class AvroDemo {
	String fileName="D:\\tmp\\student.db";
	String prefix="{\"type\":\"record\",\"name\":\"student\",\"fields\":[";
	String suffix="]}";
	String fieldSID="{\"name\":\"SID\",\"type\":\"int\"}";
	String fieldName="{\"name\":\"Name\",\"type\":\"string\"}";
	String fieldDept="{\"name\":\"Dept\",\"type\":\"string\"}";
	String fieldPhone="{\"name\":\"Phone\",\"type\":\"string\"}";
	String fieldAge="{\"name\":\"Age\",\"type\":\"int\"}";
	Schema studentSchema=Schema.parse(prefix+fieldSID+","+fieldName+","+fieldDept+","+fieldPhone+","+fieldAge+suffix);
	Schema extractSchema=Schema.parse(prefix+fieldName+","+fieldPhone+suffix);
	int SID=0;
	
	public static void main(String[] args) throws IOException{
		AvroDemo avd=new AvroDemo();
		avd.init();
		avd.print();
		avd.printExtraction();
	}
	
	//初始化学生记录
	public void init() throws IOException{
		DataFileWriter<Record> writer=new DataFileWriter<Record>(
				new GenericDatumWriter<Record>(studentSchema)).create(studentSchema,new File(fileName));
		try{
			writer.append(createStudent("zhanghua","Law","15201161111",25));
			writer.append(createStudent("Lili","Economy","15201162222",24));
			writer.append(createStudent("Wangyu","Infomation","15201163333",23));
			writer.append(createStudent("Zhaoxin","Art","15201164444",25));
			writer.append(createStudent("Sunqin","Physics","15201165555",23));
			writer.append(createStudent("Zhouping","Math","15201166666",23));
		}finally{
			writer.close();
		}
	}
	
	//添加学生记录
	private Record createStudent(String name,String dept,String phone,int age){
		Record student=new GenericData.Record(studentSchema);
		student.put("SID", (++SID));
		student.put("Name", name);
		student.put("Dept", dept);
		student.put("Phone",phone);
		student.put("Age",age);
		System.out.println("successfully added"+name);
		return student;
	}
	
	//输出学生信息
	public void print() throws IOException{
		GenericDatumReader<Record> dr=new GenericDatumReader<Record>();
		dr.setExpected(studentSchema);
		DataFileReader<Record> reader=new DataFileReader<Record>(new File(fileName),dr);
		System.out.println("\n print all the records from database");
		try{
			while(reader.hasNext()){
				Record student=reader.next();
				System.out.print(student.get("SID").toString()+" "+student.get("Name")+" "+student.get("Dept")+" "+student.get("Phone")+" "+student.get("Age").toString()+"\r\n");
			}
		}finally{
			reader.close();
		}
	}
	
	//输出学生姓名和电话
	public void printExtraction() throws IOException{
		GenericDatumReader<Record> dr=new GenericDatumReader<Record>();
		dr.setExpected(extractSchema);
		DataFileReader<Record> reader=new DataFileReader<Record>(new File(fileName),dr);
		System.out.println("\n Extract Name&Phone of the records from database");
		try{
			while(reader.hasNext()){
				Record student=reader.next();
				System.out.print(student.get("Name")+" "+student.get("Phone")+"\r\n");
			}
		}finally{
			reader.close();
		}	
	}
	
}
```

执行结果：



```
successfully addedzhanghua
successfully addedLili
successfully addedWangyu
successfully addedZhaoxin
successfully addedSunqin
successfully addedZhouping

 print all the records from database
1 zhanghua Law 15201161111 25
2 Lili Economy 15201162222 24
3 Wangyu Infomation 15201163333 23
4 Zhaoxin Art 15201164444 25
5 Sunqin Physics 15201165555 23
6 Zhouping Math 15201166666 23

 Extract Name&Phone of the records from database
zhanghua 15201161111
Lili 15201162222
Wangyu 15201163333
Zhaoxin 15201164444
Sunqin 15201165555
Zhouping 15201166666
```








