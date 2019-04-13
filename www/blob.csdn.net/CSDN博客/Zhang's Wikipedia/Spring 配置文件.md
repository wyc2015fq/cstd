
# Spring 配置文件 - Zhang's Wikipedia - CSDN博客


2018年06月05日 22:22:04[Inside_Zhang](https://me.csdn.net/lanchunhui)阅读数：563



```python
<?xml version=
```
```python
"1.0"
```
```python
encoding=
```
```python
"UTF-8"
```
```python
?>
<beans>
    <bean id=
```
```python
...
```
```python
>
    <bean id=
```
```python
...
```
```python
>
</beans>
```
<context:component-scan base-package="....">：自动扫描以创建 bean 实例；
## 1. 数据库配置
<context:property-placeholder />
<context:property-placeholder location="classpath:jdbc.properties"/>数据库配置文件（jdbc.properties）在类路径（classpath，src/main/resources/）下；
<bean id="dataSource"class="org.apache.commons.dbcp.BasicDataSource"destroy-method="close"p:driverClassName="${jdbc.driverClassName}"p:url="${jdbc.url}"p:username="${jdbc.username}"p:password="${jdbc.password}"/>

