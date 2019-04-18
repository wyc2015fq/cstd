# 去掉Mybatis Generator生成的一堆 example - z69183787的专栏 - CSDN博客
2016年04月29日 09:40:31[OkidoGreen](https://me.csdn.net/z69183787)阅读数：5621
mybatis generator自动生成的代码里老是有一堆example，虽然说他封装的东西比较多，但是大部分时候并不会用到。如果需要改的时候，在接口和mapper里修改便是了。
配置的方法如下：
```
![复制代码](http://common.cnblogs.com/images/copycode.gif)
<table schema="general" tableName="tb_table_name" domainObjectName="EntityName"
    enableCountByExample="false" enableUpdateByExample="false" enableDeleteByExample="false"
    enableSelectByExample="false" selectByExampleQueryId="false" >
    <property name="useActualColumnNames" value="true"/>
</table>
![复制代码](http://common.cnblogs.com/images/copycode.gif)
```
