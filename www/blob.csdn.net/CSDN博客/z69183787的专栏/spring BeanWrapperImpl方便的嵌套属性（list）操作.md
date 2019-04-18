# spring BeanWrapperImpl方便的嵌套属性（list）操作 - z69183787的专栏 - CSDN博客
2016年07月11日 18:10:52[OkidoGreen](https://me.csdn.net/z69183787)阅读数：2118
转载自: 
http://blog.csdn.net/zyl623/archive/2006/09/17/1232920.aspx 
-------------------------------------------------------------- 
这几天，闲着无事，看起了spring的源码，顺便也学习一下，人家的设计理念。 
spring的源码还是比较多的，但是区分的层次比较可以。对比了一下自己设计的程序，有点惭愧，每次分包的时候，设计的都比较烂。 
    闲话少说，先来看看spring 的核心包beans。 
    beans 包主要提供了接口和类用于处理java beans。 
    其中最主要的接口是BeanWrapper： 
Spring 的中心接口，用于访问javabeans
 的低层操作。默认实现为BeanWrapperImpl 提供分析和处理标准java
 beans 用于get 和set 属性，取得属性描述，查询属性的读/写能力。 
    用法非常简单 
    TestBean tb = new TestBean(); 
    BeanWrapper bw = new BeanWrapperImpl(tb); 
    bw.isReadableProperty("age");//判断age属性是否可读 
    bw.setPropertyValue("name", "tom"); //设置name属性的值为tom 
    bw.getPropertyValue("int2")//取得属性 
    //当然，最强大的是能嵌套设置属性，如： 
    //tb中有个spouse的属性，也为TestBean，这样 
    bw.setPropertyValue("spouse.name", "tom"); 
    //等价于tb.getSpouse().setName("tom"); 
    //bw 同样支持数组和集合,map属性设置 
    bw.getPropertyValue("array[0].name"); 
    bw.getPropertyValue("map[key4][0].name"); 
    beans包 还提供了一个PropertyValues用于保存多个属性值，默认的实现MutablePropertyValues 
    MutablePropertyValues pvs = new MutablePropertyValues(); 
    pvs.addPropertyValue("array[0].name", "name5"); 
    pvs.addPropertyValue("array[1].name", "name4"); 
    bw.setPropertyValues(pvs); 
    BeanWrapper的层次结构，也比较简单继承于ConfigurablePropertyAccessor ，而ConfigurablePropertyAccessor 又继承于PropertyAccessor 和   PropertyEditorRegistry 接口。 
   有一点不太明白，为什么ConfigurablePropertyAccessor 要继承于PropertyEditorRegistry 
    PropertyEditorRegistry 的主要作用是提供方法用于注册JavaBeans PropertyEditors。默认的实现为PropertyEditorRegistrySupport 
   这就使得BeanWrapperImpl 也需要继承PropertyEditorRegistrySupport，从字面意义上来说，两个类不是很有关系，其实最主要用到的方法，也就是registerDefaultEditors(),注册spring中所有的属性编辑类。 
   beans 包中还有两个接口TypeConverter和Mergeable 。TypeConverter由BeanWrapperImpl实现，主要用于转换属性值。其主要的工作由TypeConverterDelegate
 这个包内类完成的。Mergeable 主要是用来合并值得，不过具体的实现，在org.springframework.beans.factory.support 包中。 
   beans 包中提供了两个有用的工具类，BeanUtils和PropertyAccessorUtils。 
