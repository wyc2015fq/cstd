# 详解SpringMVC 自动封装枚举类的方法 - z69183787的专栏 - CSDN博客
2017年09月20日 15:37:37[OkidoGreen](https://me.csdn.net/z69183787)阅读数：1732
springmvc默认无法自动封装枚举类，本篇文章主要介绍了详解SpringMVC 自动封装枚举类的方法，有兴趣的可以了解一下。
springmvc默认无法自动封装枚举类，解决方法如下：
1.枚举类
```java
public enum GoodsPromoteEnum {
  /**
   * 0 精品
   */
  fine("精品",0),
  /**
   * 1 限时购
   */
  limit("限时购",1), 
  /**
   * 2 特价
   */
  cheap("特价",2);
  
  private String value;
  private int index;
  private GoodsPromoteEnum(String value, int index) {
    this.value = value;
    this.index = index;
  }
  
  public static GoodsPromoteEnum get(String value){
    for (GoodsPromoteEnum p : GoodsPromoteEnum.values()) {
      if (p.getValue().equals(value)) {
        return p;
      }
    }
    return null;
  }
  
  public static GoodsPromoteEnum get(int index){
    for (GoodsPromoteEnum p : GoodsPromoteEnum.values()) {
      if (p.getIndex() == index) {
        return p;
      }
    }
    return null;
  }
  public String getValue() {
    return value;
  }
  public void setValue(String value) {
    this.value = value;
  }
  public int getIndex() {
    return index;
  }
  public void setIndex(int index) {
    this.index = index;
  }
}
```
2.编写自定义处理类，继承Converter接口
```java
public class StringToGoodsConverter implements Converter<String, GoodsPromoteEnum> {
  @Override
  public GoodsPromoteEnum convert(String value) {
    if (StringUtils.isBlank(value)) {
     return null;
    }
    return GoodsPromoteEnum.get(value);
  }
}
```
3.在springmvc配置文件里配置
  <!--自定义枚举类封装 -->
  <bean id="conversionService" class="org.springframework.format.support.FormattingConversionServiceFactoryBean">
    <property name="converters">
     <set>
      <bean class="com.tentcoo.zbh.util.StringToGoodsConverter" />
     </set>
    </property>
   </bean>
