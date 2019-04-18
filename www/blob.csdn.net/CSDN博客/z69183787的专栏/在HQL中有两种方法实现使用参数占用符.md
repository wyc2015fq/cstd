# 在HQL中有两种方法实现使用参数占用符 - z69183787的专栏 - CSDN博客
2013年09月27日 16:51:20[OkidoGreen](https://me.csdn.net/z69183787)阅读数：1768
1、使用？
   使用？设置参数占位符，之后通过setString()和setInteger()等方法为其赋值。如：
   Query query = session.createQuery("from Guestbook where id<?");
   query.setInteger(0,12);
2、使用名称
   使用名称作为占位符不用依照参数的顺序来设置参数，并且使用HQL具有更好的可读性。如：
   Query query = session.createQuery("from Guestbook where id<:begin");
   query.setInteger("begin",12);
   使用名称作为参数占位符时在HQL语句中使用“：”后跟参数名，之后即可在setXXX()方法中直接根据参数名称来设置参数值。
if(condition.get("vendorName")!=null && !condition.get("vendorName").equals(""))
 {
where += " and v.NAME like :vendorName";}
if(condition.get("vendorName")!=null&& !condition.get("vendorName").equals("") ) {
q.setParameter("vendorName", "%"+condition.get("vendorName")+"%");
}
String bhql = "delete from Biding where biddingId=:id";
Query bq = session.createQuery(bhql);
bq.setParameter("id", id);
