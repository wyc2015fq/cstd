# hibernate 联合查询返回多个对象--引用 - z69183787的专栏 - CSDN博客
2014年01月15日 18:55:53[OkidoGreen](https://me.csdn.net/z69183787)阅读数：9016
一、 Hibernate简介Hibernate是一个JDO工具。它的工作原理是通过文件（一般有两种：xml文件和properties文件）把值对象和数据库表之间建立起一个映射关系。这样，我们只需要通过操作这些值对象和Hibernate提供的一些基本类，就可以达到使用数据库的目的。例如，使用Hibernate的查询，可以直接返回包含某个值对象的列表（List），而不必向传统的JDBC访问方式一样把结果集的数据逐个装载到一个值对象中，为编码工作节约了大量的时间。Hibernate提供的HQL是一种类SQL语言，它和EJBQL一样都是提供对象化的数据库查询方式，但HQL在功能和使用方式上都非常接近于标准的SQL.
    二、 Hibernate与JDBC的区别Hibernate与JDBC的主要区别如下：
    1、 Hibernate是JDBC的轻量级的对象封装，它是一个独立的对象持久层框架，和App Server，和EJB没有什么必然的联系。Hibernate可以用在任何JDBC可以使用的场合，从某种意义上来说，Hibernate在任何场合下取代JDBC. 
    2、 Hibernate是一个和JDBC密切关联的框架，所以Hibernate的兼容性和JDBC驱动，和数据库都有一定的关系，但是和使用它的[Java](http://java.chinaitlab.com/)程序，和App Server没有任何关系，也不存在兼容性问题。
    3、 Hibernate是做为JDBC的替代者出现的，不能用来直接和Entity Bean做对比。
    三、 Hibernate 进行多表关联查询Hibernate对多个表进行查询时，查询结果是多个表的笛卡尔积，或者称为“交叉”连接。 例如：from Student， Book from Student as stu， Book as boo from Student stu， Book boo注意：让查询中的Student和Book均是表student和book对应的类名，它的名字一定要和类的名字相同，包括字母的大小写。别名应该服从首字母小写的规则是一个好习惯，这和[Java](http://java.chinaitlab.com/)对局部变量的命名规范是一致的。
    下面列举一个完整的例子来说明Hibernate对多个表进行关联查询（其中粗体是我们要特别注意的地方，相应表tBookInfo和BookSelection的结构和其对应的hbm.xml、class文件就不一一列举了，有需要详细了解的朋友，请联系我）：
String sTest = "from tBookInfo book, BookSelection sel where book.id = sel.bookId"; 
Collection result = new ArrayList(); 
Transaction tx = null; 
try { 
Session session = HibernateUtil.currentSession(); 
tx = session.beginTransaction(); 
Query query = session.createQuery(sql); 
result = query.list(); 
tx.commit(); 
} catch (Exception e) { 
throw e; 
} finally { 
HibernateUtil.closeSession(); 
} 
ArrayList sList = (ArrayList) result; 
Iterator iterator1 = sList.iterator(); 
while (iterator1.hasNext()) { 
Object[] o = (Object[]) iterator1.next(); 
tBookInfo bookInfo = (tBookInfo) o[0]; 
BookSelection bookSelect = (BookSelection) o[1]; 
System.out.println("BookInfo-Title: " + bookInfo.getTitle()); 
System.out.println("BookSelection-BookSelectionId: " + bookSelect.getId()); 
}
