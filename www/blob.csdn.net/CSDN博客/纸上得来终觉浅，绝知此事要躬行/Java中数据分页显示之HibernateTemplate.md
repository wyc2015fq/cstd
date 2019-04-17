# Java中数据分页显示之HibernateTemplate - 纸上得来终觉浅，绝知此事要躬行 - CSDN博客





2011年08月31日 17:50:25[boonya](https://me.csdn.net/boonya)阅读数：1092标签：[java																[object																[null																[string																[query																[session](https://so.csdn.net/so/search/s.do?q=session&t=blog)
个人分类：[Java](https://blog.csdn.net/boonya/article/category/878447)





1、使用SSH框架（配置环境）；

 2、编写实现的基础类BaseDao:

package com.accp.jboa.dao;

import java.io.Serializable;

import java.sql.SQLException;

import java.util.Collection;

import java.util.List;

import org.hibernate.HibernateException;

import org.hibernate.Query;

import org.hibernate.Session;

import org.springframework.orm.hibernate3.HibernateCallback;

import org.springframework.orm.hibernate3.support.HibernateDaoSupport;

import com.accp.jboa.utils.PaginationSupport;

public class BaseDao extends HibernateDaoSupport {

/**********省略部分代码*******************/

 /** 根据hql查询分页数据 */

 public <T> void getPageDatasByHql(final PaginationSupport<T> pageBean,

   final String hql) {

  this.getPageDatasByHql(pageBean, hql, null);

 }

 public <T> void getPageDatasByHql(final PaginationSupport<T> pageBean,

   final String hql, final Object[] paras) {

  super.getHibernateTemplate().execute(new HibernateCallback() {

   @SuppressWarnings("unchecked")

   public Object doInHibernate(Session ses) throws HibernateException,

     SQLException {

    Query query = ses.createQuery(hql);

    if (paras != null) {

     for (int i = 0; i < paras.length; i++) {

      query.setParameter(i, paras[i]);

     }

    }

    query.setFirstResult((pageBean.getPageIndex() - 1)

      * pageBean.getPageSize());

    query.setMaxResults(pageBean.getPageSize());

    pageBean.setPageDatas(query.list());

    return null;

   }

  });

 }

}

3、编写相应接口和实现,并继承自BaseDao；

4、具体的实现类调用实现不同的分页。](https://so.csdn.net/so/search/s.do?q=query&t=blog)](https://so.csdn.net/so/search/s.do?q=string&t=blog)](https://so.csdn.net/so/search/s.do?q=null&t=blog)](https://so.csdn.net/so/search/s.do?q=object&t=blog)](https://so.csdn.net/so/search/s.do?q=java&t=blog)




