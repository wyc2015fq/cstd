# hibernate查询%_等特殊字符 - z69183787的专栏 - CSDN博客
2013年10月16日 17:05:11[OkidoGreen](https://me.csdn.net/z69183787)阅读数：5889
在sql中%,_等为特殊字符，有时候我们需要查询这些特殊字符，这个时候就需要转义了。下面这两句话就是进行转义。
select username from gg_user where username like '%xiao/_%' escape '/';   
select username from gg_user where username like '%xiao/%%' escape '/'; 
第一句SQL的意思是/后的_不再作为特殊字符使用，第二句SQL的意思是/后的%不再作为特殊字符使用
我列出自己的一段在hibernate中的代码：
public List search(String username,String key,int page,int max) {
  // TODO Auto-generated method stub
  Session session = HibernateSessionFactory.getSession();
String hql="from Content c where c.username in(select t.attenuser from Attention t where t.username=?) and  c.text like ? escape
 '/' order by c.time desc";
  Query q=session.createQuery(hql);
  q.setString(0, username);
 key=key.replaceAll("%","/%");
  key=key.replaceAll("_","/_");
  q.setString(1, "%"+key+"%");
  q.setMaxResults(max);
  q.setFirstResult((page-1)*max);
  List list=q.list();
  session.close();
  return list;
 }
