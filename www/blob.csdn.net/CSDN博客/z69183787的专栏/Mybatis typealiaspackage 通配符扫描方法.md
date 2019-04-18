# Mybatis typealiaspackage 通配符扫描方法 - z69183787的专栏 - CSDN博客
2015年10月07日 09:43:00[OkidoGreen](https://me.csdn.net/z69183787)阅读数：28485
最近两天项目需求研究了一下mybatis拦截器。对于Mybatis拦截器发现其功能强大，虽很灵活但是其内部对象转换太麻烦很多接口没有完全暴露出来。甚至不得不通过反射的方式去取其内部关联对象。可能Mybatis也不希望用户直接对其内部Statement,以及ResultSetHandler等进行操作。那这样与直接JDBC又有何区别呢？ 
通用查询其实也并非完全通用。只能是稍微的简化一下代码，减少程序员一些重复的工作罢了。本项目采用springMVC + Mybatis + EasyUi 进行构建。设想一种应用场景。我一个统计查询：统计四张表里不同数据，或者多表关联查询：从A表当中查询三个字段，从B表当中查询二个字段，从C表当中查询一个字段，从D表当中查询两个字段。这种场景对于Mybaits来说。几张表的关联查询比较头痛。两种方式，一种是建立实体对象（VO）多表关联查询然后建立映射。返回其VO实体类。另一种方式通过对象关联的方式，Mapper.xml里进行配置。两种方式在此不作讨论。（PS：如有高手有更好的方式解决这种场景请不吝赐教） 
本文主要讨论通过Mybatis拦截器实现直接取其 ResultSet 通过约定的SQL语句格式解析后生成数据格式。或者直接JSON化传给前台以作展示。 
拦截器： 
    由于我们不关心对象与字段的映射关联。所以我们只需要在 ResultSetHandler 当中进行拦截就行了，拦截其handleResultSets方法。 
直接上代码： 
Java代码  ![收藏代码](http://wyuxiao729.iteye.com/images/icon_star.png)
- /*
-  * E2ESQM-W 业务端故障诊断系统 
-  * FileName:MybatisPageInterceptor.java
-  * Company: ZZNode Technology Co., Ltd.
-  */
- package com.zznode.e2esqm.core.commons;  
- 
- import java.lang.reflect.Field;  
- import java.sql.ResultSet;  
- import java.sql.Statement;  
- import java.util.ArrayList;  
- import java.util.HashMap;  
- import java.util.List;  
- import java.util.Map;  
- import java.util.Properties;  
- 
- import org.apache.ibatis.executor.resultset.ResultSetHandler;  
- import org.apache.ibatis.mapping.BoundSql;  
- import org.apache.ibatis.plugin.Interceptor;  
- import org.apache.ibatis.plugin.Intercepts;  
- import org.apache.ibatis.plugin.Invocation;  
- import org.apache.ibatis.plugin.Plugin;  
- import org.apache.ibatis.plugin.Signature;  
- 
- /**
-  * Mybatis直接返回JSON格式拦截组件
-  * @author wangkaiping
-  * @version V1.0, 2013-5-17 下午11:43:16
-  */
- @Intercepts( {@Signature(method = "handleResultSets", type = ResultSetHandler.class, args = {Statement.class}) })  
- publicclass MybatisJsonInterceptor implements Interceptor{  
- 
- @Override
- public Object intercept(Invocation invocation) throws Throwable {  
-         ResultSetHandler resultSetHandler = (ResultSetHandler) invocation.getTarget();  
-         BoundSql boundsql = (BoundSql) ReflectUtil.getFieldValue(resultSetHandler, "boundSql");  
-         String sql = boundsql.getSql();  
- if(sql.indexOf("t_sys_privilege ch") != -1){ //测试代码写死的    这里应该根据SQL特殊标识进行解析
-             String subSql = sql.substring(6, sql.indexOf("from"));  
-             System.out.println(subSql); // 解析字段格式为:  t.id as ID,t.name as 名称,t.age as 年龄 
-             String [] colmns = subSql.split(",");  
-             List<String> colmnsArr = new ArrayList<String>(); //字段别名集合。
- for(String i : colmns){  
-                 String [] asName = i.split("as");  
-                 colmnsArr.add(asName[1]);  
-             }  
- 
-             Statement statement = (Statement) invocation.getArgs()[0]; //取得方法的参数Statement
-             ResultSet rs = statement.getResultSet(); // 取得结果集
-             List<Map> list = new ArrayList<Map>(); // 方法要求返回一个List  list里装的是K,V的键值对。 K字段别名V值 以便后续JSON化前台直接展示
- while(rs.next()){  
- if(colmnsArr.size() >0) {  
-                     Map<String,Object> map = new HashMap<String,Object>();  
- for(int i=0 ;i<colmnsArr.size();i++){  
-                         Object obj = rs.getObject(colmnsArr.get(i).trim());  
-                         map.put(colmnsArr.get(i).trim(), obj); //取得结果集后K、V关联后放到MAP当中
-                     }  
-                     list.add(map);  
-                 }  
-             }  
- return list;//这里直接返回，不要再去invocation.proceed();
-         }  
- return invocation.proceed();  
-     }  
- 
- @Override
- public Object plugin(Object target) {  
- return Plugin.wrap(target, this);   
-     }  
- 
- @Override
- publicvoid setProperties(Properties properties) {  
-         System.out.println(properties.getProperty("databaseType"));   
-     }  
- 
- 
- /**
-      * 反射工具类
-      * @author wangkaiping
-      * @version V1.0, 2013-5-17 下午11:58:50
-      */
- privatestaticclass ReflectUtil {  
- 
- /**
-          * 利用反射获取指定对象的指定属性
-          * @param obj 目标对象
-          * @param fieldName 目标属性
-          * @return 目标属性的值
-          */
- publicstatic Object getFieldValue(Object obj, String fieldName) {  
-             Object result = null;  
-             Field field = ReflectUtil.getField(obj, fieldName);  
- if (field != null) {  
-                 field.setAccessible(true);  
- try {  
-                     result = field.get(obj);  
-                 } catch (IllegalArgumentException e) {  
- // TODO Auto-generated catch block
-                     e.printStackTrace();  
-                 } catch (IllegalAccessException e) {  
- // TODO Auto-generated catch block
-                     e.printStackTrace();  
-                 }  
-             }  
- return result;  
-         }  
- 
- /**
-          * 利用反射获取指定对象里面的指定属性
-          * @param obj 目标对象
-          * @param fieldName 目标属性
-          * @return 目标字段
-          */
- privatestatic Field getField(Object obj, String fieldName) {  
-             Field field = null;  
- for (Class<?> clazz = obj.getClass(); clazz != Object.class; clazz = clazz  
-                     .getSuperclass()) {  
- try {  
-                     field = clazz.getDeclaredField(fieldName);  
- break;  
-                 } catch (NoSuchFieldException e) {  
- // 这里不用做处理，子类没有该字段可能对应的父类有，都没有就返回null。
-                 }  
-             }  
- return field;  
-         }  
- 
- /**
-          * 利用反射设置指定对象的指定属性为指定的值
-          * @param obj 目标对象
-          * @param fieldName 目标属性
-          * @param fieldValue 目标值
-          */
- publicstaticvoid setFieldValue(Object obj, String fieldName,String fieldValue) {  
-             Field field = ReflectUtil.getField(obj, fieldName);  
- if (field != null) {  
- try {  
-                     field.setAccessible(true);  
-                     field.set(obj, fieldValue);  
-                 } catch (IllegalArgumentException e) {  
- // TODO Auto-generated catch block
-                     e.printStackTrace();  
-                 } catch (IllegalAccessException e) {  
- // TODO Auto-generated catch block
-                     e.printStackTrace();  
-                 }  
-             }  
-         }  
-     }    
- 
- }  
Mybatis 整合 Spring 配置代码 。  这里我们重写了SqlSessionFactoryBean。所以这里用的是自己的。其中我们把我们的拦截器直接注入进去了。 
一个是分页的拦截器,一个是上面的我们的直接返回JSON格式的拦截器。 
Java代码  ![收藏代码](http://wyuxiao729.iteye.com/images/icon_star.png)
- <bean id="myBatisPageIntercept"class="com.zznode.e2esqm.core.commons.MybatisPageInterceptor">  
-         <property name="databaseType" value="oracle"></property>  
-     </bean>  
-     <bean id="myBatisJsonIntercept"class="com.zznode.e2esqm.core.commons.MybatisJsonInterceptor"></bean>  
-     <!-- SqlSessionFactory -->  
-     <bean id="sqlSessionFactory"class="com.zznode.e2esqm.core.commons.PackagesSqlSessionFactoryBean">  
-         <property name="dataSource" ref="dataSource" />  
-         <!-- 根据实际情况修改或添加多个 -->  
-         <property name="typeAliasesPackage" value="com.zznode.e2esqm.**.entity" />  
-         <property name="plugins">  
-             <list>  
-                                 <!-- 注入拦截器-->   
-                 <ref bean="myBatisPageIntercept"/>  
-                 <ref bean="myBatisJsonIntercept"/>  
-             </list>  
-         </property>  
-     </bean>   
重写的SqlSessionFactoryBean  代码如下 : 
Java代码  ![收藏代码](http://wyuxiao729.iteye.com/images/icon_star.png)
- package com.zznode.e2esqm.core.commons;  
- 
- import java.util.List;  
- 
- import org.apache.ibatis.plugin.Interceptor;  
- import org.apache.log4j.Logger;  
- import org.mybatis.spring.SqlSessionFactoryBean;  
- 
- import com.zznode.e2esqm.utils.PackageUtils;  
- import com.zznode.e2esqm.utils.Utils;  
- 
- /**
-  * Spring Mybatis整合
-  * 通过通配符方式配置typeAliasesPackage
-  * @author sunjian  
-  * @version 1.0 2013-2-25
-  */
- publicclass PackagesSqlSessionFactoryBean extends SqlSessionFactoryBean{  
- 
- privatefinalstatic Logger log = Logger.getLogger(PackagesSqlSessionFactoryBean.class);  
- 
- @Override
- publicvoid setTypeAliasesPackage(String typeAliasesPackage) {  
-         List<String> list = PackageUtils.getPackages(typeAliasesPackage);  
- if(list!=null&&list.size()>0){  
- super.setTypeAliasesPackage(Utils.join(list.toArray(), ","));  
-         }else{  
-             log.warn("参数typeAliasesPackage:"+typeAliasesPackage+"，未找到任何包");  
-         }  
-     }  
- 
- @Override
- publicvoid setPlugins(Interceptor[] plugins) {  
- // TODO Auto-generated method stub
- super.setPlugins(plugins);  
-     }  
- }  
Mapper接口测试代码  注意SQL语句每个字段必须写，以as别名的方式生成JSON的 KEY 
方法 ： 
@Select("select ch.id as ID,ch.name as 名称,ch.uri as URI,ch.icon as 图标,ch.description as 描述,ch.ord as 排序号") 
public List testPri(); 
测试返回结果 ： 
[{图标=pencil, 排序号=12, 名称=角色管理, 状态=1, ID=4, 父ID=1, 描述=角色管理, 类型=1, URI=role/list.do}, {图标=pictures, 排序号=21, 名称=ITV测试, 状态=1, ID=5, 父ID=3, 描述=ITV测试, 类型=1,
 URI=itvTestAction.do?list}, {图标=pictures, 排序号=14, 名称=菜单权限, 状态=1, ID=8, 父ID=1, 描述=菜单权限, 类型=1, URI=privilege/toList.do}, {图标=pie, 排序号=13, 名称=部门管理, 状态=1, ID=6, 父ID=1, 描述=部门管理 , 类型=1, URI=department/department.do}, {图标=pie, 排序号=2, 名称=系统日志, 状态=1, ID=61, 父ID=60,
 描述=系统日志, 类型=1, URI=systemLog/list.do}, {图标=pie, 排序号=3232, 名称=主界面, 状态=1, ID=10, 父ID=5, 描述=管理界面所有功能, 类型=2, URI=panel/*}, {图标=pie, 排序号=3232, 名称=权限功能, 状态=1, ID=11, 父ID=3, 描述=权限所有功能, 类型=2, URI=privilege/*}, {图标=pie, 排序号=3232, 名称=所有权限, 状态=1, ID=12, 父ID=3, 描述=42433424,
 类型=2, URI=*}, {图标=pie, 排序号=2, 名称=全局参数, 状态=1, ID=62, 父ID=1, 描述=系统全局参数, 类型=1, URI=sysParameter/toList.do}, {图标=pencil, 排序号=1, 名称=系统管理, 状态=1, ID=1, 父ID=-1, 描述=系统管理, 类型=1, URI=#}, {图标=pie, 排序号=11, 名称=用户管理, 状态=1, ID=2, 父ID=1, 描述=用户管理, 类型=1, URI=user/list.do}, {图标=folder,
 排序号=2, 名称=ITV测试, 状态=1, ID=3, 父ID=-1, 描述=ITV测试, 类型=1, URI=#}, {图标=pie, 排序号=31, 名称=日志管理, 状态=1, ID=60, 父ID=-1, 描述=日志管理, 类型=1, URI=systemLog/list.do}] 
这里只是我的测试代码，还有很多需要完善 
这里只是关于Mybatis实现一些需要关联或者多表联合查询，统计呀等类型查询方式的 一种解决方案。
