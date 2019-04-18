# BeanCopier系列之三：缓存BeanCopier提升性能 - z69183787的专栏 - CSDN博客
2017年09月20日 15:22:57[OkidoGreen](https://me.csdn.net/z69183787)阅读数：1846

[BeanCopier系列之一：特性测试](http://czj4451.iteye.com/blog/1998750)
[BeanCopier系列之二：使用Converter](http://czj4451.iteye.com/blog/2044101)
BeanCopier拷贝速度快，性能瓶颈出现在创建BeanCopier实例的过程中。 
所以，把创建过的BeanCopier实例放到缓存中，下次可以直接获取，提升性能： 
Java代码  ![收藏代码](http://czj4451.iteye.com/images/icon_star.png)
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
分享
