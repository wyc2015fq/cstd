# Mybatis generator 自动生成代码时 ，insert如何返回主键值 - z69183787的专栏 - CSDN博客
2016年06月02日 12:31:22[OkidoGreen](https://me.csdn.net/z69183787)阅读数：13336
mvn mybatis-generator:generate
如果要让generator自动添加该功能，可以如下配置：
<!-- tableName:用于自动生成代码的数据库表；domainObjectName:对应于数据库表的javaBean类名；不需要生成Example类 -->
        <table schema="" tableName="ACT_SecurityBlockLog" domainObjectName="BlockLog"
               enableCountByExample="false" enableUpdateByExample="false"
               enableDeleteByExample="false" enableSelectByExample="false"
               selectByExampleQueryId="false">
            <property name="useActualColumnNames" value="true"/>
**<generatedKey column="id" sqlStatement="MySql" identity="true"/>**
        </table>
或者手动增加：
<insert id="insert" parameterType="Activity" **keyProperty="id"     keyColumn="ID" useGeneratedKeys="true"**>
主键值有  对象.getId()获得
方法只返回成功影响的行数：1
在insert里面加入selectKey标签就可以了. 一般都是返回的int类型.对应数据库是自增长字段.
要注意的是: ibatis会直接返回int值. Mybatis则把int值包装在参数对象里面.
public int insert(User user) {
//ibatis方式.
int result = UserMapper.insert(user);
return result;
//Mybatis方式
user = UserMapper.insert(user);
return user.getId();
}
还要注意的是数据库类型不一样,生成ID的策略也不一样. 可以对selectKey添加属性(名字忘记了), pre---先生成ID. post---后生成ID. default是post.
另一种返回主键值得方法：
<insert id="insertSelective" parameterType="xxxx" >
**<selectKey resultType="java.lang.Integer" keyProperty="ID" order="AFTER" >      SELECT LAST_INSERT_ID()    </selectKey>**
    insert into XXXX
最终结果是 mapper方法返回值为主键ID，同时对象ID值也为主键ID
