# Spring集成MongoDB - 零度的博客专栏 - CSDN博客
2016年11月14日 09:36:53[零度anngle](https://me.csdn.net/zmx729618)阅读数：579
## 开篇
`    Spring提供了对非关系数据库的集成，Spring的spring-data包提供了对mongoDB和redis集成的工具包。此篇博客介绍一下Spring和MongoDB的集成。`
## 导入工具包
`    需要的工具包有spring的jar包、spring-common的jar包以及spring-data的mongoDB的jar包。其中，spring的jar包有aop、asm、aspects、beans、context、core、expression、jdbc、transaction等。spring-common的jar包有commons-beanutils、commons-codec、commons-collections、commons-io、commons-lang、commons-logging、commons-pool等。spring-data的mongoDB的jar包有spring-data-commons-aspects、spring-data-commons-core、spring-data-document-core、spring-data-mongodb、spring-data-mongodb-cross-store、spring-data-mongodb-log4j等。另外还有一个是mongo.jar`
## 工具包截图
![这里写图片描述](https://img-blog.csdn.net/20151028115904114)
## 配置文件
```xml
<?xml version="1.0" encoding="UTF-8"?>
<beans xmlns="http://www.springframework.org/schema/beans"  
          xmlns:xsi="http://www.w3.org/2001/XMLSchema-instance"  
          xmlns:context="http://www.springframework.org/schema/context"  
          xmlns:mongo="http://www.springframework.org/schema/data/mongo"  
          xsi:schemaLocation="http://www.springframework.org/schema/context   
          http://www.springframework.org/schema/context/spring-context-3.0.xsd   
          http://www.springframework.org/schema/data/mongo   
          http://www.springframework.org/schema/data/mongo/spring-mongo-1.0.xsd   
          http://www.springframework.org/schema/beans   
          http://www.springframework.org/schema/beans/spring-beans-3.0.xsd">   
        <mongo:mongo host="192.168.0.138" port="27017"/>
       <bean id="mongoTemplate" class="org.springframework.data.document.mongodb.MongoTemplate">   
        <constructor-arg ref="mongo"/>   
        <constructor-arg name="databaseName" value="db"/>   
        <constructor-arg name="defaultCollectionName" value="person" />   
      </bean>   
     <bean id="personRepository" class="com.mongo.dao.impl.PersonRepository">   
        <property name="mongoTemplate" ref="mongoTemplate"></property>   
    </bean>   
     <context:annotation-config />
</beans>
```
## 代码编写
实体类：
```java
public class Person implements Serializable {
    /**
     * 
     */
    private static final long serialVersionUID = 3617931430808763429L;
    private String id;   
    private String name;   
    private int age;
    public Person() {
        super();
    }
    public Person(String id, String name, int age) {
        super();
        this.id = id;
        this.name = name;
        this.age = age;
    }
    /**
     * @return the id
     */
    public String getId() {
        return id;
    }
    /**
     * @param id the id to set
     */
    public void setId(String id) {
        this.id = id;
    }
    /**
     * @return the name
     */
    public String getName() {
        return name;
    }
    /**
     * @param name the name to set
     */
    public void setName(String name) {
        this.name = name;
    }
    /**
     * @return the age
     */
    public int getAge() {
        return age;
    }
    /**
     * @param age the age to set
     */
    public void setAge(int age) {
        this.age = age;
    }
    /**
     * 
     * @param name
     * @param age
     */
    public Person(String name, int age) {
        super();
        this.name = name;
        this.age = age;
    }   
     public String toString() {   
            return "Person[id="+id+",name="+name+",age="+age+"]";   
        }   
}
```
DAO层接口：
``
```java
public interface AbstractRepository {
    public void insert(Person person); 
    public Person findOne(String id);   
    public List<Person> findAll();   
    public List<Person> findByRegex(String regex);
    public void removeOne(String id);   
    public void removeAll();   
    public void findAndModify(String id);   
}
```
DAO层接口实现：
```
public class PersonRepository implements AbstractRepository {
      private MongoTemplate mongoTemplate;   
    @Override
    public List<Person> findAll() {
        return getMongoTemplate().find(new Query(), Person.class);   
    }
    @Override
    public void findAndModify(String id) {
        getMongoTemplate().updateFirst(new Query(Criteria.where("id").is(id)), new Update().inc("age", 3));
    }
    @Override
    public List<Person> findByRegex(String regex) {
         Pattern pattern = Pattern.compile(regex,Pattern.CASE_INSENSITIVE);   
          Criteria criteria = new Criteria("name").regex(pattern.toString());   
            return getMongoTemplate().find(new Query(criteria), Person.class);   
    }
    @Override
    public Person findOne(String id) {
         return getMongoTemplate().findOne(new Query(Criteria.where("id").is(id)), Person.class);   
    }
    @Override
    public void insert(Person person) {
        getMongoTemplate().insert(person);   
    }
    @Override
    public void removeAll() {
        List<Person> list = this.findAll();   
        if(list != null){   
            for(Person person : list){   
                getMongoTemplate().remove(person);   
            }   
        }   
    }
    @Override
    public void removeOne(String id) {
        Criteria criteria = Criteria.where("id").in(id);   
        if(criteria == null){   
             Query query = new Query(criteria);   
             if(query != null && getMongoTemplate().findOne(query, Person.class) != null)   
                 getMongoTemplate().remove(getMongoTemplate().findOne(query, Person.class));   
        }   
    }
    /**
     * @return the mongoTemplate
     */
    public MongoTemplate getMongoTemplate() {
        return mongoTemplate;
    }
    /**
     * @param mongoTemplate the mongoTemplate to set
     */
    public void setMongoTemplate(MongoTemplate mongoTemplate) {
        this.mongoTemplate = mongoTemplate;
    }
}
```
```java
测试程序：
```
```java
```
```
public class MongoTest {
    private static Log log = LogFactory.getLog(MongoTest.class.getName());
    private  AbstractRepository pr=null;
    public void init(){
         ApplicationContext ctx = new ClassPathXmlApplicationContext("applicationContext.xml");
          pr= (PersonRepository)ctx.getBean("personRepository");
    }
    public void insert(){
        Person p=new Person("cuiran",27);
         pr.insert(p);
    }
    public void findOne(){
        String id="50c83cb552c2ceb0463177d6";
        Person p= pr.findOne(id);
        log.debug(p);
    }
    public void listAll(){
        List<Person> list=pr.findAll();
        for (Person p:list){
            log.debug(p.toString());
        }
    }
    public void start(){
        init();
        //insert();
        //listAll();
        findOne();
    }
    public static void main(String[] args) {
        MongoTest t=new MongoTest();
        t.start();
    }
}
```
```java
```
## 结束
`      通过Spring集成MongoDB的底层代码封装，项目中用到非关系型数据MongoDB存储就变得容易很多。`
