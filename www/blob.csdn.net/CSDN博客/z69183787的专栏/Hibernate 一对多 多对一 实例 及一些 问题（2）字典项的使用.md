# Hibernate 一对多 多对一 实例 及一些 问题（2）字典项的使用 - z69183787的专栏 - CSDN博客
2014年04月03日 20:24:59[OkidoGreen](https://me.csdn.net/z69183787)阅读数：1172
有时在多对一时，只有插入时需要用到 外键值（即一对象的主键），而查询时需要得到完整对象
```java
/**
 * 
 */
package com.wonders.task.sample.bo;
import javax.persistence.CascadeType;
import javax.persistence.Column;
import javax.persistence.Entity;
import javax.persistence.FetchType;
import javax.persistence.GeneratedValue;
import javax.persistence.Id;
import javax.persistence.JoinColumn;
import javax.persistence.ManyToOne;
import javax.persistence.Table;
import javax.persistence.Transient;
import org.hibernate.annotations.Cascade;
import org.hibernate.annotations.GenericGenerator;
import org.hibernate.annotations.Where;
/** 
 * @ClassName: Student 
 * @Description: TODO(这里用一句话描述这个类的作用) 
 * @author zhoushun 
 * @date 2014年2月27日 上午11:48:30 
 *  
 */
@Entity 
@Table(name="Z_STUDENT") 
public class Student {
	private String id;
	private String name;
	private Clazz clazz;
	private String classId;
	
	@Id
	@GeneratedValue(generator="system-uuid")
	@GenericGenerator(name="system-uuid", strategy = "uuid")
	@Column(name = "ID")
	public String getId() {
		return id;
	}
	public void setId(String id) {
		this.id = id;
	}
	
	@Column(name = "NAME", length = 500)
	public String getName() {
		return name;
	}
	public void setName(String name) {
		this.name = name;
	}
	
	//@ManyToOne(cascade={CascadeType.PERSIST,CascadeType.MERGE}) 
	//@ManyToOne(cascade={CascadeType.ALL},fetch=FetchType.LAZY) 
	//@JoinColumn(name="class_id")
  	@ManyToOne(cascade = CascadeType.REFRESH, fetch=FetchType.EAGER)
  	//@Cascade(value=org.hibernate.annotations.CascadeType.SAVE_UPDATE)
	@JoinColumn(name="class_id" ,nullable = true,insertable=false,updatable=false)//这2个属性保证 不会插入对象，只对查询有效
     //student类中对应外键的属性：classid 
	//@Where(clause = "removed=0")
	public Clazz getClazz() {
		return clazz;
	}
	public void setClazz(Clazz clazz) {
		this.clazz = clazz;
	}
	
	//@Transient
	@Column(name = "CLASS_ID", length = 50)
	public String getClassId() {
		return classId;
	}
	public void setClassId(String classId) {
		this.classId = classId;
	}
	
	
}
```
**insertable=false,updatable=false 只对查询有效**
**单向多对一，”一方“ 无需设置 onetomany**
在bo内同时增加 外键字段 及 一的bo对象
插入时，从前台得到 clazzId 手动设置插入，而查询时通过取消延时加载 直接获取一方的 bo 对象。
这在 一 方 为 特定 字典项 表时 比较有用，可以避免 new 对象 及 load 对象。
