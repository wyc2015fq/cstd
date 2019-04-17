# MyBatis与Druid数据库连接池集成监控统计后WallFilterSQL注入异常问题处理方案 - 纸上得来终觉浅，绝知此事要躬行 - CSDN博客





2017年04月18日 17:35:05[boonya](https://me.csdn.net/boonya)阅读数：9062








数据库连接池使用的是阿里巴巴的Druid（德鲁伊）。项目中启用了Druid的统计管理，在执行批量修改时提示；Caused by: java.sql.SQLException: sql injection violation, multi-statement not allow 。可以通过WallConfig设置multiStatementAllow=true解决此问题。官方配置参考：[https://github.com/alibaba/druid/wiki/%E9%85%8D%E7%BD%AE-wallfilter](https://github.com/alibaba/druid/wiki/%E9%85%8D%E7%BD%AE-wallfilter)[](https://github.com/alibaba/druid/wiki/%E9%85%8D%E7%BD%AE-wallfilter)


## 异常栈输出



```
2017-04-18 16:33:29,610 [main] INFO  [fmcgwms.OpenAPIControllerTest] - >>>基础数据同步(商品同步)JSON:{"attributes":[{"cdspFieldType":1,"cdspIsRequired":1,"cdspIsUom":1,"cdspName":"W","cdspNumber":"1","cdspRemark":"W","cdspSortnum":1,"cdspValue":"W"}],"cdskCdstCode":"C0000030","cdskExpiryDay":3,"cdskIsactive":1,"cdskItemChildCategory":"预包装食品","cdskItemCode":"00000000673","cdskItemHeight":10,"cdskItemLength":10,"cdskItemName":"辣条","cdskItemTotalWeight":2,"cdskItemWidth":10,"cdskUnit":"袋","cdskUnitRelationStr":"1箱 =10袋","cdskUom":"","packages":[{"cdsgBaseNumber":1,"cdsgHeigth":10,"cdsgIsDecimal":0,"cdsgIsMainUnit":0,"cdsgLength":10,"cdsgLevel":1,"cdsgMainRelation":1,"cdsgName":"袋","cdsgNumber":"1","cdsgSuperiorRelation":1,"cdsgWeight":2,"cdsgWidth":10},{"cdsgBaseNumber":1,"cdsgHeigth":12,"cdsgIsDecimal":0,"cdsgIsMainUnit":1,"cdsgLength":12,"cdsgLevel":2,"cdsgMainRelation":10,"cdsgName":"箱","cdsgNumber":"2","cdsgSuperiorRelation":10,"cdsgWeight":12,"cdsgWidth":12}],"sumEnabelNum":0}
2017-04-18 16:33:29,638 [main] DEBUG [java.sql.Connection] - ooo Using Connection [com.alibaba.druid.proxy.jdbc.ConnectionProxyImpl@65c69c85]
2017-04-18 16:33:29,638 [main] DEBUG [java.sql.Connection] - ==>  Preparing: select CDST_ID, CDST_CUSTOMER_NAME, CDST_CONTACT, CDST_CONTACTOR_TELL, CDST_ADDRESS, CREATOR, CREATE_TIME, MODIFIER, MODIFY_TIME, CDST_CUSTOMER_CODE, CDST_CUSTOMER_STATUS,INTERFACE_UPDATETIME,INTERFACE_SERIALNO,CDST_SOURCE, CDST_PROVINCE, CDST_CITY, CDST_COUNT, CDST_AREA_IDS from cd_customer_P0000020 where CDST_CUSTOMER_CODE = ? 
2017-04-18 16:33:29,658 [main] DEBUG [java.sql.PreparedStatement] - ==> Parameters: C0000030(String)
2017-04-18 16:33:29,666 [main] DEBUG [java.sql.Connection] - ooo Using Connection [com.alibaba.druid.proxy.jdbc.ConnectionProxyImpl@65c69c85]
2017-04-18 16:33:29,667 [main] DEBUG [java.sql.Connection] - ==>  Preparing: select CD_ITEM_ID, CREATOR, CREATE_TIME, MODIFIER, MODIFY_TIME, CDSK_ITEM_CODE, CDSK_ITEM_NAME, CDSK_ITEM_CHILD_CATEGORY, CDSK_ITEM_ORIGING, CDSK_ITEM_DESCRIPTION, CDSK_UOM, CDSK_UNIT, CDSK_EXTEND, CDSK_ISACTIVE, CDSK_BULK_UNIT, CDSK_WHOLE_BULK_RELATION, CDSK_EXPIRY_YEAR, CDSK_EXPIRY_MONTH, CDSK_EXPIRY_DAY, CDSK_CREATE_FIRM, CDSK_ITEM_WIDTH, CDSK_ITEM_HEIGHT, CDSK_ITEM_LENGTH, CDSK_ITEM_LENGTH_UNIT, CDSK_ITEM_WIDTH_UNIT, CDSK_ITEM_HEIGHT_UNIT, CDSK_ITEM_TOTAL_WEIGHT, CDSK_ITEM_REAL_WEIGHT, CDSK_ITEM_TOTAL_WEIGHT_UNIT, CDSK_ITEM_REAL_WEIGHT_UNIT,CDSK_CDST_ID,CDSK_GB_CODE, CDSK_FLOW_PROPERTY,CDSK_BOX_NUMBER,CDSK_TARY_NUMBER, CDSK_BATCH_ATTRIBUTE_CODE, CDSK_UNIT_RELATION_STR,CDSK_IS_DECIMAL, CDSK_SOURCE from cd_wh_itme_P0000020 where CDSK_ITEM_CODE = '00000000673' order by CD_ITEM_ID desc limit 0,1 
2017-04-18 16:33:29,671 [main] DEBUG [java.sql.PreparedStatement] - ==> Parameters: 
2017-04-18 16:33:29,676 [main] DEBUG [java.sql.Connection] - ooo Using Connection [com.alibaba.druid.proxy.jdbc.ConnectionProxyImpl@65c69c85]
2017-04-18 16:33:29,677 [main] DEBUG [java.sql.Connection] - ==>  Preparing: select CDSP_ID, CDSP_CDSK_CODE, CDSP_NUMBER, CDSP_NAME, CDSP_VALUE, CDSP_REMARK, CDSP_SORTNUM, CDSP_IS_UOM, CDSP_IS_REQUIRED, CDSP_FIELD_TYPE from cd_wh_property_P0000020 where CDSP_NUMBER = ? and CDSP_CDSK_CODE = ? 
2017-04-18 16:33:29,691 [main] DEBUG [java.sql.PreparedStatement] - ==> Parameters: 1(String), 00000000673(String)
2017-04-18 16:33:29,695 [main] DEBUG [java.sql.Connection] - ooo Using Connection [com.alibaba.druid.proxy.jdbc.ConnectionProxyImpl@65c69c85]
2017-04-18 16:33:29,695 [main] DEBUG [java.sql.Connection] - ==>  Preparing: update cd_wh_property_P0000020 SET CDSP_CDSK_CODE = ?, CDSP_NUMBER = ?, CDSP_NAME = ?, CDSP_VALUE = ?, CDSP_REMARK = ?, CDSP_SORTNUM = ?, CDSP_IS_UOM = ?, CDSP_IS_REQUIRED = ?, CDSP_FIELD_TYPE = ? where CDSP_ID = ? 
2017-04-18 16:33:29,697 [main] DEBUG [java.sql.PreparedStatement] - ==> Parameters: 00000000673(String), 1(String), W(String), W(String), W(String), 1(Integer), 1(Integer), 1(Integer), 1(Integer), 13(Integer)
2017-04-18 16:33:29,700 [main] DEBUG [java.sql.Connection] - ooo Using Connection [com.alibaba.druid.proxy.jdbc.ConnectionProxyImpl@65c69c85]
2017-04-18 16:33:29,700 [main] DEBUG [java.sql.Connection] - ==>  Preparing: select CDSG_ID, CDSG_CDSK_CODE, CDSG_NUMBER, CDSG_NAME, CDSG_MAIN_RELATION, CDSG_SUPERIOR_RELATION, CDSG_LENGTH, CDSG_WIDTH, CDSG_HEIGTH, CDSG_WEIGHT, CDSG_IS_DECIMAL, CDSG_LEVEL, CDSG_BASE_NUMBER, CDSG_IS_MAIN_UNIT from cd_wh_package_P0000020 where CDSG_NUMBER = ? and CDSG_CDSK_CODE = ? 
2017-04-18 16:33:29,701 [main] DEBUG [java.sql.PreparedStatement] - ==> Parameters: 1(String), 00000000673(String)
2017-04-18 16:33:29,704 [main] DEBUG [java.sql.Connection] - ooo Using Connection [com.alibaba.druid.proxy.jdbc.ConnectionProxyImpl@65c69c85]
2017-04-18 16:33:29,704 [main] DEBUG [java.sql.Connection] - ==>  Preparing: select CDSG_ID, CDSG_CDSK_CODE, CDSG_NUMBER, CDSG_NAME, CDSG_MAIN_RELATION, CDSG_SUPERIOR_RELATION, CDSG_LENGTH, CDSG_WIDTH, CDSG_HEIGTH, CDSG_WEIGHT, CDSG_IS_DECIMAL, CDSG_LEVEL, CDSG_BASE_NUMBER, CDSG_IS_MAIN_UNIT from cd_wh_package_P0000020 where CDSG_NUMBER = ? and CDSG_CDSK_CODE = ? 
2017-04-18 16:33:29,705 [main] DEBUG [java.sql.PreparedStatement] - ==> Parameters: 2(String), 00000000673(String)
2017-04-18 16:33:29,709 [main] DEBUG [java.sql.Connection] - ooo Using Connection [com.alibaba.druid.proxy.jdbc.ConnectionProxyImpl@65c69c85]
2017-04-18 16:33:29,709 [main] DEBUG [java.sql.Connection] - ==>  Preparing: update cd_wh_package_P0000020 SET CDSG_CDSK_CODE = ?, CDSG_NUMBER = ?, CDSG_NAME = ?, CDSG_MAIN_RELATION = ?, CDSG_SUPERIOR_RELATION = ?, CDSG_LENGTH = ?, CDSG_WIDTH = ?, CDSG_HEIGTH = ?, CDSG_WEIGHT = ?, CDSG_IS_DECIMAL = ?, CDSG_LEVEL = ?, CDSG_BASE_NUMBER = ?, CDSG_IS_MAIN_UNIT = ? where CDSG_ID = ? ; update cd_wh_package_P0000020 SET CDSG_CDSK_CODE = ?, CDSG_NUMBER = ?, CDSG_NAME = ?, CDSG_MAIN_RELATION = ?, CDSG_SUPERIOR_RELATION = ?, CDSG_LENGTH = ?, CDSG_WIDTH = ?, CDSG_HEIGTH = ?, CDSG_WEIGHT = ?, CDSG_IS_DECIMAL = ?, CDSG_LEVEL = ?, CDSG_BASE_NUMBER = ?, CDSG_IS_MAIN_UNIT = ? where CDSG_ID = ? 
2017-04-18 16:33:29,720 [main] INFO  [org.springframework.beans.factory.xml.XmlBeanDefinitionReader] - Loading XML bean definitions from class path resource [org/springframework/jdbc/support/sql-error-codes.xml]
2017-04-18 16:33:29,738 [main] INFO  [org.springframework.jdbc.support.SQLErrorCodesFactory] - SQLErrorCodes loaded: [DB2, Derby, H2, HSQL, Informix, MS-SQL, MySQL, Oracle, PostgreSQL, Sybase]
2017-04-18 16:33:29,743 [main] ERROR [com.wlyd.fmcgwms.service.platform.impl.APIForWaasServiceImpl] - >>>WAAS-API：同步商品异常-
### Error updating database.  Cause: java.sql.SQLException: sql injection violation, multi-statement not allow : update cd_wh_package_P0000020 
       SET CDSG_CDSK_CODE = ?,
	      CDSG_NUMBER = ?,
	      CDSG_NAME = ?,
	      CDSG_MAIN_RELATION = ?,
	      CDSG_SUPERIOR_RELATION = ?,
	      CDSG_LENGTH = ?,
	      CDSG_WIDTH = ?,
	      CDSG_HEIGTH = ?,
	      CDSG_WEIGHT = ?,
	      CDSG_IS_DECIMAL = ?,
	      CDSG_LEVEL = ?,
	      CDSG_BASE_NUMBER = ?,
	      CDSG_IS_MAIN_UNIT = ? 
        where CDSG_ID = ?
        ; 
      update cd_wh_package_P0000020 
       SET CDSG_CDSK_CODE = ?,
	      CDSG_NUMBER = ?,
	      CDSG_NAME = ?,
	      CDSG_MAIN_RELATION = ?,
	      CDSG_SUPERIOR_RELATION = ?,
	      CDSG_LENGTH = ?,
	      CDSG_WIDTH = ?,
	      CDSG_HEIGTH = ?,
	      CDSG_WEIGHT = ?,
	      CDSG_IS_DECIMAL = ?,
	      CDSG_LEVEL = ?,
	      CDSG_BASE_NUMBER = ?,
	      CDSG_IS_MAIN_UNIT = ? 
        where CDSG_ID = ?
### SQL: update cd_wh_package_P0000020         SET CDSG_CDSK_CODE = ?,        CDSG_NUMBER = ?,        CDSG_NAME = ?,        CDSG_MAIN_RELATION = ?,        CDSG_SUPERIOR_RELATION = ?,        CDSG_LENGTH = ?,        CDSG_WIDTH = ?,        CDSG_HEIGTH = ?,        CDSG_WEIGHT = ?,        CDSG_IS_DECIMAL = ?,        CDSG_LEVEL = ?,        CDSG_BASE_NUMBER = ?,        CDSG_IS_MAIN_UNIT = ?          where CDSG_ID = ?         ;        update cd_wh_package_P0000020         SET CDSG_CDSK_CODE = ?,        CDSG_NUMBER = ?,        CDSG_NAME = ?,        CDSG_MAIN_RELATION = ?,        CDSG_SUPERIOR_RELATION = ?,        CDSG_LENGTH = ?,        CDSG_WIDTH = ?,        CDSG_HEIGTH = ?,        CDSG_WEIGHT = ?,        CDSG_IS_DECIMAL = ?,        CDSG_LEVEL = ?,        CDSG_BASE_NUMBER = ?,        CDSG_IS_MAIN_UNIT = ?          where CDSG_ID = ?
### Cause: java.sql.SQLException: sql injection violation, multi-statement not allow : update cd_wh_package_P0000020 
       SET CDSG_CDSK_CODE = ?,
	      CDSG_NUMBER = ?,
	      CDSG_NAME = ?,
	      CDSG_MAIN_RELATION = ?,
	      CDSG_SUPERIOR_RELATION = ?,
	      CDSG_LENGTH = ?,
	      CDSG_WIDTH = ?,
	      CDSG_HEIGTH = ?,
	      CDSG_WEIGHT = ?,
	      CDSG_IS_DECIMAL = ?,
	      CDSG_LEVEL = ?,
	      CDSG_BASE_NUMBER = ?,
	      CDSG_IS_MAIN_UNIT = ? 
        where CDSG_ID = ?
        ; 
      update cd_wh_package_P0000020 
       SET CDSG_CDSK_CODE = ?,
	      CDSG_NUMBER = ?,
	      CDSG_NAME = ?,
	      CDSG_MAIN_RELATION = ?,
	      CDSG_SUPERIOR_RELATION = ?,
	      CDSG_LENGTH = ?,
	      CDSG_WIDTH = ?,
	      CDSG_HEIGTH = ?,
	      CDSG_WEIGHT = ?,
	      CDSG_IS_DECIMAL = ?,
	      CDSG_LEVEL = ?,
	      CDSG_BASE_NUMBER = ?,
	      CDSG_IS_MAIN_UNIT = ? 
        where CDSG_ID = ?
; uncategorized SQLException for SQL []; SQL state [null]; error code [0]; sql injection violation, multi-statement not allow : update cd_wh_package_P0000020 
       SET CDSG_CDSK_CODE = ?,
	      CDSG_NUMBER = ?,
	      CDSG_NAME = ?,
	      CDSG_MAIN_RELATION = ?,
	      CDSG_SUPERIOR_RELATION = ?,
	      CDSG_LENGTH = ?,
	      CDSG_WIDTH = ?,
	      CDSG_HEIGTH = ?,
	      CDSG_WEIGHT = ?,
	      CDSG_IS_DECIMAL = ?,
	      CDSG_LEVEL = ?,
	      CDSG_BASE_NUMBER = ?,
	      CDSG_IS_MAIN_UNIT = ? 
        where CDSG_ID = ?
        ; 
      update cd_wh_package_P0000020 
       SET CDSG_CDSK_CODE = ?,
	      CDSG_NUMBER = ?,
	      CDSG_NAME = ?,
	      CDSG_MAIN_RELATION = ?,
	      CDSG_SUPERIOR_RELATION = ?,
	      CDSG_LENGTH = ?,
	      CDSG_WIDTH = ?,
	      CDSG_HEIGTH = ?,
	      CDSG_WEIGHT = ?,
	      CDSG_IS_DECIMAL = ?,
	      CDSG_LEVEL = ?,
	      CDSG_BASE_NUMBER = ?,
	      CDSG_IS_MAIN_UNIT = ? 
        where CDSG_ID = ?; nested exception is java.sql.SQLException: sql injection violation, multi-statement not allow : update cd_wh_package_P0000020 
       SET CDSG_CDSK_CODE = ?,
	      CDSG_NUMBER = ?,
	      CDSG_NAME = ?,
	      CDSG_MAIN_RELATION = ?,
	      CDSG_SUPERIOR_RELATION = ?,
	      CDSG_LENGTH = ?,
	      CDSG_WIDTH = ?,
	      CDSG_HEIGTH = ?,
	      CDSG_WEIGHT = ?,
	      CDSG_IS_DECIMAL = ?,
	      CDSG_LEVEL = ?,
	      CDSG_BASE_NUMBER = ?,
	      CDSG_IS_MAIN_UNIT = ? 
        where CDSG_ID = ?
        ; 
      update cd_wh_package_P0000020 
       SET CDSG_CDSK_CODE = ?,
	      CDSG_NUMBER = ?,
	      CDSG_NAME = ?,
	      CDSG_MAIN_RELATION = ?,
	      CDSG_SUPERIOR_RELATION = ?,
	      CDSG_LENGTH = ?,
	      CDSG_WIDTH = ?,
	      CDSG_HEIGTH = ?,
	      CDSG_WEIGHT = ?,
	      CDSG_IS_DECIMAL = ?,
	      CDSG_LEVEL = ?,
	      CDSG_BASE_NUMBER = ?,
	      CDSG_IS_MAIN_UNIT = ? 
        where CDSG_ID = ?
org.springframework.jdbc.UncategorizedSQLException: 
### Error updating database.  Cause: java.sql.SQLException: sql injection violation, multi-statement not allow : update cd_wh_package_P0000020 
       SET CDSG_CDSK_CODE = ?,
	      CDSG_NUMBER = ?,
	      CDSG_NAME = ?,
	      CDSG_MAIN_RELATION = ?,
	      CDSG_SUPERIOR_RELATION = ?,
	      CDSG_LENGTH = ?,
	      CDSG_WIDTH = ?,
	      CDSG_HEIGTH = ?,
	      CDSG_WEIGHT = ?,
	      CDSG_IS_DECIMAL = ?,
	      CDSG_LEVEL = ?,
	      CDSG_BASE_NUMBER = ?,
	      CDSG_IS_MAIN_UNIT = ? 
        where CDSG_ID = ?
        ; 
      update cd_wh_package_P0000020 
       SET CDSG_CDSK_CODE = ?,
	      CDSG_NUMBER = ?,
	      CDSG_NAME = ?,
	      CDSG_MAIN_RELATION = ?,
	      CDSG_SUPERIOR_RELATION = ?,
	      CDSG_LENGTH = ?,
	      CDSG_WIDTH = ?,
	      CDSG_HEIGTH = ?,
	      CDSG_WEIGHT = ?,
	      CDSG_IS_DECIMAL = ?,
	      CDSG_LEVEL = ?,
	      CDSG_BASE_NUMBER = ?,
	      CDSG_IS_MAIN_UNIT = ? 
        where CDSG_ID = ?
### SQL: update cd_wh_package_P0000020         SET CDSG_CDSK_CODE = ?,        CDSG_NUMBER = ?,        CDSG_NAME = ?,        CDSG_MAIN_RELATION = ?,        CDSG_SUPERIOR_RELATION = ?,        CDSG_LENGTH = ?,        CDSG_WIDTH = ?,        CDSG_HEIGTH = ?,        CDSG_WEIGHT = ?,        CDSG_IS_DECIMAL = ?,        CDSG_LEVEL = ?,        CDSG_BASE_NUMBER = ?,        CDSG_IS_MAIN_UNIT = ?          where CDSG_ID = ?         ;        update cd_wh_package_P0000020         SET CDSG_CDSK_CODE = ?,        CDSG_NUMBER = ?,        CDSG_NAME = ?,        CDSG_MAIN_RELATION = ?,        CDSG_SUPERIOR_RELATION = ?,        CDSG_LENGTH = ?,        CDSG_WIDTH = ?,        CDSG_HEIGTH = ?,        CDSG_WEIGHT = ?,        CDSG_IS_DECIMAL = ?,        CDSG_LEVEL = ?,        CDSG_BASE_NUMBER = ?,        CDSG_IS_MAIN_UNIT = ?          where CDSG_ID = ?
### Cause: java.sql.SQLException: sql injection violation, multi-statement not allow : update cd_wh_package_P0000020 
       SET CDSG_CDSK_CODE = ?,
	      CDSG_NUMBER = ?,
	      CDSG_NAME = ?,
	      CDSG_MAIN_RELATION = ?,
	      CDSG_SUPERIOR_RELATION = ?,
	      CDSG_LENGTH = ?,
	      CDSG_WIDTH = ?,
	      CDSG_HEIGTH = ?,
	      CDSG_WEIGHT = ?,
	      CDSG_IS_DECIMAL = ?,
	      CDSG_LEVEL = ?,
	      CDSG_BASE_NUMBER = ?,
	      CDSG_IS_MAIN_UNIT = ? 
        where CDSG_ID = ?
        ; 
      update cd_wh_package_P0000020 
       SET CDSG_CDSK_CODE = ?,
	      CDSG_NUMBER = ?,
	      CDSG_NAME = ?,
	      CDSG_MAIN_RELATION = ?,
	      CDSG_SUPERIOR_RELATION = ?,
	      CDSG_LENGTH = ?,
	      CDSG_WIDTH = ?,
	      CDSG_HEIGTH = ?,
	      CDSG_WEIGHT = ?,
	      CDSG_IS_DECIMAL = ?,
	      CDSG_LEVEL = ?,
	      CDSG_BASE_NUMBER = ?,
	      CDSG_IS_MAIN_UNIT = ? 
        where CDSG_ID = ?
; uncategorized SQLException for SQL []; SQL state [null]; error code [0]; sql injection violation, multi-statement not allow : update cd_wh_package_P0000020 
       SET CDSG_CDSK_CODE = ?,
	      CDSG_NUMBER = ?,
	      CDSG_NAME = ?,
	      CDSG_MAIN_RELATION = ?,
	      CDSG_SUPERIOR_RELATION = ?,
	      CDSG_LENGTH = ?,
	      CDSG_WIDTH = ?,
	      CDSG_HEIGTH = ?,
	      CDSG_WEIGHT = ?,
	      CDSG_IS_DECIMAL = ?,
	      CDSG_LEVEL = ?,
	      CDSG_BASE_NUMBER = ?,
	      CDSG_IS_MAIN_UNIT = ? 
        where CDSG_ID = ?
        ; 
      update cd_wh_package_P0000020 
       SET CDSG_CDSK_CODE = ?,
	      CDSG_NUMBER = ?,
	      CDSG_NAME = ?,
	      CDSG_MAIN_RELATION = ?,
	      CDSG_SUPERIOR_RELATION = ?,
	      CDSG_LENGTH = ?,
	      CDSG_WIDTH = ?,
	      CDSG_HEIGTH = ?,
	      CDSG_WEIGHT = ?,
	      CDSG_IS_DECIMAL = ?,
	      CDSG_LEVEL = ?,
	      CDSG_BASE_NUMBER = ?,
	      CDSG_IS_MAIN_UNIT = ? 
        where CDSG_ID = ?; nested exception is java.sql.SQLException: sql injection violation, multi-statement not allow : update cd_wh_package_P0000020 
       SET CDSG_CDSK_CODE = ?,
	      CDSG_NUMBER = ?,
	      CDSG_NAME = ?,
	      CDSG_MAIN_RELATION = ?,
	      CDSG_SUPERIOR_RELATION = ?,
	      CDSG_LENGTH = ?,
	      CDSG_WIDTH = ?,
	      CDSG_HEIGTH = ?,
	      CDSG_WEIGHT = ?,
	      CDSG_IS_DECIMAL = ?,
	      CDSG_LEVEL = ?,
	      CDSG_BASE_NUMBER = ?,
	      CDSG_IS_MAIN_UNIT = ? 
        where CDSG_ID = ?
        ; 
      update cd_wh_package_P0000020 
       SET CDSG_CDSK_CODE = ?,
	      CDSG_NUMBER = ?,
	      CDSG_NAME = ?,
	      CDSG_MAIN_RELATION = ?,
	      CDSG_SUPERIOR_RELATION = ?,
	      CDSG_LENGTH = ?,
	      CDSG_WIDTH = ?,
	      CDSG_HEIGTH = ?,
	      CDSG_WEIGHT = ?,
	      CDSG_IS_DECIMAL = ?,
	      CDSG_LEVEL = ?,
	      CDSG_BASE_NUMBER = ?,
	      CDSG_IS_MAIN_UNIT = ? 
        where CDSG_ID = ?
	at org.springframework.jdbc.support.AbstractFallbackSQLExceptionTranslator.translate(AbstractFallbackSQLExceptionTranslator.java:83)
	at org.springframework.jdbc.support.AbstractFallbackSQLExceptionTranslator.translate(AbstractFallbackSQLExceptionTranslator.java:80)
	at org.springframework.jdbc.support.AbstractFallbackSQLExceptionTranslator.translate(AbstractFallbackSQLExceptionTranslator.java:80)
	at org.mybatis.spring.MyBatisExceptionTranslator.translateExceptionIfPossible(MyBatisExceptionTranslator.java:73)
	at org.mybatis.spring.SqlSessionTemplate$SqlSessionInterceptor.invoke(SqlSessionTemplate.java:368)
	at com.sun.proxy.$Proxy13.update(Unknown Source)
	at org.mybatis.spring.SqlSessionTemplate.update(SqlSessionTemplate.java:254)
	at org.apache.ibatis.binding.MapperMethod.execute(MapperMethod.java:82)
	at org.apache.ibatis.binding.MapperProxy.invoke(MapperProxy.java:40)
	at com.sun.proxy.$Proxy21.updateBatch(Unknown Source)
	at com.wlyd.fmcgwms.service.platform.impl.APIForWaasServiceImpl.insertOrUpdateProduct(APIForWaasServiceImpl.java:992)
	at sun.reflect.NativeMethodAccessorImpl.invoke0(Native Method)
	at sun.reflect.NativeMethodAccessorImpl.invoke(NativeMethodAccessorImpl.java:57)
	at sun.reflect.DelegatingMethodAccessorImpl.invoke(DelegatingMethodAccessorImpl.java:43)
	at java.lang.reflect.Method.invoke(Method.java:606)
	at org.springframework.aop.support.AopUtils.invokeJoinpointUsingReflection(AopUtils.java:317)
	at org.springframework.aop.framework.ReflectiveMethodInvocation.invokeJoinpoint(ReflectiveMethodInvocation.java:183)
	at org.springframework.aop.framework.ReflectiveMethodInvocation.proceed(ReflectiveMethodInvocation.java:150)
	at org.springframework.transaction.interceptor.TransactionInterceptor$1.proceedWithInvocation(TransactionInterceptor.java:96)
	at org.springframework.transaction.interceptor.TransactionAspectSupport.invokeWithinTransaction(TransactionAspectSupport.java:260)
	at org.springframework.transaction.interceptor.TransactionInterceptor.invoke(TransactionInterceptor.java:94)
	at org.springframework.aop.framework.ReflectiveMethodInvocation.proceed(ReflectiveMethodInvocation.java:172)
	at com.alibaba.druid.support.spring.stat.DruidStatInterceptor.invoke(DruidStatInterceptor.java:73)
	at org.springframework.aop.framework.ReflectiveMethodInvocation.proceed(ReflectiveMethodInvocation.java:172)
	at org.springframework.aop.interceptor.ExposeInvocationInterceptor.invoke(ExposeInvocationInterceptor.java:91)
	at org.springframework.aop.framework.ReflectiveMethodInvocation.proceed(ReflectiveMethodInvocation.java:172)
	at org.springframework.aop.framework.JdkDynamicAopProxy.invoke(JdkDynamicAopProxy.java:204)
	at com.sun.proxy.$Proxy52.insertOrUpdateProduct(Unknown Source)
	at fmcgwms.OpenAPIControllerTest.testProductJSON(OpenAPIControllerTest.java:471)
	at sun.reflect.NativeMethodAccessorImpl.invoke0(Native Method)
	at sun.reflect.NativeMethodAccessorImpl.invoke(NativeMethodAccessorImpl.java:57)
	at sun.reflect.DelegatingMethodAccessorImpl.invoke(DelegatingMethodAccessorImpl.java:43)
	at java.lang.reflect.Method.invoke(Method.java:606)
	at org.junit.runners.model.FrameworkMethod$1.runReflectiveCall(FrameworkMethod.java:47)
	at org.junit.internal.runners.model.ReflectiveCallable.run(ReflectiveCallable.java:12)
	at org.junit.runners.model.FrameworkMethod.invokeExplosively(FrameworkMethod.java:44)
	at org.junit.internal.runners.statements.InvokeMethod.evaluate(InvokeMethod.java:17)
	at org.springframework.test.context.junit4.statements.RunBeforeTestMethodCallbacks.evaluate(RunBeforeTestMethodCallbacks.java:74)
	at org.springframework.test.context.junit4.statements.RunAfterTestMethodCallbacks.evaluate(RunAfterTestMethodCallbacks.java:83)
	at org.springframework.test.context.junit4.statements.SpringRepeat.evaluate(SpringRepeat.java:72)
	at org.springframework.test.context.junit4.SpringJUnit4ClassRunner.runChild(SpringJUnit4ClassRunner.java:231)
	at org.springframework.test.context.junit4.SpringJUnit4ClassRunner.runChild(SpringJUnit4ClassRunner.java:88)
	at org.junit.runners.ParentRunner$3.run(ParentRunner.java:238)
	at org.junit.runners.ParentRunner$1.schedule(ParentRunner.java:63)
	at org.junit.runners.ParentRunner.runChildren(ParentRunner.java:236)
	at org.junit.runners.ParentRunner.access$000(ParentRunner.java:53)
	at org.junit.runners.ParentRunner$2.evaluate(ParentRunner.java:229)
	at org.springframework.test.context.junit4.statements.RunBeforeTestClassCallbacks.evaluate(RunBeforeTestClassCallbacks.java:61)
	at org.springframework.test.context.junit4.statements.RunAfterTestClassCallbacks.evaluate(RunAfterTestClassCallbacks.java:71)
	at org.junit.runners.ParentRunner.run(ParentRunner.java:309)
	at org.springframework.test.context.junit4.SpringJUnit4ClassRunner.run(SpringJUnit4ClassRunner.java:174)
	at org.eclipse.jdt.internal.junit4.runner.JUnit4TestReference.run(JUnit4TestReference.java:50)
	at org.eclipse.jdt.internal.junit.runner.TestExecution.run(TestExecution.java:38)
	at org.eclipse.jdt.internal.junit.runner.RemoteTestRunner.runTests(RemoteTestRunner.java:459)
	at org.eclipse.jdt.internal.junit.runner.RemoteTestRunner.runTests(RemoteTestRunner.java:675)
	at org.eclipse.jdt.internal.junit.runner.RemoteTestRunner.run(RemoteTestRunner.java:382)
	at org.eclipse.jdt.internal.junit.runner.RemoteTestRunner.main(RemoteTestRunner.java:192)
Caused by: java.sql.SQLException: sql injection violation, multi-statement not allow : update cd_wh_package_P0000020 
       SET CDSG_CDSK_CODE = ?,
	      CDSG_NUMBER = ?,
	      CDSG_NAME = ?,
	      CDSG_MAIN_RELATION = ?,
	      CDSG_SUPERIOR_RELATION = ?,
	      CDSG_LENGTH = ?,
	      CDSG_WIDTH = ?,
	      CDSG_HEIGTH = ?,
	      CDSG_WEIGHT = ?,
	      CDSG_IS_DECIMAL = ?,
	      CDSG_LEVEL = ?,
	      CDSG_BASE_NUMBER = ?,
	      CDSG_IS_MAIN_UNIT = ? 
        where CDSG_ID = ?
        ; 
      update cd_wh_package_P0000020 
       SET CDSG_CDSK_CODE = ?,
	      CDSG_NUMBER = ?,
	      CDSG_NAME = ?,
	      CDSG_MAIN_RELATION = ?,
	      CDSG_SUPERIOR_RELATION = ?,
	      CDSG_LENGTH = ?,
	      CDSG_WIDTH = ?,
	      CDSG_HEIGTH = ?,
	      CDSG_WEIGHT = ?,
	      CDSG_IS_DECIMAL = ?,
	      CDSG_LEVEL = ?,
	      CDSG_BASE_NUMBER = ?,
	      CDSG_IS_MAIN_UNIT = ? 
        where CDSG_ID = ?
	at com.alibaba.druid.wall.WallFilter.check(WallFilter.java:708)
	at com.alibaba.druid.wall.WallFilter.connection_prepareStatement(WallFilter.java:233)
	at com.alibaba.druid.filter.FilterChainImpl.connection_prepareStatement(FilterChainImpl.java:448)
	at com.alibaba.druid.proxy.jdbc.ConnectionProxyImpl.prepareStatement(ConnectionProxyImpl.java:342)
	at com.alibaba.druid.pool.DruidPooledConnection.prepareStatement(DruidPooledConnection.java:311)
	at sun.reflect.NativeMethodAccessorImpl.invoke0(Native Method)
	at sun.reflect.NativeMethodAccessorImpl.invoke(NativeMethodAccessorImpl.java:57)
	at sun.reflect.DelegatingMethodAccessorImpl.invoke(DelegatingMethodAccessorImpl.java:43)
	at java.lang.reflect.Method.invoke(Method.java:606)
	at org.apache.ibatis.logging.jdbc.ConnectionLogger.invoke(ConnectionLogger.java:53)
	at com.sun.proxy.$Proxy173.prepareStatement(Unknown Source)
	at org.apache.ibatis.executor.statement.PreparedStatementHandler.instantiateStatement(PreparedStatementHandler.java:72)
	at org.apache.ibatis.executor.statement.BaseStatementHandler.prepare(BaseStatementHandler.java:82)
	at org.apache.ibatis.executor.statement.RoutingStatementHandler.prepare(RoutingStatementHandler.java:54)
	at sun.reflect.NativeMethodAccessorImpl.invoke0(Native Method)
	at sun.reflect.NativeMethodAccessorImpl.invoke(NativeMethodAccessorImpl.java:57)
	at sun.reflect.DelegatingMethodAccessorImpl.invoke(DelegatingMethodAccessorImpl.java:43)
	at java.lang.reflect.Method.invoke(Method.java:606)
	at org.apache.ibatis.plugin.Invocation.proceed(Invocation.java:46)
	at com.wlyd.fmcgwms.util.mybatis.PagePlugin.intercept(PagePlugin.java:101)
	at org.apache.ibatis.plugin.Plugin.invoke(Plugin.java:57)
	at com.sun.proxy.$Proxy172.prepare(Unknown Source)
	at org.apache.ibatis.executor.SimpleExecutor.prepareStatement(SimpleExecutor.java:70)
	at org.apache.ibatis.executor.SimpleExecutor.doUpdate(SimpleExecutor.java:44)
	at org.apache.ibatis.executor.BaseExecutor.update(BaseExecutor.java:108)
	at org.apache.ibatis.executor.CachingExecutor.update(CachingExecutor.java:75)
	at org.apache.ibatis.session.defaults.DefaultSqlSession.update(DefaultSqlSession.java:145)
	at sun.reflect.NativeMethodAccessorImpl.invoke0(Native Method)
	at sun.reflect.NativeMethodAccessorImpl.invoke(NativeMethodAccessorImpl.java:57)
	at sun.reflect.DelegatingMethodAccessorImpl.invoke(DelegatingMethodAccessorImpl.java:43)
	at java.lang.reflect.Method.invoke(Method.java:606)
	at org.mybatis.spring.SqlSessionTemplate$SqlSessionInterceptor.invoke(SqlSessionTemplate.java:358)
	... 52 more
同步商品结果:{"IsSuccess":false,"OperationDesc":"商品同步异常","ResultCode":506}
```





## MySQL批量执行操作

注：下面是mybatis配置mapper.xml执行商品的包装和属性批量更新.


packages:




```
<!-- 批量更新-->  
  <update id="updateBatch" >
       <foreach collection="list" item="item" index="index" open="" close="" separator=";" >
      update cd_wh_package_${tableName}
      <set>
          CDSG_CDSK_CODE = #{item.cdsgCdskCode,jdbcType=VARCHAR},
          CDSG_NUMBER = #{item.cdsgNumber,jdbcType=VARCHAR},
          CDSG_NAME = #{item.cdsgName,jdbcType=VARCHAR},
          CDSG_MAIN_RELATION = #{item.cdsgMainRelation,jdbcType=INTEGER},
          CDSG_SUPERIOR_RELATION = #{item.cdsgSuperiorRelation,jdbcType=INTEGER},
          CDSG_LENGTH = #{item.cdsgLength,jdbcType=DECIMAL},
          CDSG_WIDTH = #{item.cdsgWidth,jdbcType=DECIMAL},
          CDSG_HEIGTH = #{item.cdsgHeigth,jdbcType=DECIMAL},
          CDSG_WEIGHT = #{item.cdsgWeight,jdbcType=DECIMAL},
          CDSG_IS_DECIMAL = #{item.cdsgIsDecimal,jdbcType=INTEGER},
          CDSG_LEVEL = #{item.cdsgLevel,jdbcType=INTEGER},
          CDSG_BASE_NUMBER = #{item.cdsgBaseNumber,jdbcType=INTEGER},
          CDSG_IS_MAIN_UNIT = #{item.cdsgIsMainUnit,jdbcType=INTEGER}
        </set>
        where CDSG_ID = #{item.cdsgId,jdbcType=INTEGER}
       </foreach>
  </update>
```

property:




```
<!-- 批量更新-->  
  <update id="updateBatch" >
       <foreach collection="list" item="item" index="index" open="" close="" separator=";" >
  update cd_wh_property_${tableName}
       <set>
          CDSP_CDSK_CODE = #{item.cdspCdskCode,jdbcType=VARCHAR},
          CDSP_NUMBER = #{item.cdspNumber,jdbcType=VARCHAR},
          CDSP_NAME = #{item.cdspName,jdbcType=VARCHAR},
          CDSP_VALUE = #{item.cdspValue,jdbcType=VARCHAR},
          CDSP_REMARK = #{item.cdspRemark,jdbcType=VARCHAR},
          CDSP_SORTNUM = #{item.cdspSortnum,jdbcType=INTEGER},
          CDSP_IS_UOM = #{item.cdspIsUom,jdbcType=INTEGER},
          CDSP_IS_REQUIRED = #{item.cdspIsRequired,jdbcType=INTEGER},
          CDSP_FIELD_TYPE = #{item.cdspFieldType,jdbcType=INTEGER}
       </set>
       where CDSP_ID = #{item.cdspId,jdbcType=INTEGER}
       </foreach>
  </update>
```




## 分析需要修改的设置属性值

控制台第一句就报出了check()的异常：


![](https://img-blog.csdn.net/20170418171901549)







![](https://img-blog.csdn.net/20170418171923135)




![](https://img-blog.csdn.net/20170418171935198)




![](https://img-blog.csdn.net/20170418171949565)




![](https://img-blog.csdn.net/20170418171959549)




上面的config是WallConfig:




![](https://img-blog.csdn.net/20170418171912198)




## Spring修改设置数据源和WallFilter



```
<!--允许多个批量处理配置==修改后存在SQL注入风险 -->
    <bean id="myWallConfig" class="com.alibaba.druid.wall.WallConfig">  
            <property name="multiStatementAllow" value="true" />  
    </bean>  
     
    <!--重新定义过滤器，允许多个批量处理配置 默认是wall-->
    <bean id="wall-filter" class="com.alibaba.druid.wall.WallFilter">
        <property name="config" ref="myWallConfig"/>  
    </bean>
    
    <bean id="dataSource" class="com.alibaba.druid.pool.DruidDataSource"
        init-method="init" destroy-method="close">
        <property name="driverClassName">
            <value>${jdbc.driverClass}</value>
        </property>
        <property name="url">
            <value>${jdbc.jdbcUrl}</value>
        </property>
        <property name="username">
            <value>${jdbc.user}</value>
        </property>
        <property name="password">
            <value>${jdbc.password}</value>
        </property>
        <!-- 连接池最大使用连接数  -->
        <property name="maxActive">
            <value>100</value>
        </property> 
        <!-- 初始化连接大小 -->
        <property name="initialSize">
            <value>10</value>
        </property>
        <!-- 获取连接最大等待时间 -->
        <property name="maxWait">
            <value>60000</value>
        </property>
        <!-- 连接池最大空闲   已经失效的参数
        <property name="maxIdle">
            <value>50</value>
        </property>-->
        <!-- 连接池最小空闲 -->
        <property name="minIdle">
            <value>10</value>
        </property>
        <!-- 自动清除无用连接 -->
        <property name="removeAbandoned">
            <value>true</value>
        </property>
        <!-- 清除无用连接的等待时间 -->
        <property name="removeAbandonedTimeout">
            <value>1800</value>
        </property>
        <!-- 连接属性 -->
        <property name="connectionProperties">
            <value>clientEncoding=UTF-8</value>
        </property>
        <!-- 配置监控统计拦截的filters -->
        <!-- <property name="filters" value="wall,stat" /> -->
        <property name="filters" value="stat" /> 
        <!-- JDBC Proxy Driver -->
        <property name="proxyFilters">
          <list>
              <ref bean="wall-filter"/>
          </list>
      </property>
        
    </bean>
    <!-- Druid 配置 end -->
```

Druid默认的WallFilter是wall.如果启动时有错误信息需要去查看WallFilter的配置。



## Druid配置的过滤器



Druid配置的时候还有一个大坑就是，不要自定义WallFilter配置filters，你需要通过proxyFilters来配置。


参考地址：[https://github.com/alibaba/druid/wiki/%E9%85%8D%E7%BD%AE-wallfilter](https://github.com/alibaba/druid/wiki/%E9%85%8D%E7%BD%AE-wallfilter)





DruidDataSource继承了DruidAbstractDataSource，


![](https://img-blog.csdn.net/20160128135016408?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQv/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)


![](https://img-blog.csdn.net/20160128135109237?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQv/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)


可以看出来，既可以配置filters，也可以配置proxyFilters，不同的是，filters是字符串别名，proxyFilters是类。

我们继续看一下这些字符串的值应该是啥样的：

![](https://img-blog.csdn.net/20160128135406254?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQv/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)


![](https://img-blog.csdn.net/20160128135452086?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQv/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)


![](https://img-blog.csdn.net/20160128135538505?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQv/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)




![](https://img-blog.csdn.net/20160128135725840?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQv/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)
过滤器配置：


![](https://img-blog.csdn.net/20160128135837748?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQv/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)




截图参考的是：[http://blog.csdn.net/goldenfish1919/article/details/50600053](http://blog.csdn.net/goldenfish1919/article/details/50600053)



