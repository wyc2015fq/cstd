# getHibernateTemplate().execute(new HibernateCallback())方法 - z69183787的专栏 - CSDN博客
2013年12月27日 16:22:40[OkidoGreen](https://me.csdn.net/z69183787)阅读数：4142
1. HibernateCallback是一个接口，该接口只有一个方法doInHibernate(org.hibernate.Session session)，该方法只有一个参数
 Session。
2. 方法 doInHibernate 的方法体就是 Spring执行的持久化操作。
3. 使用HibernateTemplate执行execute(new HibernateCallback())方法，从HibernateCallback中得到session,在此session中做多个操作，并希望这些操作位于同一个事务中。
4. 回调实际就是一种事件触发模式，就象连环地雷一样，一旦触发一个，这个再触发另外一个，你在find这个方法被执行时，希望同时执行其他方法，就需要回调。
例如：
          return (List<E>) getHibernateTemplate().execute(
                new HibernateCallback() {
                        public Object doInHibernate(Session session)
                              throws HibernateException {
                                          Criteria criteria = session.createCriteria(entityClass);
                                          criteria.setFirstResult(pageSize * (pageIndex));
                                          criteria.setMaxResults(pageSize);
                                          return criteria.list();
                               }
    }); 
如例子代码中，他希望在执行execute()时也执行HibernateCallback 中的一些方法。
5. HibernateTemplate提供这个HibernateCallback，
          就是为了满足这种使用了HibernateTemplate的情况下，
          仍然需要直接访问Session的需求而来的
          它提供了在HibernateTemplate里面直接访问Session的能力
          这个就是我们要使用HibernateCallback的原因.
