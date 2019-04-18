# Mybatis 自定义SqlSessionFactoryBean扫描通配符typeAliasesPackage - z69183787的专栏 - CSDN博客
2016年06月02日 13:39:38[OkidoGreen](https://me.csdn.net/z69183787)阅读数：10302
typeAliasesPackage 默认只能扫描某一个路径下，或以逗号等分割的 几个路径下的内容，不支持通配符和正则，采用重写的方式解决
```java
package com.xxxx.xxx.util.common;
import com.xxxx.xxx.util.LogUtil;
import org.apache.commons.lang3.StringUtils;
import org.mybatis.spring.SqlSessionFactoryBean;
import org.slf4j.Logger;
import org.springframework.core.io.Resource;
import org.springframework.core.io.support.PathMatchingResourcePatternResolver;
import org.springframework.core.io.support.ResourcePatternResolver;
import org.springframework.core.type.classreading.CachingMetadataReaderFactory;
import org.springframework.core.type.classreading.MetadataReader;
import org.springframework.core.type.classreading.MetadataReaderFactory;
import org.springframework.util.ClassUtils;
import java.io.IOException;
import java.util.ArrayList;
import java.util.List;
/**
 * Created by Administrator on 2015/10/6.
 */
public class PackagesSqlSessionFactoryBean extends SqlSessionFactoryBean {
    static final String DEFAULT_RESOURCE_PATTERN = "**/*.class";
    private static Logger logger = LogUtil.get();
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
```
<bean id="sqlSession" class="com.xxxx.xxxx.util.common.PackagesSqlSessionFactoryBean">
        <property name="configLocation" value="classpath:config/sqlmap/sqlmap-config.xml" />
        <property name="dataSource" ref="dataSource"/>
        <!--<property name="mapperLocations"-->
                  <!--value="classpath*:com/xxxx/xxxx/merchant/**/domain/mapper/*.xml"/>-->
        <property name="typeAliasesPackage" value="com.xxxx.xxxx.custom.*.domain"/>
        <property name="plugins">
            <array>
                <ref bean="pageInterceptor"/>
            </array>
        </property>
    </bean>
    <bean class="org.mybatis.spring.mapper.MapperScannerConfigurer">
        <property name="basePackage" value="com.xxxx.xxxx.**.dao"/>
    </bean>
```
