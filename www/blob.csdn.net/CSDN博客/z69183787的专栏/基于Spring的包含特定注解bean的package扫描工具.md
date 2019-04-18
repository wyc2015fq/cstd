# 基于Spring的包含特定注解bean的package扫描工具 - z69183787的专栏 - CSDN博客
2016年06月01日 14:59:19[OkidoGreen](https://me.csdn.net/z69183787)阅读数：1709
个人分类：[Spring-注入&注解&扫描](https://blog.csdn.net/z69183787/article/category/7066579)

本文出处：http://blog.csdn.net/chaijunkun/article/details/23921547，转载请注明。由于本人不定期会整理相关博文，会对相应内容作出完善。因此强烈建议在原始出处查看此文。
Spring框架为Java开发提供了很多便利的工具和编程方式，最近在研究LDAP认证，多数技术问题都已经搞定，但是针对LDAP的ODM（Object-Directory Mapping，也就是LDAP层面的ORM）还有些不足。
问题描述：
Spring项目中有一个名为Spring LDAP的子项目，可以简化查询逻辑，但是其中的LDAPTemplate需要做手动的Mapping，另外在查询时默认使用的是类似于SQL中的select *，这样也许会造成很多的网络流量浪费和性能下降。如果需指定返回哪些字段，必须输入一个String[]。这样做的结果就是，同样的一个查询，既要指定字段的String[]，又要将返回的字段一个一个地Mapping到Bean属性上，一旦将来字段增加或者减少，需要维护两个地方，增加了出错的几率。于是想到了能否像JPA的注解方式那样来配置ODM，后来查阅相关资料，还真有——使用OdmManagerImplFactoryBean。
首先需要在被映射的对象上增加Entry注解，然后在Bean属性上增加对应的Attribute注解就完成了映射。问题出来了，除了要加入注解外，还要将这些Bean加入到OdmManagerImplFactoryBean的managedClasses属性中，通知管理器哪些Bean属于受管Bean。这有点像早期的Spring对于Hibernate的支持。配置AnnotationSessionFactoryBean的时候需要设置annotatedClasses一样，不过从Spring 2.5.6开始增加了packagesToScan参数设置，它的作用是从指定的包下面扫描全部带有Entity、Embeddable、MappedSuperclass、org.hibernate.annotations.Entity.class注解的Bean，并进行管理。这样做的好处就是你把需要ODM的Bean都统一放到同一个package下，然后让配置去自动扫描，这样在你增加或减少Bean的时候不用再去关心配置中哪些Bean是受管的。
代码：
首先声明，这个代码是受org.springframework.orm.hibernate3.annotation.AnnotationSessionFactoryBean中scanPackages方法的启发，借鉴了其中的大部分代码，先贴出来：
**[java]**[view
 plain](http://blog.csdn.net/chaijunkun/article/details/23921547#)[copy](http://blog.csdn.net/chaijunkun/article/details/23921547#)
![在CODE上查看代码片](https://code.csdn.net/assets/CODE_ico.png)[](https://code.csdn.net/snippets/297615/fork)
- package net.csdn.blog.chaijunkun.config;  
- 
- import java.io.IOException;  
- import java.lang.annotation.Annotation;  
- import java.util.HashSet;  
- import java.util.LinkedList;  
- import java.util.List;  
- import java.util.Set;  
- 
- import org.apache.commons.logging.Log;  
- import org.apache.commons.logging.LogFactory;  
- import org.springframework.core.io.Resource;  
- import org.springframework.core.io.support.PathMatchingResourcePatternResolver;  
- import org.springframework.core.io.support.ResourcePatternResolver;  
- import org.springframework.core.type.classreading.CachingMetadataReaderFactory;  
- import org.springframework.core.type.classreading.MetadataReader;  
- import org.springframework.core.type.classreading.MetadataReaderFactory;  
- import org.springframework.core.type.filter.AnnotationTypeFilter;  
- import org.springframework.core.type.filter.TypeFilter;  
- import org.springframework.util.ClassUtils;  
- 
- 
- publicclass LoadPackageClasses {  
- 
- protectedfinal Log logger = LogFactory.getLog(getClass());  
- 
- privatestaticfinal String RESOURCE_PATTERN = "/**/*.class";  
- 
- private ResourcePatternResolver resourcePatternResolver = new PathMatchingResourcePatternResolver();  
- 
- private List<String> packagesList= new LinkedList<String>();  
- 
- private List<TypeFilter> typeFilters = new LinkedList<TypeFilter>();  
- 
- private Set<Class<?>> classSet= new HashSet<Class<?>>();  
- 
- /**
-      * 构造函数
-      * @param packagesToScan 指定哪些包需要被扫描,支持多个包"package.a,package.b"并对每个包都会递归搜索
-      * @param annotationFilter 指定扫描包中含有特定注解标记的bean,支持多个注解
-      */
- public LoadPackageClasses(String[] packagesToScan, Class<? extends Annotation>... annotationFilter){  
- if (packagesToScan != null) {  
- for (String packagePath : packagesToScan) {  
- this.packagesList.add(packagePath);  
-             }  
-         }  
- if (annotationFilter != null){  
- for (Class<? extends Annotation> annotation : annotationFilter) {  
-                 typeFilters.add(new AnnotationTypeFilter(annotation, false));  
-             }  
-         }  
-     }  
- 
- /**
-      * 将符合条件的Bean以Class集合的形式返回
-      * @return
-      * @throws IOException
-      * @throws ClassNotFoundException
-      */
- public Set<Class<?>> getClassSet() throws IOException, ClassNotFoundException {  
- this.classSet.clear();  
- if (!this.packagesList.isEmpty()) {  
- for (String pkg : this.packagesList) {  
-                     String pattern = ResourcePatternResolver.CLASSPATH_ALL_URL_PREFIX +  
-                             ClassUtils.convertClassNameToResourcePath(pkg) + RESOURCE_PATTERN;  
-                     Resource[] resources = this.resourcePatternResolver.getResources(pattern);  
-                     MetadataReaderFactory readerFactory = new CachingMetadataReaderFactory(this.resourcePatternResolver);  
- for (Resource resource : resources) {  
- if (resource.isReadable()) {  
-                             MetadataReader reader = readerFactory.getMetadataReader(resource);  
-                             String className = reader.getClassMetadata().getClassName();  
- if (matchesEntityTypeFilter(reader, readerFactory)) {  
- this.classSet.add(Class.forName(className));  
-                             }  
-                         }  
-                     }  
-                 }  
-         }  
- //输出日志
- if (logger.isInfoEnabled()){  
- for (Class<?> clazz : this.classSet) {  
-                 logger.info(String.format("Found class:%s", clazz.getName()));  
-             }  
-         }  
- returnthis.classSet;  
-     }  
- 
- 
- 
- /**
-      * 检查当前扫描到的Bean含有任何一个指定的注解标记
-      * @param reader
-      * @param readerFactory
-      * @return
-      * @throws IOException
-      */
- privateboolean matchesEntityTypeFilter(MetadataReader reader, MetadataReaderFactory readerFactory) throws IOException {  
- if (!this.typeFilters.isEmpty()) {  
- for (TypeFilter filter : this.typeFilters) {  
- if (filter.match(reader, readerFactory)) {  
- returntrue;  
-                 }  
-             }  
-         }  
- returnfalse;  
-     }  
- 
- }  
接下来我们就可以来配置了（以下配置为Spring的applicationContext.xml配置节选）：
**[html]**[view
 plain](http://blog.csdn.net/chaijunkun/article/details/23921547#)[copy](http://blog.csdn.net/chaijunkun/article/details/23921547#)
![在CODE上查看代码片](https://code.csdn.net/assets/CODE_ico.png)[](https://code.csdn.net/snippets/297615/fork)
- <beanid="loadPackageClasses"class="net.csdn.blog.chaijunkun.config.LoadPackageClasses">
- <constructor-argvalue="net.csdn.blog.chaijunkun.ldap.entity"/>
- <constructor-arg>
- <list>
- <value>org.springframework.ldap.odm.annotations.Entry</value>
- </list>
- </constructor-arg>
- </bean>
- <beanid="odmManager"class="org.springframework.ldap.odm.core.impl.OdmManagerImplFactoryBean">
- <propertyname="converterManager"ref="converterManager"/>
- <propertyname="contextSource"ref="contextSource"/>
- <propertyname="managedClasses">
- <beanclass="org.springframework.beans.factory.config.MethodInvokingFactoryBean">
- <propertyname="targetObject">
- <reflocal="loadPackageClasses"/>
- </property>
- <propertyname="targetMethod">
- <value>getClassSet</value>
- </property>
- </bean>
- </property>
- </bean>
在上述配置中，指定了扫描的包为net.csdn.blog.chaijunkun.ldap.entity，然后指定了筛选条件是包含org.springframework.ldap.odm.annotations.Entry注解（指定的注解必须在Class级，不能是property级和method级，也就是Bean头部的注解）的所有Bean。
因为获取筛选出类的集合要注入到OdmManagerImplFactoryBean中的managedClasses属性，类型为Set<Class<?>>，所以我们需要调用getClassSet()方法，用其返回值进行注入。于是使用了一个MethodInvokingFactoryBean来实现。
实验结果表明，加入了这个组件之后确实达到了预期的效果。另外由于LoadPackageClasses本身配置上很灵活，可以用于筛选任何带有特定注解的Bean，所以其他类似的场合也可以使用。当然，我更希望OdmManagerImplFactoryBean中能自带package扫描的配置，这样会让我们省好多事。
我自己的实现
```java
public class PackagesSqlSessionFactoryBean extends SqlSessionFactoryBean {
    static final String DEFAULT_RESOURCE_PATTERN = "**/*.class";
    private static Logger logger = LoggerFactory.getLogger(PackagesSqlSessionFactoryBean.class);
    @Override
    public void setTypeAliasesPackage(String typeAliasesPackage) {
        ResourcePatternResolver resolver = (ResourcePatternResolver) new PathMatchingResourcePatternResolver();
        MetadataReaderFactory metadataReaderFactory = new CachingMetadataReaderFactory(resolver);
        typeAliasesPackage = ResourcePatternResolver.CLASSPATH_ALL_URL_PREFIX +
                ClassUtils.convertClassNameToResourcePath(typeAliasesPackage) + "/" + DEFAULT_RESOURCE_PATTERN;
        //将加载多个绝对匹配的所有Resource
        //将首先通过ClassLoader.getResource("META-INF")加载非模式路径部分
        //然后进行遍历模式匹配
        try {
            List<String> result = new ArrayList<String>();
            Resource[] resources =  resolver.getResources(typeAliasesPackage);
            if(resources != null && resources.length > 0){
                MetadataReader metadataReader = null;
                for(Resource resource : resources){
                    if(resource.isReadable()){
                       metadataReader =  metadataReaderFactory.getMetadataReader(resource);
                        try {
                            result.add(Class.forName(metadataReader.getClassMetadata().getClassName()).getPackage().getName());
                        } catch (ClassNotFoundException e) {
                            e.printStackTrace();
                        }
                    }
                }
            }
            if(result.size() > 0) {
                super.setTypeAliasesPackage(StringUtils.join(result.toArray(), ","));
            }else{
                logger.warn("参数typeAliasesPackage:"+typeAliasesPackage+"，未找到任何包");
            }
            //logger.info("d");
        } catch (IOException e) {
            e.printStackTrace();
        }
    }
}
```
