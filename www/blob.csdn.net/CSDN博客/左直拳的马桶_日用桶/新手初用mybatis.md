# 新手初用mybatis - 左直拳的马桶_日用桶 - CSDN博客
2018年12月19日 11:30:25[左直拳](https://me.csdn.net/leftfist)阅读数：135
近期用了下mybatis，感觉不错，比起hibernate来，好像简单不少。使用方法总结如下：
**一、代码结构**
要有实体类，映射类。映射在于决定如何访问数据库，实体类在于接收查询返回值。
![在这里插入图片描述](https://img-blog.csdnimg.cn/20181219102545438.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L2xlZnRmaXN0,size_16,color_FFFFFF,t_70)
**二、映射**
最关键的地方在于映射了吧。
我用的是spring boot，sql定义采用的是注解的方式。
```java
package api.mapper;
import api.entity.BootFull;
import api.entity.Fishboat_Radar;
import org.apache.ibatis.annotations.Mapper;
import org.apache.ibatis.annotations.Param;
import org.apache.ibatis.annotations.Select;
import org.springframework.stereotype.Repository;
import java.util.List;
@Repository
@Mapper
public interface FishboatMapper {
	//返回单表多条记录
    @Select("select * from fishboat_radar where sequence=(select max(sequence) from fishboat_radar)")
    List<Fishboat_Radar> getNewList();
	//返回多表多条记录
    @Select("select t1.*,\n" +
            "t3.imo,\n" +
            "t3.length,\n" +
            "t3.wide,\n" +
            "t3.mothershipmmsi, \n" +
            "t3.destination,\n" +
            "t3.vendorid,\n" +
            "t3.callsign,\n" +
            "t3.shipclass,\n" +
            "t3.shiptype,\n" +
            "t3.vesselname \n" +
            "from jczs.fishboat_radar t1\n" +
            "join (select max(sequence) as sequence from jczs.fishboat_radar) t2 on t1.sequence=t2.sequence\n" +
            "left outer join jczs.yb_hlx_ais t3 on t1.guid=t3.guid")
    List<BootFull> getFullNewList();
	//返回单表单条记录
    @Select("select * from fishboat_radar where GUID=#{guid}")
    Fishboat_Radar findByGUID(@Param("guid") String guid);
}
```
**三、实体类**
实体类的属性对应数据表字段，但似乎大小写没有啥关系，mybatis会自动匹配。
```java
package api.entity;
import java.util.Date;
import lombok.Getter;
import lombok.Setter;
public class Fishboat_Radar{
    private @Getter @Setter String GUID;
    private @Getter @Setter long sequence;
    private @Getter @Setter long ID;
    private @Getter @Setter int type;
    private @Getter @Setter double latitude;
    private @Getter @Setter double longitude;
    private @Getter @Setter double speed;
    private @Getter @Setter double direction;
    private @Getter @Setter Date create_date;
    private @Getter @Setter int MMSI;
    private @Getter @Setter String targetName;
    private @Getter @Setter String timestamp;
}
```
```java
package api.entity;
import lombok.Getter;
import lombok.Setter;
public class BootFull extends Fishboat_Radar {
    private @Getter @Setter int IMO;
    private @Getter @Setter long length;
    private @Getter @Setter long wide;
    private @Getter @Setter int motherShipMMSI;
    private @Getter @Setter String destination;
    private @Getter @Setter String vendorID;
    private @Getter @Setter String callSign;
    private @Getter @Setter String shipClass;
    private @Getter @Setter String shipType;
    private @Getter @Setter String vesselName;
}
```
**四、相关配置文件**
build.gradle
```java
dependencies {
    compile("org.springframework.boot:spring-boot-starter-web")
    testCompile("org.springframework.boot:spring-boot-starter-test")
    //部署到外部tomcat
    providedCompile("org.springframework.boot:spring-boot-starter-tomcat")
    //thymeleaf
    compile("org.springframework.boot:spring-boot-starter-thymeleaf")
    //oracle
    compile("com.oracle:ojdbc7:12.1.0.1")
    //mybatis
    compile("org.mybatis.spring.boot:mybatis-spring-boot-starter:1.3.0")
    testCompile('org.mybatis.spring.boot:mybatis-spring-boot-starter-test:1.3.0')
}
```
application.properties
```
spring.jpa.database=oracle
spring.datasource.driver-class-name=oracle.jdbc.driver.OracleDriver
spring.datasource.url=jdbc:oracle:thin:@192.168.0.22:1522/pdbzjfwpt
spring.datasource.username=jczs
spring.datasource.password=jczs
spring.jpa.hibernate.ddl-auto=update
#网上有些教程说需要指明实体类所在路径，事实上不需要
#mybatis.typeAliasesPackage=api.entity
```
**五、调用**
从代码来看，应用mybatis，代码会得到简化，因为Repository与mapper合在一起了，如果是hibernate，映射归映射，仓库归仓库。
```java
package api.controller;
import api.entity.Author;
import api.entity.BootFull;
import api.entity.Fishboat_Radar;
import api.mapper.FishboatMapper;
import org.springframework.beans.factory.annotation.Autowired;
import org.springframework.web.bind.annotation.PathVariable;
import org.springframework.web.bind.annotation.RequestMapping;
import org.springframework.web.bind.annotation.RequestMethod;
import org.springframework.web.bind.annotation.RestController;
import java.util.List;
@RestController
@RequestMapping(value="/api/boats")
public class BoatController {
    @Autowired
    private FishboatMapper boatMapper;
    @RequestMapping(value = {"/",""}, method = RequestMethod.GET)
    public List<Fishboat_Radar> getList() {
        List<Fishboat_Radar> boats = boatMapper.getNewList();
        return boats;
    }
    @RequestMapping(value = "/full", method = RequestMethod.GET)
    public List<BootFull> getFullList() {
        List<BootFull> boats = boatMapper.getFullNewList();
        return boats;
    }
    @RequestMapping(value = "/{guid}", method = RequestMethod.GET)
    public Fishboat_Radar findByGUID(@PathVariable String guid) {
        Fishboat_Radar boat = boatMapper.findByGUID(guid);
        return boat;
    }
}
```
