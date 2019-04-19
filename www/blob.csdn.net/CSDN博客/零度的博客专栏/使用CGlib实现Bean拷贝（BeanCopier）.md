# 使用CGlib实现Bean拷贝（BeanCopier） - 零度的博客专栏 - CSDN博客
2017年10月27日 11:40:44[零度anngle](https://me.csdn.net/zmx729618)阅读数：6146
在做业务的时候，我们有时为了隔离变化，会将DAO查询出来的Entity，和对外提供的DTO隔离开来。大概90%的时候，它们的结构都是类似的，但是我们很不喜欢写很多冗长的b.setF1(a.getF1())这样的代码，于是我们需要BeanCopier来帮助我们。`BeanCopier`其实已经有很多开源版本，例如[DozerMapper](https://github.com/DozerMapper/dozer)、[Apache
 BeanUtils](http://commons.apache.org/proper/commons-beanutils/)、[Spring](https://spring.io/)、[Jodd
 BeanUtils](http://jodd.org/doc/beanutil.html)甚至是[Cglib](https://github.com/cglib/cglib)都提供了这样的功能。下面介绍Cglib的BeanCopier的使用。
### 1、通过拷贝bean对象来测试BeanCopier的基本使用和特性
Java代码  ![收藏代码](http://czj4451.iteye.com/images/icon_star.png)
- publicclass OrderEntity {  
- privateint id;  
- private String name;  
- // Getters and setters are omitted
- }  
Java代码  ![收藏代码](http://czj4451.iteye.com/images/icon_star.png)
- publicclass OrderDto {  
- privateint id;  
- private String name;  
- // Getters and setters are omitted
- }  
Java代码  ![收藏代码](http://czj4451.iteye.com/images/icon_star.png)
- publicclass PropWithDiffType {  
- private Integer id;  
- private String name;  
- // Getters and setters are omitted
- }  
Java代码  ![收藏代码](http://czj4451.iteye.com/images/icon_star.png)
- publicclass LackOfSetter {  
- privateint id;  
- private String name;  
- 
- public LackOfSetter() {  
-     }  
- 
- public LackOfSetter(int id, String name) {  
- this.id = id;  
- this.name = name;  
-     }  
- // Getters and setters are omitted
- // public void setName(String name) {
- //  this.name = name;
- // }
- }  
1. 属性名称、类型都相同: 
Java代码  ![收藏代码](http://czj4451.iteye.com/images/icon_star.png)
- @Test
- publicvoid normalCopyTest() {  
-     OrderEntity entity = new OrderEntity();  
-     entity.setId(1);  
-     entity.setName("orderName");  
- final BeanCopier copier = BeanCopier.create(OrderEntity.class, OrderDto.class, false);  
-     OrderDto dto = new OrderDto();  
-     copier.copy(entity, dto, null);  
-     Assert.assertEquals(1, dto.getId());  
-     Assert.assertEquals("orderName", dto.getName());  
- }  
结论：拷贝OK。 
2. 属性名称相同、类型不同： 
Java代码  ![收藏代码](http://czj4451.iteye.com/images/icon_star.png)
- @Test
- publicvoid sameNameDifferentTypeCopyTest() {  
-     OrderEntity entity = new OrderEntity();  
-     entity.setId(1);  
-     entity.setName("orderName");  
- final BeanCopier copier = BeanCopier.create(OrderEntity.class, PropWithDiffType.class, false);  
-     PropWithDiffType dto = new PropWithDiffType();  
-     copier.copy(entity, dto, null);  
-     Assert.assertEquals(null, dto.getId()); // OrderEntity的id为int类型，而PropWithDiffType的id为Integer类型，不拷贝
-     Assert.assertEquals("orderName", dto.getName());  
- }  
结论：名称相同而类型不同的属性不会被拷贝。 
注意：即使源类型是原始类型(int, short和char等)，目标类型是其包装类型(Integer, Short和Character等)，或反之：都不会被拷贝。 
3. 源类和目标类有相同的属性(两者的getter都存在),但目标类的setter不存在 
Java代码  ![收藏代码](http://czj4451.iteye.com/images/icon_star.png)
- @Test
- publicvoid targetLackOfSetterCopyTest() {  
-     OrderEntity entity = new OrderEntity();  
-     entity.setId(1);  
-     entity.setName("orderName");  
- final BeanCopier copier = BeanCopier.create(OrderEntity.class, LackOfSetter.class, false);  // 抛NullPointerException
-     LackOfSetter dto = new LackOfSetter();  
-     copier.copy(entity, dto, null);  
- }  
结论：创建BeanCopier的时候抛异常。 
导致异常的原因是BeanCopier类的第128~133行 
Java代码  ![收藏代码](http://czj4451.iteye.com/images/icon_star.png)
- for (int i = 0; i < setters.length; i++) { // 遍历目标类的属性描述集
-     PropertyDescriptor setter = setters[i];  
-     PropertyDescriptor getter = (PropertyDescriptor)names.get(setter.getName()); // 从源类获取和目标类属性名称相同的属性描述
- if (getter != null) {  
-         MethodInfo read = ReflectUtils.getMethodInfo(getter.getReadMethod()); // 获取源类属性的getter方法
-         MethodInfo write = ReflectUtils.getMethodInfo(setter.getWriteMethod()); // 获取目标类属性的setter方法。LackOfSetter类name属性的setter方法没有，所以报错
4. 源类或目标类的setter比getter少 
Java代码  ![收藏代码](http://czj4451.iteye.com/images/icon_star.png)
- @Test
- publicvoid sourceLackOfSetterCopyTest() {  
-     LackOfSetter source = new LackOfSetter(1, "throne");  
- final BeanCopier copier = BeanCopier.create(LackOfSetter.class, OrderDto.class, false);  
-     OrderDto dto = new OrderDto();  
-     copier.copy(source, dto, null);  
-     Assert.assertEquals(1, dto.getId());  
-     Assert.assertEquals("throne", dto.getName());  
- }  
结论：拷贝OK。此时的setter多余，但不会报错。 
总结： 
1. BeanCopier只拷贝名称和类型都相同的属性。 
2. 当目标类的setter数目比getter少时，创建BeanCopier会失败而导致拷贝不成功。
### 二、自定义转换器
当源和目标类的属性类型不同时，不能拷贝该属性，此时我们可以通过实现Converter接口来自定义转换器
源类和目标类： 
Java代码  ![收藏代码](http://czj4451.iteye.com/images/icon_star.png)
- publicclass AccountEntity {  
- privateint id;  
- private Timestamp createTime;  
- private BigDecimal balance;  
- // Getters and setters are omitted
- }  
Java代码  ![收藏代码](http://czj4451.iteye.com/images/icon_star.png)
- publicclass AccountDto {  
- privateint id;  
- private String name;  
- private String createTime;  
- private String balance;  
- // Getters and setters are omitted
- }  
1. 不使用Converter 
Java代码  ![收藏代码](http://czj4451.iteye.com/images/icon_star.png)
- publicclass BeanCopierConverterTest {  
- 
- @Test
- publicvoid noConverterTest() {  
-         AccountEntity po = new AccountEntity();  
-         po.setId(1);  
-         po.setCreateTime(new Timestamp(10043143243L));  
-         po.setBalance(BigDecimal.valueOf(4000L));  
-         BeanCopier copier = BeanCopier.create(AccountEntity.class, AccountDto.class, false);  
-         AccountDto dto = new AccountDto();  
-         copier.copy(po, dto, null);  
-         Assert.assertNull(dto.getCreateTime()); // 类型不同，未拷贝
-         Assert.assertNull(dto.getBalance()); // 类型不同，未拷贝
-     }  
- }  
2. 使用Converter 
基于目标对象的属性出发，如果源对象有相同名称的属性，则调一次convert方法： 
Java代码  ![收藏代码](http://czj4451.iteye.com/images/icon_star.png)
- package net.sf.cglib.core;  
- 
- publicinterface Converter {  
- // value 源对象属性，target 目标对象属性类，context 目标对象setter方法名
-     Object convert(Object value, Class target, Object context);  
- }  
Java代码  ![收藏代码](http://czj4451.iteye.com/images/icon_star.png)
- @Test
- publicvoid converterTest() {  
-     AccountEntity po = new AccountEntity();  
-     po.setId(1);  
-     po.setCreateTime(Timestamp.valueOf("2014-04-12 16:16:15"));  
-     po.setBalance(BigDecimal.valueOf(4000L));  
-     BeanCopier copier = BeanCopier.create(AccountEntity.class, AccountDto.class, true);  
-     AccountConverter converter = new AccountConverter();  
-     AccountDto dto = new AccountDto();  
-     copier.copy(po, dto, converter);  
-     Assert.assertEquals("2014-04-12 16:16:15", dto.getCreateTime());  
-     Assert.assertEquals("4000", dto.getBalance());  
- }  
- 
- staticclass AccountConverter implements Converter {  
- 
-     SimpleDateFormat sdf = new SimpleDateFormat("yyyy-MM-dd HH:mm:ss");  
- 
- @SuppressWarnings("rawtypes")  
- @Override
- public Object convert(Object value, Class target, Object context) {  
- if (value instanceof Integer) {  
- return (Integer) value;  
-         } elseif (value instanceof Timestamp) {  
-             Timestamp date = (Timestamp) value;  
- return sdf.format(date);  
-         } elseif (value instanceof BigDecimal) {  
-             BigDecimal bd = (BigDecimal) value;  
- return bd.toPlainString();  
-         }  
- returnnull;  
-     }  
- }  
注：一旦使用Converter，BeanCopier只使用Converter定义的规则去拷贝属性，所以在convert方法中要考虑所有的属性。
### 三、[缓存BeanCopier提升性能](http://czj4451.iteye.com/blog/2044150)
BeanCopier拷贝速度快，性能瓶颈出现在创建BeanCopier实例的过程中。 所以，把创建过的BeanCopier实例放到缓存中，下次可以直接获取，提升性能：
Java代码 ![复制代码](http://czj4451.iteye.com/images/icon_copy.gif)![收藏代码](http://czj4451.iteye.com/images/icon_star.png)![](http://czj4451.iteye.com/images/spinner.gif)
- publicclass CachedBeanCopier {  
- 
- staticfinal Map<String, BeanCopier> BEAN_COPIERS = new HashMap<String, BeanCopier>();  
- 
- publicstaticvoid copy(Object srcObj, Object destObj) {  
-         String key = genKey(srcObj.getClass(), destObj.getClass());  
-         BeanCopier copier = null;  
- if (!BEAN_COPIERS.containsKey(key)) {  
-             copier = BeanCopier.create(srcObj.getClass(), destObj.getClass(), false);  
-             BEAN_COPIERS.put(key, copier);  
-         } else {  
-             copier = BEAN_COPIERS.get(key);  
-         }  
-         copier.copy(srcObj, destObj, null);  
-     }  
- 
- privatestatic String genKey(Class<?> srcClazz, Class<?> destClazz) {  
- return srcClazz.getName() + destClazz.getName();  
-     }  
- }  
