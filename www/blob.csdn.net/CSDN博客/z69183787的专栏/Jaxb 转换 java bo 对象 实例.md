# Jaxb 转换 java bo 对象 实例 - z69183787的专栏 - CSDN博客
2014年03月10日 16:16:47[OkidoGreen](https://me.csdn.net/z69183787)阅读数：793
具体jaxb各属性参照：
[http://blog.csdn.net/z69183787/article/details/12970523](http://blog.csdn.net/z69183787/article/details/12970523)
```java
@XmlRootElement(name = "root")
public class CoccReportXml {
	@XmlAttribute(name = "type")
	public String type="coccMetroWeekReport";
	@XmlAttribute(name = "date")
	public String date= new java.text.SimpleDateFormat("yyyy-MM-dd").format(new java.util.Date());
	@XmlElement(name = "list")
	public CoccListVo list;
	
	
}
```
```java
@XmlRootElement(name = "bbbb")
@XmlAccessorType(XmlAccessType.FIELD)
public class CoccListVo {
	@XmlElement(name = "cocc")
	public List<CoccReportVo> list;
	
}
```
```java
/**
 * 
 */
package com.wonders.quartz.cocc.model.vo;
import javax.xml.bind.annotation.XmlAccessType;
import javax.xml.bind.annotation.XmlAccessorType;
import javax.xml.bind.annotation.XmlRootElement;
/** 
 * @ClassName: CoccReport 
 * @Description: TODO(这里用一句话描述这个类的作用) 
 * @author zhoushun 
 * @date 2013年12月10日 下午2:12:33 
 *  
 */
@XmlRootElement(name = "123")
@XmlAccessorType(XmlAccessType.FIELD)
public class CoccReportVo {
	public String id;
	public String title;
	public String source;
	public String createTime;
	public String publishTime;
	public String url;
	public String getId() {
		return id;
	}
	public void setId(String id) {
		this.id = id;
	}
	public String getTitle() {
		return title;
	}
	public void setTitle(String title) {
		this.title = title;
	}
	public String getSource() {
		return source;
	}
	public void setSource(String source) {
		this.source = source;
	}
	public String getCreateTime() {
		return createTime;
	}
	public void setCreateTime(String createTime) {
		this.createTime = createTime;
	}
	public String getPublishTime() {
		return publishTime;
	}
	public void setPublishTime(String publishTime) {
		this.publishTime = publishTime;
	}
	public String getUrl() {
		return url;
	}
	public void setUrl(String url) {
		this.url = url;
	}
	
	
}
```
```java
List<CoccReportVo> list = this.service.getResult(sql);
		CoccReportXml xml = new CoccReportXml();
		CoccListVo v = new CoccListVo();
		v.list = list ;
		xml.list = v;
		String result = "";
		try{
			StringWriter writer = new StringWriter();
			JAXBContext context = JAXBContext.newInstance(CoccReportXml.class);
			Marshaller m = context.createMarshaller();
			m.setProperty(Marshaller.JAXB_FORMATTED_OUTPUT, true); 
			m.setProperty(Marshaller.JAXB_ENCODING, "GBK"); //防止文件中文乱码
			m.marshal(xml, writer);
			result = writer.toString();
		}catch(Exception e){
			result = "none";
		}
```
xml：
```java
<?xml version="1.0" encoding="GBK" standalone="yes"?>
<root date="2014-03-10" type="coccMetroWeekReport">
    <list>
        <cocc>
            <id>58044</id>
            <title>上海轨道交通网络运营生产周报（2014年3月3日—2014年3月9日）</title>
            <source>COCC</source>
            <createTime>2014-03-10</createTime>
            <publishTime>2014-03-10</publishTime>
            <url>http://10.1.44.18/stfb/node393/node1254/201403/con1053363.htm</url>
        </cocc>
    </list>
</root>
```
