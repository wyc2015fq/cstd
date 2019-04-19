# MyBatis中resultType和resultMap的区别 - 零度的博客专栏 - CSDN博客
2017年09月11日 09:31:08[零度anngle](https://me.csdn.net/zmx729618)阅读数：5492
       MyBatis中在查询进行select映射的时候，返回类型可以用resultType，也可以用resultMap，resultType是直接表示返回类型的(对应着我们的model对象中的实体)，而resultMap则是对外部ResultMap的引用(提前定义了db和model之间的隐射key-->value关系)，但是resultType跟resultMap不能同时存在。
       在MyBatis进行查询映射时，其实查询出来的每一个属性都是放在一个对应的Map里面的，其中键是属性名，值则是其对应的值。
       ①当提供的返回类型属性是resultType时，MyBatis会将Map里面的键值对取出赋给resultType所指定的对象对应的属性。所以其实MyBatis的每一个查询映射的返回类型都是ResultMap，只是当提供的返回类型属性是resultType的时候，MyBatis对自动的给把对应的值赋给resultType所指定对象的属性。
        ②当提供的返回类型是resultMap时，因为Map不能很好表示领域模型，就需要自己再进一步的把它转化为对应的对象，这常常在复杂查询中很有作用。
       下面给出一个例子说明两者的使用差别：
```java
package com.clark.model;
import java.util.Date;
public class Goods {
	private Integer id;
	private Integer cateId;
	private String name;
	private double price;
	private String description;
	private Integer orderNo;
	private Date updateTime;
	
	public Goods(){
		
	}
	
	public Goods(Integer id, Integer cateId, String name, double price,
			String description, Integer orderNo, Date updateTime) {
		super();
		this.id = id;
		this.cateId = cateId;
		this.name = name;
		this.price = price;
		this.description = description;
		this.orderNo = orderNo;
		this.updateTime = updateTime;
	}
	public Integer getId() {
		return id;
	}
	public void setId(Integer id) {
		this.id = id;
	}
	public Integer getCateId() {
		return cateId;
	}
	public void setCateId(Integer cateId) {
		this.cateId = cateId;
	}
	public String getName() {
		return name;
	}
	public void setName(String name) {
		this.name = name;
	}
	public double getPrice() {
		return price;
	}
	public void setPrice(double price) {
		this.price = price;
	}
	public String getDescription() {
		return description;
	}
	public void setDescription(String description) {
		this.description = description;
	}
	public Integer getOrderNo() {
		return orderNo;
	}
	public void setOrderNo(Integer orderNo) {
		this.orderNo = orderNo;
	}
	public Date getTimeStamp() {
		return updateTime;
	}
	public void setTimeStamp(Date updateTime) {
		this.updateTime = updateTime;
	}
	@Override
	public String toString() {
		return "[goods include:Id="+this.getId()+",name="+this.getName()+
				",orderNo="+this.getOrderNo()+",cateId="+this.getCateId()+
				",updateTime="+this.getTimeStamp()+"]";
	}
}
```
```
<?xml version="1.0" encoding="UTF-8"?>
<!DOCTYPE configuration PUBLIC "-//mybatis.org//DTD Config 3.0//EN"
	"http://mybatis.org/dtd/mybatis-3-config.dtd">
<configuration>
	<typeAliases>
		<!-- give a alias for model -->
		<typeAlias alias="goods" type="com.clark.model.Goods"></typeAlias>
	</typeAliases>
	<environments default="development">
		<environment id="development">
			<transactionManager type="JDBC" />
			<dataSource type="POOLED">
				<property name="driver" value="oracle.jdbc.driver.OracleDriver" />
				<property name="url" value="jdbc:oracle:thin:@172.30.0.125:1521:oradb01" />
				<property name="username" value="settlement" />
				<property name="password" value="settlement" />
			</dataSource>
		</environment>
	</environments>
	<mappers>
		<mapper resource="com/clark/model/goodsMapper.xml" />
	</mappers>
</configuration></span>
```
```
<?xml version="1.0" encoding="UTF-8"?>
<!DOCTYPE mapper PUBLIC "-//mybatis.org//DTD Mapper 3.0//EN"
	"http://mybatis.org/dtd/mybatis-3-mapper.dtd">
<mapper namespace="clark">
	<resultMap type="com.clark.model.Goods" id="t_good">
		<id column="id" property="id"/>
		<result column="cate_id" property="cateId"/>
		<result column="name" property="name"/>
		<result column="price" property="price"/>
		<result column="description" property="description"/>
		<result column="order_no" property="orderNo"/>
		<result column="update_time" property="updateTime"/>
	</resultMap>
	<!--resultMap 和   resultType的使用区别-->
	<select id="selectGoodById" parameterType="int" resultType="goods">
		select id,cate_id,name,price,description,order_no,update_time 
		from goods where id = #{id}
	</select>
	
	<select id="selectAllGoods" resultMap="t_good">
		select id,cate_id,name,price,description,order_no,update_time from goods
	</select>
	
	<insert id="insertGood" parameterType="goods">
		insert into goods(id,cate_id,name,price,description,order_no,update_time)  
		values(#{id},#{cateId},#{name},#{price},#{description},#{orderNo},#{updateTime})
	</insert>
</mapper>
```
```
package com.clark.mybatis;
import java.io.IOException;
import java.io.Reader;
import java.util.List;
import org.apache.ibatis.io.Resources;
import org.apache.ibatis.session.SqlSession;
import org.apache.ibatis.session.SqlSessionFactory;
import org.apache.ibatis.session.SqlSessionFactoryBuilder;
import com.clark.model.Goods;
public class TestGoods {
	public static void main(String[] args) {
		String resource = "configuration.xml";
		try {
			Reader reader = Resources.getResourceAsReader(resource);
			SqlSessionFactory sessionFactory = new SqlSessionFactoryBuilder().build(reader);
			SqlSession session = sessionFactory.openSession();
                        
                        //使用resultType的情况
			Goods goods = (Goods)session.selectOne("clark.selectGoodById", 4);
			System.out.println(goods.toString());</span>
                       //使用resultMap的情况
			List<Goods> gs = session.selectList("clark.selectAllGoods");
			for (Goods goods2 : gs) {
				System.out.println(goods2.toString());
			}
			Goods goods = new Goods(4, 12, "clark", 12.30, "test is ok", 5, new Date());
			session.insert("clark.insertGood", goods);
			session.commit();
		} catch (IOException e) {
			e.printStackTrace();
		}
	}
}
```
结果输出为:
---使用resultType的结果---
[goods include:Id=4,name=clark,orderNo=null,cateId=null,updateTime=null]
---使用resultMap的结果---
[goods include:Id=4,name=clark,orderNo=5,cateId=12,updateTime=Wed Sep 17 15:29:58 CST 2014][goods include:Id=1,name=诺基亚N85,orderNo=1,cateId=1,updateTime=Wed Sep 17 13:52:51
 CST 2014]
[goods include:Id=2,name=金立 A30,orderNo=2,cateId=1,updateTime=Wed Sep 17 13:53:11 CST 2014][goods include:Id=3,name=金立 A30,orderNo=3,cateId=2,updateTime=Wed Sep 17 15:07:38 CST 2014]
