# Jaxb多层 java 与 xml 转换 - z69183787的专栏 - CSDN博客
2014年01月14日 14:22:19[OkidoGreen](https://me.csdn.net/z69183787)阅读数：4203
```java
/**
 * 
 */
package com.wonders.quartz.cocc.model.xml;
import java.util.List;
import javax.xml.bind.annotation.XmlAttribute;
import javax.xml.bind.annotation.XmlElement;
import javax.xml.bind.annotation.XmlRootElement;
import com.wonders.quartz.cocc.model.vo.CoccListVo;
import com.wonders.quartz.cocc.model.vo.CoccReportVo;
/** 
 * @ClassName: CoccReportXml 
 * @Description: TODO(这里用一句话描述这个类的作用) 
 * @author zhoushun 
 * @date 2013年12月10日 下午2:31:33 
 *  
 */
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
/**
 * 
 */
package com.wonders.quartz.cocc.model.vo;
import java.util.List;
import javax.xml.bind.annotation.XmlAccessType;
import javax.xml.bind.annotation.XmlAccessorType;
import javax.xml.bind.annotation.XmlElement;
import javax.xml.bind.annotation.XmlRootElement;
/** 
 * @ClassName: CoccReport 
 * @Description: TODO(这里用一句话描述这个类的作用) 
 * @author zhoushun 
 * @date 2013年12月10日 下午2:12:33 
 *  
 */
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
最终显示xml字符串的节点名称 以外部包含该类的节点上的名称为准。
XML：
```
<?xml version="1.0" encoding="GBK" standalone="yes"?>
<root date="2014-01-13" type="coccMetroWeekReport">
    <list>
        <cocc>
            <id>54844</id>
            <title>上海轨道交通网络运营生产周报（2014年1月6日—2014年1月12日）</title>
            <source>COCC</source>
            <createTime>2014-01-13</createTime>
            <publishTime>2014-01-13</publishTime>
            <url>http://10.1.44.18/stfb/node393/node1254/201401/con1050155.htm</url>
        </cocc>
    </list>
</root>
```
XML：
