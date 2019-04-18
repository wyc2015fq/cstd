# BeanUtils使用总结 setProperties - z69183787的专栏 - CSDN博客
2015年01月09日 08:26:50[OkidoGreen](https://me.csdn.net/z69183787)阅读数：9634
**1、 BeanUtils一共分4个包：**
 org.apache.commons.beanutils
 org.apache.commons.beanutils.converters
 org.apache.commons.beanutils.locale
 org.apache.commons.beanutils.locale.converters
其中上面两个是BeanUtils的默认实现，它没有针对本地化的任何处理，这个可以提高执行效率。
但是若你的程序对于本地化有要求的话，那还是使用下面2个包比较安全。
 org.apache.commons.beanutils
 这个包主要提供用于操作JavaBean的工具类，Jakarta-Common-BeanUtils的主要功能都在这个包里实现。
**2、BeanUtils可以直接get和set一个属性的值。它将property分成3种类型：**
       Simple——简单类型，如Stirng、Int……   
       Indexed——索引类型，如数组、arrayList……   
       Maped——这个不用说也该知道，就是指Map啦，比如HashMap……   
       访问不同类型的数据可以直接调用函数getProperty和setProperty。getProperty只有2个参数，第一个是JavaBean对象，第二个是要操作的属性名；setProperty提供三个参数，第一是JavaBean对象，第二个是要操作的属性名，第三个为要设置的具体的值
public class Company { private String name; private HashMap address = new HashMap(); private String[] otherInfo; private ArrayList product; private ArrayList employee; private HashMap telephone; public Company(){} public String getName() { return name; } public
 void setName(String name) { this.name = name; } public String getAddress(String type) { return address.get(type).toString(); } public void setAddress(String type, String address) { this.address.put(type,address); } public String[] getOtherInfo() { return otherInfo;
 } public void setOtherInfo(String[] otherInfo) { this.otherInfo = otherInfo; } public ArrayList getProduct() { return product; } public void setProduct(ArrayList product) { this.product = product; } public ArrayList getEmployee() { return employee; } public
 void setEmployee(ArrayList employee) { this.employee = employee; } public HashMap getTelephone() { return telephone; } public void setTelephone(HashMap telephone) { this.telephone = telephone; } }
(1)Simple
//对于Simple类型，参数二直接是属性名即可
 System.out.println(BeanUtils.getProperty(c, "name"));
(2)Map
//对于Map类型，则需要以“属性名（key值）”的形式
 System.out.println(BeanUtils.getProperty(c, "address (A2)"));
 HashMap am = new HashMap();
am.put("1","234-222-1222211");
am.put("2","021-086-1232323");
 BeanUtils.setProperty(c,"telephone",am);
System.out.println(BeanUtils.getProperty(c, "telephone (2)"));
*Map类型也可以采用BeanUtils.getMappedProperty(c, "address","A2");*
(3)index 
//对于Indexed，则为“属性名[索引值]”，注意这里对于ArrayList和数组都可以用一样的方式进行操作。
System.out.println(BeanUtils.getProperty(c, "otherInfo[2]"));
BeanUtils.setProperty(c, "product[1]", "NOTES SERVER");
System.out.println(BeanUtils.getProperty(c, "product[1]"));
*index类型还可以采用BeanUtils.getIndexedProperty(c,"product",1);*
 (4)nest 3种类也可以组合使用啦！
System.out.println(BeanUtils.getProperty(c, "employee[1].name"));
**3、bean copy功能 **
       Company c2 = new Company();
       BeanUtils.copyProperties(c2, c);
       但是这种copy都是浅拷贝，复制后的2个Bean的同一个属性可能拥有同一个对象的ref，这个在使用时要小心，特别是对于属性为自定义类的情况
**4、 BeanUtils和PropertyUtils**
       这两个类几乎有一摸一样的功能，唯一的区别是：BeanUtils在对Bean赋值是会进行类型转化。举例来说也就是在copyProperty时只要属性名相同，就算类型不同，BeanUtils也可以进行copy；而PropertyBean则可能会报错！！
       针对上面的例子，新建一个Company2的类，其中代码与Company一样，只是将otherinfo从String[]改为String。
      Company c = init();
      Company2 c2 = new Company2();
      BeanUtils.copyProperties(c2,c);
//    PropertyUtils.copyProperties(c2,c); 这句会报错！！
      System.out.println(c2.getOtherInfo());
    当然2个Bean之间的同名属性的类型必须是可以转化的，否则用BeanUtils一样会报错。
       若实现了org.apache.commons.beanutils.Converter接口则可以自定义类型之间的转化。
由于不做类型转化，用PropertyUtils在速度上会有很大提高！
此外，不作类型转化还有一个好处，如下面的代码：
      //test data type convert
//    ArrayList a1 = BeanUtils.getProperty(c,"product"); //BeanUtils返回的是String
      System.out.println("--" + BeanUtils.getProperty(c,"product"));     //取出后直接被转为String
      ArrayList a = (ArrayList)PropertyUtils.getProperty(c,"product");//PropertyUtils返回的是Object
      System.out.println("--" + a.get(1));
用BeanUtils无法返回一个对象（除非自己写一个Converter），它会自动进行类型转化，然后返回String。对于想返回java类或自定义类的话，还是请使用PropertyUtils
**5、Utils类**
       所有的XXXUtils类都提供的是静态方法，可以直接调用，其主要实现都在相应的XXXUtilsBean中：
BeanUtils     ——> BeanUtilsBean
       ConvertUtils  ——> ConvertUtilsBean
    PropertyUtils ——> PropertyUtilsBean
 （1）**PropertyUtils，获取属性的Class类型**
public static Class getPropertyType(Object bean, String name)
**（2）**ConstructorUtils，动态创建对象****
 public static Object invokeConstructor(Class klass, Object arg)
**（3）**MethodUtils，动态调用方法****
 MethodUtils.invokeMethod(bean, methodName, parameter);
