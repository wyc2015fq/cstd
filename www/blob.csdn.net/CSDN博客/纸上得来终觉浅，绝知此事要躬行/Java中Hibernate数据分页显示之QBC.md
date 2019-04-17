# Java中Hibernate数据分页显示之QBC - 纸上得来终觉浅，绝知此事要躬行 - CSDN博客





2011年08月31日 17:27:21[boonya](https://me.csdn.net/boonya)阅读数：2548








**1、使用HIbernate自带工具类（HibernateSessionFactory）**



```java
public List<Note> getAllNoteByPageBean(int index,int pageSize){
     Session session=HibernateSessionFactory.getSession();
     Criteria criteria=session.createCriteria(Note.class);
     criteria.setFirstResult((index-1)*pageSize);
     criteria.setMaxResults(pageSize);
     return  criteria.list();
 }
```



**2、调用的时候可以动态的设置firstResult和maxResults属性**



```java
criteria.setFirstResult((index-1)*pageSize);//在原来的基础上变化，剔除页面数据
  criteria.setMaxResults(pageSize);               //设置每页显示条数
```



**3、获取到数据源，到JSP页面上展现**



```
<c:if test="${pageBean.pageIndex>1}">
             <a href="note.do?method=page&nid=${pageBean.pageIndex-1}">上一页</a>
            </c:if>
            <c:forEach varStatus="vs" begin="1"  end="${pageBean.totalPages}" >
                     <a href="note.do?method=page&nid=${vs.index}"> ${vs.index}</a> 
                    </c:forEach>
            <c:if test="${pageBean.pageIndex <pageBean.totalPages}">
             <a href="note.do?method=page&nid=${pageBean.pageIndex+1}">下一页</a>
 </c:if>
```








