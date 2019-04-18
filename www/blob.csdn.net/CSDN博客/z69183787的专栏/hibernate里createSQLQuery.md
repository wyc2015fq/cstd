# hibernate里createSQLQuery - z69183787的专栏 - CSDN博客
2013年12月09日 23:17:22[OkidoGreen](https://me.csdn.net/z69183787)阅读数：804
个人分类：[对象映射框架-Hibernate](https://blog.csdn.net/z69183787/article/category/2175489)
一、addEntity()和setResultTransformer（）方法
1.
使用SQLQuery
对原生SQL查询执行的控制是通过SQLQuery接口进行的，通过执行Session.createSQLQuery()获取这个接口。最简单的情况下，我们可以采用以下形式：
List cats = sess.createSQLQuery( " select * from cats " ).addEntity(Cat. class ).list();
这个查询指定了:
SQL查询字符串
查询返回的实体
这里，结果集字段名被假设为与映射文件中指明的字段名相同。对于连接了多个表的查询，这就可能造成问题，因为可能在多个表中出现同样名字的字段。下面的方法就可以避免字段名重复的问题:
List cats = sess.createSQLQuery( " select {cat.*} from cats cat " ).addEntity( " cat " , Cat. class ).list();
这个查询指定了:
SQL查询语句，它带一个占位符，可以让Hibernate使用字段的别名.
查询返回的实体，和它的SQL表的别名.
addEntity()方法将SQL表的别名和实体类联系起来，并且确定查询结果集的形态。
addJoin()方法可以被用于载入其他的实体和集合的关联.
List cats = sess.createSQLQuery(
" select {cat.*}, {kitten.*} from cats cat, cats kitten where kitten.mother = cat.id " )
.addEntity( " cat " , Cat. class )
.addJoin( " kitten " , " cat.kittens " )
.list();
原生的SQL查询可能返回一个简单的标量值或者一个标量和实体的结合体。
Double max = (Double) sess.createSQLQuery( " select max(cat.weight) as maxWeight from cats cat " )
.addScalar( " maxWeight " , Hibernate.DOUBLE);
.uniqueResult();
除此之外，你还可以在你的hbm文件中描述结果集映射信息，在查询中使用。
List cats = sess.createSQLQuery(
" select {cat.*}, {kitten.*} from cats cat, cats kitten where kitten.mother = cat.id " )
.setResultSetMapping( " catAndKitten " )
.list();
命名SQL查询
可以在映射文档中定义查询的名字,然后就可以象调用一个命名的HQL查询一样直接调用命名SQL查询.在这种情况下,我们不 需要调用addEntity()方法.
< sql - query name = " persons " > 
< return alias = " person " class = " eg.Person " /> 
Select person.NAME AS {person.name},person.AGE AS {person.age},person.SEX AS {person.sex} FROM PERSON person Where person.NAME LIKE :namePattern
</ sql - query >List people = sess.getNamedQuery( " persons " ).setString( " namePattern " , namePattern)
.setMaxResults( 50 )
.list();
2.
使用hibernate3的createSQLQuery遇到的问题
为了给访问加速，把DAO中的一些HQL的操作改成了SQL，其实最主要的原因是：操作的是多张表，返回的数据也来源于多个表的字段；
String sql = “select A.id ID, A.name NAME, B.salary SALARY from employee A , Salary B where.......”；
Query query =getSession().createSQLQuery(sql)
.setResultTransformer(Transformers.aliasToBean(ReturnEmployee.class));
由于返回的ID, NAME, SALARY 非一个和表对应的一个BEAN，所以自己需要建立一个ReturnEmployee的BEAN，属性包括ID, NAME, SALARY；在mysql下调试，成功。
但是在ORACLE环境下却报错：
org.hibernate.PropertyNotFoundException: Could not find setter for ID on class com.ReturnEmployee
经过几个小时的查错，调试，没有发现问题的所在，只能摆脱GOOGLE了，最后在国外的一个论坛上找到了答案：
this is actually a limitation of some databases which return alias all uppercase instead of using the casing you actually specified.
until then use .addScalar(..) to workaround it.
原来是Hibernate对ORALCE的支持有BUG，所以修改代码为：
Query query = getSession().createSQLQuery(sql).addScalar("ID")
.addScalar("NAME").addScalar("SALARY");
就可以了，需要注意的是
List employeeData = query.list();
返回的employeeData 中的数据是object[]，这样取值：
List employeeBean = new ArrayList();
for (int i = 0; i < employeeData.size(); i++) {
Employee employee = new Employee();//把"裸"数据组装到自己的employee类
Object[] object = (Object[]) employeeData.get(i);
employee.setId(object[0].toString());
employee.setName(object[1].toString());
employee.setOrgType(object[2].toString());
employeeBean.add(employee);
} 
另还可以返回一个Map对象，也就是说在在list里包含多个Map，代码如下 
Query query = session.createSQLQuery("select id,name from Tree t where pid in (select id from Tree) ").setResultTransformer(Transformers.ALIAS_TO_ENTITY_MAP); //返回一个map,KEY:为DB中名称一致（大小写一致）遍历list时就可以
Map map = (Map)list.get[i];
map.get("id");map.get("name");来取值。按你的SQL语句select后的字段名来作为map的Key，但这个key必须与数据库中的字段名一模一样。
还可以用作函数方面的。如 
Query query = session.createSQLQuery("select sum(id) SUMID from Tree t where pid in (select id from Tree)
.addScalar("SUMID",Hibernate.INTEGER) //转换类型，按DB中的type转 
.setResultTransformer(Transformers.ALIAS_TO_ENTITY_MAP); //返回一个map,KEY:为DB中名称一致（大小写一致）
直接就map.get("SUMID")可以取值了
还有一点就是这个方法在Hibernate3.2版本上才能正常运行。
查到现在，有了一些眉目，小结如下：
1，oracle的char字段在hibernate里映射为character类型，是varchar的子集。
2，复杂SQL用createSQLQuery方法查询没问题，如果查询多个字段，遍历用object[]造型，下标从0开始输出值，不需要映射文件；如果愿意可以写一个映射bean，方便取用。
3，如果查询SQL中是只有一个字段，那就不能用object[]数组接收，只能用object类接收，直接输出object.toString()，即是这个字段的值。
4，可以用addScalar（String arg,Type type）方法定义要返回的字段类型，如
s.createSQLQuery(shuiQingHQL).addScalar("STCD",Hibernate.STRING).addScalar("STNM")；
这样就解决了CHAR字段类型只出一位字符的问题。
但是需要把其他字段也addScalar（）进来！
5，addScalar（String arg）里的参数是需要大写的！
二、SQLQuery增删改查
Hibernate中使用SQLQuery的查询：
            BaseHibernateDAO dao = new BaseHibernateDAO(); 
            //查询语句
            String strSql  = " select * from hrrole h where h.code like ? and h.id <> ?";
            //设置查询对象
            SQLQuery query = dao.getSession().createSQLQuery(strSql);
            //设置查询参数
            query.setString(0,(String)this.getCode().getText());
            //设置查询参数
            query.setString(1,this.getTextField1().getText().toString());
            //添加到一个类中
            query.addEntity(HrRole.class);
            //结果数据
            ArrayList<HrRole> idlist = (ArrayList<HrRole>) query.list();
            //判断是否有值
            if(idlist.size() != 0){
                 //错误信息
                 this.label2.setText("代码已存在");
            }
Hibernate中使用SQLQuery的更新：
    public void updateR(String Code,String NewCode){
         BaseHibernateDAO dao = new BaseHibernateDAO(); 
        try{
         Transaction tx = dao.getSession().beginTransaction(); 
         String sql1 = "update roleandgrant set rolecode = '" + Code + "'" + " where rolecode ='" + NewCode + "'";
         String sql2 = "update emprole set rolecode = '" + Code + "'" + " where rolecode ='" + NewCode + "'";
         SQLQuery query1 = dao.getSession().createSQLQuery(sql1);
         SQLQuery query2 = dao.getSession().createSQLQuery(sql2);
         query1.addEntity(RoleAndGrant.class);
         query2.addEntity(EmpVsRole.class);
         query2.executeUpdate();
         query1.executeUpdate();
         tx.commit();
         dao.closeSession();
        }catch(Exception ex){
            System.out.println(ex);
            dao.closeSession();
        }
    }
Hibernate中使用SQLQuery的删除：
     public void DeleteR(String Code){
        BaseHibernateDAO dao = new BaseHibernateDAO();
        try{
         Transaction tx = dao.getSession().beginTransaction(); 
         String sql1 = "delete from  roleandgrant where rolecode ='" + Code + "'";
         String sql2 = "delete from emprole where rolecode ='" + Code + "'";
         SQLQuery query1 = dao.getSession().createSQLQuery(sql1);
         SQLQuery query2 = dao.getSession().createSQLQuery(sql2);
         query1.addEntity(RoleAndGrant.class);
         query2.addEntity(EmpVsRole.class);
         query2.executeUpdate();
         query1.executeUpdate();
         tx.commit();
         dao.closeSession();
        }catch(Exception ex){
            System.out.println(ex);
            dao.closeSession();
        }
    }
