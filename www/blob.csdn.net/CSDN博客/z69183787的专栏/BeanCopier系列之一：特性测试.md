# BeanCopier系列之一：特性测试 - z69183787的专栏 - CSDN博客
2017年09月20日 15:20:47[OkidoGreen](https://me.csdn.net/z69183787)阅读数：227
[BeanCopier系列之二：使用Converter](http://czj4451.iteye.com/blog/2044101)
[BeanCopier系列之三：缓存BeanCopier提升性能](http://czj4451.iteye.com/blog/2044150)
cglib是一款比较底层的操作java字节码的框架。 
下面通过拷贝bean对象来测试BeanCopier的特性： 
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
注意：即使源类型是原始类型(int,
 short和char等)，目标类型是其包装类型(Integer, Short和Character等)，或反之：都不会被拷贝。 
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
