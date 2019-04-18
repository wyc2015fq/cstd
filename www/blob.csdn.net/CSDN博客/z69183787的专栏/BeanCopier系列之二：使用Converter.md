# BeanCopier系列之二：使用Converter - z69183787的专栏 - CSDN博客
2017年09月20日 15:20:12[OkidoGreen](https://me.csdn.net/z69183787)阅读数：892
[BeanCopier系列之一：特性测试](http://czj4451.iteye.com/blog/1998750)
[BeanCopier系列之三：缓存BeanCopier提升性能](http://czj4451.iteye.com/blog/2044150)
  当源和目标类的属性类型不同时，不能拷贝该属性，此时我们可以通过实现Converter接口来自定义转换器： 
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
