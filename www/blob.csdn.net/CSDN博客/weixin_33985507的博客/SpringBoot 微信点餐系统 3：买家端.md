# SpringBoot 微信点餐系统 3：买家端 - weixin_33985507的博客 - CSDN博客
2018年03月07日 07:18:00[weixin_33985507](https://me.csdn.net/weixin_33985507)阅读数：8
> 
配置数据库及实现 Hibernate 的查改增删
# 1. pom.xml 新增数据库的相关依赖
```
<dependency>
            <groupId>mysql</groupId>
            <artifactId>mysql-connector-java</artifactId>
        </dependency>
        <dependency>
            <groupId>org.springframework.boot</groupId>
            <artifactId>spring-boot-starter-data-jpa</artifactId>
        </dependency>
```
# 2. 配置数据库的信息
```
spring:
  datasource;
      username: root
      driver-class-name:  com.mysql.jdbc.Driver
      password: 123456
      url: jdbc:mysql://192.168.31.157:3306/wxorder_sell?characterEncoding=utf-8&useSSL=false
  jpa:
    show-sql:true
```
# 3. 配置 entity，映射 mysql 表结构
新建 dataobject/ProductCategory
Note： 不需要把所有的表字段映射到类属性，比如 createTime / updateTime ，这种不写也照常运行。
```
//映射 mysql
@Entity 
// 当类名与表名不一致时，使用此注解
@Table(name = "wxorder_product_category")
// lombok 的注解，使用此注解自动添加 get/set 方法
@Data
//@DynamicUpdate
public class ProductCategory{
      
      @Id
      @GenerateValue
      //@GenerateValue(strategy = GenerationType.IDENTITY)
      
      private Integer categoryId;
      private String categoryName;
      private Integer categoryType;
      //private Date createTime;
      // private Date updateTIme;
}
```
# 4. 配置DAO层
新建 repository/ProductCategoryRepository
```
// Integer 是 ProductCategory 映射的表的主键类型
public interface ProductCategoryRepositoy extends JpaRepository<ProductCategory, Integer>
```
# 5. 测试 Hibernate 的查改增删
右键 ProductCategoryRepository , goto，自动创建ProductCategoryRepositoryTest
#### （1）查询
```
// 这两个注解必填
@RunWith(SpringRunner.class)
@SpringBootTest
public class ProductCategoryRepositoryTest{
    // 接口对接了许多方法，所以必须引入
    @AutoWired
    private ProductCategoryRepository repository;
    @Test
    public void findOneTest(){
          Optional<ProductCategory> productCategory = repository.findById(1);
          System.out.println(productCategory);
    }
}
```
# 版本问题踩的坑
- 原来根据主键查询，findOne,现在是
```
Optional<ProductCategory> productCategory = repository.findById(1);
ProductCategory productCategory = repository.findById(1).get();
```
如果依旧使用findOne()的写法，会报错
```
Inferred type 'S' for type parameter 'S' is not within its bound; should extends com.example.demo.Girl
```
这类错误
- java9 的错误，由于模块化导致了一些错误
```
Caused by: java.lang.ClassNotFoundException: javax.xml.bind.JAXBException
```
需要在 maven 中添加一些依赖
```
<dependency>
            <groupId>javax.xml.bind</groupId>
            <artifactId>jaxb-api</artifactId>
            <version>2.2.11</version>
        </dependency>
        <dependency>
            <groupId>com.sun.xml.bind</groupId>
            <artifactId>jaxb-core</artifactId>
            <version>2.2.11</version>
        </dependency>
        <dependency>
            <groupId>com.sun.xml.bind</groupId>
            <artifactId>jaxb-impl</artifactId>
            <version>2.2.11</version>
        </dependency>
```
#### （2）插入/insert
```
@Test
public void insertTest(){
  ProductCategory productCategory = new ProductCategory();
  productCategory.setCategoryName("冬日甜品");
  productCategory.setCategoryType(7);
  repository.save(productCategory);
}
```
这时会抛出一个异常，具体的解释请看[Spring 中使用Hibernate](https://www.jianshu.com/p/8deadab53dce)，修改 ProductCategory 的 categoryId 的注解
```
@Id
@GeneratedValue(strategy=GenerationType.IDENTITY)
private Integer categoryId;
```
#### (3) update
update operation is like insert operation except update operation needs specify the primary key
- update directly
```
@Test
public void updateDirectly(){
  ProductCategory productCategory = new ProductCategory();
  productCategory.setCategoryId(2);
  productCategory.setCategoryName("圣诞特供");
  repository.save(productCategory); 
}
```
- 先查询再更新
这种操作很常见，在很多场景中我们需要先查询信息，然后根据查询的信息做一些判断然后再更新。
这部分主要是想讲注解 `@DynamicUpdate`的作用，所以先在 ProductCategory 上添加2个属性
```
private Date createTime;
private Date updateTime;
```
先注意一下表中id为2 的update time，然后进行更新操作
```
@Test
public void updateAfterQuery(){
    ProductCategory productCategory = repository.findById(2).get();
    productCategory.setCategoryType(20);
    repository.save(productCategory);
}
```
这时发现数据更新了，但是updateTime没有更新。sql 日志为
```
Hibernate: select productcat0_.category_id as category1_0_0_, productcat0_.category_name as category2_0_0_, productcat0_.category_type as category3_0_0_, productcat0_.create_time as create_t4_0_0_, productcat0_.update_time as update_t5_0_0_ from wxorder_product_category productcat0_ where productcat0_.category_id=?
Hibernate: select productcat0_.category_id as category1_0_0_, productcat0_.category_name as category2_0_0_, productcat0_.category_type as category3_0_0_, productcat0_.create_time as create_t4_0_0_, productcat0_.update_time as update_t5_0_0_ from wxorder_product_category productcat0_ where productcat0_.category_id=?
Hibernate: update wxorder_product_category set category_name=?, category_type=?, create_time=?, update_time=? where category_id=?
```
在 ProductCategory 上添加注解 `@DynamicUpdate`,再次执行，发现只有查询日志
```
Hibernate: select productcat0_.category_id as category1_0_0_, productcat0_.category_name as category2_0_0_, productcat0_.category_type as category3_0_0_, productcat0_.create_time as create_t4_0_0_, productcat0_.update_time as update_t5_0_0_ from wxorder_product_category productcat0_ where productcat0_.category_id=?
Hibernate: select productcat0_.category_id as category1_0_0_, productcat0_.category_name as category2_0_0_, productcat0_.category_type as category3_0_0_, productcat0_.create_time as create_t4_0_0_, productcat0_.update_time as update_t5_0_0_ from wxorder_product_category productcat0_ where productcat0_.category_id=?
```
这说明Hibernate 没有进行 update 操作，why？因为我们更新的数据和表中的数据相同，这说明当更新的数据和表中的数据相同的时候，Hibernate是不会进行操作的。
修改代码中更新的数据，
```
@Test
public void updateAferQuery(){
    ProductCategory productCategory = repository.findById(2).get();
    productCategory.setCategoryType(12);
    repository.save(productCategory);
}
```
这时再次观察sql日志
```
Hibernate: select productcat0_.category_id as category1_0_0_, productcat0_.category_name as category2_0_0_, productcat0_.category_type as category3_0_0_, productcat0_.create_time as create_t4_0_0_, productcat0_.update_time as update_t5_0_0_ from wxorder_product_category productcat0_ where productcat0_.category_id=?
Hibernate: select productcat0_.category_id as category1_0_0_, productcat0_.category_name as category2_0_0_, productcat0_.category_type as category3_0_0_, productcat0_.create_time as create_t4_0_0_, productcat0_.update_time as update_t5_0_0_ from wxorder_product_category productcat0_ where productcat0_.category_id=?
Hibernate: update wxorder_product_category set category_type=? where category_id=?
```
看，添加上`@DynamicUpdate`后，Hibernate 的更新语句发生了变化。
#### (4) 删除/delete
删除操作非常简单，只要删除就ok了
```
@Test
public void deleteTest(){
    repository.deleteById(1);
}
```
### (5) 执行完成后回滚
为了让数据库中不产生脏数据，可以使用`@Transactional`注解
```
@Test
@Transactional
public void transacational(){
    ProductCategory productCategory = new ProductCategory();
    productCategory.setCategoryName("男生最爱");
    productCategory.setCategoryType(123);
    repository.save(productCategory);
}
```
### (6) Hibernate 使用CategoryType 来查询一组数据
- 需要在接口里定义一个接口，注意此接口必须符合一定的格式
```
public interface ProductCategoryRepository extends JpaRepository<ProductCategory, Integer>{
      List<ProductCategory> findByCategoryTypeIn(List<Integer> categoryType);
}
```
- 测试方法
```
@Test
public void findByCategoryTypeInTest(){
      List<Integer> list = Arrays.asList(3, 70);
      List<ProductCategory> result =  repository.findByCategoryTypeIn(list);
      Assert.assertNotEquals(0, result.size());
}
```
